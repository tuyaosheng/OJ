import ipaddress

import requests
from django.utils import timezone

from account.decorators import login_required, check_contest_permission
from contest.models import ContestStatus, ContestRuleType
from judge.tasks import judge_task
from options.options import SysOptions
# from judge.dispatcher import JudgeDispatcher
from problem.models import Problem, ProblemRuleType
from utils.api import APIView, validate_serializer
from utils.cache import cache
from utils.captcha import Captcha
from utils.throttling import TokenBucket
from ..models import Submission, AICodeDiagnosis, JudgeStatus
from ..serializers import (CreateSubmissionSerializer, SubmissionModelSerializer,
                           ShareSubmissionSerializer)
from ..serializers import SubmissionSafeModelSerializer, SubmissionListSerializer


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

        submissions = Submission.objects.filter(contest_id__isnull=True).select_related("problem__created_by")
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
            submissions = submissions.filter(user_id=request.user.id)
        elif username:
            submissions = submissions.filter(username__icontains=username)
        if result:
            submissions = submissions.filter(result=result)
        data = self.paginate_data(request, submissions)
        data["results"] = SubmissionListSerializer(data["results"], many=True, user=request.user).data
        return self.success(data)


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


# 可被诊断的（非 AC 的已判完）结果
AI_DIAGNOSABLE_RESULTS = {
    JudgeStatus.COMPILE_ERROR,
    JudgeStatus.WRONG_ANSWER,
    JudgeStatus.CPU_TIME_LIMIT_EXCEEDED,
    JudgeStatus.REAL_TIME_LIMIT_EXCEEDED,
    JudgeStatus.MEMORY_LIMIT_EXCEEDED,
    JudgeStatus.RUNTIME_ERROR,
    JudgeStatus.PARTIALLY_ACCEPTED,
}

RESULT_TEXT = {
    JudgeStatus.COMPILE_ERROR: "编译错误(Compile Error)",
    JudgeStatus.WRONG_ANSWER: "答案错误(Wrong Answer)",
    JudgeStatus.CPU_TIME_LIMIT_EXCEEDED: "运行超时(Time Limit Exceeded)",
    JudgeStatus.REAL_TIME_LIMIT_EXCEEDED: "运行超时(Time Limit Exceeded)",
    JudgeStatus.MEMORY_LIMIT_EXCEEDED: "内存超限(Memory Limit Exceeded)",
    JudgeStatus.RUNTIME_ERROR: "运行错误(Runtime Error)",
    JudgeStatus.PARTIALLY_ACCEPTED: "部分正确(Partially Accepted)",
}


def build_diagnosis_prompt(submission):
    problem = submission.problem
    result_text = RESULT_TEXT.get(submission.result, "未通过")
    err_info = submission.statistic_info.get("err_info") if submission.statistic_info else None

    # 统计未通过的测试点（不泄露测试数据，仅给序号）
    failed_cases = []
    if isinstance(submission.info, dict):
        for item in submission.info.get("data", []) or []:
            if item.get("result") not in (None, JudgeStatus.ACCEPTED):
                failed_cases.append(str(item.get("test_case", "?")))

    parts = [
        "# 题目",
        f"标题：{problem.title}",
        "",
        "题目描述：",
        problem.description or "",
    ]
    if problem.input_description:
        parts += ["", "输入描述：", problem.input_description]
    if problem.output_description:
        parts += ["", "输出描述：", problem.output_description]
    if problem.samples:
        parts.append("\n样例：")
        for i, s in enumerate(problem.samples, 1):
            parts.append(f"样例{i} 输入：\n{s.get('input', '')}\n样例{i} 输出：\n{s.get('output', '')}")

    parts += [
        "",
        "# 学生提交",
        f"编程语言：{submission.language}",
        f"判题结果：{result_text}",
    ]
    if failed_cases:
        parts.append(f"未通过的测试点编号：{', '.join(failed_cases)}")
    if err_info:
        parts += ["", "错误/编译信息：", str(err_info)[:2000]]
    parts += [
        "",
        "学生代码：",
        f"```\n{submission.code}\n```",
    ]
    return "\n".join(parts)


AI_SYSTEM_PROMPT = (
    "你是一名编程教学助教，面向高校学生。学生提交的代码未通过判题，"
    "请基于题目、判题结果和学生代码，用中文分析错误原因并给出修改方向。要求：\n"
    "1. 明确指出最可能的 bug 所在（逻辑错误、边界、溢出、初始化、复杂度等），并说明为什么；\n"
    "2. 给出修正思路或关键提示，可给出小段伪代码或关键代码行，但不要直接给出整道题的完整正确代码，保留学生独立完成的空间；\n"
    "3. 语气鼓励、简洁，用 Markdown 分点表述。"
)


def call_openai_compatible(api_config, system_prompt, user_prompt):
    api_base = (api_config.get("api_base") or "").rstrip("/")
    api_key = api_config.get("api_key") or ""
    model = api_config.get("model") or ""
    if not api_base or not api_key or not model:
        raise ValueError("AI 接口未配置完整，请联系管理员")

    url = f"{api_base}/chat/completions"
    headers = {"Authorization": f"Bearer {api_key}", "Content-Type": "application/json"}
    payload = {
        "model": model,
        "messages": [
            {"role": "system", "content": system_prompt},
            {"role": "user", "content": user_prompt},
        ],
        "temperature": 0.3,
        "stream": False,
    }
    resp = requests.post(url, json=payload, headers=headers, timeout=60)
    resp.raise_for_status()
    data = resp.json()
    return data["choices"][0]["message"]["content"]


class AICodeDiagnosisAPI(APIView):
    @login_required
    def get(self, request):
        """返回本人某条提交已有的诊断结果 + 剩余可用次数"""
        result = None
        submission_id = request.GET.get("submission_id")
        if submission_id:
            diagnosis = AICodeDiagnosis.objects.filter(submission_id=submission_id,
                                                       user_id=request.user.id).first()
            if diagnosis:
                result = diagnosis.result
        limit = SysOptions.ai_daily_limit
        used = AICodeDiagnosis.objects.filter(user_id=request.user.id,
                                              create_time__date=timezone.localdate()).count()
        return self.success({
            "enabled": SysOptions.ai_diagnosis_enabled,
            "result": result,
            "daily_limit": limit,
            "remaining": max(limit - used, 0),
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

        # 已诊断过：直接返回缓存，不消耗次数
        existing = AICodeDiagnosis.objects.filter(submission_id=submission.id).first()
        if existing:
            return self.success({"result": existing.result, "cached": True})

        # 限流：管理员不限，学生按天计数
        limit = SysOptions.ai_daily_limit
        if not request.user.is_admin_role():
            used = AICodeDiagnosis.objects.filter(user_id=request.user.id,
                                                  create_time__date=timezone.localdate()).count()
            if used >= limit:
                return self.error(f"今日 AI 诊断次数已用完（上限 {limit} 次）")

        try:
            content = call_openai_compatible(SysOptions.ai_api_config,
                                             AI_SYSTEM_PROMPT,
                                             build_diagnosis_prompt(submission))
        except ValueError as e:
            return self.error(str(e))
        except requests.RequestException:
            return self.error("AI 服务调用失败，请稍后重试或联系管理员")
        except (KeyError, IndexError):
            return self.error("AI 服务返回异常，请联系管理员检查接口配置")

        diagnosis = AICodeDiagnosis.objects.create(submission=submission,
                                                   problem=submission.problem,
                                                   user_id=submission.user_id,
                                                   username=submission.username,
                                                   submission_result=submission.result,
                                                   result=content)
        remaining = limit
        if not request.user.is_admin_role():
            used = AICodeDiagnosis.objects.filter(user_id=request.user.id,
                                                  create_time__date=timezone.localdate()).count()
            remaining = max(limit - used, 0)
        return self.success({"result": diagnosis.result, "cached": False, "remaining": remaining})
