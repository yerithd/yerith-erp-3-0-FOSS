/*
 * yerith-erp-datetime-edit.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-datetime-edit.hpp"

#include "src/yerith-erp-windows.hpp"
#include "src/utils/yerith-erp-logger.hpp"

#include <QtCore/QPoint>

YerithDateTimeEdit::YerithDateTimeEdit(QWidget *parent)
:QDateTimeEdit(parent),
 _logger(new YerithLogger("YerithDateTimeEdit")),
 _calendarWidget(new QCalendarWidget)
{
    _calendarWidget->setMinimumDate(QDate(1900, 1, 1));

    _calendarWidget->setMaximumDate(QDate(70000, 1, 1));

    _calendarWidget->setGridVisible(true);


    setCalendarPopup(true);


    setCalendarWidget(_calendarWidget);


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	setLocale(YerithUtils::englishLocale);
    }
    else
    {
    	setLocale(YerithUtils::frenchLocale);
    }


    setYerithERPDateTimeEditDisplayFormat();
}


void YerithDateTimeEdit::setYerithEnabled(bool enabled)
{
    QDateTimeEdit::setReadOnly(!enabled);

    setYerithERPDateTimeEditDisplayFormat();
}


void YerithDateTimeEdit::setYerithERPDateTimeEditDisplayFormat()
{
    if (isReadOnly())
    {
        setAlignment(Qt::AlignCenter);

        setPalette(YerithUtils::YERITH_BLACK_PALETTE);
    }
    else
    {
        setAlignment(Qt::AlignLeft);

        setPalette(YerithUtils::YERITH_WHITE_PALETTE);
    }
}


void YerithDateTimeEdit::setStartDate(QDate startDate)
{
    _startDate = startDate;

    setDate(_startDate);
}
