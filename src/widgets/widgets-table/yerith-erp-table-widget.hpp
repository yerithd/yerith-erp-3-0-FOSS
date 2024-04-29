/*
 * yerith-erp-table-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YERITH_TABLE_WIDGET_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "yerith-erp-qtable-widget-item.hpp"

#include "src/utils/yerith-erp-article-vente-info.hpp"

#include <QtWidgets/QTableWidget>

#include <QtCore/QMap>

class YerithQTableWidgetItem;
class YerithLogger;

class YerithTableWidget : public QTableWidget
{
    Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

    YerithTableWidget(QWidget *parent = 0);

    virtual inline ~YerithTableWidget()
    {
    }

    virtual inline QString get_DB_ELEMENT_db_ID(int aRow)
    {
        return _mapListIdxToElement_db_ID.value(aRow);
    }

    inline const QMap<int, QString> &get_mapListIdxToElement_db_ID()
    {
        return _mapListIdxToElement_db_ID;
    }

    virtual inline int itemCount()
    {
        return _mapListIdxToElement_db_ID.size();
    }

    virtual void setQStandardItemFlags(Qt::ItemFlags &flags);

    static const QString QTE_1;


public slots:
    virtual void yerithClearTableWidgetContent();

    virtual void removeArticle(int tableWidgetRow);


protected:

    virtual void resize_columns_AND_rows_to_contents();

    virtual inline void set_MY_QStandardItemFlags()
    {
        setQStandardItemFlags(_myQStandardItemFlags);
    }


signals:

    void addedYerithTableWidget();

    void emptiedYerithTableWidget();


protected:

    int _curRow;

    Qt::ItemFlags _myQStandardItemFlags;

    QMap <int, QString> _mapListIdxToElement_db_ID;

    YerithLogger *_logger;
};


#endif /* SRC_WIDGETS_YERITH_TABLE_WIDGET_HPP_ */
