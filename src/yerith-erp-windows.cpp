/*
 * yerith-erp-windows.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/users/yerith-erp-user-caissier.hpp"

#include "src/users/yerith-erp-user-magasinier.hpp"

#include "src/users/yerith-erp-user-manager.hpp"

#include "src/users/yerith-erp-user-administrateur.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/admin/yerith-erp-admin-window.hpp"


#ifdef YERITH_SERVER
#include "src/dbus/YERITH_RUNTIME_VERIFIER.hpp"
#endif

#ifdef YERITH_CLIENT
#include "src/dbus/yerith-erp-dbus-client.hpp"
#endif


#include <QtCore/QList>

#include <QtCore/QMap>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlDriver>

#include <QtSql/QSqlQuery>

#include <QtWidgets/QDesktopWidget>


QTranslator *YerithERPWindows::_FRENCH_TRANSLATOR = 0;

QTranslator *YerithERPWindows::_ENGLISH_TRANSLATOR = 0;

QApplication *YerithERPWindows::_YERITH_ERP_PGI_APPLICATION = 0;

bool YerithERPWindows::CURRENTLY_CHANGING_USER(false);


QMap <QString, YerithSqlTableModel *> YerithERPWindows::_sqltablenameTOsqltablemodel;


YerithERPWindows::YerithERPWindows(QDesktopWidget *desktopWidget):_mainWindow(0),
    _recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW(0),
    _transactionsDunFournisseurWindow(0),
    _transactionsDunClientWindow(0),
    _historiqueDuStockWindow(0),
    _changerUtilisateurDialog(0),
    _creerFournisseurWindow(0),
    _detailsDunProgrammeDeFideliteClientsWindow(0),
    _creerUnProgrammeDeFideliteClientsWindow(0),
    _creerGroupeDeClientsWindow(0),
    _creerCompteClientWindow(0),
    _modifierFournisseurWindow(0),
    _modifierCompteClientWindow(0),
    _achatsWindow(0),
    _stocksWindow(0),
    _marchandisesWindow(0),
    _comptabiliteWindow(0),
    _listerAlertesWindow(0),
    _paiementsWindow(0),
    _ventesWindow(0),
    _modifierWindow(0),
    _sortirWindow(0),
    _statistiquesWindow(0),
    _mouvementsDeStocksWindow(0),
    _pdVenteWindow(0),
    _fournisseurDetailsWindow(0),
    _programmesDeFideliteClientsWindow(0),
    _detailsGroupeDeClientsWindow(0),
    _groupesDeClientsWindow(0),
    _groupesDunClientWindow(0),
    _clientsDetailWindow(0),
    _charges_financieresWindow(0),
    _achatsDetailWindow(0),
    _detailWindow(0),
    _entrerWindow(0),
    _pdVenteListStocksWindow(0),
    _impressionDeDocumentDialog(0),
    _pdVenteMethodePaiementCarteCreditDialog(0),
    _pdVenteMethodePaiementComptantEntreeDialog(0),
    _pdVenteMethodePaiementDialog(0),
	_groupes_DUN_EMPLOYE_window(0),
	_CREER_GROUPE_DE_PAIE_hr_window(0),
	_CREER_groupe_demployes_window(0),
	_DETAILS_DUN_GROUPE_DEMPLOYES_hr(0),
	_DETAILS_dun_groupe_de_PAIE_hr(0),
	_groupe_demployes_window(0),
	_GROUPES_DE_PAIE_Window(0),
    _fournisseursWindow(0),
    _clientsWindow(0),
    _payerAuCompteClientWindow(0),
    _sortirListStocksWindow(0),
    _adminWindow(0),
    _adminCreateWindow(0),
    _adminListerWindow(0),
    _adminDetailWindow(0),
    _adminModifierWindow(0),
    _database(0),
    _tableModel_periodes_dappartenance_groupes_de_paie_hr(0),
    _tableModel_imprimantereseau_recus_petits(0),
    _tableModel_types_doperations_financieres(0),
    _tableModel_operations_comptables(0),
    _tableModel_comptes_bancaires(0),
    _tableModel_entreprise_info(0),
    _tableModel_historique_paiements(0),
    _tableModel_users(0),
    _tableModel_titres(0),
    _tableModel_localisations(0),
    _tableModel_departements_produits(0),
    _tableModel_categories(0),
	_tableModel_ligne_budgetaire(0),
    _tableModel_programmes_de_fidelite_clients(0),
	_tableModel_groupes_demployes_hr(0),
	_tableModel_groupes_de_paie_hr(0),
    _tableModel_clients(0),
	_tableModel_groupes_de_clients(0),
    _tableModel_fournisseurs(0),
    _tableModel_alertes(0),
    _tableModel_remises(0),
    _tableModel_conditions_alertes(0),
    _tableModel_courriers_alertes(0),
    _tableModel_charges_financieres(0),
    _tableModel_achats(0),
    _tableModel_stocks(0),
    _tableModel_services_completes(0),
    _tableModel_stocks_vendu(0),
    _tableModel_stocks_sorties(0),
    _tableModel_marchandises(0),
    _tableModel_configurations(0),
    _tableModel_init_configurations(0),
#ifdef YERITH_SERVER
    _dbusServer(0),
#endif
#ifdef YERITH_CLIENT
    _dbusClient(0),
#endif
    _logger(0),
    _infoEntreprise(0),
    _user(0),
    _currentPosition(0),
    _desktopWidget(desktopWidget)
{
    _logger = new YerithLogger("YerithWindows");

    YerithERPWindows::setupConfiguration();

    static YerithDatabaseTableColumn aYerithDatabaseTableColumnStaticInstance;

    setupSqlTableModelFromName(YerithDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR,
                               &_tableModel_periodes_dappartenance_groupes_de_paie_hr);

    setupSqlTableModelFromName(YerithDatabase::IMPRIMANTERESEAU_RECUS_PETITS,
                               &_tableModel_imprimantereseau_recus_petits);

    setupSqlTableModelFromName(YerithDatabase::TYPE_DOPERATIONS_FINANCIERES,
                               &_tableModel_types_doperations_financieres);

    setupSqlTableModelFromName(YerithDatabase::OPERATIONS_COMPTABLES,
                               &_tableModel_operations_comptables);

    setupSqlTableModelFromName(YerithDatabase::COMPTES_BANCAIRES,
                               &_tableModel_comptes_bancaires);

    setupSqlTableModelFromName(YerithDatabase::ENTREPRISE_INFO,
                               &_tableModel_entreprise_info);

    setupSqlTableModelFromName(YerithDatabase::PAIEMENTS,
                               &_tableModel_historique_paiements);

    setupSqlTableModelFromName(YerithDatabase::USERS, &_tableModel_users);

    setupSqlTableModelFromName(YerithDatabase::TITRES, &_tableModel_titres);

    setupSqlTableModelFromName(YerithDatabase::LOCALISATIONS,
                               &_tableModel_localisations);

    setupSqlTableModelFromName(YerithDatabase::DEPARTEMENTS_PRODUITS,
                               &_tableModel_departements_produits);

    setupSqlTableModelFromName(YerithDatabase::CATEGORIES,
                               &_tableModel_categories);

    setupSqlTableModelFromName(YerithDatabase::LIGNES_BUDGETAIRES,
                               &_tableModel_ligne_budgetaire);

    setupSqlTableModelFromName(YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                               &_tableModel_programmes_de_fidelite_clients);

    setupSqlTableModelFromName(YerithDatabase::GROUPES_DEMPLOYES_hr,
                                   &_tableModel_groupes_demployes_hr);

    setupSqlTableModelFromName(YerithDatabase::GROUPES_DE_PAIE_hr,
                                   &_tableModel_groupes_de_paie_hr);

    setupSqlTableModelFromName(YerithDatabase::CLIENTS, &_tableModel_clients);

    setupSqlTableModelFromName(YerithDatabase::GROUPES_DE_CLIENTS,
                               &_tableModel_groupes_de_clients);

    setupSqlTableModelFromName(YerithDatabase::FOURNISSEURS,
                               &_tableModel_fournisseurs);

    setupSqlTableModelFromName(YerithDatabase::ALERTES, &_tableModel_alertes);
    setupSqlTableModelFromName(YerithDatabase::REMISES, &_tableModel_remises);
    setupSqlTableModelFromName(YerithDatabase::CONDITIONS_ALERTES,
                               &_tableModel_conditions_alertes);
    setupSqlTableModelFromName(YerithDatabase::COURRIERS_ALERTES,
                               &_tableModel_courriers_alertes);
    setupSqlTableModelFromName(YerithDatabase::CHARGES_FINANCIERES,
                               &_tableModel_charges_financieres);
    setupSqlTableModelFromName(YerithDatabase::ACHATS, &_tableModel_achats);
    setupSqlTableModelFromName(YerithDatabase::STOCKS, &_tableModel_stocks);
    setupSqlTableModelFromName(YerithDatabase::SERVICES_COMPLETES,
                               &_tableModel_services_completes);
    setupSqlTableModelFromName(YerithDatabase::STOCKS_VENDU,
                               &_tableModel_stocks_vendu);
    setupSqlTableModelFromName(YerithDatabase::STOCKS_SORTIES,
                               &_tableModel_stocks_sorties);
    setupSqlTableModelFromName(YerithDatabase::MARCHANDISES,
                               &_tableModel_marchandises);
    setupSqlTableModelFromName(YerithDatabase::CONFIGURATIONS,
                               &_tableModel_configurations);
    setupSqlTableModelFromName(YerithDatabase::INIT_CONFIGURATIONS,
                               &_tableModel_init_configurations);

    /*
     * This is to initialize constants useful for 'INTERNATIONALIZATION'.
     */
    YerithUtils
    yerithUtilsDummy;

#ifdef YERITH_SERVER
    _dbusServer = new YERITH_RUNTIME_VERIFIER;
#endif

#ifdef YERITH_CLIENT
    _dbusClient = new YERITH_RUNTIME_VERIFIER(this);
#endif
}

YerithERPWindows::~YerithERPWindows()
{
    delete _changerUtilisateurDialog;

    delete _mainWindow;
    delete _sortirWindow;
    delete _statistiquesWindow;
    delete _mouvementsDeStocksWindow;
    delete _impressionDeDocumentDialog;
    delete _pdVenteMethodePaiementCarteCreditDialog;
    delete _pdVenteMethodePaiementComptantEntreeDialog;
    delete _pdVenteMethodePaiementDialog;
    delete _pdVenteWindow;
    delete _detailWindow;
    delete _achatsDetailWindow;
    delete _charges_financieres_detailsWindow;
    delete _charges_financieresWindow;
    delete _clientsDetailWindow;
    delete _groupesDunClientWindow;
    delete _groupesDeClientsWindow;
    delete _detailsGroupeDeClientsWindow;
    delete _programmesDeFideliteClientsWindow;
    delete _fournisseurDetailsWindow;
    delete _modifierWindow;
    delete _entrerWindow;
    delete _creerCompteClientWindow;
    delete _creerGroupeDeClientsWindow;
    delete _creerUnProgrammeDeFideliteClientsWindow;
    delete _detailsDunProgrammeDeFideliteClientsWindow;
    delete _creerFournisseurWindow;
    delete _modifierCompteClientWindow;
    delete _modifierFournisseurWindow;
    delete _stocksWindow;
    delete _achatsWindow;
    delete _marchandisesWindow;
    delete _comptabiliteWindow;
    delete _listerAlertesWindow;
    delete _paiementsWindow;
    delete _ventesWindow;
    delete _pdVenteListStocksWindow;
    delete _historiqueDuStockWindow;
    delete _transactionsDunClientWindow;
    delete _transactionsDunFournisseurWindow;
    delete _recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW;
    delete _clientsWindow;
    delete _GROUPES_DE_PAIE_Window;
    delete _groupe_demployes_window;
    delete _DETAILS_dun_groupe_de_PAIE_hr;
    delete _DETAILS_DUN_GROUPE_DEMPLOYES_hr;
    delete _CREER_groupe_demployes_window;
    delete _groupes_DUN_EMPLOYE_window;
    delete _fournisseursWindow;
    delete _payerAuCompteClientWindow;
    delete _sortirListStocksWindow;

    delete _adminWindow;
    delete _adminCreateWindow;
    delete _adminListerWindow;
    delete _adminDetailWindow;
    delete _adminModifierWindow;

#ifdef YERITH_SERVER
    delete _dbusServer;
#endif

#ifdef YERITH_CLIENT
    delete _dbusClient;
#endif

    delete _logger;
    delete _infoEntreprise;
}

void YerithERPWindows::createAllYerithPosUserWindows()
{
    _infoEntreprise = new YerithInfoEntreprise;

    _changerUtilisateurDialog = new YerithPOSChangerUtilisateurDialog(this);

    _mainWindow = new YerithMainWindow;

    _currentPosition = new QPoint;

    YerithUtils::getCenterPosition(_desktopWidget->width(),
                                   _desktopWidget->height(),
                                   *_mainWindow, *_currentPosition);

    YerithWindowsCommons::setCenterPosition(_currentPosition);

    YerithPOSAdminWindowsCommons::setCenterPosition(_currentPosition);

    _adminWindow = new YerithAdminWindow;
    _adminCreateWindow = new YerithAdminCreateWindow;
    _adminListerWindow = new YerithAdminListerWindow;
    _adminDetailWindow = new YerithAdminDetailWindow;
    _adminModifierWindow = new YerithAdminModifierWindow;

    _recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW =
                    new YerithRechercheConditionnelleWindow;
    _transactionsDunFournisseurWindow =
                    new YerithTableauDesTransactionsDuFournisseurWindow;
    _transactionsDunClientWindow =
                    new YerithTableauDesTransactionsDuClientWindow;
    _historiqueDuStockWindow = new YerithHistoriqueDuStockWindow;
    _sortirWindow = new YerithSortirWindow;
    _statistiquesWindow = new YerithTableauxDeBordWindow;
    _mouvementsDeStocksWindow = new YerithMouvementsDeStocksWindow;

    _impressionDeDocumentDialog = new YerithIMPRESSION_DE_DOCUMENT_Dialog;

    _pdVenteMethodePaiementCarteCreditDialog
        = new YerithPOSVenteMethodePaiementCarteCreditDialog(_pdVenteWindow);

    _pdVenteMethodePaiementComptantEntreeDialog
        =
                        new YerithPOSVenteMethodePaiementComptantEntreeDialog(this,
                                                                              _pdVenteWindow);

    _pdVenteMethodePaiementDialog =
                    new YerithPOSVenteMethodePaiementDialog(this, _pdVenteWindow);

    _pdVenteWindow = new YerithPointDeVenteWindow;
    _fournisseurDetailsWindow = new YerithFournisseurDetailsWindow;
    _programmesDeFideliteClientsWindow =
                    new YerithERPProgrammesDeFideliteClientsWindow;
    _detailsGroupeDeClientsWindow = new YerithDetailsDunGroupeDeClientsWindow;
    _groupesDeClientsWindow = new YerithGroupesDeClientsWindow;
    _groupesDunClientWindow = new YerithGroupesDunClientWindow;
    _clientsDetailWindow = new YerithClientsDetailWindow;
    _charges_financieresWindow = new YerithChargesFinancieresWindow;
    _charges_financieres_detailsWindow =
                    new YerithChargesFinancieresDetailsWindow;
    _achatsDetailWindow = new YerithAchatsDetailWindow;
    _detailWindow = new YerithStockDetailWindow;
    _modifierWindow = new YerithModifierWindow;
    _entrerWindow = new YerithEntrerWindow;
    _creerFournisseurWindow = new YerithCreerFournisseurWindow;
    _detailsDunProgrammeDeFideliteClientsWindow =
                    new YerithDetailsDunProgrammeDeFideliteClientsWindow;
    _creerUnProgrammeDeFideliteClientsWindow =
                    new YerithCreerUnProgrammeDeFideliteClientsWindow;
    _creerGroupeDeClientsWindow = new YerithCreerGroupeDeClientsWindow;
    _creerCompteClientWindow = new YerithCreerCompteClientWindow;
    _modifierFournisseurWindow = new YerithModifierFournisseurWindow;
    _modifierCompteClientWindow = new YerithModifierCompteClientWindow;
    _achatsWindow = new YerithAchatsWindow;
    _stocksWindow = new YerithStocksWindow;
    _marchandisesWindow = new YerithMarchandisesWindow;
    _comptabiliteWindow = new YerithComptabiliteWindow;
    _listerAlertesWindow = new YerithAlertesWindow;
    _paiementsWindow = new YerithPaiementsWindow;
    _ventesWindow = new YerithVentesWindow;
    _pdVenteListStocksWindow = new YerithPointDeVenteListStocksWindow;
    _groupes_DUN_EMPLOYE_window = new YerithGROUPES_DUN_EMPLOYE_Window;
    _CREER_GROUPE_DE_PAIE_hr_window = new YerithCreerUnGroupeDePaieWindow;
    _CREER_groupe_demployes_window = new YerithCreerGroupeDemployesWindow;
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr = new YerithDetailsDunGroupeDemployesWindow;
    _DETAILS_dun_groupe_de_PAIE_hr = new YerithDetailsDunGroupeDePaieWindow;
    _groupe_demployes_window = new YerithERPGroupesDemployesHRWindow;
    _GROUPES_DE_PAIE_Window = new YerithERPGROUPES_DE_PAIE_Window;
    _fournisseursWindow = new YerithERPFournisseursWindow;
    _clientsWindow = new YerithERPClientsWindow;
    _payerAuCompteClientWindow = new YerithPayerCompteClientWindow;
    _sortirListStocksWindow = new YerithSortirListStocksWindow;

    YerithERPWindows::move(*_mainWindow);
}

void YerithERPWindows::reinitialiseSqlTableModels()
{
    delete _tableModel_periodes_dappartenance_groupes_de_paie_hr;
    delete _tableModel_imprimantereseau_recus_petits;
    delete _tableModel_types_doperations_financieres;
    delete _tableModel_operations_comptables;
    delete _tableModel_comptes_bancaires;
    delete _tableModel_entreprise_info;
    delete _tableModel_historique_paiements;
    delete _tableModel_users;
    delete _tableModel_titres;
    delete _tableModel_localisations;
    delete _tableModel_departements_produits;
    delete _tableModel_categories;
    delete _tableModel_ligne_budgetaire;
    delete _tableModel_programmes_de_fidelite_clients;
    delete _tableModel_groupes_demployes_hr;
    delete _tableModel_groupes_de_paie_hr;
    delete _tableModel_clients;
    delete _tableModel_groupes_de_clients;
    delete _tableModel_fournisseurs;
    delete _tableModel_alertes;
    delete _tableModel_remises;
    delete _tableModel_conditions_alertes;
    delete _tableModel_courriers_alertes;
    delete _tableModel_stocks;
    delete _tableModel_achats;
    delete _tableModel_marchandises;
    delete _tableModel_services_completes;
    delete _tableModel_stocks_vendu;
    delete _tableModel_stocks_sorties;
    delete _tableModel_configurations;
    delete _tableModel_init_configurations;

    _tableModel_periodes_dappartenance_groupes_de_paie_hr =
        new YerithSqlTableModel(YerithDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR);

    _tableModel_imprimantereseau_recus_petits =
        new YerithSqlTableModel(YerithDatabase::IMPRIMANTERESEAU_RECUS_PETITS);

    _tableModel_types_doperations_financieres =
        new YerithSqlTableModel(YerithDatabase::TYPE_DOPERATIONS_FINANCIERES);

    _tableModel_operations_comptables =
        new YerithSqlTableModel(YerithDatabase::OPERATIONS_COMPTABLES);

    _tableModel_comptes_bancaires =
        new YerithSqlTableModel(YerithDatabase::COMPTES_BANCAIRES);

    _tableModel_entreprise_info =
                    new YerithSqlTableModel(YerithDatabase::ENTREPRISE_INFO);
    _tableModel_historique_paiements =
                    new YerithSqlTableModel(YerithDatabase::PAIEMENTS);
    _tableModel_users = new YerithSqlTableModel(YerithDatabase::USERS);
    _tableModel_titres = new YerithSqlTableModel(YerithDatabase::TITRES);
    _tableModel_localisations =
                    new YerithSqlTableModel(YerithDatabase::LOCALISATIONS);
    _tableModel_departements_produits =
                    new YerithSqlTableModel(YerithDatabase::DEPARTEMENTS_PRODUITS);
    _tableModel_categories =
                    new YerithSqlTableModel(YerithDatabase::CATEGORIES);
    _tableModel_ligne_budgetaire =
                    new YerithSqlTableModel(YerithDatabase::LIGNES_BUDGETAIRES);
    _tableModel_programmes_de_fidelite_clients =
                    new YerithSqlTableModel(YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS);

    _tableModel_groupes_demployes_hr = new YerithSqlTableModel(YerithDatabase::GROUPES_DEMPLOYES_hr);

    _tableModel_groupes_de_paie_hr = new YerithSqlTableModel(YerithDatabase::GROUPES_DE_PAIE_hr);

    _tableModel_clients = new YerithSqlTableModel(YerithDatabase::CLIENTS);

    _tableModel_groupes_de_clients =
                    new YerithSqlTableModel(YerithDatabase::GROUPES_DE_CLIENTS);
    _tableModel_fournisseurs =
                    new YerithSqlTableModel(YerithDatabase::FOURNISSEURS);
    _tableModel_alertes = new YerithSqlTableModel(YerithDatabase::ALERTES);
    _tableModel_remises = new YerithSqlTableModel(YerithDatabase::REMISES);
    _tableModel_conditions_alertes =
                    new YerithSqlTableModel(YerithDatabase::CONDITIONS_ALERTES);
    _tableModel_courriers_alertes =
                    new YerithSqlTableModel(YerithDatabase::COURRIERS_ALERTES);
    _tableModel_achats = new YerithSqlTableModel(YerithDatabase::ACHATS);
    _tableModel_stocks = new YerithSqlTableModel(YerithDatabase::STOCKS);
    _tableModel_services_completes =
                    new YerithSqlTableModel(YerithDatabase::SERVICES_COMPLETES);
    _tableModel_marchandises =
                    new YerithSqlTableModel(YerithDatabase::MARCHANDISES);
    _tableModel_stocks_vendu =
                    new YerithSqlTableModel(YerithDatabase::STOCKS_VENDU);
    _tableModel_stocks_sorties =
                    new YerithSqlTableModel(YerithDatabase::STOCKS_SORTIES);
    _tableModel_configurations =
                    new YerithSqlTableModel(YerithDatabase::CONFIGURATIONS);
    _tableModel_init_configurations =
                    new YerithSqlTableModel(YerithDatabase::INIT_CONFIGURATIONS);
}

void YerithERPWindows::setUser(YerithPOSUser *user)
{
    _user = user;

    if (0 != _user)
    {
        _user->create_user_personal_settings_file();
    }

    switch (_user->role())
    {

    case YerithUtils::ROLE_ADMINISTRATEUR:
        definirAdministrateur();
        break;

    case YerithUtils::ROLE_MANAGER:
        definirManager();
        break;

    case YerithUtils::ROLE_VENDEUR:
        definirVendeur();
        break;

    case YerithUtils::ROLE_GESTIONNAIREDESTOCK:
        definirGestionaireDesStocks();
        break;

    case YerithUtils::ROLE_MAGASINIER:
        definirMagasinier();
        break;

    case YerithUtils::ROLE_CAISSIER:
        definirCaissier();
        break;

    default:
        break;
    }
}


void YerithERPWindows::setupSqlTableModelFromName(const QString &aSqlTableName,
                                                  YerithSqlTableModel **
                                                  aYerithSqlTableModel)
{
    *aYerithSqlTableModel = new YerithSqlTableModel(aSqlTableName);

    _sqltablenameTOsqltablemodel.insert(aSqlTableName, *aYerithSqlTableModel);
}


void YerithERPWindows::setupConfiguration()
{
    _logger->log("setupConfiguration");
}

void YerithERPWindows::definirAdministrateur()
{
    _adminWindow->definirAdministrateur();
    _adminCreateWindow->definirAdministrateur();
    _adminListerWindow->definirAdministrateur();
    _adminDetailWindow->definirAdministrateur();
    _adminModifierWindow->definirAdministrateur();
}

void YerithERPWindows::definirMagasinier()
{
    _mainWindow->definirMagasinier();
    _sortirWindow->definirMagasinier();
    _statistiquesWindow->definirMagasinier();
    _mouvementsDeStocksWindow->definirMagasinier();
    _pdVenteWindow->definirMagasinier();
    _fournisseurDetailsWindow->definirMagasinier();
    _programmesDeFideliteClientsWindow->definirMagasinier();
    _detailsGroupeDeClientsWindow->definirMagasinier();
    _groupesDeClientsWindow->definirMagasinier();
    _groupesDunClientWindow->definirMagasinier();
    _clientsDetailWindow->definirMagasinier();
    _charges_financieresWindow->definirMagasinier();
    _charges_financieres_detailsWindow->definirMagasinier();
    _achatsDetailWindow->definirMagasinier();
    _detailWindow->definirMagasinier();
    _modifierWindow->definirMagasinier();
    _entrerWindow->definirMagasinier();
    _creerFournisseurWindow->definirMagasinier();
    _detailsDunProgrammeDeFideliteClientsWindow->definirMagasinier();
    _creerUnProgrammeDeFideliteClientsWindow->definirMagasinier();
    _creerGroupeDeClientsWindow->definirMagasinier();
    _creerCompteClientWindow->definirMagasinier();
    _modifierFournisseurWindow->definirMagasinier();
    _modifierCompteClientWindow->definirMagasinier();
    _achatsWindow->definirMagasinier();
    _stocksWindow->definirMagasinier();
    _marchandisesWindow->definirMagasinier();
    _listerAlertesWindow->definirMagasinier();
    _paiementsWindow->definirMagasinier();
    _ventesWindow->definirMagasinier();
    _groupes_DUN_EMPLOYE_window->definirMagasinier();
    _CREER_GROUPE_DE_PAIE_hr_window->definirMagasinier();
    _CREER_groupe_demployes_window->definirMagasinier();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirMagasinier();
    _DETAILS_dun_groupe_de_PAIE_hr->definirMagasinier();
    _groupe_demployes_window->definirMagasinier();
    _GROUPES_DE_PAIE_Window->definirMagasinier();
    _fournisseursWindow->definirMagasinier();
    _clientsWindow->definirMagasinier();
    _payerAuCompteClientWindow->definirMagasinier();
}

void YerithERPWindows::definirCaissier()
{
    _mainWindow->definirCaissier();
    _sortirWindow->definirCaissier();
    _statistiquesWindow->definirCaissier();
    _mouvementsDeStocksWindow->definirCaissier();
    _pdVenteWindow->definirCaissier();
    _fournisseurDetailsWindow->definirCaissier();
    _programmesDeFideliteClientsWindow->definirCaissier();
    _detailsGroupeDeClientsWindow->definirCaissier();
    _groupesDeClientsWindow->definirCaissier();
    _groupesDunClientWindow->definirCaissier();
    _clientsDetailWindow->definirCaissier();
    _charges_financieresWindow->definirCaissier();
    _charges_financieres_detailsWindow->definirCaissier();
    _achatsDetailWindow->definirCaissier();
    _detailWindow->definirCaissier();
    _modifierWindow->definirCaissier();
    _entrerWindow->definirCaissier();
    _creerFournisseurWindow->definirCaissier();
    _detailsDunProgrammeDeFideliteClientsWindow->definirCaissier();
    _creerUnProgrammeDeFideliteClientsWindow->definirCaissier();
    _creerGroupeDeClientsWindow->definirCaissier();
    _creerCompteClientWindow->definirCaissier();
    _modifierFournisseurWindow->definirCaissier();
    _modifierCompteClientWindow->definirCaissier();
    _achatsWindow->definirCaissier();
    _stocksWindow->definirCaissier();
    _marchandisesWindow->definirCaissier();
    _listerAlertesWindow->definirCaissier();
    _paiementsWindow->definirCaissier();
    _ventesWindow->definirCaissier();
    _groupes_DUN_EMPLOYE_window->definirCaissier();
    _CREER_GROUPE_DE_PAIE_hr_window->definirCaissier();
    _CREER_groupe_demployes_window->definirCaissier();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirCaissier();
    _DETAILS_dun_groupe_de_PAIE_hr->definirCaissier();
    _groupe_demployes_window->definirCaissier();
    _GROUPES_DE_PAIE_Window->definirCaissier();
    _fournisseursWindow->definirCaissier();
    _clientsWindow->definirCaissier();
    _payerAuCompteClientWindow->definirCaissier();
}

void YerithERPWindows::definirManager()
{
    _mainWindow->definirManager();
    _sortirWindow->definirManager();
    _statistiquesWindow->definirManager();
    _mouvementsDeStocksWindow->definirManager();
    _pdVenteWindow->definirManager();
    _fournisseurDetailsWindow->definirManager();
    _programmesDeFideliteClientsWindow->definirManager();
    _detailsGroupeDeClientsWindow->definirManager();
    _groupesDeClientsWindow->definirManager();
    _groupesDunClientWindow->definirManager();
    _clientsDetailWindow->definirManager();
    _charges_financieresWindow->definirManager();
    _charges_financieres_detailsWindow->definirManager();
    _achatsDetailWindow->definirManager();
    _detailWindow->definirManager();
    _modifierWindow->definirManager();
    _entrerWindow->definirManager();
    _creerFournisseurWindow->definirManager();
    _detailsDunProgrammeDeFideliteClientsWindow->definirManager();
    _creerUnProgrammeDeFideliteClientsWindow->definirManager();
    _creerGroupeDeClientsWindow->definirManager();
    _creerCompteClientWindow->definirManager();
    _modifierFournisseurWindow->definirManager();
    _modifierCompteClientWindow->definirManager();
    _achatsWindow->definirManager();
    _stocksWindow->definirManager();
    _marchandisesWindow->definirManager();
    _comptabiliteWindow->definirManager();
    _listerAlertesWindow->definirManager();
    _paiementsWindow->definirManager();
    _ventesWindow->definirManager();
    _groupes_DUN_EMPLOYE_window->definirManager();
    _CREER_GROUPE_DE_PAIE_hr_window->definirManager();
    _CREER_groupe_demployes_window->definirManager();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirManager();
    _DETAILS_dun_groupe_de_PAIE_hr->definirManager();
    _groupe_demployes_window->definirManager();
    _GROUPES_DE_PAIE_Window->definirManager();
    _fournisseursWindow->definirManager();
    _clientsWindow->definirManager();
    _payerAuCompteClientWindow->definirManager();

#ifdef YERITH_FEATURES_COMPTABILITE_yes
    _comptabiliteWindow->definirManager();
#endif

    _adminWindow->definirManager();
    _adminCreateWindow->definirManager();
    _adminListerWindow->definirManager();
    _adminDetailWindow->definirManager();
    _adminModifierWindow->definirManager();
}


void YerithERPWindows::definirVendeur()
{
    _mainWindow->definirVendeur();
    _sortirWindow->definirVendeur();
    _statistiquesWindow->definirVendeur();
    _mouvementsDeStocksWindow->definirVendeur();
    _pdVenteWindow->definirVendeur();
    _fournisseurDetailsWindow->definirVendeur();
    _programmesDeFideliteClientsWindow->definirVendeur();
    _detailsGroupeDeClientsWindow->definirVendeur();
    _groupesDeClientsWindow->definirVendeur();
    _groupesDunClientWindow->definirVendeur();
    _clientsDetailWindow->definirVendeur();
    _charges_financieresWindow->definirVendeur();
    _charges_financieres_detailsWindow->definirVendeur();
    _achatsDetailWindow->definirVendeur();
    _detailWindow->definirVendeur();
    _modifierWindow->definirVendeur();
    _entrerWindow->definirVendeur();
    _creerFournisseurWindow->definirVendeur();
    _detailsDunProgrammeDeFideliteClientsWindow->definirVendeur();
    _creerUnProgrammeDeFideliteClientsWindow->definirVendeur();
    _creerGroupeDeClientsWindow->definirVendeur();
    _creerCompteClientWindow->definirVendeur();
    _modifierFournisseurWindow->definirVendeur();
    _modifierCompteClientWindow->definirVendeur();
    _achatsWindow->definirVendeur();
    _stocksWindow->definirVendeur();
    _marchandisesWindow->definirVendeur();
    _listerAlertesWindow->definirVendeur();
    _paiementsWindow->definirVendeur();
    _ventesWindow->definirVendeur();
    _groupes_DUN_EMPLOYE_window->definirVendeur();
    _CREER_GROUPE_DE_PAIE_hr_window->definirVendeur();
    _CREER_groupe_demployes_window->definirVendeur();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirVendeur();
    _DETAILS_dun_groupe_de_PAIE_hr->definirVendeur();
    _groupe_demployes_window->definirVendeur();
    _GROUPES_DE_PAIE_Window->definirVendeur();
    _fournisseursWindow->definirVendeur();
    _clientsWindow->definirVendeur();
    _payerAuCompteClientWindow->definirVendeur();

    _adminWindow->definirVendeur();
    _adminCreateWindow->definirVendeur();
    _adminListerWindow->definirVendeur();
    _adminDetailWindow->definirVendeur();
    _adminModifierWindow->definirVendeur();
}


void YerithERPWindows::definirGestionaireDesStocks()
{
    _mainWindow->definirGestionaireDesStocks();
    _sortirWindow->definirGestionaireDesStocks();
    _statistiquesWindow->definirGestionaireDesStocks();
    _mouvementsDeStocksWindow->definirGestionaireDesStocks();
    _pdVenteWindow->definirGestionaireDesStocks();
    _fournisseurDetailsWindow->definirGestionaireDesStocks();
    _programmesDeFideliteClientsWindow->definirGestionaireDesStocks();
    _detailsGroupeDeClientsWindow->definirGestionaireDesStocks();
    _groupesDeClientsWindow->definirGestionaireDesStocks();
    _groupesDunClientWindow->definirGestionaireDesStocks();
    _clientsDetailWindow->definirGestionaireDesStocks();
    _charges_financieresWindow->definirGestionaireDesStocks();
    _charges_financieres_detailsWindow->definirGestionaireDesStocks();
    _achatsDetailWindow->definirGestionaireDesStocks();
    _detailWindow->definirGestionaireDesStocks();
    _modifierWindow->definirGestionaireDesStocks();
    _entrerWindow->definirGestionaireDesStocks();
    _creerFournisseurWindow->definirGestionaireDesStocks();
    _detailsDunProgrammeDeFideliteClientsWindow->definirGestionaireDesStocks();
    _creerUnProgrammeDeFideliteClientsWindow->definirGestionaireDesStocks();
    _creerGroupeDeClientsWindow->definirGestionaireDesStocks();
    _creerCompteClientWindow->definirGestionaireDesStocks();
    _modifierFournisseurWindow->definirGestionaireDesStocks();
    _modifierCompteClientWindow->definirGestionaireDesStocks();
    _achatsWindow->definirGestionaireDesStocks();
    _stocksWindow->definirGestionaireDesStocks();
    _marchandisesWindow->definirGestionaireDesStocks();
    _listerAlertesWindow->definirGestionaireDesStocks();
    _paiementsWindow->definirGestionaireDesStocks();
    _ventesWindow->definirGestionaireDesStocks();
    _groupes_DUN_EMPLOYE_window->definirGestionaireDesStocks();
    _CREER_GROUPE_DE_PAIE_hr_window->definirGestionaireDesStocks();
    _CREER_groupe_demployes_window->definirGestionaireDesStocks();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirGestionaireDesStocks();
    _DETAILS_dun_groupe_de_PAIE_hr->definirGestionaireDesStocks();
    _groupe_demployes_window->definirGestionaireDesStocks();
    _GROUPES_DE_PAIE_Window->definirGestionaireDesStocks();
    _fournisseursWindow->definirGestionaireDesStocks();
    _clientsWindow->definirGestionaireDesStocks();
    _payerAuCompteClientWindow->definirGestionaireDesStocks();

    _adminWindow->definirGestionaireDesStocks();
    _adminCreateWindow->definirGestionaireDesStocks();
    _adminListerWindow->definirGestionaireDesStocks();
    _adminDetailWindow->definirGestionaireDesStocks();
    _adminModifierWindow->definirGestionaireDesStocks();
}


/**
 * We only need to call the method 'definirPasDe Role'
 * on the _mainWindow instance because all other widget
 * instances are not yet instantiated.
 */
void YerithERPWindows::definirPasDeRole()
{
    _mainWindow->definirPasDeRole();
    _sortirWindow->definirPasDeRole();
    _statistiquesWindow->definirPasDeRole();
    _mouvementsDeStocksWindow->definirPasDeRole();
    _pdVenteWindow->definirPasDeRole();
    _fournisseurDetailsWindow->definirPasDeRole();
    _programmesDeFideliteClientsWindow->definirPasDeRole();
    _detailsGroupeDeClientsWindow->definirPasDeRole();
    _groupesDeClientsWindow->definirPasDeRole();
    _groupesDunClientWindow->definirPasDeRole();
    _clientsDetailWindow->definirPasDeRole();
    _charges_financieresWindow->definirPasDeRole();
    _charges_financieres_detailsWindow->definirPasDeRole();
    _achatsDetailWindow->definirPasDeRole();
    _detailWindow->definirPasDeRole();
    _modifierWindow->definirPasDeRole();
    _entrerWindow->definirPasDeRole();
    _creerFournisseurWindow->definirPasDeRole();
    _detailsDunProgrammeDeFideliteClientsWindow->definirPasDeRole();
    _creerUnProgrammeDeFideliteClientsWindow->definirPasDeRole();
    _creerGroupeDeClientsWindow->definirPasDeRole();
    _creerCompteClientWindow->definirPasDeRole();
    _modifierFournisseurWindow->definirPasDeRole();
    _modifierCompteClientWindow->definirPasDeRole();
    _achatsWindow->definirPasDeRole();
    _stocksWindow->definirPasDeRole();
    _marchandisesWindow->definirPasDeRole();
    _comptabiliteWindow->definirPasDeRole();
    _listerAlertesWindow->definirPasDeRole();
    _paiementsWindow->definirPasDeRole();
    _ventesWindow->definirPasDeRole();
    _groupes_DUN_EMPLOYE_window->definirPasDeRole();
    _CREER_GROUPE_DE_PAIE_hr_window->definirPasDeRole();
    _CREER_groupe_demployes_window->definirPasDeRole();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirPasDeRole();
    _DETAILS_dun_groupe_de_PAIE_hr->definirPasDeRole();
    _groupe_demployes_window->definirPasDeRole();
    _GROUPES_DE_PAIE_Window->definirPasDeRole();
    _fournisseursWindow->definirPasDeRole();
    _clientsWindow->definirPasDeRole();
    _payerAuCompteClientWindow->definirPasDeRole();

    _adminWindow->definirPasDeRole();
    _adminCreateWindow->definirPasDeRole();
    _adminListerWindow->definirPasDeRole();
    _adminDetailWindow->definirPasDeRole();
    _adminModifierWindow->definirPasDeRole();
}

void YerithERPWindows::hideAllWindows()
{
    _sortirWindow->rendreInvisible();
    _transactionsDunFournisseurWindow->rendreInvisible();
    _transactionsDunClientWindow->rendreInvisible();
    _historiqueDuStockWindow->rendreInvisible();
    _statistiquesWindow->rendreInvisible();
    _mouvementsDeStocksWindow->rendreInvisible();
    _pdVenteWindow->rendreInvisible();
    _pdVenteListStocksWindow->rendreInvisible();
    _impressionDeDocumentDialog->rendreInvisible();
    _pdVenteMethodePaiementCarteCreditDialog->rendreInvisible();
    _pdVenteMethodePaiementComptantEntreeDialog->rendreInvisible();
    _pdVenteMethodePaiementDialog->rendreInvisible();
    _sortirListStocksWindow->rendreInvisible();
    _fournisseurDetailsWindow->rendreInvisible();
    _programmesDeFideliteClientsWindow->rendreInvisible();
    _detailsGroupeDeClientsWindow->rendreInvisible();
    _groupesDeClientsWindow->rendreInvisible();
    _groupesDunClientWindow->rendreInvisible();
    _clientsDetailWindow->rendreInvisible();
    _charges_financieresWindow->rendreInvisible();
    _charges_financieres_detailsWindow->rendreInvisible();
    _achatsDetailWindow->rendreInvisible();
    _detailWindow->rendreInvisible();
    _creerFournisseurWindow->rendreInvisible();
    _detailsDunProgrammeDeFideliteClientsWindow->rendreInvisible();
    _creerUnProgrammeDeFideliteClientsWindow->rendreInvisible();
    _creerGroupeDeClientsWindow->rendreInvisible();
    _creerCompteClientWindow->rendreInvisible();
    _modifierFournisseurWindow->rendreInvisible();
    _modifierCompteClientWindow->rendreInvisible();
    _achatsWindow->rendreInvisible();
    _stocksWindow->rendreInvisible();
    _marchandisesWindow->rendreInvisible();
    _comptabiliteWindow->rendreInvisible();
    _listerAlertesWindow->rendreInvisible();
    _paiementsWindow->rendreInvisible();
    _ventesWindow->rendreInvisible();
    _groupes_DUN_EMPLOYE_window->rendreInvisible();
    _CREER_GROUPE_DE_PAIE_hr_window->rendreInvisible();
    _CREER_groupe_demployes_window->rendreInvisible();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->rendreInvisible();
    _DETAILS_dun_groupe_de_PAIE_hr->rendreInvisible();
    _groupe_demployes_window->rendreInvisible();
    _GROUPES_DE_PAIE_Window->rendreInvisible();
    _fournisseursWindow->rendreInvisible();
    _clientsWindow->rendreInvisible();
    _payerAuCompteClientWindow->rendreInvisible();
    _modifierWindow->rendreInvisible();
    _entrerWindow->rendreInvisible();
    _adminWindow->rendreInvisible();
    _adminCreateWindow->rendreInvisible();
    _adminListerWindow->rendreInvisible();
    _adminDetailWindow->rendreInvisible();
    _adminModifierWindow->rendreInvisible();

    _mainWindow->rendreInvisible();
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr()
{
    _tableModel_periodes_dappartenance_groupes_de_paie_hr->resetFilter("src/yerith-erp-windows.cpp", 948);
    return *_tableModel_periodes_dappartenance_groupes_de_paie_hr;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_imprimantereseau_recus_petits()
{
    _tableModel_imprimantereseau_recus_petits->resetFilter("src/yerith-erp-windows.cpp", 905);
    return *_tableModel_imprimantereseau_recus_petits;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_types_doperations_financieres()
{
    _tableModel_types_doperations_financieres->resetFilter("src/yerith-erp-windows.cpp", 970);
    return *_tableModel_types_doperations_financieres;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_operations_comptables()
{
    _tableModel_operations_comptables->resetFilter("src/yerith-erp-windows.cpp", 912);
    return *_tableModel_operations_comptables;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_comptes_bancaires()
{
    _tableModel_comptes_bancaires->resetFilter("src/yerith-erp-windows.cpp", 926);
    return *_tableModel_comptes_bancaires;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_entreprise_info()
{
    _tableModel_entreprise_info->resetFilter("src/yerith-erp-windows.cpp", 933);
    return *_tableModel_entreprise_info;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_paiements()
{
    _tableModel_historique_paiements->resetFilter("src/yerith-erp-windows.cpp", 940);
    return *_tableModel_historique_paiements;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_users()
{
    _tableModel_users->resetFilter("src/yerith-erp-windows.cpp", 947);
    return *_tableModel_users;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_titres()
{
    _tableModel_titres->resetFilter("src/yerith-erp-windows.cpp", 979);
    return *_tableModel_titres;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_localisations()
{
    _tableModel_localisations->resetFilter("src/yerith-erp-windows.cpp", 986);
    return *_tableModel_localisations;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_departements_produits()
{
    _tableModel_departements_produits->resetFilter("src/yerith-erp-windows.cpp", 992);
    return *_tableModel_departements_produits;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_categories()
{
    _tableModel_categories->resetFilter("src/yerith-erp-windows.cpp", 973);
    return *_tableModel_categories;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_lignes_budgetaires()
{
	_tableModel_ligne_budgetaire->resetFilter("src/yerith-erp-windows.cpp", 979);
    return *_tableModel_ligne_budgetaire;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_alertes()
{
    _tableModel_alertes->resetFilter("src/yerith-erp-windows.cpp", 985);
    return *_tableModel_alertes;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_remises()
{
    _tableModel_remises->resetFilter("src/yerith-erp-windows.cpp", 991);
    return *_tableModel_remises;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_conditions_alertes()
{
    _tableModel_conditions_alertes->resetFilter("src/yerith-erp-windows.cpp", 998);
    return *_tableModel_conditions_alertes;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_courriers_alertes()
{
    _tableModel_courriers_alertes->resetFilter("src/yerith-erp-windows.cpp", 1004);
    return *_tableModel_courriers_alertes;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_programmes_de_fidelite_clients()
{
    _tableModel_programmes_de_fidelite_clients->resetFilter("src/yerith-erp-windows.cpp", 1010);
    return *_tableModel_programmes_de_fidelite_clients;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_groupes_demployes_hr()
{
	_tableModel_groupes_demployes_hr->resetFilter("src/yerith-erp-windows.cpp", 1016);
    return *_tableModel_groupes_demployes_hr;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_groupes_de_paie_hr()
{
	_tableModel_groupes_de_paie_hr->resetFilter("src/yerith-erp-windows.cpp", 1022);
    return *_tableModel_groupes_de_paie_hr;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_clients()
{
    _tableModel_clients->resetFilter("src/yerith-erp-windows.cpp", 1028);
    return *_tableModel_clients;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_groupes_de_clients()
{
    _tableModel_groupes_de_clients->resetFilter("src/yerith-erp-windows.cpp", 1034);
    return *_tableModel_groupes_de_clients;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_fournisseurs()
{
    _tableModel_fournisseurs->resetFilter("src/yerith-erp-windows.cpp", 1040);
    return *_tableModel_fournisseurs;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_charges_financieres()
{
    _tableModel_charges_financieres->resetFilter("src/yerith-erp-windows.cpp", 1047);
    return *_tableModel_charges_financieres;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_achats()
{
    _tableModel_achats->resetFilter("src/yerith-erp-windows.cpp", 1054);
    return *_tableModel_achats;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_stocks()
{
    _tableModel_stocks->resetFilter("src/yerith-erp-windows.cpp", 1061);
    return *_tableModel_stocks;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_services_completes()
{
    _tableModel_services_completes->resetFilter("src/yerith-erp-windows.cpp", 1068);
    return *_tableModel_services_completes;
}


YerithSqlTableModel &YerithERPWindows::getSqlTableModel_marchandises()
{
    _tableModel_marchandises->resetFilter("src/yerith-erp-windows.cpp", 1075);
    return *_tableModel_marchandises;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_stocks_vendu()
{
    _tableModel_stocks_vendu->resetFilter("src/yerith-erp-windows.cpp", 1081);
    return *_tableModel_stocks_vendu;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_stocks_sorties()
{
    _tableModel_stocks_sorties->resetFilter("src/yerith-erp-windows.cpp", 1087);
    return *_tableModel_stocks_sorties;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_configurations()
{
    _tableModel_configurations->resetFilter("src/yerith-erp-windows.cpp", 1093);
    return *_tableModel_configurations;
}

YerithSqlTableModel &YerithERPWindows::getSqlTableModel_init_configurations()
{
    _tableModel_init_configurations->resetFilter("src/yerith-erp-windows.cpp", 1099);
    return *_tableModel_init_configurations;
}

void YerithERPWindows::setCurrentPosition(int xPos, int yPos)
{
    if (_currentPosition)
    {
        _currentPosition->setX(xPos);
        _currentPosition->setY(yPos);
    }
    else
    {
        _logger->log("YerithWindows::setCurrentPosition(int, int)",
                     "_currentPosition is NULL!");
    }
}

bool YerithERPWindows::move(QMainWindow &aWindow)
{
    if (0 != _currentPosition)
    {
        //qDebug() << "pos: " << *_currentPosition;
        aWindow.move(*_currentPosition);
        return true;
    }

    //qDebug() << "no move: ";
    return false;
}


/**
 * !!! DO NOT MODIFY AT ALL !!!
 *
 *  !!! THIS IS A VERY IMPORTANT PIECE OF CODE !!!
 */
int YerithERPWindows::NEXT_TABLE_ID_MYSQL_FOR_STOCKS()
{
    int max_id = 0;

    QString MAX_ID_QUERY_FOR_STOCKS(QString("SELECT IFNULL(MAX(id), 0)"
                                            " FROM"
                                            " ("
                                            " SELECT id FROM %1"
                                            " UNION ALL"
                                            " SELECT %2 as id FROM %3"
                                            " UNION ALL"
                                            " SELECT %4 as id FROM %5"
                                            " ) a").
                                    arg(YerithDatabase::STOCKS,
                                        YerithDatabaseTableColumn::STOCKS_ID,
                                        YerithDatabase::STOCKS_VENDU,
                                        YerithDatabaseTableColumn::STOCKS_ID,
                                        YerithDatabase::SERVICES_COMPLETES));

//      QDEBUG_STRING_OUTPUT_2("MAX_ID_QUERY_FOR_STOCKS", MAX_ID_QUERY_FOR_STOCKS);

    QSqlQuery aQuery;

    int querySize = YerithUtils::execQuery(aQuery, MAX_ID_QUERY_FOR_STOCKS);

    if (querySize > 0 && aQuery.next())
    {
        max_id = aQuery.value(0).toInt();

        max_id += 1;
    }

    return max_id;
}


int YerithERPWindows::getNextIdFromTable(const QString &tableName)
{
    //qDebug() << "[YerithUtils::getLastIdFromTable()] : next id from table '" << tableName << "'";
    QString
    strQuery(QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 0, 1").arg
             (YerithDatabaseTableColumn::ID, tableName,
              YerithDatabaseTableColumn::ID));

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    if (query.last())
    {
        int lastId = query.value(0).toInt();
        ++lastId;
//        qDebug() << "\t next id 1: " << lastId;
        return lastId;
    }

    return 0;
}
