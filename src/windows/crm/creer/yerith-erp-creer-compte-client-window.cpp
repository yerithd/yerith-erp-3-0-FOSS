/*
 * yerith-erp-creer-compte-client-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-creer-compte-client-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"


YerithCreerCompteClientWindow::YerithCreerCompteClientWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithCreerCompteClientWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("créer un compte client"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YERITH_BLACK_0_0_0);

    setupLineEdits();

    localSetupLineEditsQCompleters();


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_charges_financieres->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionCHARGES_FINANCIERES, SIGNAL(triggered()), this,
            SLOT(charges_financieres()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this,
            SLOT(supprimer_image_compte_client()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerCompteClient()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();
}

void YerithCreerCompteClientWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}

void YerithCreerCompteClientWindow::setupLineEdits()
{
    lineEdit_compte_client_dette_maximale->setYerithEnabled(false);
    lineEdit_compte_client_dette_maximale->
    setValidator(&YerithUtils::POSITIVE_DoubleValidator);
}


void YerithCreerCompteClientWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithCreerCompteClientWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_charges_financieres->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerithCreerCompteClientWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_charges_financieres->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerithCreerCompteClientWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_charges_financieres->enable(this,
                                           SLOT(charges_financieres()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(clients()));
    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT
                                                 (supprimer_image_compte_client
                                                  ()));
    pushButton_selectionner_image->enable(this,
                                          SLOT
                                          (selectionner_image_compte_client
                                           ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerCompteClient()));
}


void YerithCreerCompteClientWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_charges_financieres->enable(this,
                                           SLOT(charges_financieres()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(clients()));
    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT
                                                 (supprimer_image_compte_client
                                                  ()));
    pushButton_selectionner_image->enable(this,
                                          SLOT
                                          (selectionner_image_compte_client
                                           ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerCompteClient()));
}


void YerithCreerCompteClientWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_charges_financieres->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerithCreerCompteClientWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_charges_financieres->disable(this);
    pushButton_clients->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


bool YerithCreerCompteClientWindow::customerAccountAlreadyExist()
{
    bool result = false;

    YerithSqlTableModel &clientsTableModel =
                    _allWindows->getSqlTableModel_clients();

    // ** check if customer account with same name exist
    bool resultNomEntreprise =
                    YerithUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this,
                                                                                  clientsTableModel,
                                                                                  *lineEdit_compte_client_nom_de_lentreprise);

    result = result || resultNomEntreprise;

    // ** check if customer account with same trade registry number exist
    bool resultReferenceRegistreDuCommerce =
                    YerithUtils::
                    checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(*this,
                                                                                      clientsTableModel,
                                                                                      *lineEdit_compte_client_reference_du_registre_du_commerce);

    result = result || resultReferenceRegistreDuCommerce;


    // ** check if customer account with same reference exist
    bool resultReferenceClient =
                    YerithUtils::checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(*this,
                                                                                    clientsTableModel,
                                                                                    *lineEdit_compte_client_reference_client);

    result = result || resultReferenceClient;

    return result;
}


bool YerithCreerCompteClientWindow::check_fields()
{
    bool result;

    bool nom_entreprise =
                    lineEdit_compte_client_nom_de_lentreprise->checkField();

    result = nom_entreprise;

    return result;
}


void YerithCreerCompteClientWindow::clear_all_fields()
{
    lineEdit_compte_client_reference_client->clear();
    lineEdit_compte_client_nom_de_lentreprise->clearField();
    lineEdit_compte_client_nom_du_representant->clearField();
    lineEdit_compte_client_quartier->clear();
    lineEdit_compte_client_ville->clear();
    lineEdit_compte_client_province_etat->clear();
    lineEdit_compte_client_pays->clear();
    lineEdit_compte_client_boite_postale->clear();
    lineEdit_compte_client_siege_social->clear();
    lineEdit_compte_client_email->clear();
    lineEdit_compte_client_numero_telephone_1->clear();
    lineEdit_compte_client_numero_telephone_2->clear();
    lineEdit_compte_client_reference_du_registre_du_commerce->clear();
    lineEdit_compte_client_numero_de_contribuable->clear();
    lineEdit_compte_client_dette_maximale->clear();
    lineEdit_compte_client_refereur->clear();
    textEdit_creer_compte_client_description_client->clear();
    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
}

void YerithCreerCompteClientWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}

void YerithCreerCompteClientWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    YerithPOSUser *aUser = YerithUtils::getAllWindows()->getUser();

    if (0 != YerithUtils::getAllWindows())
    {
        if (0 != aUser && aUser->isManager())
        {
            label_compte_client_dette_maximale->setVisible(true);
            lineEdit_compte_client_dette_maximale->setVisible(true);
        }
        else
        {
            label_compte_client_dette_maximale->setVisible(false);
            lineEdit_compte_client_dette_maximale->setVisible(false);
        }
    }

    localSetupLineEditsQCompleters();

    check_fields();

    lineEdit_compte_client_nom_de_lentreprise->setFocus();

    setVisible(true);

    YerithMainWindow::get_TRANSLATED_WindowName("créer un compte client");
}


bool YerithCreerCompteClientWindow::creerEnregistrerCompteClient()
{
    if (check_fields())
    {
        if (customerAccountAlreadyExist())
        {
            return false;
        }

        QString proposedClient = lineEdit_compte_client_refereur->text();

        QString retMsg(QObject::tr("Le client '"));

        YerithSqlTableModel &clientsTableModel =
                        _allWindows->getSqlTableModel_clients();

        QSqlRecord record = clientsTableModel.record();

        record.setValue(YerithDatabaseTableColumn::ID,
                        YerithERPWindows::getNextIdSqlTableModel_clients());

        record.setValue(YerithDatabaseTableColumn::REFERENCE_CLIENT,
                        lineEdit_compte_client_reference_client->text());

        record.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                        lineEdit_compte_client_nom_de_lentreprise->text());

        record.setValue(YerithDatabaseTableColumn::NOM_REPRESENTANT,
                        lineEdit_compte_client_nom_du_representant->text());

        record.setValue(YerithDatabaseTableColumn::QUARTIER,
                        lineEdit_compte_client_quartier->text());

        record.setValue(YerithDatabaseTableColumn::VILLE,
                        lineEdit_compte_client_ville->text());

        record.setValue(YerithDatabaseTableColumn::PROVINCE_ETAT,
                        lineEdit_compte_client_province_etat->text());

        record.setValue(YerithDatabaseTableColumn::PAYS,
                        lineEdit_compte_client_pays->text());

        record.setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_compte_client_boite_postale->text());

        record.setValue(YerithDatabaseTableColumn::SIEGE_SOCIAL,
                        lineEdit_compte_client_siege_social->text());

        record.setValue(YerithDatabaseTableColumn::EMAIL,
                        lineEdit_compte_client_email->text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_compte_client_numero_telephone_1->text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_compte_client_numero_telephone_2->text());

        record.setValue(YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
                 	 	lineEdit_compte_client_reference_du_registre_du_commerce->text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE,
                        lineEdit_compte_client_numero_de_contribuable->text());

        record.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT, 0.0);

        record.setValue(YerithDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
                 	 	0.0);

        record.setValue(YerithDatabaseTableColumn::LOCALISATION,
                        YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

        record.setValue(YerithDatabaseTableColumn::REFEREUR_CLIENT,
                        proposedClient);

        record.setValue(YerithDatabaseTableColumn::DESCRIPTION_CLIENT,
                        textEdit_creer_compte_client_description_client->
                        toPlainText());

        if (0 != label_image_produit && label_image_produit->pixmap())
        {
            QByteArray bytes;
            YerithUtils::savePixmapToByteArray(bytes,
                                               *label_image_produit->pixmap(),
                                               "JPG");
            record.setValue(YerithDatabaseTableColumn::IMAGE_COMPTE_CLIENT,
                            bytes);
        }

        retMsg.append(lineEdit_compte_client_nom_de_lentreprise->text());

        bool success = clientsTableModel.insertNewRecord(record,
        												 this,
														 "src/windows/crm/creer/yerith-erp-creer-compte-client-window.cpp",
														 561,
														 lineEdit_compte_client_nom_de_lentreprise->text());

        if (!success)
        {
            retMsg.append(QObject::tr("' n'a pas pu être créer !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr("créer 1 compte client"), retMsg);
            return false;
        }

        YerithUtils::creerNouveauClient(proposedClient, this);

        clientsTableModel.select();

        retMsg.append(QObject::tr("' a été créer avec succès !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("créer 1 compte client"), retMsg);

        clear_all_fields();

        clients();
    }

    return true;
}
