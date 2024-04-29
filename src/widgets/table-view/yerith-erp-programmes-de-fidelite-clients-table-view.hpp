/*
 * yerith-erp-programmes-de-fidelite-clients-table-view.hpp
 *
 * Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_PROGRAMMES_DE_FIDELITE_CLIENTS_TABLE_VIEW_HPP_
#define SRC_YERITH_PROGRAMMES_DE_FIDELITE_CLIENTS_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>


class YerithSqlTableModel;

class YerithPOSQStandardItemModel;


class YerithERPProgrammesDeFideliteClientsTableView:public
    YerithTableViewWITHpagination
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline
    YerithERPProgrammesDeFideliteClientsTableView
    ():YerithTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerithERPProgrammesDeFideliteClientsTableView(QWidget *
                                                         parent):YerithTableViewWITHpagination
        (parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerithERPProgrammesDeFideliteClientsTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &
                                                clientGroupSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YERITH_PROGRAMMES_DE_FIDELITE_CLIENTS_TABLE_VIEW_HPP_ */
