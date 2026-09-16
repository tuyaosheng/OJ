from .models import Submission
from utils.api import serializers
from utils.serializers import LanguageNameChoiceField


class CreateSubmissionSerializer(serializers.Serializer):
    problem_id = serializers.IntegerField()
    language = LanguageNameChoiceField()
    code = serializers.CharField(max_length=1024 * 1024)
    contest_id = serializers.IntegerField(required=False)
    captcha = serializers.CharField(required=False)


class ShareSubmissionSerializer(serializers.Serializer):
    id = serializers.CharField()
    shared = serializers.BooleanField()


class SubmissionModelSerializer(serializers.ModelSerializer):

    class Meta:
        model = Submission
        fields = "__all__"


# 不显示submission info的serializer, 用于ACM rule_type
class SubmissionSafeModelSerializer(serializers.ModelSerializer):
    problem = serializers.SlugRelatedField(read_only=True, slug_field="_id")

    class Meta:
        model = Submission
        exclude = ("info", "contest", "ip")


class SubmissionListSerializer(serializers.ModelSerializer):
    problem = serializers.SlugRelatedField(read_only=True, slug_field="_id")
    show_link = serializers.SerializerMethodField()
    # 供前端点击"题目"列跳转：比赛提交要跳去 contest-problem-details，不能当成全局题目处理
    contest_id = serializers.IntegerField(read_only=True)
    source = serializers.SerializerMethodField()

    def __init__(self, *args, **kwargs):
        self.user = kwargs.pop("user", None)
        # {problem_id: chapter_title}，只有不属于任何比赛/练习的提交才会用到，由调用方批量查好传进来，避免逐行查询
        self.chapter_titles = kwargs.pop("chapter_titles", None) or {}
        super().__init__(*args, **kwargs)

    class Meta:
        model = Submission
        exclude = ("info", "contest", "code", "ip")

    def get_show_link(self, obj):
        # 没传user或为匿名user
        if self.user is None or not self.user.is_authenticated:
            return False
        return obj.check_user_permission(self.user)

    def get_source(self, obj):
        # 来自哪个"练习&比赛"；没有的话看是否挂在某个章节下；都没有就留空
        if obj.contest_id:
            return obj.contest.title if obj.contest else ""
        return self.chapter_titles.get(obj.problem_id, "")
