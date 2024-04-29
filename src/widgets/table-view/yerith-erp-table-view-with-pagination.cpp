/*
 * yerith-erp-table-view-with-pagination.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-table-view-with-pagination.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QtMath>


void YerithTableViewWITHpagination::setYerithTableViewPageRowCount(uint rowCount)
{
    _yerithTableViewPageRowCount = rowCount;


    double realPageCount = YerithTableView::rowCount() / _yerithTableViewPageRowCount;


    _yerithTableViewPageCount = qCeil(realPageCount);


    _yerithTableViewCurPageNumber = 0;


    if (0 != _curYerithSqlTableModel)
    {
        viewYerithTableViewNextPage(*_curYerithSqlTableModel);
    }
}


void YerithTableViewWITHpagination::
		calculate_and_set_YerithViewLastPageNumber(YerithSqlTableModel &curYerithSqlTableModel_IN)
{
    _curYerithSqlTableModel = &curYerithSqlTableModel_IN;


    _yerithSqlTableModelTotalRowCount = curYerithSqlTableModel_IN.rowCount();


    double tmp = _yerithSqlTableModelTotalRowCount / (double) _yerithTableViewPageRowCount;


    _yerithTableViewLastPageNumber = qCeil(tmp);


    if (_yerithTableViewLastPageNumber <= 0)
    {
        _yerithTableViewLastPageNumber = 1;
    }


    if (1 == _yerithTableViewLastPageNumber)
    {
        _yerithTableViewCurPageNumber = 1;
    }


    if (0 != _currentViewWindow)
    {
        _currentViewWindow
			->setYerithTableViewLastPageNumberText
				(QString::number(_yerithTableViewLastPageNumber));
    }
}


void YerithTableViewWITHpagination::
		displayYerithTableViewPageContentRowLimit(YerithSqlTableModel &curYerithSqlTableModel_IN,
												  QString aListingStrategy /* = YerithUtils::EMPTY_STRING */)
{
    _curYerithSqlTableModel = &curYerithSqlTableModel_IN;

    uint previousPageNumber = 0;

    if (_yerithTableViewCurPageNumber > 1)
    {
        previousPageNumber = _yerithTableViewCurPageNumber - 1;
    }

    uint curPageFromRowNumber = previousPageNumber * _yerithTableViewPageRowCount;

    calculate_and_set_YerithViewLastPageNumber(curYerithSqlTableModel_IN);

    if (curPageFromRowNumber > _yerithSqlTableModelTotalRowCount)
    {
        return;
    }

    QString aCurYerithTableViewPageFilter = QString("limit %1, %2")
    											.arg(QString::number(curPageFromRowNumber),
    												 QString::number(_yerithTableViewPageRowCount));

    int querySize = 0;

    if (!YerithUtils::isEqualCaseInsensitive(curYerithSqlTableModel_IN.sqlTableName(),
    										 YerithDatabase::MARCHANDISES))
    {
        querySize = curYerithSqlTableModel_IN
        				.yerith_specify_filter_FROM_SELECT_STATEMENT(aCurYerithTableViewPageFilter);
    }
    else if (YerithUtils::isEqualCaseInsensitive(curYerithSqlTableModel_IN.sqlTableName(),
              	  	  	  	  	  	  	  	  	 YerithDatabase::MARCHANDISES))
    {
        if (!_allWindows->_marchandisesWindow->IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
        {
            querySize = curYerithSqlTableModel_IN
            				.yerith_specify_filter_FROM_SELECT_STATEMENT(aCurYerithTableViewPageFilter);
        }
    }


    if (!YerithUtils::isEqualCaseInsensitive(curYerithSqlTableModel_IN.sqlTableName(),
    										 YerithDatabase::STOCKS))
    {
        lister_les_elements_du_tableau(curYerithSqlTableModel_IN);
    }
    else
    {
        lister_les_elements_du_tableau(curYerithSqlTableModel_IN,
                                       aListingStrategy,
                                       aCurYerithTableViewPageFilter);
    }


    curYerithSqlTableModel_IN
		.yerith_RESET_specify_filter_FROM_SELECT_STATEMENT(aCurYerithTableViewPageFilter);


    construire_le_MAPPING_ORIGINAL_db_ID_VERS_db_row_Nr(curYerithSqlTableModel_IN);


    if (0 == _currentViewWindow)
    {
        return;
    }


    if (querySize > 0)
    {
        if (curPageFromRowNumber <= _yerithSqlTableModelTotalRowCount)
        {
            if (_yerithTableViewCurPageNumber != _yerithTableViewLastPageNumber)
            {
                _yerithTableViewCurPageNumber =
                                (0 == _yerithTableViewCurPageNumber) ? 1 : _yerithTableViewCurPageNumber;

                _currentViewWindow
					->setYerithTableViewCurrentPageNumberText
						(QString::number(_yerithTableViewCurPageNumber));
            }
            else
            {
                _currentViewWindow
					->setYerithTableViewCurrentPageNumberText
						(QString::number(_yerithTableViewLastPageNumber));
            }
        }
    }
    else
    {
        _yerithTableViewCurPageNumber = 1;

        _currentViewWindow
			->setYerithTableViewCurrentPageNumberText
				(QString::number(_yerithTableViewCurPageNumber));
    }
}


void YerithTableViewWITHpagination::
		queryYerithTableViewCurrentPageContentRow(YerithSqlTableModel &curYerithSqlTableModel_IN,
												  QString aListingStrategy /* = YerithUtils::EMPTY_STRING */)
{
    _curYerithSqlTableModel = &curYerithSqlTableModel_IN;

    displayYerithTableViewPageContentRowLimit(curYerithSqlTableModel_IN,
                                              aListingStrategy);
}


void YerithTableViewWITHpagination::
		slot_set_page_view_row_count(const QString & pageTableViewRowCountText)
{
    if (0 != _allWindows)
    {
        YerithPOSUser *anERPUser = _allWindows->getUser();

        if (0 != anERPUser)
        {
            anERPUser->save_user_personal_PRINTING_PARAMETER_settings();
        }
    }


    if (pageTableViewRowCountText.isEmpty())
    {
        setYerithTableViewPageRowCount(1);

        return;
    }


    int pageTableViewRowCount = pageTableViewRowCountText.toInt();


    if (0 != pageTableViewRowCount)
    {
        setYerithTableViewPageRowCount(pageTableViewRowCount);
    }
}


void YerithTableViewWITHpagination::
		viewYerithTableViewFirstPage(YerithSqlTableModel &curYerithSqlTableModel_IN)
{
    _curYerithSqlTableModel = &curYerithSqlTableModel_IN;

    _yerithTableViewCurPageNumber = 1;

    queryYerithTableViewCurrentPageContentRow(curYerithSqlTableModel_IN);
}


void YerithTableViewWITHpagination::
		viewYerithTableViewLastPage(YerithSqlTableModel &curYerithSqlTableModel_IN)
{
    _curYerithSqlTableModel = &curYerithSqlTableModel_IN;

    _yerithTableViewCurPageNumber = _yerithTableViewLastPageNumber;

    queryYerithTableViewCurrentPageContentRow(curYerithSqlTableModel_IN);
}


void YerithTableViewWITHpagination::
		viewYerithTableViewPreviousPage(YerithSqlTableModel &curYerithSqlTableModel_IN)
{
    _curYerithSqlTableModel = &curYerithSqlTableModel_IN;


    if (_yerithTableViewCurPageNumber > 1)
    {
        _yerithTableViewCurPageNumber = _yerithTableViewCurPageNumber - 1;
    }
    else
    {
        _yerithTableViewCurPageNumber = 1;
    }

    queryYerithTableViewCurrentPageContentRow(curYerithSqlTableModel_IN);
}


void YerithTableViewWITHpagination::
		viewYerithTableViewNextPage(YerithSqlTableModel &curYerithSqlTableModel_IN)
{
    _curYerithSqlTableModel = &curYerithSqlTableModel_IN;


    if (_yerithTableViewCurPageNumber < _yerithTableViewLastPageNumber)
    {
        _yerithTableViewCurPageNumber = _yerithTableViewCurPageNumber + 1;
    }


    queryYerithTableViewCurrentPageContentRow(curYerithSqlTableModel_IN);
}




