/**
 * yerith-erp-admin-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-admin-window.hpp"


#include "src/imports/yerith-erp-marchandise-import.hpp"

#include "src/imports/yerith-erp-stock-import.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/process/yerith-erp-process-info.hpp"

#include "src/utils/yerith-erp-database.hpp"

#include "src/dbus/YERITH_RUNTIME_VERIFIER.hpp"

#include "src/widgets/yerith-erp-progress-bar.hpp"

#include "src/users/yerith-erp-users.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QTemporaryFile>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QStyleFactory>


const int YerithAdminWindow::MAX_IMPORT_CSV_HEADER_SIZE(16);

const QString YerithAdminWindow::DATABASE_YERITH_ERP_3_0_KEYWORD("* database * ");


QStringList
YerithAdminWindow::comboBox_impression_sur_CHOICE_list;

QStringList
YerithAdminWindow::comboBox_format_de_facture_CHOICE_list;


#ifdef YERITH_FRANCAIS_LANGUAGE

const
QString YerithAdminWindow::EFFACER(QObject::tr("effacer"));
const QString YerithAdminWindow::EXPORTER(QObject::tr("exporter"));
const QString YerithAdminWindow::IMPORTER(QObject::tr("importer"));

const QString YerithAdminWindow::DB(QObject::tr("base de données"));
const QString YerithAdminWindow::TABLEAU(QObject::tr("tableau"));

#elif YERITH_ENGLISH_LANGUAGE

const
QString YerithAdminWindow::EFFACER(QObject::tr("erase"));
const QString YerithAdminWindow::EXPORTER(QObject::tr("export"));
const QString YerithAdminWindow::IMPORTER(QObject::tr("import"));

const QString YerithAdminWindow::DB(QObject::tr("database"));
const QString YerithAdminWindow::TABLEAU(QObject::tr("database table"));

#endif



#ifdef YERITH_FRANCAIS_LANGUAGE
const QString YerithAdminWindow::CREER(QObject::
                                       trUtf8(ACTION_ADMIN_OPERATIONS_CREATE_FR));
const QString YerithAdminWindow::LISTER(QObject::tr(ACTION_ADMIN_OPERATIONS_LIST_FR));
const QString YerithAdminWindow::MODIFIER(QObject::
                                          trUtf8(ACTION_ADMIN_OPERATIONS_MODIFY_FR));
const QString YerithAdminWindow::SUPPRIMER(QObject::
                                           trUtf8(ACTION_ADMIN_OPERATIONS_DELETE_FR));

const QString YerithAdminWindow::COMPTE_UTILISATEUR(QObject::tr
                                                    (SUBJECT_ADMIN_OPERATIONS_USER_ACCOUNT_FR));
const QString YerithAdminWindow::LOCALISATION(QObject::
                                              trUtf8(SUBJECT_ADMIN_OPERATIONS_SITE_FR));
const QString YerithAdminWindow::DEPARTEMENTS_DE_PRODUITS(QObject::tr
                                                          (SUBJECT_ADMIN_OPERATIONS_PRODUCT_DEPARTMENT_FR));
const QString YerithAdminWindow::CATEGORIE(QObject::
                                           trUtf8(SUBJECT_ADMIN_OPERATIONS_CATEGORY_FR));
const QString YerithAdminWindow::LIGNE_BUDGETAIRE(QObject::
                                           trUtf8(SUBJECT_ADMIN_OPERATIONS_BUDGET_LINE_FR));
const QString YerithAdminWindow::ALERTE(QObject::
                                        trUtf8(SUBJECT_ADMIN_OPERATIONS_ALERT_FR));
const QString YerithAdminWindow::BON_DE_COMMANDE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_FINANCIAL_EXPENSE_FR));
const QString YerithAdminWindow::COMPTE_BANCAIRE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_BANK_ACCOUNT_FR));
const QString YerithAdminWindow::REMISE(QObject::
                                        trUtf8(SUBJECT_ADMIN_OPERATIONS_DISCOUNT_FR));

#elif YERITH_ENGLISH_LANGUAGE
const QString YerithAdminWindow::CREER(QObject::tr(ACTION_ADMIN_OPERATIONS_CREATE_EN));
const QString YerithAdminWindow::LISTER(QObject::tr(ACTION_ADMIN_OPERATIONS_LIST_EN));
const QString YerithAdminWindow::MODIFIER(QObject::tr(ACTION_ADMIN_OPERATIONS_MODIFY_EN));
const QString YerithAdminWindow::SUPPRIMER(QObject::
                                           tr(ACTION_ADMIN_OPERATIONS_DELETE_EN));

const QString YerithAdminWindow::COMPTE_UTILISATEUR(QObject::tr
                                                    (SUBJECT_ADMIN_OPERATIONS_USER_ACCOUNT_EN));
const QString YerithAdminWindow::LOCALISATION(QObject::
                                              tr(SUBJECT_ADMIN_OPERATIONS_SITE_EN));
const QString YerithAdminWindow::DEPARTEMENTS_DE_PRODUITS(QObject::tr
                                                          (SUBJECT_ADMIN_OPERATIONS_PRODUCT_DEPARTMENT_EN));
const QString YerithAdminWindow::CATEGORIE(QObject::
                                           tr(SUBJECT_ADMIN_OPERATIONS_CATEGORY_EN));
const QString YerithAdminWindow::LIGNE_BUDGETAIRE(QObject::
                                           tr(SUBJECT_ADMIN_OPERATIONS_BUDGET_LINE_EN));
const QString YerithAdminWindow::ALERTE(QObject::tr(SUBJECT_ADMIN_OPERATIONS_ALERT_EN));
const QString YerithAdminWindow::BON_DE_COMMANDE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_FINANCIAL_EXPENSE_EN));
const QString YerithAdminWindow::COMPTE_BANCAIRE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_BANK_ACCOUNT_EN));
const QString YerithAdminWindow::REMISE(QObject::
                                        tr(SUBJECT_ADMIN_OPERATIONS_DISCOUNT_EN));
#endif

YerithAdminWindow::YerithAdminWindow()
:YerithQTableWidgetAdminWindowsCommons(QObject::tr("administration ~ accueil")),
 _logger(new YerithLogger("YerithAdminWindow")),
 _importer_parlant(false),
 _curAdminAction(ACTION_CREER),
 _curAdminSujetAction(SUJET_ACTION_COMPTE_UTILISATEUR),
 _curAdminMaintainAction(ACTION_EXPORTER),
 _curAdminSujetsMaintainAction(SUJET_ACTION_DB),
 _actionsToConst(0),
 _sujetActionsToConst(0)
{
    setupUi(this);

    mySetupUi(this);


    _a_CURRENT_table_widget = tableWidget_LOGGING_runtime_VERIFICATION;

    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_LOGGING_runtime_VERIFICATION->font());


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau;


    comboBox_impression_sur_CHOICE_list
            << YerithUtils::IMPRIMANTE_PDF
            << YerithUtils::IMPRIMANTE_EPSON_TM_T20ii
            << YerithUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU;


    comboBox_format_de_facture_CHOICE_list
            << YerithERPConfig::RECEIPT_FORMAT_PETIT
            << YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER
            << YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER;


    stop_TESTING_MAINTENANCE();


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
                        .arg(COLOUR_RGB_STRING_YERITH_DARK_GREEN_47_67_67,
                             COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    comboBox_localisation->setYerithEditable(true);

    populateComboBoxes();


    lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->checkField();

    lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE
		->setValidator(&YerithUtils::STRING_FOR_YERITH_ERP_3_0_IP_ADDRESS_VALIDATOR);


    lineEdit_importer_separation_csv
		->setYerithEnabled(false);

    lineEdit_importer_separation_csv
		->setText(YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    lineEdit_localisation_adresse_ip->setYerithEnabled(false);

    lineEdit_this_localisation->setYerithEnabled(false);

    lineEdit_this_localisation
		->setText(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    _yerithAdminWindowTitleStart.append(windowTitle());


    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE
		->setYerithEnabled(false);

    lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0
		->setYerithEnabled(false);

    lineEdit_administration_maintenance_commandes_effacer_un_tableau
		->setYerithEnabled(false);

    lineEdit_administration_maintenance_commandes_exporter_un_tableau
		->setYerithEnabled(false);

    lineEdit_administration_maintenance_commandes_importer_un_tableau
		->setYerithEnabled(false);

    lineEdit_administration_maintenance_commandes_supprimer_un_tableau
		->setYerithEnabled(false);


    lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0
		->setText(QString("mysqldump --databases -u '%1' -p"));

    lineEdit_administration_maintenance_commandes_exporter_un_tableau
		->setText(QString("mysqldump db -u '%1' -p"));


    setupValidators();

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    _actionsToConst = new QMap<QString, AdminAction>;

    _sujetActionsToConst = new QMap<QString, AdminSujetAction>;

    _actionsToConst->insert(CREER, ACTION_CREER);
    _actionsToConst->insert(LISTER, ACTION_LISTER);
    _actionsToConst->insert(MODIFIER, ACTION_MODIFIER);
    _actionsToConst->insert(SUPPRIMER, ACTION_SUPPRIMER);
    _actionsToConst->insert(EXPORTER, ACTION_EXPORTER);
    _actionsToConst->insert(IMPORTER, ACTION_IMPORTER);
    _actionsToConst->insert(EFFACER, ACTION_EFFACER);

    _sujetActionsToConst->insert(ALERTE, SUJET_ACTION_ALERTE);

    _sujetActionsToConst->insert(BON_DE_COMMANDE,
                                 SUJET_ACTION_CHARGE_FINANCIERE);

    _sujetActionsToConst->insert(CATEGORIE, SUJET_ACTION_CATEGORIE);

    _sujetActionsToConst->insert(LIGNE_BUDGETAIRE, SUJET_ACTION_ligne_budgetaire);

    _sujetActionsToConst->insert(COMPTE_BANCAIRE,
                                 SUJET_ACTION_COMPTE_BANCAIRE);

    _sujetActionsToConst->insert(COMPTE_UTILISATEUR,
                                 SUJET_ACTION_COMPTE_UTILISATEUR);

    _sujetActionsToConst->insert(DEPARTEMENTS_DE_PRODUITS,
                                 SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

    _sujetActionsToConst->insert(LOCALISATION, SUJET_ACTION_LOCALISATION);

    _sujetActionsToConst->insert(REMISE, SUJET_ACTION_REMISE);

    _sujetActionsToConst->insert(DB, SUJET_ACTION_DB);

    _sujetActionsToConst->insert(TABLEAU, SUJET_ACTION_TABLEAU);

    comboBox_sujets_actions->addItem(ALERTE);
    comboBox_sujets_actions->addItem(BON_DE_COMMANDE);
    comboBox_sujets_actions->addItem(CATEGORIE);
    comboBox_sujets_actions->addItem(LIGNE_BUDGETAIRE);
    comboBox_sujets_actions->addItem(COMPTE_BANCAIRE);
    comboBox_sujets_actions->addItem(COMPTE_UTILISATEUR);
    comboBox_sujets_actions->addItem(DEPARTEMENTS_DE_PRODUITS);
    comboBox_sujets_actions->addItem(LOCALISATION);
    comboBox_sujets_actions->addItem(REMISE);

    YERITH_ERP_3_populate_all_tables();

    comboBox_strategie_vente_sortie
		->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

    comboBox_strategie_vente_sortie
		->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

    comboBox_strategie_vente_sortie
		->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

    comboBox_strategie_vente_sortie
		->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    comboBox_actions->addItem(CREER);
    comboBox_actions->addItem(LISTER);
    comboBox_actions->addItem(MODIFIER);
    comboBox_actions->addItem(SUPPRIMER);


    comboBox_operations_maintenance->addItem(EFFACER);
    comboBox_operations_maintenance->addItem(EXPORTER);
    comboBox_operations_maintenance->addItem(IMPORTER);
    comboBox_operations_maintenance->addItem(SUPPRIMER);


    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_operation_go->disable(this);


    pushButton_reinitialiser_le_logo_de_lentreprise->disable(this);

    pushButton_selectionner_logo_de_lentreprise->disable(this);

    pushButton_supprimer_logo_de_lentreprise->disable(this);


    pushButton_maintenance_valider
		->enable(this,
				 SLOT(EXECUTER_COMMANDE_MAINTENANCE()));

    pushButton_maintenance_reinitialiser
		->enable(this,
				 SLOT(reinitialiser_AFFICHAGE_COMMANDE_MAINTENANCE()));

    pushButton_entreprise_reinitialiser
		->enable(this,
				 SLOT(read_entreprise_info_database_table()));

    pushButton_entreprise_enregistrer
		->enable(this,
				 SLOT(enregistrer_entreprise_info_database_table()));


    pushButton_REFRESH_STATUS_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(REFRESH_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS()));

    pushButton_START_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(START_YRI_DB_RUNTIME_VERIF_DAEMON_PROCESS()));

    pushButton_STOP_DB_RUNTIME_VERIFICATOR
		->enable(this,
				 SLOT(STOP_YRI_DB_RUNTIME_VERIF_DAEMON_PROCESS()));


    pushButton_rafraichir_statut_alert_daemon
		->enable(this,
                 SLOT(set_colour_status_of_alert_daemon_process_buttons()));

    pushButton_demarrer_alert_daemon
		->enable(this,
                 SLOT(start_alert_daemon_process()));

    pushButton_stopper_alert_daemon
		->enable(this,
				 SLOT(stop_alert_daemon_process()));

    label_ON->setVisible(false);
    label_OFF->setVisible(true);

    label_ONLINE->setVisible(false);
    label_OFFLINE->setVisible(true);

    pushButton_choose_mariadb_client_full_path_dir->enable(this,
                                                           SLOT
                                                           (choose_path_mariadb_client_dir
                                                            ()));

    pushButton_choose_pdfReader->enable(this, SLOT(choose_path_pdfReader()));

    pushButton_choose_fichier_systeme_imprimante_thermique->enable(this,
                                                                   SLOT
                                                                   (choose_path_thermalPrinterDeviceFile
                                                                    ()));

    pushButton_choose_repertoire_systeme_latex->enable(this,
                                                       SLOT
                                                       (choose_path_latex_system_root_folder
                                                        ()));

    pushButton_choose_repertoire_des_sauvegardes->enable(this,
                                                         SLOT
                                                         (choose_repertoire_des_sauvegardes
                                                          ()));

    pushButton_choose_repertoire_fichiers_temporaires->enable(this,
                                                              SLOT
                                                              (choose_repertoire_fichiers_temporaires
                                                               ()));

    pushButton_yerith_erp_3_0_system_daemon_parameters_reinitialiser->enable
    (this,
     SLOT(reinitialiser_YERITH_ERP_3_0_SYSTEM_DAEMON_configuration()));

    pushButton_yerith_erp_3_0_system_daemon_parameters_enregistrer->enable
    (this, SLOT(enregistrer_YERITH_ERP_3_0_SYSTEM_DAEMON_configuration()));

    pushButton_parametres_reinitialiser->enable(this,
                                                SLOT
                                                (reinitialiser_app_parameters_configuration
                                                 ()));

    pushButton_parametres_enregistrer->enable(this,
                                              SLOT
                                              (enregistrer_app_parameters_configuration
                                               ()));

    pushButton_connecter_localisation->enable(this,
                                              SLOT(connecter_localisation_db
                                                   ()));

    pushButton_deconnecter_localisation->enable(this,
                                                SLOT(deconnecter_localisation_db()));


    connect(action_TRADUIRE_EN_ANGLAIS,
            SIGNAL(triggered()),
            this,
            SLOT(CHANGER_DE_LANGUE()));


    connect(ACTION_GET_runtime_verification_report_PDF,
            SIGNAL(triggered()),
            this,
            SLOT(GET_runtime_verification_report_PDF()));



    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
            SIGNAL(triggered()),
            (YerithQTableWidgetAdminWindowsCommons *) this,
            SLOT(incrementFontSize__OF_TABLE__WIDGET()));

    connect(actiondiminuer_la_police_du_tableau,
            SIGNAL(triggered()),
            (YerithQTableWidgetAdminWindowsCommons *) this,
            SLOT(decrementFontSize__OF_TABLE__WIDGET()));


    connect(action_START_YR_QVGE_dynamic_analysis_tool,
            SIGNAL(triggered()),
            this,
            SLOT(START_YRI_DB_RUNTIME_VERIF_GUI()));



    connect(comboBox_operations_maintenance,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_changer_commande_MAINTENANCE_OPERATION(const QString &)));


    connect(comboBox_sujets_maintenance,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_changer_commande_MAINTENANCE_SUJET(const QString &)));


    connect(comboBox_tableaux_mariadb_sql,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_changer_tableau_dimportation(const QString &)));


    connect(checkBox_importer_parlant, SIGNAL(stateChanged(int)),
            this, SLOT(handle_importer_parlant_checkBox(int)));


    connect(tabWidget_maintenance_verification_DATABASE,
            SIGNAL(currentChanged(int)),
			this,
            SLOT(handle__tabWidget_maintenance_verification_DATABASE(int)));


    connect(comboBox_impression_sur,
            SIGNAL(currentIndexChanged(const QString &)), this,
            SLOT(handleThermalPrinterConfiguration(const QString &)));

    connect(checkBox_activer_registre_de_caisse, SIGNAL(stateChanged(int)),
            this, SLOT(handleCheckboxActiverRegistreCaisse(int)));

    connect(comboBox_impression_sur,
            SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(choix_registre_de_caisse(const QString &)));

    connect(actionStocks, SIGNAL(triggered()), this,
            SLOT(action_lister()));
    connect(actionCreer, SIGNAL(triggered()), this, SLOT(action_creer()));
    connect(actionGo, SIGNAL(triggered()), this,
            SLOT(gerer_choix_action()));
    connect(actionModifier, SIGNAL(triggered()), this,
            SLOT(action_lister()));
    connect(actionSupprimer, SIGNAL(triggered()), this,
            SLOT(action_lister()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this,
            SLOT(apropos()));
    connect(actionGuide_PRATIQUE_DE_LUTILISATEUR, SIGNAL(triggered()), this,
            SLOT(getManuelUtilisateurPDF()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this,
            SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

    connect(comboBox_sujets_actions, SIGNAL(activated(int)), this,
            SLOT(gerer_choix_action()));

    connect(comboBox_localisation,
            SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(set_localisation_adresse_ip_text()));

    connect(tabWidget_administration,
            SIGNAL(currentChanged(int)),
            this,
            SLOT(handleTabChanged(int)));

    initialize_admin_importer_csv_tableau();


    set_shortcuts();
}

YerithAdminWindow::~YerithAdminWindow()
{
    delete _logger;
    delete _actionsToConst;
    delete _sujetActionsToConst;
}

void YerithAdminWindow::setupValidators()
{
    lineEdit_sauvegarde_de_yerith_erp_3_secondes->setValidator(&YerithUtils::
                                                               Int_BACKUP_YERITH_ERP_3_Validator);

    lineEdit_alert_period_time_interval->
    setValidator(&YerithUtils::UintValidator);

    lineEdit_alert_quantity_time_interval->
    setValidator(&YerithUtils::UintValidator);

    lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setValidator
    (&YerithUtils::
     nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE_Validator);

    lineEdit_longueur_maximale_string->
    setValidator(&YerithUtils::UintValidator);

    lineEdit_taille_de_pagination_par_defaut->
    setValidator(&YerithUtils::UintValidator);

    lineEdit_MAINTENANCE_VERIFICATION_log_size_max->
    setValidator(&YerithUtils::UintValidator);

    lineEdit_annee_depart_rapports_chiffre_affaire->
    setValidator(&YerithUtils::UintValidator);

    lineEdit_annee_de_depart_pour_la_pagination->
    setValidator(&YerithUtils::UintValidator);
}


void YerithAdminWindow::PRINT_SIGNALS_SOURCE_TARGET_info()
{
	int size = YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.size();

	static bool FIRST_TIME_EVER_call = true;

	if (FIRST_TIME_EVER_call)
	{
		if (size < YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.reserve(YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT);
		}

		FIRST_TIME_EVER_call = false;
	}

	QStringList cur_string_list;

	for (uint i = 0; i < size; ++i)
	{
		for (uint k = 0; k < YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT && k < size; ++k)
		{
			cur_string_list =
					YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.at(k)
					.split(";", Qt::KeepEmptyParts);

			tableWidget_LOGGING_runtime_VERIFICATION->addArticle(cur_string_list.at(0),
																 cur_string_list.at(1),
																 cur_string_list.at(2),
																 cur_string_list.at(3),
																 cur_string_list.at(4));
		}

		YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.clear();

		YERITH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.reserve(YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT);

		return ;
	}
}


void YerithAdminWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);


    pushButton_reinitialiser_le_logo_de_lentreprise->disable(this);

    pushButton_selectionner_logo_de_lentreprise->disable(this);

    pushButton_supprimer_logo_de_lentreprise->disable(this);


    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_operation_go->disable(this);
}

void YerithAdminWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");


    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, true);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);


    pushButton_reinitialiser_le_logo_de_lentreprise->disable(this);

    pushButton_selectionner_logo_de_lentreprise->disable(this);

    pushButton_supprimer_logo_de_lentreprise->disable(this);


    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
    pushButton_operation_go->enable(this, SLOT(gerer_choix_action()));
}


void YerithAdminWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);


    tabWidget_administration->setCurrentIndex(OPERATIONS);


    pushButton_reinitialiser_le_logo_de_lentreprise->enable(this,
                                                            SLOT
                                                            (reinitialiser_la_derniere_selection_du_logo_de_lentreprise
                                                             ()));

    pushButton_selectionner_logo_de_lentreprise->enable(this,
                                                        SLOT
                                                        (selectionner_logo_de_lentreprise
                                                         ()));

    pushButton_supprimer_logo_de_lentreprise->enable(this,
                                                     SLOT
                                                     (supprimer_logo_de_lentreprise
                                                      ()));


    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
    pushButton_operation_go->enable(this, SLOT(gerer_choix_action()));
}


void YerithAdminWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
    retranslateUi(this);

    _allWindows->_mainWindow->write_AFTER_SETUP_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    stop_TESTING_MAINTENANCE();

    groupBox_yerith_erp_3_0_parametres_serveur->setVisible(false);

    YerithPOSUser *user = _allWindows->getUser();

    if (0 != user && user->isManager())
    {
        if (user->titre() == "Prof" &&
        	user->nom_complet().startsWith("Prof Dr Xavier Noundou"))
        {
            start_TESTING_MAINTENANCE();

            PRINT_SIGNALS_SOURCE_TARGET_info();
        }

        YerithERPWindows *allWindows = YerithUtils::getAllWindows();

        QString localIpAddress;

        if (0 != allWindows)
        {
            localIpAddress = allWindows->getDatabase().db_ip_address();
        }

        groupBox_yerith_erp_3_0_parametres_serveur->setTitle(QObject::tr
                                                             ("paramètres serveur [ \"%1\" ]").
                                                             arg
                                                             (localIpAddress));

        groupBox_yerith_erp_3_0_parametres_serveur->setVisible(true);
    }

    if (0 != stocksTableModel)
    {
        _curStocksTableModel = stocksTableModel;
    }

    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            tabWidget_administration->setTabEnabled(DONNEES_ENTREPRISE, true);
        }
        else
        {
            tabWidget_administration->setTabEnabled(DONNEES_ENTREPRISE, false);
        }
    }

    set_colour_status_of_alert_daemon_process_buttons();

    read_configuration();

    populateComboBoxes();

    if (YerithERPConfig::_distantSiteConnected)
    {
        comboBox_localisation
			->find_AND_SET_CURRENT_INDEX(YerithERPConfig::_connectedSite);

        lineEdit_localisation_adresse_ip
			->setText(YerithERPConfig::_connectedSiteIPAddress);

        label_ONLINE->setVisible(true);

        label_OFFLINE->setVisible(false);
    }
    else
    {
        comboBox_localisation->resetYerithComboBox();

        lineEdit_localisation_adresse_ip->clear();

        label_ONLINE->setVisible(false);

        label_OFFLINE->setVisible(true);
    }

    _allWindows->_adminListerWindow->lister_remise(&_allWindows->getSqlTableModel_remises());

    _allWindows->_adminListerWindow->lister_alerte(&_allWindows->getSqlTableModel_alertes());

    _allWindows->_adminListerWindow
		->lister_compte_bancaire(&_allWindows->getSqlTableModel_comptes_bancaires());

    _allWindows->_adminListerWindow->lister_categorie(&_allWindows->getSqlTableModel_categories());

    _allWindows->_adminListerWindow
		->lister_LIGNE_BUDGETAIRE(&_allWindows->getSqlTableModel_lignes_budgetaires());

    _allWindows->_adminListerWindow->lister_utilisateur(&_allWindows->getSqlTableModel_users());

    _allWindows->_adminListerWindow
		->lister_departements_de_produits(&_allWindows->getSqlTableModel_departements_produits());

    _allWindows->_adminListerWindow
		->lister_localisation(&_allWindows->getSqlTableModel_localisations());

    handleTabChanged(tabWidget_administration->currentIndex());

    read_entreprise_info_database_table(false);

    setVisible(true);
}


void YerithAdminWindow::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}


bool YerithAdminWindow::imprimer_UNE_LISTE(bool *full_list /* = 0 */)
{
    return false;
}


void YerithAdminWindow::set_shortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);
}


void YerithAdminWindow::reinitialiser_la_derniere_selection_du_logo_de_lentreprise()
{
    if (!YerithInfoEntreprise::
            load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO
            (label_logo_de_lentreprise))
    {
        label_logo_de_lentreprise->setAutoFillBackground(false);
    }
}


void YerithAdminWindow::supprimer_logo_de_lentreprise()
{
    YerithERPWindows *allWindows = YerithUtils::getAllWindows();

    if (0 == allWindows)
    {
        return;
    }

    if (0 == label_logo_de_lentreprise->pixmap())
    {
        return;
    }


    YerithSqlTableModel &entreprise_info_TableModel =
                    allWindows->getSqlTableModel_entreprise_info();

    int row = 1;

    QSqlRecord entrepriseInfoRecord = entreprise_info_TableModel.record(row);

    if (entrepriseInfoRecord.isEmpty())
    {
        return;
    }


    QVariant
    un_logo_de_lentreprise(entrepriseInfoRecord.value
                           (YerithDatabaseTableColumn::LOGO_ENTREPRISE));

    if (un_logo_de_lentreprise.toByteArray().isEmpty())
    {
        QString
        msg(QObject::tr("L'entreprise n'a pas un logo enregistré !"));

        YerithQMessageBox::information(this, QObject::tr("suppression"), msg);

        return;
    }


    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression du logo de l'entreprise ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression d'1 logo de l'entreprise"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        entrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::LOGO_ENTREPRISE,
                 QVariant(QVariant::ByteArray));


        bool recordUpdated =
                        entreprise_info_TableModel.updateRecord(row,
                        										entrepriseInfoRecord,
																"src/admin/yerith-erp-admin-window.cpp",
																839);

        if (recordUpdated)
        {
            YerithUtils::supprimer_image(*label_logo_de_lentreprise);

            msgSupprimer =
                            QObject::tr("1 logo de l'entreprise a été supprimée !");

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression d'1 logo de l'entreprise - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("1 logo de l'entreprise n'a pas pu être supprimé !");

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression d'1 logo de l'entreprise - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerithAdminWindow::YERITH_ERP_3_populate_all_tables()
{
    comboBox_sujets_maintenance->addItem(QString("%1 %2").arg
                                         (YerithAdminWindow::
                                          DATABASE_YERITH_ERP_3_0_KEYWORD,
                                          YerithERPConfig::_db_name));

    QString strShowTABLESQuery("SHOW TABLES");

    QSqlQuery query;

    int querySize = YerithUtils::execQuery(query, strShowTABLESQuery);

    for (int k = 0; k < querySize && query.next(); ++k)
    {
        comboBox_sujets_maintenance->addItem(query.value(0).toString());
    }
}


void YerithAdminWindow::EXECUTER_COMMANDE_MAINTENANCE()
{
    if (lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->isEmpty
            ())
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("EXÉCUTER COMMANDE MAINTENANCE"),
                                   QObject::tr
                                   ("Sélectionner 1 commande à exécuter !"));

        return;
    }

    QString
    maintenanceCommand
    (lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->text
     ());

    QString currentProgram_mysql_mysqldump(YerithUtils::MYSQL);

    maintenanceCommand.replace("'", YerithUtils::EMPTY_STRING);

    int lenToRemoveFromMAINTENANCE_COMMAND = 0;

    if (maintenanceCommand.contains(YerithUtils::MYSQL_DUMP))
    {
        currentProgram_mysql_mysqldump = YerithUtils::MYSQL_DUMP;

        lenToRemoveFromMAINTENANCE_COMMAND = YerithUtils::MYSQL_DUMP.length();
    }
    else if (maintenanceCommand.contains(YerithUtils::MYSQL))
    {
        currentProgram_mysql_mysqldump = YerithUtils::MYSQL;

        lenToRemoveFromMAINTENANCE_COMMAND = YerithUtils::MYSQL.length();
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("EXÉCUTER COMMANDE MAINTENANCE"),
                                   QObject::tr
                                   ("Commande à exécuter INVALIDE !"));

        return;
    }

//      QDEBUG_STRING_OUTPUT_2("maintenanceCommand", maintenanceCommand);

    QString progArgsString(maintenanceCommand.remove(0,
                                                     lenToRemoveFromMAINTENANCE_COMMAND).
                           trimmed());

    progArgsString.replace("--databases",
                           QString("--databases %1").
                           arg(YerithERPConfig::_db_name));

    progArgsString.replace("db",
                           QString("%1 %2").arg(YerithERPConfig::_db_name,
                                                comboBox_sujets_maintenance->
                                                currentText()));

    progArgsString.replace("-p",
                           QString("-p%1").
                           arg(YerithERPConfig::_db_user_pwd));

//      QDEBUG_STRING_OUTPUT_2("progArgsString", progArgsString);


    QString progArgString_NOT_TO_SPLIT;

    QString progArgString_TO_SPLIT(progArgsString);

    if (progArgsString.contains("--execute"))
    {
        QStringList splitted_cmd_string(progArgsString.split("--execute"));

        progArgString_TO_SPLIT = splitted_cmd_string.at(0).trimmed();

        progArgString_NOT_TO_SPLIT = splitted_cmd_string.at(1).trimmed();

//              QDEBUG_STRING_OUTPUT_2("progArgString_NOT_TO_SPLIT", progArgString_NOT_TO_SPLIT);

//              QDEBUG_STRING_OUTPUT_2("progArgString_TO_SPLIT", progArgString_TO_SPLIT);
    }

    QStringList
    progArguments(progArgString_TO_SPLIT.split
                  (YerithUtils::EMPTY_SPACE_REGEXP));

    QString yerith_erp_3_0_restore_backup_sql_file(QString("%1.sql").arg
                                                   (FILE_NAME_USERID_CURRENT_TIME
                                                    ("yerith_erp_3_0_BACKUP_RESTORE")));

    QString
    mysqlProcessProgram(QString("%1/%2").arg
                        (YerithERPConfig::pathToMARIA_DB_BASE_DIR,
                         currentProgram_mysql_mysqldump));

    if (progArgsString.contains("--execute"))
    {
        progArguments.append(QString("--execute%1").
                             arg(progArgString_NOT_TO_SPLIT));
    }

//      QDEBUG_STRINGS_OUTPUT_QSTRINGLIST("progArguments", progArguments);

    int output_file_size =
                    YerithERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE
                    (mysqlProcessProgram,
                     YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY,
                     yerith_erp_3_0_restore_backup_sql_file,
                     progArguments);

    if (!progArgsString.contains("--execute"))
    {
        YerithUtils::GZIP_YERITH_FILE
        (YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY,
         QString("%1/%2").arg
         (YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY,
          yerith_erp_3_0_restore_backup_sql_file));
    }


    QString
    userViewPrettyCommand(QString("%1 %2").arg
                          (comboBox_operations_maintenance->currentText(),
                           comboBox_sujets_maintenance->currentText()));

    if (output_file_size <= 0 && !progArgsString.contains("--execute"))
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("EXÉCUTER COMMANDE MAINTENANCE"),
                                   QObject::tr
                                   ("Commande non exécutable: \"%1\" (%2) !\n\n"
//                                                              "%3\n\n"
                                    "Répertoire d'exécution: \"%3\".").arg
                                   (mysqlProcessProgram, userViewPrettyCommand,
//                                                               lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->text(),
                                    YerithERPConfig::
                                    fullpathToBACKUP_YERITH_ERP_3_DIRECTORY));
        return;
    }

    if (output_file_size > 0)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("EXÉCUTER COMMANDE MAINTENANCE"),
                                       QObject::tr
                                       ("La commande \"%1\" a été exécuter avec succès !\n\n"
//                                                              "%2\n\n"
                                        "Répertoire d'exécution: \"%2\".").
                                       arg(userViewPrettyCommand,
//                                                               lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->text(),
                                           YerithERPConfig::
                                           fullpathToBACKUP_YERITH_ERP_3_DIRECTORY));
    }
}


void YerithAdminWindow::reinitialiser_AFFICHAGE_COMMANDE_MAINTENANCE()
{
    comboBox_sujets_maintenance->setCurrentIndex(0);
    comboBox_operations_maintenance->setCurrentIndex(0);

    label_administration_exporter_yerith_erp_3->setVisible(true);
    label_administration_effacer_tableau->setVisible(true);
    label_administration_exporter_tableau->setVisible(true);
    label_administration_importer_tableau->setVisible(true);
    label_administration_supprimer_tableau->setVisible(true);

    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->clear();
    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->setVisible
    (true);
    lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_effacer_un_tableau->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_exporter_un_tableau->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_importer_un_tableau->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_supprimer_un_tableau->
    setVisible(true);
}


void YerithAdminWindow::handleCheckboxActiver_SHOW_DATABASE_RAW(int state)
{
    if (Qt::Checked == state)
    {
        YerithERPConfig::SHOW_DATABASE_RAW = true;
    }
    else
    {
        YerithERPConfig::SHOW_DATABASE_RAW = false;
    }
}


void YerithAdminWindow::handleCheckboxActiverRegistreCaisse(int state)
{
    if (Qt::Checked == state)
    {
        YerithERPConfig::ouvrirRegistreDeCaisse = true;
    }
    else
    {
        YerithERPConfig::ouvrirRegistreDeCaisse = false;
    }
}


void
YerithAdminWindow::

handle_changer_commande_MAINTENANCE_OPERATION_SUJET_for_database(const
                                                                 QString &
                                                                 commande_MAINTENANCE,
                                                                 const
                                                                 QString &
                                                                 sqlTableName)
{
    if (!YerithUtils::isEqualCaseInsensitive(EXPORTER, commande_MAINTENANCE))
    {
        comboBox_operations_maintenance->setCurrentIndex
        (comboBox_operations_maintenance->findText(EXPORTER));
    }

    changer_commande_YERITH_LINE_EDIT
    (*lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0,
     sqlTableName);

    label_administration_exporter_yerith_erp_3->setVisible(true);

    lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0->
    setVisible(true);

    cacher_autres_commandes_YERITH_LINE_EDIT
    (lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0);
}


void YerithAdminWindow::handle_changer_commande_MAINTENANCE_OPERATION(const
                                                                      QString &
                                                                      commande_MAINTENANCE)
{
    QString sqlTableName(comboBox_sujets_maintenance->currentText());

    if (sqlTableName.contains
            (YerithAdminWindow::DATABASE_YERITH_ERP_3_0_KEYWORD))
    {
        handle_changer_commande_MAINTENANCE_OPERATION_SUJET_for_database
        (commande_MAINTENANCE, sqlTableName);
        return;
    }

    if (YerithUtils::isEqualCaseInsensitive(EFFACER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_effacer_un_tableau,
         sqlTableName);
    }
    else if (YerithUtils::isEqualCaseInsensitive(EXPORTER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_exporter_un_tableau,
         sqlTableName);
    }
    else if (YerithUtils::isEqualCaseInsensitive(IMPORTER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_importer_un_tableau,
         sqlTableName);
    }
    else if (YerithUtils::isEqualCaseInsensitive(SUPPRIMER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_supprimer_un_tableau,
         sqlTableName);
    }
    else
    {
        QDEBUG_STRING_OUTPUT_1(" ## INVALID COMMAND FOR MAINTENANCE ##");
    }
}


void YerithAdminWindow::handle_changer_commande_MAINTENANCE_SUJET(const QString &
                                                                  sqlTableName)
{
    QString
    commande_MAINTENANCE(comboBox_operations_maintenance->currentText());

    if (sqlTableName.contains
            (YerithAdminWindow::DATABASE_YERITH_ERP_3_0_KEYWORD))
    {
        handle_changer_commande_MAINTENANCE_OPERATION_SUJET_for_database
        (commande_MAINTENANCE, sqlTableName);
        return;
    }

    if (YerithUtils::isEqualCaseInsensitive(EFFACER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_effacer_un_tableau,
         sqlTableName);
    }
    else if (YerithUtils::isEqualCaseInsensitive(EXPORTER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_exporter_un_tableau,
         sqlTableName);
    }
    else if (YerithUtils::isEqualCaseInsensitive(IMPORTER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_importer_un_tableau,
         sqlTableName);
    }
    else if (YerithUtils::isEqualCaseInsensitive(SUPPRIMER, commande_MAINTENANCE))
    {
        changer_commande_YERITH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_supprimer_un_tableau,
         sqlTableName);
    }
    else
    {
        QDEBUG_STRING_OUTPUT_1(" ## INVALID COMMAND FOR MAINTENANCE ##");
    }
}


void YerithAdminWindow::handle_importer_parlant_checkBox(int aState)
{
    if (checkBox_importer_parlant->isChecked())
    {
        setImporterParlant(true);
    }
    else
    {
        setImporterParlant(false);
    }
}


void YerithAdminWindow::choix_registre_de_caisse(const QString &
                                                 labelImpressionSur)
{
    if (YerithUtils::isEqualCaseInsensitive(labelImpressionSur,
                                            YerithUtils::IMPRIMANTE_EPSON_TM_T20ii))
    {
        label_activer_registre_de_caisse->setEnabled(true);
        checkBox_activer_registre_de_caisse->setEnabled(true);

        comboBox_format_de_facture->clear();
        comboBox_format_de_facture->
        addItem(YerithERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->
        addItem(YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER);
        comboBox_format_de_facture->
        addItem(YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
        comboBox_format_de_facture->setYerithEnabled(true);
    }
    else if (YerithUtils::isEqualCaseInsensitive(labelImpressionSur,
                                                 YerithUtils::IMPRIMANTE_PDF))
    {
        label_activer_registre_de_caisse->setEnabled(false);
        checkBox_activer_registre_de_caisse->setChecked(false);
        checkBox_activer_registre_de_caisse->setEnabled(false);

        comboBox_format_de_facture->clear();
        comboBox_format_de_facture->
        addItem(YerithERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->
        addItem(YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER);
        comboBox_format_de_facture->
        addItem(YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
        comboBox_format_de_facture->setYerithEnabled(true);
    }
}


void YerithAdminWindow::reset_import_current_selected_csv_file()
{
    if (!lineEdit_fichier_csv_a_importer->text().isEmpty())
    {
        YerithUtils::import_csv_file_content(lineEdit_fichier_csv_a_importer->
                                             text(),
                                             _curCsvFileToImportContentWordList);

        generate_table_header_mapping_entries_for_csv_import();

        for (int k = 0; k < _csvContentIdxToSQLTableImportHeader.size(); ++k)
        {
            _csvContentIdxToSQLTableImportHeader.value(k)->
            resetYerithComboBox();
        }
    }
    else
    {
        clear_csv_import_panel_content_mapping();
    }
}


void YerithAdminWindow::import_current_selected_csv_file()
{
    if (_curCsvFileToImportContentWordList.size() <= 0)
    {
        return;
    }

    QStringList csvHeaderContent =
                    _curCsvFileToImportContentWordList.at(0).
                    split(YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    int curCsvFileLineSize = csvHeaderContent.size();

    QString dbFieldName;

    YerithERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

    YerithComboBox *aMappedComboBox = 0;

    for (int i = 0; i < curCsvFileLineSize; ++i)
    {
        aMappedComboBox = _csvContentIdxToSQLTableImportHeader.value(i);

        if (0 != aMappedComboBox)
        {
            dbFieldName = aMappedComboBox->currentText();

            curDatabaseTableColumnInfo =
                            new YerithERPDatabaseTableColumnInfo(dbFieldName,
                                                                 _dbTableColumnToType.value
                                                                 (dbFieldName));

            _csvContentIdxToDatabaseTableColumnInfo.insert(i,
                                                           curDatabaseTableColumnInfo);

            _allDatabaseTableColumnInfo.append(curDatabaseTableColumnInfo);
        }
    }

    _current_selected_import_table =
                    comboBox_tableaux_mariadb_sql->currentText();

    int successImportCount = 0;

    if (YerithUtils::isEqualCaseInsensitive(_current_selected_import_table,
                                            YerithDatabase::STOCKS))
    {
        YerithERPStockImport erpStockImport(*this,
                                            _curCsvFileToImportContentWordList,
                                            _csvContentIdxToDatabaseTableColumnInfo);

        YerithProgressBar(this)(&erpStockImport,
                                &YerithERPStockImport::import_csv_file,
                                &successImportCount,
                                _curCsvFileToImportContentWordList.size());
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (_current_selected_import_table, YerithDatabase::MARCHANDISES))
    {
        YerithERPMarchandiseImport stockMarchandiseImport(*this,
                                                          _curCsvFileToImportContentWordList,
                                                          _csvContentIdxToDatabaseTableColumnInfo);

        YerithProgressBar(this)(&stockMarchandiseImport,
                                &YerithERPMarchandiseImport::import_csv_file,
                                &successImportCount,
                                _curCsvFileToImportContentWordList.size());
    }


    QString msg;

    if (successImportCount > 0)
    {
        msg = QObject::tr("'%1' entrée(s) du fichier CSV ont "
                              "été importée(s) avec succès !").arg
              (QString::number(successImportCount));


        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("résultat importation fichier CSV"),
                                       msg);
    }
    else
    {
        msg =
                        QObject::tr
                        ("AUCUNE DONNÉE DU FICHIER (.csv) n'a été importée !");


        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("résultat importation fichier CSV"),
                                       msg);
    }
}


bool YerithAdminWindow::generate_table_header_mapping_entries_for_csv_import()
{
    if (_curCsvFileToImportContentWordList.size() <= 1)
    {
        return false;
    }

    QStringList csvHeaderContent =
                    _curCsvFileToImportContentWordList.at(0).
                    split(YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    bool csvFileHasVisibleContentToImport = false;

    int curCsvFileColumnSize = csvHeaderContent.size();

    QString aCsvHeaderString;

    QLabel *aCsvHeaderLabel = 0;

    YerithComboBox *aMappedComboBox = 0;

    for (int k = 0; k < csvHeaderContent.size(); ++k)
    {
        aCsvHeaderString = csvHeaderContent.at(k);

        aCsvHeaderLabel = _csvContentIdxToCsvFileContentImportHeader.value(k);

        if (0 != aCsvHeaderLabel)
        {
            aCsvHeaderLabel->setVisible(true);
            aCsvHeaderLabel->setEnabled(true);

            aCsvHeaderLabel->setText(QString("%1 (%2)")
                                .arg(aCsvHeaderString,
                                     QString::number(k + 1)));

            csvFileHasVisibleContentToImport = true;
        }
    }


    QMapIterator<int, YerithComboBox *> itIndexToSQLTableImportHeader(_csvContentIdxToSQLTableImportHeader);

    while (itIndexToSQLTableImportHeader.hasNext())
    {
        itIndexToSQLTableImportHeader.next();

        aMappedComboBox = itIndexToSQLTableImportHeader.value();

        if (0 != aMappedComboBox)
        {
            aMappedComboBox->setVisible(false);
            aMappedComboBox->setEnabled(false);
            aMappedComboBox->clear();
        }
    }


    YerithUtils::fillDBTableColumnNameToDBTableColumnType_TEST
                    (comboBox_tableaux_mariadb_sql->currentText(),
                     _dbTableColumnToType,
                     _dbTableColumnToIsNotNULL);


    for (int i = 0; i < curCsvFileColumnSize; ++i)
    {
        aMappedComboBox = _csvContentIdxToSQLTableImportHeader.value(i);

        if (0 != aMappedComboBox)
        {
            aMappedComboBox->setVisible(true);
            aMappedComboBox->setEnabled(true);
            aMappedComboBox->addItems(_dbTableColumnToType.keys());

            aMappedComboBox
                ->find_AND_SET_CURRENT_INDEX_No_ADD_NEW_Item(csvHeaderContent.at(i));

            csvFileHasVisibleContentToImport = true;
        }
    }

    for (int j = curCsvFileColumnSize; j < MAX_IMPORT_CSV_HEADER_SIZE; ++j)
    {
        aCsvHeaderLabel = _csvContentIdxToCsvFileContentImportHeader.value(j);

        if (0 != aCsvHeaderLabel)
        {
            aCsvHeaderLabel->setVisible(false);
            aCsvHeaderLabel->setEnabled(false);
        }
    }

    YerithERPStockImport::_dbTableColumnToIsNotNULL =
                    &_dbTableColumnToIsNotNULL;

    pushButton_importer_fichier_csv_reinitialiser->enable(this,
                                                          SLOT
                                                          (reset_import_current_selected_csv_file
                                                           ()));

    pushButton_importer_fichier_csv_valider->enable(this,
                                                    SLOT
                                                    (import_current_selected_csv_file
                                                     ()));

    return csvFileHasVisibleContentToImport;
}


void YerithAdminWindow::populateComboBoxes()
{
    comboBox_localisation
    	->populateComboBoxRawString(YerithDatabase::LOCALISATIONS,
                              	    YerithDatabaseTableColumn::NOM_LOCALISATION);
}


void YerithAdminWindow::cacher_autres_commandes_YERITH_LINE_EDIT(YerithLineEdit *
                                                                 aYerithCommandLineEdit)
{
    if (0 != aYerithCommandLineEdit)
    {
        aYerithCommandLineEdit->setVisible(true);

        if (aYerithCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0)
        {
            label_administration_exporter_yerith_erp_3->setVisible(false);
            lineEdit_administration_maintenance_commandes_exporter_yeritherp3_0->
            setVisible(false);
        }
        else
        {
            label_administration_exporter_yerith_erp_3->setVisible(true);
        }

        if (aYerithCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_effacer_un_tableau)
        {
            label_administration_effacer_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_effacer_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_effacer_tableau->setVisible(true);
        }

        if (aYerithCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_exporter_un_tableau)
        {
            label_administration_exporter_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_exporter_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_exporter_tableau->setVisible(true);
        }

        if (aYerithCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_importer_un_tableau)
        {
            label_administration_importer_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_importer_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_importer_tableau->setVisible(true);
        }

        if (aYerithCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_supprimer_un_tableau)
        {
            label_administration_supprimer_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_supprimer_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_supprimer_tableau->setVisible(true);
        }
    }
}


void YerithAdminWindow::changer_commande_YERITH_LINE_EDIT(YerithLineEdit &
                                                          aYerithCommandLineEdit,
                                                          const QString &
                                                          sqlTableName)
{
    cacher_autres_commandes_YERITH_LINE_EDIT(&aYerithCommandLineEdit);

    QString curCOMMANDString(aYerithCommandLineEdit.text());

    curCOMMANDString.replace("%1",
                             QString("%1").
                             arg(YerithERPConfig::_db_user_name));

    curCOMMANDString.replace("%3",
                             QString("%1").arg(YerithERPConfig::_db_name));

    if (sqlTableName.contains
            (YerithAdminWindow::DATABASE_YERITH_ERP_3_0_KEYWORD))
    {
        curCOMMANDString.replace("%2",
                                 QString("%1").
                                 arg(YerithERPConfig::_db_name));
    }
    else
    {
        curCOMMANDString.replace("%2",
                                 QString("%1.%2").
                                 arg(YerithERPConfig::_db_name,
                                     sqlTableName));
    }

//      QDEBUG_STRING_OUTPUT_2("cur_COMMAND_string", curCOMMANDString);

    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->setText
    (curCOMMANDString);
}


void YerithAdminWindow::clear_csv_import_panel_content_mapping()
{
    QLabel *aCsvHeaderLabel = 0;

    for (int k = 0; k < _csvContentIdxToCsvFileContentImportHeader.size(); ++k)
    {
        aCsvHeaderLabel = _csvContentIdxToCsvFileContentImportHeader.value(k);

        if (0 != aCsvHeaderLabel)
        {
            aCsvHeaderLabel->setVisible(false);
            aCsvHeaderLabel->setEnabled(false);
            aCsvHeaderLabel->setText(YerithUtils::EMPTY_STRING);
        }
    }

    QMapIterator < int,
                 YerithComboBox *
                 >itIndexToSQLTableImportHeader(_csvContentIdxToSQLTableImportHeader);

    YerithComboBox *aMappedComboBox = 0;

    while (itIndexToSQLTableImportHeader.hasNext())
    {
        itIndexToSQLTableImportHeader.next();

        aMappedComboBox = itIndexToSQLTableImportHeader.value();

        if (0 != aMappedComboBox)
        {
            aMappedComboBox->setVisible(false);
            aMappedComboBox->setEnabled(false);
            aMappedComboBox->clear();
        }
    }

    pushButton_importer_fichier_csv_reinitialiser->setEnabled(false);

    pushButton_importer_fichier_csv_valider->setEnabled(false);
}


void YerithAdminWindow::initialize_admin_importer_csv_tableau()
{
    pushButton_fichier_csv_a_importer->enable(this,
                                              SLOT
                                              (choose_fichier_csv_a_importer
                                               ()));

    _csvContentIdxToCsvFileContentImportHeader.insert(0,
                                                      label_importer_fichier_csv_entete_0);
    _csvContentIdxToCsvFileContentImportHeader.insert(1,
                                                      label_importer_fichier_csv_entete_1);
    _csvContentIdxToCsvFileContentImportHeader.insert(2,
                                                      label_importer_fichier_csv_entete_2);
    _csvContentIdxToCsvFileContentImportHeader.insert(3,
                                                      label_importer_fichier_csv_entete_3);
    _csvContentIdxToCsvFileContentImportHeader.insert(4,
                                                      label_importer_fichier_csv_entete_4);
    _csvContentIdxToCsvFileContentImportHeader.insert(5,
                                                      label_importer_fichier_csv_entete_5);
    _csvContentIdxToCsvFileContentImportHeader.insert(6,
                                                      label_importer_fichier_csv_entete_6);
    _csvContentIdxToCsvFileContentImportHeader.insert(7,
                                                      label_importer_fichier_csv_entete_7);
    _csvContentIdxToCsvFileContentImportHeader.insert(8,
                                                      label_importer_fichier_csv_entete_8);
    _csvContentIdxToCsvFileContentImportHeader.insert(9,
                                                      label_importer_fichier_csv_entete_9);
    _csvContentIdxToCsvFileContentImportHeader.insert(10,
                                                      label_importer_fichier_csv_entete_10);
    _csvContentIdxToCsvFileContentImportHeader.insert(11,
                                                      label_importer_fichier_csv_entete_11);
    _csvContentIdxToCsvFileContentImportHeader.insert(12,
                                                      label_importer_fichier_csv_entete_12);
    _csvContentIdxToCsvFileContentImportHeader.insert(13,
                                                      label_importer_fichier_csv_entete_13);
    _csvContentIdxToCsvFileContentImportHeader.insert(14,
                                                      label_importer_fichier_csv_entete_14);
    _csvContentIdxToCsvFileContentImportHeader.insert(15,
                                                      label_importer_fichier_csv_entete_15);

    for (int k = 0; k < _csvContentIdxToCsvFileContentImportHeader.size(); ++k)
    {
        _csvContentIdxToCsvFileContentImportHeader.value(k)->
        setVisible(false);
    }

    _csvContentIdxToSQLTableImportHeader.insert(0,
                                                comboBox_importer_tableau_entete_0);
    _csvContentIdxToSQLTableImportHeader.insert(1,
                                                comboBox_importer_tableau_entete_1);
    _csvContentIdxToSQLTableImportHeader.insert(2,
                                                comboBox_importer_tableau_entete_2);
    _csvContentIdxToSQLTableImportHeader.insert(3,
                                                comboBox_importer_tableau_entete_3);
    _csvContentIdxToSQLTableImportHeader.insert(4,
                                                comboBox_importer_tableau_entete_4);
    _csvContentIdxToSQLTableImportHeader.insert(5,
                                                comboBox_importer_tableau_entete_5);
    _csvContentIdxToSQLTableImportHeader.insert(6,
                                                comboBox_importer_tableau_entete_6);
    _csvContentIdxToSQLTableImportHeader.insert(7,
                                                comboBox_importer_tableau_entete_7);
    _csvContentIdxToSQLTableImportHeader.insert(8,
                                                comboBox_importer_tableau_entete_8);
    _csvContentIdxToSQLTableImportHeader.insert(9,
                                                comboBox_importer_tableau_entete_9);
    _csvContentIdxToSQLTableImportHeader.insert(10,
                                                comboBox_importer_tableau_entete_10);
    _csvContentIdxToSQLTableImportHeader.insert(11,
                                                comboBox_importer_tableau_entete_11);
    _csvContentIdxToSQLTableImportHeader.insert(12,
                                                comboBox_importer_tableau_entete_12);
    _csvContentIdxToSQLTableImportHeader.insert(13,
                                                comboBox_importer_tableau_entete_13);
    _csvContentIdxToSQLTableImportHeader.insert(14,
                                                comboBox_importer_tableau_entete_14);
    _csvContentIdxToSQLTableImportHeader.insert(15,
                                                comboBox_importer_tableau_entete_15);

    for (int k = 0; k < _csvContentIdxToSQLTableImportHeader.size(); ++k)
    {
        _csvContentIdxToSQLTableImportHeader.value(k)->setVisible(false);
    }


    QSqlQuery databaseTableNameQuery;

    YerithERPWindows *yerithERPWindows = YerithUtils::getAllWindows();

    if (0 != yerithERPWindows)
    {
        comboBox_tableaux_mariadb_sql->addItem(YerithDatabase::MARCHANDISES);
        comboBox_tableaux_mariadb_sql->addItem(YerithDatabase::STOCKS);
    }
}


void YerithAdminWindow::creer(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminCreateWindow
        ->rendreVisible(_curStocksTableModel,
                        selectedSujetAction);
    rendreInvisible();
}


void YerithAdminWindow::lister(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminListerWindow->rendreVisible(selectedSujetAction);

    rendreInvisible();
}


void YerithAdminWindow::modifier(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminListerWindow->rendreVisible(selectedSujetAction);
    rendreInvisible();
}


void YerithAdminWindow::CHANGER_DE_LANGUE()
{
    YerithWindowsCommons::CHANGER_DE_LANGUE(this,
                                            action_TRADUIRE_EN_ANGLAIS);

    rendreVisible(_curStocksTableModel);
}


void YerithAdminWindow::DISABLE_YRI_DB_RUNTIME_VERIF_BUTTONS()
{
	tabWidget_maintenance_verification_DATABASE->setTabVisible(0, false);

	groupBox_yri_db_runtime_verif->setVisible(false);

	groupBox_yri_db_runtime_verif->setEnabled(false);

	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(false);

	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(false);

	label_QT_DBUS_ERROR_DB_RUNTIME_VERIFICATOR->setVisible(false);

    pushButton_REFRESH_STATUS_DB_RUNTIME_VERIFICATOR->setVisible(false);

    pushButton_START_DB_RUNTIME_VERIFICATOR->setVisible(false);

    pushButton_STOP_DB_RUNTIME_VERIFICATOR->setVisible(false);
}


void YerithAdminWindow::ENABLE_YRI_DB_RUNTIME_VERIF_BUTTONS()
{
	tabWidget_maintenance_verification_DATABASE->setTabVisible(0, true);

	groupBox_yri_db_runtime_verif->setVisible(true);

	groupBox_yri_db_runtime_verif->setEnabled(true);

	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(true);

	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(true);

    pushButton_REFRESH_STATUS_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(REFRESH_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS()));

    pushButton_START_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(START_YRI_DB_RUNTIME_VERIF_DAEMON_PROCESS()));

    pushButton_STOP_DB_RUNTIME_VERIFICATOR
		->enable(this,
				 SLOT(STOP_YRI_DB_RUNTIME_VERIF_DAEMON_PROCESS()));

    REFRESH_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS();
}


void YerithAdminWindow::stop_TESTING_MAINTENANCE()
{
    int indexEffacer = comboBox_operations_maintenance->findText(EFFACER);

    if (indexEffacer > -1)
    {
        comboBox_operations_maintenance->removeItem(indexEffacer);
    }

    int indexSupprimer = comboBox_operations_maintenance->findText(SUPPRIMER);

    if (indexSupprimer > -1)
    {
        comboBox_operations_maintenance->removeItem(indexSupprimer);
    }

    groupBox_maintenance_commandes->setVisible(false);

    groupBox_maintenance_commandes->setEnabled(false);

    DISABLE_YRI_DB_RUNTIME_VERIF_BUTTONS();
}


void YerithAdminWindow::start_TESTING_MAINTENANCE()
{
    int indexEffacer = comboBox_operations_maintenance->findText(EFFACER);

    if (indexEffacer < 0)
    {
        comboBox_operations_maintenance->addItem(EFFACER);
    }

    int indexSupprimer = comboBox_operations_maintenance->findText(SUPPRIMER);

    if (indexSupprimer < 0)
    {
        comboBox_operations_maintenance->addItem(SUPPRIMER);
    }

    lineEdit_administration_maintenance_commandes_importer_un_tableau->setText
    (QString("mysql -u '%1' -p < '%2'"));

    lineEdit_administration_maintenance_commandes_effacer_un_tableau->setText
    (QString("mysql -u '%1' -p --execute=truncate table %2"));

    lineEdit_administration_maintenance_commandes_supprimer_un_tableau->setText
    (QString("mysql -u '%1' -p --execute=drop table %2"));

    groupBox_maintenance_commandes->setVisible(true);

    groupBox_maintenance_commandes->setEnabled(true);

    ENABLE_YRI_DB_RUNTIME_VERIF_BUTTONS();
}


void YerithAdminWindow::enableAllOperationsTabPushButtons()
{
    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
}

void YerithAdminWindow::disableAllOperationsTabPushButtons()
{
    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
}

void YerithAdminWindow::
	setCurrentAdminWindowTitle(enum YerithAdminWindowTabPositionsTypes tabPosition)
{
    QStringList adminWindowTitleParts =
                    _yerithAdminWindowTitleStart.split('~');

    //adminWindowTitleParts.at(0) contient le string 'Yerith-pgi-3.0'.

    if (2 <= adminWindowTitleParts.size())
    {
        QString newAdminWindowTitle(adminWindowTitleParts.at(0).trimmed());

        newAdminWindowTitle.append(" ~ ")
        				   .append(tabWidget_administration->tabText(tabPosition))
						   .append(" ~ ")
						   .append(adminWindowTitleParts.at(1).trimmed());


        setWindowTitle(newAdminWindowTitle);
    }
}

void YerithAdminWindow::handleTabChanged(int currentTab)
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(ACTION_GET_runtime_verification_report_PDF, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);



    switch (currentTab)
    {
    case CONNECTER_LOCALISATION:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(CONNECTER_LOCALISATION);
        break;

    case DONNEES_ENTREPRISE:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(DONNEES_ENTREPRISE);
        break;

    case IMPORTER_CSV_TABLEAU:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(IMPORTER_CSV_TABLEAU);
        break;

    case MAINTENANCE:

        handle__tabWidget_maintenance_verification_DATABASE
            (tabWidget_maintenance_verification_DATABASE->currentIndex());

        disableAllOperationsTabPushButtons();

        setCurrentAdminWindowTitle(MAINTENANCE);

        break;

    case OPERATIONS:
        enableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(OPERATIONS);
        break;

    case PARAMETRES_APPLICATION:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(PARAMETRES_APPLICATION);
        break;

    case SYSTEME_DALERTES:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(SYSTEME_DALERTES);
        break;

    default:
        disableAllOperationsTabPushButtons();
        setWindowTitle(_yerithAdminWindowTitleStart);
        break;
    }
}

void YerithAdminWindow::action_creer()
{
//    _curAdminSujetAction =
//                    _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        creer(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        creer(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        creer(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        creer(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        creer(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        creer(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_ALERTE:
        creer(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_REMISE:
        creer(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        creer(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerithAdminWindow::action_lister()
{
//    _curAdminSujetAction =
//    		_sujetActionsToConst->value(comboBox_sujets_actions->currentText());

    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        lister(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        lister(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        lister(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        lister(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        lister(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        lister(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_ALERTE:
        lister(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_REMISE:
        lister(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        lister(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerithAdminWindow::action_modifier()
{
//    _curAdminSujetAction =
//                    _sujetActionsToConst->value(comboBox_sujets_actions->currentText());

    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        modifier(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        modifier(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        modifier(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        modifier(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        modifier(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_REMISE:
        modifier(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_ALERTE:
        modifier(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        modifier(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerithAdminWindow::action_supprimer()
{
//    _curAdminSujetAction =
//                    _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        modifier(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        modifier(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        modifier(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        modifier(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        modifier(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        modifier(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_ALERTE:
        modifier(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_REMISE:
        modifier(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        modifier(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerithAdminWindow::action_exporter()
{
    _curAdminSujetsMaintainAction =
    		_sujetActionsToConst->value(comboBox_sujets_maintenance->currentText());

    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_exporter(SUJET_ACTION_DB);
        break;

    case SUJET_ACTION_TABLEAU:
        maintenance_exporter(SUJET_ACTION_TABLEAU);
        break;

    default:
        break;
    }
}

void YerithAdminWindow::action_importer()
{
    _curAdminSujetsMaintainAction =
    		_sujetActionsToConst->value(comboBox_sujets_maintenance->currentText());

    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_importer(SUJET_ACTION_DB);
        break;

    case SUJET_ACTION_TABLEAU:
        maintenance_importer(SUJET_ACTION_TABLEAU);
        break;

    default:
        break;
    }
}

void YerithAdminWindow::action_effacer()
{
    _curAdminSujetsMaintainAction =
    		_sujetActionsToConst->value(comboBox_sujets_maintenance->currentText());

    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_importer(SUJET_ACTION_DB);
        break;

    case SUJET_ACTION_TABLEAU:
        maintenance_importer(SUJET_ACTION_TABLEAU);
        break;

    case SUJET_ACTION_DONNEES:
        maintenance_effacer(SUJET_ACTION_DONNEES);
        break;

    default:
        break;
    }
}

void YerithAdminWindow::gerer_choix_action()
{
    //qDebug() << "YerithAdminWindow::gerer_choix_action()";
	_curAdminAction = _actionsToConst->value(comboBox_actions->currentText());

	_curAdminSujetAction = (enum AdminSujetAction) comboBox_sujets_actions->currentIndex();

    switch (_curAdminAction)
    {
    case ACTION_CREER:
        action_creer();
        break;

    case ACTION_LISTER:
        action_lister();
        break;

    case ACTION_MODIFIER:
        action_modifier();
        break;

    case ACTION_SUPPRIMER:
        action_supprimer();
        break;

    case ACTION_EXPORTER:
        action_exporter();
        break;

    case ACTION_IMPORTER:
        action_importer();
        break;

    default:
        break;
    }
}


void YerithAdminWindow::choose_fichier_csv_a_importer()
{
    QString csvFilePath =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Choisir le chemin qui mène au fichier au format (.csv)"),
										 QString::null,
										 QObject::tr(".csv (*.csv)"));
    if (!csvFilePath.isEmpty())
    {
        lineEdit_fichier_csv_a_importer->setText(csvFilePath);

        YerithUtils::import_csv_file_content(csvFilePath,
                                             _curCsvFileToImportContentWordList);

        generate_table_header_mapping_entries_for_csv_import();
    }
}


void YerithAdminWindow::choose_path_mariadb_client_dir()
{
    QString mariaDBClientBASE_DIR_FULL_PATH =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Choisir le chemin qui mène au répertoire de base de 'MariaDB-client'"),
										 QString::null,
										 QString::null);

    if (!mariaDBClientBASE_DIR_FULL_PATH.isEmpty())
    {
        lineEdit_mariadb_client_base_dir_full_path
			->setText(mariaDBClientBASE_DIR_FULL_PATH);
    }
}


void YerithAdminWindow::choose_path_pdfReader()
{
    QString pdfReaderFilePath =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Choisir le chemin qui mène au lecteur de fichiers PDF"),
										 QString::null,
										 QString::null);

    if (!pdfReaderFilePath.isEmpty())
    {
        lineEdit_pdfReader->setText(pdfReaderFilePath);
    }
}


void YerithAdminWindow::choose_path_thermalPrinterDeviceFile()
{
    QString thermalPrinterDeviceFileFullPath =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Chemin jusqu'au fichier système représentant l'imprimante thermique"),
										 QString::null,
										 QString::null);

    if (!thermalPrinterDeviceFileFullPath.isEmpty())
    {
        lineEdit_fichier_systeme_imprimante_thermique
			->setText(thermalPrinterDeviceFileFullPath);
    }
}


void YerithAdminWindow::choose_path_latex_system_root_folder()
{
    QString latexSystemRootFolderPath =
    		QFileDialog::getExistingDirectory(this,
    										  QObject::trUtf8("Choisir le chemin jusqu'au répertoire du système de documentation Latex"),
											  QString::null,
											  QFileDialog::ShowDirsOnly);

    if (!latexSystemRootFolderPath.isEmpty())
    {
        lineEdit_repertoire_systeme_latex->setText(latexSystemRootFolderPath);
    }
}


void YerithAdminWindow::choose_repertoire_des_sauvegardes()
{
    QString backupFileDirPath =
    		QFileDialog::getExistingDirectory(this,
    										  QObject::tr("Choisir le chemin qui mène au répertoire des sauvegardes"),
                                              QString::null,
											  QFileDialog::ShowDirsOnly);

    if (!backupFileDirPath.isEmpty())
    {
        lineEdit_repertoire_des_sauvegardes->setText(backupFileDirPath);
    }
}


void YerithAdminWindow::choose_repertoire_fichiers_temporaires()
{
    QString temporaryFileDirPath =
    		QFileDialog::getExistingDirectory(this,
    										  QObject::tr("Choisir le chemin qui mène au répertoire des fichiers temporaires"),
											  QString::null,
											  QFileDialog::ShowDirsOnly);

    if (!temporaryFileDirPath.isEmpty())
    {
        lineEdit_repertoire_fichiers_temporaires
			->setText(temporaryFileDirPath);
    }
}


void YerithAdminWindow::read_configuration()
{
	checkBox_activer_RAW_DB_TABLE_column
		->setChecked(YerithERPConfig::SHOW_DATABASE_RAW);

    checkBox_activer_registre_de_caisse
		->setChecked(YerithERPConfig::ouvrirRegistreDeCaisse);

    lineEdit_devise->setText(YerithERPConfig::currency);

    lineEdit_mariadb_client_base_dir_full_path
		->setText(YerithERPConfig::pathToMARIA_DB_BASE_DIR);

    lineEdit_pdfReader->setText(YerithERPConfig::pathToPdfReader);

    lineEdit_fichier_systeme_imprimante_thermique
		->setText(YerithERPConfig::pathToThermalPrinterDeviceFile);

    lineEdit_repertoire_systeme_latex
		->setText(YerithERPConfig::pathToLatexSystemRootFolder);


    lineEdit_sauvegarde_de_yerith_erp_3_secondes
		->setText(QString::number(YerithERPConfig::backup_restore_yerith_erp_3_time_interval));


    lineEdit_alert_period_time_interval
		->setText(QString::number(YerithERPConfig::alert_period_time_interval));


    lineEdit_alert_quantity_time_interval
		->setText(QString::number(YerithERPConfig::alert_quantity_time_interval));


    lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE
		->setText(QString::number(YerithERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING));


    lineEdit_longueur_maximale_string
		->setText(QString::number(YerithERPConfig::max_string_display_length));


    lineEdit_taille_de_pagination_par_defaut
		->setText(QString::number(YerithERPConfig::pagination_number_of_table_rows));


    lineEdit_MAINTENANCE_VERIFICATION_log_size_max
		->setText(QString::number(YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT));


    lineEdit_repertoire_fichiers_temporaires->setText(YerithERPConfig::temporaryFilesDir);

    lineEdit_tva_value->setText(YerithUtils::getTvaString());


    lineEdit_annee_depart_rapports_chiffre_affaire
		->setText(YerithERPConfig::annee_depart_rapports_chiffre_affaire_value);


    lineEdit_repertoire_des_sauvegardes
		->setText(YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY);


    lineEdit_annee_de_depart_pour_la_pagination
		->setText(YerithERPConfig::annee_depart_pour_la_pagination);


    lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE
		->setText(YerithERPConfig::IP_ADDRESS_NETWORK_printer);


    comboBox_strategie_vente_sortie->clear();

    comboBox_strategie_vente_sortie->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

    comboBox_strategie_vente_sortie->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

    comboBox_strategie_vente_sortie->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

    comboBox_strategie_vente_sortie->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    if (YerithUtils::isEqualCaseInsensitive(YerithERPConfig::salesStrategy,
                                            YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerithUtils::STRATEGIE_ALL_COMBOBOX_INDEX);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::salesStrategy,
              YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerithUtils::STRATEGIE_FEFO_COMBOBOX_INDEX);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::salesStrategy,
              YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerithUtils::STRATEGIE_FIFO_COMBOBOX_INDEX);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::salesStrategy,
              YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerithUtils::STRATEGIE_LIFO_COMBOBOX_INDEX);
    }

    comboBox_impression_sur
		->addItems_AS_INITIALIZATION(YerithERPConfig::printer,
									 YerithERPConfig::printer,
									 comboBox_impression_sur_CHOICE_list);
}


void YerithAdminWindow::handle__tabWidget_maintenance_verification_DATABASE(int current_index)
{
	switch (current_index)
	{
	case 0:
        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(ACTION_GET_runtime_verification_report_PDF, true);

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, true);
		break;

	default:
        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(ACTION_GET_runtime_verification_report_PDF, false);

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);
		break;
	}
}


void YerithAdminWindow::handleThermalPrinterConfiguration(const QString &
                                                          addedPrinterValue)
{
    comboBox_format_de_facture->
    addItems_AS_INITIALIZATION(YerithERPConfig::receiptFormat,
                               YerithERPConfig::receiptFormat,
                               comboBox_format_de_facture_CHOICE_list);

    comboBox_format_de_facture->setYerithEnabled(true);

    if (YerithUtils::isEqualCaseInsensitive
            (YerithUtils::IMPRIMANTE_EPSON_TM_T20ii, addedPrinterValue))
    {
        label_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        label_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (true);

        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (true);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU, addedPrinterValue))
    {
        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->checkField();

        label_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(true);

        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(true);

        label_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);

        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);
    }
    else
    {
        label_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        label_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);

        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);
    }
}


void YerithAdminWindow::read_entreprise_info_database_table(bool
                                                            initalizationValues
                                                            /* = true */)
{
    YerithERPWindows *allWindows = YerithUtils::getAllWindows();

    if (0 != allWindows)
    {
        YerithSqlTableModel &entreprise_info_TableModel =
                        allWindows->getSqlTableModel_entreprise_info();

        int row = 0;

        if (true == initalizationValues)
        {
            QString msgReinit(QObject::tr
                              ("Réinitialiser les données de l'entreprise "
                               "avec les paramètres par défaut ?"));

            if (QMessageBox::Ok == YerithQMessageBox::question(this,
                                                               QObject::tr
                                                               ("réinitialiser"),
                                                               msgReinit,
                                                               QMessageBox::
                                                               Cancel,
                                                               QMessageBox::
                                                               Ok))
            {
                row = 0;
            }
            else
            {
                return;
            }
        }
        else
        {
            row = 1;
        }


        if (false == initalizationValues)
        {
            if (!YerithInfoEntreprise::
                    load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO
                    (label_logo_de_lentreprise))
            {
                label_logo_de_lentreprise->setAutoFillBackground(false);
            }
        }


        QSqlRecord entrepriseInfoRecord =
                        entreprise_info_TableModel.record(row);

        lineEdit_entreprise_denomination_de_lentreprise->setText(QString
                                                                 (GET_SQL_RECORD_DATA
                                                                  (entrepriseInfoRecord,
                                                                   1)));

        lineEdit_entreprise_siege_social->setText(QString
                                                  (GET_SQL_RECORD_DATA
                                                   (entrepriseInfoRecord, 2)));

        lineEdit_entreprise_localisation->setYerithEnabled(false);

        lineEdit_entreprise_localisation->
        setText(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

        lineEdit_entreprise_boite_postale->setText(QString
                                                   (GET_SQL_RECORD_DATA
                                                    (entrepriseInfoRecord,
                                                     4)));

        lineEdit_entreprise_adresse->setText(QString
                                             (GET_SQL_RECORD_DATA
                                              (entrepriseInfoRecord, 5)));

        lineEdit_entreprise_ville->setText(QString
                                           (GET_SQL_RECORD_DATA
                                            (entrepriseInfoRecord, 6)));

        lineEdit_entreprise_pays->setText(QString
                                          (GET_SQL_RECORD_DATA
                                           (entrepriseInfoRecord, 7)));

        lineEdit_entreprise_email->setText(QString
                                           (GET_SQL_RECORD_DATA
                                            (entrepriseInfoRecord, 8)));

        lineEdit_entreprise_telephone->setText(QString
                                               (GET_SQL_RECORD_DATA
                                                (entrepriseInfoRecord, 9)));

        lineEdit_entreprise_numero_de_contribuable->setText(QString
                                                            (GET_SQL_RECORD_DATA
                                                             (entrepriseInfoRecord,
                                                              10)));

        lineEdit_entreprise_secteurs_dactivites->setText(QString
                                                         (GET_SQL_RECORD_DATA
                                                          (entrepriseInfoRecord,
                                                           11)));

        lineEdit_entreprise_agence_du_compte_bancaire->setText(QString
                                                               (GET_SQL_RECORD_DATA
                                                                (entrepriseInfoRecord,
                                                                 12)));

        lineEdit_entreprise_reference_du_compte_bancaire->setText(QString
                                                                  (GET_SQL_RECORD_DATA
                                                                   (entrepriseInfoRecord,
                                                                    13)));

        lineEdit_entreprise_reference_registre_du_commerce->setText(QString
                                                                    (GET_SQL_RECORD_DATA
                                                                     (entrepriseInfoRecord,
                                                                      14)));

        lineEdit_entreprise_denomination_de_lentreprise_SUR_LA_CARTE_DE_FIDELITE_CLIENT->
        setText(GET_SQL_RECORD_DATA
                (entrepriseInfoRecord,
                 YerithDatabaseTableColumn::
                 CARTE_DE_FIDELITE_CLIENT_nom_entreprise));

        lineEdit_entreprise_service_de_gestion_de_la_clientele_SIEGE->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege));

        lineEdit_entreprise_service_de_gestion_de_la_clientele->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerithDatabaseTableColumn::
          CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele));

        lineEdit_entreprise_service_du_programme_de_fidelite_clients->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerithDatabaseTableColumn::
          CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client));

        lineEdit_entreprise_email_du_service_de_gestion_de_la_clientele->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email));

        lineEdit_entreprise_telephone_du_service_de_gestion_de_la_clientele->
        setText(GET_SQL_RECORD_DATA
                (entrepriseInfoRecord,
                 YerithDatabaseTableColumn::
                 CARTE_DE_FIDELITE_CLIENT_telephone));
    }
}


void YerithAdminWindow::read_YERITH_ERP_3_0_SYSTEM_DAEMON_init_configuration()
{
    YerithSqlTableModel &initConfigurationsTableModel =
                    _allWindows->getSqlTableModel_init_configurations();

    //***
    QSqlRecord initConfigurationRecord =
                    initConfigurationsTableModel.record
                    (YerithERPConfig::
                     CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3);

    QString CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3_VALUE
    (GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.record
                    (YerithERPConfig::CONFIG_BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL);

    QString
    BACKUP_RESTORE_DATABASE_YERITH_ERP_3_IntervalValue(GET_SQL_RECORD_DATA
                                                       (initConfigurationRecord,
                                                        "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerithERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);

    QString
    alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA
                                 (initConfigurationRecord,
                                  "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerithERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);

    QString
    alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA
                                   (initConfigurationRecord,
                                    "valeur_configuration"));


    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerithERPConfig::CONFIG_SALES_STRATEGY);

    QString
    salesStrategyValue(GET_SQL_RECORD_DATA
                       (initConfigurationRecord, "valeur_configuration"));


    lineEdit_repertoire_des_sauvegardes->setText
    (CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3_VALUE);

    lineEdit_sauvegarde_de_yerith_erp_3_secondes->setText
    (BACKUP_RESTORE_DATABASE_YERITH_ERP_3_IntervalValue);

    lineEdit_alert_period_time_interval->setText(alertPeriodTimeIntervalValue);

    lineEdit_alert_quantity_time_interval->setText
    (alertQuantityTimeIntervalValue);
}

void YerithAdminWindow::read_app_parameters_init_configuration()
{
    YerithSqlTableModel &initConfigurationsTableModel =
                    _allWindows->getSqlTableModel_init_configurations();

    //***
    QSqlRecord initConfigurationRecord =
                    initConfigurationsTableModel.record(YerithERPConfig::CONFIG_TVA_VALUE);
    QString
    tvaValue(GET_SQL_RECORD_DATA
             (initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerithERPConfig::CONFIG_SALES_STRATEGY);
    QString
    salesStrategyValue(GET_SQL_RECORD_DATA
                       (initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.record(YerithERPConfig::CONFIG_CURRENCY);
    QString
    currencyValue(GET_SQL_RECORD_DATA
                  (initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerithERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR);
    QString
    pageBeginYearValue(GET_SQL_RECORD_DATA
                       (initConfigurationRecord, "valeur_configuration"));

    QString
    userLanguageReceiptFormatValue
    (YerithUtils::getCurrentAdminWindowReceiptsFormatAccordingToLanguage
     (YerithERPConfig::receiptFormat));

    /*
     * SHOW DATABASE table column RAW INITIAL CONFIGURATION
     */
    if (YerithERPConfig::SHOW_DATABASE_RAW)
    {
    	checkBox_activer_RAW_DB_TABLE_column->setChecked(true);
    }
    else
    {
    	checkBox_activer_RAW_DB_TABLE_column->setChecked(false);
    }

    /*
     * OPEN_CASH_DRAWER INITIALIZATION CONFIGURATION
     */
    if (YerithERPConfig::ouvrirRegistreDeCaisse)
    {
        checkBox_activer_registre_de_caisse->setChecked(true);
    }
    else
    {
        checkBox_activer_registre_de_caisse->setChecked(false);
    }

    /*
     * PRINTER INITIALIZATION CONFIGURATION
     */
    comboBox_impression_sur->
    addItems_AS_INITIALIZATION(YerithERPConfig::printer,
                               YerithUtils::IMPRIMANTE_PDF,
                               comboBox_impression_sur_CHOICE_list);

    comboBox_format_de_facture->addItems_AS_INITIALIZATION
    (userLanguageReceiptFormatValue, userLanguageReceiptFormatValue,
     comboBox_format_de_facture_CHOICE_list);

    lineEdit_devise->setText(currencyValue);

    lineEdit_mariadb_client_base_dir_full_path->
    setText(YerithERPConfig::pathToMARIA_DB_BASE_DIR);

    lineEdit_pdfReader->setText(YerithERPConfig::pathToPdfReader);

    lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setText
    (QString::number
     (YerithERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING));

    lineEdit_longueur_maximale_string->
    setText(QString::number(YerithERPConfig::max_string_display_length));

    lineEdit_taille_de_pagination_par_defaut->
		setText(QString::number(YerithERPConfig::pagination_number_of_table_rows));

    lineEdit_MAINTENANCE_VERIFICATION_log_size_max->
		setText(QString::number(YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT));

    lineEdit_fichier_systeme_imprimante_thermique->
    setText(YerithERPConfig::pathToThermalPrinterDeviceFile);

    lineEdit_repertoire_systeme_latex->
    setText(YerithERPConfig::pathToLatexSystemRootFolder);

    lineEdit_repertoire_fichiers_temporaires->
    setText(YerithERPConfig::temporaryFilesDir);

    lineEdit_tva_value->setText(tvaValue);

    if (!pageBeginYearValue.isEmpty())
    {
        lineEdit_annee_de_depart_pour_la_pagination->setText
        (pageBeginYearValue);
    }
    else
    {
        lineEdit_annee_de_depart_pour_la_pagination->setText
        (GET_CURRENT_DATE_YEAR);
    }

    lineEdit_annee_depart_rapports_chiffre_affaire->
    setText(YerithERPConfig::annee_depart_rapports_chiffre_affaire_value);

    {
        comboBox_strategie_vente_sortie->clear();
        //qDebug() << "++ test";
        comboBox_strategie_vente_sortie->
        addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL);
        comboBox_strategie_vente_sortie->
        addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);
        comboBox_strategie_vente_sortie->
        addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
        comboBox_strategie_vente_sortie->
        addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

        int salesStrategyId =
                        comboBox_strategie_vente_sortie->findText(salesStrategyValue);

        comboBox_strategie_vente_sortie->setCurrentIndex(salesStrategyId);
    }
}

void YerithAdminWindow::reinitialiser_YERITH_ERP_3_0_SYSTEM_DAEMON_configuration()
{
    _logger->log("reinitialiser_configuration");

    QString msgReinit(QObject::tr
                      ("Réinitialiser la configuration du 'SYSTEM DAEMON' "
                       "avec les paramètres par défaut ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this, QObject::tr("réinitialiser"),
                                        msgReinit, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        read_YERITH_ERP_3_0_SYSTEM_DAEMON_init_configuration();
    }
    else
    {
    }
}

void YerithAdminWindow::reinitialiser_app_parameters_configuration()
{
    _logger->log("reinitialiser_configuration");

    QString
    msgReinit(QObject::tr
              ("Réinitialiser la configuration (générale) "
               "avec les paramètres par défaut ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this, QObject::tr("réinitialiser"),
                                        msgReinit, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        read_app_parameters_init_configuration();
    }
    else
    {
    }
}


void YerithAdminWindow::enregistrer_entreprise_info_database_table()
{
    _logger->log("enregistrer_entreprise_info_database_table");

    QString
    msgEnregistrer(QObject::tr
                   ("Enregistrer les données de l'entreprise (modifiées) ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("enregistrer"),
                                        msgEnregistrer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        YerithSqlTableModel &entreprise_info_TableModel =
                        _allWindows->getSqlTableModel_entreprise_info();

        QSqlRecord aNewEntrepriseInfoRecord =
                        entreprise_info_TableModel.record(1);

        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                 lineEdit_entreprise_denomination_de_lentreprise->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::SIEGE_SOCIAL,
                 lineEdit_entreprise_siege_social->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::LOCALISATION,
                 YerithERPConfig::THIS_SITE_LOCALISATION_NAME);
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::BOITE_POSTALE,
                 lineEdit_entreprise_boite_postale->text());
        aNewEntrepriseInfoRecord.setValue(YerithDatabaseTableColumn::ADRESSE,
                                          lineEdit_entreprise_adresse->
                                          text());
        aNewEntrepriseInfoRecord.setValue(YerithDatabaseTableColumn::VILLE,
                                          lineEdit_entreprise_ville->text());
        aNewEntrepriseInfoRecord.setValue(YerithDatabaseTableColumn::PAYS,
                                          lineEdit_entreprise_pays->text());
        aNewEntrepriseInfoRecord.setValue(YerithDatabaseTableColumn::EMAIL,
                                          lineEdit_entreprise_email->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1,
                 lineEdit_entreprise_telephone->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE,
                 lineEdit_entreprise_numero_de_contribuable->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::SECTEURS_DACTIVITES,
                 lineEdit_entreprise_secteurs_dactivites->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::AGENCE_DU_COMPTE_BANCAIRE,
                 lineEdit_entreprise_agence_du_compte_bancaire->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
                 lineEdit_entreprise_reference_du_compte_bancaire->text());
        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
                 lineEdit_entreprise_reference_registre_du_commerce->
                 text());

        aNewEntrepriseInfoRecord.setValue
        (YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_nom_entreprise,
         lineEdit_entreprise_denomination_de_lentreprise_SUR_LA_CARTE_DE_FIDELITE_CLIENT->
         text());

        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege,
                 lineEdit_entreprise_service_de_gestion_de_la_clientele_SIEGE->
                 text());

        aNewEntrepriseInfoRecord.setValue
        (YerithDatabaseTableColumn::
         CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele,
         lineEdit_entreprise_service_de_gestion_de_la_clientele->text());

        aNewEntrepriseInfoRecord.setValue
        (YerithDatabaseTableColumn::
         CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client,
         lineEdit_entreprise_service_du_programme_de_fidelite_clients->
         text());

        aNewEntrepriseInfoRecord.
        setValue(YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email,
                 lineEdit_entreprise_email_du_service_de_gestion_de_la_clientele->
                 text());

        aNewEntrepriseInfoRecord.setValue
        (YerithDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_telephone,
         lineEdit_entreprise_telephone_du_service_de_gestion_de_la_clientele->
         text());

        if (label_logo_de_lentreprise->pixmap())
        {
            QByteArray bytes;
            YerithUtils::savePixmapToByteArray(bytes,
                                               *label_logo_de_lentreprise->
                                               pixmap(), "JPG");
            aNewEntrepriseInfoRecord.
            setValue(YerithDatabaseTableColumn::LOGO_ENTREPRISE, bytes);
        }

        bool recordUpdated =
                        entreprise_info_TableModel.updateRecord(1,
                        										aNewEntrepriseInfoRecord,
																"src/admin/yerith-erp-admin-window.cpp",
																3000);

        QString retMesg;

        if (true == recordUpdated)
        {
            retMesg =
                            QObject::tr("Les données commerciales de l'entreprise "
                                            "ont été enregistrées avec succès !");

            YerithQMessageBox::information(this, QObject::tr("succès"),
                                           retMesg);
        }
        else
        {
            retMesg =
                            QObject::tr
                            ("Échec de l'enregistrement des données commerciales de l'entreprise !");

            if (QMessageBox::Ok ==
                    YerithQMessageBox::warning(this, QObject::tr("échec"),
                                               retMesg))
            {
            }
            else
            {
            }
        }
    }
    else
    {
    }

    _allWindows->getInfoEntreprise().toString();
}


void YerithAdminWindow::enregistrer_YERITH_ERP_3_0_SYSTEM_DAEMON_configuration()
{
    _logger->log("enregistrer_YERITH_ERP_3_0_SYSTEM_DAEMON_configuration");

    QString
    msgEnregistrer(QObject::tr
                   ("Enregistrer la configuration (SYSTEM DAEMON) modifiée ?"));
    if (QMessageBox::Cancel ==
            YerithQMessageBox::question(this, QObject::tr("enregistrer"),
                                        msgEnregistrer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        return;
    }
    else
    {
        YerithSqlTableModel &configurationsTableModel =
                        _allWindows->getSqlTableModel_configurations();

        bool success = false;

        QSqlRecord configurationsRecord;

        if (lineEdit_sauvegarde_de_yerith_erp_3_secondes->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.record
                            (YerithERPConfig::
                             CONFIG_BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_sauvegarde_de_yerith_erp_3_secondes->
                                          text());

            success = success ||
                      configurationsTableModel.updateRecord
                      	  (YerithERPConfig::CONFIG_BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL,
                      	   configurationsRecord,
						   "src/admin/yerith-erp-admin-window.cpp",
						   3075);

            if (success)
            {
                YerithERPConfig::backup_restore_yerith_erp_3_time_interval =
                                lineEdit_sauvegarde_de_yerith_erp_3_secondes->text().
                                toUInt();
            }
        }

        if (lineEdit_repertoire_des_sauvegardes->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.record
                            (YerithERPConfig::
                             CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_repertoire_des_sauvegardes->
                                          text());

            bool success = configurationsTableModel.updateRecord
                           	   (YerithERPConfig::CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3,
                           	    configurationsRecord,
								"src/admin/yerith-erp-admin-window.cpp",
								3101);

            if (success)
            {
                YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY =
                                lineEdit_repertoire_des_sauvegardes->text();
            }
        }

        if (lineEdit_alert_period_time_interval->checkField())
        {
            configurationsRecord.clear();

            configurationsRecord =
                            configurationsTableModel.
                            record(YerithERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_alert_period_time_interval->
                                          text());

            success = success ||
                      configurationsTableModel.updateRecord(YerithERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL,
                                   	   	   	   	   	   	    configurationsRecord,
															"src/admin/yerith-erp-admin-window.cpp",
															3126);

            if (success)
            {
                YerithERPConfig::alert_period_time_interval =
                                lineEdit_alert_period_time_interval->text().toUInt();
            }
        }

        if (lineEdit_alert_quantity_time_interval->checkField())
        {
            configurationsRecord.clear();

            configurationsRecord =
                            configurationsTableModel.
                            record(YerithERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_alert_quantity_time_interval->
                                          text());

            success = success ||
                      configurationsTableModel.updateRecord
                      	  (YerithERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL,
                      	   configurationsRecord,
						   "src/admin/yerith-erp-admin-window.cpp",
						   3151);

            if (success)
            {
                YerithERPConfig::alert_quantity_time_interval =
                                lineEdit_alert_quantity_time_interval->text().toUInt();
            }
        }

//        QDEBUG_STRING_OUTPUT_2("success", BOOL_TO_STRING(success));

        if (success)
        {
            stop_alert_daemon_process();

            start_alert_daemon_process();
        }
    }
}


void YerithAdminWindow::enregistrer_system_local_app_parameters_configuration()
{
    _logger->log("enregistrer_system_local_app_parameters_configuration");

    if (checkBox_activer_RAW_DB_TABLE_column->isChecked())
    {
        YerithERPConfig::SHOW_DATABASE_RAW = true;
    }
    else
    {
        YerithERPConfig::SHOW_DATABASE_RAW = false;
    }

    if (checkBox_activer_registre_de_caisse->isChecked())
    {
        YerithERPConfig::ouvrirRegistreDeCaisse = true;
    }
    else
    {
        YerithERPConfig::ouvrirRegistreDeCaisse = false;
    }

    if (lineEdit_fichier_systeme_imprimante_thermique->checkField())
    {
        YerithERPConfig::pathToThermalPrinterDeviceFile =
                        lineEdit_fichier_systeme_imprimante_thermique->text();
    }

    if (lineEdit_repertoire_systeme_latex->checkField())
    {
        YerithERPConfig::pathToLatexSystemRootFolder =
                        lineEdit_repertoire_systeme_latex->text();
    }

    if (lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->checkField
            ())
    {
        YerithERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING =
                        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->text().
                        toInt();
    }

    if (lineEdit_longueur_maximale_string->checkField())
    {
        YerithERPConfig::max_string_display_length =
                        lineEdit_longueur_maximale_string->text().toInt();
    }

    if (lineEdit_taille_de_pagination_par_defaut->checkField())
    {
        YerithERPConfig::pagination_number_of_table_rows =
                        lineEdit_taille_de_pagination_par_defaut->text().toInt();
    }

    if (lineEdit_MAINTENANCE_VERIFICATION_log_size_max->checkField())
    {
    	YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
    			lineEdit_MAINTENANCE_VERIFICATION_log_size_max->text().toInt();

    	YERITH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
    			YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT;
    }

    if (lineEdit_mariadb_client_base_dir_full_path->checkField())
    {
        YerithERPConfig::pathToMARIA_DB_BASE_DIR =
                        lineEdit_mariadb_client_base_dir_full_path->text();
    }

    if (lineEdit_pdfReader->checkField())
    {
        YerithERPConfig::pathToPdfReader = lineEdit_pdfReader->text();
    }

    if (comboBox_format_de_facture->checkField())
    {
        YerithUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage
        (comboBox_format_de_facture->currentText());
    }

    if (lineEdit_repertoire_fichiers_temporaires->checkField())
    {
        YerithERPConfig::temporaryFilesDir =
                        lineEdit_repertoire_fichiers_temporaires->text();
    }

    if (lineEdit_annee_depart_rapports_chiffre_affaire->checkField())
    {
        YerithERPConfig::annee_depart_rapports_chiffre_affaire_value =
                        lineEdit_annee_depart_rapports_chiffre_affaire->text();
    }

    if (comboBox_impression_sur->checkField())
    {
        YerithERPConfig::printer = comboBox_impression_sur->currentText();

        if (YerithUtils::isEqualCaseInsensitive(YerithERPConfig::printer,
                                                YerithUtils::
                                                IMPRIMANTE_EPSON_TM_T20ii_RESEAU))
        {
            YerithERPConfig::IP_ADDRESS_NETWORK_printer =
                            lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->text();
        }
    }

    YerithERPConfig::saveYerithConfig();
}


void YerithAdminWindow::enregistrer_app_parameters_configuration()
{
    _logger->log("enregistrer_app_parameters_configuration");

    QString msgEnregistrer(QObject::tr
                           ("Enregistrer la configuration (générale) modifiée ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this, QObject::tr("enregistrer"),
                                        msgEnregistrer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        enregistrer_system_local_app_parameters_configuration();

        YerithSqlTableModel &configurationsTableModel =
                        _allWindows->getSqlTableModel_configurations();

        QSqlRecord configurationsRecord;

        if (lineEdit_devise->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerithERPConfig::CONFIG_CURRENCY);
            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_devise->text());

            bool success = configurationsTableModel
            					.updateRecord(YerithERPConfig::CONFIG_CURRENCY,
            							      configurationsRecord,
											  "src/admin/yerith-erp-admin-window.cpp",
											  3313);

            if (success)
            {
                YerithERPConfig::currency = lineEdit_devise->text();
            }
        }

        if (lineEdit_annee_de_depart_pour_la_pagination->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerithERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR);
            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_annee_de_depart_pour_la_pagination->
                                          text());

            bool success = configurationsTableModel
            					.updateRecord(YerithERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR,
                                         	  configurationsRecord,
											  "src/admin/yerith-erp-admin-window.cpp",
											  3334);

            if (success)
            {
                YerithERPConfig::annee_depart_pour_la_pagination =
                                lineEdit_annee_de_depart_pour_la_pagination->text();
            }
        }

        if (lineEdit_tva_value->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerithERPConfig::CONFIG_TVA_VALUE);
            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_tva_value->text());

            bool success = configurationsTableModel
            				.updateRecord(YerithERPConfig::CONFIG_TVA_VALUE,
                                          configurationsRecord,
										  "src/admin/yerith-erp-admin-window.cpp",
										  3355);
            if (success)
            {
                YerithERPConfig::tva_value =
                                (lineEdit_tva_value->text().toDouble() / 100.0);
            }
        }

        if (comboBox_strategie_vente_sortie->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerithERPConfig::CONFIG_SALES_STRATEGY);
            configurationsRecord.setValue("valeur_configuration",
                                          comboBox_strategie_vente_sortie->
                                          currentText());

            bool success = configurationsTableModel
            					.updateRecord(YerithERPConfig::CONFIG_SALES_STRATEGY,
                                         	  configurationsRecord,
											  "src/admin/yerith-erp-admin-window.cpp",
											  3376);

            if (success)
            {
                YerithERPConfig::salesStrategy =
                                comboBox_strategie_vente_sortie->currentText();
            }
        }

#ifdef YERITH_SERVER
        YERITH_RUNTIME_VERIFIER *dbusServer = _allWindows->dbusServer();
        if (dbusServer)
        {
            //dbusServer->emit_signal_refresh_app_parameters();
            qDebug() << "YerithAdminWindow application "
            			"parameters have been changed on the server";
        }
#endif

    }
    else
    {
    }
}


void YerithAdminWindow::REFRESH_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS()
{
    _logger
        ->log("yerith-erp-3-0 | YerithAdminWindow::check_Yr_Db_Runtime_Verif_SERVICE_Running");


    comboBox_QT_DBUS_ERROR_DB_RUNTIME_VERIFICATOR
        ->populateComboBoxRawString(YerithDatabase::YRI_DB_RUNTIME_VERIF,
                                    YerithDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME);


    comboBox_QT_DBUS_ERROR_DB_RUNTIME_VERIFICATOR->setCurrentIndex(1);


    bool started =
    		YerithProcessInfo::check_Yr_Db_Runtime_Verif_Daemon_Running(_logger);

    if (started)
    {
        //qDebug() << "++ OK 3: " << SET_COLOUR_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS_BUTTONS ;
    	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(true);
    	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(false);
    }
    else
    {
        //qDebug() << "++ OK pas demarre: ";
    	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(false);
    	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(true);
    }
}


void YerithAdminWindow::START_YRI_DB_RUNTIME_VERIF_GUI()
{
    YerithERPProcess::START_yri_db_runtime_verif_GUI();

	REFRESH_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS();
}


void YerithAdminWindow::START_YRI_DB_RUNTIME_VERIF_DAEMON_PROCESS()
{
    YerithERPProcess::START_yri_db_runtime_verif();

	REFRESH_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS();
}


void YerithAdminWindow::STOP_YRI_DB_RUNTIME_VERIF_DAEMON_PROCESS()
{
    _logger->log("STOP_YRI_DB_RUNTIME_VERIF_DAEMON_PROCESS");

    YerithERPProcess::STOP_yri_db_runtime_verif();

	REFRESH_STATUS_OF_YRI_DB_RUNTIME_VERIF_PROCESS();
}


void YerithAdminWindow::set_colour_status_of_alert_daemon_process_buttons()
{
    _logger->log
    ("yerith-erp-3-0 | YerithAdminWindow::check_alert_daemon_process");

    bool started =
                    YerithProcessInfo::checkYerithERPAlertDaemonRunning(_logger);

    if (started)
    {
        //qDebug() << "++ OK 3: " << checkAlertDeamonProcessOutput ;
        label_ON->setVisible(true);
        label_OFF->setVisible(false);
    }
    else
    {
        //qDebug() << "++ OK pas demarre: ";
        label_ON->setVisible(false);
        label_OFF->setVisible(true);
    }
}


void YerithAdminWindow::start_alert_daemon_process()
{
    _logger->log("start_alert_daemon_process");

    YerithERPProcess::startYerithERPAlertProcess();

    set_colour_status_of_alert_daemon_process_buttons();
}

void YerithAdminWindow::stop_alert_daemon_process()
{
    _logger->log("stop_alert_daemon_process");

    YerithERPProcess::stopYerithERPAlertProcess();

    set_colour_status_of_alert_daemon_process_buttons();
}


void YerithAdminWindow::set_localisation_adresse_ip_text()
{
    YerithSqlTableModel &localisationSqlTableModel =
                    _allWindows->getSqlTableModel_localisations();

    localisationSqlTableModel.yerithSetFilter_WITH_where_clause
    (GENERATE_SQL_IS_STMT
     (YerithDatabaseTableColumn::NOM_LOCALISATION,
      comboBox_localisation->currentText()));

    int localisationSqlTableModelRowCount =
    		localisationSqlTableModel.easySelect("src/admin/yerith-erp-admin-window.cpp", 3486);

    if (localisationSqlTableModelRowCount > 0)
    {
        QSqlRecord localisationRecord = localisationSqlTableModel.record(0);

        _localisation_adresse_ip =
                        GET_SQL_RECORD_DATA(localisationRecord, "adresse_ip");

        lineEdit_localisation_adresse_ip->setText(_localisation_adresse_ip);
    }

    //qDebug() << "++localisation_adresse_ip: " << _localisation_adresse_ip;
    localisationSqlTableModel.resetFilter();
}


void YerithAdminWindow::connecter_localisation_db()
{
    if (YerithUtils::slot_connecter_localisation(*this,
                                                 _allWindows,
                                                 comboBox_localisation->
                                                 currentText()))
    {
        label_ONLINE->setVisible(true);
        label_OFFLINE->setVisible(false);
    }
    else
    {
        comboBox_localisation->resetYerithComboBox();

        lineEdit_localisation_adresse_ip->clear();

        label_ONLINE->setVisible(false);
        label_OFFLINE->setVisible(true);
    }

    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();
}

void YerithAdminWindow::deconnecter_localisation_db()
{
    YerithUtils::slot_deconnecter_localisation(_allWindows);

    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();

    comboBox_localisation->resetYerithComboBox();

    lineEdit_localisation_adresse_ip->clear();

    label_ONLINE->setVisible(false);
    label_OFFLINE->setVisible(true);
}
