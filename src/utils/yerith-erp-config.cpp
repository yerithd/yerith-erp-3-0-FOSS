/*
 * yerith-erp-config.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-config.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <QtCore/QDebug>


QString YerithERPConfig::_db_type("MYSQL");

QString YerithERPConfig::_db_name("yerith_erp_3");

QString YerithERPConfig::_db_ip_address("localhost");

QString YerithERPConfig::_db_user_name("yerith_erp_3");

QString YerithERPConfig::_db_user_pwd("12345");

QString YerithERPConfig::_db_connection_options("");

bool YerithERPConfig::ouvrirRegistreDeCaisse(false);

bool YerithERPConfig::SHOW_DATABASE_RAW(false);

QString YerithERPConfig::currency("$");

QString YerithERPConfig::IP_ADDRESS_NETWORK_printer("127.0.0.1");

QString YerithERPConfig::printer("pdf");

QString
YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY
("/opt/yerith-erp-3.0-standalone/YERITH_sql_backup");

QString YerithERPConfig::pathToPdfCOMPANY_LOGO("");

QString YerithERPConfig::pathToPdfReader("/usr/bin/evince");

QString YerithERPConfig::pathToThermalPrinterDeviceFile("/dev/usb/lp0");

QString YerithERPConfig::pathToLatexSystemRootFolder("/usr/bin");

QString YerithERPConfig::receiptFormat("grand (A4)");

QString YerithERPConfig::receiptType("");

QString YerithERPConfig::salesStrategy("");

QString YerithERPConfig::YERITH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER("/etc");

QString YerithERPConfig::YERITH_ERP_3_0_yri_db_runtime_verif_HOME_FOLDER("");

QString YerithERPConfig::YERITH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER("");

QString YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER("");

QString YerithERPConfig::YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER("");

QString
YerithERPConfig::
FILE_ABSOLUTEPATH_YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE
("");

QString
YerithERPConfig::
FILE_ABSOLUTEPATH_YERITH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER
(FILE_YERITH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER);

QString
YerithERPConfig::
FILE_ABSOLUTEPATH_YERITH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER_EN
(FILE_YERITH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER_EN);

QString YerithERPConfig::YRI_DB_RUNTIME_VERIF_SYSTEM_DAEMON_ID_STR("yri-db-runtime");

QString YerithERPConfig::YRI_DB_RUNTIME_VERIF_SYSTEM_DAEMON_ID("yri-db-runtime-verif");

QString YerithERPConfig::YERITH_ERP_3_0_SYSTEM_DAEMON_ID_STR("yerith-erp-3-0-");

QString YerithERPConfig::YERITH_ERP_3_0_SYSTEM_DAEMON_ID("yerith-erp-3-0-system-daemon");

uint YerithERPConfig::backup_restore_yerith_erp_3_time_interval(0);

uint YerithERPConfig::alert_period_time_interval(0);

uint YerithERPConfig::alert_quantity_time_interval(0);

uint YerithERPConfig::pagination_number_of_table_rows(12);

uint YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT(48);

uint YerithERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING(9);

uint YerithERPConfig::max_string_display_length(22);

double YerithERPConfig::tva_value(19.25 / 100.0);

QString
YerithERPConfig::annee_depart_rapports_chiffre_affaire_value
(GET_CURRENT_DATE_YEAR);

QString YerithERPConfig::annee_depart_pour_la_pagination(GET_CURRENT_DATE_YEAR);

QString YerithERPConfig::pathToMARIA_DB_BASE_DIR("/usr/bin");

QString YerithERPConfig::temporaryFilesDir("/tmp");

#ifndef YERITH_DEVEL_TARGET
QString YerithERPConfig::THIS_SITE_LOCALISATION_NAME("YERITH_RD_TEST");
#endif //YERITH_DEVEL_TARGET

bool YerithERPConfig::_distantSiteConnected(false);

QString
YerithERPConfig::_connectedSite;

QString
YerithERPConfig::_connectedSiteIPAddress;

#ifdef YERITH_FRANCAIS_LANGUAGE
const
QString YerithERPConfig::RECEIPT_FORMAT_PETIT(QObject::
                                              trUtf8(RECEIPT_FORMAT_SMALL_FR));
const QString YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER(QObject::tr
                                                            (RECEIPT_FORMAT_LARGE_A5PAPER_FR));
const QString YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER(QObject::tr
                                                            (RECEIPT_FORMAT_LARGE_A4PAPER_FR));
#endif

#ifdef YERITH_ENGLISH_LANGUAGE
const QString YerithERPConfig::RECEIPT_FORMAT_PETIT(QObject::
                                                    trUtf8(RECEIPT_FORMAT_SMALL_EN));
const QString YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER(QObject::tr
                                                            (RECEIPT_FORMAT_LARGE_A5PAPER_EN));
const QString YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER(QObject::tr
                                                            (RECEIPT_FORMAT_LARGE_A4PAPER_EN));
#endif

const QString YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL(QObject::tr
                                                          (OPTION_SALES_STRATEGY_ALL));

const QString YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO(QObject::tr
                                                           (OPTION_SALES_STRATEGY_FIFO));

const QString YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO(QObject::tr
                                                           (OPTION_SALES_STRATEGY_LIFO));

const QString YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO(QObject::tr
                                                           (OPTION_SALES_STRATEGY_FEFO));

const int YerithERPConfig::SHOW_DATABASE_RAW_table_column(1);

const int YerithERPConfig::HIDE_DATABASE_RAW_table_column(0);

const int YerithERPConfig::OPEN_CASH_DRAWER(1);

const int YerithERPConfig::CLOSE_CASH_DRAWER(0);


const unsigned int YerithERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL(0);

const unsigned int YerithERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL(1);

const unsigned int YerithERPConfig::CONFIG_TVA_VALUE(2);

const unsigned int YerithERPConfig::CONFIG_SALES_STRATEGY(3);

const unsigned int YerithERPConfig::CONFIG_CURRENCY(4);

const unsigned int YerithERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR(5);

const unsigned int YerithERPConfig::CONFIG_BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL(6);

const unsigned int
YerithERPConfig::CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3
(7);


void YerithERPConfig::initYerithConfig(QString initCfg)
{
    QFile file(initCfg);

    if (!file.open(QFile::ReadOnly))
    {
        return;
    }

    QTextStream init_cfg(&file);
    QString line;
    QStringList list;

    do
    {
        line = init_cfg.readLine();
        list =
                        line.split(YERITH_ERP_3_0_CONFIGURATION_FILE_SEPARATION_OPERATOR);
        //logger << "++ line: " << line << "\n";

        if ("db_type" == list.at(0))
        {
            YerithERPConfig::_db_type = list.at(1).trimmed();
            // logger << "++ db_type = " << db_type << "\n";
        }
        if ("db_name" == list.at(0))
        {
#ifndef YERITH_ERP_3_0_TESTING_UNIT_TEST
            YerithERPConfig::_db_name = list.at(1).trimmed();
#else
            YerithERPConfig::_db_name =
                            QString("%1_test").arg(list.at(1).trimmed());
#endif

            //logger << "++ db_name = " << db_name << "\n";
        }
        else if ("db_ip_address" == list.at(0))
        {
            YerithERPConfig::_db_ip_address = list.at(1).trimmed();
            //logger << "++ db_ip_address = " << db_ip_address << "\n";
        }
        else if ("db_user_name" == list.at(0))
        {
            YerithERPConfig::_db_user_name = list.at(1).trimmed();
            //logger << "++ db_user_name = " << db_user_name << "\n";
        }
        else if ("db_user_pwd" == list.at(0))
        {
            YerithERPConfig::_db_user_pwd = list.at(1).trimmed();
            //logger << "++ db_user_pwd = " << db_user_pwd << "\n";
        }
        else if ("db_connection_options" == list.at(0))
        {
            YerithERPConfig::_db_connection_options = list.at(1).trimmed();
            //logger << "++ db_connection_options = " << db_connection_options << "\n";
        }
    }
    while (!line.isNull());
}


void YerithERPConfig::saveYerithConfig()
{
    QFile file(YerithERPConfig::FILE_ABSOLUTEPATH_YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE);

    if (!file.open(QFile::WriteOnly))
    {
        return;
    }

    QTextStream textStream(&file);

    textStream
            << QString("local_parameter_full_path_pdf_reader=%1\n")
				.arg(YerithERPConfig::pathToPdfReader)

			<< QString("local_parameter_full_path_pdf_latex_executable_root_dir=%1\n")
				.arg(YerithERPConfig::pathToLatexSystemRootFolder)

			<< QString("local_parameter_full_path_mariadb_client_executables_root_dir=%1\n")
				.arg(YerithERPConfig::pathToMARIA_DB_BASE_DIR)

			<< QString("local_parameter_full_path_file_temporary_folder=%1\n")
				.arg(YerithERPConfig::temporaryFilesDir)

			<< QString("local_parameter_file_system_device_thermal_printer=%1\n")
				.arg(YerithERPConfig::pathToThermalPrinterDeviceFile)

			<< QString("local_parameter_type_receipt=%1\n")
				.arg(YerithERPConfig::receiptType)

			<< QString("local_parameter_IP_address_network_thermal_printer=%1\n")
				.arg(YerithERPConfig::IP_ADDRESS_NETWORK_printer)

			<< QString("local_parameter_thermal_printer=%1\n")
				.arg(YerithERPConfig::printer)

			<< QString("local_parameter_SHOW_DATABASE_RAW_table_column=%1\n")
					.arg(YerithERPConfig::SHOW_DATABASE_RAW ? 1 : 0)

			<< QString("local_parameter_state_open_cash_register=%1\n")
				.arg(YerithERPConfig::ouvrirRegistreDeCaisse ? 1 : 0)

			<< QString("local_parameter_format_receipt=%1\n")
				.arg(YerithERPConfig::receiptFormat)

			<< QString("local_parameter_starting_year_business_dashboard=%1\n")
				.arg(YerithERPConfig::annee_depart_rapports_chiffre_affaire_value)

			<< QString("local_parameter_length_maximal_display_string=%1\n")
				.arg(QString::number(YerithERPConfig::max_string_display_length))

			<< QString("local_parameter_length_line_feed_thermal_print_character_count=%1\n")
				.arg(QString::number(YerithERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING))

			<< QString("local_parameter_length_standard_pagination_number=%1\n")
				.arg(QString::number(YerithERPConfig::pagination_number_of_table_rows))

			<< QString("local_parameter_length_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT=%1\n")
				.arg(QString::number(YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT));

    file.close();
}
