/*
 * yerith-erp-user-table-view.hpp
 *
 * Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ADMIN_USER_TABLE_VIEW_HPP_
#define SRC_YERITH_ADMIN_USER_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/admin/widgets/table-view/yerith-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp"


#include <QtSql/QSqlTableModel>


class YerithSqlTableModel;

class YerithPOSQStandardItemModel;


class YerithERPUserTableView : public yerithERPAdminUpperTableViewClass
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline YerithERPUserTableView()
    :yerithERPAdminUpperTableViewClass()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerithERPUserTableView(QWidget *parent)
    :yerithERPAdminUpperTableViewClass(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerithERPUserTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &aSqlTableModel);
};


#endif /* SRC_YERITH_ADMIN_USER_TABLE_VIEW_HPP_ */
