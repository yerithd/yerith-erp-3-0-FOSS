/*
 * yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_VENTE_METHODE_PAIEMENT_COMPTANT_ENTREE_DIALOG_HPP_
#define YERITH_ERP_VENTE_METHODE_PAIEMENT_COMPTANT_ENTREE_DIALOG_HPP_

#include "../../ui_yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.h"

#include "yerith-erp-dialog-commons.hpp"

class QPoint;

class YerithWindowsCommons;
class YerithERPWindows;

class YerithPOSVenteMethodePaiementComptantEntreeDialog:public
    YerithPOSDialogCommons,
    public Ui_YerithPOSVenteMethodePaiementComptantEntreeDialog
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS YerithPOSVenteMethodePaiementComptantEntreeDialog(YerithERPWindows *
                                                                             allWindows,
                                                                             YerithWindowsCommons *
                                                                             parent);

    inline ~YerithPOSVenteMethodePaiementComptantEntreeDialog()
    {
        delete _currentPosition;
    }

    virtual QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

public slots:
    inline void clearLineEditValue()
    {
        lineEdit_vente_montant_recu->clear();
    }

    virtual void show();

    virtual void rendreInvisible();

    void valider();

    inline void annuler()
    {
        rendreInvisible();
    }

private:

    static const QString DIALOG_WINDOW_TITLE;

    QPoint *_currentPosition;

};


#endif /* YERITH_ERP_VENTE_METHODE_PAIEMENT_COMPTANT_ENTREE_DIALOG_HPP_ */
