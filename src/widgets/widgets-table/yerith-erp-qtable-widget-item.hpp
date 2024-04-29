/*
 * yerith-erp-qtable-widget-item.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_3_0_QTABLE_WIDGET_ITEM_HPP_
#define YERITH_ERP_3_0_QTABLE_WIDGET_ITEM_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QTableWidgetItem>

class YerithQTableWidgetItem:public QTableWidgetItem
{

public:

    YERITH_CLASS_OPERATORS inline YerithQTableWidgetItem():QTableWidgetItem()
    {
        setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    }

    inline YerithQTableWidgetItem(const QString &text):QTableWidgetItem(text)
    {
        setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    }

    inline YerithQTableWidgetItem(const QIcon &icon,
                                  const QString &text):QTableWidgetItem(icon,
                                                                            text)
    {
        setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    }

    inline ~YerithQTableWidgetItem()
    {
    }
};


#endif /* YERITH_ERP_3_0_QTABLE_WIDGET_ITEM_HPP_ */
