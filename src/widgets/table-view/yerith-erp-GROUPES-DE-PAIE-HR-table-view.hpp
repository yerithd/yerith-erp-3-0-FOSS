/*
 * yerith-erp-GROUPES-DE-PAIE-HR-table-view.hpp
 *
 * Author: XAVIER NOUNDOU, PH.D., P. ENG.
 */

#ifndef _SRC_YERITH_GROUPES_DE_PAIE_HR_TABLE_VIEW_HPP_
#define _SRC_YERITH_GROUPES_DE_PAIE_HR_TABLE_VIEW_HPP_


#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>


class YerithSqlTableModel;

class YerithPOSQStandardItemModel;


class YerithERPGroupesDePaieHRTableView : public YerithTableViewWITHpagination
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline YerithERPGroupesDePaieHRTableView()
    :YerithTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerithERPGroupesDePaieHRTableView(QWidget *parent)
    :YerithTableViewWITHpagination(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerithERPGroupesDePaieHRTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &clientGroupSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* _SRC_YERITH_GROUPES_DE_PAIE_HR_TABLE_VIEW_HPP_ */
