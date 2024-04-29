/*
 * yerith-erp-stocks-table-view.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_STOCKS_TABLE_VIEW_HPP_
#define SRC_YERITH_STOCKS_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerithSqlTableModel;

class YerithPOSQStandardItemModel;

class YerithERPStocksTableView:public YerithTableViewWITHpagination
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithERPStocksTableView():YerithTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerithERPStocksTableView(QWidget *
                                    parent):YerithTableViewWITHpagination
        (parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerithERPStocksTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &tableModel,
                                                    	   const QString &aStockListingStrategy,
    													   const QString &aCurYerithTableViewPageFilter = "",
    													   bool DEPRECATED_stocks_lister_view = false);

    virtual inline void lister_les_elements_du_tableau(YerithSqlTableModel &tableModel,
                                                	   const QString &aStockListingStrategy,
													   const QString &aCurYerithTableViewPageFilter = "")
    {
    	lister_les_elements_du_tableau(tableModel,
    								   aStockListingStrategy,
									   aCurYerithTableViewPageFilter,
									   false);
    }


protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YERITH_STOCKS_TABLE_VIEW_HPP_ */
