#!/bin/sh

SCRIPTBASENAME=$(basename $0)

USAGE="
------------------------------------------------------------
'YERITH-ERP-3.0' Build System
 @auteur: Dr.-Ing. XAVIER NOUNDOU
------------------------------------------------------------
 Usage: "$SCRIPTBASENAME"
 [-h] : help
 [-i] : input file containing user enabling actions in FORM
				'action,false', or 'action,true'
 [-o] : specifies an output file name.
------------------------------------------------------------"

NUMBER_OF_JOBS=4

rawFlag=
inputFlag=
outputFlag=

while getopts 'rhi:o:' OPTION
do
  case $OPTION in

    h)	echo "$USAGE"
				exit 1;
		;;

		r)	rawFlag=1
		;;
	
	  i)	inputFlag=1
      	inputVal="$OPTARG"
		;;
	
		o)	outputFlag=1
      	outputVal="$OPTARG"
		;;

    ?)	printf "$USAGE\n" >&2
        exit 2
		;;

  esac
done
shift $(($OPTIND - 1))

YERITH_FILE_INPUT=""

YERITH_FILE_OUTPUT=""

if [ ! $inputFlag ]; then
	echo "$USAGE"
	echo "... MISSING INPUT FILE NAME ..."
	exit 1
else
	YERITH_FILE_INPUT="${inputVal}"
fi

if [ ! $outputFlag ]; then
	echo "$USAGE"
	echo "... MISSING OUTPUT FILE NAME ..."
	exit 3
else
	YERITH_FILE_OUTPUT="${outputVal}"
fi


#echo "${SCRIPTBASENAME} | WORKING WITH INPUT FILE: ${YERITH_FILE_INPUT}"

#echo "${SCRIPTBASENAME} | WORKING WITH OUTPUT FILE: ${YERITH_FILE_OUTPUT}"


# FOLLOWING LINE ISOLATE A LINE PATTERN AS: 'YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);'
# TO RESULTING ACTION USER WORD: 'actionEntrer, false'.

# OPTION '-F' OF AWK ENABLES US TO SPECIFY A SEPARATING PATTERN FOR
# WORD OTHER THAN EMPTY SPACE (' ')!

YERITH_TEXT_TO_PLACE_INSIDE_CODE=""

if [ $rawFlag ]; then
	YERITH_TEMPORARY_USER_ACTION_FILE_CONTENT="${YERITH_FILE_INPUT%%".txt"}-raw.txt"
	cat "${YERITH_FILE_INPUT}" | awk -F'(' '//{print $2}' | awk -F')' '//{print $1}' > "${YERITH_TEMPORARY_USER_ACTION_FILE_CONTENT}"
	
	YERITH_TEXT_TO_PLACE_INSIDE_CODE=`cat "${YERITH_TEMPORARY_USER_ACTION_FILE_CONTENT}" | \
	awk '//{print "YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(" $0 ");" }'`

	exit 7
else
	YERITH_TEXT_TO_PLACE_INSIDE_CODE=`cat "${YERITH_FILE_INPUT}" | \
	awk '//{print "YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(" $0 "); " }'`
fi

YERITH_TEMP_OUTPUT=""

counter=0
for i in ${YERITH_TEXT_TO_PLACE_INSIDE_CODE}; do
	if [ $counter -gt 0	]; then
		YERITH_TEMP_OUTPUT="${YERITH_TEMP_OUTPUT}\\\\n    $i"
	else
		YERITH_TEMP_OUTPUT="$i"
	fi
	#echo "$counter: ${YERITH_TEMP_OUTPUT}"
	counter=$((counter+1))
done

YERITH_TEXT_TO_PLACE_INSIDE_CODE="${YERITH_TEMP_OUTPUT}"

#echo "${YERITH_TEXT_TO_PLACE_INSIDE_CODE}"

echo "${YERITH_TEXT_TO_PLACE_INSIDE_CODE}" > "${YERITH_FILE_OUTPUT}"


