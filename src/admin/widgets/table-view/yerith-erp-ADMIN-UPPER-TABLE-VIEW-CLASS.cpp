/*
 * yerith-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.cpp
 *
 *      Author: XAVIER NOUNDOU, DIPL.-INF., PH.D, P.ENG.
 */


#include "yerith-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp"


#include "src/yerith-erp-windows.hpp"


#include "src/utils/yerith-erp-utils.hpp"

#include "src/widgets/yerith-erp-qstandard-item.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>



void yerithERPAdminUpperTableViewClass::
		SAVE_YERITH_TABLEVIEW_header_COLUMN_order(YerithWindowsCommons &a_window)
{
    QStringList yerithTableView_model_raw_visible_headers =
    		getTableModelRawHeaders();

    const QStringList *a_visible_DB_columnname_string_List =
    		a_window.get_visible_DB_column_name_str_list();


    QHeaderView *q_header_view = verticalHeader();


    QString pageTableColumnOrder_STRING;


    if (0 != q_header_view 					  &&
    	0 != a_visible_DB_columnname_string_List)
    {
        for (uint i = 0;
        	 i < yerithTableView_model_raw_visible_headers.size();
        	 ++i)
        {
            const QString &headerColumnData =
            		yerithTableView_model_raw_visible_headers.at(i);

            if (!headerColumnData.isEmpty())
            {
                if (a_visible_DB_columnname_string_List->contains(headerColumnData))
                {
                    pageTableColumnOrder_STRING
						.append(QString("%1:%2;")
								 .arg(headerColumnData,
									  QString::number(q_header_view->visualIndex(i))));
                }
            }
        }
    }


    a_window.set_PARAMETER_TABLE_COLUMN_ORDER(pageTableColumnOrder_STRING);


    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        aUser->save_user_personal_PRINTING_PARAMETER_settings(&a_window);
    }
}


void yerithERPAdminUpperTableViewClass::
		lister_les_elements_du_tableau(YerithSqlTableModel &aSqlTableModel)
{
    _stdItemModel->_curSqlTableModel = &aSqlTableModel;

    emit signal_lister(aSqlTableModel);

    bool s = true;

    if (_needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING)
    {
        s = aSqlTableModel.select();
    }

    int rows = aSqlTableModel.rowCount();
    int columns = aSqlTableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerithUtils::createTableModelHeaders(aSqlTableModel,
                                         *_stdItemModel,
                                         _tableModelHeaders,
                                         _tableModelRawHeaders_IN_OUT,
										 _currentViewWindow);

    _stdItemModel->setColumnCount(_tableModelRawHeaders_IN_OUT.size());

    if (!s || (0 == _tableModelHeaders.size()) ||
    	(0 == _tableModelRawHeaders_IN_OUT.size()))
    {
        return;
    }

    QString curTableModelRawHdr;

    QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = aSqlTableModel.record(i).fieldName(k);

            if (!_tableModelRawHeaders_IN_OUT.contains(curTableModelRawHdr))
            {
                continue;
            }

            qv.setValue(aSqlTableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:
                anItem = new YerithQStandardItem(GET_NUM_STRING(qv.toUInt()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:
                if (!YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerithDatabaseTableColumn::ID))
                {
                    anItem = new YerithQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem = new YerithQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem = new YerithQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::ULongLong:
                anItem = new YerithQStandardItem(GET_NUM_STRING(qv.toULongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::LongLong:
                anItem = new YerithQStandardItem(GET_NUM_STRING(qv.toLongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Char:
                anItem = new YerithQStandardItem(QString(qv.toChar()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::String:
                tmpQvString.clear();
                tmpQvString.append(qv.toString());

                anItem = new YerithQStandardItem(tmpQvString, false);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Bool:
                anItem = new YerithQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Date:
                anItem = new YerithQStandardItem(DATE_TO_STRING(qv.toDate()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem = new YerithQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerithQStandardItem(YerithUtils::EMPTY_STRING);
                //qDebug() << "YerithTableView:::lister(): undecoded QVariant -> " << qv.type();
                break;
            }
        }
    }

    static YerithERPWindows *curAllWindows = YerithUtils::getAllWindows();

    YERITH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows,
                                               curAllWindows->_adminListerWindow)

    resizeColumnsToContents();

    curAllWindows->_adminListerWindow->handle_some_actions_tools_enabled();
}



