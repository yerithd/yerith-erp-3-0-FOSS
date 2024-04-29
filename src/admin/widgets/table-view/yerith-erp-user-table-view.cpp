/*
 * yerith-erp-user-table-view.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-user-table-view.hpp"


#include "src/yerith-erp-windows.hpp"


#include "src/utils/yerith-erp-utils.hpp"

#include "src/widgets/yerith-erp-qstandard-item.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>



void YerithERPUserTableView::
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
                if (YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerithDatabaseTableColumn::ROLE))
                {
                	anItem = new YerithQStandardItem(YerithUtils::_roleToUserViewString.value(qv.toInt()));
                }
                else if(YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerithDatabaseTableColumn::TITRE))
                {
                	anItem = new YerithQStandardItem(YerithUtils::_titreToUserViewString.value(qv.toInt()));
                }
                else if (YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerithDatabaseTableColumn::ID))
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
