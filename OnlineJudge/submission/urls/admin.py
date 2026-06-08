from django.conf.urls import url

from ..views.admin import SubmissionRejudgeAPI, SubmissionTestCaseDetailAPI

urlpatterns = [
    url(r"^submission/rejudge?$", SubmissionRejudgeAPI.as_view(), name="submission_rejudge_api"),
    url(r"^submission/test_case_detail/?$", SubmissionTestCaseDetailAPI.as_view(), name="submission_test_case_detail_api"),
]
