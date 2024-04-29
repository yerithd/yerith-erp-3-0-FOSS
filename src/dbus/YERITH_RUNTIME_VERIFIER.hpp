/*
 * YERITH_RUNTIME_VERIFIER.hpp
 *
 *      Author: PROF. DR.-ING. DIPL.-INF. Xavier Noundou
 */

#ifndef SRC_DBUS_YERITH_RUNTIME_VERIFIER_HPP_
#define SRC_DBUS_YERITH_RUNTIME_VERIFIER_HPP_


#include "src/IYRIruntimeverificationAdaptor_interface.h"


#include <QtCore/QObject>


using namespace com::yerith::rd;


class YERITH_RUNTIME_VERIFIER : public QObject
{
	Q_OBJECT

public:

	YERITH_RUNTIME_VERIFIER();

	virtual inline ~YERITH_RUNTIME_VERIFIER()
	{
	}


protected:

    void timerEvent(QTimerEvent *event);


public Q_SLOTS:

	void YRI_slot_refresh_DELETE_DB_MYSQL__CALLED(QString SQL_modified_CHANGED_table = "",
	   											  uint qty_DATA_modified_CHANGED = 1);

	void YRI_slot_refresh_INSERT_DB_MYSQL__CALLED(QString SQL_modified_CHANGED_table = "",
	 											  uint qty_DATA_modified_CHANGED = 1);

	void YRI_slot_refresh_SELECT_DB_MYSQL__CALLED(QString SQL_modified_CHANGED_table = "",
	   											  uint qty_DATA_modified_CHANGED = 1);

	void YRI_slot_refresh_UPDATE_DB_MYSQL__CALLED(QString SQL_modified_CHANGED_table = "",
	   											  uint qty_DATA_modified_CHANGED = 1);


public:

	static uint _MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT;

	static uint _current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;

	static QStringList _LAST_SIGNALS_source_target_strings;

	static const QString DBUS_TIME_FORMAT_WITH_MILLISECONDS;

	static const QString YERITH_ERP_PGI_dbus_verifier_tester_ID;


private:

    QList<com::yerith::rd::IYRIruntimeverification *> _YRI_DB_RUNTIME_VERIF_instances;
};


#define DBUS_FORMAT_TIME_WITH_MS        "HH:mm:ss:zzz"

#define DBUS_CURRENT_TIME_WITH_MILLISECONDS QTime::currentTime().toString(YERITH_RUNTIME_VERIFIER::DBUS_TIME_FORMAT_WITH_MILLISECONDS)


#endif /* SRC_DBUS_YERITH_RUNTIME_VERIFIER_HPP_ */
