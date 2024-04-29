/*
 * yerith-erp-changer-utilisateur-dialog.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_CHANGER_UTILISATEUR_DIALOG_
#define YERITH_ERP_CHANGER_UTILISATEUR_DIALOG_

#include "../../ui_yerith-erp-changer-utilisateur-dialog.h"

#include "src/widgets/yerith-erp-push-button-PASSWORD.hpp"

#include "yerith-erp-dialog-commons.hpp"

class YerithERPWindows;
class YerithPOSUser;

class YerithPOSChangerUtilisateurDialog : public YerithPOSDialogCommons,
    									  private Ui_YerithPOSChangerUtilisateurDialog
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS YerithPOSChangerUtilisateurDialog(YerithERPWindows *allWindows,
                                                             QMainWindow *parent = 0);

    ~YerithPOSChangerUtilisateurDialog();

    virtual QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

//signals:

    //void disconnectUser();

public slots:
    void annuler();

    void valider();

    virtual void show();

protected:

    virtual void closeEvent(QCloseEvent *closeEvent);

private:

    YerithPOSUser *createUser(QSqlRecord &userRecord, int role);

    void checkCourriersAlertes();

    static const QString DIALOG_WINDOW_TITLE;

    QPoint *_currentPosition;

    YerithPushButtonPASSWORD *_push_button_password;

    YerithPOSUser *_user;

};


#endif /* YERITH_ERP_CHANGER_UTILISATEUR_DIALOG_ */
