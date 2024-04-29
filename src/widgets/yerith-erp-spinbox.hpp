/*
 * yerith-erp-spinbox.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_SPINBOX_HPP_
#define YERITH_SPINBOX_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"


#include <QtWidgets/QDoubleSpinBox>


#include "src/utils/yerith-erp-utils.hpp"

class YerithUtils;

class YerithSpinBox:public QDoubleSpinBox
{
public:

    YERITH_CLASS_OPERATORS inline YerithSpinBox(QWidget *parent = 0):QDoubleSpinBox
        (parent)
    {
        YerithUtils::yerithSetWidgetColor(this);
    }

    inline ~YerithSpinBox()
    {
    }

    bool checkField();

    void clearField();

    void displayValue(double intValue);

    inline double valueMultiplyBy(double aValue)
    {
        return value() * aValue;
    }

    void setYerithEnabled(bool enabled);

private:

    QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* YERITH_SPINBOX_HPP_ */
