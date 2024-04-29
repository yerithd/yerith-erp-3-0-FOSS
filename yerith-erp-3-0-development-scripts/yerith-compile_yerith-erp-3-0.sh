#/bin/bash

SCRIPTBASENAME=$(basename $0)

USAGE="
       ------------------------------------------------------------
       'YERITH-ERP-3.0' Build System
        @auteur: Dr.-Ing. XAVIER NOUNDOU
	@email:  YERITH.D@GMAIL.COM
       ------------------------------------------------------------
       Usage: "$SCRIPTBASENAME"
	[-h] : help
	[-t] : compile 'yerith-erp-3.0' with QT Test library activated
					for unit tests	
	[-r] : generates an official release build-executable,
	       with 'LAST BUILD ID' set.                                                                                                                                                                                                                                                                           !!! NOT TO USE DURING THE TIME you still are modifying                                                                                                files during development !!!                                                                                                                                                                                      
	[-s] : simulate 'yerith-erp-3.0' compilation
	[-t] : compile 'yerith-erp-3.0' with QT Test library activated
					for unit tests
	[-a] : compile 'yerith-erp-3.0' using financial accounting feature
	[-k] : compile 'yerith-erp-3.0' to use with virtual keyboard
	[-c] : continue previous stopped compilation
	[-g] : compiles 'yerith-erp-3.0' with debug information
	[-v <'yerith-erp-3.0'-version>] :
	       'yerith-erp-3.0'-version := YERITH_CLIENT     | 
	       		        YERITH_SERVER     | 
			        YERITH_STANDALONE |
			        YERITH_ACADEMIC_EVALUATION_VERSION
	[-e] : compiles 'yerith-erp-3.0' in English
	[-f] : compiles 'yerith-erp-3.0' in French
	[-j <n>] : compiles 'yerith-erp-3.0' using 'n' number of jobs
       ------------------------------------------------------------"

NUMBER_OF_JOBS=4

yerithCheckMemoryFlag=
qtTestLibFlag=
financial_accounting_flag=
virtualKeyboardFlag=
simulationFlag=
jobsFlag=
yerithVersionFlag=
debugFlag=
yerithVersionFlag=
continueFlag=
releaseBuildFlag=


while getopts 'mtakhsgv:fecj:r' OPTION
do
  case $OPTION in

    h)	echo "$USAGE"
				exit 1;
		;;

		s)	simulationFlag=1
      	simulationVal="$OPTARG"
        echo "simulation activée."
		;;

		t)	qtTestLibFlag=1
        echo "QT testlib incluse."
		;;

    a)	financial_accounting_flag=1
        echo "COMPTABILITÉ ACTIVÉE"
		;;

		k)	virtualKeyboardFlag=1
        echo "clavier virtuel activé."
		;;

		m)	yerithCheckMemoryFlag=1
     		yerithCheckMemoryVal="YERITH_CHECK_MEMORY"
		;;

    g)	debugFlag=1
     		debugVal="YERITH_DEBUG_LOG"
		;;

    v)	yerithVersionFlag=1
     		yerithVersionVal="$OPTARG"
		;;

    f)	languageFlag=1
      	languageVal="YERITH_FRANCAIS_LANGUAGE"
		;;

    e)	languageFlag=1
      	languageVal="YERITH_ENGLISH_LANGUAGE"
        echo "compilation pour la langue anglaise"
		;;

    c)	continueFlag=1
        echo "continue la compilation"
		;;

		r)	releaseBuildFlag=1
        echo "BUILD OFFICIEL: $(git rev-parse origin/master)"
		;;

    j)	jobsFlag=1
      	jobsVal="$OPTARG"
        echo "compilation activée avec '$jobsVal' jobs."
		;;

    ?)	printf "$USAGE\n" >&2
        exit 2
		;;

  esac
done
shift $(($OPTIND - 1))


yerith-REPLACE-TO-COMPILE-USER-ACTION-ENABLING-IN-CPP-SOURCE-CODE.sh


set +x

if [ ! $yerithCheckMemoryFlag ]; then
  yerithCheckMemoryVal="NO_YERITH_CHECK_MEMORY"
fi

if [ ! $debugFlag ]; then
  debugVal="YERITH_NO_DEBUG"
fi

if [ $yerithVersionFlag ]; then
  if [ "YERITH_CLIENT" = $yerithVersionVal ]; then
    echo "configure to compile 'yerith-erp-3.0' client [YERITH_CLIENT]"
    yerithVersionVal="YERITH_CLIENT"
  elif [ "YERITH_SERVER" = $yerithVersionVal ]; then
    echo "configure to compile 'yerith-erp-3.0' server [YERITH_SERVER]"
    yerithVersionVal="YERITH_SERVER"
  elif [ "YERITH_ACADEMIC_EVALUATION_VERSION" = $yerithVersionVal ]; then
    echo "configure to compile 'yerith-erp-3.0' academic version [YERITH_ACADEMIC_EVALUATION_VERSION]"
    yerithVersionVal="YERITH_ACADEMIC_EVALUATION_VERSION"
  elif [ "YERITH_STANDALONE" = $yerithVersionVal ]; then
    echo "configure to compile 'yerith-erp-3.0' standalone version [YERITH_STANDALONE]"
    yerithVersionVal="YERITH_STANDALONE"
  else
    echo "No version configuration information from user"
    echo "configure to compile 'yerith-erp-3.0' standalone version [YERITH_STANDALONE]"
    yerithVersionVal="YERITH_STANDALONE"
  fi
fi

if [ ! $yerithVersionFlag ]; then
    yerithVersionVal="YERITH_STANDALONE"
fi

if [ ! $jobsFlag ]; then
  jobsVal="$NUMBER_OF_JOBS"
fi

if [ ! $languageFlag ]; then
  languageVal="YERITH_FRANCAIS_LANGUAGE"
fi

if [ $qtTestLibFlag ]; then
		echo "["$SCRIPTBASENAME"] WORKING WITH FILE: ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0.pro !"
		
		sed -i "5s/NO_YERITH_ERP_3_0_TESTING_UNIT_TEST/YERITH_ERP_3_0_TESTING_UNIT_TEST/" ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0.pro
		
		qtTestLibVal="YERITH_ERP_3_0_TESTING_UNIT_TEST"

	else

    qtTestLibVal="NO_YERITH_ERP_3_0_TESTING_UNIT_TEST"
fi

if [ $financial_accounting_flag ]; then
	financial_accounting_value="YERITH_FEATURES_COMPTABILITE_yes"
else
	financial_accounting_value="YERITH_FEATURES_COMPTABILITE_no"
fi

if [ $virtualKeyboardFlag ]; then
    virtualKeyboardVal="YERITH_ERP_3_0_TOUCH_SCREEN"
	else
    virtualKeyboardVal="NO_YERITH_ERP_3_0_TOUCH_SCREEN"
fi


YERITH_BUILD_DATE_TIMESTAMP="$(date '+%H:%M:%S-%s')"

YERITH_BUILD_DATE="$(date '+%d-%m-%Y')"

YERITH_GIT_PUSH_COMMIT_ID="$(git rev-parse origin/master)"

YERITH_GIT_PUSH_COMMIT_ID_TEXT="LAST BUILD ID: '${YERITH_GIT_PUSH_COMMIT_ID}'.\\\n\\\n\""

YERITH_BUILD_COMPUTER="$(uname -srm)"

YERITH_BUILD_COMPUTER_TEXT="ON BUILD_COMPUTER: '${YERITH_BUILD_COMPUTER}'.\\\n\\\n\""

YERITH_CURRENT_LOCALISATION_FOR_RELEASE_BUILD="${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0-development-scripts/YERITH_ERP_3_0_CURRENT_LOCALISATION_FOR_RELEASE_BUILD"

YERITH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE="CLIENT DEDICATED COMPANY LOCALISATION SITE: '$(sed -n '1,1p;1q' ${YERITH_CURRENT_LOCALISATION_FOR_RELEASE_BUILD})'.\\\n\\\n\"";

YERITH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE_CONFIG_CPP="$(sed -n '1,1p;1q' ${YERITH_CURRENT_LOCALISATION_FOR_RELEASE_BUILD})"


if [ $releaseBuildFlag ]; then
		sed -i "s/YERITH_GIT_PUSH_COMMIT_ID/${YERITH_GIT_PUSH_COMMIT_ID}/g" src/utils/yerith-erp-utils.cpp
		sed -i "s/YERITH_BUILD_DATE_TIMESTAMP/${YERITH_BUILD_DATE_TIMESTAMP}/g" src/utils/yerith-erp-utils.cpp
		sed -i "s/YERITH_BUILD_DATE/${YERITH_BUILD_DATE}/g" src/utils/yerith-erp-utils.cpp
		sed -i "s/LAST BUILD ID: .*/${YERITH_GIT_PUSH_COMMIT_ID_TEXT}/g" src/utils/yerith-erp-utils.cpp
		sed -i "s/ON BUILD_COMPUTER: .*/${YERITH_BUILD_COMPUTER_TEXT}/g" src/utils/yerith-erp-utils.cpp
			src/utils/yerith-erp-utils.cpp
		sed -i "s/CLIENT DEDICATED COMPANY LOCALISATION SITE: .*/${YERITH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE}/g" src/utils/yerith-erp-utils.cpp
		sed -i "s/THIS_SITE_LOCALISATION_HARD_CODED_BY_DEVELOPER_FOR_MAXIMUM_BENEFIT_FOR_CLIENT/${YERITH_ERP_3_0_STANDALONE_CURRENT_LOCALISATION_FOR_BUILD_RELEASE_CONFIG_CPP}/g" src/utils/yerith-erp-config.cpp

		if [ "$languageVal" == YERITH_FRANCAIS_LANGUAGE ]; then
			sed -i "s/\bYERITH_ERP_3_0_BINARY_FULL_PATH\b/\/opt\/yerith-erp-3-0-standalone\/bin\/yerith-erp-3-0-standalone/g" yerith-erp-3-0-sql/create_table_configurations.sql
			sed -i "s/\bYERITH_ERP_3_0_BINARY_FULL_PATH\b/\/opt\/yerith-erp-3-0-standalone\/bin\/yerith-erp-3-0-standalone/g" yerith-erp-3-0-sql/create_table_init_configurations.sql
		else
			sed -i "s/\bYERITH_ERP_3_0_BINARY_FULL_PATH\b/\/opt\/yerith-erp-3-0-standalone-ENGLISH\/bin\/yerith-erp-3-0-standalone-ENGLISH/g" yerith-erp-3-0-sql/create_table_configurations.sql
			sed -i "s/\bYERITH_ERP_3_0_BINARY_FULL_PATH\b/\/opt\/yerith-erp-3-0-standalone-ENGLISH\/bin\/yerith-erp-3-0-standalone-ENGLISH/g" yerith-erp-3-0-sql/create_table_init_configurations.sql
		fi
fi


if [ $continueFlag ]; then
	echo "make -j$jobsVal YERITH_ERP_3_0_TEST=$qtTestLibVal YERITH_FEATURES_COMPTABILITE_VALUE=$financial_accounting_value YERITH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YERITH_DEBUG_LOG=$debugVal YERITH_VERSION=$yerithVersionVal YERITH_LANGUAGE=$languageVal YERITH_CHECK_MEMORY=$yerithCheckMemoryVal > ${yerithVersionVal}.out.log	2> ${yerithVersionVal}.error.log"
else
	echo "make clean &&  make -j$jobsVal YERITH_ERP_3_0_TEST=$qtTestLibVal YERITH_FEATURES_COMPTABILITE_VALUE=$financial_accounting_value YERITH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal YERITH_DEBUG_LOG=$debugVal YERITH_VERSION=$yerithVersionVal YERITH_LANGUAGE=$languageVal YERITH_CHECK_MEMORY=$yerithCheckMemoryVal > ${yerithVersionVal}.out.log	2> ${yerithVersionVal}.error.log"
fi


if [ $simulationFlag ]; then
	exit 3
fi


if [ ${languageVal} == "YERITH_ENGLISH_LANGUAGE" ]; then
		cp yerith-erp-3-0-english.qrc yerith-erp-3-0.qrc
elif [ ${languageVal} == "YERITH_FRANCAIS_LANGUAGE" ]; then
		cp yerith-erp-3-0-french.qrc yerith-erp-3-0.qrc
fi

qmake

if [ $continueFlag ]; then
	make -j$jobsVal \
		YERITH_ERP_3_0_TEST=$qtTestLibVal \
		YERITH_FEATURES_COMPTABILITE_VALUE=$financial_accounting_value \
		YERITH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
		YERITH_CHECK_MEMORY=$yerithCheckMemoryVal \
		YERITH_DEBUG_LOG=$debugVal \
		YERITH_VERSION=$yerithVersionVal \
		YERITH_LANGUAGE=$languageVal > "${yerithVersionVal}.out.log" \
		2> "${yerithVersionVal}.error.log" 
else
	make clean && \
	make -j$jobsVal \
		YERITH_ERP_3_0_TEST=$qtTestLibVal \
		YERITH_FEATURES_COMPTABILITE_VALUE=$financial_accounting_value \
		YERITH_VIRTUAL_KEYBOARD_OPTIONS=$virtualKeyboardVal \
		YERITH_CHECK_MEMORY=$yerithCheckMemoryVal \
		YERITH_DEBUG_LOG=$debugVal \
		YERITH_VERSION=$yerithVersionVal \
		YERITH_LANGUAGE=$languageVal > "${yerithVersionVal}.out.log" \
		2> "${yerithVersionVal}.error.log" 
fi

BUILD_SUCCESSFUL="$?"

if [ ${BUILD_SUCCESSFUL} -eq 0 ] && [ $releaseBuildFlag ]; then
		git checkout yerith-erp-3-0-sql/create_table_configurations.sql
		git checkout yerith-erp-3-0-sql/create_table_init_configurations.sql
		git checkout src/utils/yerith-erp-utils.cpp
		git checkout src/utils/yerith-erp-config.cpp
fi

