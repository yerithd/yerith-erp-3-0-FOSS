/*
 * yerith-erp-sqltable-model.cpp
 *
  *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-sqltable-model.hpp"


#include "src/utils/yerith-erp-utils.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-logger.hpp"


#ifdef YERITH_SERVER

	#include "src/yerith-erp-windows.hpp"

	#include "src/dbus/YERITH_RUNTIME_VERIFIER.hpp"

#endif


#include <QtCore/QDebug>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QMainWindow>

#include <QtWidgets/QMessageBox>


YerithSqlTableModel::YerithSqlTableModel():_logger(0), _fromQSqlQuery(false)
{
    _logger = new YerithLogger("YerithSqlTableModel");

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerithSqlTableModel::YerithSqlTableModel(const QSqlQuery &aQSqlQuery):
    _logger(0),
    _fromQSqlQuery(true)
{
    _logger = new YerithLogger("YerithSqlTableModel");

    yerithSetQuery(aQSqlQuery);

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerithSqlTableModel::YerithSqlTableModel(const QString &sqlTableName,
                                         bool setTableName /* = true */)
    :
    _sqlTableName(sqlTableName),
    _logger(0),
    _fromQSqlQuery(false)
{
    _logger = new YerithLogger("YerithSqlTableModel");

    _logger->log("YerithSqlTableModel",
                 QString
                 ("table name: %1, edit strategy: QSqlTableModel::OnFieldChange").
                 arg(sqlTableName));

    if (setTableName)
    {
        setTable(sqlTableName);
    }

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerithSqlTableModel::~YerithSqlTableModel()
{
    YERITH_DELETE_FREE_POINTER_NOW(_logger);
}


int YerithSqlTableModel::yerith_RESET_specify_filter_FROM_SELECT_STATEMENT(const
                                                                           QString
                                                                           &
                                                                           filter)
{
    QString curYerithSelectStatement(yerithSelectStatement());

    curYerithSelectStatement.remove(filter);

//      QDEBUG_STRING_OUTPUT_2("RESET filter", curYerithSelectStatement);

    int queryResultSize = yerithSetQueryRowCount(curYerithSelectStatement);

    return queryResultSize;
}


int YerithSqlTableModel::yerith_specify_filter_FROM_SELECT_STATEMENT(const
                                                                     QString &
                                                                     filter)
{
    QString curYerithSelectStatement(yerithSelectStatement());

    curYerithSelectStatement.append(QString(" %1").arg(filter));

//      QDEBUG_STRING_OUTPUT_2("filter", curYerithSelectStatement);

    int queryResultSize = yerithSetQueryRowCount(curYerithSelectStatement);

    return queryResultSize;
}


int YerithSqlTableModel::Is_SearchQSqlTable(QString tableColumn,
                                            QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_IS_STMT(tableColumn, searchTerm));

    yerithSetFilter_WITH_where_clause(queryFilter);

    int filterRowCount = rowCount();

    _logger->log("Is_SearchQSqlTable",
                 QString
                 ("(table: %1, column: %2) | queryFilter (%3) | row count: %4").
                 arg(_sqlTableName, tableColumn, queryFilter,
                     QString::number(filterRowCount)));

    resetFilter("src/utils/yerith-erp-sqltable-model.cpp", 135);

    return filterRowCount;
}


int YerithSqlTableModel::_Is_SearchQSqlTable(enum YerithSqlQueryType,
                                             QString tableColumn,
                                             QStringList &searchTermsList)
{
    QString queryTerm;

    const int size = searchTermsList.size();
    const int max = size - 1;

    for (int k = 0; k < size; ++k)
    {
        if (k < max)
        {
            queryTerm.append(GENERATE_SQL_IS_STMT
                             (tableColumn,
                              searchTermsList.at(k))).append(" AND ");
        }
        else
        {
            queryTerm.append(GENERATE_SQL_IS_STMT
                             (tableColumn, searchTermsList.at(k)));
        }
    }

    yerithSetFilter_WITH_where_clause(queryTerm);

    int filterRowCount = rowCount();

    resetFilter("src/utils/yerith-erp-sqltable-model.cpp");

    return filterRowCount;
}

int YerithSqlTableModel::Like_SearchQSqlTable(QString tableColumn,
                                              QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_LIKE_STMT(tableColumn, searchTerm));

    yerithSetFilter_WITH_where_clause(queryFilter);

    int filterRowCount = rowCount();

    _logger->log("Like_SearchQSqlTable",
                 QString
                 ("(table: %1, column: %2) | queryFilter (%3) | row count: %4").
                 arg(_sqlTableName).arg(tableColumn).arg(queryFilter).
                 arg(filterRowCount));

    /*qDebug() <<
       QString("YerithSqlTableModel | Like_SearchQSqlTable (table: %1, column: %2) \n\t\t| queryFilter (%3) | row count: %4")
       .arg(_sqlTableName)
       .arg(tableColumn)
       .arg(queryFilter)
       .arg(filterRowCount); */

    resetFilter("src/utils/yerith-erp-sqltable-model.cpp");

    return filterRowCount;
}

int YerithSqlTableModel::_Like_SearchQSqlTable(enum YerithSqlQueryType,
                                               QString tableColumn,
                                               QStringList &searchTermsList)
{
    QString queryTerm;

    const int size = searchTermsList.size();
    const int max = size - 1;

    for (int k = 0; k < size; ++k)
    {
        if (k < max)
        {
            queryTerm.append(GENERATE_SQL_LIKE_STMT
                             (tableColumn,
                              searchTermsList.at(k))).append(" AND ");
        }
        else
        {
            queryTerm.append(GENERATE_SQL_LIKE_STMT
                             (tableColumn, searchTermsList.at(k)));
        }
    }

    yerithSetFilter_WITH_where_clause(queryTerm);

    int filterRowCount = rowCount();

    resetFilter("src/utils/yerith-erp-sqltable-model.cpp");

    return filterRowCount;
}

bool YerithSqlTableModel::insertNewRecord(QSqlRecord  &record,
                                          QMainWindow *parent /* = 0 */,
					    				  QString 	  cpp_file /* = "" */,
										  int	 	  cpp_LINE_nr /* = -1 */,
										  QString 	  an_INSERTED_VALUE /* = YerithUtils::EMPTY_STRING */)
{
    _logger->log("insertNewRecord",
                 QString("Table name: %1")
				 	 .arg(sqlTableName()));

    resetFilter("src/utils/yerith-erp-sqltable-model.cpp", 241);

    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    bool success = insertRecord(-1, record);

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    //qDebug() << QString("YerithSqlTableModel::insertNewRecord, success: %1")
    //                          .arg(BOOL_TO_STRING(success));

    if (success)
    {
#ifdef YERITH_SERVER

    	YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

    	dbusServer->YRI_slot_refresh_INSERT_DB_MYSQL__CALLED
						(QString("%1.%2;%3;%4")
							.arg(_sqlTableName,
								 an_INSERTED_VALUE,
    							 cpp_file,
    							 QString::number(cpp_LINE_nr)));


#endif //YERITH_SERVER
    }
    else
    {
        QString retMsg =
        	QObject::tr("Une erreur (base de données) est survenue pendant l'insertion dans le tableau %1 !\n")
                   .arg(sqlTableName());

        retMsg.append(QObject::tr("Contactez l'administrateur de Yerith-pgi-3.0 !"));

        if (0 != parent)
        {
            QMessageBox::warning(parent,
                                 QObject::tr("sqltablemodel - insertNewRecord"), retMsg);
        }

        qDebug() << QString("%1 - reason for failing: %2")
						.arg(sqlTableName(),
                             lastError().text());

        _logger->log("insertNewRecord",
                     QString("reason for failing: %1")
					 	 .arg(lastError().text()));
    }

    return success;
}


//bool YerithSqlTableModel::Record(QSqlRecord &record)
//{
//    _logger->log("updateRecord",
//                 QString("Table name: %1").arg(sqlTableName()));
//
//    bool success = false;
//
//    QString aCurFieldValue;
//
//    QString dbFieldIDValue;
//
//    QString aCurFieldName;
//
//    QString SQL_UPDATE_STRING_COMMAND = QString("UPDATE %1 SET ")
//                                                                              .arg(_sqlTableName);
//
//    for (int k = 0; k < record.count(); ++k)
//    {
//      aCurFieldName.clear();
//
//      aCurFieldName.append(record.fieldName(k));
//
//      if (!YerithUtils::isEqualCaseInsensitive(YerithDatabaseTableColumn::ID, aCurFieldName))
//      {
//              if (!aCurFieldName.contains("date_"))
//              {
//                      aCurFieldValue = YerithUtils::get_text(record.value(aCurFieldName));
//
//              SQL_UPDATE_STRING_COMMAND.append(QString("%1='%2', ")
//                                                                                      .arg(aCurFieldName,
//                                                                                               aCurFieldValue));
//              }
//              else
//              {
//                      aCurFieldValue = DATE_TO_DB_FORMAT_STRING(record.value(aCurFieldName).toDate());
//
////                            QDEBUG_STRING_OUTPUT_2("aCurFieldValue (DATE_TO_DB_FORMAT_STRING)", aCurFieldValue);
//
//                      if (!aCurFieldValue.isEmpty())
//                      {
//                              SQL_UPDATE_STRING_COMMAND.append(QString("%1='%2', ")
//                                                                                                      .arg(aCurFieldName,
//                                                                                                               aCurFieldValue));
//                      }
//              }
//      }
//      else
//      {
//              dbFieldIDValue = YerithUtils::get_text(record.value(aCurFieldName));
//      }
//    }
//
//    int len = SQL_UPDATE_STRING_COMMAND.length();
//
//    SQL_UPDATE_STRING_COMMAND.remove(len - 2, 2);
//
//      SQL_UPDATE_STRING_COMMAND.append(QString(" WHERE %1 = '%2';")
//                                                                              .arg(YerithDatabaseTableColumn::ID,
//                                                                                       dbFieldIDValue));
//
////    QDEBUG_STRING_OUTPUT_2("SQL_UPDATE_STRING_COMMAND", SQL_UPDATE_STRING_COMMAND);
//
//    success = YerithUtils::execQuery(SQL_UPDATE_STRING_COMMAND);
//
//    return success;
//}


bool YerithSqlTableModel::updateRecord(unsigned    row,
									   QSqlRecord &record,
					 				   QString 	   cpp_file /* = "" */,
									   int	  	   cpp_LINE_nr /* = -1 */,
									   QString 	   an_INSERTED_VALUE /* = "" */)
{
    _logger->log("updateRecord",
                 QString("Table name: %1")
				 	 .arg(sqlTableName()));

    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    bool success = setRecord(row, record);

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    if (!success)
    {
        _logger->log("updateRecord",
                     QString("updating row %1. reason for failing: %2")
					 	 .arg(QString::number(row),
					 		  lastError().text()));
    }
    else
    {

    #ifdef YERITH_SERVER

    	YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

    	dbusServer->YRI_slot_refresh_UPDATE_DB_MYSQL__CALLED(QString("%1.%2;%3;%4")
    															.arg(_sqlTableName,
    																 an_INSERTED_VALUE,
																	 cpp_file,
																	 QString::number(cpp_LINE_nr)));

    #endif //YERITH_SERVER

        _logger->log("updateRecord",
                     QString("updated row %1 with success")
					 	 .arg(row));
    }

    return success;
}


void YerithSqlTableModel::resetFilter(QString	cpp_file 	/* = "" */,
 	  	 	 	 	 	 	 	 	  int	  	cpp_LINE_nr /* = -1 */)
{
    yerithSetFilter_WITH_where_clause(YerithUtils::EMPTY_STRING);

    bool ret_SELECT_VALUE = select();

#ifdef YERITH_SERVER

    if (true == ret_SELECT_VALUE)
    {
    	YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

    	dbusServer->YRI_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
																.arg(_sqlTableName,
																     cpp_file,
																	 QString::number(cpp_LINE_nr)));
    }

#endif //YERITH_SERVER

}


int YerithSqlTableModel::easySelect(QString cpp_file 	 /* = "" */,
	  	  	  	  	  	  	  	    int	  	cpp_LINE_nr /* = -1 */)
{
    bool success = select();

    if (!success)
    {
        return -1;
    }

#ifdef YERITH_SERVER

    YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

	dbusServer->YRI_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
															.arg(_sqlTableName,
															     cpp_file,
																 QString::number(cpp_LINE_nr)));

#endif //YERITH_SERVER


    return rowCount();
}


bool YerithSqlTableModel::yerithSetSort(int 			column,
										Qt::SortOrder 	order,
										QString 		cpp_file /* = "" */,
										int	  	 		cpp_LINE_nr /* = -1 */)
{
    setSort(column, order);

    bool ret_SELECT_VALUE = select();

#ifdef YERITH_SERVER

    if (true == ret_SELECT_VALUE)
    {
    	YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

    	dbusServer->YRI_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
    															.arg(_sqlTableName,
    															     cpp_file,
    																 QString::number(cpp_LINE_nr)));
    }

#endif //YERITH_SERVER

    return ret_SELECT_VALUE;
}

int YerithSqlTableModel::yerithSetQueryRowCount(const QString &STRING_aSqlQuery)
{
    QSqlQueryModel::setQuery(STRING_aSqlQuery);

    if (lastError().isValid())
    {
        qDebug() << "++ YerithSqlTableModel::yerithSetQuery(QString): \n\t"
                 << STRING_aSqlQuery << "\n" << lastError();

        return -1;
    }

    int queryResultRowCount = QSqlQueryModel::rowCount();

    return queryResultRowCount;
}


bool YerithSqlTableModel::yerithSetQuery(const QString &aSqlQuery)
{
    QSqlQueryModel::setQuery(aSqlQuery);

    if (lastError().isValid())
    {
        qDebug() << "++ YerithSqlTableModel::yerithSetQuery(QString): \n\t"
                 << aSqlQuery << "\n" << lastError();

        return false;
    }

    return true;
}
