/*
 * yerith-erp-table-widget.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-table-widget.hpp"


#include "src/utils/yerith-erp-logger.hpp"

#include "src/widgets/yerith-erp-line-edit.hpp"


#include <QtCore/QDebug>


const QString YerithTableWidget::QTE_1("1");


YerithTableWidget::YerithTableWidget(QWidget *parent):QTableWidget(parent),
    _logger(0)
{
    _logger = new YerithLogger("YerithTableWidget");

    _curRow = 0;

    _myQStandardItemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    setFrameShadow(QFrame::Plain);

    setEditTriggers(QAbstractItemView::CurrentChanged);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    setSelectionMode(QAbstractItemView::SingleSelection);

    resize_columns_AND_rows_to_contents();
}


void YerithTableWidget::yerithClearTableWidgetContent()
{
    QTableWidget::clearContents();

    _mapListIdxToElement_db_ID.clear();

    _curRow = 0;

    setRowCount(0);


    setFrameShadow(QFrame::Plain);

    setEditTriggers(QAbstractItemView::CurrentChanged);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    setSelectionMode(QAbstractItemView::SingleSelection);
}


void YerithTableWidget::removeArticle(int tableWidgetRow)
{
    _logger->log("removeArticle(int)");

    removeRow(tableWidgetRow);

    if (0 == rowCount())
    {
        emit emptiedYerithTableWidget();
    }

    _curRow = _curRow - 1;

    QMap < int, QString > newMapListIdxToItems;

    QMapIterator < int, QString > i(_mapListIdxToElement_db_ID);

    int next_pos = 0;

    int k = 0;

    while (i.hasNext())
    {
        i.next();

        if (k != tableWidgetRow)
        {
            newMapListIdxToItems.insert(next_pos, i.value());

            _logger->debug("enlever_article",
                           QString("key: %1, value: %2").
                           arg(QString::number(k), i.value()));
            ++next_pos;
        }

        ++k;
    }

    _mapListIdxToElement_db_ID.clear();
    _mapListIdxToElement_db_ID = newMapListIdxToItems;
}


void YerithTableWidget::resize_columns_AND_rows_to_contents()
{
    if (columnCount() > 1)
    {
        resizeColumnsToContents();
    }

    resizeRowsToContents();
}


void YerithTableWidget::setQStandardItemFlags(Qt::ItemFlags &flags)
{
    QTableWidgetItem *curItem = 0;

    for (int i = 0; i < rowCount(); ++i)
    {
        for (int j = 0; j < columnCount(); ++j)
        {
            curItem = item(i, j);

            if (0 != curItem)
            {
                curItem->setFlags(flags);
            }
        }
    }
}
