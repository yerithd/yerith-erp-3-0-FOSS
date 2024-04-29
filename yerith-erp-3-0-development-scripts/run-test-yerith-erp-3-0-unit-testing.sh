#/bin/bash
#@author: Dr.-Ing. XAVIER NOUNDOU

SCRIPTBASENAME=$(basename $0)

USAGE="
       ------------------------------------------------------------
       'YERITH-ERP-3.0' Unit Testing Running System
        @auteur: Dr.-Ing. XAVIER NOUNDOU
       ------------------------------------------------------------
       Usage: "${SCRIPTBASENAME}"
	[-h] : help
	[-r] : output test result on screen (stdout) rather than file
	[-s] : simulate 'yerith-erp-3.0' unit testing run
	[-c] : run 'yerith-erp-3.0' unit test and also generates
	        coverage information with 'gcov'
       ------------------------------------------------------------"

jobsVal=2
jobsFlag=
buildFlag=
screenOutputFlag=
simulationFlag=
coverageGenFlag=

while getopts 'cbj:srh' OPTION
do
  case $OPTION in

		h)	echo "$USAGE"
				exit 1;
		;;

		r)	screenOutputFlag=1
		;;

		s)	simulationFlag=1
        echo "simulation activée."
		;;

		j)	jobsFlag=1
      	jobsVal="$OPTARG"
        echo "compilation activée avec '$jobsVal' jobs."
		;;


		b)	buildFlag=1
		;;

    c)	coverageGenFlag=1
		;;

    ?)	printf "$USAGE\n" >&2
        exit 2
		;;

  esac
done
shift $(($OPTIND - 1))

if [ $buildFlag ]; then
		if [ $simulationFlag ]; then
				echo "yerith-compile_yerith-erp-3-0-standalone.sh -j${jobsVal} -c"
		else
				yerith-compile_yerith-erp-3-0-standalone.sh -j${jobsVal} -c
		fi
fi


if [ $simulationFlag ]; then
	if [ $screenOutputFlag ]; then
		echo "${YERITH_ERP_3_0_HOME_FOLDER}/bin/yerith-erp-3-0-standalone"
	else
		echo "${YERITH_ERP_3_0_HOME_FOLDER}/bin/yerith-erp-3-0-standalone > test-results-yerith-erp-3-0.txt 2>&1"
	fi
else
	if [ $screenOutputFlag ]; then
		${YERITH_ERP_3_0_HOME_FOLDER}/bin/yerith-erp-3-0-standalone
	else
		${YERITH_ERP_3_0_HOME_FOLDER}/bin/yerith-erp-3-0-standalone > test-results-yerith-erp-3-0.txt 2>&1
	fi
fi


if [ $coverageGenFlag ]; then
		if [ $simulationFlag ]; then
				echo "run-test-yerith-erp-3-0-coverage-gen-html.sh > test-results-yerith-erp-3-0-coverage-output.txt 2>&1"
		else
				run-test-yerith-erp-3-0-coverage-gen-html.sh > test-results-yerith-erp-3-0-coverage-output.txt 2>&1
		fi
fi

git checkout ${YERITH_ERP_3_0_HOME_FOLDER}/yerith-erp-3-0.pro

