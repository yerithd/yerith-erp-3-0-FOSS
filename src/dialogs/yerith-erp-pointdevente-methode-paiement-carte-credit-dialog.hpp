/*
 * yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_VENTE_METHODE_PAIEMENT_CARTE_CREDIT_DIALOG_HPP_
#define YERITH_ERP_VENTE_METHODE_PAIEMENT_CARTE_CREDIT_DIALOG_HPP_

#include "../../ui_yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.h"

#include "yerith-erp-dialog-commons.hpp"

class QPoint;

class YerithPOSCreditCardInfo;
class YerithWindowsCommons;

class YerithPOSVenteMethodePaiementCarteCreditDialog:public
    YerithPOSDialogCommons,
    public Ui_YerithPOSVenteMethodePaiementCarteCreditDialog
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS YerithPOSVenteMethodePaiementCarteCreditDialog(YerithWindowsCommons *
                                                                          parent);

    ~YerithPOSVenteMethodePaiementCarteCreditDialog();

    virtual QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

    inline void setCreditCardInfoDataStructure(YerithPOSCreditCardInfo *
                                               creditCardInfo)
    {
        _creditCardInfo = creditCardInfo;
    }

    void setCreditCardIssuingCompany(QString anIssuingCompany);

    void setCreditCardNameOnCard(QString aNameOnCreditCard);

    void setCreditCardPostalCode(QString aPostalCode);

    void setCreditCardNumber(QString aCardNumber);

    void setCreditCardExpiryDateYY(QString anExpiryDate_YY);

    void setCreditCardExpiryDateMM(QString anExpiryDate_MM);

    void setCreditCardCVCode(QString aCVCode);

    QString getCreditCardIssuingCompany();

    QString getCreditCardNameOnCard();

    QString getCreditCardPostalCode();

    QString getCreditCardNumber();

    QString getCreditCardExpiryDateYY();

    QString getCreditCardExpiryDateMM();

    QString getCreditCardCVCode();

public slots:
    void setupLineEdits();

    void reset_all_fields();

    virtual void show();

    virtual void rendreInvisible();

    bool checkAllMandatoryCreditCardFields();

    void valider();

    inline void annuler()
    {
        rendreInvisible();
    }

Q_SIGNALS:

    void SIGNAL_CREDIT_CARD_INFO_ALL_PROCESSED();

private:

    inline void emit_SIGNAL_CREDIT_CARD_INFO_ALL_PROCESSED()
    {
        emit SIGNAL_CREDIT_CARD_INFO_ALL_PROCESSED();
    }

    static const QString DIALOG_WINDOW_TITLE;

    QPoint *_currentPosition;

    /**
     *  _creditCardInfo isinstantiated in the class "YerithPOSVenteWindow",
     *  and set within the method
     * 	"YerithPOSVenteMethodePaiementDialog::paiement_carte_de_credit()".
     *
     */
    YerithPOSCreditCardInfo *_creditCardInfo;
};


#endif /* YERITH_ERP_VENTE_METHODE_PAIEMENT_CARTE_CREDIT_DIALOG_HPP_ */
