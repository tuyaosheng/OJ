import hashlib
import json
import os
# import shutil
import tempfile
import zipfile
from wsgiref.util import FileWrapper

VIDEO_UPLOAD_DIR = None  # 在首次使用时从 settings 读取
ALLOWED_VIDEO_TYPES = {"video/mp4", "video/webm", "video/ogg"}
MAX_VIDEO_SIZE = 500 * 1024 * 1024  # 500MB

from django.conf import settings
from django.db import transaction
from django.db.models import Q
from django.http import StreamingHttpResponse, FileResponse

from account.decorators import problem_permission_required, ensure_created_by
from contest.models import Contest, ContestStatus
from fps.parser import FPSHelper, FPSParser
from judge.dispatcher import SPJCompiler
from options.options import SysOptions
from submission.models import Submission, JudgeStatus
from utils.api import APIView, CSRFExemptAPIView, validate_serializer, APIError
from utils.constants import Difficulty
from utils.shortcuts import rand_str, natural_sort_key
from utils.tasks import delete_files
from django.utils import timezone
from ..models import Problem, ProblemRuleType, ProblemTag, Chapter, ChapterProblem, ClassSession, StudentActivity, IPBinding
from ..serializers import (CreateContestProblemSerializer, CompileSPJSerializer,
                           CreateProblemSerializer, EditProblemSerializer, EditContestProblemSerializer,
                           ProblemAdminSerializer, TestCaseUploadForm, ContestProblemMakePublicSerializer,
                           AddContestProblemSerializer, ExportProblemSerializer,
                           ExportProblemRequestSerialzier, UploadProblemForm, ImportProblemSerializer,
                           FPSProblemSerializer, CreateOrEditChapterSerializer, EditChapterSerializer,
                           ChapterSerializer, ChapterDetailSerializer,
                           AddChapterProblemSerializer, BatchAddChapterProblemSerializer,
                           ChapterProblemOrderSerializer,
                           ClassSessionSerializer, CreateOrEditClassSessionSerializer, EditClassSessionSerializer,
                           IPBindingSerializer, CreateOrEditIPBindingSerializer, EditIPBindingSerializer)
from ..utils import TEMPLATE_BASE, build_problem_template


class TestCaseZipProcessor(object):
    def process_zip(self, uploaded_zip_file, spj, dir=""):
        try:
            zip_file = zipfile.ZipFile(uploaded_zip_file, "r")
        except zipfile.BadZipFile:
            raise APIError("Bad zip file")
        name_list = zip_file.namelist()
        test_case_list = self.filter_name_list(name_list, spj=spj, dir=dir)
        if not test_case_list:
            raise APIError("Empty file")

        test_case_id = rand_str()
        test_case_dir = os.path.join(settings.TEST_CASE_DIR, test_case_id)
        os.mkdir(test_case_dir)
        os.chmod(test_case_dir, 0o710)

        size_cache = {}
        md5_cache = {}

        for item in test_case_list:
            with open(os.path.join(test_case_dir, item), "wb") as f:
                content = zip_file.read(f"{dir}{item}").replace(b"\r\n", b"\n")
                size_cache[item] = len(content)
                if item.endswith(".out"):
                    md5_cache[item] = hashlib.md5(content.rstrip()).hexdigest()
                f.write(content)
        test_case_info = {"spj": spj, "test_cases": {}}

        info = []

        if spj:
            for index, item in enumerate(test_case_list):
                data = {"input_name": item, "input_size": size_cache[item]}
                info.append(data)
                test_case_info["test_cases"][str(index + 1)] = data
        else:
            # ["1.in", "1.out", "2.in", "2.out"] => [("1.in", "1.out"), ("2.in", "2.out")]
            test_case_list = zip(*[test_case_list[i::2] for i in range(2)])
            for index, item in enumerate(test_case_list):
                data = {"stripped_output_md5": md5_cache[item[1]],
                        "input_size": size_cache[item[0]],
                        "output_size": size_cache[item[1]],
                        "input_name": item[0],
                        "output_name": item[1]}
                info.append(data)
                test_case_info["test_cases"][str(index + 1)] = data

        with open(os.path.join(test_case_dir, "info"), "w", encoding="utf-8") as f:
            f.write(json.dumps(test_case_info, indent=4))

        for item in os.listdir(test_case_dir):
            os.chmod(os.path.join(test_case_dir, item), 0o640)

        return info, test_case_id

    def filter_name_list(self, name_list, spj, dir=""):
        ret = []
        prefix = 1
        if spj:
            while True:
                in_name = f"{prefix}.in"
                if f"{dir}{in_name}" in name_list:
                    ret.append(in_name)
                    prefix += 1
                    continue
                else:
                    return sorted(ret, key=natural_sort_key)
        else:
            while True:
                in_name = f"{prefix}.in"
                out_name = f"{prefix}.out"
                if f"{dir}{in_name}" in name_list and f"{dir}{out_name}" in name_list:
                    ret.append(in_name)
                    ret.append(out_name)
                    prefix += 1
                    continue
                else:
                    return sorted(ret, key=natural_sort_key)


class TestCaseAPI(CSRFExemptAPIView, TestCaseZipProcessor):
    request_parsers = ()

    def get(self, request):
        problem_id = request.GET.get("problem_id")
        if not problem_id:
            return self.error("Parameter error, problem_id is required")
        try:
            problem = Problem.objects.get(id=problem_id)
        except Problem.DoesNotExist:
            return self.error("Problem does not exists")

        if problem.contest:
            ensure_created_by(problem.contest, request.user)
        else:
            ensure_created_by(problem, request.user)

        test_case_dir = os.path.join(settings.TEST_CASE_DIR, problem.test_case_id)
        if not os.path.isdir(test_case_dir):
            return self.error("Test case does not exists")
        name_list = self.filter_name_list(os.listdir(test_case_dir), problem.spj)
        name_list.append("info")
        file_name = os.path.join(test_case_dir, problem.test_case_id + ".zip")
        with zipfile.ZipFile(file_name, "w") as file:
            for test_case in name_list:
                file.write(f"{test_case_dir}/{test_case}", test_case)
        response = StreamingHttpResponse(FileWrapper(open(file_name, "rb")),
                                         content_type="application/octet-stream")

        response["Content-Disposition"] = f"attachment; filename=problem_{problem.id}_test_cases.zip"
        response["Content-Length"] = os.path.getsize(file_name)
        return response

    def post(self, request):
        form = TestCaseUploadForm(request.POST, request.FILES)
        if form.is_valid():
            spj = form.cleaned_data["spj"] == "true"
            file = form.cleaned_data["file"]
        else:
            return self.error("Upload failed")
        zip_file = f"/tmp/{rand_str()}.zip"
        with open(zip_file, "wb") as f:
            for chunk in file:
                f.write(chunk)
        info, test_case_id = self.process_zip(zip_file, spj=spj)
        os.remove(zip_file)
        return self.success({"id": test_case_id, "info": info, "spj": spj})


class CompileSPJAPI(APIView):
    @validate_serializer(CompileSPJSerializer)
    def post(self, request):
        data = request.data
        spj_version = rand_str(8)
        error = SPJCompiler(data["spj_code"], spj_version, data["spj_language"]).compile_spj()
        if error:
            return self.error(error)
        else:
            return self.success()


class ProblemBase(APIView):
    def common_checks(self, request):
        data = request.data
        if data["spj"]:
            if not data["spj_language"] or not data["spj_code"]:
                return "Invalid spj"
            if not data["spj_compile_ok"]:
                return "SPJ code must be compiled successfully"
            data["spj_version"] = hashlib.md5(
                (data["spj_language"] + ":" + data["spj_code"]).encode("utf-8")).hexdigest()
        else:
            data["spj_language"] = None
            data["spj_code"] = None
        if data["rule_type"] == ProblemRuleType.OI:
            total_score = 0
            for item in data["test_case_score"]:
                if item["score"] <= 0:
                    return "Invalid score"
                else:
                    total_score += item["score"]
            data["total_score"] = total_score
        data["languages"] = list(data["languages"])


class ProblemAPI(ProblemBase):
    @problem_permission_required
    @validate_serializer(CreateProblemSerializer)
    def post(self, request):
        data = request.data
        _id = data["_id"]
        if not _id:
            return self.error("Display ID is required")
        if Problem.objects.filter(_id=_id, contest_id__isnull=True).exists():
            return self.error("Display ID already exists")

        error_info = self.common_checks(request)
        if error_info:
            return self.error(error_info)

        # todo check filename and score info
        tags = data.pop("tags")
        data["created_by"] = request.user
        problem = Problem.objects.create(**data)

        for item in tags:
            try:
                tag = ProblemTag.objects.get(name=item)
            except ProblemTag.DoesNotExist:
                tag = ProblemTag.objects.create(name=item)
            problem.tags.add(tag)
        return self.success(ProblemAdminSerializer(problem).data)

    @problem_permission_required
    def get(self, request):
        problem_id = request.GET.get("id")
        rule_type = request.GET.get("rule_type")
        user = request.user
        if problem_id:
            try:
                problem = Problem.objects.get(id=problem_id)
                ensure_created_by(problem, request.user)
                return self.success(ProblemAdminSerializer(problem).data)
            except Problem.DoesNotExist:
                return self.error("Problem does not exist")

        problems = Problem.objects.filter(contest_id__isnull=True).order_by("-create_time")
        if rule_type:
            if rule_type not in ProblemRuleType.choices():
                return self.error("Invalid rule_type")
            else:
                problems = problems.filter(rule_type=rule_type)

        keyword = request.GET.get("keyword", "").strip()
        if keyword:
            problems = problems.filter(Q(title__icontains=keyword) | Q(_id__icontains=keyword))
        if not user.can_mgmt_all_problem():
            problems = problems.filter(created_by=user)
        return self.success(self.paginate_data(request, problems, ProblemAdminSerializer))

    @problem_permission_required
    @validate_serializer(EditProblemSerializer)
    def put(self, request):
        data = request.data
        problem_id = data.pop("id")

        try:
            problem = Problem.objects.get(id=problem_id)
            ensure_created_by(problem, request.user)
        except Problem.DoesNotExist:
            return self.error("Problem does not exist")

        _id = data["_id"]
        if not _id:
            return self.error("Display ID is required")
        if Problem.objects.exclude(id=problem_id).filter(_id=_id, contest_id__isnull=True).exists():
            return self.error("Display ID already exists")

        error_info = self.common_checks(request)
        if error_info:
            return self.error(error_info)
        # todo check filename and score info
        tags = data.pop("tags")
        data["languages"] = list(data["languages"])

        for k, v in data.items():
            setattr(problem, k, v)
        problem.save()

        problem.tags.remove(*problem.tags.all())
        for tag in tags:
            try:
                tag = ProblemTag.objects.get(name=tag)
            except ProblemTag.DoesNotExist:
                tag = ProblemTag.objects.create(name=tag)
            problem.tags.add(tag)

        return self.success()

    @problem_permission_required
    def delete(self, request):
        id = request.GET.get("id")
        if not id:
            return self.error("Invalid parameter, id is required")
        try:
            problem = Problem.objects.get(id=id, contest_id__isnull=True)
        except Problem.DoesNotExist:
            return self.error("Problem does not exists")
        ensure_created_by(problem, request.user)
        # d = os.path.join(settings.TEST_CASE_DIR, problem.test_case_id)
        # if os.path.isdir(d):
        #     shutil.rmtree(d, ignore_errors=True)
        problem.delete()
        return self.success()


class ContestProblemAPI(ProblemBase):
    @validate_serializer(CreateContestProblemSerializer)
    def post(self, request):
        data = request.data
        try:
            contest = Contest.objects.get(id=data.pop("contest_id"))
            ensure_created_by(contest, request.user)
        except Contest.DoesNotExist:
            return self.error("Contest does not exist")

        if data["rule_type"] != contest.rule_type:
            return self.error("Invalid rule type")

        _id = data["_id"]
        if not _id:
            return self.error("Display ID is required")

        if Problem.objects.filter(_id=_id, contest=contest).exists():
            return self.error("Duplicate Display id")

        error_info = self.common_checks(request)
        if error_info:
            return self.error(error_info)

        # todo check filename and score info
        data["contest"] = contest
        tags = data.pop("tags")
        data["created_by"] = request.user
        problem = Problem.objects.create(**data)

        for item in tags:
            try:
                tag = ProblemTag.objects.get(name=item)
            except ProblemTag.DoesNotExist:
                tag = ProblemTag.objects.create(name=item)
            problem.tags.add(tag)
        return self.success(ProblemAdminSerializer(problem).data)

    def get(self, request):
        problem_id = request.GET.get("id")
        contest_id = request.GET.get("contest_id")
        user = request.user
        if problem_id:
            try:
                problem = Problem.objects.get(id=problem_id)
                ensure_created_by(problem.contest, user)
            except Problem.DoesNotExist:
                return self.error("Problem does not exist")
            return self.success(ProblemAdminSerializer(problem).data)

        if not contest_id:
            return self.error("Contest id is required")
        try:
            contest = Contest.objects.get(id=contest_id)
            ensure_created_by(contest, user)
        except Contest.DoesNotExist:
            return self.error("Contest does not exist")
        problems = Problem.objects.filter(contest=contest).order_by("-create_time")
        if user.is_admin():
            problems = problems.filter(contest__created_by=user)
        keyword = request.GET.get("keyword")
        if keyword:
            problems = problems.filter(title__contains=keyword)
        return self.success(self.paginate_data(request, problems, ProblemAdminSerializer))

    @validate_serializer(EditContestProblemSerializer)
    def put(self, request):
        data = request.data
        user = request.user

        try:
            contest = Contest.objects.get(id=data.pop("contest_id"))
            ensure_created_by(contest, user)
        except Contest.DoesNotExist:
            return self.error("Contest does not exist")

        if data["rule_type"] != contest.rule_type:
            return self.error("Invalid rule type")

        problem_id = data.pop("id")

        try:
            problem = Problem.objects.get(id=problem_id, contest=contest)
        except Problem.DoesNotExist:
            return self.error("Problem does not exist")

        _id = data["_id"]
        if not _id:
            return self.error("Display ID is required")
        if Problem.objects.exclude(id=problem_id).filter(_id=_id, contest=contest).exists():
            return self.error("Display ID already exists")

        error_info = self.common_checks(request)
        if error_info:
            return self.error(error_info)
        # todo check filename and score info
        tags = data.pop("tags")
        data["languages"] = list(data["languages"])

        for k, v in data.items():
            setattr(problem, k, v)
        problem.save()

        problem.tags.remove(*problem.tags.all())
        for tag in tags:
            try:
                tag = ProblemTag.objects.get(name=tag)
            except ProblemTag.DoesNotExist:
                tag = ProblemTag.objects.create(name=tag)
            problem.tags.add(tag)
        return self.success()

    def delete(self, request):
        id = request.GET.get("id")
        if not id:
            return self.error("Invalid parameter, id is required")
        try:
            problem = Problem.objects.get(id=id, contest_id__isnull=False)
        except Problem.DoesNotExist:
            return self.error("Problem does not exists")
        ensure_created_by(problem.contest, request.user)
        if Submission.objects.filter(problem=problem).exists():
            return self.error("Can't delete the problem as it has submissions")
        # d = os.path.join(settings.TEST_CASE_DIR, problem.test_case_id)
        # if os.path.isdir(d):
        #    shutil.rmtree(d, ignore_errors=True)
        problem.delete()
        return self.success()


class MakeContestProblemPublicAPIView(APIView):
    @validate_serializer(ContestProblemMakePublicSerializer)
    @problem_permission_required
    def post(self, request):
        data = request.data
        display_id = data.get("display_id")
        if Problem.objects.filter(_id=display_id, contest_id__isnull=True).exists():
            return self.error("Duplicate display ID")

        try:
            problem = Problem.objects.get(id=data["id"])
        except Problem.DoesNotExist:
            return self.error("Problem does not exist")

        if not problem.contest or problem.is_public:
            return self.error("Already be a public problem")
        problem.is_public = True
        problem.save()
        # https://docs.djangoproject.com/en/1.11/topics/db/queries/#copying-model-instances
        tags = problem.tags.all()
        problem.pk = None
        problem.contest = None
        problem._id = display_id
        problem.visible = False
        problem.submission_number = problem.accepted_number = 0
        problem.statistic_info = {}
        problem.save()
        problem.tags.set(tags)
        return self.success()


class AddContestProblemAPI(APIView):
    @validate_serializer(AddContestProblemSerializer)
    def post(self, request):
        data = request.data
        try:
            contest = Contest.objects.get(id=data["contest_id"])
            problem = Problem.objects.get(id=data["problem_id"])
        except (Contest.DoesNotExist, Problem.DoesNotExist):
            return self.error("Contest or Problem does not exist")

        if contest.status == ContestStatus.CONTEST_ENDED:
            return self.error("Contest has ended")
        if Problem.objects.filter(contest=contest, _id=data["display_id"]).exists():
            return self.error("Duplicate display id in this contest")

        tags = problem.tags.all()
        problem.pk = None
        problem.contest = contest
        problem.is_public = True
        problem.visible = True
        problem._id = request.data["display_id"]
        problem.submission_number = problem.accepted_number = 0
        problem.statistic_info = {}
        problem.save()
        problem.tags.set(tags)
        return self.success()


class ExportProblemAPI(APIView):
    def choose_answers(self, user, problem):
        ret = []
        for item in problem.languages:
            submission = Submission.objects.filter(problem=problem,
                                                   user_id=user.id,
                                                   language=item,
                                                   result=JudgeStatus.ACCEPTED).order_by("-create_time").first()
            if submission:
                ret.append({"language": submission.language, "code": submission.code})
        return ret

    def process_one_problem(self, zip_file, user, problem, index):
        info = ExportProblemSerializer(problem).data
        info["answers"] = self.choose_answers(user, problem=problem)
        compression = zipfile.ZIP_DEFLATED
        zip_file.writestr(zinfo_or_arcname=f"{index}/problem.json",
                          data=json.dumps(info, indent=4),
                          compress_type=compression)
        problem_test_case_dir = os.path.join(settings.TEST_CASE_DIR, problem.test_case_id)
        with open(os.path.join(problem_test_case_dir, "info")) as f:
            info = json.load(f)
        for k, v in info["test_cases"].items():
            zip_file.write(filename=os.path.join(problem_test_case_dir, v["input_name"]),
                           arcname=f"{index}/testcase/{v['input_name']}",
                           compress_type=compression)
            if not info["spj"]:
                zip_file.write(filename=os.path.join(problem_test_case_dir, v["output_name"]),
                               arcname=f"{index}/testcase/{v['output_name']}",
                               compress_type=compression)

    @validate_serializer(ExportProblemRequestSerialzier)
    def get(self, request):
        problems = Problem.objects.filter(id__in=request.data["problem_id"])
        for problem in problems:
            if problem.contest:
                ensure_created_by(problem.contest, request.user)
            else:
                ensure_created_by(problem, request.user)
        path = f"/tmp/{rand_str()}.zip"
        with zipfile.ZipFile(path, "w") as zip_file:
            for index, problem in enumerate(problems):
                self.process_one_problem(zip_file=zip_file, user=request.user, problem=problem, index=index + 1)
        delete_files.send_with_options(args=(path,), delay=300_000)
        resp = FileResponse(open(path, "rb"))
        resp["Content-Type"] = "application/zip"
        resp["Content-Disposition"] = "attachment;filename=problem-export.zip"
        return resp


class ImportProblemAPI(CSRFExemptAPIView, TestCaseZipProcessor):
    request_parsers = ()

    def post(self, request):
        form = UploadProblemForm(request.POST, request.FILES)
        if form.is_valid():
            file = form.cleaned_data["file"]
            tmp_file = f"/tmp/{rand_str()}.zip"
            with open(tmp_file, "wb") as f:
                for chunk in file:
                    f.write(chunk)
        else:
            return self.error("Upload failed")

        count = 0
        with zipfile.ZipFile(tmp_file, "r") as zip_file:
            name_list = zip_file.namelist()
            for item in name_list:
                if "/problem.json" in item:
                    count += 1
            with transaction.atomic():
                for i in range(1, count + 1):
                    with zip_file.open(f"{i}/problem.json") as f:
                        problem_info = json.load(f)
                        serializer = ImportProblemSerializer(data=problem_info)
                        if not serializer.is_valid():
                            return self.error(f"Invalid problem format, error is {serializer.errors}")
                        else:
                            problem_info = serializer.data
                            for item in problem_info["template"].keys():
                                if item not in SysOptions.language_names:
                                    return self.error(f"Unsupported language {item}")

                        problem_info["display_id"] = problem_info["display_id"][:24]
                        for k, v in problem_info["template"].items():
                            problem_info["template"][k] = build_problem_template(v["prepend"], v["template"],
                                                                                 v["append"])

                        spj = problem_info["spj"] is not None
                        rule_type = problem_info["rule_type"]
                        test_case_score = problem_info["test_case_score"]

                        # process test case
                        _, test_case_id = self.process_zip(tmp_file, spj=spj, dir=f"{i}/testcase/")

                        problem_obj = Problem.objects.create(_id=problem_info["display_id"],
                                                             title=problem_info["title"],
                                                             description=problem_info["description"]["value"],
                                                             input_description=problem_info["input_description"][
                                                                 "value"],
                                                             output_description=problem_info["output_description"][
                                                                 "value"],
                                                             hint=problem_info["hint"]["value"],
                                                             test_case_score=test_case_score if test_case_score else [],
                                                             time_limit=problem_info["time_limit"],
                                                             memory_limit=problem_info["memory_limit"],
                                                             samples=problem_info["samples"],
                                                             template=problem_info["template"],
                                                             rule_type=problem_info["rule_type"],
                                                             source=problem_info["source"],
                                                             spj=spj,
                                                             spj_code=problem_info["spj"]["code"] if spj else None,
                                                             spj_language=problem_info["spj"][
                                                                 "language"] if spj else None,
                                                             spj_version=rand_str(8) if spj else "",
                                                             languages=SysOptions.language_names,
                                                             created_by=request.user,
                                                             visible=False,
                                                             difficulty=Difficulty.MID,
                                                             total_score=sum(item["score"] for item in test_case_score)
                                                             if rule_type == ProblemRuleType.OI else 0,
                                                             test_case_id=test_case_id
                                                             )
                        for tag_name in problem_info["tags"]:
                            tag_obj, _ = ProblemTag.objects.get_or_create(name=tag_name)
                            problem_obj.tags.add(tag_obj)
        return self.success({"import_count": count})


class ChapterAdminAPI(APIView):
    @problem_permission_required
    @validate_serializer(CreateOrEditChapterSerializer)
    def post(self, request):
        data = request.data
        chapter = Chapter.objects.create(
            title=data["title"],
            description=data.get("description", ""),
            order=data.get("order", 0),
            created_by=request.user
        )
        return self.success(ChapterSerializer(chapter).data)

    @problem_permission_required
    def get(self, request):
        chapter_id = request.GET.get("id")
        if chapter_id:
            try:
                chapter = Chapter.objects.get(id=chapter_id)
                return self.success(ChapterDetailSerializer(chapter).data)
            except Chapter.DoesNotExist:
                return self.error("Chapter does not exist")
        chapters = Chapter.objects.all()
        return self.success(ChapterSerializer(chapters, many=True).data)

    @problem_permission_required
    @validate_serializer(EditChapterSerializer)
    def put(self, request):
        data = request.data
        try:
            chapter = Chapter.objects.get(id=data["id"])
        except Chapter.DoesNotExist:
            return self.error("Chapter does not exist")
        chapter.title = data["title"]
        chapter.description = data.get("description", chapter.description)
        chapter.order = data.get("order", chapter.order)
        chapter.save()
        return self.success(ChapterSerializer(chapter).data)

    @problem_permission_required
    def delete(self, request):
        chapter_id = request.GET.get("id")
        if not chapter_id:
            return self.error("Parameter id is required")
        try:
            Chapter.objects.get(id=chapter_id).delete()
        except Chapter.DoesNotExist:
            return self.error("Chapter does not exist")
        return self.success()


class ChapterProblemAdminAPI(APIView):
    @problem_permission_required
    @validate_serializer(AddChapterProblemSerializer)
    def post(self, request):
        data = request.data
        try:
            chapter = Chapter.objects.get(id=data["chapter_id"])
            problem = Problem.objects.get(id=data["problem_id"], contest_id__isnull=True)
        except Chapter.DoesNotExist:
            return self.error("Chapter does not exist")
        except Problem.DoesNotExist:
            return self.error("Problem does not exist")
        if ChapterProblem.objects.filter(chapter=chapter, problem=problem).exists():
            return self.error("Problem already in this chapter")
        max_order = chapter.chapter_problems.count()
        ChapterProblem.objects.create(chapter=chapter, problem=problem, order=max_order)
        return self.success(ChapterDetailSerializer(chapter).data)

    @problem_permission_required
    def delete(self, request):
        chapter_id = request.GET.get("chapter_id")
        problem_id = request.GET.get("problem_id")
        if not chapter_id or not problem_id:
            return self.error("chapter_id and problem_id are required")
        ChapterProblem.objects.filter(chapter_id=chapter_id, problem_id=problem_id).delete()
        return self.success()

    @problem_permission_required
    @validate_serializer(ChapterProblemOrderSerializer)
    def put(self, request):
        data = request.data
        try:
            chapter = Chapter.objects.get(id=data["chapter_id"])
        except Chapter.DoesNotExist:
            return self.error("Chapter does not exist")
        problem_ids = data["problem_ids"]
        for idx, pid in enumerate(problem_ids):
            ChapterProblem.objects.filter(chapter=chapter, problem_id=pid).update(order=idx)
        return self.success()


class BatchAddChapterProblemAPI(APIView):
    @problem_permission_required
    @validate_serializer(BatchAddChapterProblemSerializer)
    def post(self, request):
        data = request.data
        try:
            chapter = Chapter.objects.get(id=data["chapter_id"])
        except Chapter.DoesNotExist:
            return self.error("Chapter does not exist")
        problem_ids = data["problem_ids"]
        problems = Problem.objects.filter(id__in=problem_ids, contest_id__isnull=True)
        existing = set(ChapterProblem.objects.filter(
            chapter=chapter, problem_id__in=problem_ids
        ).values_list("problem_id", flat=True))
        base_order = chapter.chapter_problems.count()
        new_entries = []
        for i, problem in enumerate(problems):
            if problem.id not in existing:
                new_entries.append(ChapterProblem(
                    chapter=chapter, problem=problem, order=base_order + i
                ))
        ChapterProblem.objects.bulk_create(new_entries)
        return self.success({
            "added": len(new_entries),
            "skipped": len(problem_ids) - len(new_entries)
        })


class ProblemVideoAPI(CSRFExemptAPIView):
    """上传或删除题目题解视频，仅管理员可操作，video 字段非必填"""
    request_parsers = ()

    def _video_dir(self):
        video_dir = os.path.join(settings.UPLOAD_DIR, "videos")
        os.makedirs(video_dir, exist_ok=True)
        return video_dir

    def _video_url(self, filename):
        return f"{settings.UPLOAD_PREFIX}/videos/{filename}"

    @problem_permission_required
    def post(self, request):
        problem_id = request.POST.get("problem_id")
        if not problem_id:
            return self.error("problem_id is required")
        try:
            problem = Problem.objects.get(id=problem_id)
            ensure_created_by(problem, request.user)
        except Problem.DoesNotExist:
            return self.error("Problem does not exist")

        video_file = request.FILES.get("file")
        if not video_file:
            return self.error("No file uploaded")
        if video_file.content_type not in ALLOWED_VIDEO_TYPES:
            return self.error("Only mp4/webm/ogg video formats are supported")
        if video_file.size > MAX_VIDEO_SIZE:
            return self.error("Video file size must be under 500MB")

        # 删除旧视频
        if problem.video:
            old_path = os.path.join(settings.DATA_DIR, problem.video.lstrip("/"))
            if os.path.isfile(old_path):
                os.remove(old_path)

        ext = os.path.splitext(video_file.name)[1] or ".mp4"
        filename = f"problem_{problem_id}_{rand_str(8)}{ext}"
        save_path = os.path.join(self._video_dir(), filename)
        with open(save_path, "wb") as f:
            for chunk in video_file.chunks(8192):
                f.write(chunk)

        problem.video = self._video_url(filename)
        problem.save(update_fields=["video"])
        return self.success({"video": problem.video})

    @problem_permission_required
    def delete(self, request):
        problem_id = request.GET.get("problem_id")
        if not problem_id:
            return self.error("problem_id is required")
        try:
            problem = Problem.objects.get(id=problem_id)
            ensure_created_by(problem, request.user)
        except Problem.DoesNotExist:
            return self.error("Problem does not exist")

        if problem.video:
            old_path = os.path.join(settings.DATA_DIR, problem.video.lstrip("/"))
            if os.path.isfile(old_path):
                os.remove(old_path)
            problem.video = None
            problem.save(update_fields=["video"])
        return self.success()


class ClassSessionAdminAPI(APIView):
    @problem_permission_required
    @validate_serializer(CreateOrEditClassSessionSerializer)
    def post(self, request):
        data = request.data
        try:
            chapter = Chapter.objects.get(id=data["chapter_id"])
        except Chapter.DoesNotExist:
            return self.error("Chapter does not exist")
        session = ClassSession.objects.create(
            title=data["title"],
            chapter=chapter,
            start_time=data["start_time"],
            end_time=data["end_time"],
            created_by=request.user
        )
        return self.success(ClassSessionSerializer(session).data)

    @problem_permission_required
    def get(self, request):
        session_id = request.GET.get("id")
        if session_id:
            try:
                session = ClassSession.objects.get(id=session_id)
                return self.success(ClassSessionSerializer(session).data)
            except ClassSession.DoesNotExist:
                return self.error("Session does not exist")
        sessions = ClassSession.objects.select_related("chapter").all()
        return self.success(ClassSessionSerializer(sessions, many=True).data)

    @problem_permission_required
    @validate_serializer(EditClassSessionSerializer)
    def put(self, request):
        data = request.data
        try:
            session = ClassSession.objects.get(id=data["id"])
            chapter = Chapter.objects.get(id=data["chapter_id"])
        except (ClassSession.DoesNotExist, Chapter.DoesNotExist) as e:
            return self.error(str(e))
        session.title = data["title"]
        session.chapter = chapter
        session.start_time = data["start_time"]
        session.end_time = data["end_time"]
        session.save()
        return self.success(ClassSessionSerializer(session).data)

    @problem_permission_required
    def delete(self, request):
        session_id = request.GET.get("id")
        if not session_id:
            return self.error("id is required")
        ClassSession.objects.filter(id=session_id).delete()
        return self.success()


class ClassDashboardAPI(APIView):
    """大屏进度看板数据接口"""
    @problem_permission_required
    def get(self, request):
        session_id = request.GET.get("session_id")
        if not session_id:
            return self.error("session_id is required")
        try:
            session = ClassSession.objects.select_related("chapter").get(id=session_id)
        except ClassSession.DoesNotExist:
            return self.error("Session does not exist")

        # 章节内题目
        cps = session.chapter.chapter_problems.select_related("problem").order_by("order")
        problems = [{"id": cp.problem.id, "_id": cp.problem._id, "title": cp.problem.title} for cp in cps]
        problem_ids = [p["id"] for p in problems]

        # IP绑定表
        ip_map = {b.ip_address: {"hostname": b.hostname, "note": b.note}
                  for b in IPBinding.objects.all()}

        # 学生当前活动
        activities = StudentActivity.objects.filter(session=session).select_related("user", "current_problem")
        student_map = {}
        for act in activities:
            student_map[act.user.id] = {
                "user_id": act.user.id,
                "username": act.user.username,
                "ip": act.ip_address,
                "binding": ip_map.get(act.ip_address, {}),
                "current_problem_id": act.current_problem_id,
                "current_problem_display": act.current_problem._id if act.current_problem else None,
                "updated_at": act.updated_at.isoformat(),
            }

        # 提交记录（本次课堂时间窗口内，针对章节题目）
        from submission.models import Submission, JudgeStatus
        submissions = Submission.objects.filter(
            problem_id__in=problem_ids,
            create_time__gte=session.start_time,
            create_time__lte=session.end_time
        ).values("user_id", "problem_id", "result")

        # 每个学生每道题的最好结果
        solved = {}  # {user_id: {problem_id: status}}  0=AC, else=attempted
        for sub in submissions:
            uid, pid, result = sub["user_id"], sub["problem_id"], sub["result"]
            if uid not in solved:
                solved[uid] = {}
            if pid not in solved[uid] or solved[uid][pid] != 0:
                solved[uid][pid] = 0 if result == JudgeStatus.ACCEPTED else 1

        # 合并：补充所有出现过的用户
        all_user_ids = set(student_map.keys()) | set(solved.keys())
        from account.models import User
        users_qs = User.objects.filter(id__in=all_user_ids).values("id", "username")
        for u in users_qs:
            if u["id"] not in student_map:
                student_map[u["id"]] = {
                    "user_id": u["id"],
                    "username": u["username"],
                    "ip": "",
                    "binding": {},
                    "current_problem_id": None,
                    "current_problem_display": None,
                    "updated_at": None,
                }

        students = list(student_map.values())
        for s in students:
            s["solved"] = solved.get(s["user_id"], {})

        return self.success({
            "session": ClassSessionSerializer(session).data,
            "problems": problems,
            "students": students,
        })


class IPBindingAdminAPI(APIView):
    @problem_permission_required
    @validate_serializer(CreateOrEditIPBindingSerializer)
    def post(self, request):
        data = request.data
        binding, created = IPBinding.objects.update_or_create(
            ip_address=data["ip_address"],
            defaults={"hostname": data["hostname"], "note": data.get("note", "")}
        )
        return self.success(IPBindingSerializer(binding).data)

    @problem_permission_required
    def get(self, request):
        return self.success(IPBindingSerializer(IPBinding.objects.all(), many=True).data)

    @problem_permission_required
    @validate_serializer(EditIPBindingSerializer)
    def put(self, request):
        data = request.data
        try:
            binding = IPBinding.objects.get(id=data["id"])
        except IPBinding.DoesNotExist:
            return self.error("Binding does not exist")
        binding.ip_address = data["ip_address"]
        binding.hostname = data["hostname"]
        binding.note = data.get("note", "")
        binding.save()
        return self.success(IPBindingSerializer(binding).data)

    @problem_permission_required
    def delete(self, request):
        bid = request.GET.get("id")
        IPBinding.objects.filter(id=bid).delete()
        return self.success()


class FPSProblemImport(CSRFExemptAPIView):
    request_parsers = ()

    def _create_problem(self, problem_data, creator):
        if problem_data["time_limit"]["unit"] == "ms":
            time_limit = problem_data["time_limit"]["value"]
        else:
            time_limit = problem_data["time_limit"]["value"] * 1000
        template = {}
        prepend = {}
        append = {}
        for t in problem_data["prepend"]:
            prepend[t["language"]] = t["code"]
        for t in problem_data["append"]:
            append[t["language"]] = t["code"]
        for t in problem_data["template"]:
            our_lang = lang = t["language"]
            if lang == "Python":
                our_lang = "Python3"
            template[our_lang] = TEMPLATE_BASE.format(prepend.get(lang, ""), t["code"], append.get(lang, ""))
        spj = problem_data["spj"] is not None

        # 显示ID：优先使用FPS中的<id>，否则自动生成
        display_id = problem_data.get("id") or f"fps-{rand_str(4)}"
        # 若该ID已存在则追加随机后缀避免冲突
        if Problem.objects.filter(_id=display_id, contest_id__isnull=True).exists():
            display_id = f"{display_id}-{rand_str(4)}"

        # 分数：优先使用FPS中各测试点的score属性，否则平均分配100分
        score_list = problem_data["test_case_score"]
        has_custom_score = any(item.get("score") is not None for item in score_list)
        if not has_custom_score:
            n = len(score_list)
            base = 100 // n if n else 0
            remainder = 100 - base * n if n else 0
            for i, item in enumerate(score_list):
                item["score"] = base + (1 if i < remainder else 0)
        else:
            for item in score_list:
                if item.get("score") is None:
                    item["score"] = 0

        total_score = sum(item["score"] for item in score_list)

        problem = Problem.objects.create(
            _id=display_id,
            title=problem_data["title"],
            description=problem_data["description"],
            input_description=problem_data["input"],
            output_description=problem_data["output"],
            hint=problem_data["hint"],
            test_case_score=score_list,
            time_limit=time_limit,
            memory_limit=problem_data["memory_limit"]["value"],
            samples=problem_data["samples"],
            template=template,
            rule_type=ProblemRuleType.OI,
            source=problem_data.get("source", ""),
            spj=spj,
            spj_code=problem_data["spj"]["code"] if spj else None,
            spj_language=problem_data["spj"]["language"] if spj else None,
            spj_version=rand_str(8) if spj else "",
            visible=False,
            languages=SysOptions.language_names,
            created_by=creator,
            difficulty=Difficulty.MID,
            total_score=total_score,
            test_case_id=problem_data["test_case_id"]
        )

        # 标签：优先使用FPS中的<tag>，否则默认加"C++"
        tag_names = problem_data.get("tags") or ["C++"]
        for tag_name in tag_names:
            tag_obj, _ = ProblemTag.objects.get_or_create(name=tag_name)
            problem.tags.add(tag_obj)

    def post(self, request):
        form = UploadProblemForm(request.POST, request.FILES)
        if form.is_valid():
            file = form.cleaned_data["file"]
            with tempfile.NamedTemporaryFile("wb") as tf:
                for chunk in file.chunks(4096):
                    tf.file.write(chunk)

                tf.file.flush()
                os.fsync(tf.file)

                problems = FPSParser(tf.name).parse()
        else:
            return self.error("Parse upload file error")

        helper = FPSHelper()
        with transaction.atomic():
            for _problem in problems:
                test_case_id = rand_str()
                test_case_dir = os.path.join(settings.TEST_CASE_DIR, test_case_id)
                os.mkdir(test_case_dir)
                # 保留原始test_cases中的score属性，供_create_problem分配分数使用
                original_scores = {i: tc.get("score") for i, tc in enumerate(_problem.get("test_cases", []))}
                score = []
                for idx, item in enumerate(helper.save_test_case(_problem, test_case_dir)["test_cases"].values()):
                    score.append({
                        "score": original_scores.get(idx),  # None表示未指定，后续自动分配
                        "input_name": item["input_name"],
                        "output_name": item.get("output_name")
                    })
                problem_data = helper.save_image(_problem, settings.UPLOAD_DIR, settings.UPLOAD_PREFIX)
                # 把扩展字段传给_create_problem
                problem_data["id"] = _problem.get("id")
                problem_data["tags"] = _problem.get("tags", [])
                s = FPSProblemSerializer(data=problem_data)
                if not s.is_valid():
                    return self.error(f"Parse FPS file error: {s.errors}")
                problem_data = s.data
                problem_data["test_case_id"] = test_case_id
                problem_data["test_case_score"] = score
                problem_data["id"] = _problem.get("id")
                problem_data["tags"] = _problem.get("tags", [])
                self._create_problem(problem_data, request.user)
        return self.success({"import_count": len(problems)})
