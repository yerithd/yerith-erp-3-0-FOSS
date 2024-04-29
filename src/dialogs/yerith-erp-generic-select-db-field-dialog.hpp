/*
 * yerith-erp-generic-dialog.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_GENERIC_SELECT_DB_FIELDS_DIALOG_
#define YERITH_ERP_GENERIC_SELECT_DB_FIELDS_DIALOG_

#include "../../ui_yerith-erp-changer-utilisateur-dialog.h"

#include "yerith-erp-dialog-commons.hpp"

class YerithERPWindows;
class YerithPOSUser;

class YerithERPGenericSelectDBFieldDialog : public YerithPOSDialogCommons
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithERPGenericSelectDBFieldDialog(YerithERPWindows 		*allWindows,
                                        YerithWindowsCommons 	*associatedWindow);

    virtual inline ~YerithERPGenericSelectDBFieldDialog()
    {
        delete _currentPosition;
    }

    virtual void show();

    virtual QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

protected:

    virtual void closeEvent(QCloseEvent *closeEvent);


public:

    YerithWindowsCommons *_associatedWindow;

    QPoint *_currentPosition;
};


#endif /* YERITH_ERP_GENERIC_SELECT_DB_FIELDS_DIALOG_ */
