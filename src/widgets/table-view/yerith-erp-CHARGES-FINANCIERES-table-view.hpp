/*
 * yerith-erp-CHARGES-FINANCIERES-table-view.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_CHARGES_FINANCIERES_TABLE_VIEW_HPP_
#define SRC_YERITH_CHARGES_FINANCIERES_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerithSqlTableModel;

class YerithPOSQStandardItemModel;

class YerithERP_CHARGES_FINANCIERES_TableView : public YerithTableViewWITHpagination
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS


    inline YerithERP_CHARGES_FINANCIERES_TableView()
    :YerithTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerithERP_CHARGES_FINANCIERES_TableView(QWidget *parent)
    :YerithTableViewWITHpagination(parent)
    {
        _stdItemModel->_curTableView = this;
    }


    virtual inline ~YerithERP_CHARGES_FINANCIERES_TableView()
    {
    }


    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &achatAUXFournisseursSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YERITH_CHARGES_FINANCIERES_TABLE_VIEW_HPP_ */
