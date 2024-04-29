/*
 * yerith-erp-paiements-table-view.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "src/widgets/table-view/yerith-erp-paiements-table-view.hpp"

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


void YerithERPPaiementsTableView::lister_les_elements_du_tableau(YerithSqlTableModel &tableModel)
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


    if (!s || (0 == _tableModelHeaders.size()) ||
    	(0 == _tableModelRawHeaders_IN_OUT.size()))
    {
        return;
    }

    QString
    REFERENCE_user_view_string(QObject::tr
                               ("Référence (article ou service)"));

    uint
    index_of_reference_article_OU_service_header =
                    _tableModelRawHeaders_IN_OUT.
                    indexOf(YerithDatabaseTableColumn::REFERENCE);


    _tableModelHeaders.replace(index_of_reference_article_OU_service_header,
                               REFERENCE_user_view_string);


    QString
    compte_fournisseur_user_view_string;

    {
        compte_fournisseur_user_view_string
            =
                            YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                            (YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);

        uint
        index_of_compte_fournisseur_header =
                        _tableModelRawHeaders_IN_OUT.
                        indexOf(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);

        compte_fournisseur_user_view_string.
        append(QObject::tr(" (après)"));

        _tableModelHeaders.replace(index_of_compte_fournisseur_header,
                                   compte_fournisseur_user_view_string);
    }


    QString
    compte_client_PROGRAMME_DE_FIDELITE_CLIENTS_user_view_string;

    {
        compte_client_PROGRAMME_DE_FIDELITE_CLIENTS_user_view_string
            =
                            YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                            (YerithDatabaseTableColumn::
                             COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS);

        uint
        index_of_compte_client_PROGRAMME_DE_FIDELITE_CLIENTS_header =
                        _tableModelRawHeaders_IN_OUT.indexOf
                        (YerithDatabaseTableColumn::
                         COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS);

        compte_client_PROGRAMME_DE_FIDELITE_CLIENTS_user_view_string.append
        (QObject::tr(" (après)"));

        _tableModelHeaders.replace
        (index_of_compte_client_PROGRAMME_DE_FIDELITE_CLIENTS_header,
         compte_client_PROGRAMME_DE_FIDELITE_CLIENTS_user_view_string);
    }


    QString
    compte_client_user_view_string;

    {
        compte_client_user_view_string
            =
                            YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                            (YerithDatabaseTableColumn::COMPTE_CLIENT);

        uint
        index_of_compte_client_header =
                        _tableModelRawHeaders_IN_OUT.
                        indexOf(YerithDatabaseTableColumn::COMPTE_CLIENT);

        compte_client_user_view_string.append(QObject::tr(" (après)"));

        _tableModelHeaders.replace(index_of_compte_client_header,
                                   compte_client_user_view_string);

    }


//    QDEBUG_QSTRINGLIST_OUTPUT("_tableModelHeaders", _tableModelHeaders);


    for (unsigned i = 0; i < tableModel.columnCount(); ++i)
    {
        if (YerithUtils::isEqualCaseInsensitive
                (tableModel.record(0).fieldName(i),
                 YerithDatabaseTableColumn::REFERENCE))
        {
            _stdItemModel->setHeaderData(i,
                                         Qt::Horizontal,
                                         REFERENCE_user_view_string);
        }

        if (YerithUtils::isEqualCaseInsensitive
                (tableModel.record(0).fieldName(i),
                 YerithDatabaseTableColumn::COMPTE_FOURNISSEUR))
        {
            _stdItemModel->setHeaderData(i,
                                         Qt::Horizontal,
                                         compte_fournisseur_user_view_string);
        }

        if (YerithUtils::isEqualCaseInsensitive
                (tableModel.record(0).fieldName(i),
                 YerithDatabaseTableColumn::
                 COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS))
        {
            _stdItemModel->setHeaderData(i,
                                         Qt::Horizontal,
                                         compte_client_PROGRAMME_DE_FIDELITE_CLIENTS_user_view_string);
        }

        if (YerithUtils::isEqualCaseInsensitive
                (tableModel.record(0).fieldName(i),
                 YerithDatabaseTableColumn::COMPTE_CLIENT))
        {
            _stdItemModel->setHeaderData(i,
                                         Qt::Horizontal,
                                         compte_client_user_view_string);
        }
    }


    _stdItemModel->setColumnCount(_tableModelRawHeaders_IN_OUT.size());


    QString
    curTableModelRawHdr;

    QString
    tmpQvString;

    QStandardItem *
    anItem = 0;

    QVariant
    qv;

    double
    a_double_value = 0.0;

    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < columns; ++k)
        {
            a_double_value = 0.0;

            curTableModelRawHdr = tableModel.record(i).fieldName(k);

            if (!_tableModelRawHeaders_IN_OUT.contains(curTableModelRawHdr))
            {
                continue;
            }

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete
                anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:
                anItem =
                                new YerithQStandardItem(GET_NUM_STRING(qv.toUInt()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerithDatabaseTableColumn::
                                                        TYPE_DE_PAIEMENT))
                {
                    tmpQvString =
                                    YerithUtils::_typedepaiementToUserViewString.value(qv.
                                                                                       toInt
                                                                                       ());
                    anItem = new YerithQStandardItem(tmpQvString, false);
                }
                else if (!YerithUtils::isEqualCaseInsensitive
                         (curTableModelRawHdr, YerithDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerithQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    YERITH_SAVE_ID_TO_ROW_NUMBER_FOR_YERITH_TABLE_VIEW
                    (tmpQvString, qv.toInt(), i) anItem =
                                    new YerithQStandardItem(tmpQvString);
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:

                a_double_value = qv.toDouble();

                anItem =
                                new YerithQStandardItem(GET_DOUBLE_STRING(a_double_value));

                if (YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerithDatabaseTableColumn::
                                                        MONTANT_PAYE)
                        || YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                               YerithDatabaseTableColumn::
                                                               COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS)
                        || YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                               YerithDatabaseTableColumn::
                                                               COMPTE_CLIENT)
                        || YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                               YerithDatabaseTableColumn::
                                                               COMPTE_FOURNISSEUR))
                {
                    anItem->
                    setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
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
                                                         REFERENCE)
                        && !YerithUtils::
                        isEqualCaseInsensitive(curTableModelRawHdr,
                                               YerithDatabaseTableColumn::
                                               REFERENCE_RECU_PAIEMENT_CLIENT))
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
                                new YerithQStandardItem(BOOL_TO_STRING(qv.toBool()));
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

                if (a_double_value > 0)
                {
                    anItem->
                    setForeground(YerithUtils::YERITH_YERITHGREEN_COLOR);
                }
                else if (a_double_value < 0)
                {
                    anItem->setForeground(YerithUtils::YERITH_RED_COLOR);
                }
            }

        }
    }

    static YerithERPWindows *
    curAllWindows = YerithUtils::getAllWindows();

    YERITH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows,
                                               curAllWindows->_paiementsWindow)
    resizeColumnsToContents();

    curAllWindows->_paiementsWindow->handle_some_actions_tools_enabled();
}


void YerithERPPaiementsTableView::selectionChanged(const QItemSelection &
                                                   selected,
                                                   const QItemSelection &
                                                   deselected)
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
            curAllWindows->
            _paiementsWindow->getQModelIndex_dbID_from_MODEL_INDEX
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
