import random
from django.db.models import Q, Count
from utils.api import APIView
from account.decorators import check_contest_permission
from django.utils import timezone
from ..models import ProblemTag, Problem, ProblemRuleType, Chapter, ClassSession, StudentActivity, IPBinding
from ..serializers import ProblemSerializer, TagSerializer, ProblemSafeSerializer, ChapterDetailSerializer, ClassSessionSerializer
from contest.models import ContestRuleType


class ProblemTagAPI(APIView):
    def get(self, request):
        qs = ProblemTag.objects
        keyword = request.GET.get("keyword")
        if keyword:
            qs = ProblemTag.objects.filter(name__icontains=keyword)
        tags = qs.annotate(problem_count=Count("problem")).filter(problem_count__gt=0)
        return self.success(TagSerializer(tags, many=True).data)


class PickOneAPI(APIView):
    def get(self, request):
        problems = Problem.objects.filter(contest_id__isnull=True, visible=True)
        count = problems.count()
        if count == 0:
            return self.error("No problem to pick")
        return self.success(problems[random.randint(0, count - 1)]._id)


class ProblemAPI(APIView):
    @staticmethod
    def _add_problem_status(request, queryset_values):
        if request.user.is_authenticated:
            profile = request.user.userprofile
            acm_problems_status = profile.acm_problems_status.get("problems", {})
            oi_problems_status = profile.oi_problems_status.get("problems", {})
            # paginate data
            results = queryset_values.get("results")
            if results is not None:
                problems = results
            else:
                problems = [queryset_values, ]
            for problem in problems:
                if problem["rule_type"] == ProblemRuleType.ACM:
                    problem["my_status"] = acm_problems_status.get(str(problem["id"]), {}).get("status")
                else:
                    problem["my_status"] = oi_problems_status.get(str(problem["id"]), {}).get("status")

    def get(self, request):
        # 问题详情页
        problem_id = request.GET.get("problem_id")
        if problem_id:
            try:
                problem = Problem.objects.select_related("created_by") \
                    .get(_id=problem_id, contest_id__isnull=True, visible=True)
                problem_data = ProblemSerializer(problem).data
                self._add_problem_status(request, problem_data)
                return self.success(problem_data)
            except Problem.DoesNotExist:
                return self.error("Problem does not exist")

        limit = request.GET.get("limit")
        if not limit:
            return self.error("Limit is needed")

        problems = Problem.objects.select_related("created_by").filter(contest_id__isnull=True, visible=True)
        # 按照标签筛选
        tag_text = request.GET.get("tag")
        if tag_text:
            problems = problems.filter(tags__name=tag_text)

        # 搜索的情况
        keyword = request.GET.get("keyword", "").strip()
        if keyword:
            problems = problems.filter(Q(title__icontains=keyword) | Q(_id__icontains=keyword))

        # 难度筛选
        difficulty = request.GET.get("difficulty")
        if difficulty:
            problems = problems.filter(difficulty=difficulty)
        # 根据profile 为做过的题目添加标记
        data = self.paginate_data(request, problems, ProblemSerializer)
        self._add_problem_status(request, data)
        return self.success(data)


class ChapterAPI(APIView):
    def get(self, request):
        chapters = Chapter.objects.prefetch_related("chapter_problems__problem__tags").all()
        data = ChapterDetailSerializer(chapters, many=True).data
        if request.user.is_authenticated:
            profile = request.user.userprofile
            acm_status = profile.acm_problems_status.get("problems", {})
            oi_status = profile.oi_problems_status.get("problems", {})
            for chapter in data:
                for p in chapter["problems"]:
                    pid = str(p["id"])
                    if p.get("rule_type") == ProblemRuleType.ACM:
                        p["my_status"] = acm_status.get(pid, {}).get("status")
                    else:
                        p["my_status"] = oi_status.get(pid, {}).get("status")
        return self.success(data)


class ActiveClassSessionAPI(APIView):
    """学生查询当前是否有进行中的课堂"""
    def get(self, request):
        now = timezone.now()
        sessions = ClassSession.objects.filter(
            start_time__lte=now, end_time__gte=now
        ).select_related("chapter").order_by("-start_time")
        return self.success(ClassSessionSerializer(sessions, many=True).data)


class StudentActivityAPI(APIView):
    """学生上报当前正在查看的题目"""
    def post(self, request):
        if not request.user.is_authenticated:
            return self.error("Login required")
        session_id = request.data.get("session_id")
        problem_id = request.data.get("problem_id")
        if not session_id:
            return self.error("session_id required")
        try:
            session = ClassSession.objects.get(id=session_id)
        except ClassSession.DoesNotExist:
            return self.error("Session not found")
        now = timezone.now()
        if not (session.start_time <= now <= session.end_time):
            return self.error("Session is not active")
        problem = None
        if problem_id:
            try:
                problem = Problem.objects.get(id=problem_id)
            except Problem.DoesNotExist:
                pass
        ip = request.META.get("HTTP_X_REAL_IP") or request.META.get("REMOTE_ADDR", "")
        StudentActivity.objects.update_or_create(
            session=session,
            user=request.user,
            defaults={
                "ip_address": ip,
                "current_problem": problem,
            }
        )
        return self.success()


class ClassProgressAPI(APIView):
    """学生专注视图：本次课堂的题目 + 各题通过人数"""
    def get(self, request):
        session_id = request.GET.get("session_id")
        if not session_id:
            return self.error("session_id required")
        try:
            session = ClassSession.objects.select_related("chapter").get(id=session_id)
        except ClassSession.DoesNotExist:
            return self.error("Session not found")

        now = timezone.now()
        if not (session.start_time <= now <= session.end_time):
            return self.error("Session is not active")

        cps = session.chapter.chapter_problems.select_related("problem").order_by("order")
        problem_ids = [cp.problem.id for cp in cps]

        from submission.models import Submission, JudgeStatus
        ac_counts = {}
        for pid in problem_ids:
            ac_counts[pid] = Submission.objects.filter(
                problem_id=pid,
                result=JudgeStatus.ACCEPTED,
                create_time__gte=session.start_time,
                create_time__lte=session.end_time
            ).values("user_id").distinct().count()

        my_status = {}
        if request.user.is_authenticated:
            profile = request.user.userprofile
            oi_status = profile.oi_problems_status.get("problems", {})
            acm_status = profile.acm_problems_status.get("problems", {})
            for cp in cps:
                pid = str(cp.problem.id)
                rt = cp.problem.rule_type
                if rt == ProblemRuleType.OI:
                    my_status[cp.problem.id] = oi_status.get(pid, {}).get("status")
                else:
                    my_status[cp.problem.id] = acm_status.get(pid, {}).get("status")

        problems = []
        for cp in cps:
            p = cp.problem
            problems.append({
                "id": p.id,
                "_id": p._id,
                "title": p.title,
                "difficulty": p.difficulty,
                "ac_count": ac_counts.get(p.id, 0),
                "my_status": my_status.get(p.id),
            })

        return self.success({
            "session": ClassSessionSerializer(session).data,
            "problems": problems,
        })


class ContestProblemAPI(APIView):
    def _add_problem_status(self, request, queryset_values):
        if request.user.is_authenticated:
            profile = request.user.userprofile
            if self.contest.rule_type == ContestRuleType.ACM:
                problems_status = profile.acm_problems_status.get("contest_problems", {})
            else:
                problems_status = profile.oi_problems_status.get("contest_problems", {})
            for problem in queryset_values:
                problem["my_status"] = problems_status.get(str(problem["id"]), {}).get("status")

    @check_contest_permission(check_type="problems")
    def get(self, request):
        problem_id = request.GET.get("problem_id")
        if problem_id:
            try:
                problem = Problem.objects.select_related("created_by").get(_id=problem_id,
                                                                           contest=self.contest,
                                                                           visible=True)
            except Problem.DoesNotExist:
                return self.error("Problem does not exist.")
            if self.contest.problem_details_permission(request.user):
                problem_data = ProblemSerializer(problem).data
                self._add_problem_status(request, [problem_data, ])
            else:
                problem_data = ProblemSafeSerializer(problem).data
            return self.success(problem_data)

        contest_problems = Problem.objects.select_related("created_by").filter(contest=self.contest, visible=True)
        if self.contest.problem_details_permission(request.user):
            data = ProblemSerializer(contest_problems, many=True).data
            self._add_problem_status(request, data)
        else:
            data = ProblemSafeSerializer(contest_problems, many=True).data
        return self.success(data)
