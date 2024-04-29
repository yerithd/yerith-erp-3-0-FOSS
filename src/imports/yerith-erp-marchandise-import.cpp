/*
 * yerith-erp-marchandise-import.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-marchandise-import.hpp"


#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"


#include <QtCore/QDebug>


YerithERPMarchandiseImport::YerithERPMarchandiseImport(QStringList &
                                                       aCurCsvFileToImportContentWordList,
                                                       QMap < int,
                                                       YerithERPDatabaseTableColumnInfo
                                                       *
                                                       > &aCsvContentIdxToDatabaseTableColumnInfo):
    YerithERPStockImport(aCurCsvFileToImportContentWordList,
                         aCsvContentIdxToDatabaseTableColumnInfo)
{
    _allMandatoryTableColumns.clear();

    _allMandatoryTableColumns.append(YerithDatabaseTableColumn::DESIGNATION);

    _allMandatoryTableColumns.
    append(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    _allMandatoryTableColumns.append(YerithDatabaseTableColumn::CATEGORIE);
}


YerithERPMarchandiseImport::YerithERPMarchandiseImport
(YerithPOSAdminWindowsCommons &aCallingWindow,
 QStringList &aCurCsvFileToImportContentWordList, QMap < int,
 YerithERPDatabaseTableColumnInfo * > &anIndexToDatabaseTableColumnInfo):
    YerithERPStockImport(aCallingWindow, aCurCsvFileToImportContentWordList,
                         anIndexToDatabaseTableColumnInfo)
{
    _allMandatoryTableColumns.clear();

    _allMandatoryTableColumns.append(YerithDatabaseTableColumn::DESIGNATION);

    _allMandatoryTableColumns.
    append(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    _allMandatoryTableColumns.append(YerithDatabaseTableColumn::CATEGORIE);
}


enum import_csv_entry_row_return_status YerithERPMarchandiseImport::import_csv_entry_row(bool importerParlant,
                                                                                         QStringList &
                                                                                         aCsvFileEntryLine)
{

    enum import_csv_entry_row_return_status insertionReturnStatus =
                    IMPORT_DATA_CSV_INSERTION_FAILED;

    YerithERPWindows *allWindows = YerithUtils::getAllWindows();

    if (0 == allWindows)
    {
        return IMPORT_DATA_CSV_INSERTION_FAILED;
    }

    YerithERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

    YerithSqlTableModel &curMarchandisesTableModel =
    allWindows->getSqlTableModel_marchandises();

    QSqlRecord record = curMarchandisesTableModel.record();

    int marchandises_id_to_save =
    YerithERPWindows::getNextIdSqlTableModel_marchandises();

    int querySize = -1;

    QStringList allImportedTableColumns;

    QString curTableColumnType;
    QString curTableColumnName;
    QString curColumnRowEntry;

    QString productReference;
    QString productName;
    QString productDepartment;
    QString productCategorie;

    for (int j = 0; j < aCsvFileEntryLine.size(); ++j)
    {
        curColumnRowEntry = aCsvFileEntryLine.at(j).trimmed();

        if (!curColumnRowEntry.isEmpty())
        {
            if (curColumnRowEntry.at(0) == '"')
            {
                curColumnRowEntry.replace(0, 1, "");
            }

            if (curColumnRowEntry.at(curColumnRowEntry.length() - 1) == '"')
            {
                curColumnRowEntry.replace(curColumnRowEntry.length() - 1, 1,
                                          "");
            }
        }

        curDatabaseTableColumnInfo =
                        _csvContentIdxToDatabaseTableColumnInfo->value(j);

        if (0 != curDatabaseTableColumnInfo)
        {
            curTableColumnType = curDatabaseTableColumnInfo->getColumnType();

            curTableColumnName = curDatabaseTableColumnInfo->getColumnName();

//                      QDEBUG_STRING_OUTPUT_2("curTableColumnName", curTableColumnName);
//
//                      QDEBUG_STRING_OUTPUT_2("curTableColumnType", curTableColumnType);
//
//                      QDEBUG_STRING_OUTPUT_2("curColumnRowEntry", curColumnRowEntry);

            if (YerithUtils::isEqualCaseInsensitive
                    (YerithDatabaseTableColumn::REFERENCE, curTableColumnName))
            {
                productReference = curColumnRowEntry;
            }

            if (_allMandatoryTableColumns.contains(curTableColumnName) &&
                    YerithUtils::isEqualCaseInsensitive(YerithDatabaseTableColumn::
                                                        DESIGNATION,
                                                        curTableColumnName))
            {
                productName = curColumnRowEntry;
            }

            if (_allMandatoryTableColumns.contains(curTableColumnName) &&
                    YerithUtils::isEqualCaseInsensitive
                    (YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                     curTableColumnName))
            {
                productDepartment = curColumnRowEntry;

                QString
                queryProductDepartmentStr(QString
                                          ("select * from %1 WHERE %2 = '%3'").
                                          arg(YerithDatabase::
                                              DEPARTEMENTS_PRODUITS,
                                              YerithDatabaseTableColumn::
                                              NOM_DEPARTEMENT_PRODUIT,
                                              curColumnRowEntry));

                querySize =
                                YerithUtils::execQueryRowCount(queryProductDepartmentStr);

                if (querySize <= 0)
                {
                    queryProductDepartmentStr =
                                    QString
                                    ("insert into %1 (%2, %3) values ('%4', '%5')").arg
                                    (YerithDatabase::DEPARTEMENTS_PRODUITS,
                                     YerithDatabaseTableColumn::ID,
                                     YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                                     QString::number
                                     (allWindows->getNextIdSqlTableModel_departements_produits
                                      ()), curColumnRowEntry);

                    if (!YerithUtils::execQuery(queryProductDepartmentStr))
                    {
                        QString infoMesg =
                                        QObject::tr
                                        ("Le département de produits '%1' ne pouvait pas être créée !").
                                        arg(curColumnRowEntry);

                        if (0 != _callingWindow)
                        {
                            if (importerParlant)
                            {
                                YerithQMessageBox::warning(_callingWindow,
                                                           QObject::tr
                                                           ("création d'1 département de produits"),
                                                           infoMesg);
                            }
                        }
                        else
                        {
                            qDebug() << infoMesg;
                        }

                        return IMPORT_DATA_CSV_INSERTION_FAILED;
                    }
                }
            }

            if (_allMandatoryTableColumns.contains(curTableColumnName) &&
                    YerithUtils::isEqualCaseInsensitive(YerithDatabaseTableColumn::
                                                        CATEGORIE,
                                                        curTableColumnName))
            {
                productCategorie = curColumnRowEntry;

                QString
                queryCategoryStr(QString
                                 ("select * from %1 WHERE %2 = '%3'").arg
                                 (YerithDatabase::CATEGORIES,
                                  YerithDatabaseTableColumn::NOM_CATEGORIE,
                                  curColumnRowEntry));

                querySize = YerithUtils::execQueryRowCount(queryCategoryStr);

                if (querySize <= 0)
                {
                    queryCategoryStr =
                                    QString
                                    ("insert into %1 (%2, %3) values ('%4', '%5')").arg
                                    (YerithDatabase::CATEGORIES,
                                     YerithDatabaseTableColumn::ID,
                                     YerithDatabaseTableColumn::NOM_CATEGORIE,
                                     QString::number(allWindows->
                                                     getNextIdSqlTableModel_categories()),
                                     curColumnRowEntry);

                    if (!YerithUtils::execQuery(queryCategoryStr))
                    {
                        QString infoMesg =
                                        QObject::tr("La catégorie '%1' ne pouvait pas "
                                                        "être créée !").arg
                                        (curColumnRowEntry);

                        if (0 != _callingWindow)
                        {
                            if (importerParlant)
                            {
                                YerithQMessageBox::warning(_callingWindow,
                                                           QObject::tr
                                                           ("création de catégorie d'articles"),
                                                           infoMesg);
                            }
                        }
                        else
                        {
                            qDebug() << infoMesg;
                        }

                        return IMPORT_DATA_CSV_INSERTION_FAILED;
                    }
                }
            }


            if (YERITH_QSTRING_CONTAINS
                    (curTableColumnType,
                     YerithUtils::DATABASE_MYSQL_INT_TYPE_STRING))
            {
                int aCurIntValue = curColumnRowEntry.toInt();

                record.setValue(curTableColumnName, aCurIntValue);

                if (YerithUtils::isEqualCaseInsensitive
                        (YerithDatabaseTableColumn::ID, curTableColumnName))
                {
                    marchandises_id_to_save = aCurIntValue;
                }
            }
            else if (YERITH_QSTRING_CONTAINS
                     (curTableColumnType,
                      YerithUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING))
            {
                record.setValue(curTableColumnName,
                                YerithUtils::
                                YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED
                                (curColumnRowEntry));
            }
            else if (YERITH_QSTRING_CONTAINS
                     (curTableColumnType,
                      YerithUtils::DATABASE_MYSQL_DATE_TYPE_STRING))
            {
                record.setValue(curTableColumnName,
                                GET_DATE_FROM_STRING(curColumnRowEntry));
            }
            else
            {
                record.setValue(curTableColumnName, curColumnRowEntry);
            }

            allImportedTableColumns.append(curTableColumnName);
        }
    }


    QString infoMesg;

    QString curExistingReferenceDesignationCategory_PRODUCT_in_out;

    enum service_stock_already_exist_type
    SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST =
                    YerithUtils::IS_STOCK_DESIGNATION_OR_REFERENCE_UNIQUE(productReference,
                                                                          productCategorie,
                                                                          productName,
                                                                          curExistingReferenceDesignationCategory_PRODUCT_in_out);


//    QDEBUG_STRING_OUTPUT_2("YERITH-MARCHANDISE-IMPORT",
//                                              QString("productReference: %1, productCategorie: %2, "
//                                                          "productName: %3, curExistingReferenceDesignationCategory_PRODUCT_in_out: %4")
//                                                      .arg(productReference,
//                                                               productCategorie,
//                                                                       productName,
//                                                                       curExistingReferenceDesignationCategory_PRODUCT_in_out));
//
//
//    QDEBUG_STRING_OUTPUT_2_N("SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT",
//                                               SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST);

    if (SERVICE_REFERENCE_EXISTS == SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
        if (!curExistingReferenceDesignationCategory_PRODUCT_in_out.isEmpty())
        {
            infoMesg = QObject::tr("Cette référence ('%1') "
                                       "est déjà utilisée par la marchandise '%2' !").
                       arg(productReference,
                           curExistingReferenceDesignationCategory_PRODUCT_in_out);
        }
    }
    else if (SERVICE_STOCK_CATEGORY_EXIST ==
             SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
        QString infoMesg =
                        QObject::tr
                        ("La marchandise '%1' est déjà dans la catégorie ('%2') !").arg
                        (productCategorie,
                         curExistingReferenceDesignationCategory_PRODUCT_in_out);
    }
    else if (SERVICE_STOCK_DESIGNATION_EXIST ==
             SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
        infoMesg =
                        QObject::tr
                        ("La marchandise ('%1') utilise déjà la référence '%2' !").arg
                        (productName, curExistingReferenceDesignationCategory_PRODUCT_in_out);
    }

    if (SERVICE_REFERENCE_EXISTS == SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST ||
            SERVICE_STOCK_CATEGORY_EXIST ==
            SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST
            || SERVICE_STOCK_DESIGNATION_EXIST ==
            SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST
            || SERVICE_STOCK_EXIST_IN_MERCHANDISE_DB_TABLE ==
            SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
        if (0 != _callingWindow)
        {
            if (importerParlant)
            {
                YerithQMessageBox::warning(_callingWindow, "enregistrer",
                                           infoMesg);
            }
        }
        else
        {
            qDebug() << infoMesg;
        }

        return IMPORT_DATA_CSV_INSERTION_FAILED;
    }


    record.setValue(YerithDatabaseTableColumn::ID, marchandises_id_to_save);

    record.setValue(YerithDatabaseTableColumn::IS_SERVICE, 0);


    if (0 != _dbTableColumnToIsNotNULL)
    {
        QString aCurSqlTableImportColumn;

        QStringList allSqltableColumns = _dbTableColumnToIsNotNULL->keys();

        allSqltableColumns.removeAll(YerithDatabaseTableColumn::ID);
        allSqltableColumns.removeAll(YerithDatabaseTableColumn::IS_SERVICE);

        for (int k = 0; k < allSqltableColumns.size(); ++k)
        {
            aCurSqlTableImportColumn = allSqltableColumns.at(k);

            if (allImportedTableColumns.contains(aCurSqlTableImportColumn))
            {
                continue;
            }

            if (false ==
                    _dbTableColumnToIsNotNULL->value(aCurSqlTableImportColumn))
            {
                insertionReturnStatus =
                                IMPORT_DATA_CSV_MANDATORY_COLUMN_VALUE_MISSING;
                /*
                 * This SQL stock table column MUST BE NOT NULL.
                 * So we attribute it a standard value.
                 */
                if (!YERITH_QSTRING_CONTAINS
                        (_allMissingMandatoryColumnValue, aCurSqlTableImportColumn))
                {
                    _allMissingMandatoryColumnValue.append(QString(" , %1").
                                                           arg
                                                           (aCurSqlTableImportColumn));
                }
            }
        }

        if (IMPORT_DATA_CSV_MANDATORY_COLUMN_VALUE_MISSING ==
                insertionReturnStatus)
        {
            return IMPORT_DATA_CSV_MANDATORY_COLUMN_VALUE_MISSING;
        }
    }

    bool queryResut = curMarchandisesTableModel.insertNewRecord(record);

    if (queryResut)
    {
        return IMPORT_DATA_CSV_INSERTION_SUCCEED;
    }

    return IMPORT_DATA_CSV_INSERTION_FAILED;
}
