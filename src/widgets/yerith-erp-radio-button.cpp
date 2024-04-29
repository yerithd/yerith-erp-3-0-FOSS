/*
 * yerith-erp-radio-button.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "src/widgets/yerith-erp-radio-button.hpp"


void YerithRadioButton::setYerithEnabled(bool enabled)
{
    setVisible(enabled);
    setEnabled(enabled);
}
