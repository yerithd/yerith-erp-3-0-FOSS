#!/bin/bash

USAGE="$(basename $0) [-e] <-v [qsqlite3 | standalone | academic | client | server]>"


function chown_and_grp_for_lintian ()
{
		FILE_OR_FOLDER="$1"

		sudo chown -R root "${FILE_OR_FOLDER}"
		sudo chgrp -R root "${FILE_OR_FOLDER}"
}


# We don't want to have temporary files "*.bak"
# within our '.deb' packages
find . -name "*.bak" -delete

if [ $# -lt 2 ]; then
 echo "${USAGE}"
 exit 1
fi 

yerithpos7VersionFlag=
qsqlite3Flag=

while getopts 's:c:ev:' OPTION
do
  case $OPTION in
    s)	yerithpos7VersionFlag=1
			  yerithpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerithpos7VersionVal} version of Yerith-erp-3.0"
        ;;

    c)	yerithpos7VersionFlag=1
			  yerithpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerithpos7VersionVal} version of Yerith-erp-3.0"
        ;;

    v)	yerithpos7VersionFlag=1
			  yerithpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerithpos7VersionVal} version of Yerith-erp-3.0"
        ;;
    e)	qsqlite3Flag=1
       	echo "Creating evaluation .deb file, using qsqlite3"
        ;;
    ?)	printf "$USAGE" >&2
        exit 2
	;;
  esac
done
shift $(($OPTIND - 1))

if [ ! "$yerithpos7VersionFlag" ]; then
  echo "${USAGE}"
	exit 3
fi

if [ "$yerithpos7VersionVal" == "client" ]; then
	echo "yerithpos7VersionVal: client"
elif [ "$yerithpos7VersionVal" == "server" ]; then
	echo "yerithpos7VersionVal: server"
elif [ "$yerithpos7VersionVal" == "standalone" ]; then
	echo "yerithpos7VersionVal: standalone"
elif [ "$yerithpos7VersionVal" == "academic" ]; then
	echo "yerithpos7VersionVal: academic"
else
	echo "$(basename $0) | please, give a correct Yerith-erp-3.0 version"
	echo "${USAGE}"	
	exit 4
fi

set -x

YERITH_ERP_3_0_LICENSE_TYPE="${yerithpos7VersionVal}"

YERITH_ERP_3_0_BINARY_NAME="yerith-erp-3-0-${YERITH_ERP_3_0_LICENSE_TYPE}-configurations-data"

YERITH_ERP_3_0_DEB_FILE_NAME="${YERITH_ERP_3_0_HOME_FOLDER}/${YERITH_ERP_3_0_BINARY_NAME}.deb"

TARGET_INSTALLATION_FOLDER="${YERITH_ERP_3_0_HOME_FOLDER}/${YERITH_ERP_3_0_BINARY_NAME}"

if [ ! -d "${TARGET_INSTALLATION_FOLDER}" ]; then
    echo "creating installation directory: ${TARGET_INSTALLATION_FOLDER}"
    mkdir -p "${TARGET_INSTALLATION_FOLDER}"
fi


OPT_INSTALLATION_BASE_TARGET_DIR="${TARGET_INSTALLATION_FOLDER}/opt"

OPT_INSTALLATION_TARGET_DIR="${OPT_INSTALLATION_BASE_TARGET_DIR}/${YERITH_ERP_3_0_BINARY_NAME}"

mkdir -p "${OPT_INSTALLATION_TARGET_DIR}"

YERITH_SQL_BACKUP_FOLDER_NAME="yerith-erp-3-0-sql-backup-restore"

TARGET_INSTALLATION_SQL_BACKUP_FOLDER="${OPT_INSTALLATION_BASE_TARGET_DIR}/${YERITH_SQL_BACKUP_FOLDER_NAME}"

mkdir -p "${TARGET_INSTALLATION_SQL_BACKUP_FOLDER}"

CP="cp"

YERITH_ERP_3_0_DEB_PACKAGE_INFORMATION="Package: ${YERITH_ERP_3_0_BINARY_NAME}"

mkdir "${TARGET_INSTALLATION_FOLDER}/DEBIAN"


if [ ! "$qsqlite3Flag" ]; then
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/DEBIAN/control_configurations_data "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/DEBIAN/postinst "${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst"
else
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/DEBIAN/control_academic "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
fi


echo -e "${YERITH_ERP_3_0_DEB_PACKAGE_INFORMATION}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control


if [ ! "$qsqlite3Flag" ]; then
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-deployment-configuration-scripts/yerith-erp-3-0.properties \
		"${OPT_INSTALLATION_TARGET_DIR}"

	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-deployment-configuration-scripts/yerith-erp-3-0-system-local-configuration.properties \
			"${OPT_INSTALLATION_TARGET_DIR}"
else
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-deployment-configuration-scripts/yerith-erp-3-0-qsqlite3.properties \
			"${OPT_INSTALLATION_TARGET_DIR}"

	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-deployment-configuration-scripts/yerith-erp-3-0-system-local-configuration.properties \
			"${OPT_INSTALLATION_TARGET_DIR}"
fi

YERITH_ERP_3_0_POSTRM_STR="#!/bin/bash
sed -i '/YERITH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER/d' /etc/environment
sed -i '/YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE/d' /etc/environment"

echo -e "${YERITH_ERP_3_0_POSTRM_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm

YERITH_ERP_3_0_POSTINST_STR="#!/bin/bash
echo -e \"export YERITH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER=/opt/${YERITH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE=/opt/${YERITH_ERP_3_0_BINARY_NAME}/yerith-erp-3-0-system-local-configuration.properties\" >> /etc/environment
chmod go+rw /opt/${YERITH_SQL_BACKUP_FOLDER_NAME}
chmod go+rw /opt/${YERITH_ERP_3_0_BINARY_NAME}/yerith-erp-3-0-system-local-configuration.properties"

echo -e "${YERITH_ERP_3_0_POSTINST_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

# ********************************************************************************** #

rm -f ${YERITH_ERP_3_0_DEB_FILE_NAME}

TARGET_INSTALLATION_FOLDER_DEBIAN="${TARGET_INSTALLATION_FOLDER}/DEBIAN"

TARGET_INSTALLATION_FOLDER_OPT="${TARGET_INSTALLATION_FOLDER}/opt"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_DEBIAN}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_OPT}"

dpkg-deb --verbose --build ${YERITH_ERP_3_0_HOME_FOLDER}/${YERITH_ERP_3_0_BINARY_NAME}
