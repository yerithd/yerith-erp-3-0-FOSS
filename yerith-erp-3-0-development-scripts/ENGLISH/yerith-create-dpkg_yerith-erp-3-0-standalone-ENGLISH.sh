#!/bin/bash

set -e

sudo rm -rf "${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-standalone-ENGLISH"

"${YERITH_ERP_3_0_HOME_FOLDER}"/yerith-erp-3-0-development-scripts/ENGLISH/yerith-create-dpkg_yerith-erp-3-0-ENGLISH.sh -v standalone
