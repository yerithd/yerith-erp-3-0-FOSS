/*
 * yerith-erp-qstandard-item-model.cpp
 *
 * 		Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-qstandard-item-model.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/widgets/table-view/yerith-erp-table-view.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <QtCore/QDebug>


void YerithPOSQStandardItemModel::yerithPOSClear()
{
    int rows = rowCount();
    int columns = columnCount();

    QStandardItem *anItem = 0;

    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < columns; ++k)
        {
            anItem = item(i, k);

            if (0 != anItem)
            {
                delete anItem;
            }
        }
    }

    QStandardItemModel::clear();
}


QModelIndex
YerithPOSQStandardItemModel::create_ZERO_ZERO_MODEL_INDEX_FOR_TABLE_VIEWING
()
{
    return createIndex(0, 0, (void *) &YerithUtils::EMPTY_STRING);
}


void YerithPOSQStandardItemModel::sort(int column, Qt::SortOrder order
                                       /*= Qt::AscendingOrder*/)
{
    if (0 == _curSqlTableModel)
    {
        return;
    }

    //qDebug() << "++ YerithPOSQStandardItemModel::sort not null";
    _curSqlTableModel->yerithSetSort(column, order);

    if (0 == _curTableView)
    {
        return;
    }

    if (YerithUtils::instanceOf__YerithTableViewWITHpagination(_curTableView))
    {
        ((YerithTableViewWITHpagination *)
         _curTableView)->queryYerithTableViewCurrentPageContentRow
        (*_curSqlTableModel);
    }
    else
    {
        _curTableView->lister_les_elements_du_tableau(*_curSqlTableModel);
    }
}
