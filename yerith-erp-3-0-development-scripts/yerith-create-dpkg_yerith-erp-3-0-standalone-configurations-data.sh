#!/bin/bash

set -e

sudo rm -rf "${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-standalone-configurations-data"

"${YERITH_ERP_3_0_HOME_FOLDER}"/yerith-erp-3-0-development-scripts/yerith-create-dpkg_yerith-erp-3-0-configurations-data.sh -v standalone
