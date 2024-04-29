/*
 * yerith-erp-radio-button.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_RADIO_BUTTON_HPP_
#define YERITH_ERP_RADIO_BUTTON_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QRadioButton>

class QToolBar;
class YerithPOSAdminWindowsCommons;
class YerithWindowsCommons;

class YerithRadioButton:public QRadioButton
{

public:

    YERITH_CLASS_OPERATORS inline YerithRadioButton(const QString &text, QWidget *parent = 0):QRadioButton(text,
                                                                                                               parent)
    {
    }

    inline YerithRadioButton(QWidget *parent = 0):QRadioButton(parent)
    {
    }

    inline ~YerithRadioButton()
    {
    }

    void setYerithEnabled(bool enabled);
};


#endif /* YERITH_ERP_RADIO_BUTTON_HPP_ */
