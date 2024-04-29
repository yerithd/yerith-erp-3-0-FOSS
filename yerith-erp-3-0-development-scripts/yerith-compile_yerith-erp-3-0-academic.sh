#!/bin/bash
# @file: yerith-compile_yerith-erp-3-0-academic.sh
# @author: Dr.-Ing. XAVIER NOUNDOU
# @email: YERITH.D@GMAIL.COM

OUT_CMD_TIME=$(yerith-compile_yerith-erp-3-0.sh -v YERITH_ACADEMIC_EVALUATION_VERSION "${@}" 2> academic.error.log) && \
  mv bin/yerith-erp-3-0 bin/yerith-erp-3.0-academic

val=$(cat academic.error.log | grep error | wc -l)

if [ ${val} -eq 0 ]; then
		echo [compilation avec succes]
else
		echo [compilation avec des erreurs]
fi

echo ${OUT_CMD_TIME}

