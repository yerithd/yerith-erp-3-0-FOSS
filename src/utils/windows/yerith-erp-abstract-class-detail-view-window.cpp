/*
 * yerith-erp-abstract-class-detail-view-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-abstract-class-detail-view-window.hpp"


void
YerithERPAbstractClassDetailViewFOR_WINDOW::setDetailViewFilter
(YerithSqlTableModel *detailView_Yerith_SQL_TABLE_MODEL)
{
    setPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter
    (detailView_Yerith_SQL_TABLE_MODEL->filter());

    showItem_YerithERPAbstractClassDetailViewFOR_WINDOW();
}
