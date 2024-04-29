/*
 * yerith-erp-qstandard-item-model.hpp
 *
 * 		Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_QSTANDARD_ITEM_MODEL_HPP_
#define YERITH_ERP_QSTANDARD_ITEM_MODEL_HPP_


#include "src/include/yerith-erp-3-0-definition-oo-class-operators.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"


#include <QtGui/QStandardItemModel>


class YerithTableView;


class YerithPOSQStandardItemModel:public QStandardItemModel
{

public:

    YERITH_CLASS_OPERATORS inline YerithPOSQStandardItemModel(QObject *parent = 0):QStandardItemModel(parent),
        _curTableView
        (0)
    {
        _curSqlTableModel = 0;
    }


    inline YerithPOSQStandardItemModel(int rows, int columns, QObject *parent = 0):QStandardItemModel(rows, columns, parent),
        _curTableView
        (0)
    {
        _curSqlTableModel = 0;
    }

    inline ~YerithPOSQStandardItemModel()
    {
        yerithPOSClear();
    }

    void yerithPOSClear();

    QModelIndex create_ZERO_ZERO_MODEL_INDEX_FOR_TABLE_VIEWING();

    inline const YerithSqlTableModel *get_yerith_sql_table_model()
    {
        return _curSqlTableModel;
    }

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:

    YerithTableView *_curTableView;

    YerithSqlTableModel *_curSqlTableModel;
};


#endif /* YERITH_ERP_QSTANDARD_ITEM_MODEL_HPP_ */
