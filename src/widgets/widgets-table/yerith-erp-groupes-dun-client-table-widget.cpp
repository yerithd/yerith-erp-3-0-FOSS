/*
 * yerith-erp-groupes-dun-client-table-widget.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "src/widgets/widgets-table/yerith-erp-groupes-dun-client-table-widget.hpp"


int YerithGroupesDunClientTableWidget::insert_group(const QString &group_db_ID,
                                                    const QString &
                                                    groupDesignation,
                                                    const QString &
                                                    groupReference,
                                                    const QString &groupNumber)
{
//      QDEBUG_STRING_OUTPUT_2("YerithGroupesDunClientWindow::insert_group", groupDesignation);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setRowCount(_curRow + 1);

    _groupDesignationItem = new YerithQTableWidgetItem(groupDesignation);
    _groupReferenceItem = new YerithQTableWidgetItem(groupReference);
    _groupNumberItem = new YerithQTableWidgetItem(groupNumber);

    _mapListIdxToElement_db_ID.insert(_curRow, group_db_ID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerithPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _groupDesignationItem);
    setItem(_curRow, idx++, _groupReferenceItem);
    setItem(_curRow, idx++, _groupNumberItem);

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    if (_curRow >= 0)
    {
        selectRow(_curRow);
    }

    ++_curRow;

    set_MY_QStandardItemFlags();

    emit addedYerithTableWidget();

    return lastCurRow;
}
