/*
 * yerith-erp-comptes-doperations-comptabilite-table-view.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_COMPTES_DOPERATIONS_COMPTABLE_TABLE_VIEW_HPP_
#define SRC_YERITH_COMPTES_DOPERATIONS_COMPTABLE_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include <QtSql/QSqlTableModel>

class YerithSqlTableModel;

class YerithPOSQStandardItemModel;

class YerithERPComptes_doperations_comptabilite_TableView :
        public YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION
{
    Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

    inline YerithERPComptes_doperations_comptabilite_TableView()
    :YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerithERPComptes_doperations_comptabilite_TableView(QWidget *parent)
    :YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION(parent)
    {
        _stdItemModel->_curTableView = this;
    }


    virtual inline ~YerithERPComptes_doperations_comptabilite_TableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &tableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YERITH_COMPTES_DOPERATIONS_COMPTABLE_TABLE_VIEW_HPP_ */
