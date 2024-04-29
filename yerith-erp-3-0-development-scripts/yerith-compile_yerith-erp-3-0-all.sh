#!/bin/bash
# @file: yerith-compile_yerith-erp-3-0-all.sh
# @author: Dr.-Ing. XAVIER NOUNDOU
# @email: YERITH.D@GMAIL.COM

#yerith-compile_yerith-erp-3-0-standalone.sh "$@"; \
  yerith-compile_yerith-erp-3-0-server.sh "$@"; \
  yerith-compile_yerith-erp-3-0-client.sh "$@"; \
  yerith-compile_yerith-erp-3-0-academic.sh "$@"
