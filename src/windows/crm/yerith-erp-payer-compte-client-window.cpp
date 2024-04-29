/*
 * yerith-erp-payer-compte-client-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-payer-compte-client-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-payment-processing-information.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerithPayerCompteClientWindow::YerithPayerCompteClientWindow()
:YerithWindowsCommons(),
 _curReferenceEngagementResteAPayer(0.0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("payer au compte client"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
                        .arg(COLOUR_RGB_STRING_YERITH_GOLD_245_194_17,
                             COLOUR_RGB_STRING_YERITH_BLUE_53_132_228);


    comboBox_comptes_clients_reference->setYerithEditable(true);

    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_reference
        ->setYerithEditable(true);

    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale
        ->setYerithEditable(true);


    checkBox_ne_pas_inclure_les_ventes_comptant->setChecked(true);


    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    comboBox_clients_typedepaiement->setYerithEnabled(false);

    populatePayerAuCompteClientsComboBoxes();

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_payer->disable(this);

    pushButton_clients->disable(this);
    pushButton_entrer->disable(this);
    pushButton_paiements->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);


    pushButton_transactions_compte_client_consulter->disable(this);
    pushButton_transactions_compte_client_reinitialiser_consultation->disable(this);



    connect(actionAFFICHER_encaissement_et_facturation,
    		SIGNAL(triggered()),
			this,
            SLOT(TOGGLE_encaissement_ET_facturation()));

    connect(actionConsulter_des_transactions_financieres,
    		SIGNAL(triggered()),
			this,
            SLOT(TOGGLE_transaction_financieres()));


    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));

    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));

    connect(actionReinitialiser,
            SIGNAL(triggered()),
            this,
            SLOT(reinitialiser_donnees_de_paiement_au_compteclient()));

    connect(actionAfficherPDF,
            SIGNAL(triggered()),
            this,
            SLOT(generer_FACTURE_RELIQUAT_DE_PAIEMENT()));

    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));

    connect(actionGENERER_FACTURE_RELIQUAT_DE_PAIEMENT,
            SIGNAL(triggered()),
            this,
            SLOT(generer_FACTURE_RELIQUAT_DE_PAIEMENT()));

    connect(actionEntrer, SIGNAL(triggered()), this,SLOT(vendre_au_COMPTE_CLIENT()));
    connect(actionPaiements, SIGNAL(triggered()), this, SLOT(paiements()));
    connect(actionAfficherDetailsCompteClient, SIGNAL(triggered()), this, SLOT(afficher_detail_client()));
    connect(actionPayerAuCompteClient, SIGNAL(triggered()), this, SLOT(putCashIntoCustomerAccount()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


    activer_la_generation_des_factures_reliquat_de_paiement_client
    (YerithUtils::EMPTY_STRING);


    connect(comboBox_comptes_clients_reference,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT
            (activer_la_generation_des_factures_reliquat_de_paiement_client
             (const QString &)));

    connect(comboBox_clients_typedepaiement,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handleComboBoxClients_Typedepaiement_TextChanged
                 (const QString &)));


    groupBox_TRANSACTIONS_FINANCIERES->setVisible(true);

    groupBox_ENCAISSEMENT_ET_FACTURATION->setVisible(true);

}


void YerithPayerCompteClientWindow::hideEvent(QHideEvent *hideEvent)
{
    _allWindows->_transactionsDunClientWindow->close();
}


void YerithPayerCompteClientWindow::
        handleComboBoxClients_Typedepaiement_TextChanged(const QString &currentText)
{
    if (YerithUtils::isEqualCaseInsensitive(currentText,
                                            YerithUtils::_typedepaiementToUserViewString
                                                .value(YerithUtils::ENCAISSEMENT_BANCAIRE)) ||
        YerithUtils::isEqualCaseInsensitive(currentText,
                                            YerithUtils::_typedepaiementToUserViewString
                                                .value(YerithUtils::ENCAISSEMENT_TELEPHONE)) ||
        YerithUtils::isEqualCaseInsensitive(currentText,
                                            YerithUtils::_typedepaiementToUserViewString
                                                .value(YerithUtils::ENCAISSEMENT_VIREMENT_BANCAIRE)))
    {
        comboBox_clients_intitule_du_compte_bancaire->setYerithEnabled(true);
    }
    else
    {
        comboBox_clients_intitule_du_compte_bancaire->setYerithEnabled(false);
    }
}


void
YerithPayerCompteClientWindow::activer_la_generation_des_factures_reliquat_de_paiement_client
(const QString &currentText)
{
    if (!currentText.isEmpty())
    {
        actionAfficherPDF->setVisible(true);

        actionGENERER_FACTURE_RELIQUAT_DE_PAIEMENT->setVisible(true);

        pushButton_facture->enable(this,
                                   SLOT(generer_FACTURE_RELIQUAT_DE_PAIEMENT
                                        ()));
    }
    else
    {
        actionAfficherPDF->setVisible(false);

        actionGENERER_FACTURE_RELIQUAT_DE_PAIEMENT->setVisible(false);

        pushButton_facture->disable(this);
    }
}


QString YerithPayerCompteClientWindow::generer_FACTURE_RELIQUAT_DE_PAIEMENT()
{
    YERITH_CLIENT_ACCOUNT_SALE_info sale_info;


    YerithSqlTableModel &stocksVenduTableModel =
                    _allWindows->getSqlTableModel_stocks_vendu();

    QString
    stocksVenduFilter(QString("%1 = '%2' AND %3 = '%4' ").arg
                      (YerithDatabaseTableColumn::REFERENCE,
                       comboBox_comptes_clients_reference->currentText(),
                       YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                       lineEdit_comptes_clients_designation_de_lentreprise->
                       text()));

//      QDEBUG_STRING_OUTPUT_2("stocksVenduFilter", stocksVenduFilter);

    stocksVenduTableModel.yerithSetFilter_WITH_where_clause(stocksVenduFilter);

    int rowCount =
    		stocksVenduTableModel.easySelect("src/windows/crm/yerith-erp-payer-compte-client-windows.cpp", 236);

    if (rowCount > 0)
    {
        YerithPOSUser *user = _allWindows->getUser();

        if (0 != user)
        {
            sale_info._site_of_invoice_emission = user->localisation();
        }

        QSqlRecord stocksVenduRecord = stocksVenduTableModel.record(0);

        sale_info._sale_type =
                        YerithUtils::_typedeventeToUserViewString.value(GET_SQL_RECORD_DATA
                                                                        (stocksVenduRecord,
                                                                         YerithDatabaseTableColumn::TYPE_DE_VENTE).toInt
                                                                        ());

        sale_info._db_ID =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::ID);

        sale_info._taxes =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::MONTANT_TVA);
        sale_info._quantity =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::QUANTITE_VENDUE);
        sale_info._unit_price =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::PRIX_UNITAIRE);
        sale_info._category =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::CATEGORIE);
        sale_info._product_reference =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::REFERENCE);
        sale_info._product_name =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::DESIGNATION);
        sale_info._total_remaining_amount =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::MONTANT_A_REMBOURSER);
        sale_info._total_amount_ATC =
                        GET_SQL_RECORD_DATA(stocksVenduRecord,
                                            YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE);

        if (_curClientTableModel->select())
        {
            QSqlRecord aCLIENTQSqlRecord;

            _allWindows->
            _clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
            (aCLIENTQSqlRecord);

            sale_info._client_city =
                            GET_SQL_RECORD_DATA(aCLIENTQSqlRecord,
                                                YerithDatabaseTableColumn::VILLE);
            sale_info._client_email =
                            GET_SQL_RECORD_DATA(aCLIENTQSqlRecord,
                                                YerithDatabaseTableColumn::EMAIL);
            sale_info._client_name =
                            GET_SQL_RECORD_DATA(aCLIENTQSqlRecord,
                                                YerithDatabaseTableColumn::NOM_ENTREPRISE);
            sale_info._client_representant_name =
                            GET_SQL_RECORD_DATA(aCLIENTQSqlRecord,
                                                YerithDatabaseTableColumn::NOM_REPRESENTANT);
            sale_info._client_phone =
                            GET_SQL_RECORD_DATA(aCLIENTQSqlRecord,
                                                YerithDatabaseTableColumn::
                                                NUMERO_TELEPHONE_1);
            sale_info._client_po_box =
                            GET_SQL_RECORD_DATA(aCLIENTQSqlRecord,
                                                YerithDatabaseTableColumn::BOITE_POSTALE);
        }

        stocksVenduTableModel.resetFilter();

        QString
        pdfReceiptFileName =
                        YerithUtils::IMPRIMER_FACTURE_FOR_CLIENT_ACCOUNT_SALE_INVOICE_FACTURE
                        (*this, sale_info);

        if (!pdfReceiptFileName.isEmpty())
        {
            return YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
        }
    }

    stocksVenduTableModel.resetFilter();

    return YerithUtils::EMPTY_STRING;
}


void YerithPayerCompteClientWindow::handleReferenceChange(const QString &
                                                          referenceEngagement)
{
    if (!referenceEngagement.isEmpty())
    {
        comboBox_clients_typedepaiement->setYerithEnabled(true);

        QString aConditionStr;

        YerithSqlTableModel &stocksVenduSqlTableModel =
                        _allWindows->getSqlTableModel_stocks_vendu();

        aConditionStr =
                        QString("%1 = '%2' AND %3 = '%4'").
                        arg(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                            lineEdit_comptes_clients_designation_de_lentreprise->text(),
                            YerithDatabaseTableColumn::REFERENCE, referenceEngagement);

        stocksVenduSqlTableModel.yerithSetFilter_WITH_where_clause
        (aConditionStr);

        int rows =
        		stocksVenduSqlTableModel
					.easySelect("src/windows/crm/yerith-erp-payer-compte-client-windows.cpp", 354);

        if (rows > 0)
        {
            int firstRecord = 0;

            QSqlRecord aStocksVenduRecord =
                            stocksVenduSqlTableModel.record(firstRecord);

            lineEdit_clients_payer_designation->setText(GET_SQL_RECORD_DATA
                                                        (aStocksVenduRecord,
                                                         YerithDatabaseTableColumn::DESIGNATION));

            _curReferenceEngagementResteAPayer =
                            GET_SQL_RECORD_DATA(aStocksVenduRecord,
                                                YerithDatabaseTableColumn::
                                                MONTANT_A_REMBOURSER).toDouble();
        }
        else
        {
            _curReferenceEngagementResteAPayer = 0.0;
        }

        stocksVenduSqlTableModel.resetFilter();

        /*
         * WE HAVE TO AVOID READING THIS QLINEEDIT TO
         * PERFORM INTERNAL COMPUTATIONS.
         */
        lineEdit_comptes_clients_reference_reste_a_payer->setText
        (GET_CURRENCY_STRING_NUM((-1 * _curReferenceEngagementResteAPayer)));
    }
    else
    {
        comboBox_clients_typedepaiement->setYerithEnabled(false);

        return;
    }
}


void YerithPayerCompteClientWindow::afficher_detail_client()
{
    rendreInvisible();

    _allWindows->_clientsDetailWindow->rendreVisible(_curClientTableModel,
                                                     _curStocksTableModel);
}


void
YerithPayerCompteClientWindow::updateStocksVenduTable
(YerithERPPaymentProcessingInformation &payment_processing_info)
{
    YerithSqlTableModel &stocksVenduTableModel =
                    _allWindows->getSqlTableModel_stocks_vendu();

    QString
    stocksVenduFilter(QString("%1 = '%2' AND %3 = '%4' ").arg
                      (YerithDatabaseTableColumn::REFERENCE,
                       payment_processing_info._reference,
                       YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                       payment_processing_info._nom_entreprise));

    stocksVenduTableModel.yerithSetFilter_WITH_where_clause(stocksVenduFilter);

    int rowCount =
    		stocksVenduTableModel
				.easySelect("src/windows/crm/yerith-erp-payer-compte-client-windows.cpp", 422);

    if (rowCount > 0)
    {
        double
        montantPaye = payment_processing_info._montant_paye;

        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        QSqlRecord
        stocksVenduRecord = stocksVenduTableModel.record(0);

        double
        montantARembourser = GET_SQL_RECORD_DATA(stocksVenduRecord,
                                                 YerithDatabaseTableColumn::MONTANT_A_REMBOURSER).
                             toDouble();

        double
        nouveau_montant_a_rembourser = montantARembourser - montantPaye;

        if (nouveau_montant_a_rembourser <= 0)
        {
            nouveau_montant_a_rembourser = 0.0;
        }

        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_A_REMBOURSER,
                 QString::number(nouveau_montant_a_rembourser));

        stocksVenduTableModel.updateRecord(0,
        								   stocksVenduRecord,
										   "src/windows/crm/yerith-erp-payer-compte-client-window.cpp",
										   453);

        stocksVenduTableModel.resetFilter();

        if (0 == nouveau_montant_a_rembourser)
        {
            QString
            stocksVenduID(GET_SQL_RECORD_DATA
                          (stocksVenduRecord,
                           YerithDatabaseTableColumn::ID));

            //copy row from 'stocksVendu' to 'services_completes'
            QString
            copyRowQuery(QString("DROP TABLE IF EXISTS TempData;"
                                 "CREATE TEMPORARY TABLE TempData LIKE %1;"
                                 "INSERT INTO TempData SELECT * FROM %2 WHERE id = '%3';"
                                 "ALTER TABLE TempData CHANGE COLUMN `id` `id` INT(11) NULL, DROP PRIMARY KEY;"
                                 "UPDATE TempData SET id = %4;"
                                 "INSERT INTO %5 SELECT * FROM TempData;"
                                 "DROP TABLE IF EXISTS TempData;").arg
                         (YerithDatabase::STOCKS_VENDU,
                          YerithDatabase::STOCKS_VENDU, stocksVenduID,
                          QString::number
                          (YerithERPWindows::
                           getNextIdSqlTableModel_services_completes()),
                          YerithDatabase::SERVICES_COMPLETES));

            if (YerithUtils::execQuery(copyRowQuery))
            {
                //handle stocksVendu table
                QString
                removeStocksVenduRowQuery(QString
                                          ("DELETE FROM %1 WHERE %2='%3'").
                                          arg(YerithDatabase::STOCKS_VENDU,
                                              YerithDatabaseTableColumn::ID,
                                              stocksVenduID));

                if (YerithUtils::execQuery(removeStocksVenduRowQuery))
                {
                    //handle marchandise table
                    QString
                    removeMarchandisesRowQuery(QString
                                               ("DELETE FROM %1 WHERE %2='%3' AND %4='%5'").
                                               arg(YerithDatabase::
                                                   MARCHANDISES,
                                                   YerithDatabaseTableColumn::
                                                   REFERENCE,
                                                   payment_processing_info.
                                                   _reference,
                                                   YerithDatabaseTableColumn::
                                                   NOM_ENTREPRISE_CLIENT,
                                                   payment_processing_info.
                                                   _nom_entreprise));

                    YerithUtils::execQuery(removeMarchandisesRowQuery);
                }
            }
        }

        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


bool
YerithPayerCompteClientWindow::createPaymentForCustomerAccount
(YerithERPPaymentProcessingInformation &payment_processing_info)
{
    bool
    success = payment_processing_info.save_payment_info_record();

    if (success)
    {
        updateStocksVenduTable(payment_processing_info);
    }

    return success;
}


void
YerithPayerCompteClientWindow::
reinitialiser_donnees_de_paiement_au_compteclient()
{
    textEdit_description->clear();
    lineEdit_clients_payer_designation->myClear();
    lineEdit_montant_a_payer->myClear();
    comboBox_comptes_clients_reference->resetYerithComboBox();
    comboBox_clients_typedepaiement->resetYerithComboBox();
    comboBox_clients_intitule_du_compte_bancaire->resetYerithComboBox();
}


void
YerithPayerCompteClientWindow::
reinitialiser_donnees_de_consultation_des_transactions_financieres()
{
    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_reference->
    resetYerithComboBox();

    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
    resetYerithComboBox();

    dateEdit_transactions_compte_client_debut->reset();
    dateEdit_transactions_compte_client_fin->reset();
}


bool YerithPayerCompteClientWindow::putCashIntoCustomerAccount()
{
    if (comboBox_comptes_clients_reference->currentText().isEmpty())
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("référence"),
                                       QObject::tr
                                       ("Veuillez entrer un engagement (référence) !"));

        return false;
    }

    if (lineEdit_montant_a_payer->text().isEmpty())
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("montant à verser"),
                                       QObject::tr
                                       ("Veuillez entrer le montant à verser !"));

        return false;
    }

    if (comboBox_clients_typedepaiement->currentText().isEmpty())
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("montant à verser"),
                                       QObject::tr
                                       ("Veuillez choisir le mode de paiement !"));

        return false;
    }

    if (comboBox_clients_intitule_du_compte_bancaire->isEnabled() &&
            comboBox_clients_intitule_du_compte_bancaire->currentText().isEmpty())
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("montant à verser"),
                                       QObject::tr
                                       ("Vous n'avez choisi aucun compte bancaire !"));
    }


    YerithSqlTableModel &clientsTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString clientFilter;
    clientFilter.append(QString("%1 = '%2'").arg
                        (YerithDatabaseTableColumn::NOM_ENTREPRISE,
                         _curCompanyName));

    clientsTableModel.yerithSetFilter_WITH_where_clause(clientFilter);

    bool success = false;

    double compte_client = 0.0;

    int clientsTableModelRowCount =
    		clientsTableModel
				.easySelect("src/windows/crm/yerith-erp-payer-compte-client-windows.cpp", 617);

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);

        compte_client =
                        GET_SQL_RECORD_DATA(clientsRecord,
                                            YerithDatabaseTableColumn::COMPTE_CLIENT).
                        toDouble();

        double cashPaymentAmount =
                        lineEdit_montant_a_payer->text().toDouble();

        compte_client = compte_client + cashPaymentAmount;

        clientsTableModel.resetFilter();

        QString msg;

        if (cashPaymentAmount <= 0)
        {
            msg =
                            QObject::tr
                            ("Le montant donné doit être supérieur à %1 !").arg
                            (GET_CURRENCY_STRING_NUM(cashPaymentAmount));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("annulation du paiement"), msg,
                                           QMessageBox::Ok);

            return false;
        }

        if (cashPaymentAmount > _curReferenceEngagementResteAPayer)
        {
            msg =
                            QObject::tr
                            ("Le montant donné (%1) ne doit pas être supérieur à la dette restante !").
                            arg(GET_CURRENCY_STRING_NUM(cashPaymentAmount));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("annulation du paiement"), msg,
                                           QMessageBox::Ok);

            return false;
        }
        else
        {
            msg =
                            QObject::tr("Poursuivre avec le paiement de %1 ?").arg
                            (GET_CURRENCY_STRING_NUM(cashPaymentAmount));

            if (QMessageBox::Cancel ==
                    YerithQMessageBox::question(this, QObject::tr("paiement"),
                                                msg,
                                                QMessageBox::Cancel,
                                                QMessageBox::Ok))
            {
                YerithQMessageBox::information(this,
                                               QObject::tr
                                               ("annulation du paiement"),
                                               QObject::tr
                                               ("Vous avez annulé le paiement en cours !"),
                                               QMessageBox::Ok);

                return false;
            }
            else
            {
            }
        }


        double dette_maximale_compte_client =
                        GET_SQL_RECORD_DATA(clientsRecord,
                                            YerithDatabaseTableColumn::
                                            DETTE_MAXIMALE_COMPTE_CLIENT).toDouble();

        double nouvelle_dette_maximale_compte_client =
                        dette_maximale_compte_client - cashPaymentAmount;

        if (nouvelle_dette_maximale_compte_client < 0)
        {
            nouvelle_dette_maximale_compte_client = 0.0;
        }

        QString queryStr;

        queryStr.append(QString
                        ("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'").
                        arg(YerithDatabase::CLIENTS,
                            YerithDatabaseTableColumn::COMPTE_CLIENT,
                            QString::number(compte_client),
                            YerithDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
                            QString::number
                            (nouvelle_dette_maximale_compte_client),
                            YerithDatabaseTableColumn::NOM_ENTREPRISE,
                            _curCompanyName));

        YerithERPPaymentProcessingInformation payment_processing_info;

        payment_processing_info._NOTES = textEdit_description->toPlainText();

        payment_processing_info._designation =
                        lineEdit_clients_payer_designation->text();

        payment_processing_info._reference =
                        comboBox_comptes_clients_reference->currentText();

        payment_processing_info._type_de_paiement =
                        YerithUtils::getComboBoxDatabaseQueryValue
                        (comboBox_clients_typedepaiement->currentText(),
                         YerithUtils::_typedepaiementToUserViewString);

        payment_processing_info._nom_entreprise = _curCompanyName;


        {
            QString compteClient_PROGRAMME_DE_FIDELITE_CLIENTS_inchange =
                            QString("select %1 from %2 where %3='%4'").arg
                            (YerithDatabaseTableColumn::
                             COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                             YerithDatabase::CLIENTS,
                             YerithDatabaseTableColumn::NOM_ENTREPRISE,
                             _curCompanyName);

            QSqlQuery a_qsql_query;

            int query_size = YerithUtils::execQuery(a_qsql_query,
                                                    compteClient_PROGRAMME_DE_FIDELITE_CLIENTS_inchange);

            if (query_size > 0 && a_qsql_query.next())
            {
                payment_processing_info.
                _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS =
                                a_qsql_query.
                                value(YerithDatabaseTableColumn::
                                      COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS).toDouble();
            }
        }


        payment_processing_info._nouveau_compte_client = compte_client;

        payment_processing_info._montant_paye = cashPaymentAmount;

        payment_processing_info._paiement_intitule_compte_bancaire =
                        comboBox_clients_intitule_du_compte_bancaire->currentText();

        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        success = YerithUtils::execQuery(queryStr);

        success = success
                  && createPaymentForCustomerAccount(payment_processing_info);

        if (success)
        {
            YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
        }
        else
        {
            YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;
        }
    }
    else
    {
        QString msg(QObject::tr
                    ("l'entreprise '%1' n'existe pas dans la base de données !").
                    arg(_curCompanyName));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("entreprise non existante"), msg);
    }

    if (true == success)
    {
        QString
        msg(QObject::tr
            ("%1 (%2) a été ajouté au compte client %3").arg
            (lineEdit_montant_a_payer->text(), YerithERPConfig::currency,
             _curCompanyName));

        YerithQMessageBox::information(this,
                                       QObject::tr("paiement effectué"),
                                       msg);

        textEdit_description->clear();

        lineEdit_comptes_clients_reference_reste_a_payer->clear();

        lineEdit_clients_payer_designation->myClear();

        lineEdit_montant_a_payer->clear();

        comboBox_comptes_clients_reference->resetYerithComboBox();
        comboBox_clients_typedepaiement->resetYerithComboBox();
        comboBox_clients_intitule_du_compte_bancaire->resetYerithComboBox();

        populatePayerAuCompteClientsComboBoxes();
//              setupLineEditsQCompleters();

        updateLineEdits();
    }
    else
    {
        QString msg(QObject::tr
                    ("Erreur lors du paiement '%1 (%2)' pour le compte client '%3'").
                    arg(lineEdit_montant_a_payer->text(),
                        YerithERPConfig::currency, _curCompanyName));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("paiement pas effectué"), msg);
    }

    return success;
}


void YerithPayerCompteClientWindow::setupLineEdits()
{
    lineEdit_comptes_clients_reference_reste_a_payer->setYerithEnabled(false);

    lineEdit_comptes_clients_designation_de_lentreprise->setYerithEnabled
    (false);

    lineEdit_comptes_clients_valeur_compte_client_AVANCE_DE_CASH->
    setYerithEnabled(false);

    lineEdit_comptes_clients_valeur_compte_client_FIDELITE->setYerithEnabled
    (false);

    lineEdit_comptes_clients_valeur_compte_client->setYerithEnabled(false);

    lineEdit_comptes_clients_valeur_dette_maximale->setYerithEnabled(false);

    lineEdit_comptes_clients_numero_du_contribuable->setYerithEnabled(false);

    lineEdit_comptes_clients_reference_registre_du_commerce->setYerithEnabled
    (false);

    lineEdit_comptes_clients_siege_social->setYerithEnabled(false);

    lineEdit_clients_payer_designation->setYerithEnabled(false);

    lineEdit_montant_a_payer->
    setPlaceholderText(QObject::tr("montant à payer"));

    lineEdit_montant_a_payer->
    setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_comptes_clients_designation_de_lentreprise->setText
    (_curCompanyName);
}


void YerithPayerCompteClientWindow::setupLineEditsQCompleters()
{
//      QString aConditionStr(YerithUtils::generateSqlIs(YerithDatabaseTableColumn::TYPE_DE_VENTE,
//                                                                                                       QString::number(YerithUtils::VENTE_COMPTE_CLIENT)));
//
//      aConditionStr.append(QString(" AND %1 = '%2' AND %3 > '0'")
//                                                      .arg(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
//                                                               _curCompanyName,
//                                                               YerithDatabaseTableColumn::MONTANT_A_REMBOURSER));
}


void YerithPayerCompteClientWindow::setupDateTimeEdits()
{
    dateEdit_transactions_compte_client_debut->
    setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_transactions_compte_client_fin->setStartDate(GET_CURRENT_DATE);
}


void YerithPayerCompteClientWindow::populatePayerAuCompteClientsComboBoxes()
{
    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
    populateComboBoxRawString(YerithDatabase::LOCALISATIONS,
                              YerithDatabaseTableColumn::NOM_LOCALISATION);

    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
    addItem_AFTER_POPULATE(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_reference->
    populateComboBoxRawString(YerithDatabase::STOCKS_VENDU,
                              YerithDatabaseTableColumn::REFERENCE,
                              QString("%1='%2' AND %3 > 0").
                              arg(YerithDatabaseTableColumn::
                                  NOM_ENTREPRISE_CLIENT, _curCompanyName,
                                  YerithDatabaseTableColumn::
                                  MONTANT_A_REMBOURSER));

    comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_reference->
    populateComboBoxRawString_APPEND(YerithDatabase::SERVICES_COMPLETES,
                                     YerithDatabaseTableColumn::REFERENCE,
                                     QString("%1='%2'").
                                     arg(YerithDatabaseTableColumn::
                                         NOM_ENTREPRISE_CLIENT,
                                         _curCompanyName));


    QString
    aConditionStr(YerithUtils::generateSqlIs
                  (YerithDatabaseTableColumn::TYPE_DE_VENTE,
                   QString::number(YerithUtils::VENTE_COMPTE_CLIENT)));

    aConditionStr.append(QString(" AND %1 = '%2' AND %3 > '0'").arg
                         (YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                          _curCompanyName,
                          YerithDatabaseTableColumn::MONTANT_A_REMBOURSER));

    comboBox_comptes_clients_reference->populateComboBoxRawString
    (YerithDatabase::STOCKS_VENDU, YerithDatabaseTableColumn::REFERENCE,
     aConditionStr);


    comboBox_clients_intitule_du_compte_bancaire->setYerithEnabled(false);

    comboBox_clients_typedepaiement->
    setupPopulateNORawString(YerithDatabase::TYPE_DE_PAIEMENT,
                             YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
                             &YerithUtils::_typedepaiementToUserViewString);


    comboBox_comptes_clients_reference->checkField();


    QList < int >content_to_delete_from_user_view_int_values_IN_OUT;

    content_to_delete_from_user_view_int_values_IN_OUT
            << YerithUtils::DECAISSEMENT_CHEQUE
            << YerithUtils::DECAISSEMENT_COMPTANT
            << YerithUtils::DECAISSEMENT_TELEPHONE
            << YerithUtils::DECAISSEMENT_BANCAIRE
            << YerithUtils::DECAISSEMENT_VIREMENT_BANCAIRE
            << YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT
            << YerithUtils::DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT
            << YerithUtils::ENCAISSEMENT_AVANCE_DE_CASH
            << YerithUtils::ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE;

    comboBox_clients_typedepaiement->populateComboBoxWithout
    (content_to_delete_from_user_view_int_values_IN_OUT);

    comboBox_clients_intitule_du_compte_bancaire->populateComboBoxRawString
    (YerithDatabase::COMPTES_BANCAIRES,
     YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);
}


void YerithPayerCompteClientWindow::updateLineEdits()
{
    lineEdit_comptes_clients_reference_reste_a_payer->clear();

    if (_curClientTableModel->select())
    {
        QSqlRecord aQSqlRecord;

        _allWindows->
        _clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
        (aQSqlRecord);

        double compteClient_AVANCE_DE_CASH = GET_SQL_RECORD_DATA(aQSqlRecord,
                                                                 YerithDatabaseTableColumn::
                                                                 COMPTECLIENT_AVANCE_DE_CASH).toDouble
                                             ();

        lineEdit_comptes_clients_valeur_compte_client_AVANCE_DE_CASH->setText
        (GET_CURRENCY_STRING_NUM(compteClient_AVANCE_DE_CASH));


        double compteClient_LOYALTY = GET_SQL_RECORD_DATA(aQSqlRecord,
                                                          YerithDatabaseTableColumn::
                                                          COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS).
                                      toDouble();

        lineEdit_comptes_clients_valeur_compte_client_FIDELITE->setText
        (GET_CURRENCY_STRING_NUM(-1.0 * compteClient_LOYALTY));


        double compteClient = GET_SQL_RECORD_DATA(aQSqlRecord,
                                                  YerithDatabaseTableColumn::COMPTE_CLIENT).
                              toDouble();

        lineEdit_comptes_clients_valeur_compte_client->setText
        (GET_CURRENCY_STRING_NUM(compteClient));

        double detteMaximale = GET_SQL_RECORD_DATA(aQSqlRecord,
                                                   YerithDatabaseTableColumn::
                                                   DETTE_MAXIMALE_COMPTE_CLIENT).toDouble
                               ();

        lineEdit_comptes_clients_valeur_dette_maximale->setText
        (GET_CURRENCY_STRING_NUM(detteMaximale));

        lineEdit_comptes_clients_reference_registre_du_commerce->setText
        (GET_SQL_RECORD_DATA
         (aQSqlRecord,
          YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

        lineEdit_comptes_clients_numero_du_contribuable->setText
        (GET_SQL_RECORD_DATA
         (aQSqlRecord, YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE));

        lineEdit_comptes_clients_siege_social->setText(GET_SQL_RECORD_DATA
                                                       (aQSqlRecord,
                                                        YerithDatabaseTableColumn::SIEGE_SOCIAL));
    }
}


void YerithPayerCompteClientWindow::rendreVisible(YerithSqlTableModel *clientTableModel,
                                                  YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curClientTableModel = clientTableModel;

    _curStocksTableModel = stocksTableModel;

    QSqlRecord aQSqlRecord;

    _allWindows->_clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(aQSqlRecord);

    _curCompanyName = GET_SQL_RECORD_DATA(aQSqlRecord,
                                          YerithDatabaseTableColumn::NOM_ENTREPRISE);

    setupLineEdits();

    populatePayerAuCompteClientsComboBoxes();

    setVisible(true);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("payer au compte client"),
    								 _curClientTableModel);

    updateLineEdits();

    QVariant img(aQSqlRecord.value(YerithDatabaseTableColumn::IMAGE_COMPTE_CLIENT));

    if (!img.isNull())
    {
        YerithUtils::loadPixmapFromDB(*label_image_client, img, "JPG");
    }
    else
    {
        YerithWindowsCommons::supprimer_image(*label_image_client);
    }

    connect(comboBox_comptes_clients_reference,
            SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(handleReferenceChange(const QString &)));

    dateEdit_transactions_compte_client_debut->setFocus();
}


void YerithPayerCompteClientWindow::rendreInvisible()
{
    _curCompanyName.clear();

    textEdit_description->clear();

    lineEdit_comptes_clients_reference_reste_a_payer->clear();

    lineEdit_clients_payer_designation->myClear();

    lineEdit_montant_a_payer->clear();

    comboBox_comptes_clients_reference->resetYerithComboBox();

    comboBox_clients_intitule_du_compte_bancaire->resetYerithComboBox();

    comboBox_clients_typedepaiement->resetYerithComboBox();

    YerithWindowsCommons::rendreInvisible();
}


void YerithPayerCompteClientWindow::TOGGLE_encaissement_ET_facturation()
{
	if (0 != groupBox_ENCAISSEMENT_ET_FACTURATION)
	{
		bool isVisible = groupBox_ENCAISSEMENT_ET_FACTURATION->isVisible();

		groupBox_ENCAISSEMENT_ET_FACTURATION->setVisible(!isVisible);
	}
}


void YerithPayerCompteClientWindow::TOGGLE_transaction_financieres()
{
	if (0 != groupBox_TRANSACTIONS_FINANCIERES)
	{
		bool isVisible = groupBox_TRANSACTIONS_FINANCIERES->isVisible();

		groupBox_TRANSACTIONS_FINANCIERES->setVisible(!isVisible);
	}
}


void
YerithPayerCompteClientWindow::
PUBLIC_slot_afficher_les_transactions_dun_client_WITH_ONE_LOCALISATION()
{
//      qDebug() << QString("_clientLastSelectedRow: %1")
//                                      .arg(QString::number(_clientLastSelectedRow));

    int
    aClientLastSelectedRow =
                    _allWindows->_clientsWindow->getLastListerSelectedRow__ID_AS_INTEGER();

    if (0 != _curClientTableModel &&
            _curClientTableModel->rowCount() > 0 && aClientLastSelectedRow > -1)
    {
        QSqlRecord
        record;

        _allWindows->
        _clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
        (record);

//          int stockId = GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID).toInt();

        QString
        clientCompanyName(GET_SQL_RECORD_DATA
                          (record,
                           YerithDatabaseTableColumn::NOM_ENTREPRISE));

//          YerithHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
//                                                                                               stockReference,
//                                                                                                       QString::number(stockId),
//                                                                                                       stockDesignation);


        QString
        clientTransactionsPaiementsQueryStr;

        QString
        clientTransactionsServicesCompletesQueryStr;

        QString
        clientTransactionsStockVenduQueryStr;

        QString
        paiementComptantQueryStr;


        QString
        CURRENT_PAYMENT_REFERENCE =
                        comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_reference->
                        currentText();

        if (!CURRENT_PAYMENT_REFERENCE.isEmpty())
        {
            clientTransactionsPaiementsQueryStr.append(QString
                                                       ("select nom_entreprise, "
                                                        "%1 as 'Date transaction', "
                                                        "%2 as 'Heure transaction', "
                                                        "%3 as 'Type de paiement', "
                                                        "%4 as 'Total transaction', "
                                                        "compte_client as 'Compte client (apres)', "
                                                        "designation as 'Designation', "
                                                        "reference as 'Raison', "
                                                        "reference_recu_paiement_client as 'Recu', "
                                                        "CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %5 "
                                                        "WHERE localisation='%6' AND reference='%7' AND compte_client IS NOT NULL AND date_paiement >= '%8' AND date_paiement <= '%9'").
                                                       arg
                                                       (YerithDatabaseTableColumn::DATE_PAIEMENT,
                                                        YerithDatabaseTableColumn::HEURE_PAIEMENT,
                                                        YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
                                                        YerithDatabaseTableColumn::MONTANT_PAYE,
                                                        YerithDatabase::PAIEMENTS,
                                                        comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
                                                        currentText(),
                                                        CURRENT_PAYMENT_REFERENCE,
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_debut->date
                                                         ()),
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_fin->date
                                                         ())));

            clientTransactionsServicesCompletesQueryStr.append(QString
                                                               ("select nom_entreprise_client, "
                                                                "%1 as 'Date transaction', "
                                                                "%2 as 'Heure transaction', "
                                                                "%3 as 'Type de paiement', "
                                                                "%4 as 'Total transaction', "
                                                                "compte_client as 'Compte client (apres)', "
                                                                "designation as 'Designation', "
                                                                "reference as 'Raison', "
                                                                "reference_recu_vendu as 'Recu', "
                                                                "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %5 "
                                                                "where localisation='%6' AND reference='%7' AND date_vente >= '%8' and date_vente <= '%9'").
                                                               arg
                                                               (YerithDatabaseTableColumn::DATE_VENTE,
                                                                YerithDatabaseTableColumn::HEURE_VENTE,
                                                                YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                                                YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                                                                YerithDatabase::SERVICES_COMPLETES,
                                                                comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
                                                                currentText(),
                                                                CURRENT_PAYMENT_REFERENCE,
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_debut->date
                                                                 ()),
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_fin->date
                                                                 ())));



            if (checkBox_ne_pas_inclure_les_ventes_comptant->isChecked())
            {
                paiementComptantQueryStr =
                                QString(" and (%1 != '%2')").
                                arg(YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                    QString::number(YerithUtils::VENTE_COMPTANT));
            }

            clientTransactionsStockVenduQueryStr.append(QString
                                                        ("select nom_entreprise_client, "
                                                         "%1 as 'Date transaction', "
                                                         "%2 as 'Heure transaction', "
                                                         "%3 as 'Type de paiement', "
                                                         "montant_total_vente as 'Total transaction', "
                                                         "compte_client as 'Compte client (apres)', "
                                                         "designation as 'Designation', "
                                                         "reference as 'Raison', "
                                                         "reference_recu_vendu as 'Recu', "
                                                         "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %4 "
                                                         "where localisation='%5' AND reference='%6' AND (date_vente >= '%7' and date_vente <= '%8') "
                                                         "%9").arg
                                                        (YerithDatabaseTableColumn::DATE_VENTE,
                                                         YerithDatabaseTableColumn::HEURE_VENTE,
                                                         YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                                         YerithDatabase::STOCKS_VENDU,
                                                         comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
                                                         currentText(),
                                                         CURRENT_PAYMENT_REFERENCE,
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_debut->
                                                          date()),
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_fin->
                                                          date()),
                                                         paiementComptantQueryStr));
        }
        else
        {
            clientTransactionsPaiementsQueryStr.append(QString
                                                       ("select nom_entreprise, "
                                                        "%1 as 'Date transaction', "
                                                        "%2 as 'Heure transaction', "
                                                        "%3 as 'Type de paiement', "
                                                        "%4 as 'Total transaction', "
                                                        "%5 as 'Compte client (apres)', "
                                                        "designation as 'Designation', "
                                                        "reference as 'Raison', "
                                                        "reference_recu_paiement_client as 'Recu', "
                                                        "CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %6 "
                                                        "WHERE localisation='%7' AND compte_client IS NOT NULL AND date_paiement >= '%8' AND date_paiement <= '%9'").
                                                       arg
                                                       (YerithDatabaseTableColumn::DATE_PAIEMENT,
                                                        YerithDatabaseTableColumn::HEURE_PAIEMENT,
                                                        YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
                                                        YerithDatabaseTableColumn::MONTANT_PAYE,
                                                        YerithDatabaseTableColumn::COMPTE_CLIENT,
                                                        YerithDatabase::PAIEMENTS,
                                                        comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
                                                        currentText(),
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_debut->date
                                                         ()),
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_fin->date
                                                         ())));

            clientTransactionsServicesCompletesQueryStr.append(QString
                                                               ("select nom_entreprise_client, "
                                                                "%1 as 'Date transaction', "
                                                                "%2 as 'Heure transaction', "
                                                                "%3 as 'Type de paiement', "
                                                                "%4 as 'Total transaction', "
                                                                "%5 as 'Compte client (apres)', "
                                                                "designation as 'Designation', "
                                                                "reference as 'Raison', "
                                                                "reference_recu_vendu as 'Recu', "
                                                                "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %6 "
                                                                "where localisation='%7' and date_vente >= '%8' and date_vente <= '%9'").
                                                               arg
                                                               (YerithDatabaseTableColumn::DATE_VENTE,
                                                                YerithDatabaseTableColumn::HEURE_VENTE,
                                                                YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                                                YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                                                                YerithDatabaseTableColumn::COMPTE_CLIENT,
                                                                YerithDatabase::SERVICES_COMPLETES,
                                                                comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
                                                                currentText(),
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_debut->date
                                                                 ()),
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_fin->date
                                                                 ())));



            if (checkBox_ne_pas_inclure_les_ventes_comptant->isChecked())
            {
                paiementComptantQueryStr =
                                QString(" and (%1 != '%2')").
                                arg(YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                    QString::number(YerithUtils::VENTE_COMPTANT));
            }

            clientTransactionsStockVenduQueryStr.append(QString
                                                        ("select nom_entreprise_client, "
                                                         "%1 as 'Date transaction', "
                                                         "%2 as 'Heure transaction', "
                                                         "%3 as 'Type de paiement', "
                                                         "%4 as 'Total transaction', "
                                                         "compte_client as 'Compte client (apres)', "
                                                         "designation as 'Designation', "
                                                         "reference as 'Raison', "
                                                         "reference_recu_vendu as 'Recu', "
                                                         "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %5 "
                                                         "where ( localisation='%6' and date_vente >= '%7' and date_vente <= '%8') "
                                                         "%9").arg
                                                        (YerithDatabaseTableColumn::DATE_VENTE,
                                                         YerithDatabaseTableColumn::HEURE_VENTE,
                                                         YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                                         YerithDatabaseTableColumn::
                                                         MONTANT_TOTAL_VENTE,
                                                         YerithDatabase::STOCKS_VENDU,
                                                         comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->
                                                         currentText(),
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_debut->
                                                          date()),
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_fin->
                                                          date()),
                                                         paiementComptantQueryStr));
        }

        QString
        clientTransactionsUnionQueryStr(QString
                                        ("SELECT * FROM (%1 UNION %2 UNION %3 ORDER BY Temps ASC) AS U WHERE U.%4 = '%5'").
                                        arg
                                        (clientTransactionsPaiementsQueryStr,
                                         clientTransactionsServicesCompletesQueryStr,
                                         clientTransactionsStockVenduQueryStr,
                                         YerithDatabaseTableColumn::
                                         NOM_ENTREPRISE, clientCompanyName));

        //qDebug() << QString("++ clientTransactionsUnionQueryStr: %1")
        //                          .arg(clientTransactionsUnionQueryStr);

        QSqlQuery
        sqlClientTransactionsUnionQuery;

        int
        querySize = YerithUtils::execQuery(sqlClientTransactionsUnionQuery,
                                           clientTransactionsUnionQueryStr);

//              qDebug() << QString("++ querySize: %1")
//                                              .arg(QString::number(querySize));

        if (querySize > 0)
        {
            YerithUtils::getAllWindows()->_transactionsDunClientWindow->
            listerTransactionsDunClient
            (dateEdit_transactions_compte_client_debut->date(),
             dateEdit_transactions_compte_client_fin->date(),
             clientCompanyName, sqlClientTransactionsUnionQuery);
        }
        else
        {
            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("payer au compte client - transactions d'un client"),
                                           QObject::tr
                                           ("Il n'y a pas de transactions de ce client à lister !"));
        }
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("payer au compte client - transactions d'un client"),
                                       QObject::tr
                                       ("Il n'y a pas de transactions de ce client à lister !"));
    }
}


void
YerithPayerCompteClientWindow::
PUBLIC_slot_afficher_les_transactions_dun_client()
{
//      qDebug() << QString("_clientLastSelectedRow: %1")
//                                      .arg(QString::number(_clientLastSelectedRow));

    if (!comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_succursale->isEmpty())
    {
        PUBLIC_slot_afficher_les_transactions_dun_client_WITH_ONE_LOCALISATION
        ();

        return;
    }

    int
    aClientLastSelectedRow =
                    _allWindows->_clientsWindow->getLastListerSelectedRow__ID_AS_INTEGER();

    if (0 != _curClientTableModel &&
            _curClientTableModel->rowCount() > 0 && aClientLastSelectedRow > -1)
    {
        QSqlRecord
        record;

        _allWindows->
        _clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
        (record);

//          int stockId = GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID).toInt();

        QString
        clientCompanyName(GET_SQL_RECORD_DATA
                          (record,
                           YerithDatabaseTableColumn::NOM_ENTREPRISE));

//          YerithHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
//                                                                                               stockReference,
//                                                                                                       QString::number(stockId),
//                                                                                                       stockDesignation);


        QString
        clientTransactionsPaiementsQueryStr;

        QString
        clientTransactionsServicesCompletesQueryStr;

        QString
        clientTransactionsStockVenduQueryStr;

        QString
        paiementComptantQueryStr;


        QString
        CURRENT_PAYMENT_REFERENCE =
                        comboBox_comptes_clients_CONSULTER_TRANSACTIONS_FINANCIERES_reference->
                        currentText();

        if (!CURRENT_PAYMENT_REFERENCE.isEmpty())
        {
            clientTransactionsPaiementsQueryStr.append(QString("select %1, "
                                                               "%2 as 'Date transaction', "
                                                               "%3 as 'Heure transaction', "
                                                               "%4 as 'Type de paiement', "
                                                               "%5 as 'Total transaction', "
                                                               "compte_client as 'Compte client (apres)', "
                                                               "designation as 'Designation', "
                                                               "reference as 'Raison', "
                                                               "reference_recu_paiement_client as 'Recu', "
                                                               "CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %6 "
                                                               "WHERE reference='%7' AND compte_client IS NOT NULL AND date_paiement >= '%8' AND date_paiement <= '%9'").
                                                       arg
                                                       (YerithDatabaseTableColumn::
                                                        NOM_ENTREPRISE,
                                                        YerithDatabaseTableColumn::
                                                        DATE_PAIEMENT,
                                                        YerithDatabaseTableColumn::
                                                        HEURE_PAIEMENT,
                                                        YerithDatabaseTableColumn::
                                                        TYPE_DE_PAIEMENT,
                                                        YerithDatabaseTableColumn::
                                                        MONTANT_PAYE,
                                                        YerithDatabase::
                                                        PAIEMENTS,
                                                        CURRENT_PAYMENT_REFERENCE,
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_debut->
                                                         date()),
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_fin->
                                                         date())));

            clientTransactionsServicesCompletesQueryStr.append(QString
                                                               ("select %1, "
                                                                "%2 as 'Date transaction', "
                                                                "%3 as 'Heure transaction', "
                                                                "%4 as 'Type de paiement', "
                                                                "%5 as 'Total transaction', "
                                                                "compte_client as 'Compte client (apres)', "
                                                                "designation as 'Designation', "
                                                                "reference as 'Raison', "
                                                                "reference_recu_vendu as 'Recu', "
                                                                "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %6 "
                                                                "where reference='%7' AND date_vente >= '%8' and date_vente <= '%9'").
                                                               arg
                                                               (YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                                                                YerithDatabaseTableColumn::DATE_VENTE,
                                                                YerithDatabaseTableColumn::HEURE_VENTE,
                                                                YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                                                YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                                                                YerithDatabase::SERVICES_COMPLETES,
                                                                CURRENT_PAYMENT_REFERENCE,
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_debut->date
                                                                 ()),
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_fin->date
                                                                 ())));



            if (checkBox_ne_pas_inclure_les_ventes_comptant->isChecked())
            {
                paiementComptantQueryStr =
                                QString(" and (%1 != '%2')").
                                arg(YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                    QString::number(YerithUtils::VENTE_COMPTANT));
            }

            clientTransactionsStockVenduQueryStr.append(QString("select %1, "
                                                                "%2 as 'Date transaction', "
                                                                "%3 as 'Heure transaction', "
                                                                "%4 as 'Type de paiement', "
                                                                "montant_total_vente as 'Total transaction', "
                                                                "compte_client as 'Compte client (apres)', "
                                                                "designation as 'Designation', "
                                                                "reference as 'Raison', "
                                                                "reference_recu_vendu as 'Recu', "
                                                                "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %5 "
                                                                "where reference='%6' AND (date_vente >= '%7' and date_vente <= '%8') "
                                                                "%9").arg
                                                        (YerithDatabaseTableColumn::
                                                         NOM_ENTREPRISE_CLIENT,
                                                         YerithDatabaseTableColumn::
                                                         DATE_VENTE,
                                                         YerithDatabaseTableColumn::
                                                         HEURE_VENTE,
                                                         YerithDatabaseTableColumn::
                                                         TYPE_DE_VENTE,
                                                         YerithDatabase::
                                                         STOCKS_VENDU,
                                                         CURRENT_PAYMENT_REFERENCE,
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_debut->
                                                          date()),
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_fin->
                                                          date()),
                                                         paiementComptantQueryStr));
        }
        else
        {
            clientTransactionsPaiementsQueryStr.append(QString("select %1, "
                                                               "%2 as 'Date transaction', "
                                                               "%3 as 'Heure transaction', "
                                                               "%4 as 'Type de paiement', "
                                                               "%5 as 'Total transaction', "
                                                               "%6 as 'Compte client (apres)', "
                                                               "designation as 'Designation', "
                                                               "reference as 'Raison', "
                                                               "reference_recu_paiement_client as 'Recu', "
                                                               "CONCAT(date_paiement,' ',heure_paiement) as 'Temps' from %7 "
                                                               "WHERE compte_client IS NOT NULL AND date_paiement >= '%8' AND date_paiement <= '%9'").
                                                       arg
                                                       (YerithDatabaseTableColumn::
                                                        NOM_ENTREPRISE,
                                                        YerithDatabaseTableColumn::
                                                        DATE_PAIEMENT,
                                                        YerithDatabaseTableColumn::
                                                        HEURE_PAIEMENT,
                                                        YerithDatabaseTableColumn::
                                                        TYPE_DE_PAIEMENT,
                                                        YerithDatabaseTableColumn::
                                                        MONTANT_PAYE,
                                                        YerithDatabaseTableColumn::
                                                        COMPTE_CLIENT,
                                                        YerithDatabase::
                                                        PAIEMENTS,
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_debut->
                                                         date()),
                                                        DATE_TO_DB_FORMAT_STRING
                                                        (dateEdit_transactions_compte_client_fin->
                                                         date())));

            clientTransactionsServicesCompletesQueryStr.append(QString
                                                               ("select %1, "
                                                                "%2 as 'Date transaction', "
                                                                "%3 as 'Heure transaction', "
                                                                "%4 as 'Type de paiement', "
                                                                "%5 as 'Total transaction', "
                                                                "%6 as 'Compte client (apres)', "
                                                                "designation as 'Designation', "
                                                                "reference as 'Raison', "
                                                                "reference_recu_vendu as 'Recu', "
                                                                "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %7 "
                                                                "where date_vente >= '%8' and date_vente <= '%9'").
                                                               arg
                                                               (YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                                                                YerithDatabaseTableColumn::DATE_VENTE,
                                                                YerithDatabaseTableColumn::HEURE_VENTE,
                                                                YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                                                YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                                                                YerithDatabaseTableColumn::COMPTE_CLIENT,
                                                                YerithDatabase::SERVICES_COMPLETES,
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_debut->date
                                                                 ()),
                                                                DATE_TO_DB_FORMAT_STRING
                                                                (dateEdit_transactions_compte_client_fin->date
                                                                 ())));



            if (checkBox_ne_pas_inclure_les_ventes_comptant->isChecked())
            {
                paiementComptantQueryStr =
                                QString(" and (%1 != '%2')").
                                arg(YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                    QString::number(YerithUtils::VENTE_COMPTANT));
            }

            clientTransactionsStockVenduQueryStr.append(QString("select %1, "
                                                                "%2 as 'Date transaction', "
                                                                "%3 as 'Heure transaction', "
                                                                "%4 as 'Type de paiement', "
                                                                "%5 as 'Total transaction', "
                                                                "compte_client as 'Compte client (apres)', "
                                                                "designation as 'Designation', "
                                                                "reference as 'Raison', "
                                                                "reference_recu_vendu as 'Recu', "
                                                                "CONCAT(date_vente,' ',heure_vente) as 'Temps' from %6 "
                                                                "where ( date_vente >= '%7' and date_vente <= '%8') "
                                                                "%9").arg
                                                        (YerithDatabaseTableColumn::
                                                         NOM_ENTREPRISE_CLIENT,
                                                         YerithDatabaseTableColumn::
                                                         DATE_VENTE,
                                                         YerithDatabaseTableColumn::
                                                         HEURE_VENTE,
                                                         YerithDatabaseTableColumn::
                                                         TYPE_DE_VENTE,
                                                         YerithDatabaseTableColumn::
                                                         MONTANT_TOTAL_VENTE,
                                                         YerithDatabase::
                                                         STOCKS_VENDU,
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_debut->
                                                          date()),
                                                         DATE_TO_DB_FORMAT_STRING
                                                         (dateEdit_transactions_compte_client_fin->
                                                          date()),
                                                         paiementComptantQueryStr));
        }

        QString
        clientTransactionsUnionQueryStr(QString
                                        ("SELECT * FROM (%1 UNION %2 UNION %3 ORDER BY Temps ASC) AS U WHERE U.%4 = '%5'").
                                        arg
                                        (clientTransactionsPaiementsQueryStr,
                                         clientTransactionsServicesCompletesQueryStr,
                                         clientTransactionsStockVenduQueryStr,
                                         YerithDatabaseTableColumn::
                                         NOM_ENTREPRISE, clientCompanyName));

        //qDebug() << QString("++ clientTransactionsUnionQueryStr: %1")
        //                          .arg(clientTransactionsUnionQueryStr);

        QSqlQuery
        sqlClientTransactionsUnionQuery;

        int
        querySize = YerithUtils::execQuery(sqlClientTransactionsUnionQuery,
                                           clientTransactionsUnionQueryStr);

//              qDebug() << QString("++ querySize: %1")
//                                              .arg(QString::number(querySize));

        if (querySize > 0)
        {
            YerithUtils::getAllWindows()->_transactionsDunClientWindow->
            listerTransactionsDunClient
            (dateEdit_transactions_compte_client_debut->date(),
             dateEdit_transactions_compte_client_fin->date(),
             clientCompanyName, sqlClientTransactionsUnionQuery);
        }
        else
        {
            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("payer au compte client - transactions d'un client"),
                                           QObject::tr
                                           ("Il n'y a pas de transactions de ce client à lister !"));
        }
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("payer au compte client - transactions d'un client"),
                                       QObject::tr
                                       ("Il n'y a pas de transactions de ce client à lister !"));
    }
}



void YerithPayerCompteClientWindow::definirManager()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_entrer->enable(this, SLOT(vendre_au_COMPTE_CLIENT_local()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_reinitialiser->enable(this,
                                     SLOT
                                     (reinitialiser_donnees_de_paiement_au_compteclient
                                      ()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_detail_client->enable(this, SLOT(afficher_detail_client()));

    pushButton_transactions_compte_client_consulter->enable(this,
                                                            SLOT
                                                            (PUBLIC_slot_afficher_les_transactions_dun_client
                                                             ()));

    pushButton_transactions_compte_client_reinitialiser_consultation->enable
    (this,
     SLOT(reinitialiser_donnees_de_consultation_des_transactions_financieres
          ()));
}


void YerithPayerCompteClientWindow::definirVendeur()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    pushButton_payer->enable(this, SLOT(putCashIntoCustomerAccount()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_entrer->enable(this, SLOT(vendre_au_COMPTE_CLIENT()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_reinitialiser->enable(this,
                                     SLOT
                                     (reinitialiser_donnees_de_paiement_au_compteclient
                                      ()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_detail_client->enable(this, SLOT(afficher_detail_client()));

    pushButton_transactions_compte_client_consulter->enable(this,
                                                            SLOT
                                                            (PUBLIC_slot_afficher_les_transactions_dun_client
                                                             ()));

    pushButton_transactions_compte_client_reinitialiser_consultation->enable
    (this,
     SLOT(reinitialiser_donnees_de_consultation_des_transactions_financieres
          ()));
}


void YerithPayerCompteClientWindow::definirPasDeRole()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiser, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsCompteClient,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_facture->disable(this);
    pushButton_payer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_paiements->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_detail_client->disable(this);
    pushButton_transactions_compte_client_consulter->disable(this);
    pushButton_transactions_compte_client_reinitialiser_consultation->disable
    (this);
}
