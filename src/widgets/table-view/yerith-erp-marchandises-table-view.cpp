/*
 * yerith-erp-marchandises-table-view.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-marchandises-table-view.hpp"

#include "src/yerith-erp-windows.hpp"

#ifdef YERITH_SERVER

	#include "src/dbus/YERITH_RUNTIME_VERIFIER.hpp"

#endif

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


void YerithERPMarchandisesTableView::lister_les_elements_du_tableau(YerithSqlTableModel &tableModel)
{
    _stdItemModel->_curSqlTableModel = &tableModel;

    YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow->
    _qteTotaleDarticlesEnStock = 0.0;

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

//    qDebug() << "++  rows: " << rows;

    if (tableModel.isFromQSqlQuery() && rows <= 0)
    {
        _stdItemModel->yerithPOSClear();

        //qDebug() << "++  yerithPOSClear";
    }

    YerithUtils::createTableModelHeaders(tableModel,
                                         *_stdItemModel,
                                         _tableModelHeaders,
                                         _tableModelRawHeaders_IN_OUT,
										 _currentViewWindow);

    _stdItemModel->setColumnCount(_tableModelRawHeaders_IN_OUT.size());

    if (!s || (0 == _tableModelHeaders.size())
            || (0 == _tableModelRawHeaders_IN_OUT.size()))
    {
        return;
    }

    QString
    curTableModelRawHdr;

    QString
    tmpQvString;

    QStandardItem *
    anItem = 0;

    QVariant
    qv;

    int
    querySize = 0;

    QString
    nom_departement_produit_str;
    QString
    designation_str;
    QString
    categorie_str;

    QStringList
    aStringList;

    QString
    sqlSearchStockTableQueryStr;
    QSqlQuery
    sqlSearchStockTableQuery;

    QSqlRecord
    record;

    QVariant
    nom_department_produit;
    QVariant
    designation;
    QVariant
    categorie;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        nom_department_produit =
                        record.value(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

        designation = record.value(YerithDatabaseTableColumn::DESIGNATION);

        categorie = record.value(YerithDatabaseTableColumn::CATEGORIE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = _tableModelRawHeaders_IN_OUT.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete
                anItem;
            }

            anItem = new YerithQStandardItem;

            switch (qv.type())
            {
            case QVariant::UInt:
                anItem =
                                new YerithQStandardItem(GET_NUM_STRING(qv.toUInt()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerithUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerithDatabaseTableColumn::ID))
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
                anItem =
                                new YerithQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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

            if (0 != anItem)
            {
                sqlSearchStockTableQuery.clear();

                nom_departement_produit_str =
                                nom_department_produit.toString();
                designation_str = designation.toString();
                categorie_str = categorie.toString();

                sqlSearchStockTableQueryStr =
                                QString
                                ("SELECT * FROM %1 WHERE %2='%3' AND %4='%5' AND %6='%7'").arg
                                (YerithDatabase::STOCKS,
                                 YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                                 nom_departement_produit_str,
                                 YerithDatabaseTableColumn::CATEGORIE, categorie_str,
                                 YerithDatabaseTableColumn::DESIGNATION, designation_str);

                //                                      QDEBUG_STRING_OUTPUT_2("sqlSearchStockTableQueryStr", sqlSearchStockTableQueryStr);

                querySize = YerithUtils::execQuery(sqlSearchStockTableQuery,
                                                   sqlSearchStockTableQueryStr);

                //                                      QDEBUG_STRING_OUTPUT_2_N("querySize", querySize);

                /**
                	 * WE USE THIS INFORMATION ABOUT PRODUCT NAME AND
                	 * CATEGORY TO FILTER EMPTY MERCHANDISE OUT !
                	 *
                	 * !!! DO NOT MODIFY IT !!!
                	 */
                if (querySize <= 0)
                {
                    anItem->setForeground
                    (YerithUtils::
                     YERITH_WHITE_COLOR_FOR_EMPTY_MERCHANDISE_FILTERING);

                    anItem->setAccessibleText(QString("%1|%2|%3").arg
                                              (nom_departement_produit_str,
                                               categorie_str,
                                               designation_str));
                }
                else
                {
                    anItem->setForeground(YerithUtils::YERITH_BLUE_COLOR);
                }
            }			//if (0 != item)
        }
    }


    QItemSelectionModel *
    a_selection_model = selectionModel();

    if (0 != a_selection_model)
    {
        selectionChanged(a_selection_model->selection(),
                         a_selection_model->selection());
    }


    static YerithERPWindows *
    curAllWindows = YerithUtils::getAllWindows();

    YERITH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows,
                                               curAllWindows->
                                               _marchandisesWindow)
    resizeColumnsToContents();

    curAllWindows->_marchandisesWindow->handle_some_actions_tools_enabled();
}


void YerithERPMarchandisesTableView::startEditingModeSelection()
{
    int lastSelectRecord_ROW_NUMBER =
                    YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER->
                    _marchandisesWindow->get_INT_last_selected_row_number();

    QStandardItem *anItem = _stdItemModel
    							->item(lastSelectRecord_ROW_NUMBER,
                                       YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
									   	   ->_marchandisesWindow
										   	   ->getDBTableFieldColumnIndex
                                                	(YerithDatabaseTableColumn::REFERENCE));

    anItem->setForeground(YerithUtils::YERITH_BLACK_COLOR);
    anItem->setBackground(Qt::white);


    anItem = _stdItemModel
    			->item(lastSelectRecord_ROW_NUMBER,
                       YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
					   	   ->_marchandisesWindow
						   	   ->getDBTableFieldColumnIndex
							   	   (YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT));

    anItem->setForeground(YerithUtils::YERITH_BLACK_COLOR);
    anItem->setBackground(Qt::white);


    anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
                                 YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
								 	 ->_marchandisesWindow
									 	 ->getDBTableFieldColumnIndex
										 	 (YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT));

    anItem->setForeground(YerithUtils::YERITH_BLACK_COLOR);
    anItem->setBackground(Qt::white);


    anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
                                 YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
								 	 ->_marchandisesWindow
									 	 ->getDBTableFieldColumnIndex
										 	 (YerithDatabaseTableColumn::DESCRIPTION_PRODUIT));

    anItem->setForeground(YerithUtils::YERITH_BLACK_COLOR);
    anItem->setBackground(Qt::white);


    setEditTriggers(QAbstractItemView::SelectedClicked |
                    QAbstractItemView::DoubleClicked);

    setWriteEnabled(true);

    QObject::connect(_stdItemModel,
                     SIGNAL(dataChanged (const QModelIndex &, const QModelIndex &)),
					 this,
                     SLOT(dataChanged (const QModelIndex &, const QModelIndex &)));
}


void YerithERPMarchandisesTableView::dataChanged(const QModelIndex &index,
                                                 const QModelIndex &bottomRight,
                                                 const QVector < int > &roles
                                                 /*= QVector<int>()*/
                                                )
{
//      QDEBUG_STRING_OUTPUT_2_N("bottomRight.column()", bottomRight.column());

    if (index != bottomRight)
    {
        return;
    }

    if (!_writeEnabled)
    {
        return;
    }

    if (0 == _stdItemModel->item(index.row(), 0))
    {
        return;
    }

    QString curIDText(_stdItemModel->item(index.row(), 0)->text());

    QString columnHeaderText_VISIBLE(_tableModelHeaders.at(index.column()));

    QString
    columnHeaderText_NATIVE(_tableModelRawHeaders_IN_OUT.at
                            (index.column()));

    if (!YerithUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
                                             YerithDatabaseTableColumn::REFERENCE)
            && !YerithUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
                                                    YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT)
			&& !YerithUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
										            YerithDatabaseTableColumn::DESCRIPTION_PRODUIT)
            && !YerithUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
                                                    YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT))
    {
        YerithQMessageBox::information
        (YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
         QObject::tr("modification non possible"),
         QObject::tr("modification non possible de la colone '%1' !")
		 	 .arg(columnHeaderText_VISIBLE));

        return;
    }


    QString cellTextData;

    QString categorie_text;

    QString designationText;

    QString
    REAL_DB_ID_NAME_marchandiseTableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.
     key(columnHeaderText_VISIBLE));

    bool success = false;

    QSqlQuery mySqlQuery;

    QString prevValueDataString;

    QVariant qvIndexData = index.data();

    cellTextData = YerithUtils::get_text(qvIndexData);

    QString
    strSelectPreviousValueQuery(QString
                                ("SELECT %1, %2, %3 FROM %4 WHERE %5 = '%6'").
                                arg
                                (REAL_DB_ID_NAME_marchandiseTableColumnProperty,
                                 YerithDatabaseTableColumn::DESIGNATION,
                                 YerithDatabaseTableColumn::CATEGORIE,
                                 *_tableName, YerithDatabaseTableColumn::ID,
                                 curIDText));

    QString
    strUpdateMarchandisesTableQuery(QString
                                    ("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'").
                                    arg(*_tableName,
                                        REAL_DB_ID_NAME_marchandiseTableColumnProperty,
                                        cellTextData,
                                        YerithDatabaseTableColumn::ID,
                                        curIDText));


    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    int mySqlQuerySize =
                    YerithUtils::execQuery(mySqlQuery, strSelectPreviousValueQuery);

    if (mySqlQuerySize > 0)
    {
        mySqlQuery.next();

        prevValueDataString = mySqlQuery.value(0).toString();

        designationText = mySqlQuery.value(1).toString();

        categorie_text = mySqlQuery.value(2).toString();

        mySqlQuery.clear();

#ifdef YERITH_SERVER

	YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

	dbusServer->YRI_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
															.arg(*_tableName,
																 "YerithERPMarchandisesTableView::dataChanged",
																 QString::number(-1)));

#endif //YERITH_SERVER
    }

    //                  QDEBUG_STRING_OUTPUT_2(QString::number(mySqlQuerySize), strUpdateMarchandisesTableQuery);

    success = YerithUtils::execQuery(strUpdateMarchandisesTableQuery);


    if (success &&
            YerithUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
                                                YerithDatabaseTableColumn::DESCRIPTION_PRODUIT))
    {
    	QString update_PRODUC_description_STOCKS_TABLE_QUERY =
    			QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
					.arg(YerithDatabase::STOCKS,
						 YerithDatabaseTableColumn::DESCRIPTION_PRODUIT,
						 cellTextData,
						 YerithDatabaseTableColumn::DESIGNATION,
						 designationText);

    	//QDEBUG_STRING_OUTPUT_2("update_PRODUC_description_STOCKS_TABLE_QUERY",
    	//						update_PRODUC_description_STOCKS_TABLE_QUERY);

    	bool success_UPDATE_STOCK_description_PRODUIT =
    			YerithUtils::execQuery(update_PRODUC_description_STOCKS_TABLE_QUERY);
    }


    if (success &&
            YerithUtils::isEqualCaseInsensitive(columnHeaderText_NATIVE,
                                                YerithDatabaseTableColumn::REFERENCE))
    {

#ifdef YERITH_SERVER

	YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

	dbusServer->YRI_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
															.arg(*_tableName,
																 "YerithERPMarchandisesTableView::dataChanged",
																 QString::number(-1)));

#endif //YERITH_SERVER

        QStringList allToUpdateTables;

        allToUpdateTables << YerithDatabase::PAIEMENTS
                          << YerithDatabase::STOCKS
                          << YerithDatabase::STOCKS_SORTIES
                          << YerithDatabase::STOCKS_VENDU
						  << YerithDatabase::ACHATS;

        QString strUpdateTableQuery;

        for (uint i = 0; i < allToUpdateTables.size(); ++i)
        {
            strUpdateTableQuery.clear();

            if (0 != i)
            {
                strUpdateTableQuery.append(QString
                                           ("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5' AND %6='%7'").
                                           arg(allToUpdateTables.at(i),
                                               YerithDatabaseTableColumn::REFERENCE,
                                               cellTextData,
                                               YerithDatabaseTableColumn::DESIGNATION,
                                               designationText,
                                               YerithDatabaseTableColumn::CATEGORIE,
                                               categorie_text));
            }
            else
            {
                /**
                	 * PAIEMENTS TABLE ONLY !!!
                	 */
                strUpdateTableQuery.append(QString
                                           ("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'").
                                           arg(allToUpdateTables.at(i),
                                               YerithDatabaseTableColumn::REFERENCE,
                                               cellTextData,
                                               YerithDatabaseTableColumn::DESIGNATION,
                                               designationText));
            }

            bool success1 = YerithUtils::execQuery(strUpdateTableQuery,
                                             YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
											 	 ->_marchandisesWindow->getLogger())
                      && success;

            if (success1)
            {
#ifdef YERITH_SERVER

	YERITH_RUNTIME_VERIFIER *dbusServer = YerithUtils::getAllWindows()->dbusServer();

	dbusServer->YRI_slot_refresh_UPDATE_DB_MYSQL__CALLED(QString("%1;%2;%3")
															.arg(allToUpdateTables.at(i),
																 "YerithERPMarchandisesTableView::dataChanged",
																 QString::number(-1)));

#endif //YERITH_SERVER

				success = success && success1;
            }
        }
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;


    if (success)
    {
        QString
        succesMsgBoxTitle(QObject::tr("succès modification (%1) ").arg
                          (columnHeaderText_VISIBLE));

        YerithQMessageBox::information
        (YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
         succesMsgBoxTitle,
         QObject::tr("Succès de la modification de la colone '%1 (%2)' "
                         "de la marchandise '%3' !").arg
         (columnHeaderText_VISIBLE, cellTextData, designationText),
         QMessageBox::Ok);
    }
    else
    {
        QString echecMsgBoxTitle(QObject::tr("échec modification (%1) ").
                                 arg(columnHeaderText_VISIBLE));

        YerithQMessageBox::information
        (YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER->_marchandisesWindow,
         echecMsgBoxTitle,
         QObject::tr("Échec de la modification de la colone '%1 (%2)' "
                         "de la marchandise '%3' !").arg
         (columnHeaderText_VISIBLE, cellTextData, designationText),
         QMessageBox::Ok);
    }

    stopEditingModeSelection();
}


void YerithERPMarchandisesTableView::selectionChanged(const QItemSelection &
                                                      selected,
                                                      const QItemSelection &
                                                      deselected)
{
    static YerithERPWindows *curAllWindows = YerithUtils::getAllWindows();

    YerithMarchandisesWindow *marchandises_window =
                    curAllWindows->_marchandisesWindow;

    _MAP_lastSelected_Row__TO__DB_ID.clear();

    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();


    QString cur_marchandise_departement_produit;

    QString cur_marchandise_categorie;

    QString cur_marchandise_designation;


    double cur_marchandises_quantite_totale = 0.0;

    QString db_ID_in_out;

    if (selectedIndexes.size() > 0)
    {
        uint last_Row_ID = selectedIndexes.size() - 1;

        for (uint j = 0; j < selectedIndexes.size(); ++j)
        {
            marchandises_window->getQModelIndex_db_VALUE_from_MODEL_INDEX
            (YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
             selectedIndexes.at(j), cur_marchandise_departement_produit);

            marchandises_window->getQModelIndex_db_VALUE_from_MODEL_INDEX
            (YerithDatabaseTableColumn::CATEGORIE, selectedIndexes.at(j),
             cur_marchandise_categorie);

            marchandises_window->getQModelIndex_db_VALUE_from_MODEL_INDEX
            (YerithDatabaseTableColumn::DESIGNATION, selectedIndexes.at(j),
             cur_marchandise_designation);

            cur_marchandises_quantite_totale =
                            YerithUtils::GET_QUANTITE_TOTALE_EN_STOCK
                            (cur_marchandise_departement_produit, cur_marchandise_categorie,
                             cur_marchandise_designation);

            marchandises_window->set_quantite_totale_label_string
            (cur_marchandises_quantite_totale);


            marchandises_window->getQModelIndex_dbID_from_MODEL_INDEX
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


void YerithERPMarchandisesTableView::stopEditingModeSelection()
{
    if (!_writeEnabled)
    {
        return;
    }

    setWriteEnabled(false);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    QObject::disconnect(_stdItemModel,
                        SIGNAL(dataChanged
                               (const QModelIndex &, const QModelIndex &)),
                        this,
                        SLOT(dataChanged
                             (const QModelIndex &, const QModelIndex &)));

    int lastSelectRecord_ROW_NUMBER =
                    YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER->
                    _marchandisesWindow->get_INT_last_selected_row_number();

    QStandardItem *anItem = _stdItemModel->item(lastSelectRecord_ROW_NUMBER,
                                                YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER->
                                                _marchandisesWindow->
                                                getDBTableFieldColumnIndex
                                                (YerithDatabaseTableColumn::
                                                 REFERENCE));

    anItem->setForeground(Qt::white);
    anItem->setBackground(YerithUtils::YERITH_BLACK_COLOR);


    anItem = _stdItemModel
    			->item(lastSelectRecord_ROW_NUMBER,
    					YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
							->_marchandisesWindow
								->getDBTableFieldColumnIndex
									(YerithDatabaseTableColumn::DESCRIPTION_PRODUIT));

    anItem->setForeground(Qt::white);
    anItem->setBackground(YerithUtils::YERITH_BLACK_COLOR);


    anItem = _stdItemModel
    			->item(lastSelectRecord_ROW_NUMBER,
    				   YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
					   	   ->_marchandisesWindow
						   	   ->getDBTableFieldColumnIndex
							   	   (YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT));

    anItem->setForeground(Qt::white);
    anItem->setBackground(YerithUtils::YERITH_BLACK_COLOR);


    anItem = _stdItemModel
    			->item(lastSelectRecord_ROW_NUMBER,
    					YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER
							->_marchandisesWindow
								->getDBTableFieldColumnIndex
									(YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT));

    anItem->setForeground(Qt::white);
    anItem->setBackground(YerithUtils::YERITH_BLACK_COLOR);
}
