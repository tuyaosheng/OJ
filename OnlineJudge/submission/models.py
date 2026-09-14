from django.db import models

from utils.constants import ContestStatus
from utils.models import JSONField
from problem.models import Problem
from contest.models import Contest

from utils.shortcuts import rand_str


class JudgeStatus:
    COMPILE_ERROR = -2
    WRONG_ANSWER = -1
    ACCEPTED = 0
    CPU_TIME_LIMIT_EXCEEDED = 1
    REAL_TIME_LIMIT_EXCEEDED = 2
    MEMORY_LIMIT_EXCEEDED = 3
    RUNTIME_ERROR = 4
    SYSTEM_ERROR = 5
    PENDING = 6
    JUDGING = 7
    PARTIALLY_ACCEPTED = 8


class Submission(models.Model):
    id = models.TextField(default=rand_str, primary_key=True, db_index=True)
    contest = models.ForeignKey(Contest, null=True, on_delete=models.CASCADE)
    problem = models.ForeignKey(Problem, on_delete=models.CASCADE)
    create_time = models.DateTimeField(auto_now_add=True)
    user_id = models.IntegerField(db_index=True)
    username = models.TextField()
    code = models.TextField()
    result = models.IntegerField(db_index=True, default=JudgeStatus.PENDING)
    # 从JudgeServer返回的判题详情
    info = JSONField(default=dict)
    language = models.TextField()
    shared = models.BooleanField(default=False)
    # 存储该提交所用时间和内存值，方便提交列表显示
    # {time_cost: "", memory_cost: "", err_info: "", score: 0}
    statistic_info = JSONField(default=dict)
    ip = models.TextField(null=True)

    def check_user_permission(self, user, check_share=True):
        if self.user_id == user.id or user.is_super_admin() or user.can_mgmt_all_problem() or self.problem.created_by_id == user.id:
            return True

        if check_share:
            if self.contest and self.contest.status != ContestStatus.CONTEST_ENDED:
                return False
            if self.problem.share_submission or self.shared:
                return True
        return False

    class Meta:
        db_table = "submission"
        ordering = ("-create_time",)

    def __str__(self):
        return self.id


class AIDiagnosisStatus:
    PENDING = "pending"
    SUCCESS = "success"
    FAILED = "failed"


class AICodeDiagnosis(models.Model):
    # 一条提交只诊断一次并缓存，重复点击不再消耗次数
    submission = models.OneToOneField(Submission, on_delete=models.CASCADE, related_name="ai_diagnosis")
    problem = models.ForeignKey(Problem, on_delete=models.CASCADE)
    user_id = models.IntegerField(db_index=True)
    username = models.TextField()
    # 提交时的判题结果，便于教师端筛选/展示
    submission_result = models.IntegerField(default=JudgeStatus.WRONG_ANSWER)
    # AI 返回的诊断文本（Markdown），诊断中/失败时为空
    result = models.TextField(blank=True, default="")
    # pending: 已提交给后台任务，正在等待大模型返回；success: 已完成；failed: 调用失败（可重新发起）
    status = models.CharField(max_length=16, default=AIDiagnosisStatus.SUCCESS, db_index=True)
    error = models.TextField(null=True, blank=True)
    create_time = models.DateTimeField(auto_now_add=True)

    class Meta:
        db_table = "ai_code_diagnosis"
        ordering = ("-create_time",)
