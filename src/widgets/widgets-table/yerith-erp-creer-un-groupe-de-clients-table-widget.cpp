/*
 * yerith-erp-creer-un-groupe-de-clients-table-widget.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-creer-un-groupe-de-clients-table-widget.hpp"


#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"

#include "src/utils/yerith-erp-logger.hpp"


unsigned int YerithCreerUnGroupeDeClientsTableWidget::
				addAClientGroupMember(const QString &aClientGroupMember_db_ID,
                                      const QString &aClientGroupMemberReferenceClient,
                                      const QString &aClientGroupMemberNomDuClient)
{
    setRowCount(_curRow + 1);

    _clientReferenceItem = new YerithQTableWidgetItem(aClientGroupMemberReferenceClient);

    _clientNameItem = new YerithQTableWidgetItem(aClientGroupMemberNomDuClient);

    _mapListIdxToElement_db_ID.insert(_curRow, aClientGroupMember_db_ID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerithPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _clientReferenceItem);
    setItem(_curRow, idx++, _clientNameItem);

    set_MY_QStandardItemFlags();

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    if (_curRow >= 0)
    {
        selectRow(_curRow);
    }

    ++_curRow;

    return lastCurRow;
}
