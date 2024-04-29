/*
 * yerith-erp-payer-compte-client-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_PAYER_COMPTE_CLIENT_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_PAYER_COMPTE_CLIENT_WINDOW_HPP_

#include "../../../ui_yerith-erp-payer-compte-client-window.h"

#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;


class YerithERPPaymentProcessingInformation;
class YerithSqlTableModel;



class YerithPayerCompteClientWindow : public YerithWindowsCommons,
                                      private Ui_YerithPayerCompteClientWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithPayerCompteClientWindow();

    virtual inline ~YerithPayerCompteClientWindow()
    {
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_payer_compte_clientWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    virtual inline void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }

    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    void updateLineEdits();

    void rendreVisible(YerithSqlTableModel *clientTableModel,
                       YerithSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();


public slots:

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    inline void vendre_au_COMPTE_CLIENT_local()
    {
    	vendre_au_COMPTE_CLIENT(true);
    }

    void TOGGLE_encaissement_ET_facturation();

    void TOGGLE_transaction_financieres();

    void PUBLIC_slot_afficher_les_transactions_dun_client_WITH_ONE_LOCALISATION();

    void PUBLIC_slot_afficher_les_transactions_dun_client();


protected:

    virtual void hideEvent(QHideEvent *hideEvent);


protected slots:

    void handleComboBoxClients_Typedepaiement_TextChanged(const
                                                          QString
                                                          &
                                                          currentText);

private slots:

    void handleReferenceChange(const QString &
                               referenceEngagement);

    void activer_la_generation_des_factures_reliquat_de_paiement_client(const
                                                                        QString
                                                                        &);

    QString generer_FACTURE_RELIQUAT_DE_PAIEMENT();

    void afficher_detail_client();

    void updateStocksVenduTable(YerithERPPaymentProcessingInformation &
                                payment_processing_info);

    bool createPaymentForCustomerAccount(YerithERPPaymentProcessingInformation
                                         & payment_processing_info);

    void reinitialiser_donnees_de_paiement_au_compteclient();

    void reinitialiser_donnees_de_consultation_des_transactions_financieres();

    bool putCashIntoCustomerAccount();


private:

    void setupLineEdits();

    void setupLineEditsQCompleters();

    void setupDateTimeEdits();

    void populatePayerAuCompteClientsComboBoxes();


    YerithSqlTableModel *_curClientTableModel;

    QString _curCompanyName;

    double _curReferenceEngagementResteAPayer;
};

#endif /* SRC_WINDOWS_YERITH_PAYER_COMPTE_CLIENT_WINDOW_HPP_ */
