/**
 * yerith-erp-dialog-commons.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-dialog-commons.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include "src/yerith-erp-windows.hpp"


YerithERPWindows *YerithPOSDialogCommons::_allWindows(0);


void YerithPOSDialogCommons::showAsModalDialogWithParent(QWidget &parent)
{
    setParent(&parent, Qt::Popup | Qt::Dialog);
    show();
    activateWindow();
}


void YerithPOSDialogCommons::closeEvent(QCloseEvent *closeEvent)
{
    rendreInvisible();
}
