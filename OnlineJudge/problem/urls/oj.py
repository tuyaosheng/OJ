from django.conf.urls import url

from ..views.oj import ProblemTagAPI, ProblemAPI, ContestProblemAPI, PickOneAPI, ChapterAPI, \
    ActiveClassSessionAPI, StudentActivityAPI, ClassProgressAPI

urlpatterns = [
    url(r"^problem/tags/?$", ProblemTagAPI.as_view(), name="problem_tag_list_api"),
    url(r"^problem/?$", ProblemAPI.as_view(), name="problem_api"),
    url(r"^pickone/?$", PickOneAPI.as_view(), name="pick_one_api"),
    url(r"^chapters/?$", ChapterAPI.as_view(), name="chapter_api"),
    url(r"^class_session/active/?$", ActiveClassSessionAPI.as_view(), name="active_class_session_api"),
    url(r"^class_session/activity/?$", StudentActivityAPI.as_view(), name="student_activity_api"),
    url(r"^class_session/progress/?$", ClassProgressAPI.as_view(), name="class_progress_api"),
    url(r"^contest/problem/?$", ContestProblemAPI.as_view(), name="contest_problem_api"),
]
