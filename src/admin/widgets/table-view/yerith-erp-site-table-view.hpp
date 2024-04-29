/*
 * yerith-erp-site-table-view.hpp
 *
 * Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ADMIN_SITE_TABLE_VIEW_HPP_
#define SRC_YERITH_ADMIN_SITE_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/admin/widgets/table-view/yerith-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp"


#include <QtSql/QSqlTableModel>


class YerithSqlTableModel;

class YerithPOSQStandardItemModel;


class YerithERPSiteTableView : public yerithERPAdminUpperTableViewClass
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline YerithERPSiteTableView()
    :yerithERPAdminUpperTableViewClass()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerithERPSiteTableView(QWidget *parent)
    :yerithERPAdminUpperTableViewClass(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerithERPSiteTableView()
    {
    }
};


#endif /* SRC_YERITH_ADMIN_SITE_TABLE_VIEW_HPP_ */
