/*
 * yerith-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp
 *
 * Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ADMIN_UPPER_TABLE_VIEW_CLASS_HPP_
#define SRC_YERITH_ADMIN_UPPER_TABLE_VIEW_CLASS_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include <QtSql/QSqlTableModel>

class YerithSqlTableModel;

class YerithPOSQStandardItemModel;

class yerithERPAdminUpperTableViewClass : public YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline yerithERPAdminUpperTableViewClass()
    :YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
    {
        _stdItemModel->_curTableView = this;
    }

    inline yerithERPAdminUpperTableViewClass(QWidget *parent)
    :YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~yerithERPAdminUpperTableViewClass()
    {
    }

    virtual void SAVE_YERITH_TABLEVIEW_header_COLUMN_order(YerithWindowsCommons &a_window);

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &aSqlTableModel);
};


#endif /* SRC_YERITH_ADMIN_UPPER_TABLE_VIEW_CLASS_HPP_ */
