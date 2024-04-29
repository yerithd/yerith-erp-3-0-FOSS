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
       	echo "Creating .deb file, for ${yerithpos7VersionVal} version of Yerith-erp-3-0"
        ;;

    c)	yerithpos7VersionFlag=1
			  yerithpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerithpos7VersionVal} version of Yerith-erp-3-0"
        ;;

    v)	yerithpos7VersionFlag=1
			  yerithpos7VersionVal="$OPTARG"
       	echo "Creating .deb file, for ${yerithpos7VersionVal} version of Yerith-erp-3-0"
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
	echo "$(basename $0) | please, give a correct Yerith-erp-3-0 version"
	echo "${USAGE}"	
	exit 4
fi

set -x

USR_SHARE="/usr/share"

YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER=".yerith_erp_3_0"

YERITH_ERP_3_0_DEB_FILE_FOLDER="${YERITH_ERP_3_0_HOME_FOLDER}/$yerith-erp-3-0-deb-file-repository"

YERITH_ERP_3_0_LICENSE_TYPE="${yerithpos7VersionVal}"

YERITH_ERP_3_0_BINARY_NAME="yerith-erp-3-0-${YERITH_ERP_3_0_LICENSE_TYPE}"

YERITH_ERP_3_0_BINARY_NAME_FOR_FILE="yerith-erp-3-0-${YERITH_ERP_3_0_LICENSE_TYPE}"

YERITH_ERP_3_0_DEB_FILE_NAME="${YERITH_ERP_3_0_HOME_FOLDER}/${YERITH_ERP_3_0_BINARY_NAME}.deb"

YERITH_ERP_3_0_BINARY_FILE_PATH="${YERITH_ERP_3_0_HOME_FOLDER}/bin/${YERITH_ERP_3_0_BINARY_NAME}"


TARGET_INSTALLATION_FOLDER="${YERITH_ERP_3_0_HOME_FOLDER}/${YERITH_ERP_3_0_BINARY_NAME}"

if [ ! -d "${TARGET_INSTALLATION_FOLDER}" ]; then
    echo "creating installation directory: ${TARGET_INSTALLATION_FOLDER}"
    mkdir -p "${TARGET_INSTALLATION_FOLDER}"
fi


OPT_INSTALLATION_TARGET_DIR="${TARGET_INSTALLATION_FOLDER}/opt/${YERITH_ERP_3_0_BINARY_NAME}"

mkdir -p "${OPT_INSTALLATION_TARGET_DIR}"

TMP_INSTALLATION_TARGET_DIR="${TARGET_INSTALLATION_FOLDER}/tmp"

mkdir -p "${TMP_INSTALLATION_TARGET_DIR}"

TARGET_INSTALLATION_FOLDER_script="${OPT_INSTALLATION_TARGET_DIR}/bin"

TARGET_INSTALLATION_FOLDER_BIN="${OPT_INSTALLATION_TARGET_DIR}/bin"

mkdir -p "${TARGET_INSTALLATION_FOLDER_BIN}"

TARGET_INSTALLATION_USER_LOCAL_SETTINGS_FOLDER="${OPT_INSTALLATION_TARGET_DIR}/${YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER}"

mkdir -p --mode=777 "${TARGET_INSTALLATION_USER_LOCAL_SETTINGS_FOLDER}"

TARGET_INSTALLATION_FOLDER_DOC="${TARGET_INSTALLATION_FOLDER}${USR_SHARE}/doc/yerith-erp-3-0"

mkdir -p "${TARGET_INSTALLATION_FOLDER_DOC}"

YERITH_ERP_3_0_PROPERTIES_FILE_DIR=${OPT_INSTALLATION_TARGET_DIR}

touch "$OPT_INSTALLATION_TARGET_DIR/yerith-erp-3-0.log"

ECHO="echo"

CP="cp"

YERITH_ERP_3_0_DEB_PACKAGE_INFORMATION="Package: ${YERITH_ERP_3_0_BINARY_NAME}"

mkdir "${TARGET_INSTALLATION_FOLDER}/DEBIAN"


if [ ! "$qsqlite3Flag" ]; then
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/DEBIAN/control "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/DEBIAN/postinst "${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst"
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/DEBIAN/postrm "${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm"
else
	${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/DEBIAN/control_academic "${TARGET_INSTALLATION_FOLDER}/DEBIAN/control"
fi


${ECHO} -e "${YERITH_ERP_3_0_DEB_PACKAGE_INFORMATION}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/control


YERITH_ERP_3_0_POSTRM_STR="#!/bin/bash
sed -i '/YERITH_ERP_3_0_HOME_FOLDER/d' /etc/environment
sed -i '/YERITH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER/d' /etc/environment"

${ECHO} -e "${YERITH_ERP_3_0_POSTRM_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postrm


YERITH_ERP_3_0_POSTINST_STR="#!/bin/bash
echo -e \"export YERITH_ERP_3_0_HOME_FOLDER=/opt/${YERITH_ERP_3_0_BINARY_NAME}\" >> /etc/environment
echo -e \"export YERITH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER=/opt/yerith-erp-3-0-system-daemon\" >> /etc/environment
touch /opt/${YERITH_ERP_3_0_BINARY_NAME}/yerith-erp-3-0.log
chmod go+w /opt/${YERITH_ERP_3_0_BINARY_NAME}/yerith-erp-3-0.log"

${ECHO} -e "${YERITH_ERP_3_0_POSTINST_STR}$(cat ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst)" > ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

chmod 755 ${TARGET_INSTALLATION_FOLDER}/DEBIAN/postinst

${ECHO} "*** YERITH *** ${YERITH_ERP_3_0_BINARY_FILE_PATH}"

${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-deployment-configuration-scripts/yerith-erp-3-0-configure-mysql-server-set-root-pwd.exp \
	${TMP_INSTALLATION_TARGET_DIR}

${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-deployment-configuration-scripts/yerith-erp-3-0-configure-mysql-server.sh ${TARGET_INSTALLATION_FOLDER_script}

${CP} ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-deployment-configuration-scripts/yerith-erp-3-0-standalone-setup-database-and-user.sh ${TARGET_INSTALLATION_FOLDER_script}

${CP} ${YERITH_ERP_3_0_BINARY_FILE_PATH} ${TARGET_INSTALLATION_FOLDER_BIN}

${CP} ${YERITH_ERP_3_0_BINARY_FILE_PATH}.sh ${TARGET_INSTALLATION_FOLDER_BIN}

# ********************************************************************************** #

YERITH_ERP_3_0_SCRIPT_INSTALL_ROOT_DIR="${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-development-scripts"

YERITH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR="${YERITH_ERP_3_0_HOME_FOLDER}/doc"

SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR="${YERITH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yerith-fiche-de-donnees"

FRANCAIS_COMPARAISON_ERP_DIR="${YERITH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yerith-erp-document-comparaisons"

SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR="${YERITH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yerith-whitepapers"

SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR="${YERITH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yerith-brochure"

SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DE_GESTION_COMMERCIALE_DIR="${YERITH_ERP_3_2_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yerith-gestion-commerciale-notions-elementaires"

FRANCAIS_COMPARAISON_ERP_PDF_CHEMIN="${FRANCAIS_COMPARAISON_ERP_DIR}/yerith-erp-3-0-document-comparaisons.pdf"

FRANCAIS_JH_NISSI_COMPENDIUM_PDF_CHEMIN="${YERITH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/JH_NISSI_ERP_PGI_COMPENDIUM.pdf"

FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_FICHE_DE_DONNEES_DIR}/yerith-erp-3-0-fiche-de-donnees.pdf"

FRANCAIS_WHITE_PAPERS_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_WHITE_PAPERS_DIR}/yerith-erp-*.pdf"

FRANCAIS_BROCHURE_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DIR}/yerith-erp-3-0-brochure.pdf"

FRANCAIS_BROCHURE_DE_GESTION_COMMERCIALE_PDF_CHEMIN="${SOURCE_DOCUMENTATION_FRANCAIS_BROCHURE_DE_GESTION_COMMERCIALE_DIR}/yerith-erp-3-0-brochure-gestion-commerciale-notions.pdf"

FRANCAIS_MANUEL_DE_LUTILISATEUR_CHEMIN="${YERITH_ERP_3_0_DOCUMENTATION_FRANCAIS_ROOT_DIR}/yerith-erp-3-0-software-system-uses/*.pdf"

mkdir -p "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_JH_NISSI_COMPENDIUM_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_FICHE_DE_DONNEES_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_COMPARAISON_ERP_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_WHITE_PAPERS_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_BROCHURE_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_BROCHURE_DE_GESTION_COMMERCIALE_PDF_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

${CP} ${FRANCAIS_MANUEL_DE_LUTILISATEUR_CHEMIN} "${TARGET_INSTALLATION_FOLDER_DOC}/francais"

# ********************************************************************************** #

USR_SHARE_PIXMAPS="${USR_SHARE}/pixmaps"

USR_SHARE_APPLICATIONS="${USR_SHARE}/applications"

TARGET_INSTALLATION_FOLDER_USR_SHARE_PIXMAPS="${TARGET_INSTALLATION_FOLDER}${USR_SHARE_PIXMAPS}"

mkdir -p "${TARGET_INSTALLATION_FOLDER_USR_SHARE_PIXMAPS}"

TARGET_INSTALLATION_FOLDER_USR_SHARE_APPLICATIONS="${TARGET_INSTALLATION_FOLDER}${USR_SHARE_APPLICATIONS}"

mkdir -p "${TARGET_INSTALLATION_FOLDER_USR_SHARE_APPLICATIONS}"

LOGO_YERITH_ERP_3_0=${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-images/yerith-pgi-3-0.svg

DESKTOP_FILE_YERITH_ERP_3_0=${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-gnome-files-desktop/${YERITH_ERP_3_0_BINARY_NAME_FOR_FILE}.desktop

sudo ${CP} ${DESKTOP_FILE_YERITH_ERP_3_0} ${TARGET_INSTALLATION_FOLDER_USR_SHARE_APPLICATIONS}

sudo ${CP} ${LOGO_YERITH_ERP_3_0} ${TARGET_INSTALLATION_FOLDER_USR_SHARE_PIXMAPS}

rm -f ${YERITH_ERP_3_0_DEB_FILE_NAME}

TARGET_INSTALLATION_FOLDER_DEBIAN="${TARGET_INSTALLATION_FOLDER}/DEBIAN"

TARGET_INSTALLATION_FOLDER_OPT="${TARGET_INSTALLATION_FOLDER}/opt"

TARGET_INSTALLATION_FOLDER_USR="${TARGET_INSTALLATION_FOLDER}/usr"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_DEBIAN}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_OPT}"

chown_and_grp_for_lintian "${TARGET_INSTALLATION_FOLDER_USR}"



dpkg-deb --verbose --build ${YERITH_ERP_3_0_HOME_FOLDER}/${YERITH_ERP_3_0_BINARY_NAME}
