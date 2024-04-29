/*
 * yerith-erp-ventes-table-view.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "src/widgets/table-view/yerith-erp-ventes-table-view.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/widgets/yerith-erp-qstandard-item.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>


void YerithERPVentesTableView::lister_les_elements_du_tableau(YerithSqlTableModel &tableModel)
{
    _stdItemModel->_curSqlTableModel = &tableModel;

    emit signal_lister(tableModel);

    bool s = true;

    if (_needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING)
    {
        s = tableModel.select();
    }

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerithUtils::createTableModelHeaders(tableModel,
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

    YerithPOSUser *aUser = 0;

    if (0 != _allWindows)
    {
        aUser = _allWindows->getUser();
    }


    QString curTableModelRawHdr;

    QString tmpQvString;

    QStandardItem *anItem = 0;

    QVariant qv;

    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModel.record(i).fieldName(k);

            if (!_tableModelRawHeaders_IN_OUT.contains(curTableModelRawHdr))
            {
                continue;
            }

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:
                anItem =
                                new YerithQStandardItem(GET_NUM_STRING(qv.toUInt()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerithDatabaseTableColumn::
                                                         TYPE_DE_VENTE)
                        && !YerithUtils::
                        isEqualCaseInsensitive(curTableModelRawHdr,
                                               YerithDatabaseTableColumn::ID)
                        && !YerithUtils::
                        isEqualCaseInsensitive(curTableModelRawHdr,
                                               YerithDatabaseTableColumn::
                                               CLIENTS_ID)
                        && !YerithUtils::
                        isEqualCaseInsensitive(curTableModelRawHdr,
                                               YerithDatabaseTableColumn::
                                               STOCKS_ID))
                {
                    anItem =
                                    new YerithQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else if (YerithUtils::isEqualCaseInsensitive
                         (curTableModelRawHdr,
                          YerithDatabaseTableColumn::TYPE_DE_VENTE))
                {
                    tmpQvString =
                                    YerithUtils::_typedeventeToUserViewString.value(qv.toInt
                                                                                    ());
                    anItem = new YerithQStandardItem(tmpQvString, false);
                }
                else if (YerithUtils::isEqualCaseInsensitive
                         (curTableModelRawHdr,
                          YerithDatabaseTableColumn::CLIENTS_ID)
                         ||
                         YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                             YerithDatabaseTableColumn::
                                                             STOCKS_ID))
                {
                    anItem =
                                    new YerithQStandardItem(QString::number(qv.toInt()));
                }
                else if (YerithUtils::isEqualCaseInsensitive
                         (curTableModelRawHdr, YerithDatabaseTableColumn::ID))
                {
                    YERITH_SAVE_ID_TO_ROW_NUMBER_FOR_YERITH_TABLE_VIEW
                    (tmpQvString, qv.toInt(), i) anItem =
                                    new YerithQStandardItem(tmpQvString);
                }
                else		//I have this just as a safe alternative. IT SHALL NEVER BE EXECUTED !
                {
                    anItem =
                                    new YerithQStandardItem(GET_NUM_STRING(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:

                if (!YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerithDatabaseTableColumn::
                                                         MARGE_BENEFICIAIRE))
                {
                    anItem =
                                    new
                    YerithQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
                }
                else
                {
                    anItem = new YerithQStandardItem(GET_DOUBLE_STRING(0.0));

                    if (0 != aUser && aUser->isManager())
                    {
                        YERITH_DELETE_FREE_POINTER_NOW(anItem);

                        anItem =
                                        new
                        YerithQStandardItem(GET_DOUBLE_STRING
                                            (qv.toDouble()));
                    }
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::ULongLong:
                anItem =
                                new YerithQStandardItem(GET_NUM_STRING(qv.toULongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::LongLong:
                anItem =
                                new YerithQStandardItem(GET_NUM_STRING(qv.toLongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Char:
                anItem = new YerithQStandardItem(QString(qv.toChar()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::String:
                tmpQvString.clear();
                tmpQvString.append(qv.toString());

                if (!YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerithDatabaseTableColumn::
                                                         REFERENCE))
                {
                    anItem = new YerithQStandardItem(tmpQvString, false);
                }
                else
                {
                    anItem = new YerithQStandardItem(tmpQvString);
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Bool:
                anItem =
                                new YerithQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE :
                                                        BOOLEAN_STRING_FALSE);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Date:
                anItem =
                                new YerithQStandardItem(DATE_TO_STRING(qv.toDate()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem =
                                new YerithQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerithQStandardItem(YerithUtils::EMPTY_STRING);
                _stdItemModel->setItem(i, k, anItem);
                //qDebug() << "YerithTableView::lister(): undecoded QVariant -> " << qv.type();
                break;
            }

            if (anItem)
            {
                anItem->setForeground(Qt::white);
            }
        }
    }

    static YerithERPWindows *curAllWindows = YerithUtils::getAllWindows();

    YERITH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows,
                                               curAllWindows->_ventesWindow)
    resizeColumnsToContents();

    curAllWindows->_ventesWindow->handle_some_actions_tools_enabled();
}


void YerithERPVentesTableView::selectionChanged(const QItemSelection &selected,
                                                const QItemSelection &deselected)
{
    static YerithERPWindows *curAllWindows = YerithUtils::getAllWindows();

    _MAP_lastSelected_Row__TO__DB_ID.clear();

    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();

    QString db_ID_in_out;

    if (selectedIndexes.size() > 0)
    {
        uint last_Row_ID = selectedIndexes.size() - 1;

        for (uint j = 0; j < selectedIndexes.size(); ++j)
        {
            curAllWindows->_ventesWindow->getQModelIndex_dbID_from_MODEL_INDEX
            (selectedIndexes.at(j), db_ID_in_out);

            _MAP_lastSelected_Row__TO__DB_ID.
            insert(QString::number(selectedIndexes.at(j).row()),
                   db_ID_in_out);

            if (last_Row_ID == j)
            {
                _lastSelected_Row__ID = selectedIndexes.at(j).row();
            }
        }
    }

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    clearFocus();
}
