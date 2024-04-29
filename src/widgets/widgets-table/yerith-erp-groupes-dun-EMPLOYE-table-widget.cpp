/*
 * yerith-erp-groupes-dun-EMPLOYE-table-widget.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "src/widgets/widgets-table/yerith-erp-groupes-dun-EMPLOYE-table-widget.hpp"


#include "src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.hpp"


int YerithGroupesDunEmployeTableWidget::insert_group(const QString &group_db_ID,
                                                     const QString &groupDesignation,
                                                     const QString &groupReference,
                                                     const QString &groupNumber,
                                                     bool          COLOR_RED /* = false */)
{
    setRowCount(_curRow + 1);

    _groupDesignationItem = new YerithQTableWidgetItem(groupDesignation);
    _groupReferenceItem = new YerithQTableWidgetItem(groupReference);
    _groupNumberItem = new YerithQTableWidgetItem(groupNumber);

    _mapListIdxToElement_db_ID.insert(_curRow, group_db_ID);

    unsigned idx = 0;

    setItem(_curRow, idx++, _groupDesignationItem);
    setItem(_curRow, idx++, _groupReferenceItem);
    setItem(_curRow, idx++, _groupNumberItem);


    _groupDesignationItem->setFlags(_myQStandardItemFlags);
    _groupReferenceItem->setFlags(_myQStandardItemFlags);
    _groupNumberItem->setFlags(_myQStandardItemFlags);


    if (COLOR_RED)
    {
        _groupDesignationItem->setForeground(YerithUtils::YERITH_RED_COLOR);
        _groupReferenceItem->setForeground(YerithUtils::YERITH_RED_COLOR);
        _groupNumberItem->setForeground(YerithUtils::YERITH_RED_COLOR);
    }
    else
    {
        _groupDesignationItem->setForeground(YerithUtils::YERITH_WHITE_COLOR);
        _groupReferenceItem->setForeground(YerithUtils::YERITH_WHITE_COLOR);
        _groupNumberItem->setForeground(YerithUtils::YERITH_WHITE_COLOR);
    }


    resize_columns_AND_rows_to_contents();

    int lastCurRow = _curRow;

    ++_curRow;

    emit addedYerithTableWidget();

    return lastCurRow;
}


void YerithGroupesDunEmployeTableWidget::
        selectionChanged(const QItemSelection &selected,
                         const QItemSelection &deselected)
{
    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();

    QString db_ID_in_out;

    if (0 != a_current_window   &&
        selectedIndexes.size() > 0)
    {
        int row = selectedIndexes.at(0).row();

        int column = selectedIndexes.at(0).column();

        a_current_window
            ->handle_DATE_DEBUT_et_DATE_FIN_dappartenance(row);
    }
}



