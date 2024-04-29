#!/bin/bash
#@author: Dr.-Ing. XAVIER NOUNDOU

YERITH_ERP_3_0_IMAGES=${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-images

echo "$(basename $0): processing '.png' files in folder ${YERITH_ERP_3_0_IMAGES}"

find ${YERITH_ERP_3_0_IMAGES} -type f -name "*.png" -exec convert {} -strip {} \;
