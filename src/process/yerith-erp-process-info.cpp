/*
 * yerith-erp-process-info.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-process-info.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include <QtCore/QByteArray>

#include <QtCore/QDebug>

#include <QtCore/QFile>

#include <QtCore/QProcess>




bool YerithProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(QString a_daemon_process_ID_STR,
																QString executable_full_path,
																YerithLogger *logger /* = 0 */)
{
	if (logger)
	{
		logger->log
		("yerith-erp-3-0 | YerithProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING");
	}

	QStringList progArguments;

	progArguments << "-a"
				  << a_daemon_process_ID_STR;

	QString PGREP_Program("/usr/bin/pgrep");

	QString yerithpsoutput_file(QString("%1/%2_psoutput")
									.arg(YerithERPConfig::temporaryFilesDir,
										 QString(".%1")
										 	 .arg(a_daemon_process_ID_STR)));

//	QDEBUG_STRING_OUTPUT_2(QString("%1: temporary psoutput file: ")
//								.arg(a_daemon_process_ID_STR),
//						   yerithpsoutput_file);

	int output_file_size =
			YerithERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE
								(PGREP_Program,
								 yerithpsoutput_file,
								 progArguments);

	if (output_file_size > 0)
	{
//		QDEBUG_STRING_OUTPUT_2_N(QString("%1; output_file_size: ")
//									.arg(a_daemon_process_ID_STR),
//								  output_file_size);

		return YerithUtils::GREP_YERITH_FILE_CONTENT(yerithpsoutput_file,
													 executable_full_path,
													 a_daemon_process_ID_STR);
	}

	return false;
}

