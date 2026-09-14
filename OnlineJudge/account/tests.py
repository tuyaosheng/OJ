import time

from unittest import mock
from datetime import timedelta
from copy import deepcopy

from django.contrib import auth
from django.utils.timezone import now
from otpauth import OtpAuth

from utils.api.tests import APIClient, APITestCase
from utils.shortcuts import rand_str
from options.options import SysOptions

from .models import AdminType, ProblemPermission, User, UserIdentity, UserProfile
from utils.constants import ContestRuleType


class PermissionDecoratorTest(APITestCase):
    def setUp(self):
        self.regular_user = User.objects.create(username="regular_user")
        self.admin = User.objects.create(username="admin")
        self.super_admin = User.objects.create(username="super_admin")
        self.request = mock.MagicMock()
        self.request.user.is_authenticated = mock.MagicMock()

    def test_login_required(self):
        self.request.user.is_authenticated.return_value = False

    def test_admin_required(self):
        pass

    def test_super_admin_required(self):
        pass


class DuplicateUserCheckAPITest(APITestCase):
    def setUp(self):
        user = self.create_user("test", "test123", login=False)
        user.email = "test@test.com"
        user.save()
        self.url = self.reverse("check_username_or_email")

    def test_duplicate_username(self):
        resp = self.client.post(self.url, data={"username": "test"})
        data = resp.data["data"]
        self.assertEqual(data["username"], True)
        resp = self.client.post(self.url, data={"username": "Test"})
        self.assertEqual(resp.data["data"]["username"], True)

    def test_ok_username(self):
        resp = self.client.post(self.url, data={"username": "test1"})
        data = resp.data["data"]
        self.assertFalse(data["username"])

    def test_duplicate_email(self):
        resp = self.client.post(self.url, data={"email": "test@test.com"})
        self.assertEqual(resp.data["data"]["email"], True)
        resp = self.client.post(self.url, data={"email": "Test@Test.com"})
        self.assertTrue(resp.data["data"]["email"])

    def test_ok_email(self):
        resp = self.client.post(self.url, data={"email": "aa@test.com"})
        self.assertFalse(resp.data["data"]["email"])


class TFARequiredCheckAPITest(APITestCase):
    def setUp(self):
        self.url = self.reverse("tfa_required_check")
        self.create_user("test", "test123", login=False)

    def test_not_required_tfa(self):
        resp = self.client.post(self.url, data={"username": "test"})
        self.assertSuccess(resp)
        self.assertEqual(resp.data["data"]["result"], False)

    def test_required_tfa(self):
        user = User.objects.first()
        user.two_factor_auth = True
        user.save()
        resp = self.client.post(self.url, data={"username": "test"})
        self.assertEqual(resp.data["data"]["result"], True)


class UserLoginAPITest(APITestCase):
    def setUp(self):
        self.username = self.password = "test"
        self.user = self.create_user(username=self.username, password=self.password, login=False)
        self.login_url = self.reverse("user_login_api")

    def _set_tfa(self):
        self.user.two_factor_auth = True
        tfa_token = rand_str(32)
        self.user.tfa_token = tfa_token
        self.user.save()
        return tfa_token

    def test_login_with_correct_info(self):
        response = self.client.post(self.login_url,
                                    data={"username": self.username, "password": self.password})
        self.assertDictEqual(response.data, {"error": None, "data": "Succeeded"})

        user = auth.get_user(self.client)
        self.assertTrue(user.is_authenticated)

    def test_login_with_correct_info_upper_username(self):
        resp = self.client.post(self.login_url, data={"username": self.username.upper(), "password": self.password})
        self.assertDictEqual(resp.data, {"error": None, "data": "Succeeded"})
        user = auth.get_user(self.client)
        self.assertTrue(user.is_authenticated)

    def test_login_with_wrong_info(self):
        response = self.client.post(self.login_url,
                                    data={"username": self.username, "password": "invalid_password"})
        self.assertDictEqual(response.data, {"error": "error", "data": "Invalid username or password"})

        user = auth.get_user(self.client)
        self.assertFalse(user.is_authenticated)

    def test_tfa_login(self):
        token = self._set_tfa()
        code = OtpAuth(token).totp()
        if len(str(code)) < 6:
            code = (6 - len(str(code))) * "0" + str(code)
        response = self.client.post(self.login_url,
                                    data={"username": self.username,
                                          "password": self.password,
                                          "tfa_code": code})
        self.assertDictEqual(response.data, {"error": None, "data": "Succeeded"})

        user = auth.get_user(self.client)
        self.assertTrue(user.is_authenticated)

    def test_tfa_login_wrong_code(self):
        self._set_tfa()
        response = self.client.post(self.login_url,
                                    data={"username": self.username,
                                          "password": self.password,
                                          "tfa_code": "qqqqqq"})
        self.assertDictEqual(response.data, {"error": "error", "data": "Invalid two factor verification code"})

        user = auth.get_user(self.client)
        self.assertFalse(user.is_authenticated)

    def test_tfa_login_without_code(self):
        self._set_tfa()
        response = self.client.post(self.login_url,
                                    data={"username": self.username,
                                          "password": self.password})
        self.assertDictEqual(response.data, {"error": "error", "data": "tfa_required"})

        user = auth.get_user(self.client)
        self.assertFalse(user.is_authenticated)

    def test_user_disabled(self):
        self.user.is_disabled = True
        self.user.save()
        resp = self.client.post(self.login_url, data={"username": self.username,
                                                      "password": self.password})
        self.assertDictEqual(resp.data, {"error": "error", "data": "Your account has been disabled"})


class CaptchaTest(APITestCase):
    def _set_captcha(self, session):
        captcha = rand_str(4)
        session["_django_captcha_key"] = captcha
        session["_django_captcha_expires_time"] = int(time.time()) + 30
        session.save()
        return captcha


class UserRegisterAPITest(CaptchaTest):
    def setUp(self):
        self.client = APIClient()
        self.register_url = self.reverse("user_register_api")
        self.captcha = rand_str(4)

        self.data = {"username": "test_user", "password": "testuserpassword",
                     "grade": 1, "class_name": "计算机2301班",
                     "captcha": self._set_captcha(self.client.session)}

    def test_website_config_limit(self):
        SysOptions.allow_register = False
        resp = self.client.post(self.register_url, data=self.data)
        self.assertDictEqual(resp.data, {"error": "error", "data": "Register function has been disabled by admin"})

    def test_invalid_captcha(self):
        self.data["captcha"] = "****"
        response = self.client.post(self.register_url, data=self.data)
        self.assertDictEqual(response.data, {"error": "error", "data": "Invalid captcha"})

        self.data.pop("captcha")
        response = self.client.post(self.register_url, data=self.data)
        self.assertTrue(response.data["error"] is not None)

    def test_register_with_correct_info(self):
        response = self.client.post(self.register_url, data=self.data)
        self.assertDictEqual(response.data, {"error": None, "data": "Succeeded"})

    def test_register_sets_student_identity_grade_class(self):
        # 注册流程不采集邮箱，但应该把年级/班级存好、身份记成学生
        self.test_register_with_correct_info()
        user = User.objects.get(username="test_user")
        self.assertIsNone(user.email)
        profile = user.userprofile
        self.assertEqual(profile.identity, UserIdentity.STUDENT)
        self.assertEqual(profile.grade, 1)
        self.assertEqual(profile.class_name, "计算机2301班")

    def test_username_already_exists(self):
        self.test_register_with_correct_info()

        self.data["captcha"] = self._set_captcha(self.client.session)
        response = self.client.post(self.register_url, data=self.data)
        self.assertDictEqual(response.data, {"error": "error", "data": "Username already exists"})

    def test_race_condition_returns_friendly_error_not_500(self):
        # 并发注册：两个请求前后脚都通过了 exists() 检查，第二个落到唯一约束上，
        # 应该被 IntegrityError 兜住返回友好提示，而不是让 500 抛出来
        User.objects.create(username=self.data["username"])
        with mock.patch("account.views.oj.User.objects.filter") as mock_filter:
            mock_filter.return_value.exists.return_value = False
            response = self.client.post(self.register_url, data=self.data)
        self.assertDictEqual(response.data, {"error": "error", "data": "Username already exists"})
        self.assertEqual(User.objects.filter(username=self.data["username"]).count(), 1)


class ClassNameListAPITest(APITestCase):
    def setUp(self):
        self.url = self.reverse("class_name_list_api")

    def test_list_distinct_class_names_ordered_by_frequency(self):
        for i in range(3):
            u = self.create_user(f"s{i}", "pass123", login=False)
            u.userprofile.class_name = "计算机2301班"
            u.userprofile.save()
        u2 = self.create_user("s3", "pass123", login=False)
        u2.userprofile.class_name = "计算机2302班"
        u2.userprofile.save()
        u3 = self.create_user("s4", "pass123", login=False)
        u3.userprofile.class_name = ""
        u3.userprofile.save()

        resp = self.client.get(self.url)
        self.assertSuccess(resp)
        self.assertEqual(resp.data["data"], ["计算机2301班", "计算机2302班"])


class SessionManagementAPITest(APITestCase):
    def setUp(self):
        self.create_user("test", "test123")
        self.url = self.reverse("session_management_api")
        # launch a request to provide session data
        login_url = self.reverse("user_login_api")
        self.client.post(login_url, data={"username": "test", "password": "test123"})

    def test_get_sessions(self):
        resp = self.client.get(self.url)
        self.assertSuccess(resp)
        data = resp.data["data"]
        self.assertEqual(len(data), 1)

    # def test_delete_session_key(self):
    #     resp = self.client.delete(self.url + "?session_key=" + self.session_key)
    #     self.assertSuccess(resp)

    def test_delete_session_with_invalid_key(self):
        resp = self.client.delete(self.url + "?session_key=aaaaaaaaaa")
        self.assertDictEqual(resp.data, {"error": "error", "data": "Invalid session_key"})


class UserProfileAPITest(APITestCase):
    def setUp(self):
        self.url = self.reverse("user_profile_api")

    def test_get_profile_without_login(self):
        resp = self.client.get(self.url)
        self.assertDictEqual(resp.data, {"error": None, "data": None})

    def test_get_profile(self):
        self.create_user("test", "test123")
        resp = self.client.get(self.url)
        self.assertSuccess(resp)

    def test_update_profile(self):
        self.create_user("test", "test123")
        update_data = {"real_name": "zemal", "submission_number": 233, "language": "en-US"}
        resp = self.client.put(self.url, data=update_data)
        self.assertSuccess(resp)
        data = resp.data["data"]
        self.assertEqual(data["real_name"], "zemal")
        self.assertEqual(data["submission_number"], 0)
        self.assertEqual(data["language"], "en-US")


class TwoFactorAuthAPITest(APITestCase):
    def setUp(self):
        self.url = self.reverse("two_factor_auth_api")
        self.create_user("test", "test123")

    def _get_tfa_code(self):
        user = User.objects.first()
        code = OtpAuth(user.tfa_token).totp()
        if len(str(code)) < 6:
            code = (6 - len(str(code))) * "0" + str(code)
        return code

    def test_get_image(self):
        resp = self.client.get(self.url)
        self.assertSuccess(resp)

    def test_open_tfa_with_invalid_code(self):
        self.test_get_image()
        resp = self.client.post(self.url, data={"code": "000000"})
        self.assertDictEqual(resp.data, {"error": "error", "data": "Invalid code"})

    def test_open_tfa_with_correct_code(self):
        self.test_get_image()
        code = self._get_tfa_code()
        resp = self.client.post(self.url, data={"code": code})
        self.assertSuccess(resp)
        user = User.objects.first()
        self.assertEqual(user.two_factor_auth, True)

    def test_close_tfa_with_invalid_code(self):
        self.test_open_tfa_with_correct_code()
        resp = self.client.post(self.url, data={"code": "000000"})
        self.assertDictEqual(resp.data, {"error": "error", "data": "Invalid code"})

    def test_close_tfa_with_correct_code(self):
        self.test_open_tfa_with_correct_code()
        code = self._get_tfa_code()
        resp = self.client.put(self.url, data={"code": code})
        self.assertSuccess(resp)
        user = User.objects.first()
        self.assertEqual(user.two_factor_auth, False)


@mock.patch("account.views.oj.send_email_async.send")
class ApplyResetPasswordAPITest(CaptchaTest):
    def setUp(self):
        self.create_user("test", "test123", login=False)
        user = User.objects.first()
        user.email = "test@oj.com"
        user.save()
        self.url = self.reverse("apply_reset_password_api")
        self.data = {"email": "test@oj.com", "captcha": self._set_captcha(self.client.session)}

    def _refresh_captcha(self):
        self.data["captcha"] = self._set_captcha(self.client.session)

    def test_apply_reset_password(self, send_email_send):
        resp = self.client.post(self.url, data=self.data)
        self.assertSuccess(resp)
        send_email_send.assert_called()

    def test_apply_reset_password_twice_in_20_mins(self, send_email_send):
        self.test_apply_reset_password()
        send_email_send.reset_mock()
        self._refresh_captcha()
        resp = self.client.post(self.url, data=self.data)
        self.assertDictEqual(resp.data, {"error": "error", "data": "You can only reset password once per 20 minutes"})
        send_email_send.assert_not_called()

    def test_apply_reset_password_again_after_20_mins(self, send_email_send):
        self.test_apply_reset_password()
        user = User.objects.first()
        user.reset_password_token_expire_time = now() - timedelta(minutes=21)
        user.save()
        self._refresh_captcha()
        self.test_apply_reset_password()


class ResetPasswordAPITest(CaptchaTest):
    def setUp(self):
        self.create_user("test", "test123", login=False)
        self.url = self.reverse("reset_password_api")
        user = User.objects.first()
        user.reset_password_token = "online_judge?"
        user.reset_password_token_expire_time = now() + timedelta(minutes=20)
        user.save()
        self.data = {"token": user.reset_password_token,
                     "captcha": self._set_captcha(self.client.session),
                     "password": "test456"}

    def test_reset_password_with_correct_token(self):
        resp = self.client.post(self.url, data=self.data)
        self.assertSuccess(resp)
        self.assertTrue(self.client.login(username="test", password="test456"))

    def test_reset_password_with_invalid_token(self):
        self.data["token"] = "aaaaaaaaaaa"
        resp = self.client.post(self.url, data=self.data)
        self.assertDictEqual(resp.data, {"error": "error", "data": "Token does not exist"})

    def test_reset_password_with_expired_token(self):
        user = User.objects.first()
        user.reset_password_token_expire_time = now() - timedelta(seconds=30)
        user.save()
        resp = self.client.post(self.url, data=self.data)
        self.assertDictEqual(resp.data, {"error": "error", "data": "Token has expired"})


class UserChangeEmailAPITest(APITestCase):
    def setUp(self):
        self.url = self.reverse("user_change_email_api")
        self.user = self.create_user("test", "test123")
        self.new_mail = "test@oj.com"
        self.data = {"password": "test123", "new_email": self.new_mail}

    def test_change_email_success(self):
        resp = self.client.post(self.url, data=self.data)
        self.assertSuccess(resp)

    def test_wrong_password(self):
        self.data["password"] = "aaaa"
        resp = self.client.post(self.url, data=self.data)
        self.assertDictEqual(resp.data, {"error": "error", "data": "Wrong password"})

    def test_duplicate_email(self):
        u = self.create_user("aa", "bb", login=False)
        u.email = self.new_mail
        u.save()
        resp = self.client.post(self.url, data=self.data)
        self.assertDictEqual(resp.data, {"error": "error", "data": "The email is owned by other account"})


class UserChangePasswordAPITest(APITestCase):
    def setUp(self):
        self.url = self.reverse("user_change_password_api")

        # Create user at first
        self.username = "test_user"
        self.old_password = "testuserpassword"
        self.new_password = "new_password"
        self.user = self.create_user(username=self.username, password=self.old_password, login=False)

        self.data = {"old_password": self.old_password, "new_password": self.new_password}

    def _get_tfa_code(self):
        user = User.objects.first()
        code = OtpAuth(user.tfa_token).totp()
        if len(str(code)) < 6:
            code = (6 - len(str(code))) * "0" + str(code)
        return code

    def test_login_required(self):
        response = self.client.post(self.url, data=self.data)
        self.assertEqual(response.data, {"error": "permission-denied", "data": "Please login first"})

    def test_valid_ola_password(self):
        self.assertTrue(self.client.login(username=self.username, password=self.old_password))
        response = self.client.post(self.url, data=self.data)
        self.assertEqual(response.data, {"error": None, "data": "Succeeded"})
        self.assertTrue(self.client.login(username=self.username, password=self.new_password))

    def test_invalid_old_password(self):
        self.assertTrue(self.client.login(username=self.username, password=self.old_password))
        self.data["old_password"] = "invalid"
        response = self.client.post(self.url, data=self.data)
        self.assertEqual(response.data, {"error": "error", "data": "Invalid old password"})

    def test_tfa_code_required(self):
        self.user.two_factor_auth = True
        self.user.tfa_token = "tfa_token"
        self.user.save()
        self.assertTrue(self.client.login(username=self.username, password=self.old_password))
        self.data["tfa_code"] = rand_str(6)
        resp = self.client.post(self.url, data=self.data)
        self.assertEqual(resp.data, {"error": "error", "data": "Invalid two factor verification code"})

        self.data["tfa_code"] = self._get_tfa_code()
        resp = self.client.post(self.url, data=self.data)
        self.assertSuccess(resp)


class UserRankAPITest(APITestCase):
    def setUp(self):
        self.url = self.reverse("user_rank_api")
        self.create_user("test1", "test123", login=False)
        self.create_user("test2", "test123", login=False)
        test1 = User.objects.get(username="test1")
        profile1 = test1.userprofile
        profile1.submission_number = 10
        profile1.accepted_number = 10
        profile1.total_score = 240
        profile1.save()

        test2 = User.objects.get(username="test2")
        profile2 = test2.userprofile
        profile2.submission_number = 15
        profile2.accepted_number = 10
        profile2.total_score = 700
        profile2.save()

    def test_get_acm_rank(self):
        resp = self.client.get(self.url, data={"rule": ContestRuleType.ACM})
        self.assertSuccess(resp)
        data = resp.data["data"]["results"]
        self.assertEqual(data[0]["user"]["username"], "test1")
        self.assertEqual(data[1]["user"]["username"], "test2")

    def test_get_oi_rank(self):
        resp = self.client.get(self.url, data={"rule": ContestRuleType.OI})
        self.assertSuccess(resp)
        data = resp.data["data"]["results"]
        self.assertEqual(data[0]["user"]["username"], "test2")
        self.assertEqual(data[1]["user"]["username"], "test1")

    def test_admin_role_filted(self):
        self.create_admin("admin", "admin123")
        admin = User.objects.get(username="admin")
        profile1 = admin.userprofile
        profile1.submission_number = 20
        profile1.accepted_number = 5
        profile1.total_score = 300
        profile1.save()
        resp = self.client.get(self.url, data={"rule": ContestRuleType.ACM})
        self.assertSuccess(resp)
        self.assertEqual(len(resp.data["data"]), 2)

        resp = self.client.get(self.url, data={"rule": ContestRuleType.OI})
        self.assertSuccess(resp)
        self.assertEqual(len(resp.data["data"]), 2)


class ProfileProblemDisplayIDRefreshAPITest(APITestCase):
    def setUp(self):
        pass


class AdminUserTest(APITestCase):
    def setUp(self):
        self.user = self.create_super_admin(login=True)
        self.username = self.password = "test"
        self.regular_user = self.create_user(username=self.username, password=self.password, login=False)
        self.url = self.reverse("user_admin_api")
        self.data = {"id": self.regular_user.id, "username": self.username, "real_name": "test_name",
                     "email": "test@qq.com", "admin_type": AdminType.REGULAR_USER,
                     "problem_permission": ProblemPermission.OWN, "open_api": True,
                     "two_factor_auth": False, "is_disabled": False}

    def test_user_list(self):
        response = self.client.get(self.url)
        self.assertSuccess(response)

    def test_edit_user_successfully(self):
        response = self.client.put(self.url, data=self.data)
        self.assertSuccess(response)
        resp_data = response.data["data"]
        self.assertEqual(resp_data["username"], self.username)
        self.assertEqual(resp_data["email"], "test@qq.com")
        self.assertEqual(resp_data["open_api"], True)
        self.assertEqual(resp_data["two_factor_auth"], False)
        self.assertEqual(resp_data["is_disabled"], False)
        self.assertEqual(resp_data["problem_permission"], ProblemPermission.NONE)

        self.assertTrue(self.regular_user.check_password("test"))

    def test_edit_user_password(self):
        data = self.data
        new_password = "testpassword"
        data["password"] = new_password
        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)
        user = User.objects.get(id=self.regular_user.id)
        self.assertFalse(user.check_password(self.password))
        self.assertTrue(user.check_password(new_password))

    def test_edit_user_tfa(self):
        data = self.data
        self.assertIsNone(self.regular_user.tfa_token)
        data["two_factor_auth"] = True
        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)
        resp_data = response.data["data"]
        # if `tfa_token` is None, a new value will be generated
        self.assertTrue(resp_data["two_factor_auth"])
        token = User.objects.get(id=self.regular_user.id).tfa_token
        self.assertIsNotNone(token)

        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)
        resp_data = response.data["data"]
        # if `tfa_token` is not None, the value is not changed
        self.assertTrue(resp_data["two_factor_auth"])
        self.assertEqual(User.objects.get(id=self.regular_user.id).tfa_token, token)

    def test_edit_user_openapi(self):
        data = self.data
        self.assertIsNone(self.regular_user.open_api_appkey)
        data["open_api"] = True
        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)
        resp_data = response.data["data"]
        # if `open_api_appkey` is None, a new value will be generated
        self.assertTrue(resp_data["open_api"])
        key = User.objects.get(id=self.regular_user.id).open_api_appkey
        self.assertIsNotNone(key)

        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)
        resp_data = response.data["data"]
        # if `openapi_app_key` is not None, the value is not changed
        self.assertTrue(resp_data["open_api"])
        self.assertEqual(User.objects.get(id=self.regular_user.id).open_api_appkey, key)

    def test_import_users(self):
        data = {"users": [["user1", "pass1", "eami1@e.com", "user1"],
                          ["user2", "pass3", "eamil3@e.com", "user2"]]
                }
        resp = self.client.post(self.url, data)
        self.assertSuccess(resp)
        # successfully created 2 users
        self.assertEqual(User.objects.all().count(), 4)

    def test_import_duplicate_user(self):
        data = {"users": [["user1", "pass1", "eami1@e.com", "user1"],
                          ["user1", "pass1", "eami1@e.com", "user1"]]
                }
        resp = self.client.post(self.url, data)
        self.assertFailed(resp, "DETAIL:  Key (username)=(user1) already exists.")
        # no user is created
        self.assertEqual(User.objects.all().count(), 2)

    def test_delete_users(self):
        self.test_import_users()
        user_ids = User.objects.filter(username__in=["user1", "user2"]).values_list("id", flat=True)
        user_ids = ",".join([str(id) for id in user_ids])
        resp = self.client.delete(self.url + "?id=" + user_ids)
        self.assertSuccess(resp)
        self.assertEqual(User.objects.all().count(), 2)

    def test_import_users_sets_student_identity(self):
        # 批量导入的账号应该被视为学生，这样才会出现在身份筛选/批量升年级里
        self.test_import_users()
        user = User.objects.get(username="user1")
        self.assertEqual(user.userprofile.identity, UserIdentity.STUDENT)

    def test_edit_user_without_email(self):
        # 新注册流程不收集邮箱，管理员编辑这些账号时 email 必须允许为空
        data = dict(self.data)
        data["email"] = None
        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)
        self.assertIsNone(User.objects.get(id=self.regular_user.id).email)

    def test_edit_two_users_with_blank_email_no_false_conflict(self):
        # 两个都没填邮箱的账号不应该互相报"邮箱已存在"
        data = dict(self.data)
        data["email"] = None
        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)

        other = self.create_user(username="other_user", password="test123", login=False)
        data2 = dict(self.data)
        data2["id"] = other.id
        data2["username"] = "other_user"
        data2["email"] = None
        response2 = self.client.put(self.url, data=data2)
        self.assertSuccess(response2)

    def test_edit_user_identity_grade_class_name(self):
        # 管理员应该能在编辑弹窗里修正学生的年级/班级/身份
        data = dict(self.data)
        data["identity"] = UserIdentity.STUDENT
        data["grade"] = 3
        data["class_name"] = "计算机2301班"
        response = self.client.put(self.url, data=data)
        self.assertSuccess(response)
        profile = UserProfile.objects.get(user_id=self.regular_user.id)
        self.assertEqual(profile.identity, UserIdentity.STUDENT)
        self.assertEqual(profile.grade, 3)
        self.assertEqual(profile.class_name, "计算机2301班")


class GenerateUserAPITest(APITestCase):
    def setUp(self):
        self.create_super_admin()
        self.url = self.reverse("generate_user_api")
        self.data = {
            "number_from": 100, "number_to": 105,
            "prefix": "pre", "suffix": "suf",
            "default_email": "test@test.com",
            "password_length": 8
        }

    def test_error_case(self):
        data = deepcopy(self.data)
        data["prefix"] = "t" * 16
        data["suffix"] = "s" * 14
        resp = self.client.post(self.url, data=data)
        self.assertEqual(resp.data["data"], "Username should not more than 32 characters")

        data2 = deepcopy(self.data)
        data2["number_from"] = 106
        resp = self.client.post(self.url, data=data2)
        self.assertEqual(resp.data["data"], "Start number must be lower than end number")

    @mock.patch("account.views.admin.xlsxwriter.Workbook")
    def test_generate_user_success(self, mock_workbook):
        resp = self.client.post(self.url, data=self.data)
        self.assertSuccess(resp)
        mock_workbook.assert_called()

    @mock.patch("account.views.admin.xlsxwriter.Workbook")
    def test_generate_user_sets_student_identity(self, mock_workbook):
        # 批量生成的考试机账号应该被视为学生，这样才会出现在身份筛选/批量升年级里
        resp = self.client.post(self.url, data=self.data)
        self.assertSuccess(resp)
        user = User.objects.get(username="pre100suf")
        self.assertEqual(user.userprofile.identity, UserIdentity.STUDENT)


class CreateTeacherAPITest(APITestCase):
    def setUp(self):
        self.create_super_admin()
        self.url = self.reverse("create_teacher_api")
        self.data = {"username": "teacher1", "password": "test123", "real_name": "teacher_name"}

    def test_create_teacher_gets_admin_role(self):
        # 教师账号必须拿到后台访问权限，否则打不开 AI 诊断记录/章节管理等 admin_role_required 接口
        resp = self.client.post(self.url, data=self.data)
        self.assertSuccess(resp)
        user = User.objects.get(username="teacher1")
        self.assertTrue(user.is_admin_role())
        self.assertEqual(user.userprofile.identity, UserIdentity.TEACHER)


class OpenAPIAppkeyAPITest(APITestCase):
    def setUp(self):
        self.user = self.create_super_admin()
        self.url = self.reverse("open_api_appkey_api")

    def test_reset_appkey(self):
        resp = self.client.post(self.url, data={})
        self.assertFailed(resp)

        self.user.open_api = True
        self.user.save()
        resp = self.client.post(self.url, data={})
        self.assertSuccess(resp)
        self.assertEqual(resp.data["data"]["appkey"], User.objects.get(username=self.user.username).open_api_appkey)
