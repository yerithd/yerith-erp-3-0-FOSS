/*
 * yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp"

#include"src/yerith-erp-windows.hpp"

#include"src/utils/yerith-erp-utils.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"

#include "src/utils/credit-card/yerith-erp-carte-credit-info.hpp"

#include <QtCore/QPoint>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QDialog>

#include <QtWidgets/QMainWindow>


const QString YerithPOSVenteMethodePaiementCarteCreditDialog::DIALOG_WINDOW_TITLE(QString
                                                                                  ("%1 - %2").arg
                                                                                  (GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                                                                                   QObject::tr
                                                                                   ("PAIEMENT PAR CARTE DE CRÉDIT")));

YerithPOSVenteMethodePaiementCarteCreditDialog::YerithPOSVenteMethodePaiementCarteCreditDialog(YerithWindowsCommons *parent):YerithPOSDialogCommons
    (YerithPOSVenteMethodePaiementCarteCreditDialog::DIALOG_WINDOW_TITLE),
    _currentPosition(new QPoint), _creditCardInfo(0)
{
    setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

    YerithUtils::getCenterPosition(desktopWidget.width(),
                                   desktopWidget.height(),
                                   *this, *_currentPosition);

    setWindowTitle
    (YerithPOSVenteMethodePaiementCarteCreditDialog::DIALOG_WINDOW_TITLE);

    setFixedSize(width(), height());

    pushButton_annuler->disable(this);

    pushButton_valider->disable(this);

    setupLineEdits();
}


YerithPOSVenteMethodePaiementCarteCreditDialog::
~YerithPOSVenteMethodePaiementCarteCreditDialog()
{
    reset_all_fields();

    delete _currentPosition;
}


void
YerithPOSVenteMethodePaiementCarteCreditDialog::setCreditCardIssuingCompany
(QString anIssuingCompany)
{
    if (0 != _creditCardInfo)
    {
        _creditCardInfo->_issuingCompany = anIssuingCompany;
    }
}


void
YerithPOSVenteMethodePaiementCarteCreditDialog::setCreditCardNameOnCard
(QString aNameOnCard)
{
    if (0 != _creditCardInfo)
    {
        _creditCardInfo->_nameOnCard = aNameOnCard;
    }
}


void
YerithPOSVenteMethodePaiementCarteCreditDialog::setCreditCardPostalCode
(QString aPostalCode)
{
    if (0 != _creditCardInfo)
    {
        _creditCardInfo->_postalCode = aPostalCode;
    }
}


void YerithPOSVenteMethodePaiementCarteCreditDialog::setCreditCardNumber(QString
                                                                         aCardNumber)
{
    if (0 != _creditCardInfo)
    {
        _creditCardInfo->_cardNumber = aCardNumber;
    }
}


void
YerithPOSVenteMethodePaiementCarteCreditDialog::setCreditCardExpiryDateYY
(QString anExpiryDate_YY)
{
    if (0 != _creditCardInfo)
    {
        _creditCardInfo->_expiryDate_YY = anExpiryDate_YY;
    }
}


void
YerithPOSVenteMethodePaiementCarteCreditDialog::setCreditCardExpiryDateMM
(QString anExpiryDate_MM)
{
    if (0 != _creditCardInfo)
    {
        _creditCardInfo->_expiryDate_MM = anExpiryDate_MM;
    }
}


void YerithPOSVenteMethodePaiementCarteCreditDialog::setCreditCardCVCode(QString
                                                                         aCVCode)
{
    if (0 != _creditCardInfo)
    {
        _creditCardInfo->_cvCode = aCVCode;
    }
}


QString
YerithPOSVenteMethodePaiementCarteCreditDialog::getCreditCardIssuingCompany
()
{
    if (0 != _creditCardInfo)
    {
        return _creditCardInfo->_issuingCompany;
    }

    return "";
}


QString YerithPOSVenteMethodePaiementCarteCreditDialog::getCreditCardNameOnCard()
{
    if (0 != _creditCardInfo)
    {
        return _creditCardInfo->_nameOnCard;
    }

    return "";
}


QString YerithPOSVenteMethodePaiementCarteCreditDialog::getCreditCardPostalCode()
{
    if (0 != _creditCardInfo)
    {
        return _creditCardInfo->_postalCode;
    }

    return "";
}


QString YerithPOSVenteMethodePaiementCarteCreditDialog::getCreditCardNumber()
{
    if (0 != _creditCardInfo)
    {
        return _creditCardInfo->_cardNumber;
    }

    return "";
}


QString YerithPOSVenteMethodePaiementCarteCreditDialog::getCreditCardExpiryDateYY()
{
    if (0 != _creditCardInfo)
    {
        return _creditCardInfo->_expiryDate_YY;
    }

    return "";
}


QString YerithPOSVenteMethodePaiementCarteCreditDialog::getCreditCardExpiryDateMM()
{
    if (0 != _creditCardInfo)
    {
        return _creditCardInfo->_expiryDate_MM;
    }

    return "";
}


QString YerithPOSVenteMethodePaiementCarteCreditDialog::getCreditCardCVCode()
{
    if (0 != _creditCardInfo)
    {
        return _creditCardInfo->_cvCode;
    }

    return "";
}


void YerithPOSVenteMethodePaiementCarteCreditDialog::setupLineEdits()
{
    comboBox_carte_credit_societe->
    populateComboBoxRawString(YerithDatabase::CREDIT_CARD_ISSUING_COMPANY,
                              QString("credit_card_issuing_company"));

    lineEdit_carte_credit_numero->setInputMask("9999 - 9999 - 9999 - 9999");
    lineEdit_carte_credit_expiration_annee->setInputMask("99");
    lineEdit_carte_credit_expiration_mois->setInputMask("99");
    lineEdit_carte_credit_ccv->setInputMask("999");
}


void YerithPOSVenteMethodePaiementCarteCreditDialog::reset_all_fields()
{
    comboBox_carte_credit_societe->clearField();

    lineEdit_carte_credit_numero->clearField();
    lineEdit_carte_credit_expiration_annee->clearField();
    lineEdit_carte_credit_expiration_mois->clearField();
    lineEdit_carte_credit_nom->clearField();
    lineEdit_carte_credit_ccv->clearField();
}


void YerithPOSVenteMethodePaiementCarteCreditDialog::show()
{
    move(*_currentPosition);

    pushButton_annuler->enable(this, SLOT(annuler()));

    pushButton_valider->enable(this, SLOT(valider()));

    YerithPOSDialogCommons::show();
}


void YerithPOSVenteMethodePaiementCarteCreditDialog::rendreInvisible()
{
    pushButton_annuler->disable(this);

    pushButton_valider->disable(this);

    YerithPOSDialogCommons::rendreInvisible();
}


bool
YerithPOSVenteMethodePaiementCarteCreditDialog::
checkAllMandatoryCreditCardFields()
{
    bool
    cIssuingCompany = comboBox_carte_credit_societe->checkField();
    bool
    cCardNumber = lineEdit_carte_credit_numero->checkField();
    bool
    cExpiryDate_YY = lineEdit_carte_credit_expiration_annee->checkField();
    bool
    cExpiryDate_MM = lineEdit_carte_credit_expiration_mois->checkField();
    bool
    cNameOnCard = lineEdit_carte_credit_nom->checkField();
    bool
    cCvCode = lineEdit_carte_credit_ccv->checkField();


    return cIssuingCompany &&
           cCardNumber && cExpiryDate_YY && cExpiryDate_MM && cNameOnCard && cCvCode;
}


void YerithPOSVenteMethodePaiementCarteCreditDialog::valider()
{
    if (0 != _creditCardInfo && checkAllMandatoryCreditCardFields())
    {
        _creditCardInfo->_issuingCompany =
                        comboBox_carte_credit_societe->currentText();
        _creditCardInfo->_cardNumber = lineEdit_carte_credit_numero->text();
        _creditCardInfo->_expiryDate_YY =
                        lineEdit_carte_credit_expiration_annee->text();
        _creditCardInfo->_expiryDate_MM =
                        lineEdit_carte_credit_expiration_mois->text();
        _creditCardInfo->_nameOnCard = lineEdit_carte_credit_nom->text();
        _creditCardInfo->_cvCode = lineEdit_carte_credit_ccv->text();

        emit_SIGNAL_CREDIT_CARD_INFO_ALL_PROCESSED();
    }
    else
    {
        YerithQMessageBox::information(_allWindows->_pdVenteWindow,
                                       QObject::tr
                                       ("remplir tous les champs de texte"),
                                       QObject::tr
                                       ("S'il vous plait, remplir tous les champs de texte !"),
                                       QMessageBox::Ok);
    }
}
