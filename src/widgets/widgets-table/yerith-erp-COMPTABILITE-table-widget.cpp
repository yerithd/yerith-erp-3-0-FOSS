/*
 * yerith-erp-COMPTABILITE-table-widget.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-COMPTABILITE-table-widget.hpp"

#include "src/utils/yerith-erp-logger.hpp"



const unsigned int YerithComptabiliteTableWidget::ID_COLUMN(0);

const unsigned int YerithComptabiliteTableWidget::TYPE_DOPERATION_FINANCIERE_COLUMN(1);

const unsigned int YerithComptabiliteTableWidget::TYPE_DE_CREDIT_DEBIT_COLUMN(2);

const unsigned int YerithComptabiliteTableWidget::INTITULE_LIGNE_BUDGETAIRE_COLUMN(3);



int YerithComptabiliteTableWidget::
        add_an_association(QString yr_type_doperation_financiere_DB_ID,
                           QString yr_type_doperation_financiere,
                           QString yr_type_de_credit_debit,
                           QString yr_intitule_ligne_budgetaire)
{
    _logger->log("YerithComptabiliteTableWidget::add_an_association");


    int ALREADY_EXISTING_VALUE_row =
        _mapListIdxToElement_db_ID.key(yr_type_doperation_financiere_DB_ID,
                                       -1);

    if (-1 != ALREADY_EXISTING_VALUE_row)
    {
        _yr_intitule_ligne_budgetaire_item =
            new YerithQTableWidgetItem(yr_intitule_ligne_budgetaire);

        setItem(ALREADY_EXISTING_VALUE_row,
                YerithComptabiliteTableWidget::INTITULE_LIGNE_BUDGETAIRE_COLUMN,
                _yr_intitule_ligne_budgetaire_item);

        resize_columns_AND_rows_to_contents();

        return ALREADY_EXISTING_VALUE_row;
    }


    setRowCount(_curRow + 1);


    _yr_DB_ID_item =
        new YerithQTableWidgetItem(yr_type_doperation_financiere_DB_ID);

    _yr_type_doperation_financiere_item =
        new YerithQTableWidgetItem(yr_type_doperation_financiere);

    _yr_type_de_credit_debit_item =
        new YerithQTableWidgetItem(yr_type_de_credit_debit);

    _yr_intitule_ligne_budgetaire_item =
        new YerithQTableWidgetItem(yr_intitule_ligne_budgetaire);


    _mapListIdxToElement_db_ID
        .insert(_curRow,
                yr_type_doperation_financiere_DB_ID);


    unsigned idx = 0;

    setItem(_curRow, idx++, _yr_DB_ID_item);
    setItem(_curRow, idx++, _yr_type_doperation_financiere_item);
    setItem(_curRow, idx++, _yr_type_de_credit_debit_item);
    setItem(_curRow, idx, _yr_intitule_ligne_budgetaire_item);

    _yr_DB_ID_item->setFlags(_myQStandardItemFlags);
    _yr_type_doperation_financiere_item->setFlags(_myQStandardItemFlags);
    _yr_type_de_credit_debit_item->setFlags(_myQStandardItemFlags);
    _yr_intitule_ligne_budgetaire_item->setFlags(_myQStandardItemFlags);

    resize_columns_AND_rows_to_contents();

    int lastCurRow = _curRow;

    ++_curRow;

    return lastCurRow;
}


