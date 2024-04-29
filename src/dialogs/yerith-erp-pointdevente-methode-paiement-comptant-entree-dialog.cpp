/*
 * yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp"

#include"src/yerith-erp-windows.hpp"

#include"src/utils/yerith-erp-utils.hpp"

#include <QtCore/QPoint>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QDialog>

#include <QtWidgets/QMainWindow>


const QString
YerithPOSVenteMethodePaiementComptantEntreeDialog::DIALOG_WINDOW_TITLE
(QString("%1 - %2").arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                        QObject::
                        trUtf8
                        ("Entrée du Montant de Paiement Comptant")));

YerithPOSVenteMethodePaiementComptantEntreeDialog::YerithPOSVenteMethodePaiementComptantEntreeDialog(YerithERPWindows *allWindows, YerithWindowsCommons *parent):YerithPOSDialogCommons
    (YerithPOSVenteMethodePaiementComptantEntreeDialog::DIALOG_WINDOW_TITLE),
    _currentPosition(new QPoint)
{
    setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

    YerithUtils::getCenterPosition(desktopWidget.width(),
                                   desktopWidget.height(),
                                   *this, *_currentPosition);

    setWindowTitle
    (YerithPOSVenteMethodePaiementComptantEntreeDialog::DIALOG_WINDOW_TITLE);

    setFixedSize(width(), height());

    lineEdit_vente_montant_recu->
    setValidator(&YerithUtils::POSITIVE_DoubleValidator);
}

void YerithPOSVenteMethodePaiementComptantEntreeDialog::valider()
{
    _allWindows->_pdVenteWindow->
    setMontantComptantRecu(lineEdit_vente_montant_recu->text().toDouble());

    if (-1 != _allWindows->_pdVenteWindow->effectuer_check_out_comptant())
    {
        rendreInvisible();
    }
    else
    {
        setFocus();
    }
}


void YerithPOSVenteMethodePaiementComptantEntreeDialog::show()
{
    move(*_currentPosition);

    clearLineEditValue();

    lineEdit_vente_montant_recu->setFocus();

    pushButton_annuler->enable(this, SLOT(annuler()));

    pushButton_valider->enable(this, SLOT(valider()));

    YerithPOSDialogCommons::show();
}


void YerithPOSVenteMethodePaiementComptantEntreeDialog::rendreInvisible()
{
    pushButton_annuler->disable(this);

    pushButton_valider->disable(this);

    YerithPOSDialogCommons::rendreInvisible();
}
