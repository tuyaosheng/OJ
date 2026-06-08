import json
import os

from django.conf import settings

from account.decorators import super_admin_required, admin_role_required
from judge.tasks import judge_task
from utils.api import APIView
from ..models import Submission


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
