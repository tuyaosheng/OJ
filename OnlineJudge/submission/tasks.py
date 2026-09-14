import logging

import dramatiq

from options.options import SysOptions
from utils.shortcuts import DRAMATIQ_WORKER_ARGS
from .ai_diagnosis import AI_SYSTEM_PROMPT, build_diagnosis_prompt, call_openai_compatible
from .models import AICodeDiagnosis, AIDiagnosisStatus

logger = logging.getLogger(__name__)


@dramatiq.actor(**DRAMATIQ_WORKER_ARGS(max_retries=0, time_limit=120_000))
def ai_diagnosis_task(diagnosis_id):
    try:
        diagnosis = AICodeDiagnosis.objects.select_related("submission", "submission__problem").get(id=diagnosis_id)
    except AICodeDiagnosis.DoesNotExist:
        return

    try:
        content = call_openai_compatible(SysOptions.ai_api_config,
                                         AI_SYSTEM_PROMPT,
                                         build_diagnosis_prompt(diagnosis.submission))
        diagnosis.result = content
        diagnosis.status = AIDiagnosisStatus.SUCCESS
        diagnosis.save(update_fields=["result", "status"])
    except Exception as e:
        logger.exception(e)
        diagnosis.status = AIDiagnosisStatus.FAILED
        diagnosis.error = str(e)[:500]
        diagnosis.save(update_fields=["status", "error"])
