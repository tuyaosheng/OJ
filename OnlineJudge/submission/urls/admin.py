from django.conf.urls import url

from ..views.admin import (SubmissionRejudgeAPI, SubmissionTestCaseDetailAPI,
                           AIDiagnosisConfigAPI, AIDiagnosisListAPI)

urlpatterns = [
    url(r"^submission/rejudge?$", SubmissionRejudgeAPI.as_view(), name="submission_rejudge_api"),
    url(r"^submission/test_case_detail/?$", SubmissionTestCaseDetailAPI.as_view(), name="submission_test_case_detail_api"),
    url(r"^ai_diagnosis/config/?$", AIDiagnosisConfigAPI.as_view(), name="ai_diagnosis_config_api"),
    url(r"^ai_diagnosis/list/?$", AIDiagnosisListAPI.as_view(), name="ai_diagnosis_list_api"),
]
