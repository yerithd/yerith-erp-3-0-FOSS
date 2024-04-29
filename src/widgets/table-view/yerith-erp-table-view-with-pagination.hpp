/*
 * yerith-erp-table-view-with-pagination.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_TABLE_VIEW_WITH_PAGING_HPP_
#define SRC_YERITH_TABLE_VIEW_WITH_PAGING_HPP_


#include "src/utils/yerith-erp-utils.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/widgets/table-view/yerith-erp-table-view.hpp"


class YerithTableViewWITHpagination : public YerithTableView
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

    inline YerithTableViewWITHpagination()
    :YerithTableView(),
     _currentViewWindow(0),
     _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING(false),
     _yerithSqlTableModelTotalRowCount(0),
     _yerithTableViewLastPageNumber(1),
     _yerithTableViewCurPageNumber(1),
     _yerithTableViewPageCount(1),
     _yerithTableViewPageRowCount(YerithERPConfig::pagination_number_of_table_rows),
     _allWindows(YerithUtils::getAllWindows()),
	  _curYerithSqlTableModel(0)
    {
    }

    inline YerithTableViewWITHpagination(QWidget *parent)
    :YerithTableView(parent),
     _currentViewWindow(0),
     _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING(false),
     _yerithSqlTableModelTotalRowCount(0), _yerithTableViewLastPageNumber(1),
     _yerithTableViewCurPageNumber(1), _yerithTableViewPageCount(1),
     _yerithTableViewPageRowCount(YerithERPConfig::pagination_number_of_table_rows),
     _allWindows(YerithUtils::getAllWindows()),
	 _curYerithSqlTableModel(0)
    {
    }

    virtual inline ~YerithTableViewWITHpagination()
    {
    }

    inline int getYerithSqlTableModelTotalRowCount()
    {
        return _yerithSqlTableModelTotalRowCount;
    }

    inline int getYerithTableViewPageCount()
    {
        return _yerithTableViewPageCount;
    }

    inline int getYerithTableViewPageRowCount()
    {
        return _yerithTableViewPageRowCount;
    }

    inline int getNEEDExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING()
    {
        return _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING;
    }

    inline void setNEEDExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING(bool aBooleanValue)
    {
        _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING = aBooleanValue;
    }

    virtual void setYerithTableViewPageRowCount(uint rowCount);

    void calculate_and_set_YerithViewLastPageNumber(YerithSqlTableModel &curYerithSqlTableModel_IN);

    virtual void displayYerithTableViewPageContentRowLimit(YerithSqlTableModel  &curYerithSqlTableModel,
                                                           QString 				aListingStrategy = YerithUtils::EMPTY_STRING);

    virtual void queryYerithTableViewCurrentPageContentRow(YerithSqlTableModel 	&curYerithSqlTableModel_IN,
                                                           QString 				aListingStrategy = YerithUtils::EMPTY_STRING);

public slots:

    virtual void slot_set_page_view_row_count(const QString &pageTableViewRowCountText);

    virtual void viewYerithTableViewFirstPage(YerithSqlTableModel &curYerithSqlTableModel_IN);

    virtual void viewYerithTableViewLastPage(YerithSqlTableModel &curYerithSqlTableModel_IN);

    virtual void viewYerithTableViewPreviousPage(YerithSqlTableModel &curYerithSqlTableModel_IN);

    virtual void viewYerithTableViewNextPage(YerithSqlTableModel &curYerithSqlTableModel_IN);

public:

    YerithWindowsCommons *_currentViewWindow;

protected:

    bool _needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING;

    int _yerithSqlTableModelTotalRowCount;


    int _yerithTableViewLastPageNumber;

    int _yerithTableViewCurPageNumber;

    int _yerithTableViewPageCount;

    int _yerithTableViewPageRowCount;

    YerithERPWindows *_allWindows;

private:

    YerithSqlTableModel *_curYerithSqlTableModel;
};


#endif /* SRC_YERITH_TABLE_VIEW_WITH_PAGING_HPP_ */
