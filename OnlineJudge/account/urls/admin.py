from django.conf.urls import url

from ..views.admin import UserAdminAPI, GenerateUserAPI, CreateTeacherAPI, BatchUpgradeGradeAPI

urlpatterns = [
    url(r"^user/?$", UserAdminAPI.as_view(), name="user_admin_api"),
    url(r"^generate_user/?$", GenerateUserAPI.as_view(), name="generate_user_api"),
    url(r"^create_teacher/?$", CreateTeacherAPI.as_view(), name="create_teacher_api"),
    url(r"^batch_upgrade_grade/?$", BatchUpgradeGradeAPI.as_view(), name="batch_upgrade_grade_api"),
]
