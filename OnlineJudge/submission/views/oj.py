import ipaddress

from django.db import IntegrityError
from django.utils import timezone

from account.decorators import login_required, check_contest_permission
from contest.models import ContestStatus, ContestRuleType
from judge.tasks import judge_task
from options.options import SysOptions
# from judge.dispatcher import JudgeDispatcher
from problem.models import Problem, ProblemRuleType, ChapterProblem
from utils.api import APIView, validate_serializer
from utils.cache import cache
from utils.captcha import Captcha
from utils.throttling import TokenBucket
from ..ai_diagnosis import AI_DIAGNOSABLE_RESULTS, resolve_stale_diagnosis
from ..models import Submission, AICodeDiagnosis, AIDiagnosisStatus, JudgeStatus
from ..serializers import (CreateSubmissionSerializer, SubmissionModelSerializer,
                           ShareSubmissionSerializer)
from ..serializers import SubmissionSafeModelSerializer, SubmissionListSerializer
from ..tasks import ai_diagnosis_task


class SubmissionAPI(APIView):
    def throttling(self, request):
        # 使用 open_api 的请求暂不做限制
        auth_method = getattr(request, "auth_method", "")
        if auth_method == "api_key":
            return
        user_bucket = TokenBucket(key=str(request.user.id),
                                  redis_conn=cache, **SysOptions.throttling["user"])
        can_consume, wait = user_bucket.consume()
        if not can_consume:
            return "Please wait %d seconds" % (int(wait))

        # ip_bucket = TokenBucket(key=request.session["ip"],
        #                         redis_conn=cache, **SysOptions.throttling["ip"])
        # can_consume, wait = ip_bucket.consume()
        # if not can_consume:
        #     return "Captcha is required"

    @check_contest_permission(check_type="problems")
    def check_contest_permission(self, request):
        contest = self.contest
        if contest.status == ContestStatus.CONTEST_ENDED:
            return self.error("The contest have ended")
        if not request.user.is_contest_admin(contest):
            user_ip = ipaddress.ip_address(request.session.get("ip"))
            if contest.allowed_ip_ranges:
                if not any(user_ip in ipaddress.ip_network(cidr, strict=False) for cidr in contest.allowed_ip_ranges):
                    return self.error("Your IP is not allowed in this contest")

    @validate_serializer(CreateSubmissionSerializer)
    @login_required
    def post(self, request):
        data = request.data
        hide_id = False
        if data.get("contest_id"):
            error = self.check_contest_permission(request)
            if error:
                return error
            contest = self.contest
            if not contest.problem_details_permission(request.user):
                hide_id = True

        if data.get("captcha"):
            if not Captcha(request).check(data["captcha"]):
                return self.error("Invalid captcha")
        error = self.throttling(request)
        if error:
            return self.error(error)

        try:
            problem = Problem.objects.get(id=data["problem_id"], contest_id=data.get("contest_id"), visible=True)
        except Problem.DoesNotExist:
            return self.error("Problem not exist")
        if data["language"] not in problem.languages:
            return self.error(f"{data['language']} is not allowed in the problem")
        submission = Submission.objects.create(user_id=request.user.id,
                                               username=request.user.username,
                                               language=data["language"],
                                               code=data["code"],
                                               problem_id=problem.id,
                                               ip=request.session["ip"],
                                               contest_id=data.get("contest_id"))
        # use this for debug
        # JudgeDispatcher(submission.id, problem.id).judge()
        judge_task.send(submission.id, problem.id)
        if hide_id:
            return self.success()
        else:
            return self.success({"submission_id": submission.id})

    @login_required
    def get(self, request):
        submission_id = request.GET.get("id")
        if not submission_id:
            return self.error("Parameter id doesn't exist")
        try:
            submission = Submission.objects.select_related("problem").get(id=submission_id)
        except Submission.DoesNotExist:
            return self.error("Submission doesn't exist")
        if not submission.check_user_permission(request.user):
            return self.error("No permission for this submission")

        if submission.problem.rule_type == ProblemRuleType.OI or request.user.is_admin_role():
            submission_data = SubmissionModelSerializer(submission).data
        else:
            submission_data = SubmissionSafeModelSerializer(submission).data
        # 是否有权限取消共享
        submission_data["can_unshare"] = submission.check_user_permission(request.user, check_share=False)
        return self.success(submission_data)

    @validate_serializer(ShareSubmissionSerializer)
    @login_required
    def put(self, request):
        """
        share submission
        """
        try:
            submission = Submission.objects.select_related("problem").get(id=request.data["id"])
        except Submission.DoesNotExist:
            return self.error("Submission doesn't exist")
        if not submission.check_user_permission(request.user, check_share=False):
            return self.error("No permission to share the submission")
        if submission.contest and submission.contest.status == ContestStatus.CONTEST_UNDERWAY:
            return self.error("Can not share submission now")
        submission.shared = request.data["shared"]
        submission.save(update_fields=["shared"])
        return self.success()


class SubmissionListAPI(APIView):
    def get(self, request):
        if not request.GET.get("limit"):
            return self.error("Limit is needed")
        if request.GET.get("contest_id"):
            return self.error("Parameter error")

        submissions = Submission.objects.select_related("problem__created_by", "contest")
        problem_id = request.GET.get("problem_id")
        myself = request.GET.get("myself")
        result = request.GET.get("result")
        username = request.GET.get("username")
        if problem_id:
            try:
                problem = Problem.objects.get(_id=problem_id, contest_id__isnull=True, visible=True)
            except Problem.DoesNotExist:
                return self.error("Problem doesn't exist")
            submissions = submissions.filter(problem=problem)
        if (myself and myself == "1") or not SysOptions.submission_list_show_all:
            # 只看自己：把"练习&比赛"（contest）里的提交也算进来，反正是本人的数据，不存在泄露问题
            submissions = submissions.filter(user_id=request.user.id)
        else:
            # 看全站：不含练习/比赛提交，避免把还在进行中的比赛/练习提交暴露给非参赛者
            submissions = submissions.filter(contest_id__isnull=True)
            if username:
                submissions = submissions.filter(username__icontains=username)
        if result:
            submissions = submissions.filter(result=result)
        data = self.paginate_data(request, submissions)
        chapter_titles = self._build_chapter_source_map(data["results"])
        data["results"] = SubmissionListSerializer(data["results"], many=True, user=request.user,
                                                    chapter_titles=chapter_titles).data
        return self.success(data)

    @staticmethod
    def _build_chapter_source_map(submissions):
        """不属于任何比赛/练习的提交，找它所在的章节标题给"来源"列用；批量查，避免逐行查询"""
        problem_ids = {s.problem_id for s in submissions if not s.contest_id}
        if not problem_ids:
            return {}
        chapter_problems = ChapterProblem.objects.filter(problem_id__in=problem_ids) \
            .select_related("chapter").order_by("chapter__order")
        titles = {}
        for cp in chapter_problems:
            titles.setdefault(cp.problem_id, cp.chapter.title)
        return titles


class ContestSubmissionListAPI(APIView):
    @check_contest_permission(check_type="submissions")
    def get(self, request):
        if not request.GET.get("limit"):
            return self.error("Limit is needed")

        contest = self.contest
        submissions = Submission.objects.filter(contest_id=contest.id).select_related("problem__created_by")
        problem_id = request.GET.get("problem_id")
        myself = request.GET.get("myself")
        result = request.GET.get("result")
        username = request.GET.get("username")
        if problem_id:
            try:
                problem = Problem.objects.get(_id=problem_id, contest_id=contest.id, visible=True)
            except Problem.DoesNotExist:
                return self.error("Problem doesn't exist")
            submissions = submissions.filter(problem=problem)

        if myself and myself == "1":
            submissions = submissions.filter(user_id=request.user.id)
        elif username:
            submissions = submissions.filter(username__icontains=username)
        if result:
            submissions = submissions.filter(result=result)

        # filter the test submissions submitted before contest start
        if contest.status != ContestStatus.CONTEST_NOT_START:
            submissions = submissions.filter(create_time__gte=contest.start_time)

        # 封榜的时候只能看到自己的提交
        if contest.rule_type == ContestRuleType.ACM:
            if not contest.real_time_rank and not request.user.is_contest_admin(contest):
                submissions = submissions.filter(user_id=request.user.id)

        data = self.paginate_data(request, submissions)
        data["results"] = SubmissionListSerializer(data["results"], many=True, user=request.user).data
        return self.success(data)


class SubmissionExistsAPI(APIView):
    def get(self, request):
        if not request.GET.get("problem_id"):
            return self.error("Parameter error, problem_id is required")
        return self.success(request.user.is_authenticated and
                            Submission.objects.filter(problem_id=request.GET["problem_id"],
                                                      user_id=request.user.id).exists())


def _ai_diagnosis_remaining(user):
    if user.is_admin_role():
        return SysOptions.ai_daily_limit
    used = AICodeDiagnosis.objects.filter(user_id=user.id,
                                          create_time__date=timezone.localdate()).count()
    return max(SysOptions.ai_daily_limit - used, 0)


class AICodeDiagnosisAPI(APIView):
    @login_required
    def get(self, request):
        """返回本人某条提交已有的诊断状态/结果 + 剩余可用次数，供前端轮询"""
        status, result, error = None, None, None
        submission_id = request.GET.get("submission_id")
        if submission_id:
            diagnosis = AICodeDiagnosis.objects.filter(submission_id=submission_id).first()
            # 本人或管理员可查看该提交的诊断状态（与 POST 缓存逻辑一致）
            if diagnosis and (diagnosis.user_id == request.user.id or request.user.is_admin_role()):
                diagnosis = resolve_stale_diagnosis(diagnosis)
                status = diagnosis.status
                if diagnosis.status == AIDiagnosisStatus.SUCCESS:
                    result = diagnosis.result
                elif diagnosis.status == AIDiagnosisStatus.FAILED:
                    error = diagnosis.error
        return self.success({
            "enabled": SysOptions.ai_diagnosis_enabled,
            "status": status,
            "result": result,
            "error": error,
            "daily_limit": SysOptions.ai_daily_limit,
            "remaining": _ai_diagnosis_remaining(request.user),
            "allowed_results": SysOptions.ai_allowed_results,
        })

    @login_required
    def post(self, request):
        if not SysOptions.ai_diagnosis_enabled:
            return self.error("AI 诊断功能未开启")

        submission_id = request.data.get("submission_id")
        if not submission_id:
            return self.error("参数错误：缺少 submission_id")

        try:
            submission = Submission.objects.select_related("problem").get(id=submission_id)
        except Submission.DoesNotExist:
            return self.error("提交记录不存在")

        # 学生只能诊断自己的提交
        if submission.user_id != request.user.id and not request.user.is_admin_role():
            return self.error("无权诊断该提交")

        if submission.result == JudgeStatus.ACCEPTED:
            return self.error("该提交已通过，无需诊断")
        if submission.result not in AI_DIAGNOSABLE_RESULTS:
            return self.error("该提交尚未判题完成，请稍后再试")
        # 管理员可配置只对部分判题状态开放诊断
        if submission.result not in set(SysOptions.ai_allowed_results or []):
            return self.error("该类型的提交未开放 AI 诊断")

        existing = AICodeDiagnosis.objects.filter(submission_id=submission.id).first()
        if existing:
            existing = resolve_stale_diagnosis(existing)
            if existing.status == AIDiagnosisStatus.SUCCESS:
                # 已诊断过：直接返回缓存，不消耗次数
                return self.success({"status": "success", "result": existing.result, "cached": True})
            if existing.status == AIDiagnosisStatus.PENDING:
                # 已经在后台跑了（比如刷新页面后重新点击）：不要重复发起，让前端继续轮询
                return self.success({"status": "pending"})
            # 上次失败：允许重新发起，删掉旧记录后走下面的新建流程
            existing.delete()

        # 限流：管理员不限，学生按天计数（pending 状态也占用当天次数，防止靠反复点击绕过限流）
        limit = SysOptions.ai_daily_limit
        if not request.user.is_admin_role():
            used = AICodeDiagnosis.objects.filter(user_id=request.user.id,
                                                  create_time__date=timezone.localdate()).count()
            if used >= limit:
                return self.error(f"今日 AI 诊断次数已用完（上限 {limit} 次）")

        try:
            diagnosis = AICodeDiagnosis.objects.create(submission=submission,
                                                       problem=submission.problem,
                                                       user_id=submission.user_id,
                                                       username=submission.username,
                                                       submission_result=submission.result,
                                                       status=AIDiagnosisStatus.PENDING)
        except IntegrityError:
            # 同一条提交被并发重复诊断（如多标签页同时点击）：另一个请求已抢先创建，让前端轮询它的结果
            return self.success({"status": "pending"})

        # 真正调用大模型的耗时操作放到后台任务里跑，接口立即返回，避免请求被代理/客户端超时掐断
        ai_diagnosis_task.send(diagnosis.id)
        return self.success({"status": "pending", "remaining": _ai_diagnosis_remaining(request.user)})
