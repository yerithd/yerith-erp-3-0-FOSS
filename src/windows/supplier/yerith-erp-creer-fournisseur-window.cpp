/*
 * yerith-erp-creer-fournisseur-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-creer-fournisseur-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>



QString YerithCreerFournisseurWindow::STRING_FOURNISSEUR("supplier");

QString YerithCreerFournisseurWindow::STRING_EMPLOYE("employee");



YerithCreerFournisseurWindow::YerithCreerFournisseurWindow():YerithWindowsCommons(),
    _logger(new
            YerithLogger("YerithCreerFournisseurWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("créer un EMPLOYÉ / fournisseur"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    setupLineEdits();

    populateCreerFournisseurComboBoxes();



    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_charges_financieres->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */

    QObject::connect(comboBox_FOURNISSEUR_ou_EMPLOYE,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(SWITCH_TO_EMPLOYE_fournisseur(const QString &)));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionCHARGES_FINANCIERES, SIGNAL(triggered()), this,
            SLOT(charges_financieres()));
    connect(actionFournisseurs, SIGNAL(triggered()), this,
            SLOT(fournisseurs()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this,
            SLOT(supprimer_image_fournisseur()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerFournisseur()));
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


void YerithCreerFournisseurWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}


void YerithCreerFournisseurWindow::populateCreerFournisseurComboBoxes()
{
    comboBox_FOURNISSEUR_ou_EMPLOYE->clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        YerithCreerFournisseurWindow::STRING_FOURNISSEUR = "supplier";

        YerithCreerFournisseurWindow::STRING_EMPLOYE = "employee";
    }
    else
    {
        YerithCreerFournisseurWindow::STRING_FOURNISSEUR = "FOURNISSEUR";

        YerithCreerFournisseurWindow::STRING_EMPLOYE = "EMPLOYÉ";
    }
}


void YerithCreerFournisseurWindow::setupLineEdits()
{
    lineEdit_fournisseur_dette_maximale->setYerithEnabled(false);
    lineEdit_fournisseur_dette_maximale->setValidator(&YerithUtils::POSITIVE_DoubleValidator);
}


void YerithCreerFournisseurWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithCreerFournisseurWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);

    pushButton_charges_financieres->disable(this);
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerithCreerFournisseurWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}

void YerithCreerFournisseurWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
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
    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT
                                                 (supprimer_image_fournisseur
                                                  ()));
    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_fournisseur
                                               ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerFournisseur()));
}


void YerithCreerFournisseurWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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
    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerFournisseur()));
}


void YerithCreerFournisseurWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerithCreerFournisseurWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
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
    pushButton_fournisseurs->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_enregistrer->disable(this);
}


bool YerithCreerFournisseurWindow::supplierAlreadyExist()
{
    bool result = false;

    YerithSqlTableModel &fournisseursTableModel =
                    _allWindows->getSqlTableModel_fournisseurs();

    // ** check if customer account with same name exist
    bool resultNomEntreprise =
                    YerithUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this,
                                                                                  fournisseursTableModel,
                                                                                  *lineEdit_fournisseur_nom_de_lentreprise);

    result = result || resultNomEntreprise;

    // ** check if customer account with same trade registry number exist
    bool resultReferenceRegistreDuCommerce =
                    YerithUtils::
                    checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(*this,
                                                                                      fournisseursTableModel,
                                                                                      *lineEdit_fournisseur_reference_du_registre_du_commerce);

    result = result || resultReferenceRegistreDuCommerce;


    // ** check if SUPPLIER account with same reference exist
    bool resultReference_FOURNISSEUR =
                    YerithUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                                fournisseursTableModel,
                                                                *lineEdit_fournisseur_reference_fournisseur,
                                                                QObject::tr
                                                                ("référence du fournisseur"),
                                                                YerithDatabaseTableColumn::
                                                                REFERENCE_FOURNISSEUR);

    result = result || resultReference_FOURNISSEUR;

    return result;
}


bool YerithCreerFournisseurWindow::check_fields()
{
    bool result;

    bool nom_entreprise =
                    lineEdit_fournisseur_nom_de_lentreprise->checkField();

    result = nom_entreprise;

    return result;
}


void YerithCreerFournisseurWindow::clear_all_fields()
{
    lineEdit_fournisseur_reference_fournisseur->clear();
    lineEdit_fournisseur_nom_de_lentreprise->clearField();
    lineEdit_fournisseur_nom_du_representant->clearField();
    lineEdit_fournisseur_quartier->clear();
    lineEdit_fournisseur_ville->clear();
    lineEdit_fournisseur_province_etat->clear();
    lineEdit_fournisseur_pays->clear();
    lineEdit_fournisseur_boite_postale->clear();
    lineEdit_fournisseur_siege_social->clear();
    lineEdit_fournisseur_email->clear();
    lineEdit_fournisseur_numero_telephone_1->clear();
    lineEdit_fournisseur_numero_telephone_2->clear();
    lineEdit_fournisseur_reference_du_registre_du_commerce->clear();
    lineEdit_fournisseur_numero_de_contribuable->clear();
    lineEdit_fournisseur_dette_maximale->clear();
    comboBox_FOURNISSEUR_ou_EMPLOYE->resetYerithComboBox();
    textEdit_creer_fournisseur_description_fournisseur->clear();
    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
}


void YerithCreerFournisseurWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}


void YerithCreerFournisseurWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;


    populateCreerFournisseurComboBoxes();


    YerithPOSUser *aUser = YerithUtils::getAllWindows()->getUser();

    if (0 != YerithUtils::getAllWindows())
    {
        if (0 != aUser && aUser->isManager())
        {
        	if (!comboBox_FOURNISSEUR_ou_EMPLOYE->contains(YerithCreerFournisseurWindow::STRING_FOURNISSEUR))
        	{
        		comboBox_FOURNISSEUR_ou_EMPLOYE->addItem(YerithCreerFournisseurWindow::STRING_FOURNISSEUR);
        	}

        	if (!comboBox_FOURNISSEUR_ou_EMPLOYE->contains(YerithCreerFournisseurWindow::STRING_EMPLOYE))
        	{
        		comboBox_FOURNISSEUR_ou_EMPLOYE->addItem(YerithCreerFournisseurWindow::STRING_EMPLOYE);
        	}

            label_fournisseur_dette_maximale->setVisible(true);
            lineEdit_fournisseur_dette_maximale->setVisible(true);
        }
        else
        {
            label_fournisseur_dette_maximale->setVisible(false);
            lineEdit_fournisseur_dette_maximale->setVisible(false);
        }
    }


    SWITCH_TO_EMPLOYE_fournisseur();


    check_fields();

    lineEdit_fournisseur_nom_de_lentreprise->setFocus();

    setVisible(true);
}


void YerithCreerFournisseurWindow::SWITCH_TO_EMPLOYE_fournisseur(const QString &EMPLOYE_ou_FOURNISSEUR /* = YerithUtils::EMPTY_STRING */)
{
    if (!YerithUtils::isEqualCaseInsensitive(STRING_EMPLOYE,
    										 comboBox_FOURNISSEUR_ou_EMPLOYE->currentText()))
    {
        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName(QObject::tr("créer 1 fournisseur")),
                                         _curStocksTableModel);


        tabWidget_creer_fournisseur
            ->setTabText(0, QObject::tr("Données du Fournisseur"));


        tabWidget_creer_fournisseur
            ->setTabText(1, QObject::tr("DESCRIPTION DU FOURNISSEUR"));


        groupBox_2->setTitle(QObject::tr("Image de l'entreprise"));


    	label_designation->setText(QObject::tr("Nom de l'entreprise"));


    	label_employe_SUPERIEUR_hierarchique->setVisible(false);
    	label_employe_DEPARTEMENT->setVisible(false);
		label_EMPLOYE_position_fonction->setVisible(false);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(false);
		lineEdit_employe_DEPARTEMENT->setVisible(false);
		lineEdit_EMPLOYE_position_fonction->setVisible(false);
    }
    else
    {
        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName(QObject::tr("créer 1 Employé(e)")),
                                         _curStocksTableModel);


        tabWidget_creer_fournisseur
            ->setTabText(0, QObject::tr("Données de L'EMPLOYÉ"));


        tabWidget_creer_fournisseur
            ->setTabText(1, QObject::tr("DESCRIPTION de l'employé"));


        groupBox_2->setTitle(QObject::tr("Image de l'employé(e)"));

    	label_designation->setText(QObject::tr("Nom de l'employé(e)"));

    	label_employe_SUPERIEUR_hierarchique->setVisible(true);
    	label_employe_DEPARTEMENT->setVisible(true);
		label_EMPLOYE_position_fonction->setVisible(true);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(true);
		lineEdit_employe_DEPARTEMENT->setVisible(true);
		lineEdit_EMPLOYE_position_fonction->setVisible(true);
    }
}


bool YerithCreerFournisseurWindow::creerEnregistrerFournisseur()
{
    if (check_fields())
    {
        if (supplierAlreadyExist())
        {
            return false;
        }

        QString retMsg(QObject::tr("Le fournisseur '"));

        YerithSqlTableModel &fournisseursTableModel =
                        _allWindows->getSqlTableModel_fournisseurs();

        QSqlRecord record = fournisseursTableModel.record();

        record.setValue(YerithDatabaseTableColumn::ID,
                        YerithERPWindows::getNextIdSqlTableModel_fournisseurs
                        ());
        record.setValue(YerithDatabaseTableColumn::REFERENCE_FOURNISSEUR,
                        lineEdit_fournisseur_reference_fournisseur->text());

        record.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                        lineEdit_fournisseur_nom_de_lentreprise->text());

        record.setValue(YerithDatabaseTableColumn::NOM_REPRESENTANT,
                        lineEdit_fournisseur_nom_du_representant->text());

        record.setValue(YerithDatabaseTableColumn::QUARTIER,
                        lineEdit_fournisseur_quartier->text());

        record.setValue(YerithDatabaseTableColumn::VILLE,
                        lineEdit_fournisseur_ville->text());

        record.setValue(YerithDatabaseTableColumn::PROVINCE_ETAT,
                        lineEdit_fournisseur_province_etat->text());

        record.setValue(YerithDatabaseTableColumn::PAYS,
                        lineEdit_fournisseur_pays->text());

        record.setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_fournisseur_boite_postale->text());

        record.setValue(YerithDatabaseTableColumn::SIEGE_SOCIAL,
                        lineEdit_fournisseur_siege_social->text());

        record.setValue(YerithDatabaseTableColumn::EMAIL,
                        lineEdit_fournisseur_email->text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_fournisseur_numero_telephone_1->text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_fournisseur_numero_telephone_2->text());

        record.
        setValue(YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
                 lineEdit_fournisseur_reference_du_registre_du_commerce->
                 text());

        record.setValue(YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE,
                        lineEdit_fournisseur_numero_de_contribuable->text());

        record.setValue(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR, 0.0);

        record.setValue(YerithDatabaseTableColumn::DESCRIPTION_FOURNISSEUR,
                        textEdit_creer_fournisseur_description_fournisseur->toPlainText());

        if (0 != label_image_produit && label_image_produit->pixmap())
        {
            QByteArray bytes;

            YerithUtils::savePixmapToByteArray(bytes,
                                               *label_image_produit->pixmap(),
                                               "JPG");

            record.setValue(YerithDatabaseTableColumn::IMAGE_FOURNISSEUR,
                            bytes);
        }


        if (YerithUtils::isEqualCaseInsensitive(STRING_EMPLOYE,
        										comboBox_FOURNISSEUR_ou_EMPLOYE->currentText()))
        {
            record.setValue(YerithDatabaseTableColumn::EMPLOYE, true);


            record.setValue(YerithDatabaseTableColumn::MANAGER,
            		lineEdit_employe_SUPERIEUR_hierarchique->text());

            record.setValue(YerithDatabaseTableColumn::DEPARTEMENT,
            		lineEdit_employe_DEPARTEMENT->text());

            record.setValue(YerithDatabaseTableColumn::POSITION_FONCTION,
            		lineEdit_EMPLOYE_position_fonction->text());
        }
        else
        {
        	record.setValue(YerithDatabaseTableColumn::EMPLOYE, false);
        }


        retMsg.append(lineEdit_fournisseur_nom_de_lentreprise->text());

        bool success = fournisseursTableModel
        				.insertNewRecord(record,
        								 this,
										 "src/windows/supplier/yerith-erp-creer-fournisseur-window.cpp",
										 648,
										 lineEdit_fournisseur_nom_de_lentreprise->text());

        if (!success)
        {
            retMsg.append(QObject::tr("' n'a pas pu être créer !"));

            YerithQMessageBox::warning(this,
                                       QObject::tr("Yerith-pgi-3.0 ~ administration ~ créer ~ fournisseur"),
                                       retMsg);
            return false;
        }

        fournisseursTableModel.select();

        retMsg.append(QObject::tr("' a été créer avec succès !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("Yerith-pgi-3.0 ~ administration ~ créer ~ fournisseur"),
                                       retMsg);

        clear_all_fields();

        fournisseurs();
    }

    return true;
}
