/*
 * yerith-erp-fournisseurs-table-view.hpp
 *
 * Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_FOURNISSEURS_TABLE_VIEW_HPP_
#define SRC_YERITH_FOURNISSEURS_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>


class YerithSqlTableModel;

class YerithPOSQStandardItemModel;


class YerithERPFournisseursTableView:public YerithTableViewWITHpagination
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithERPFournisseursTableView():YerithTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerithERPFournisseursTableView(QWidget *
                                          parent):YerithTableViewWITHpagination
        (parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerithERPFournisseursTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &
                                                achatSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YERITH_FOURNISSEURS_TABLE_VIEW_HPP_ */
