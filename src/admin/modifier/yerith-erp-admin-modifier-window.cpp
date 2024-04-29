/*
 * yerith-erp-admin-modifier-window.cpp
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-admin-modifier-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include <QtCore/QDebug>

#include <QtWidgets/QDesktopWidget>

#include <QtSql/QSqlError>

YerithAdminModifierWindow::YerithAdminModifierWindow()
:YerithPOSAdminWindowsCommons(QObject::tr("administration ~ modifier")),
 _logger(new YerithLogger("YerithAdminModifierWindow")),
 _curSearchSqlTableModel(0),
 _savePassword(false)
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_DARK_GREEN_47_67_67,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    comboBox_modifier_utilisateur_titre
		->setupPopulateNORawString(YerithDatabase::TITRES,
								   YerithDatabaseTableColumn::APPELATION_TITRE,
								   &YerithUtils::_titreToUserViewString);


    comboBox_modifier_utilisateur_role
		->setupPopulateNORawString(YerithDatabase::ROLES,
								   YerithDatabaseTableColumn::NOM_ROLE,
								   &YerithUtils::_roleToUserViewString);


    setupLineEdits();

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);

    pushButton_menu->enable(this, SLOT(menu()));

    pushButton_modifier_localisation_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_CHARGE_FINANCIERE_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_categorie_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_lignebudgetaire_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_compte_bancaire_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_utilisateur_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_departements_de_produits_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_alerte_annuler->enable(this, SLOT(annuler()));

    pushButton_modifier_utilisateur_valider
		->enable(this, SLOT(modifier_utilisateur()));

    pushButton_modifier_localisation_valider
		->enable(this, SLOT(modifier_localisation()));

    pushButton_modifier_CHARGE_FINANCIERE_valider
		->enable(this, SLOT(modifier_CHARGE_FINANCIERE()));

    pushButton_modifier_categorie_valider->enable(this, SLOT(modifier_categorie()));

    pushButton_modifier_LIGNE_BUDGETAIRE_valider->enable(this, SLOT(modifier_LIGNE_BUDGETAIRE()));

    pushButton_modifier_compte_bancaire_valider->enable(this, SLOT(modifier_compte_bancaire()));

    pushButton_modifier_departements_de_produits_valider
		->enable(this, SLOT(modifier_departements_de_produits()));

    pushButton_modifier_alerte_valider->enable(this, SLOT(modifier_alerte()));

    pushButton_creer->enable(this, SLOT(creer()));
    pushButton_lister->enable(this, SLOT(lister()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));

    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionCreer, SIGNAL(triggered()), this, SLOT(creer()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));
    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this,
            SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(radioButton_modifier_alerte_quantite, SIGNAL(clicked(bool)),
            this, SLOT(radioButtons_quantite()));
    connect(radioButton_modifier_alerte_periode_temps, SIGNAL(clicked(bool)),
            this, SLOT(radioButtons_periode_temps()));
}


void YerithAdminModifierWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 false);
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);
}


void YerithAdminModifierWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);
}


void YerithAdminModifierWindow::definirManager()
{
    _logger->log("definirManager");
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 true);
}


void YerithAdminModifierWindow::setupLineEdits()
{
	//FINANCIAL EXPENSE WIDGET SETUP
    dateEdit_date_de_reception->setYerithEnabled(false);
    dateEdit_date_de_commande->setYerithEnabled(false);

    lineEdit_departement->setYerithEnabled(false);
    lineEdit_reference_produit->setYerithEnabled(false);
    lineEdit_designation->setYerithEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerithEnabled(false);
    lineEdit_LIGNE_BUDGETAIRE->setYerithEnabled(false);
    lineEdit_quantite->setYerithEnabled(false);
    lineEdit_prix_dachat->setYerithEnabled(false);
    lineEdit_prix_unitaire->setYerithEnabled(false);

    lineEdit_STATUT_DE_LACHAT->setYerithEnabled(false);
	lineEdit_MONTANT_TVA->setYerithEnabled(false);
	lineEdit_ref_RECU_DACHAT->setYerithEnabled(false);
	lineEdit_LOCALISATION->setYerithEnabled(false);
	lineEdit_ID_commandeur->setYerithEnabled(false);

    textEdit_une_CHARGE_FINANCIERE->setYerithEnabled(false);



	lineEdit_modifier_categorie_nom_departement_produit->setYerithEnabled(false);

	lineEdit_modifier_montant_de_la_LIGNE_BUDGETAIRE->setYerithEnabled(false);
	lineEdit_modifier_montant_RESTANT_de_la_LIGNE_BUDGETAIRE->setYerithEnabled(false);

    lineEdit_modifier_utilisateur_id->setYerithEnabled(false);
    lineEdit_modifier_alerte_designation_alerte->setYerithEnabled(false);
    lineEdit_modifier_alerte_quantite->setYerithEnabled(false);
    lineEdit_modifier_localisation_nom->setYerithEnabled(false);
    lineEdit_modifier_utilisateur_mot_passe_1->setEchoMode(QLineEdit::Password);
    lineEdit_modifier_utilisateur_mot_passe_2->setEchoMode(QLineEdit::Password);
}


void YerithAdminModifierWindow::rendreInvisible()
{
    clear_utilisateur_all_fields();
    clear_departements_de_produits_all_fields();
    clear_CHARGE_FINANCIERE_all_fields();
    clear_categorie_all_fields();
    clear_LIGNE_BUDGETAIRE_all_fields();
    clear_localisation_all_fields();
    clear_alerte_all_fields();
    clear_remise_all_fields();

    _savePassword = false;

    YerithPOSAdminWindowsCommons::rendreInvisible();
}


void YerithAdminModifierWindow::enableOtherTabs(enum AdminSujetAction curAction,
                                                bool enabled)
{
    tabWidget_modifier->setTabEnabled(curAction, !enabled);
    for (int k = 0; k <= SUJET_ACTION_LOCALISATION; ++k)
    {
        if (k != curAction)
        {
            tabWidget_modifier->setTabEnabled(k, enabled);
        }
    }
}


void YerithAdminModifierWindow::rendreVisible(enum AdminSujetAction selectedSujetAction)
{
    retranslateUi(this);

    clear_utilisateur_all_fields();
    clear_departements_de_produits_all_fields();
    clear_categorie_all_fields();
    clear_CHARGE_FINANCIERE_all_fields();
    clear_LIGNE_BUDGETAIRE_all_fields();
    clear_compte_bancaire_all_fields();
    clear_localisation_all_fields();
    clear_alerte_all_fields();
    clear_remise_all_fields();

    switch (selectedSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_COMPTE_UTILISATEUR);
        enableOtherTabs(SUJET_ACTION_COMPTE_UTILISATEUR, false);
        setupEditCompteUtilisateur();
        break;

    case SUJET_ACTION_LOCALISATION:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_LOCALISATION);
        enableOtherTabs(SUJET_ACTION_LOCALISATION, false);
        setupEditLocalisation();
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        tabWidget_modifier->setCurrentIndex
        (SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        enableOtherTabs(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS, false);
        setupEditDepartementsDeProduits();
        break;

    case SUJET_ACTION_CATEGORIE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_CATEGORIE);
        enableOtherTabs(SUJET_ACTION_CATEGORIE, false);
        setupEditCategorie();
        break;

    case SUJET_ACTION_ligne_budgetaire:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_ligne_budgetaire);
        enableOtherTabs(SUJET_ACTION_ligne_budgetaire, false);
        setupEdit_LIGNE_BUDGETAIRE();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_COMPTE_BANCAIRE);
        enableOtherTabs(SUJET_ACTION_COMPTE_BANCAIRE, false);
        setupEditCompteBancaire();
        break;

    case SUJET_ACTION_ALERTE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_ALERTE);
        enableOtherTabs(SUJET_ACTION_ALERTE, false);
        setupEditAlerte();
        break;

    case SUJET_ACTION_REMISE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_REMISE);
        enableOtherTabs(SUJET_ACTION_REMISE, false);
        setupEditRemise();
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        tabWidget_modifier->setCurrentIndex(SUJET_ACTION_CHARGE_FINANCIERE);
        enableOtherTabs(SUJET_ACTION_CHARGE_FINANCIERE, false);
        setupEdit_CHARGE_FINANCIERE();
        break;

    default:
        break;
    }

    setVisible(true);
}


void YerithAdminModifierWindow::retourListerCompteUtilisateur()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourListerLocalisation()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourListerDepartementsDeProduits()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourListerCategorie()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourLister_CHARGE_FINANCIERE()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CHARGE_FINANCIERE);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourLister_LIGNE_BUDGETAIRE()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourListerCompteBancaire()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourListerAlerte()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);
    rendreInvisible();
}


void YerithAdminModifierWindow::retourListerRemise()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_REMISE);
    rendreInvisible();
}


void YerithAdminModifierWindow::annuler()
{
    switch (tabWidget_modifier->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        clear_utilisateur_all_fields();
        retourListerCompteUtilisateur();
        break;

    case SUJET_ACTION_LOCALISATION:
        clear_localisation_all_fields();
        retourListerLocalisation();
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        clear_departements_de_produits_all_fields();
        retourListerDepartementsDeProduits();
        break;

    case SUJET_ACTION_CATEGORIE:
        clear_categorie_all_fields();
        retourListerCategorie();
        break;

    case SUJET_ACTION_ligne_budgetaire:
    	clear_LIGNE_BUDGETAIRE_all_fields();
    	retourLister_LIGNE_BUDGETAIRE();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        clear_compte_bancaire_all_fields();
        retourListerCompteBancaire();
        break;

    case SUJET_ACTION_ALERTE:
        clear_alerte_all_fields();
        retourListerAlerte();
        break;

    case SUJET_ACTION_REMISE:
        clear_remise_all_fields();
        retourListerRemise();
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	clear_CHARGE_FINANCIERE_all_fields();
    	retourLister_CHARGE_FINANCIERE();
        break;

    default:
        break;
    }

    rendreInvisible();
}


void YerithAdminModifierWindow::creer()
{
    _allWindows->_adminCreateWindow
        ->rendreVisible(_curStocksTableModel,
                        tabWidget_modifier->currentIndex());
    rendreInvisible();
}


void YerithAdminModifierWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_modifier->currentIndex());
    rendreInvisible();
}


void YerithAdminModifierWindow::supprimer()
{
    _allWindows->_adminListerWindow->supprimer();
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_modifier->currentIndex());
    rendreInvisible();
}


void YerithAdminModifierWindow::radioButtons_quantite()
{
    if (radioButton_modifier_alerte_periode_temps->isChecked())
    {
        radioButton_modifier_alerte_periode_temps->setChecked(false);
    }
}


void YerithAdminModifierWindow::radioButtons_periode_temps()
{
    if (radioButton_modifier_alerte_quantite->isChecked())
    {
        radioButton_modifier_alerte_quantite->setChecked(false);
    }
}



#include "modifier-utilisateur.cpp"

#include "modifier-localisation.cpp"

#include "modifier-departements_de_produits.cpp"

#include "modifier-CHARGE-FINANCIERE.cpp"

#include "modifier-categorie.cpp"

#include "modifier-LIGNE-BUDGETAIRE.cpp"

#include "modifier-compte-bancaire.cpp"

#include "modifier-alerte.cpp"

#include"modifier-remise.cpp"
