from copy import deepcopy
from datetime import timedelta
from unittest import mock

from django.utils import timezone

from contest.models import Contest, ContestRuleType
from problem.models import Problem, ProblemTag, Chapter, ChapterProblem
from utils.api.tests import APITestCase
from .models import Submission, AICodeDiagnosis

DEFAULT_CONTEST_DATA = {"title": "test contest", "description": "test description",
                        "start_time": timezone.localtime(timezone.now()),
                        "end_time": timezone.localtime(timezone.now()) + timedelta(days=1),
                        "rule_type": ContestRuleType.ACM,
                        "password": None,
                        "allowed_ip_ranges": [],
                        "visible": True, "real_time_rank": True}

DEFAULT_PROBLEM_DATA = {"_id": "A-110", "title": "test", "description": "<p>test</p>", "input_description": "test",
                        "output_description": "test", "time_limit": 1000, "memory_limit": 256, "difficulty": "Low",
                        "visible": True, "tags": ["test"], "languages": ["C", "C++", "Java", "Python2"], "template": {},
                        "samples": [{"input": "test", "output": "test"}], "spj": False, "spj_language": "C",
                        "spj_code": "", "test_case_id": "499b26290cc7994e0b497212e842ea85",
                        "test_case_score": [{"output_name": "1.out", "input_name": "1.in", "output_size": 0,
                                             "stripped_output_md5": "d41d8cd98f00b204e9800998ecf8427e",
                                             "input_size": 0, "score": 0}],
                        "rule_type": "ACM", "hint": "<p>test</p>", "source": "test"}

DEFAULT_SUBMISSION_DATA = {
    "problem_id": "1",
    "user_id": 1,
    "username": "test",
    "code": "xxxxxxxxxxxxxx",
    "result": -2,
    "info": {},
    "language": "C",
    "statistic_info": {}
}


# todo contest submission


class SubmissionPrepare(APITestCase):
    def _create_problem_and_submission(self):
        user = self.create_admin("test", "test123", login=False)
        problem_data = deepcopy(DEFAULT_PROBLEM_DATA)
        tags = problem_data.pop("tags")
        problem_data["created_by"] = user
        self.problem = Problem.objects.create(**problem_data)
        for tag in tags:
            tag = ProblemTag.objects.create(name=tag)
            self.problem.tags.add(tag)
        self.problem.save()
        self.submission_data = deepcopy(DEFAULT_SUBMISSION_DATA)
        self.submission_data["problem_id"] = self.problem.id
        self.submission = Submission.objects.create(**self.submission_data)


class SubmissionListTest(SubmissionPrepare):
    def setUp(self):
        self._create_problem_and_submission()
        self.create_user("123", "345")
        self.url = self.reverse("submission_list_api")


class SubmissionListContestVisibilityTest(SubmissionPrepare):
    """"练习&比赛"（contest）提交本人应该能在总状态/我的提交里看到，但不该混进公开的"全部"列表"""
    def setUp(self):
        self._create_problem_and_submission()
        self.user = self.create_user("stu1", "stu123")
        self.contest = Contest.objects.create(created_by=self.user, **DEFAULT_CONTEST_DATA)
        self.contest_submission_data = deepcopy(DEFAULT_SUBMISSION_DATA)
        self.contest_submission_data["problem_id"] = self.problem.id
        self.contest_submission_data["user_id"] = self.user.id
        self.contest_submission_data["username"] = self.user.username
        self.contest_submission_data["contest_id"] = self.contest.id
        self.contest_submission = Submission.objects.create(**self.contest_submission_data)
        self.url = self.reverse("submission_list_api")

    def test_myself_filter_includes_contest_submission(self):
        resp = self.client.get(self.url, data={"limit": "10", "myself": "1"})
        self.assertSuccess(resp)
        ids = [r["id"] for r in resp.data["data"]["results"]]
        self.assertIn(self.contest_submission.id, ids)

    def test_contest_submission_source_is_contest_title(self):
        resp = self.client.get(self.url, data={"limit": "10", "myself": "1"})
        self.assertSuccess(resp)
        row = next(r for r in resp.data["data"]["results"] if r["id"] == self.contest_submission.id)
        self.assertEqual(row["source"], self.contest.title)
        self.assertEqual(row["contest_id"], self.contest.id)

    def test_all_submissions_excludes_contest_submission(self):
        resp = self.client.get(self.url, data={"limit": "10"})
        self.assertSuccess(resp)
        ids = [r["id"] for r in resp.data["data"]["results"]]
        self.assertNotIn(self.contest_submission.id, ids)

    def test_get_submission_list(self):
        resp = self.client.get(self.url, data={"limit": "10"})
        self.assertSuccess(resp)


class SubmissionSourceFieldTest(SubmissionPrepare):
    """状态页"来源"列：有比赛就显示比赛名，没有比赛但在章节里就显示章节名，都没有就空着"""
    def setUp(self):
        self._create_problem_and_submission()
        self.create_user("viewer", "viewer123")
        self.url = self.reverse("submission_list_api")

    def test_source_falls_back_to_chapter_when_no_contest(self):
        admin = self.problem.created_by
        chapter = Chapter.objects.create(title="第一章 高精度计算", created_by=admin)
        ChapterProblem.objects.create(chapter=chapter, problem=self.problem)

        resp = self.client.get(self.url, data={"limit": "10"})
        self.assertSuccess(resp)
        row = next(r for r in resp.data["data"]["results"] if r["id"] == self.submission.id)
        self.assertEqual(row["source"], chapter.title)

    def test_source_blank_when_neither_contest_nor_chapter(self):
        resp = self.client.get(self.url, data={"limit": "10"})
        self.assertSuccess(resp)
        row = next(r for r in resp.data["data"]["results"] if r["id"] == self.submission.id)
        self.assertEqual(row["source"], "")


@mock.patch("submission.views.oj.judge_task.send")
class SubmissionAPITest(SubmissionPrepare):
    def setUp(self):
        self._create_problem_and_submission()
        self.user = self.create_user("123", "test123")
        self.url = self.reverse("submission_api")

    def test_create_submission(self, judge_task):
        resp = self.client.post(self.url, self.submission_data)
        self.assertSuccess(resp)
        judge_task.assert_called()

    def test_create_submission_with_wrong_language(self, judge_task):
        self.submission_data.update({"language": "Python3"})
        resp = self.client.post(self.url, self.submission_data)
        self.assertFailed(resp)
        self.assertDictEqual(resp.data, {"error": "error",
                                         "data": "Python3 is now allowed in the problem"})
        judge_task.assert_not_called()


class SubmissionAdminDeleteAPITest(SubmissionPrepare):
    def setUp(self):
        self._create_problem_and_submission()
        self.url = self.reverse("submission_delete_api")

    def test_delete_without_login(self):
        resp = self.client.delete(self.url + "?id=" + str(self.submission.id))
        self.assertFailed(resp)
        self.assertTrue(Submission.objects.filter(id=self.submission.id).exists())

    def test_delete_as_regular_user(self):
        self.create_user("regular", "regular123")
        resp = self.client.delete(self.url + "?id=" + str(self.submission.id))
        self.assertFailed(resp)
        self.assertTrue(Submission.objects.filter(id=self.submission.id).exists())

    def test_delete_as_admin_forbidden(self):
        self.create_admin("admin2", "admin123")
        resp = self.client.delete(self.url + "?id=" + str(self.submission.id))
        self.assertFailed(resp)
        self.assertTrue(Submission.objects.filter(id=self.submission.id).exists())

    def test_delete_missing_id(self):
        self.create_super_admin()
        resp = self.client.delete(self.url)
        self.assertFailed(resp)

    def test_delete_nonexistent_submission(self):
        self.create_super_admin()
        resp = self.client.delete(self.url + "?id=nonexistent-id")
        self.assertFailed(resp)

    def test_delete_as_super_admin(self):
        self.create_super_admin()
        resp = self.client.delete(self.url + "?id=" + str(self.submission.id))
        self.assertSuccess(resp)
        self.assertFalse(Submission.objects.filter(id=self.submission.id).exists())


class AIDiagnosisListAPITest(SubmissionPrepare):
    def setUp(self):
        self._create_problem_and_submission()
        self.create_super_admin()
        AICodeDiagnosis.objects.create(
            submission=self.submission,
            problem=self.problem,
            user_id=1,
            username="test",
            submission_result=self.submission.result,
            result="some diagnosis text"
        )
        self.url = self.reverse("ai_diagnosis_list_api")

    def test_get_list(self):
        resp = self.client.get(self.url)
        self.assertSuccess(resp)
        results = resp.data["data"]["results"]
        self.assertEqual(len(results), 1)
        item = results[0]
        self.assertEqual(item["code"], self.submission_data["code"])
        self.assertEqual(item["language"], self.submission_data["language"])
        self.assertEqual(item["problem_pk"], self.problem.id)
