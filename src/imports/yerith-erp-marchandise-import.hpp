/*
 * yerith-erp-stock-marchandise.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_MARCHANDISE_IMPORT_WINDOW_HPP_
#define YERITH_MARCHANDISE_IMPORT_WINDOW_HPP_


#include "src/imports/yerith-erp-stock-import.hpp"


class YerithERPMarchandiseImport:public YerithERPStockImport
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS YerithERPMarchandiseImport(QStringList &
                                                      aCurCsvFileToImportContentWordList,
                                                      QMap < int,
                                                      YerithERPDatabaseTableColumnInfo *
                                                      > &aCsvContentIdxToDatabaseTableColumnInfo);


    YerithERPMarchandiseImport(YerithPOSAdminWindowsCommons &aCallingWindow,
                               QStringList &
                               aCurCsvFileToImportContentWordList,
                               QMap < int,
                               YerithERPDatabaseTableColumnInfo *
                               > &anIndexToDatabaseTableColumnInfo);

    virtual inline ~YerithERPMarchandiseImport()
    {
    }

    virtual inline int import_csv_file()
    {
        return YerithERPStockImport::import_csv_file();
    }

    virtual inline int import(bool importerParlant = false)
    {
        return YerithERPStockImport::import(importerParlant);
    }

protected:

    virtual enum import_csv_entry_row_return_status import_csv_entry_row(bool importerParlant,
                                                                         QStringList &aCsvFileEntryLine);
};


#endif /* YERITH_MARCHANDISE_IMPORT_WINDOW_HPP_ */
