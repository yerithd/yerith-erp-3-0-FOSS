/*
 * yerith-erp-abstract-class-detail-view-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_UTILS_WINDOWS_YERITH_ABSTRACT_CLASS_VIEW_DETAIL_WINDOW_HPP_
#define SRC_UTILS_WINDOWS_YERITH_ABSTRACT_CLASS_VIEW_DETAIL_WINDOW_HPP_


#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtCore/QString>


class YerithERPAbstractClassDetailViewFOR_WINDOW
{

protected:

    YERITH_CLASS_OPERATORS inline YerithERPAbstractClassDetailViewFOR_WINDOW()
    {
    }

    virtual inline ~YerithERPAbstractClassDetailViewFOR_WINDOW()
    {
        _previousBeforeTHISWINDOWEnteringFilter.clear();
    }

    virtual void showItem_YerithERPAbstractClassDetailViewFOR_WINDOW() = 0;

    inline void resetDetailViewFilter(YerithSqlTableModel *
                                      detailView_Yerith_SQL_TABLE_MODEL)
    {
        detailView_Yerith_SQL_TABLE_MODEL->setFilter
        (getPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter());
    }

    void setDetailViewFilter(YerithSqlTableModel *
                             detailView_Yerith_SQL_TABLE_MODEL);

    inline void setPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter(QString
                                                                      aPreviousBeforeTHISWINDOWEnteringFilter)
    {
        _previousBeforeTHISWINDOWEnteringFilter =
                        aPreviousBeforeTHISWINDOWEnteringFilter;
    }

    inline const QString &getPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter() const
    {
        return _previousBeforeTHISWINDOWEnteringFilter;
    }

private:

    QString _previousBeforeTHISWINDOWEnteringFilter;
};


#endif /* SRC_UTILS_WINDOWS_YERITH_ABSTRACT_CLASS_VIEW_DETAIL_WINDOW_HPP_ */
