/*
 * yerith-erp-spinbox.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-spinbox.hpp"


void YerithSpinBox::setYerithEnabled(bool enabled)
{
    _originalPaletteBeforeMissingInformation = palette();

    QDoubleSpinBox::setReadOnly(!enabled);

    if (!isReadOnly())
    {
        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        setBackgroundRole(QPalette::WindowText);
        setForegroundRole(QPalette::Window);
    }
    else
    {
        setPalette(YerithUtils::YERITH_BLACK_PALETTE);
        setBackgroundRole(QPalette::Window);
        setForegroundRole(QPalette::WindowText);
    }
}


bool YerithSpinBox::checkField()
{
    if (0 == value())
    {
        setPalette(YerithUtils::YERITH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}


void YerithSpinBox::clearField()
{
    clear();

    setPalette(_originalPaletteBeforeMissingInformation);
}


void YerithSpinBox::displayValue(double intValue)
{
    setValue(intValue);
    setYerithEnabled(false);
}
