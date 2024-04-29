/*
 * yerith-erp-qcolumn-view.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_QCOLUMN_VIEW_HPP_
#define YERITH_QCOLUMN_VIEW_HPP_


#include "src/utils/yerith-erp-utils.hpp"


#include <QtWidgets/QColumnView>


class YerithQColumnView:public QComboBox
{

public:

    YERITH_CLASS_OPERATORS
    inline YerithQColumnView(QWidget *parent):QColumnView(parent)
    {
    }

    inline ~YerithQColumnView()
    {
    }

private:

};


#endif /* YERITH_QCOLUMN_VIEW_HPP_ */
