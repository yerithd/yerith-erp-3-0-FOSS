/*
 * yerith-erp-database-table-import-info.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_DATABASE_TABLE_INFO_HPP_
#define YERITH_ERP_DATABASE_TABLE_INFO_HPP_


#include "src/utils/yerith-erp-utils.hpp"


#include <QtCore/QDebug>


class YerithERPDatabaseTableColumnInfo
{
public:

    inline YerithERPDatabaseTableColumnInfo(const QString &aColumnName,
                                            const QString &
                                            aColumnType):_tableColumnName
        (aColumnName), _tableColumntype(aColumnType)
    {
    }

    inline void toString()
    {
        qDebug() << QString("++ %1 has type %2").arg(_tableColumnName,
                                                     _tableColumntype);
    }

    inline const QString &getColumnName() const
    {
        return _tableColumnName;
    }

    inline const QString &getColumnType() const
    {
        return _tableColumntype;
    }

private:

    QString _tableColumnName;

    QString _tableColumntype;
};


#endif /* YERITH_ERP_DATABASE_TABLE_INFO_HPP_ */
