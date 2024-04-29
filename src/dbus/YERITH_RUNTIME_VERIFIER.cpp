/*
 * YERITH_RUNTIME_VERIFIER.hpp
 *
 *      Author: PROF. DR.-ING. DIPL.-INF. Xavier Noundou
 */


#include "YERITH_RUNTIME_VERIFIER.hpp"


#include "src/utils/yerith-erp-utils.hpp"





uint YERITH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT(0);

uint YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index(0);

QStringList YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings;

const QString YERITH_RUNTIME_VERIFIER::DBUS_TIME_FORMAT_WITH_MILLISECONDS(DBUS_FORMAT_TIME_WITH_MS);

const QString YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID("yerith-erp-pgi-3.0");



YERITH_RUNTIME_VERIFIER::YERITH_RUNTIME_VERIFIER()
{
	QString systemYerithService = "yri.db-runtime.verif";


	com::yerith::rd::IYRIruntimeverification	*a_YRI_DB_RUNTIME_VERIF_instance_Paper_sample_1 =
			new com::yerith::rd::IYRIruntimeverification(systemYerithService,
														"/YERITH_QVGE_sample_PAPER_extended_version_PROPERY",
														QDBusConnection::systemBus(),
														this);

    _YRI_DB_RUNTIME_VERIF_instances.append(a_YRI_DB_RUNTIME_VERIF_instance_Paper_sample_1);


//	com::yerith::rd::IYRIruntimeverification	*a_YRI_DB_RUNTIME_VERIF_instance =
//			new com::yerith::rd::IYRIruntimeverification(systemYerithService,
//														"/yri_sd_runtime_verif_language_EXAMPLE_realcase",
//														QDBusConnection::systemBus(),
//														this);
//
//    _YRI_DB_RUNTIME_VERIF_instances.append(a_YRI_DB_RUNTIME_VERIF_instance);
//
//
//	com::yerith::rd::IYRIruntimeverification	*a_YRI_DB_RUNTIME_VERIF_instance_1 =
//			new com::yerith::rd::IYRIruntimeverification(systemYerithService,
//														"/YERITH_QVGE_sample_SAFETY_PROPERY_one_Recovery_SAMPLE",
//														QDBusConnection::systemBus(),
//														this);
//
//    _YRI_DB_RUNTIME_VERIF_instances.append(a_YRI_DB_RUNTIME_VERIF_instance_1);
//
//
//	com::yerith::rd::IYRIruntimeverification	*a_YRI_DB_RUNTIME_VERIF_instance_2 =
//			new com::yerith::rd::IYRIruntimeverification(systemYerithService,
//														"/YERITH_QVGE_sample_SAFETY_PROPERY_one",
//														QDBusConnection::systemBus(),
//														this);
//
//    _YRI_DB_RUNTIME_VERIF_instances.append(a_YRI_DB_RUNTIME_VERIF_instance_2);


	startTimer(3000);
}


void YERITH_RUNTIME_VERIFIER::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (_YRI_DB_RUNTIME_VERIF_instances.size() <= 0)
    {
        return ;
    }


    com::yerith::rd::IYRIruntimeverification *currrent_verification_logger_INSTANCE =
        _YRI_DB_RUNTIME_VERIF_instances.at(0);


    if (0 != currrent_verification_logger_INSTANCE &&
    	currrent_verification_logger_INSTANCE->isValid())
    {
        //qDebug() << "connected";
    }
    else
    {
    	//qDebug() << "disconnected";
    }
}


void YERITH_RUNTIME_VERIFIER::YRI_slot_refresh_DELETE_DB_MYSQL__CALLED(QString in0 /* = "" */,
   											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
    QString yr_in0 = QString("%1;%2")
                        .arg(in0,
                             YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID);


    com::yerith::rd::IYRIruntimeverification *currrent_verification_logger_INSTANCE = 0;

    for (uint k = 0; k < _YRI_DB_RUNTIME_VERIF_instances.size(); ++k)
    {
        currrent_verification_logger_INSTANCE = _YRI_DB_RUNTIME_VERIF_instances.at(k);

        if (0 != currrent_verification_logger_INSTANCE)
        {
            currrent_verification_logger_INSTANCE
                ->YRI_slot_refresh_DELETE_DB_MYSQL(yr_in0,
                                                  in1);
        }
    }


	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"DELETE",
						sql_table_name,
						YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID,
						"runtime_verif",
						QString::number(in1));

		if (YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YERITH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}


void YERITH_RUNTIME_VERIFIER::YRI_slot_refresh_INSERT_DB_MYSQL__CALLED(QString in0 /* = "" */,
 											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
    QString yr_in0 = QString("%1;%2")
                        .arg(in0,
                             YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID);


    com::yerith::rd::IYRIruntimeverification *currrent_verification_logger_INSTANCE = 0;

    for (uint k = 0; k < _YRI_DB_RUNTIME_VERIF_instances.size(); ++k)
    {
        currrent_verification_logger_INSTANCE = _YRI_DB_RUNTIME_VERIF_instances.at(k);

        if (0 != currrent_verification_logger_INSTANCE)
        {
            currrent_verification_logger_INSTANCE
                ->YRI_slot_refresh_INSERT_DB_MYSQL(yr_in0,
                                                  in1);
        }
    }


	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"INSERT",
						sql_table_name,
						YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID,
						"runtime_verif",
						QString::number(in1));

		if (YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YERITH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}


void YERITH_RUNTIME_VERIFIER::YRI_slot_refresh_SELECT_DB_MYSQL__CALLED(QString in0 /* = "" */,
   											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
    QString yr_in0 = QString("%1;%2")
                        .arg(in0,
                             YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID);


    com::yerith::rd::IYRIruntimeverification *currrent_verification_logger_INSTANCE = 0;

    for (uint k = 0; k < _YRI_DB_RUNTIME_VERIF_instances.size(); ++k)
    {
        currrent_verification_logger_INSTANCE = _YRI_DB_RUNTIME_VERIF_instances.at(k);

        if (0 != currrent_verification_logger_INSTANCE)
        {
            currrent_verification_logger_INSTANCE
                ->YRI_slot_refresh_SELECT_DB_MYSQL(yr_in0, in1);
        }
    }


	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"SELECT",
						sql_table_name,
						YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID,
						"runtime_verif",
						QString::number(in1));

		if (YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YERITH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}


void YERITH_RUNTIME_VERIFIER::YRI_slot_refresh_UPDATE_DB_MYSQL__CALLED(QString in0 /* = "" */,
   											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
    QString yr_in0 = QString("%1;%2")
                        .arg(in0,
                             YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID);


    com::yerith::rd::IYRIruntimeverification *currrent_verification_logger_INSTANCE = 0;

    for (uint k = 0; k < _YRI_DB_RUNTIME_VERIF_instances.size(); ++k)
    {
        currrent_verification_logger_INSTANCE = _YRI_DB_RUNTIME_VERIF_instances.at(k);

        if (0 != currrent_verification_logger_INSTANCE)
        {
            currrent_verification_logger_INSTANCE
                ->YRI_slot_refresh_UPDATE_DB_MYSQL(yr_in0,
                                                  in1);
        }
    }


	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"UPDATE",
						sql_table_name,
						YERITH_RUNTIME_VERIFIER::YERITH_ERP_PGI_dbus_verifier_tester_ID,
						"runtime_verif",
						QString::number(in1));

		if (YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YERITH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YERITH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}



