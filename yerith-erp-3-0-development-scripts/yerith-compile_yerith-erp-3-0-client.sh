#!/bin/bash
# @file: yerith-compile_yerith-erp-3-0-client.sh
# @author: Dr.-Ing. XAVIER NOUNDOU
# @email: YERITH.D@GMAIL.COM



if [[ "${@}" == *"s"* || "${@}" == *"h"* ]]; then
		yerith-compile_yerith-erp-3-0.sh -v YERITH_CLIENT "${@}"
		exit
fi

if [[ "${@}" == *"e"* ]]; then
	OUT_CMD_TIME=$(time yerith-compile_yerith-erp-3-0.sh -v YERITH_CLIENT "${@}" > /dev/null 2>&1) && \
		mv bin/yerith-erp-3-0 bin/yerith-erp-3-0-server-ENGLISH
else
	OUT_CMD_TIME=$(time yerith-compile_yerith-erp-3-0.sh -v YERITH_CLIENT "${@}" > /dev/null 2>&1) && \
		mv bin/yerith-erp-3-0 bin/yerith-erp-3-0-server
fi

val=$(cat YERITH_CLIENT.error.log | grep 'error\|ERROR\|Error' | wc -l)

if [ ${val} -eq 0 ]; then
		echo "[compilation avec succes]"
else
		echo "[compilation avec des erreurs (${val})]"
fi

echo ${OUT_CMD_TIME}
