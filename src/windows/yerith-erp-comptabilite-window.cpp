/*
 * yerith-erp-comptabilite-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-comptabilite-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-user-administrateur.hpp"

#include "src/users/yerith-erp-user-caissier.hpp"

#include "src/users/yerith-erp-user-magasinier.hpp"

#include "src/users/yerith-erp-user-manager.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/utils/yerith-erp-info-entreprise.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/dialogs/yerith-erp-changer-utilisateur-dialog.hpp"

#include "src/dialogs/yerith-erp-generic-select-db-field-dialog.hpp"

#include <QtSql/QSqlField>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerithComptabiliteWindow::YerithComptabiliteWindow()
:YerithQTableWidgetWindowsCommons("yerith-erp-listing-comptabilite"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::LIGNES_BUDGETAIRES),
 _logger(new YerithLogger("YerithComptabiliteWindow")),
 _pushButton_comptabilite_filtrer_font(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("comptabilité"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    _a_CURRENT_table_widget = tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE;


    checkBox_EXACT_STRING_MATCH->setChecked(true);


#ifdef YERITH_FEATURES_COMPTABILITE_yes

    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_comptes_doperations_comptables);


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_comptes_doperations_comptables);

//    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
//              << pushButton_afficher;

    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
        << actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF;

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_GREEN_2_160_170,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    setup_select_configure_dbcolumn(YerithDatabase::LIGNES_BUDGETAIRES);

    _lineEditsToANDContentForSearch.insert(&lineEdit_comptabilite_terme_recherche,
                                           YerithUtils::EMPTY_STRING);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerithDatabase::LIGNES_BUDGETAIRES);

    reinitialiser_colones_db_visibles();

    setupLineEdits();

    setupDateTimeEdits();

    populateComboBoxes();

    _pushButton_comptabilite_filtrer_font =
                    new QFont(pushButton_comptabilite_filtrer->font());

    tableView_comptes_doperations_comptables
    	->setSqlTableName(&YerithDatabase::LIGNES_BUDGETAIRES);


    YerithQTableWidgetWindowsCommons::
        connect_Qtable_Widgets(*tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_Dissocier_TYPE_DOPERATION__compte_doperation
        ->disable(this);

    pushButton_ASSOCIER_TYPE_DOPERATION__compte_doperation
        ->disable(this);


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(handle_filtres_checkBox(int)));


    connect(tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE,
            SIGNAL(clicked(const QModelIndex &)),
            this,
            SLOT(ON_CLICKED_SELECT_COMMERCIAL_OPERATION_TYPE(const QModelIndex &)));


    connect(tableView_comptes_doperations_comptables,
    		SIGNAL(clicked(const QModelIndex &)),
            this,
			SLOT(ON_CLICKED_Show_paid_in_paid_out_VALUES_amount(const QModelIndex &)));


    connect(tabWidget_creer,
            SIGNAL(currentChanged(int)),
            this,
            SLOT(handle_change_tab(int)));

    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));

    connect(action_IMPRIMER_UNE_LISTE_DOPERATIONS_COMMERCIALES_JUSQUAU_selectionne,
            SIGNAL(triggered()),
            this,
            SLOT(imprimer_UNE_LISTE_JUSQUAU_selectionne_WITH_A_YERITH_PROGRESS_BAR()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));

    connect(actionReinitialiserChampsDBVisible,
    		SIGNAL(triggered()),
			this,
			SLOT(slot_reinitialiser_colones_db_visibles()));
    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

    setupShortcuts();


    handle_Enable_Disable_BUTTONS();


#endif //#ifdef YERITH_FEATURES_COMPTABILITE_yes
}


YerithComptabiliteWindow::~YerithComptabiliteWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _logger;
}


void YerithComptabiliteWindow::
        afficher_comptes_bancaires(YerithSqlTableModel &aYerithSqlTableModel)
{
    tableView_comptes_doperations_comptables
        ->queryYerithTableViewCurrentPageContentRow(aYerithSqlTableModel);


    SLOT_calculer_TOUTES_les_lignes_budgetaires();


    tableView_show_or_hide_columns(*tableView_comptes_doperations_comptables);

    int rowCount =
        tableView_comptes_doperations_comptables
            ->getYerithSqlTableModelTotalRowCount();

    int last_selected_trow_ID = getLastListerSelectedRow__ID_AS_INTEGER();

    ON_CLICKED_Show_paid_in_paid_out_VALUES_amount(last_selected_trow_ID);

    lineEdit_nombre_de_comptes_bancaires->setText(GET_NUM_STRING(rowCount));
}


void YerithComptabiliteWindow::
        afficher_ASSOCIATIONS_OperationsCommerciales__compte_bancaires()
{
    tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
        ->yerithClearTableWidgetContent();


    YerithSqlTableModel &type_doperation_SqlTbleModel =
        _allWindows->getSqlTableModel_types_doperations_financieres();


	int rows = type_doperation_SqlTbleModel
                .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 250);


    QString Record_type_doperation_DB_ID;

    QString Record_type_de_Credit_Debit;

    QString Record_type_de_Credit_Debit_CODE;

    QString Record_type_doperation_financiere;

    QString Record_type_doperation_financiere_CODE;

    QString Record_INTITULE_LigneBudgetaire;


    int last_current_row = 0;

    for (int j = 0; j < rows; ++j)
    {
		QSqlRecord record = type_doperation_SqlTbleModel.record(j);

        Record_type_doperation_DB_ID =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::ID);

        Record_type_de_Credit_Debit_CODE =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::TYPE_DE_CREDIT_DEBIT);

        Record_type_doperation_financiere_CODE =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE);

        Record_INTITULE_LigneBudgetaire =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE);


        Record_type_de_Credit_Debit =
            YerithUtils::_typedecreditdebitToUserViewString
                .value(Record_type_de_Credit_Debit_CODE.toInt());


        Record_type_doperation_financiere =
            YerithUtils::_typedoperationfinancieresToUserViewString
                .value(Record_type_doperation_financiere_CODE.toInt());


        last_current_row =
            tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
                ->add_an_association(Record_type_doperation_DB_ID,
                                     Record_type_doperation_financiere,
                                     Record_type_de_Credit_Debit,
                                     Record_INTITULE_LigneBudgetaire);


        tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
            ->selectRow(last_current_row);
    }


    int tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE__RowCount =
            tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE->rowCount();


    if (tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE__RowCount > 0)
    {
        enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
    }
    else
    {
        if (tableView_comptes_doperations_comptables <= 0)
        {
            disable_yerith_widgets();
        }
    }
}


void YerithComptabiliteWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE
            << YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire
            << YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire;
}


void YerithComptabiliteWindow::hideEvent(QHideEvent *hideEvent)
{
    _selectExportDBQDialog->close();
}


void YerithComptabiliteWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (FINANCIAL_Operation_Type__Financial_Operation_Type_Account ==
            tabWidget_creer->currentIndex())
    {
        QMenu menu(this);
        menu.setPalette(toolBar_comptabilite_Window->palette());
        menu.addAction(action_IMPRIMER_UNE_LISTE_DOPERATIONS_COMMERCIALES_JUSQUAU_selectionne);
        menu.exec(event->globalPos());
    }
}


void YerithComptabiliteWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


double YerithComptabiliteWindow::GET_Human_RESOURCE_EXPENSE_Amount()
{
    double DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount = 0.0;


    YerithSqlTableModel &paiements_SqlTableModel =
        _allWindows->getSqlTableModel_paiements();


    QString ONLY_HUMAN_RESOURCES_filtering_STRING =
        QString("(%1 = 1)")
          .arg(YerithDatabaseTableColumn::IS_PAYEMENT_HR_HR);


    paiements_SqlTableModel
        .yerithSetFilter_WITH_where_clause
            (ONLY_HUMAN_RESOURCES_filtering_STRING);


    int row_COUNT_DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES =
        paiements_SqlTableModel
            .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 393);


    double montant_paye_a_LEMPLOYE = 0.0;


    int type_de_paiement_DECAISSEMENT =
        YerithUtils::DECAISSEMENT_INDEFINI;


    QSqlField QSQLFIELD_compte_fournisseur;

    QSqlRecord record_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES;

    double TEMP_DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount = 0.0;

    for (int k = 0; k < row_COUNT_DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES; ++k)
    {
        record_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES =
            paiements_SqlTableModel.record(k);

        type_de_paiement_DECAISSEMENT =
            GET_SQL_RECORD_DATA(record_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES,
                                YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt();

        QSQLFIELD_compte_fournisseur =
            record_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES
                .field(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);


        if (QSQLFIELD_compte_fournisseur.isNull())
        {
            continue;
        }


        montant_paye_a_LEMPLOYE =
            YerithUtils::montant_paye_au_fournisseur_OU_Employe
                (record_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES);


        if (montant_paye_a_LEMPLOYE < 0)
        {
            DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount =
                DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount +
                    montant_paye_a_LEMPLOYE;
        }
    }


    paiements_SqlTableModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 445);


    return DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount;
}


double YerithComptabiliteWindow::GET_FINANCIAL_EXPENSE_Amount()
{
    double DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount = 0.0;


    YerithSqlTableModel &paiements_SqlTableModel =
        _allWindows->getSqlTableModel_paiements();


    int row_COUNT_DEBIT_sortie_dargent_CHARGES_FINANCIERES =
        paiements_SqlTableModel
            .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 315);


    double montant_paye_au_fournisseur = 0.0;


    int type_de_paiement_DECAISSEMENT =
        YerithUtils::DECAISSEMENT_INDEFINI;


    QSqlField QSQLFIELD_compte_fournisseur;

    QSqlRecord record_sortie_dargent_CHARGES_FINANCIERES;

    double TEMP_DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount = 0.0;

    for (int k = 0; k < row_COUNT_DEBIT_sortie_dargent_CHARGES_FINANCIERES; ++k)
    {
        record_sortie_dargent_CHARGES_FINANCIERES =
            paiements_SqlTableModel.record(k);

        type_de_paiement_DECAISSEMENT =
            GET_SQL_RECORD_DATA(record_sortie_dargent_CHARGES_FINANCIERES,
                                YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt();

        QSQLFIELD_compte_fournisseur =
            record_sortie_dargent_CHARGES_FINANCIERES
                .field(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);


        if (QSQLFIELD_compte_fournisseur.isNull())
        {
            continue;
        }


        montant_paye_au_fournisseur =
            YerithUtils::montant_paye_au_fournisseur_OU_Employe
                (record_sortie_dargent_CHARGES_FINANCIERES);


        if (montant_paye_au_fournisseur < 0)
        {
            DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount =
                DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount +
                    montant_paye_au_fournisseur;
        }
    }


    paiements_SqlTableModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 367);


    return DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount;
}


double YerithComptabiliteWindow::GET_asset_PURCHASE_Amount()
{
    double DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount = 0.0;


    YerithSqlTableModel &achats_SqlTableModel =
        _allWindows->getSqlTableModel_achats();


    //Assets are databse tablöe entries where a selling
    //price is equal to 0.
    QString QUERY_ASSET_purchase_string_filter =
        QString("%1 = '0'")
          .arg(YerithDatabaseTableColumn::PRIX_VENTE);


    achats_SqlTableModel
        .yerithSetFilter_WITH_where_clause(QUERY_ASSET_purchase_string_filter);


    int row_COUNT_Debit_SORTIE_dargent =
        achats_SqlTableModel
            .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 425);


    QSqlRecord record;

    double quantite = 0.0;
    double prix_dachat = 0.0;
    double montant_total_dachat = 0.0;

    double TEMP_DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount = 0.0;

    for (int k = 0; k < row_COUNT_Debit_SORTIE_dargent; ++k)
    {
        record = achats_SqlTableModel.record(k);

        quantite =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();

        prix_dachat =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::PRIX_DACHAT).toDouble();

        TEMP_DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount =
            (quantite * prix_dachat);


        DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount =
            DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount +
                TEMP_DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount;
    }

    achats_SqlTableModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 460);


    DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount =
        -1.0 * DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount;


    return DEBIT_sortie_dargent_Achat_DE_MATERIELS_Immobilisations_Amount;
}


double YerithComptabiliteWindow::GET_merchandise_PURCHASE_Amount()
{
    double DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount = 0.0;


    YerithSqlTableModel &achats_SqlTableModel =
        _allWindows->getSqlTableModel_achats();


    QString QUERY_merchandise_purchase_string_filter =
        QString("%1 != '0'")
          .arg(YerithDatabaseTableColumn::PRIX_VENTE);


    achats_SqlTableModel
        .yerithSetFilter_WITH_where_clause(QUERY_merchandise_purchase_string_filter);


    int row_COUNT_Debit_SORTIE_dargent =
        achats_SqlTableModel
            .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 384);


    QSqlRecord record;

    double quantite = 0.0;
    double prix_dachat = 0.0;
    double montant_total_dachat = 0.0;

    double TEMP_DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount = 0.0;

    for (int k = 0; k < row_COUNT_Debit_SORTIE_dargent; ++k)
    {
        record = achats_SqlTableModel.record(k);

        quantite =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();

        prix_dachat =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::PRIX_DACHAT).toDouble();

        TEMP_DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount =
            (quantite * prix_dachat);


        DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount =
            DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount +
                TEMP_DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount;
    }



    achats_SqlTableModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 420);


    DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount =
        -1.0 * DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount;


    return DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount;
}


double YerithComptabiliteWindow::GET_sales_cash_AND_sales_client_account_Amount()
{
    double CREDIT_entree_dargent_Vente_Comptant_Amount = 0.0;


    YerithSqlTableModel &stocks_vendu_SqlTbleModel =
        _allWindows->getSqlTableModel_stocks_vendu();


    QString QUERY_sales_STOCKS_VENDU_string_filter =
        QString("%1 = '%2' OR %3 = '%4'")
        .arg(YerithDatabaseTableColumn::TYPE_DE_VENTE,
             QString::number(YerithUtils::VENTE_COMPTANT),
             YerithDatabaseTableColumn::TYPE_DE_VENTE,
             QString::number(YerithUtils::VENTE_COMPTE_CLIENT));


    stocks_vendu_SqlTbleModel
        .yerithSetFilter_WITH_where_clause(QUERY_sales_STOCKS_VENDU_string_filter);


    int row_COUNT_credit_entree_dargent =
        stocks_vendu_SqlTbleModel
            .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 449);


    QSqlRecord record;

    double TEMP_CREDIT_entree_dargent_Vente_Comptant_Amount = 0.0;

    for (int k = 0; k < row_COUNT_credit_entree_dargent; ++k)
    {
        record = stocks_vendu_SqlTbleModel.record(k);

        TEMP_CREDIT_entree_dargent_Vente_Comptant_Amount =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();

        CREDIT_entree_dargent_Vente_Comptant_Amount =
            CREDIT_entree_dargent_Vente_Comptant_Amount +
            TEMP_CREDIT_entree_dargent_Vente_Comptant_Amount;
    }


    stocks_vendu_SqlTbleModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 472);


    return CREDIT_entree_dargent_Vente_Comptant_Amount;
}


bool YerithComptabiliteWindow::
        ON_CLICKED_Show_paid_in_paid_out_VALUES_amount(int a_modelIndex_ROW)
{
    bool ret_showed_values = false;

    int cur_selected_row = a_modelIndex_ROW;

//    QDEBUG_STRING_OUTPUT_2_N("ON_CLICKED_Show_paid_in_paid_out_VALUES_amount; cur_selected_row",
//                              cur_selected_row);


    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    QString last_select_row_DB_ID =
                tableView_comptes_doperations_comptables->lastSelectedRow__ID();


    QString A_Budget_Line_Title_TO_query_for;



    {
        YerithSqlTableModel &LIGNES_BUDGETAIRES_SqlTableModel =
            _allWindows->getSqlTableModel_lignes_budgetaires();


        LIGNES_BUDGETAIRES_SqlTableModel
            .yerithSetFilter_WITH_where_clause(QString("%1 = '%2'")
                                                 .arg(YerithDatabaseTableColumn::ID,
                                                      last_select_row_DB_ID));

        int row_COUNT_LIGNES_BUDGETAIRES =
            LIGNES_BUDGETAIRES_SqlTableModel
                .easySelect("src/windows/yerith-erp-comptabilite-window.cpp",
                            512);

        QSqlRecord a_qsql_record_LIGNES_BUDGETAIRES;

        if (1 == row_COUNT_LIGNES_BUDGETAIRES)
        {
            a_qsql_record_LIGNES_BUDGETAIRES =
                LIGNES_BUDGETAIRES_SqlTableModel.record(0);

            A_Budget_Line_Title_TO_query_for =
                GET_SQL_RECORD_DATA(a_qsql_record_LIGNES_BUDGETAIRES,
                                    YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire);
        }

        LIGNES_BUDGETAIRES_SqlTableModel
            .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 529);
    }


    if (A_Budget_Line_Title_TO_query_for.isEmpty())
    {
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        return ret_showed_values;
    }



    double ALL_Credit_Incoming_CASH_MONEY = 0.0;

    double ALL_DEBIT_OUTGOING_CASH_MONEY = 0.0;


    {
        YerithSqlTableModel &type_doperation_SqlTbleModel =
            _allWindows->getSqlTableModel_types_doperations_financieres();


        /*
         * Search for all db table rows where this budget line
         * is used for putting (crediting) money into bank
         * account.
         */
        QString FILTER_BUGET_LINE_TITLE_credit_entree_dargent =
            QString("(%1 = '%2' AND %3 = '%4')")
                .arg(YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE,
                     A_Budget_Line_Title_TO_query_for,
                     YerithDatabaseTableColumn::TYPE_DE_CREDIT_DEBIT,
                     QString::number(YerithUtils::CREDIT_ENTREE_DARGENT));


        type_doperation_SqlTbleModel
            .yerithSetFilter_WITH_where_clause
                (FILTER_BUGET_LINE_TITLE_credit_entree_dargent);


        int row_COUNT_credit_entree_dargent =
            type_doperation_SqlTbleModel
                .easySelect("src/windows/yerith-erp-comptabilite-window.cpp",
                            571);


        QString a_type_doperation_financiere_CREDIT_ENTREE_DARGENT;

        QSqlRecord record_credit_entree_dargent;


        if (row_COUNT_credit_entree_dargent > 0)
        {
            record_credit_entree_dargent = type_doperation_SqlTbleModel.record(0);

            a_type_doperation_financiere_CREDIT_ENTREE_DARGENT =
                GET_SQL_RECORD_DATA(record_credit_entree_dargent,
                                    YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE);

//        QDEBUG_STRING_OUTPUT_2(QString("TYPE D'OPÉRATION FINANCIÈRE DE \"%1\" COMME CRÉDIT entrée d'argent")
//                                 .arg(FILTER_BUGET_LINE_TITLE_credit_entree_dargent),
//                               QString("(%1): %2")
//                                 .arg(a_type_doperation_financiere_CREDIT_ENTREE_DARGENT,
//                                      YerithUtils::_typedoperationfinancieresToUserViewString
//                                        .value(a_type_doperation_financiere_CREDIT_ENTREE_DARGENT.toUInt())));


            if (YerithUtils::OPERATION_FINANCIERE_VENTE ==
                    a_type_doperation_financiere_CREDIT_ENTREE_DARGENT.toInt())
            {
                double CREDIT_entree_dargent_Vente_Comptant_Amount =
                            GET_sales_cash_AND_sales_client_account_Amount();

                ALL_Credit_Incoming_CASH_MONEY =
                    ALL_Credit_Incoming_CASH_MONEY +
                        CREDIT_entree_dargent_Vente_Comptant_Amount;
            }
        }


        type_doperation_SqlTbleModel
            .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 612);
    }



    // ######################## ENTRÉES D'ARGENTS (VENTES) ########################
    {
        double CREDIT_entree_dargent_Vente_Comptant_Amount_2 =
                GET_sales_cash_AND_sales_client_account_Amount();


        lineEdit_Comptabilite_VENTES
            ->setText(GET_CURRENCY_STRING_NUM
                        (CREDIT_entree_dargent_Vente_Comptant_Amount_2));
    }



    // ######################## SORTIES D'ARGENT ########################
    {
        QString a_type_doperation_financiere_DEBIT_SORTIE_DARGENT;

        QSqlRecord record_Debit_Sortie_dargent;


        YerithSqlTableModel &type_doperation_SqlTbleModel =
            _allWindows->getSqlTableModel_types_doperations_financieres();


        QString FILTER_BUGET_LINE_TITLE_Debit_Sortie_dargent =
            QString("(%1 = '%2' AND %3 = '%4')")
                .arg(YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE,
                     A_Budget_Line_Title_TO_query_for,
                     YerithDatabaseTableColumn::TYPE_DE_CREDIT_DEBIT,
                     QString::number(YerithUtils::DEBIT_SORTIE_DARGENT));


//        QDEBUG_STRING_OUTPUT_2("FILTER_BUGET_LINE_TITLE_Debit_Sortie_dargent",
//                                FILTER_BUGET_LINE_TITLE_Debit_Sortie_dargent);


        type_doperation_SqlTbleModel
            .yerithSetFilter_WITH_where_clause
                (FILTER_BUGET_LINE_TITLE_Debit_Sortie_dargent);


        int row_COUNT_debit_sortie_dargent =
            type_doperation_SqlTbleModel
                .easySelect("src/windows/yerith-erp-comptabilite-window.cpp",
                            646);


//        QDEBUG_STRING_OUTPUT_2_N("row_COUNT_debit_sortie_dargent",
//                                  row_COUNT_debit_sortie_dargent);


        uint entry_count_Human_RESOURCE_EXPENSE = 0;

        uint entry_count_FINANCIAL_EXPENSE = 0;

        uint entry_count_Asset_PURCHASE = 0;

        uint entry_count_Merchandise_PURCHASE = 0;


        for (int i = 0; i < row_COUNT_debit_sortie_dargent; ++i)
        {
            record_Debit_Sortie_dargent = type_doperation_SqlTbleModel.record(i);

            a_type_doperation_financiere_DEBIT_SORTIE_DARGENT =
                GET_SQL_RECORD_DATA(record_Debit_Sortie_dargent,
                                    YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE);


//            QDEBUG_STRING_OUTPUT_2("a_type_doperation_financiere_DEBIT_SORTIE_DARGENT",
//                                    YerithUtils::_typedoperationfinancieresToUserViewString
//                                        .value(a_type_doperation_financiere_DEBIT_SORTIE_DARGENT.toInt()));

            if (0 == entry_count_Asset_PURCHASE                       &&
                YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MATERIEL_IMMOBILISATIONS ==
                    a_type_doperation_financiere_DEBIT_SORTIE_DARGENT.toInt())
            {
                double DEBIT_sortie_dargent_Achat_DE_Materiel_IMMOBILISATIONS_Amount =
                    GET_asset_PURCHASE_Amount();


                lineEdit_Comptabilite_ASSETS_PURCHASES
                    ->setText(GET_CURRENCY_STRING_NUM
                                (DEBIT_sortie_dargent_Achat_DE_Materiel_IMMOBILISATIONS_Amount));


                ALL_DEBIT_OUTGOING_CASH_MONEY =
                    ALL_DEBIT_OUTGOING_CASH_MONEY +
                        DEBIT_sortie_dargent_Achat_DE_Materiel_IMMOBILISATIONS_Amount;

                ++entry_count_Asset_PURCHASE;
            }


            if (0 == entry_count_Merchandise_PURCHASE                       &&
                YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES ==
                    a_type_doperation_financiere_DEBIT_SORTIE_DARGENT.toInt())
            {
                double DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount =
                    GET_merchandise_PURCHASE_Amount();


                lineEdit_Comptabilite_ACHATS_DE_MARCHANDISES
                    ->setText(GET_CURRENCY_STRING_NUM(DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount));


                ALL_DEBIT_OUTGOING_CASH_MONEY =
                    ALL_DEBIT_OUTGOING_CASH_MONEY +
                        DEBIT_sortie_dargent_Achat_DE_Marchandises_Amount;

                ++entry_count_Merchandise_PURCHASE;
            }


            if (0 == entry_count_Human_RESOURCE_EXPENSE                          &&
                YerithUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE ==
                    a_type_doperation_financiere_DEBIT_SORTIE_DARGENT.toInt() )
            {
                double DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount =
                    GET_Human_RESOURCE_EXPENSE_Amount();


                lineEdit_Comptabilite_Ressource_Humaines_Payement_Salaires
                    ->setText(GET_CURRENCY_STRING_NUM(DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount));


                ALL_DEBIT_OUTGOING_CASH_MONEY =
                    ALL_DEBIT_OUTGOING_CASH_MONEY +
                        DEBIT_sortie_dargent_PAYEMENT_RESSOURCES_HUMAINES_Amount;

                ++entry_count_Human_RESOURCE_EXPENSE;
            }


            if (0 == entry_count_FINANCIAL_EXPENSE                          &&
                YerithUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE ==
                    a_type_doperation_financiere_DEBIT_SORTIE_DARGENT.toInt() )
            {
                double DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount =
                    GET_FINANCIAL_EXPENSE_Amount();


                lineEdit_Comptabilite_CHARGES_FINANCIERES
                    ->setText(GET_CURRENCY_STRING_NUM(DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount));


                ALL_DEBIT_OUTGOING_CASH_MONEY =
                    ALL_DEBIT_OUTGOING_CASH_MONEY +
                        DEBIT_sortie_dargent_CHARGES_FINANCIERES_Amount;

                ++entry_count_FINANCIAL_EXPENSE;
            }
        }

        entry_count_FINANCIAL_EXPENSE = 0;

        entry_count_Asset_PURCHASE = 0;

        entry_count_Merchandise_PURCHASE = 0;


//        QDEBUG_STRING_OUTPUT_2_N("ALL_DEBIT_OUTGOING_CASH_MONEY",
//                                  ALL_DEBIT_OUTGOING_CASH_MONEY);


        type_doperation_SqlTbleModel
            .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 714);
    }



    lineEdit_comptabilite_encaisse
        ->setText(GET_CURRENCY_STRING_NUM(ALL_Credit_Incoming_CASH_MONEY));


    lineEdit_comptabilite_debourse
        ->setText(GET_CURRENCY_STRING_NUM(ALL_DEBIT_OUTGOING_CASH_MONEY));



    ret_showed_values = true;



    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;


    return ret_showed_values;
}


bool YerithComptabiliteWindow::
        ON_CLICKED_SELECT_COMMERCIAL_OPERATION_TYPE(const QModelIndex &modelIndex)
{
    bool retValue = false;

    int cur_selected_row = modelIndex.row();

    QTableWidgetItem *cur_item_TYPE_OF_COMMERCIAL_OPERATION =
        tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
            ->item(cur_selected_row,
                   YerithComptabiliteTableWidget::TYPE_DOPERATION_FINANCIERE_COLUMN);

    QString TYPE_OF_COMMERCIAL_OPERATION_string;

    if (0 != cur_item_TYPE_OF_COMMERCIAL_OPERATION)
    {
        TYPE_OF_COMMERCIAL_OPERATION_string =
            cur_item_TYPE_OF_COMMERCIAL_OPERATION->text();

        retValue = true;
    }


    comboBox_types_doperations_comptables
        ->find_AND_SET_CURRENT_INDEX_No_ADD_NEW_Item
            (TYPE_OF_COMMERCIAL_OPERATION_string);


    return retValue;
}


QString YerithComptabiliteWindow::
            SLOT_get_Budget_Line_SALES_and_SALES_client_account(int *code_IN_db_OUT /* = 0 */)
{
    QString a_budget_Line_SALES_and_SALES_client_account_RESULT;

    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    YerithSqlTableModel &type_doperation_SqlTbleModel =
        _allWindows->getSqlTableModel_types_doperations_financieres();


    //FOR THIS COMMERCIAl PERATION, YerithUtils::VENTE_COMPTANT
    //is sufficient also to specify
    //YerithUtils::VENTE_COMPTE_CLIENT
    int CODE_type_doperation_commerciale_OPERATION_FINANCIERE_VENTE =
            YerithUtils::OPERATION_FINANCIERE_VENTE;


    if (0 != code_IN_db_OUT)
    {
        *code_IN_db_OUT = CODE_type_doperation_commerciale_OPERATION_FINANCIERE_VENTE;
    }


    QString FILTER_COMMERCIAL_OPERATION_TYPE =
        QString("%1 = '%2'")
            .arg(YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE,
                 QString::number(CODE_type_doperation_commerciale_OPERATION_FINANCIERE_VENTE));


    //QDEBUG_STRING_OUTPUT_2("FILTER_COMMERCIAL_OPERATION_TYPE",
    //                        FILTER_COMMERCIAL_OPERATION_TYPE);


    type_doperation_SqlTbleModel
        .yerithSetFilter_WITH_where_clause
            (FILTER_COMMERCIAL_OPERATION_TYPE);


	int row_COUNT = type_doperation_SqlTbleModel
                        .easySelect("src/windows/yerith-erp-comptabilite-window.cpp",
                                    506);


    //QDEBUG_STRING_OUTPUT_2_N("row_COUNT",
    //                            row_COUNT);


    if (1 == row_COUNT)
    {
        QSqlRecord record = type_doperation_SqlTbleModel.record(0);

        a_budget_Line_SALES_and_SALES_client_account_RESULT =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE);
    }

    type_doperation_SqlTbleModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 524);


    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;


    return a_budget_Line_SALES_and_SALES_client_account_RESULT;
}


void YerithComptabiliteWindow::SLOT_calculer_TOUTES_les_lignes_budgetaires()
{
    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    //ASSOCIATE all sale commercial operations to
    //an associated budget line.
    YerithSqlTableModel &ventes_YerithSqlTableModel =
        _allWindows->getSqlTableModel_stocks_vendu();


    QString ALL_SALES_filtering_STRING =
        QString("(%1 = '%2' OR %3 = '%4')")
          .arg(YerithDatabaseTableColumn::TYPE_DE_VENTE,
               QString::number(YerithUtils::VENTE_COMPTANT),
               YerithDatabaseTableColumn::TYPE_DE_VENTE,
               QString::number(YerithUtils::VENTE_COMPTE_CLIENT));


    ventes_YerithSqlTableModel
        .yerithSetFilter_WITH_where_clause
            (ALL_SALES_filtering_STRING);


    int row_count =
        ventes_YerithSqlTableModel
            .easySelect("src/windows/yerith-erp-comptabilite-window.cpp",
                        559);

    QMap<QString, double> LIGNES_BUDGETAIRES__to__SALES_amount_devise;

    int Budget_Line_SALES_CODE_OUT;

    QString Budget_Line_SALES =
        SLOT_get_Budget_Line_SALES_and_SALES_client_account(&Budget_Line_SALES_CODE_OUT);

    //QDEBUG_STRING_OUTPUT_1(QString("Budget_Line_SALES: %1 -- CODE: %2")
    //                        .arg(Budget_Line_SALES,
    //                             QString::number(Budget_Line_SALES_CODE_OUT)));

    QSqlRecord a_qsql_record;

    //Contains Tax collected as VAT (tva en francais)
    double Budget_Line_SALES__CUMULATED_amount = 0.0;

    double Budget_Line_SALES_temporary_amount = 0.0;

    for (int k = 0; k < row_count; ++k)
    {
        a_qsql_record = ventes_YerithSqlTableModel.record(k);

        Budget_Line_SALES_temporary_amount =
            GET_SQL_RECORD_DATA(a_qsql_record,
                                YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();

        Budget_Line_SALES__CUMULATED_amount =
            Budget_Line_SALES__CUMULATED_amount +
            Budget_Line_SALES_temporary_amount;

    }


    //QDEBUG_STRING_OUTPUT_2_N("Budget_Line_SALES__CUMULATED_amount",
    //                          Budget_Line_SALES__CUMULATED_amount);


    LIGNES_BUDGETAIRES__to__SALES_amount_devise
        .insert(Budget_Line_SALES,
                Budget_Line_SALES__CUMULATED_amount);

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


bool YerithComptabiliteWindow::SLOT_Dissocier_TypeDoperation__CompteDoperation()
{

    bool Dissociation_reussie = false;

    int current_row =
            tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE->currentRow();


    static const uint COLUMN_TYPE_INTITULE_LIGNE_BUDGETAIRE = 1;


    QTableWidgetItem *cur_item_TYPE_INTITULE_LIGNE_BUDGETAIRE =
        tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
            ->item(current_row,
                   COLUMN_TYPE_INTITULE_LIGNE_BUDGETAIRE);

    if (0 != cur_item_TYPE_INTITULE_LIGNE_BUDGETAIRE)
    {
        if (cur_item_TYPE_INTITULE_LIGNE_BUDGETAIRE->text().isEmpty())
        {
            return false;
        }
    }


    YerithSqlTableModel &type_doperation_SqlTbleModel =
        _allWindows->getSqlTableModel_types_doperations_financieres();


    int CODE_type_doperation_commerciale =
        YerithUtils::OPERATION_FINANCIERE_INDEFINI;

    QString current_type_doperation_commerciale;

    QTableWidgetItem *cur_item_TYPE_DOPERATION_COMMERCIALE =
        tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
            ->item(current_row,
                   YerithComptabiliteTableWidget::TYPE_DOPERATION_FINANCIERE_COLUMN);

    if (0 != cur_item_TYPE_DOPERATION_COMMERCIALE)
    {
        current_type_doperation_commerciale =
            cur_item_TYPE_DOPERATION_COMMERCIALE->text();
    }


    if (YerithUtils::isEqualCaseSensitive
            (YerithUtils::_typedoperationfinancieresToUserViewString
                .value(YerithUtils::OPERATION_FINANCIERE_VENTE),
                       current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale = YerithUtils::OPERATION_FINANCIERE_VENTE;
    }
    else if (YerithUtils::isEqualCaseSensitive
                (YerithUtils::_typedoperationfinancieresToUserViewString
                    .value(YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MATERIEL_IMMOBILISATIONS),
                           current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale =
            YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MATERIEL_IMMOBILISATIONS;
    }
    else if (YerithUtils::isEqualCaseSensitive
                (YerithUtils::_typedoperationfinancieresToUserViewString
                    .value(YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES),
                           current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale = YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES;
    }
    else if (YerithUtils::isEqualCaseSensitive
                (YerithUtils::_typedoperationfinancieresToUserViewString
                    .value(YerithUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE),
                           current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale = YerithUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE;
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("TYPE D'OPÉRATION COMMERCIALE inconnue"),
                                   QObject::tr("TYPE D'OPÉRATION COMMERCIALE inconnue: (%1) !")
                                        .arg(current_type_doperation_commerciale));
        return false;
    }


    QString FILTER_COMMERCIAL_OPERATION_TYPE =
        QString("%1 = '%2'")
            .arg(YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE,
                 QString::number(CODE_type_doperation_commerciale));


    type_doperation_SqlTbleModel
        .yerithSetFilter_WITH_where_clause
            (FILTER_COMMERCIAL_OPERATION_TYPE);


	int rows = type_doperation_SqlTbleModel
                .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 695);


	if (1 == rows)
	{
		QSqlRecord record = type_doperation_SqlTbleModel.record(0);

		QString current_intitule_de_la_ligne_budgetaire =
                GET_SQL_RECORD_DATA(record,
                                    YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE);


        int Record_type_doperation_integer =
                GET_SQL_RECORD_DATA(record,
                                    YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE).toInt();


        if (CODE_type_doperation_commerciale == Record_type_doperation_integer)
        {
            record.setValue(YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE,
                            YerithUtils::EMPTY_STRING);

            bool success = type_doperation_SqlTbleModel
                            .updateRecord(0,
                                          record,
                                          "src/windows/yerith-erp-comptabilite-window.cpp",
                                          1107);

            QString retMsg =
                QObject::tr("Le type d'opération commerciale '%1' ")
                    .arg(YerithUtils::_typedoperationfinancieresToUserViewString
                            .value(CODE_type_doperation_commerciale));

            if (!success)
            {
                Dissociation_reussie = false;

                retMsg.append
                    (QObject::tr("n'a pas pu être dissocié à la "
                                 "ligne bugétaire intitulée \"%1\"")
                            .arg(current_intitule_de_la_ligne_budgetaire));

                YerithQMessageBox::warning(this,
                                           QObject::tr("échec - DISSOCIATION OPÉRATION, ligne budgétaire"),
                                           retMsg);
            }
            else
            {
                Dissociation_reussie = true;

                QString current_type_doperation_commerciale_DB_ID =
                            GET_SQL_RECORD_DATA(record,
                                                YerithDatabaseTableColumn::ID);

                retMsg.append
                    (QObject::tr("à été dissocié à la ligne bugétaire "
                                 "intitulée \"%1\"")
                            .arg(current_intitule_de_la_ligne_budgetaire));

                YerithQMessageBox::information(this,
                                               QObject::tr("succès - DISSOCIATION OPÉRATION, ligne budgétaire"),
                                               retMsg);
            }

            afficher_ASSOCIATIONS_OperationsCommerciales__compte_bancaires();
        }
	}


    type_doperation_SqlTbleModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 765);


    //tabWidget_creer->setCurrentIndex(0);

    //tabWidget_creer->setCurrentIndex(1);

    afficher_ASSOCIATIONS_OperationsCommerciales__compte_bancaires();


    return Dissociation_reussie;
}


bool YerithComptabiliteWindow::SLOT_associer_TypeDoperation__CompteDoperation()
{
    bool association_reussie = false;

    YerithSqlTableModel &type_doperation_SqlTbleModel =
        _allWindows->getSqlTableModel_types_doperations_financieres();

    int CODE_type_doperation_commerciale =
        YerithUtils::OPERATION_FINANCIERE_INDEFINI;

    QString current_type_doperation_commerciale =
        comboBox_types_doperations_comptables->currentText();


    if (YerithUtils::isEqualCaseSensitive
            (YerithUtils::_typedoperationfinancieresToUserViewString
                .value(YerithUtils::OPERATION_FINANCIERE_VENTE),
                       current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale = YerithUtils::OPERATION_FINANCIERE_VENTE;
    }
    else if (YerithUtils::isEqualCaseSensitive
                (YerithUtils::_typedoperationfinancieresToUserViewString
                    .value(YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MATERIEL_IMMOBILISATIONS),
                           current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale =
            YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MATERIEL_IMMOBILISATIONS;
    }
    else if (YerithUtils::isEqualCaseSensitive
                (YerithUtils::_typedoperationfinancieresToUserViewString
                    .value(YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES),
                           current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale = YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES;
    }
    else if (YerithUtils::isEqualCaseSensitive
                (YerithUtils::_typedoperationfinancieresToUserViewString
                    .value(YerithUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE),
                           current_type_doperation_commerciale))
    {
        CODE_type_doperation_commerciale = YerithUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE;
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("TYPE D'OPÉRATION COMMERCIALE inconnue"),
                                   QObject::tr("TYPE D'OPÉRATION COMMERCIALE inconnue !"));
        return false;
    }


    QString FILTER_COMMERCIAL_OPERATION_TYPE =
        QString("%1 = '%2'")
            .arg(YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE,
                 QString::number(CODE_type_doperation_commerciale));


    type_doperation_SqlTbleModel
        .yerithSetFilter_WITH_where_clause
            (FILTER_COMMERCIAL_OPERATION_TYPE);


	int rows = type_doperation_SqlTbleModel
                .easySelect("src/windows/yerith-erp-comptabilite-window.cpp", 832);

	if (1 == rows)
	{
		QSqlRecord record = type_doperation_SqlTbleModel.record(0);

        int Record_type_doperation_integer =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE).toInt();

		QString CURRENT_intitule_de_la_ligne_budgetaire =
            comboBox_comptes_doperations_comptables->currentText();


        if (CODE_type_doperation_commerciale == Record_type_doperation_integer &&
            !CURRENT_intitule_de_la_ligne_budgetaire.isEmpty())
        {
            record.setValue(YerithDatabaseTableColumn::INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE,
                            CURRENT_intitule_de_la_ligne_budgetaire);

            bool success = type_doperation_SqlTbleModel
                            .updateRecord(0,
                                          record,
                                          "src/windows/yerith-erp-comptabilite-window.cpp",
                                          853);

            QString retMsg =
                QObject::tr("Le type d'opération commerciale '%1' ")
                    .arg(YerithUtils::_typedoperationfinancieresToUserViewString
                            .value(CODE_type_doperation_commerciale));


            if (!success)
            {
                association_reussie = false;

                retMsg.append
                    (QObject::tr("n'a pas pu être associé à la "
                                 "ligne bugétaire intitulée \"%1\"")
                            .arg(CURRENT_intitule_de_la_ligne_budgetaire));

                YerithQMessageBox::warning(this,
                                           QObject::tr("échec - ASSIGNATION OPÉRATION, ligne budgétaire"),
                                           retMsg);
            }
            else
            {
                association_reussie = true;

                QString current_type_de_credit_debit =
                            GET_SQL_RECORD_DATA(record,
                                                YerithDatabaseTableColumn::TYPE_DE_CREDIT_DEBIT);

                QString current_type_doperation_commerciale_DB_ID =
                            GET_SQL_RECORD_DATA(record,
                                                YerithDatabaseTableColumn::ID);


                int lastCurRow =
                    tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
                        ->add_an_association
                            (current_type_doperation_commerciale_DB_ID,
                             current_type_doperation_commerciale,
                             current_type_de_credit_debit,
                             CURRENT_intitule_de_la_ligne_budgetaire);


                tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE
                    ->selectRow(lastCurRow);


                retMsg.append
                    (QObject::tr("à été associé à la ligne bugétaire "
                                 "intitulée \"%1\"")
                            .arg(CURRENT_intitule_de_la_ligne_budgetaire));

                YerithQMessageBox::information(this,
                                               QObject::tr("succès - ASSIGNATION OPÉRATION, ligne budgétaire"),
                                               retMsg);
            }

            return association_reussie;
        }
	}



    type_doperation_SqlTbleModel
        .resetFilter("src/windows/yerith-erp-comptabilite-window.cpp", 1320);


    return association_reussie;
}


void YerithComptabiliteWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    afficher_comptes_bancaires();
}


void YerithComptabiliteWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_comptabilite_element_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_comptabilite_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter
                .append(QString("%1")
                         .arg(GENERATE_SQL_LIKE_STMT
                                (YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire,
                                 partSearchTerm)));

            if (k != lastIdx)
            {
                _searchFilter.append(" AND ");
            }
        }
    }


    {
        bool GENERATE_Like_SQL_STMT = false;

        if (!checkBox_EXACT_STRING_MATCH->isChecked())
        {
            GENERATE_Like_SQL_STMT = true;
        }

        YerithWindowsCommons::setYerithLineEditQCompleterSearchFilter(_searchFilter,
                                                                      GENERATE_Like_SQL_STMT);
    }


    YerithLineEdit *aYerithLineEdit = 0;

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    QMapIterator<YerithLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    while (it.hasNext())
    {
        it.next();

        aYerithLineEdit = *it.key();

        correspondingDBFieldKeyValue = it.value();

        if (0 != aYerithLineEdit)
        {
            aTableColumnFieldContentForANDSearch = aYerithLineEdit->text();

            if (!correspondingDBFieldKeyValue.isEmpty() &&
                    !aTableColumnFieldContentForANDSearch.isEmpty())
            {
                if (!_searchFilter.isEmpty())
                {
                    _searchFilter.append(" AND ");
                }

                _searchFilter.append(GENERATE_SQL_IS_STMT
                                     (correspondingDBFieldKeyValue,
                                      aTableColumnFieldContentForANDSearch));
            }
        }
    }


//    QDEBUG_STRING_OUTPUT_2("finalSearchFilter", finalSearchFilter);

    if (_yerithSqlTableModel->select())
    {
        afficher_comptes_bancaires(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithStocksWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }
}


void YerithComptabiliteWindow::disable_yerith_widgets()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);
}


void YerithComptabiliteWindow::enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()
{
	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);
}


void YerithComptabiliteWindow::handle_Enable_Disable_BUTTONS()
{
    if (tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE->rowCount() > 0)
    {
        pushButton_Dissocier_TYPE_DOPERATION__compte_doperation
            ->setVisible(true);
    }
    else
    {
        pushButton_Dissocier_TYPE_DOPERATION__compte_doperation
            ->setVisible(false);
    }


    if (comboBox_types_doperations_comptables->hasItem() &&
        comboBox_comptes_doperations_comptables->hasItem())
    {
        pushButton_ASSOCIER_TYPE_DOPERATION__compte_doperation
            ->setVisible(true);
    }

    if (!comboBox_types_doperations_comptables->hasItem() ||
        !comboBox_comptes_doperations_comptables->hasItem())
    {
        pushButton_ASSOCIER_TYPE_DOPERATION__compte_doperation
            ->setVisible(false);
    }
}


void YerithComptabiliteWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_comptabilite_element_de_BUDGET->setVisible(IS_VISIBLE);
    comboBox_comptabilite_element_de_BUDGET_condition->setVisible(IS_VISIBLE);
    lineEdit_comptabilite_element_resultat->setVisible(IS_VISIBLE);
    pushButton_comptabilite_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_comptabilite_filtrer->setVisible(IS_VISIBLE);
}


void YerithComptabiliteWindow::handle_change_tab(int current_tab_index)
{
    switch (current_tab_index)
    {
    case BUGET_Line__Operation_Type_Account:

        setWindowTitle(QObject::tr("Yerith-pgi-3.0 - comptabilité - Lignes budgétaires"));


        _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&_yerithTableView_FROM_WINDOWS_COMMONS->font());

        _a_CURRENT_table_widget = 0;


        QObject::disconnect(actionAUGMENTER_LA_POLICE_DU_TABLEAU, 0, 0, 0);

        QObject::disconnect(actiondiminuer_la_police_du_tableau, 0, 0, 0);

        connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
                SIGNAL(triggered()),
                (YerithWindowsCommons *) this,
                SLOT(incrementFontSize__OF_TABLE()));

        connect(actiondiminuer_la_police_du_tableau,
                SIGNAL(triggered()),
                (YerithWindowsCommons *) this,
                SLOT(decrementFontSize__OF_TABLE()));


        QObject::disconnect(actionAfficherPDF, 0, 0, 0);

        connect(actionAfficherPDF,
                SIGNAL(triggered()),
                this,
                SLOT(print_PDF_PREVIOUSLY_SETUP()));

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChampsDBVisible, true);
        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserChampsDBVisible, true);

        afficher_comptes_bancaires();

        break;

    case FINANCIAL_Operation_Type__Financial_Operation_Type_Account:

        setWindowTitle(QObject::tr("Yerith-pgi-3.0 - comptabilité - type d'opérations commerciales"));


        _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE->font());

        _a_CURRENT_table_widget = tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE;


        QObject::disconnect(actionAUGMENTER_LA_POLICE_DU_TABLEAU, 0, 0, 0);

        QObject::disconnect(actiondiminuer_la_police_du_tableau, 0, 0, 0);

        connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
                SIGNAL(triggered()),
                (YerithQTableWidgetWindowsCommons *) this,
                SLOT(incrementFontSize__OF_TABLE__WIDGET()));

        connect(actiondiminuer_la_police_du_tableau,
                SIGNAL(triggered()),
                (YerithQTableWidgetWindowsCommons *) this,
                SLOT(decrementFontSize__OF_TABLE__WIDGET()));


        QObject::disconnect(actionAfficherPDF, 0, 0, 0);

        connect(actionAfficherPDF,
                SIGNAL(triggered()),
                this,
                SLOT(imprimer_UNE_LISTE_WITH_A_YERITH_PROGRESS_BAR()));

        updateComboBoxes();

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChampsDBVisible, false);
        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserChampsDBVisible, false);

        afficher_ASSOCIATIONS_OperationsCommerciales__compte_bancaires();

        break;

    default:
        break;
    }

    handle_Enable_Disable_BUTTONS();
}


void YerithComptabiliteWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_comptabilite_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_comptabilite_filtrer_font->setUnderline(false);
    }

    pushButton_comptabilite_filtrer->setFont(*_pushButton_comptabilite_filtrer_font);
}


void YerithComptabiliteWindow::refineYerithLineEdits()
{
//      setupLineEditsQCompleters((QObject *)this);

    afficher_comptes_bancaires();
}


void YerithComptabiliteWindow::
        get_PRINT_OUT_TexTableString_LOCAL(QString    &texTable_IN_OUT,
                                           int         row_MAX_TO_GO_export /* = -1 */)
{
	QString Header_TEMPLATE_texTable_In =
                "\\begin{table*}[!htbp]\n"
                "\\centering\n"
                "\\resizebox{\\textwidth}{!}{\n"
                "\\begin{tabular}"
                "{|c|c|l|l|l|} \\hline";


    QString ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In =
                "& & & &				                       \\\\ \n"
                "\\textbf{N\\textsuperscript{o}}			   & 	 \n"
                "\\textbf{DB id}			                   & 	 \n"
                "\\textbf{TYPE OF COMMERCIAL OPERATION}		   & 	 \n"
                "\\textbf{\\it entrance OR going out (money)}  & 	 \n"
                "\\textbf{\\small Budget LINE TITLE}		   \\\\ \n"
                "& & & &				                       \\\\ \\hline \\hline \n";


   QString FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In =
                "& & & &				                            \\\\ \n"
                "\\textbf{N\\textsuperscript{o}}			        & 	 \n"
                "\\textbf{DB id}			                        & 	 \n"
                "\\textbf{TYPE D'OPÉRATION COMMERCIALE}		        & 	 \n"
                "\\textbf{\\it entrée OR sortie [ARGENT]}           & 	 \n"
                "\\textbf{\\small Intitulé d'$1$ LIGNE BUDGÉTAIRE}  \\\\ \n"
                "& & & &				                            \\\\ \\hline \\hline \n";


    YerithQTableWidgetWindowsCommons::
        get_PRINT_OUT_TexTableString(*tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE,
                                     texTable_IN_OUT,
                                     Header_TEMPLATE_texTable_In,
                                     FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In,
                                     ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In,
                                     row_MAX_TO_GO_export);
}


void YerithComptabiliteWindow::updateComboBoxes()
{
    YerithUtils::RELOAD_VIEW_STRINGS();


    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire);

    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::_tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);



    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerithDatabaseTableColumn::montant_de_la_ligne_budgetaire));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerithDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire));

    comboBox_comptabilite_element_de_BUDGET->clear();

    comboBox_comptabilite_element_de_BUDGET->addItems(aQStringList);



    comboBox_comptabilite_element_de_BUDGET_condition->clear();

    comboBox_comptabilite_element_de_BUDGET_condition
        ->addItems(_ALL_WINDOWS_FILTER_NUMERIC_CONDITIONS);



    comboBox_comptes_doperations_comptables->clear();

    comboBox_types_doperations_comptables->clear();

    comboBox_comptes_doperations_comptables
        ->populateComboBoxMissingRawString
            (YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
             YerithDatabase::LIGNES_BUDGETAIRES,
             YerithUtils::EMPTY_STRING);

    comboBox_types_doperations_comptables->populateComboBox();
}


void YerithComptabiliteWindow::populateComboBoxes()
{
    YerithUtils::RELOAD_VIEW_STRINGS();


    comboBox_comptes_doperations_comptables
        ->populateComboBoxMissingRawString
            (YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
             YerithDatabase::LIGNES_BUDGETAIRES,
             YerithUtils::EMPTY_STRING);


    comboBox_types_doperations_comptables
        ->setupPopulateNORawString(YerithDatabase::TYPE_DOPERATIONS_FINANCIERES,
                                   YerithDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE,
                                   &YerithUtils::_typedoperationfinancieresToUserViewString);

    comboBox_types_doperations_comptables->populateComboBox();
}


void YerithComptabiliteWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_comptabilite_nombre_de_lignes_par_page;

    lineEdit_comptabilite_terme_recherche
        ->enableForSearch(QObject::tr("terme à rechercher (description de la Ligne budgétaire)"));

    lineEdit_nom_element_string_db
        ->enableForSearch(QObject::tr("valeur à rechercher"));

    lineEdit_comptabilite_element_resultat
        ->setValidator(&YerithUtils::POSITIVE_DoubleValidator);


    lineEdit_nombre_de_comptes_bancaires->setYerithEnabled(false);
    lineEdit_comptabilite_encaisse->setYerithEnabled(false);
    lineEdit_comptabilite_debourse->setYerithEnabled(false);


    lineEdit_Comptabilite_Ressource_Humaines_Payement_Salaires
        ->setYerithEnabled(false);

    lineEdit_Comptabilite_VENTES->setYerithEnabled(false);
    lineEdit_Comptabilite_CHARGES_FINANCIERES->setYerithEnabled(false);
    lineEdit_Comptabilite_ACHATS_DE_MARCHANDISES->setYerithEnabled(false);
    lineEdit_Comptabilite_ASSETS_PURCHASES->setYerithEnabled(false);


    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_comptabilite_nombre_de_lignes_par_page,
     tableView_comptes_doperations_comptables);
}


void YerithComptabiliteWindow::setupDateTimeEdits()
{
    dateEdit_comptabilite_ASSOCIATIONS_debut
        ->setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_comptabilite_ASSOCIATIONS_fin->setStartDate(GET_CURRENT_DATE);


    dateEdit_comptabilite_debut
        ->setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_comptabilite_fin->setStartDate(GET_CURRENT_DATE);


    connect(dateEdit_comptabilite_debut,
            SIGNAL(dateChanged(const QDate &)),
            this,
            SLOT(refineYerithLineEdits()));

    connect(dateEdit_comptabilite_fin,
            SIGNAL(dateChanged(const QDate &)),
            this,
            SLOT(refineYerithLineEdits()));
}



void YerithComptabiliteWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);



    pushButton_Dissocier_TYPE_DOPERATION__compte_doperation
        ->enable(this,
                 SLOT(EXECUTER_DB_SLOT_Dissocier_TypeDoperation__CompteDoperation()));


    pushButton_ASSOCIER_TYPE_DOPERATION__compte_doperation
        ->enable(this,
                 SLOT(EXECUTER_DB_SLOT_associer_TypeDoperation__CompteDoperation()));



    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curCompte_Lignes_Budgetaires_SqlTableModel)
}


void YerithComptabiliteWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_Dissocier_TYPE_DOPERATION__compte_doperation
        ->disable(this);

    pushButton_ASSOCIER_TYPE_DOPERATION__compte_doperation
        ->disable(this);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
}


void YerithComptabiliteWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
    retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curCompte_Lignes_Budgetaires_SqlTableModel =
        &_allWindows->getSqlTableModel_lignes_budgetaires();

    setYerithSqlTableModel(_curCompte_Lignes_Budgetaires_SqlTableModel);

    updateComboBoxes();


    setVisible(true);


    afficher_ASSOCIATIONS_OperationsCommerciales__compte_bancaires();

    afficher_comptes_bancaires();


    handle_change_tab(tabWidget_creer->currentIndex());


    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("comptabilité"),
    								 _curCompte_Lignes_Budgetaires_SqlTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();


    lineEdit_comptabilite_terme_recherche->setFocus();
}


bool YerithComptabiliteWindow::imprimer_UNE_LISTE(bool *full_list /* = 0 */)
{
	QString a_yerith_qtable_widget__tex_table;

	QString a_yerith_qtable_widget__TexDocument_In_Out;


    if (0 != full_list)
    {
        if (*full_list)
        {
            get_PRINT_OUT_TexTableString_LOCAL(a_yerith_qtable_widget__tex_table);
        }
        else
        {
            int a_row_FOR_pdf_printing_max = getLast_SelectedRow_Row_ID() + 1;

            get_PRINT_OUT_TexTableString_LOCAL(a_yerith_qtable_widget__tex_table,
                                               a_row_FOR_pdf_printing_max);
        }
    }
    else
    {
        return false;
    }


	emit SIGNAL_INCREMENT_PROGRESS_BAR(11);


	YerithUtils::get_TYPEDOPERATION_UNELIGNEBUDGETAIRE_TexDocumentString
                                (a_yerith_qtable_widget__TexDocument_In_Out,
                                 a_yerith_qtable_widget__tex_table);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(23);


    QString file_name_Subject_Title_FRENCH =
        (*full_list) ? "Types d'opérations commerciales"
                     : "Types d'opérations commerciales (Partielle)";

    QString file_name_Subject_Title_ENGLISH =
        (*full_list) ? "COMMERCIAL OPERATIONS TYPES"
                     : "COMMERCIAL OPERATIONS TYPES (Excerpt)";

    QString prefix_file_name_FRENCH =
        (*full_list) ? "yerith-erp-Types-doperations-commerciales"
                     : "yerith-erp-Types-doperations-commerciales-PARTIELLE";

    QString prefix_file_name_ENGLISH =
        (*full_list) ? "yerith-erp-commercial-operations-types"
                     : "yerith-erp-commercial-operations-types-PARTIAL";


	return
        YerithQTableWidgetWindowsCommons::
                imprimer_UNE_LISTE(*tableWidget_TYPEDOPERATION_UNELIGNEBUDGETAIRE,
                                   a_yerith_qtable_widget__TexDocument_In_Out,
                                   file_name_Subject_Title_FRENCH,
                                   file_name_Subject_Title_ENGLISH,
                                   prefix_file_name_FRENCH,
                                   prefix_file_name_ENGLISH);
}


bool YerithComptabiliteWindow::export_csv_file()
{
    bool success = false;

    QList < int >tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_comptes_doperations_comptables,
                                               tableColumnsToIgnore,
                                               "yerith-erp-financial-accounting-operation-listing-csv-format",
                                               "financial accounting operation listing");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_comptes_doperations_comptables,
                                               tableColumnsToIgnore,
                                               "yerith-erp-fichier-comptes-doperations-comptables-format-csv",
                                               "liste des comptes opérations comptables");
    }

    return success;
}


QString YerithComptabiliteWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_comptabilite_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_comptabilite_tex;
    }

    return _latex_template_print_pdf_content;
}


void YerithComptabiliteWindow::deconnecter_utilisateur()
{
    YerithWindowsCommons::deconnecter_utilisateur();
}
