/*
 * yerith-erp-pointdevente-methode-paiement-dialog.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_VENTE_METHODE_PAIEMENT_DIALOG_HPP_
#define YERITH_ERP_VENTE_METHODE_PAIEMENT_DIALOG_HPP_

#include "../../ui_yerith-erp-pointdevente-methode-paiement-dialog.h"

#include "yerith-erp-dialog-commons.hpp"

class QPoint;

class YerithWindowsCommons;
class YerithERPWindows;

class YerithPOSVenteMethodePaiementDialog:public YerithPOSDialogCommons,
    private Ui_YerithPOSVenteMethodePaiementDialog
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS YerithPOSVenteMethodePaiementDialog(YerithERPWindows *allWindows,
                                                               YerithWindowsCommons *parent);

    inline ~YerithPOSVenteMethodePaiementDialog()
    {
        delete _currentPosition;
    }

    inline void setPushbuttonCompteClient__AVANCE_DE_CASH_Enabled(bool enabled)
    {
        pushButton_compte_client_AVANCE_DE_CASH->setEnabled(enabled);
    }

    virtual QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

public slots:
    virtual void show();

    void paiement_carte_de_credit();

    void paiement_comptant();

    void effectuer_checkout_compteclient_AVANCE_DE_CASH();

    void paiement_compteclient_AVANCE_DE_CASH();

private:

    static const QString DIALOG_WINDOW_TITLE;

    QPoint *_currentPosition;
};


#endif /* YERITH_ERP_VENTE_METHODE_PAIEMENT_DIALOG_HPP_ */
