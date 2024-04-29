/*
 * yerith-erp-process-info.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_UTILS_YERITH_PROCESS_INFO_HPP_
#define SRC_UTILS_YERITH_PROCESS_INFO_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"


#include "src/process/yerith-erp-process.hpp"


#include <QtCore/QString>


class YerithUtils;
class YerithLogger;

class YerithProcessInfo
{
public:

    YERITH_CLASS_OPERATORS inline YerithProcessInfo()
    {
    }

    inline ~YerithProcessInfo()
    {
    }

    static bool CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(QString a_daemon_process_ID_STR,
    													QString executable_full_path,
    													YerithLogger *logger = 0);

    inline static bool check_Yr_Db_Runtime_Verif_Daemon_Running(YerithLogger *logger = 0)
    {
    	return
    	  YerithProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(
    			  	  	  	  	  YerithERPConfig::YRI_DB_RUNTIME_VERIF_SYSTEM_DAEMON_ID_STR,
								  YerithUtils::GET_YRI_DB_RUNTIME_VERIF_EXECUTABLE_FULL_PATH(),
								  logger);
    }

    inline static bool checkYerithERPAlertDaemonRunning(YerithLogger *logger = 0)
    {
    	return
    	  YerithProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(
    			  	  	  	  	  YerithERPConfig::YERITH_ERP_3_0_SYSTEM_DAEMON_ID_STR,
								  YerithUtils::getYerithAlertDaemonExecutableFullpath(),
								  logger);
    }
};


#endif /* SRC_UTILS_YERITH_PROCESS_INFO_HPP_ */
