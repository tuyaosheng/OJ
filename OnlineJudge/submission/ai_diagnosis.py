import requests
from django.utils import timezone

from .models import AIDiagnosisStatus, JudgeStatus

# 后台任务硬超时是 120s（见 tasks.py 的 time_limit），超过这个再加点余量还是 pending，
# 基本可以判定任务已经卡死或 worker 异常退出，不然学生会永远卡在"诊断中"、连重试的入口都没有
AI_DIAGNOSIS_STALE_SECONDS = 150


def resolve_stale_diagnosis(diagnosis):
    """pending 记录若明显超过任务应有的最长耗时，自动判失败，让学生/管理员能重新发起而不用死等"""
    if diagnosis.status == AIDiagnosisStatus.PENDING:
        elapsed = (timezone.now() - diagnosis.create_time).total_seconds()
        if elapsed > AI_DIAGNOSIS_STALE_SECONDS:
            diagnosis.status = AIDiagnosisStatus.FAILED
            diagnosis.error = "AI 服务响应超时，请重新尝试"
            diagnosis.save(update_fields=["status", "error"])
    return diagnosis


# 可被诊断的（非 AC 的已判完）结果
AI_DIAGNOSABLE_RESULTS = {
    JudgeStatus.COMPILE_ERROR,
    JudgeStatus.WRONG_ANSWER,
    JudgeStatus.CPU_TIME_LIMIT_EXCEEDED,
    JudgeStatus.REAL_TIME_LIMIT_EXCEEDED,
    JudgeStatus.MEMORY_LIMIT_EXCEEDED,
    JudgeStatus.RUNTIME_ERROR,
    JudgeStatus.PARTIALLY_ACCEPTED,
}

RESULT_TEXT = {
    JudgeStatus.COMPILE_ERROR: "编译错误(Compile Error)",
    JudgeStatus.WRONG_ANSWER: "答案错误(Wrong Answer)",
    JudgeStatus.CPU_TIME_LIMIT_EXCEEDED: "运行超时(Time Limit Exceeded)",
    JudgeStatus.REAL_TIME_LIMIT_EXCEEDED: "运行超时(Time Limit Exceeded)",
    JudgeStatus.MEMORY_LIMIT_EXCEEDED: "内存超限(Memory Limit Exceeded)",
    JudgeStatus.RUNTIME_ERROR: "运行错误(Runtime Error)",
    JudgeStatus.PARTIALLY_ACCEPTED: "部分正确(Partially Accepted)",
}


def build_diagnosis_prompt(submission):
    problem = submission.problem
    result_text = RESULT_TEXT.get(submission.result, "未通过")
    err_info = submission.statistic_info.get("err_info") if submission.statistic_info else None

    # 统计未通过的测试点（不泄露测试数据，仅给序号）
    failed_cases = []
    if isinstance(submission.info, dict):
        for item in submission.info.get("data", []) or []:
            if item.get("result") not in (None, JudgeStatus.ACCEPTED):
                failed_cases.append(str(item.get("test_case", "?")))

    parts = [
        "# 题目",
        f"标题：{problem.title}",
        "",
        "题目描述：",
        problem.description or "",
    ]
    if problem.input_description:
        parts += ["", "输入描述：", problem.input_description]
    if problem.output_description:
        parts += ["", "输出描述：", problem.output_description]
    if problem.samples:
        parts.append("\n样例：")
        for i, s in enumerate(problem.samples, 1):
            parts.append(f"样例{i} 输入：\n{s.get('input', '')}\n样例{i} 输出：\n{s.get('output', '')}")

    parts += [
        "",
        "# 学生提交",
        f"编程语言：{submission.language}",
        f"判题结果：{result_text}",
    ]
    if failed_cases:
        parts.append(f"未通过的测试点编号：{', '.join(failed_cases)}")
    if err_info:
        parts += ["", "错误/编译信息：", str(err_info)[:2000]]
    parts += [
        "",
        "学生代码：",
        f"```\n{submission.code}\n```",
    ]
    return "\n".join(parts)


AI_SYSTEM_PROMPT = (
    "你是一名编程教学助教，面向高校学生。学生提交的代码未通过判题，"
    "请基于题目、判题结果和学生代码，用中文分析错误原因并给出修改方向。要求：\n"
    "1. 明确指出最可能的 bug 所在（逻辑错误、边界、溢出、初始化、复杂度等），并说明为什么；\n"
    "2. 给出修正思路或关键提示，可给出小段伪代码或关键代码行，但不要直接给出整道题的完整正确代码，保留学生独立完成的空间；\n"
    "3. 语气鼓励、简洁，用 Markdown 分点表述。"
)


def call_openai_compatible(api_config, system_prompt, user_prompt):
    api_base = (api_config.get("api_base") or "").rstrip("/")
    api_key = api_config.get("api_key") or ""
    model = api_config.get("model") or ""
    if not api_base or not api_key or not model:
        raise ValueError("AI 接口未配置完整，请联系管理员")

    url = f"{api_base}/chat/completions"
    headers = {"Authorization": f"Bearer {api_key}", "Content-Type": "application/json"}
    payload = {
        "model": model,
        "messages": [
            {"role": "system", "content": system_prompt},
            {"role": "user", "content": user_prompt},
        ],
        "temperature": 0.3,
        "stream": False,
    }
    resp = requests.post(url, json=payload, headers=headers, timeout=60)
    resp.raise_for_status()
    data = resp.json()
    return data["choices"][0]["message"]["content"]
