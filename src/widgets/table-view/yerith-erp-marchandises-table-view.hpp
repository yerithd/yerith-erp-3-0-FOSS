/*
 * yerith-erp-marchandises-table-view.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_MARCHANDISES_TABLE_VIEW_HPP_
#define SRC_YERITH_MARCHANDISES_TABLE_VIEW_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>


class YerithSqlTableModel;

class YerithPOSQStandardItemModel;


class YerithERPMarchandisesTableView:public YerithTableViewWITHpagination
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithERPMarchandisesTableView():YerithTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerithERPMarchandisesTableView(QWidget *
                                          parent):YerithTableViewWITHpagination
        (parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerithERPMarchandisesTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &
                                                tableModel);

public slots:
    void startEditingModeSelection();

    void stopEditingModeSelection();

protected slots:
    virtual void dataChanged(const QModelIndex &index,
                             const QModelIndex &bottomRight,
                             const QVector < int > &roles =
                                             QVector < int >());

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YERITH_MARCHANDISES_TABLE_VIEW_HPP_ */
