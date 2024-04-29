/*
 * yerith-erp-push-button.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-push-button.hpp"

#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"

#include "src/utils/yerith-erp-utils.hpp"


YerithPushButton::~YerithPushButton()
{
    _yerithCurWindowSqlTableModel = 0;
    _receiver = 0;
    _func = 0;
}


void YerithPushButton::setAuthorizedForCurrentUser(bool authorized)
{
    _is_yerith_pushbutton_authorized_for_current_user = authorized;
    QWidget::setEnabled(authorized);
    QWidget::setVisible(authorized);
}


void YerithPushButton::enable(const QObject *receiver, const char *f)
{
    QObject::connect(this, SIGNAL(clicked()), receiver, f,
                     Qt::UniqueConnection);
    setAuthorizedForCurrentUser(true);
}


void YerithPushButton::disable(const QObject *receiver)
{
    QObject::disconnect(this, SIGNAL(clicked()), receiver, 0);
    setAuthorizedForCurrentUser(false);
}


void
YerithPushButton::enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT
(YerithAbstractClassYerithSearchWindow *receiver,
 YerithAbstractClassYerithSearchWindow::fptr func,
 YerithSqlTableModel **aYerithSqlTableModel_IN)
{
    _yerithCurWindowSqlTableModel = aYerithSqlTableModel_IN;

    if (0 != receiver)
    {
        _receiver = receiver;
    }

    if (0 != func)
    {
        _func = func;
    }

    QObject::connect(this, SIGNAL(clicked()), this,
                     SLOT(call_view_page_function()), Qt::UniqueConnection);

    setEnabled(true);
}


void YerithPushButton::call_view_page_function()
{
    if (0 != _receiver && 0 != _func && 0 != *_yerithCurWindowSqlTableModel)
    {
        (_receiver->*_func)(*(*_yerithCurWindowSqlTableModel));
    }
}
