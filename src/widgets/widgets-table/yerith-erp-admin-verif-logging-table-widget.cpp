/*
 * yerith-erp-admin-verif-logging-table-widget.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-admin-verif-logging-table-widget.hpp"

#include "src/utils/yerith-erp-logger.hpp"


#include "src/dbus/YERITH_RUNTIME_VERIFIER.hpp"


const unsigned int YerithADMINVerifLoggingTableWidget::TIME_STAMP_COLUMN(0);

const unsigned int YerithADMINVerifLoggingTableWidget::SIGNAL_COLUMN(1);

const unsigned int YerithADMINVerifLoggingTableWidget::SOURCE_COLUMN(2);

const unsigned int YerithADMINVerifLoggingTableWidget::TARGET_COLUMN(3);

const unsigned int YerithADMINVerifLoggingTableWidget::changed_or_MODIFIED_database_QTY_COLUMN(4);



int YerithADMINVerifLoggingTableWidget::addArticle(QString TIMESTAMPtem,
												   QString SIGNALItem,
		   	   	   	   	   	   	   	   	   	  	   QString SOURCEItem,
												   QString TARGETItem,
												   QString changed_OR_modified_database_qty_Item)
{
    _logger->log("addArticle");

    setRowCount(_curRow + 1);

    _TIMESTAMPtem = new YerithQTableWidgetItem(TIMESTAMPtem);
    _SIGNALItem = new YerithQTableWidgetItem(SIGNALItem);
    _SOURCEItem = new YerithQTableWidgetItem(SOURCEItem);
    _TARGETItem = new YerithQTableWidgetItem(TARGETItem);

    _changed_OR_modified_database_qty_Item =
    		new YerithQTableWidgetItem(changed_OR_modified_database_qty_Item);

    _mapListIdxToElement_db_ID.insert(_curRow, SIGNALItem);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerithPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _TIMESTAMPtem);
    setItem(_curRow, idx++, _SIGNALItem);
    setItem(_curRow, idx++, _SOURCEItem);
    setItem(_curRow, idx++, _TARGETItem);
    setItem(_curRow, idx++, _changed_OR_modified_database_qty_Item);

    _TIMESTAMPtem->setFlags(_myQStandardItemFlags);
    _SIGNALItem->setFlags(_myQStandardItemFlags);
    _SOURCEItem->setFlags(_myQStandardItemFlags);
    _TARGETItem->setFlags(_myQStandardItemFlags);
    _changed_OR_modified_database_qty_Item->setFlags(_myQStandardItemFlags);

    selectRow(_curRow);

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    if (_curRow < YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
    {
    	++_curRow;
    }
    else
    {
    	_curRow = 0;
    }

    emit addedYerithTableWidget();

    return lastCurRow;
}
