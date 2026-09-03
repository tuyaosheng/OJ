import json
import os

from django.conf import settings

from account.decorators import super_admin_required, admin_role_required
from judge.tasks import judge_task
from options.options import SysOptions
from utils.api import APIView
from utils.shortcuts import datetime2str
from ..models import Submission, AICodeDiagnosis


class SubmissionRejudgeAPI(APIView):
    @super_admin_required
    def get(self, request):
        id = request.GET.get("id")
        if not id:
            return self.error("Parameter error, id is required")
        try:
            submission = Submission.objects.select_related("problem").get(id=id, contest_id__isnull=True)
        except Submission.DoesNotExist:
            return self.error("Submission does not exists")
        submission.statistic_info = {}
        submission.save()

        judge_task.send(submission.id, submission.problem.id)
        return self.success()


class SubmissionAdminDeleteAPI(APIView):
    @super_admin_required
    def delete(self, request):
        id = request.GET.get("id")
        if not id:
            return self.error("Parameter error, id is required")
        try:
            submission = Submission.objects.get(id=id)
        except Submission.DoesNotExist:
            return self.error("Submission does not exists")
        submission.delete()
        return self.success()


class SubmissionTestCaseDetailAPI(APIView):
    @admin_role_required
    def get(self, request):
        submission_id = request.GET.get("submission_id")
        if not submission_id:
            return self.error("submission_id is required")
        try:
            submission = Submission.objects.select_related("problem").get(id=submission_id)
        except Submission.DoesNotExist:
            return self.error("Submission not found")

        problem = submission.problem
        test_case_dir = os.path.join(settings.TEST_CASE_DIR, problem.test_case_id)

        try:
            with open(os.path.join(test_case_dir, "info"), encoding="utf-8") as f:
                info = json.load(f)
        except Exception:
            return self.error("Test case info not found")

        MAX_LEN = 2000
        judge_data = []
        if submission.info and isinstance(submission.info, dict):
            judge_data = submission.info.get("data", [])

        result = []
        for i, tc in enumerate(info.get("test_cases", {}).values()):
            try:
                with open(os.path.join(test_case_dir, tc["input_name"]), "r", encoding="utf-8", errors="replace") as f:
                    raw = f.read(MAX_LEN + 1)
                inp = raw[:MAX_LEN]
                inp_truncated = len(raw) > MAX_LEN
            except Exception:
                inp, inp_truncated = "", False

            try:
                with open(os.path.join(test_case_dir, tc["output_name"]), "r", encoding="utf-8", errors="replace") as f:
                    raw = f.read(MAX_LEN + 1)
                exp = raw[:MAX_LEN]
                exp_truncated = len(raw) > MAX_LEN
            except Exception:
                exp, exp_truncated = "", False

            actual, act_truncated = "", False
            if i < len(judge_data):
                raw_act = judge_data[i].get("output", "") or ""
                if len(raw_act) > MAX_LEN:
                    actual = raw_act[:MAX_LEN]
                    act_truncated = True
                else:
                    actual = raw_act

            result.append({
                "index": i + 1,
                "input": inp,
                "input_truncated": inp_truncated,
                "expected_output": exp,
                "expected_truncated": exp_truncated,
                "actual_output": actual,
                "actual_truncated": act_truncated,
            })

        return self.success(result)


class AIDiagnosisConfigAPI(APIView):
    @super_admin_required
    def get(self, request):
        api_config = SysOptions.ai_api_config or {}
        return self.success({
            "enabled": SysOptions.ai_diagnosis_enabled,
            "daily_limit": SysOptions.ai_daily_limit,
            "api_base": api_config.get("api_base", ""),
            "model": api_config.get("model", ""),
            # 出于安全，api_key 只返回是否已配置
            "api_key_set": bool(api_config.get("api_key")),
            "allowed_results": SysOptions.ai_allowed_results,
        })

    @super_admin_required
    def post(self, request):
        data = request.data
        SysOptions.ai_diagnosis_enabled = bool(data.get("enabled", False))
        try:
            limit = int(data.get("daily_limit", 5))
        except (TypeError, ValueError):
            return self.error("每日次数必须是整数")
        SysOptions.ai_daily_limit = max(limit, 0)

        if "allowed_results" in data:
            allowed = data.get("allowed_results") or []
            valid = {-2, -1, 1, 2, 3, 4, 8}
            try:
                SysOptions.ai_allowed_results = [int(x) for x in allowed if int(x) in valid]
            except (TypeError, ValueError):
                return self.error("allowed_results 格式错误")

        api_config = dict(SysOptions.ai_api_config or {})
        api_config["api_base"] = (data.get("api_base") or "").strip()
        api_config["model"] = (data.get("model") or "").strip()
        # 仅在传入非空 api_key 时更新，留空表示不修改
        new_key = data.get("api_key")
        if new_key:
            api_config["api_key"] = new_key.strip()
        SysOptions.ai_api_config = api_config
        return self.success()


class AIDiagnosisListAPI(APIView):
    @admin_role_required
    def get(self, request):
        diagnoses = AICodeDiagnosis.objects.select_related("problem").all()
        username = request.GET.get("username")
        problem_keyword = request.GET.get("problem")
        if username:
            diagnoses = diagnoses.filter(username__icontains=username)
        if problem_keyword:
            diagnoses = diagnoses.filter(problem___id=problem_keyword)

        data = self.paginate_data(request, diagnoses)
        data["results"] = [{
            "id": d.id,
            "submission_id": d.submission_id,
            "username": d.username,
            "problem_id": d.problem._id,
            "problem_title": d.problem.title,
            "submission_result": d.submission_result,
            "result": d.result,
            "create_time": datetime2str(d.create_time),
        } for d in data["results"]]
        return self.success(data)
