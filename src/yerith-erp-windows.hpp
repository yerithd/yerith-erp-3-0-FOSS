/*
 * yerith-erp-windows.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_WINDOWS_HPP_
#define SRC_YERITH_WINDOWS_HPP_

#include <QtCore/QObject>

#include "src/windows/yerith-erp-main-window.hpp"


#include "src/windows/yerith-erp-recherche-conditionnelle-window.hpp"
#include "src/windows/yerith-erp-paiements-window.hpp"
#include "src/windows/yerith-erp-tableaux-de-bord-window.hpp"
#include "src/windows/yerith-erp-comptabilite-window.hpp"
#include "src/windows/yerith-erp-alertes-window.hpp"
#include "src/windows/yerith-erp-ventes-window.hpp"


//** STOCKS
#include "src/windows/stocks/yerith-erp-mouvements_de_stocks-window.hpp"
#include "src/windows/yerith-erp-entrer-window.hpp"
#include "src/windows/stocks/yerith-erp-historique-du-stock-window.hpp"
#include "src/windows/stocks/yerith-erp-marchandises-window.hpp"
#include "src/windows/stocks/yerith-erp-modifier-window.hpp"
#include "src/windows/stocks/yerith-erp-stock-detail-window.hpp"
#include "src/windows/stocks/yerith-erp-sortir-liststocks-window.hpp"
#include "src/windows/stocks/yerith-erp-stocks-window.hpp"
#include "src/windows/stocks/yerith-erp-sortir-window.hpp"


//** DIALOGS
#include "src/dialogs/yerith-erp-changer-utilisateur-dialog.hpp"
#include "src/dialogs/yerith-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp"
#include "src/dialogs/yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp"
#include "src/dialogs/yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp"
#include "src/dialogs/yerith-erp-pointdevente-methode-paiement-dialog.hpp"


//** POS
#include "src/windows/pos/yerith-erp-pointdevente-liststocks-window.hpp"
#include "src/windows/pos/yerith-erp-pointdevente-window.hpp"

//** CHARGES FINANCIÈRES
#include "src/windows/yerith-erp-charges-financieres-window.hpp"
#include "src/windows/yerith-erp-charges-financieres-detail-window.hpp"


//** PURCHASE
#include "src/windows/purchase/yerith-erp-achats-window.hpp"
#include "src/windows/purchase/yerith-erp-achats-detail-window.hpp"


//** SUPPLIER
#include "src/windows/supplier/yerith-erp-tableau-des-transactions-du-fournisseur-window.hpp"
#include "src/windows/supplier/yerith-erp-fournisseur-details-window.hpp"
#include "src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.hpp"

#include "src/windows/supplier/yerith-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.hpp"

#include "src/windows/supplier/yerith-erp-creer-GROUPE-DE-PAIE-hr-window.hpp"
#include "src/windows/supplier/yerith-erp-creer-groupe-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yerith-erp-creer-groupe-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yerith-erp-GROUPES-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yerith-erp-GROUPES-DE-PAIE-hr-window.hpp"
#include "src/windows/supplier/yerith-erp-fournisseurs-window.hpp"
#include "src/windows/supplier/yerith-erp-creer-fournisseur-window.hpp"
#include "src/windows/supplier/yerith-erp-modifier-fournisseur-window.hpp"


//** CRM
#include "src/windows/crm/creer/yerith-erp-creer-un-programme-de-fidelite-clients-window.hpp"
#include "src/windows/crm/creer/yerith-erp-creer-groupe-de-clients-window.hpp"
#include "src/windows/crm/creer/yerith-erp-creer-compte-client-window.hpp"

#include "src/windows/crm/details/yerith-erp-details-dun-groupe-de-clients-window.hpp"
#include "src/windows/crm/details/yerith-erp-details-dun-programme-de-fidelite-clients-window.hpp"
#include "src/windows/crm/details/yerith-erp-clients-detail-window.hpp"

#include "src/windows/crm/modifier/yerith-erp-modifier-compte-client-window.hpp"

#include "src/windows/crm/yerith-erp-programmes-de-fidelite-clients-window.hpp"
#include "src/windows/crm/yerith-erp-groupes-de-clients-window.hpp"
#include "src/windows/crm/yerith-erp-groupes-dun-client-window.hpp"
#include "src/windows/crm/yerith-erp-clients-window.hpp"
#include "src/windows/crm/yerith-erp-tableau-des-transactions-du-client-window.hpp"
#include "src/windows/crm/yerith-erp-payer-compte-client-window.hpp"




//** ADMINISTRATION
#include "src/admin/yerith-erp-admin-window.hpp"
#include "src/admin/creer/yerith-erp-admin-create-window.hpp"
#include "src/admin/lister/yerith-erp-admin-lister-window.hpp"
#include "src/admin/detail/yerith-erp-admin-detail-window.hpp"
#include "src/admin/modifier/yerith-erp-admin-modifier-window.hpp"


//** UTILS
#include "src/utils/yerith-erp-database.hpp"
#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-info-entreprise.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <QtSql/QSqlDatabase>


class YerithSqlTableModel;
class YerithMainWindow;
class YerithRechercheConditionnelleWindow;
class YerithPaiementsWindow;
class YerithTableauDesTransactionsDuFournisseurWindow;
class YerithTableauDesTransactionsDuClientWindow;
class YerithHistoriqueDuStockWindow;
class YerithPOSChangerUtilisateurDialog;
class YerithCreerFournisseurWindow;
class YerithDetailsDunProgrammeDeFideliteClientsWindow;
class YerithCreerUnProgrammeDeFideliteClientsWindow;
class YerithCreerGroupeDeClientsWindow;
class YerithCreerCompteClientWindow;
class YerithModifierFournisseurWindow;
class YerithModifierCompteClientWindow;
class YerithAchatsWindow;
class YerithStocksWindow;
class YerithMarchandisesWindow;
class YerithComptabiliteWindow;
class YerithAlertesWindow;
class YerithVentesWindow;
class YerithModifierWindow;
class YerithSortirWindow;
class YerithTableauxDeBordWindow;
class YerithMouvementsDeStocksWindow;
class YerithDetailsDunGroupeDeClientsWindow;
class YerithPointDeVenteWindow;
class YerithPOSVenteMethodePaiementCarteCreditDialog;
class YerithPOSVenteMethodePaiementComptantEntreeDialog;
class YerithPOSVenteMethodePaiementDialog;
class YerithFournisseurDetailsWindow;
class YerithGroupesDunClientWindow;
class YerithClientsDetailWindow;
class YerithChargesFinancieresWindow;
class YerithChargesFinancieresDetailsWindow;
class YerithAchatsDetailWindow;
class YerithStockDetailWindow;
class YerithEntrerWindow;
class YerithAdminWindow;
class YerithAdminCreateWindow;
class YerithAdminListerWindow;
class YerithAdminDetailWindow;
class YerithAdminModifierWindow;
class YerithPOSUser;
class YerithInfoEntreprise;
class YerithLogger;
class YerithDatabase;
class YerithPointDeVenteListStocksWindow;
class YerithPayerCompteClientWindow;
class YerithGROUPES_DUN_EMPLOYE_Window;
class YerithCreerUnGroupeDePaieWindow;
class YerithCreerGroupeDemployesWindow;
class YerithDetailsDunGroupeDePaieWindow;
class YerithERPGroupesDemployesHRWindow;
class YerithERPGROUPES_DE_PAIE_Window;
class YerithERPFournisseursWindow;
class YerithERPClientsWindow;
class YerithSortirListStocksWindow;
class YerithUtils;

#ifdef YERITH_SERVER
class YERITH_RUNTIME_VERIFIER;
#endif

#ifdef YERITH_CLIENT
class YerithDBusClient;
#endif

class QString;
class QDesktopWidget;

class YerithERPWindows : public QObject
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS YerithERPWindows(QDesktopWidget *desktopWidget);

    virtual ~YerithERPWindows();

    void createAllYerithPosUserWindows();

    void reinitialiseSqlTableModels();

    void setupSqlTableModelFromName(const QString       &aSqlTableName,
                                    YerithSqlTableModel **aYerithSqlTableModel);

    void setupConfiguration();


    YerithSqlTableModel &getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();

    YerithSqlTableModel &getSqlTableModel_imprimantereseau_recus_petits();

    YerithSqlTableModel &getSqlTableModel_types_doperations_financieres();

    YerithSqlTableModel &getSqlTableModel_operations_comptables();

    YerithSqlTableModel &getSqlTableModel_comptes_bancaires();

    YerithSqlTableModel &getSqlTableModel_entreprise_info();

    YerithSqlTableModel &getSqlTableModel_paiements();

    YerithSqlTableModel &getSqlTableModel_users();

    YerithSqlTableModel &getSqlTableModel_titres();

    YerithSqlTableModel &getSqlTableModel_localisations();

    YerithSqlTableModel &getSqlTableModel_departements_produits();

    YerithSqlTableModel &getSqlTableModel_categories();

    YerithSqlTableModel &getSqlTableModel_lignes_budgetaires();

    YerithSqlTableModel &getSqlTableModel_alertes();

    YerithSqlTableModel &getSqlTableModel_remises();

    YerithSqlTableModel &getSqlTableModel_conditions_alertes();

    YerithSqlTableModel &getSqlTableModel_courriers_alertes();

    YerithSqlTableModel &getSqlTableModel_programmes_de_fidelite_clients();

    YerithSqlTableModel &getSqlTableModel_clients();

    YerithSqlTableModel &getSqlTableModel_groupes_demployes_hr();

    YerithSqlTableModel &getSqlTableModel_groupes_de_paie_hr();

    YerithSqlTableModel &getSqlTableModel_groupes_de_clients();

    YerithSqlTableModel &getSqlTableModel_fournisseurs();

    YerithSqlTableModel &getSqlTableModel_charges_financieres();

    YerithSqlTableModel &getSqlTableModel_achats();

    YerithSqlTableModel &getSqlTableModel_stocks();

    YerithSqlTableModel &getSqlTableModel_services_completes();

    YerithSqlTableModel &getSqlTableModel_marchandises();

    YerithSqlTableModel &getSqlTableModel_stocks_vendu();

    YerithSqlTableModel &getSqlTableModel_stocks_sorties();

    YerithSqlTableModel &getSqlTableModel_configurations();

    YerithSqlTableModel &getSqlTableModel_init_configurations();

#ifdef YERITH_SERVER
    inline YERITH_RUNTIME_VERIFIER *dbusServer()
    {
        return _dbusServer;
    }
#endif

#ifdef YERITH_CLIENT
    inline YerithDBusClient *dbusClient()
    {
        return _dbusClient;
    }
#endif

    inline static const QString &get_last_lister_selected_row_db_ID()
    {
        return YerithWindowsCommons::get_last_lister_selected_row_db_ID();
    }

    inline void setDesktopWidget(QDesktopWidget *desktopWidget)
    {
        _desktopWidget = desktopWidget;
    }

    inline QDesktopWidget &desktopWidget()
    {
        return *_desktopWidget;
    }

    static inline int getNextIdSqlTableModel_periodes_dappartenance_groupes_de_paie_hr()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR);
    }

    static inline int getNextIdSqlTableModel_users()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::USERS);
    }

    static inline int getNextIdSqlTableModel_titres()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::TITRES);
    }

    static inline int getNextIdSqlTableModel_localisations()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    LOCALISATIONS);
    }

    static inline int getNextIdSqlTableModel_departements_produits()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    DEPARTEMENTS_PRODUITS);
    }

    static inline int getNextIdSqlTableModel_categories()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::CATEGORIES);
    }

    static inline int getNextIdSqlTableModel_LIGNES_BUDGETAIRES()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::LIGNES_BUDGETAIRES);
    }

    static inline int getNextIdSqlTableModel_alertes()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::ALERTES);
    }

    static inline int getNextIdSqlTableModel_conditions_alertes()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    CONDITIONS_ALERTES);
    }

    static inline int getNextIdSqlTableModel_courriers_alertes()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    COURRIERS_ALERTES);
    }

    static inline int getNextIdSqlTableModel_programme_de_fidelite_clients()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    PROGRAMMES_DE_FIDELITE_CLIENTS);
    }

    static inline int getNextIdSqlTableModel_groupes_demployes_hr()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::GROUPES_DEMPLOYES_hr);
    }

    static inline int getNextIdSqlTableModel_groupes_de_paie_hr()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::GROUPES_DE_PAIE_hr);
    }

    static inline int getNextIdSqlTableModel_clients()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::CLIENTS);
    }

    static inline int getNextIdSqlTableModel_groupes_de_clients()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    GROUPES_DE_CLIENTS);
    }

    static inline int getNextIdSqlTableModel_fournisseurs()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    FOURNISSEURS);
    }

    static inline int getNextIdSqlTableModel_comptes_bancaires()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    COMPTES_BANCAIRES);
    }


    static inline int getNextIdSqlTableModel_imprimantereseau_recus_petits()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    IMPRIMANTERESEAU_RECUS_PETITS);
    }


    static inline int getNextIdSqlTableModel_types_doperations_financieres()
    {
        return YerithERPWindows::getNextIdFromTable
                (YerithDatabase::TYPE_DOPERATIONS_FINANCIERES);
    }


    static inline int getNextIdSqlTableModel_operations_comptables()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    OPERATIONS_COMPTABLES);
    }


    static inline int getNextIdSqlTableModel_charges_financieres()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    CHARGES_FINANCIERES);
    }

    static inline int getNextIdSqlTableModel_achats()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::ACHATS);
    }

    static inline int getNextIdSqlTableModel_marchandises()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    MARCHANDISES);
    }

    static inline int getNextIdSqlTableModel_paiements()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::PAIEMENTS);
    }

    static inline int getNextIdSqlTableModel_stocks()
    {
        return YerithERPWindows::NEXT_TABLE_ID_MYSQL_FOR_STOCKS();
    }

    static inline int getNextIdSqlTableModel_services_completes()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    SERVICES_COMPLETES);
    }

    static inline int getNextIdSqlTableModel_stocks_vendu()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    STOCKS_VENDU);
    }

    static inline int getNextIdSqlTableModel_stocks_sorties()
    {
        return YerithERPWindows::getNextIdFromTable(YerithDatabase::
                                                    STOCKS_SORTIES);
    }

    inline void setCurrentPosition(QPoint *currentPosition)
    {
        _currentPosition = currentPosition;
    }

    void setCurrentPosition(int xPos, int yPos);

    inline QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

    inline YerithPOSUser *getUser()
    {
        return _user;
    }

    inline YerithDatabase &getDatabase()
    {
        return *_database;
    }

    inline void setDatabase(YerithDatabase *database)
    {
        _database = database;
    }

    inline YerithInfoEntreprise &getInfoEntreprise()
    {
        return *_infoEntreprise;
    }

    static inline YerithSqlTableModel *getSqlTableModelFromName(const QString &
                                                                aSqlTableName)
    {
        return _sqltablenameTOsqltablemodel.value(aSqlTableName);
    }

    bool move(QMainWindow &aWindow);

    bool updateUserData(int userRecordRow);

    void setUser(YerithPOSUser *user);

    void definirAdministrateur();

    void definirMagasinier();

    void definirCaissier();

    void definirManager();

    void definirVendeur();

    void definirGestionaireDesStocks();

    void definirPasDeRole();

    void hideAllWindows();

    static QTranslator *_FRENCH_TRANSLATOR;

    static QTranslator *_ENGLISH_TRANSLATOR;

    static QApplication *_YERITH_ERP_PGI_APPLICATION;

    static bool CURRENTLY_CHANGING_USER;


    YerithMainWindow *_mainWindow;

    YerithRechercheConditionnelleWindow *_recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW;
    YerithPOSChangerUtilisateurDialog *_changerUtilisateurDialog;
    YerithTableauDesTransactionsDuFournisseurWindow
    *_transactionsDunFournisseurWindow;
    YerithTableauDesTransactionsDuClientWindow *_transactionsDunClientWindow;
    YerithHistoriqueDuStockWindow *_historiqueDuStockWindow;
    YerithCreerFournisseurWindow *_creerFournisseurWindow;
    YerithDetailsDunProgrammeDeFideliteClientsWindow
    *_detailsDunProgrammeDeFideliteClientsWindow;
    YerithCreerUnProgrammeDeFideliteClientsWindow
    *_creerUnProgrammeDeFideliteClientsWindow;
    YerithCreerGroupeDeClientsWindow *_creerGroupeDeClientsWindow;
    YerithCreerCompteClientWindow *_creerCompteClientWindow;
    YerithModifierFournisseurWindow *_modifierFournisseurWindow;
    YerithModifierCompteClientWindow *_modifierCompteClientWindow;
    YerithAchatsWindow *_achatsWindow;
    YerithStocksWindow *_stocksWindow;
    YerithMarchandisesWindow *_marchandisesWindow;
    YerithComptabiliteWindow *_comptabiliteWindow;
    YerithAlertesWindow *_listerAlertesWindow;
    YerithPaiementsWindow *_paiementsWindow;
    YerithVentesWindow *_ventesWindow;
    YerithModifierWindow *_modifierWindow;
    YerithSortirWindow *_sortirWindow;
    YerithTableauxDeBordWindow *_statistiquesWindow;
    YerithMouvementsDeStocksWindow *_mouvementsDeStocksWindow;
    YerithPointDeVenteWindow *_pdVenteWindow;
    YerithFournisseurDetailsWindow *_fournisseurDetailsWindow;
    YerithERPProgrammesDeFideliteClientsWindow
    *_programmesDeFideliteClientsWindow;
    YerithDetailsDunGroupeDeClientsWindow *_detailsGroupeDeClientsWindow;
    YerithGroupesDeClientsWindow *_groupesDeClientsWindow;
    YerithGroupesDunClientWindow *_groupesDunClientWindow;
    YerithClientsDetailWindow *_clientsDetailWindow;
    YerithChargesFinancieresWindow *_charges_financieresWindow;
    YerithChargesFinancieresDetailsWindow *_charges_financieres_detailsWindow;
    YerithAchatsDetailWindow *_achatsDetailWindow;
    YerithStockDetailWindow *_detailWindow;
    YerithEntrerWindow *_entrerWindow;
    YerithIMPRESSION_DE_DOCUMENT_Dialog *_impressionDeDocumentDialog;
    YerithPointDeVenteListStocksWindow *_pdVenteListStocksWindow;
    YerithPOSVenteMethodePaiementCarteCreditDialog
    *_pdVenteMethodePaiementCarteCreditDialog;
    YerithPOSVenteMethodePaiementComptantEntreeDialog
    *_pdVenteMethodePaiementComptantEntreeDialog;
    YerithPOSVenteMethodePaiementDialog *_pdVenteMethodePaiementDialog;
    YerithGROUPES_DUN_EMPLOYE_Window *_groupes_DUN_EMPLOYE_window;
    YerithCreerUnGroupeDePaieWindow *_CREER_GROUPE_DE_PAIE_hr_window;
    YerithCreerGroupeDemployesWindow *_CREER_groupe_demployes_window;
    YerithDetailsDunGroupeDemployesWindow *_DETAILS_DUN_GROUPE_DEMPLOYES_hr;
    YerithDetailsDunGroupeDePaieWindow *_DETAILS_dun_groupe_de_PAIE_hr;
    YerithERPGroupesDemployesHRWindow *_groupe_demployes_window;
    YerithERPGROUPES_DE_PAIE_Window *_GROUPES_DE_PAIE_Window;
    YerithERPFournisseursWindow *_fournisseursWindow;
    YerithERPClientsWindow *_clientsWindow;
    YerithPayerCompteClientWindow *_payerAuCompteClientWindow;
    YerithSortirListStocksWindow *_sortirListStocksWindow;
    YerithAdminWindow *_adminWindow;
    YerithAdminCreateWindow *_adminCreateWindow;
    YerithAdminListerWindow *_adminListerWindow;
    YerithAdminDetailWindow *_adminDetailWindow;
    YerithAdminModifierWindow *_adminModifierWindow;

private:

    static int NEXT_TABLE_ID_MYSQL_FOR_STOCKS();

    static int getNextIdFromTable(const QString &tableName);

    YerithDatabase *_database;

    static QMap<QString, YerithSqlTableModel *> _sqltablenameTOsqltablemodel;

    YerithSqlTableModel *_tableModel_periodes_dappartenance_groupes_de_paie_hr;
    YerithSqlTableModel *_tableModel_imprimantereseau_recus_petits;
    YerithSqlTableModel *_tableModel_types_doperations_financieres;
    YerithSqlTableModel *_tableModel_operations_comptables;
    YerithSqlTableModel *_tableModel_comptes_bancaires;
    YerithSqlTableModel *_tableModel_entreprise_info;
    YerithSqlTableModel *_tableModel_historique_paiements;
    YerithSqlTableModel *_tableModel_users;
    YerithSqlTableModel *_tableModel_titres;
    YerithSqlTableModel *_tableModel_localisations;
    YerithSqlTableModel *_tableModel_departements_produits;
    YerithSqlTableModel *_tableModel_categories;
    YerithSqlTableModel *_tableModel_ligne_budgetaire;
    YerithSqlTableModel *_tableModel_programmes_de_fidelite_clients;
    YerithSqlTableModel *_tableModel_groupes_demployes_hr;
    YerithSqlTableModel *_tableModel_groupes_de_paie_hr;
    YerithSqlTableModel *_tableModel_clients;
    YerithSqlTableModel *_tableModel_groupes_de_clients;
    YerithSqlTableModel *_tableModel_fournisseurs;
    YerithSqlTableModel *_tableModel_alertes;
    YerithSqlTableModel *_tableModel_remises;
    YerithSqlTableModel *_tableModel_conditions_alertes;
    YerithSqlTableModel *_tableModel_courriers_alertes;
    YerithSqlTableModel *_tableModel_charges_financieres;
    YerithSqlTableModel *_tableModel_achats;
    YerithSqlTableModel *_tableModel_stocks;
    YerithSqlTableModel *_tableModel_services_completes;
    YerithSqlTableModel *_tableModel_marchandises;
    YerithSqlTableModel *_tableModel_stocks_vendu;
    YerithSqlTableModel *_tableModel_stocks_sorties;
    YerithSqlTableModel *_tableModel_configurations;
    YerithSqlTableModel *_tableModel_init_configurations;

#ifdef YERITH_SERVER
    YERITH_RUNTIME_VERIFIER *_dbusServer;
#endif

#ifdef YERITH_CLIENT
    YerithDBusClient *_dbusClient;
#endif


    YerithLogger *_logger;

    YerithInfoEntreprise *_infoEntreprise;
    YerithPOSUser *_user;

    QPoint *_currentPosition;
    QDesktopWidget *_desktopWidget;
};

#endif /* SRC_YERITH_WINDOWS_HPP_ */
