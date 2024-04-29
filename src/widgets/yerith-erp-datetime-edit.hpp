/*
 * yerith-erp-datetime-edit.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_DATETIME_EDIT_HPP_
#define SRC_WIDGETS_YERITH_DATETIME_EDIT_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QCalendarWidget>

class QPoint;
class QDateTimeEdit;
class QCalendarWidget;
class QMainWindow;

class YerithERPWindows;
class YerithLogger;

class YerithDateTimeEdit : public QDateTimeEdit
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithDateTimeEdit(QWidget *parent = 0);

    inline ~YerithDateTimeEdit()
    {
        delete _calendarWidget;
    }

    void setYerithEnabled(bool enabled);

    void setYerithERPDateTimeEditDisplayFormat();

    void setStartDate(QDate startDate);

    inline void setMyDate(QDate aDate)
    {
        setDate(aDate);
    }

    inline QCalendarWidget *getCalendarWidget()
    {
        return _calendarWidget;
    }

public slots:

    inline void reset()
    {
        setDate(_startDate);
    }

private:

    YerithLogger *_logger;

    QDate _startDate;

    QCalendarWidget *_calendarWidget;
};


#endif /* SRC_WIDGETS_YERITH_DATETIME_EDIT_HPP_ */
