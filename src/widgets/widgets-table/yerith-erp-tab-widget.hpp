/*
 * yerith-erp-tab-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_TAB_WIDGET_HPP_
#define YERITH_ERP_TAB_WIDGET_HPP_

#include <QtWidgets/QTabWidget>

#include "src/utils/yerith-erp-utils.hpp"

class YerithUtils;

class YerithERPTabWidget:public QTabWidget
{

public:

    YERITH_CLASS_OPERATORS
    inline YerithERPTabWidget(QWidget *parent):QTabWidget(parent)
    {
    }

    inline ~YerithERPTabWidget()
    {
    }

    inline void resetYerithERPTabWidget()
    {
        setCurrentIndex(0);
    }
};


#endif /* YERITH_ERP_TAB_WIDGET_HPP_ */
