/*
 * yerith-erp-stock-import.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_STOCK_IMPORT_WINDOW_HPP_
#define YERITH_STOCK_IMPORT_WINDOW_HPP_

#include "src/imports/yerith-erp-database-table-import-info.hpp"

#include "src/utils/yerith-erp-utils.hpp"


enum import_csv_entry_row_return_status
{
    IMPORT_DATA_CSV_INSERTION_SUCCEED = 0,
    IMPORT_DATA_CSV_INSERTION_ALREADY_EXISTANT,
    IMPORT_DATA_CSV_INSERTION_FAILED,
    IMPORT_DATA_CSV_INCORRECT_COLUMN_VALUE,
    IMPORT_DATA_CSV_MANDATORY_COLUMN_VALUE_MISSING,
    IMPORT_DATA_CSV_UNDEFINED
};


class YerithERPStockImport:public QObject
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS YerithERPStockImport(QStringList &aCurCsvFileToImportContentWordList,
                                                QMap < int,
                                                YerithERPDatabaseTableColumnInfo *
                                                > &anIndexToDatabaseTableColumnInfo);

    YerithERPStockImport(YerithPOSAdminWindowsCommons &aCallingWindow,
                         QStringList &aCurCsvFileToImportContentWordList,
                         QMap < int,
                         YerithERPDatabaseTableColumnInfo *
                         > &anIndexToDatabaseTableColumnInfo);

    virtual inline ~YerithERPStockImport()
    {
    }

    virtual inline int import_csv_file()
    {
        return import();
    }

    virtual int import(bool importerParlant = false);

    static QString _allMissingMandatoryColumnValue;

    static QMap < QString, bool > *_dbTableColumnToIsNotNULL;

signals:

    void SIGNAL_INCREMENT_PROGRESS_BAR(int countSuccess);

protected:

    void missing_mandatory_item_field_msg(const QString &aMandatoryColumn);

    bool check_mandatory_item_field();

    virtual enum import_csv_entry_row_return_status import_csv_entry_row(bool importerParlant,
                                                                         QStringList &aCsvFileEntryLine);


    QStringList _allSqlTableImportColumns;

    QStringList _allMandatoryTableColumns;

    QStringList *_curCsvFileToImportContentWordList;

    QMap < int,
         YerithERPDatabaseTableColumnInfo *
         > *_csvContentIdxToDatabaseTableColumnInfo;

    YerithPOSAdminWindowsCommons *_callingWindow;
};


#endif /* YERITH_STOCK_IMPORT_WINDOW_HPP_ */
