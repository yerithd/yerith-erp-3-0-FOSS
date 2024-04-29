/*
   * yerith-erp-ventes-window.cpp
   *
   *      Author: Prof Dr Xavier Noundou
   */
#include "yerith-erp-ventes-window.hpp"


#include "src/process/yerith-erp-process.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/windows/stocks/yerith-erp-stock-detail-window.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

#include <QtGui/QContextMenuEvent>

#include <QtCore/QThread>

#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcessEnvironment>


#include <unistd.h>


YerithVentesWindow::YerithVentesWindow()
:YerithWindowsCommons("yerith-erp-journal-ventes"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::STOCKS_VENDU),
 _retourVenteTabWidget(0),
 _logger(new YerithLogger("YerithVentesWindow")),
 _pushButton_ventes_filtrer_font(0),
 _ventesDateFilter(YerithUtils::EMPTY_STRING),
 _curStocksVenduTableModel(&_allWindows->getSqlTableModel_stocks_vendu())
{
    setYerithSqlTableModel(_curStocksVenduTableModel);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("ventes"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    enableResizing();

    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_ventes);


    _ACTIONS_TO_DISABLE_WHEN_NOT_PRINT_ACTION
            << action_parametrer_les_impressions
            << actionExporter_au_format_csv
            << actionAfficher_lhistorique_de_ce_stock
			<< actionAfficherPDF;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
        << action_regenerer_recu_client_vente
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionAfficher_lhistorique_de_ce_stock
        << actionAnnulerCetteVente
		<< actionAfficherVenteDetail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_ventes);


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
                        .arg(COLOUR_RGB_STRING_YERITH_WHITE_255_255_255,
                             COLOUR_RGB_STRING_YERITH_BLUE_53_132_228);


    _retourVenteTabWidgetTitle = tabWidget_ventes->tabText(RetourDuneVente);

    _retourVenteTabWidget = tabWidget_ventes->widget(RetourDuneVente);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF
            << YerithDatabaseTableColumn::HISTORIQUE_STOCK;


    setup_select_configure_dbcolumn(YerithDatabase::STOCKS_VENDU);


    _lineEditsToANDContentForSearch.insert(&lineEdit_ventes_terme_recherche,
                                           YerithUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_ventes_nom_caissier,
                                           YerithDatabaseTableColumn::
                                           NOM_CAISSIER);

    _yerith_WINDOW_references_dbColumnString
        .insert(YerithDatabaseTableColumn::REFERENCE);

    _comboBoxesToANDContentForSearch.insert(&comboBox_ventes_type_de_vente,
                                            YerithDatabaseTableColumn::TYPE_DE_VENTE);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION
    (YerithDatabase::STOCKS_VENDU);

    reinitialiser_colones_db_visibles();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    tableView_ventes->setSqlTableName(&YerithDatabase::STOCKS_VENDU);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    _pushButton_ventes_filtrer_font =
                    new QFont(pushButton_ventes_filtrer->font());

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));


    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));


    connect(actionAfficher_lhistorique_de_ce_stock, SIGNAL(triggered()),
            this, SLOT(private_slot_afficher_historique_du_stock()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionAnnulerCetteVente, SIGNAL(triggered()), this,
            SLOT(annuler_cette_vente()));
    connect(actionAfficherVenteDetail, SIGNAL(triggered()), this,
            SLOT(afficher_vente_detail()));
    connect(action_regenerer_recu_client_vente, SIGNAL(triggered()), this,
            SLOT(slot_regenerer_recu_client_vente()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_ventes, SIGNAL(currentChanged(int)), this,
            SLOT(handleCurrentTabChanged(int)));

    connect(tableView_ventes, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(afficher_vente_detail()));

    connect(tableView_ventes, SIGNAL(activated(const QModelIndex &)), this,
            SLOT(afficher_vente_detail()));

    connect(tableView_ventes->getStandardItemModel(),
            SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            this, SLOT(modifier_visibilite_actions_sur_cette_vente()));

    connect(tableView_ventes->getStandardItemModel(),
            SIGNAL(rowsInserted(const QModelIndex &, int, int)),
            this, SLOT(modifier_visibilite_actions_sur_cette_vente()));

    setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING
    (YerithDatabaseTableColumn::TYPE_DE_VENTE);

    setupShortcuts();

    comboBox_retour_vente_nom_client->setYerithEditable(true);
}


YerithVentesWindow::~YerithVentesWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithVentesWindow::initialiser_FILTRES()
{
    comboBox_ventes_element_de_vente->setVisible(false);
    comboBox_ventes_element_de_vente_condition->setVisible(false);
    lineEdit_ventes_element_de_vente_resultat->setVisible(false);
    pushButton_ventes_reinitialiser_filtre->setVisible(false);
    pushButton_ventes_filtrer->setVisible(false);
}


void YerithVentesWindow::private_slot_afficher_historique_du_stock()
{
    //qDebug() << QString("lastSelectedRow__ID: %1")
    //                              .arg(QString::number(lastSelectedRow__ID));

    if (0 != _curStocksTableModel && _curStocksTableModel->rowCount() > 0)
    {
        QSqlRecord record;

        _allWindows->
        _ventesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

        QString
        historiqueStockSelectionne(GET_SQL_RECORD_DATA
                                   (record,
                                    YerithDatabaseTableColumn::
                                    HISTORIQUE_STOCK));


        QString
        stockReference(GET_SQL_RECORD_DATA
                       (record, YerithDatabaseTableColumn::REFERENCE));

        QString
        stockDesignation(GET_SQL_RECORD_DATA
                         (record, YerithDatabaseTableColumn::DESIGNATION));

        bool show_asset = YerithUtils::IS_QSQL_RECORD_AN_ASSET(record);

//          qDebug() << QString("++ historiqueStockSelectionne: %1")
//                                      .arg(historiqueStockSelectionne);

        YerithHistoriqueStock::listHistoriqueDuStock
        (historiqueStockSelectionne, stockReference, stockDesignation,
         show_asset);
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("historique du stock"),
                                       QObject::tr
                                       ("Il n'y a pas de stocks listés !"));
    }
}


void YerithVentesWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_ventes_element_de_vente->setVisible(IS_VISIBLE);
    comboBox_ventes_element_de_vente_condition->setVisible(IS_VISIBLE);
    lineEdit_ventes_element_de_vente_resultat->setVisible(IS_VISIBLE);
    pushButton_ventes_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_ventes_filtrer->setVisible(IS_VISIBLE);
}


void YerithVentesWindow::slot_regenerer_recu_client_vente()
{
    int ventesTableViewRowCount = tableView_ventes->rowCount();

    if (ventesTableViewRowCount <= 0)
    {
        return;
    }

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord curStocksVenduRecord;

    _allWindows->_ventesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (curStocksVenduRecord);

    QString reference_recu_vendu = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                       YerithDatabaseTableColumn::
                                                       REFERENCE_RECU_VENDU);


    QString SELECT_ALL_SOLD_ITEMS_FOR_THIS_SALE =
                    QString("select * from %1 WHERE %2='%3'").
                    arg(YerithDatabase::STOCKS_VENDU,
                        YerithDatabaseTableColumn::REFERENCE_RECU_VENDU,
                        reference_recu_vendu);

    QSqlQuery a_ref_recu_vendu_qsql_query;

    int aquery_size = YerithUtils::execQuery(a_ref_recu_vendu_qsql_query,
                                             SELECT_ALL_SOLD_ITEMS_FOR_THIS_SALE);

    if (aquery_size <= 0)
    {
        return;
    }

    uint columns_size = 8;

    QTableWidget tableau_nouveau_recu;

    tableau_nouveau_recu.setRowCount(aquery_size);

    tableau_nouveau_recu.setColumnCount(columns_size);

    tableau_nouveau_recu.setHorizontalHeaderItem(0,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Référence")));
    tableau_nouveau_recu.setHorizontalHeaderItem(1,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Désignation")));
    tableau_nouveau_recu.setHorizontalHeaderItem(2,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Catégorie")));
    tableau_nouveau_recu.setHorizontalHeaderItem(3,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr("Qté")));
    tableau_nouveau_recu.setHorizontalHeaderItem(4,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Qté en stock")));
    tableau_nouveau_recu.setHorizontalHeaderItem(5,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Prix unitaire")));
    tableau_nouveau_recu.setHorizontalHeaderItem(6,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr("TVA")));
    tableau_nouveau_recu.setHorizontalHeaderItem(7,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Total TTC")));


    double quantite_totale_vendue = 0.0;

    double montant_tva = 0.0;

    double montant_total_vente = 0.0;


    QString date_vente;

    QString heure_de_vente;

    QString NOM_CAISSIER;

    QString nomClient;

    int type_de_vente = YerithUtils::VENTE_COMPTANT;

    uint row = 0;

    while (a_ref_recu_vendu_qsql_query.next())
    {
        if (0 == row)
        {
            date_vente =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerithDatabaseTableColumn::DATE_VENTE);

            heure_de_vente =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerithDatabaseTableColumn::HEURE_VENTE);

            NOM_CAISSIER =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerithDatabaseTableColumn::NOM_CAISSIER);

            nomClient =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerithDatabaseTableColumn::
                                                NOM_ENTREPRISE_CLIENT);

            type_de_vente =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerithDatabaseTableColumn::TYPE_DE_VENTE).
                            toInt();
        }


        montant_tva +=
                        GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                            YerithDatabaseTableColumn::MONTANT_TVA).
                        toDouble();

        montant_total_vente +=
                        GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                            YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        quantite_totale_vendue +=
                        qFabs(GET_SQL_RECORD_DATA
                              (a_ref_recu_vendu_qsql_query,
                               YerithDatabaseTableColumn::QUANTITE_VENDUE).toDouble());


        tableau_nouveau_recu.setItem(row, 0,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerithDatabaseTableColumn::
                                                       REFERENCE)));

        tableau_nouveau_recu.setItem(row, 1,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerithDatabaseTableColumn::
                                                       DESIGNATION)));

        tableau_nouveau_recu.setItem(row, 2,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerithDatabaseTableColumn::
                                                       CATEGORIE)));

        tableau_nouveau_recu.setItem(row, 3,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerithDatabaseTableColumn::
                                                       QUANTITE_VENDUE)));

        tableau_nouveau_recu.setItem(row, 4,
                                     new
                                     QTableWidgetItem
                                     (YerithUtils::EMPTY_STRING));

        tableau_nouveau_recu.setItem(row, 5,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerithDatabaseTableColumn::
                                                       PRIX_UNITAIRE)));

        tableau_nouveau_recu.setItem(row, 6,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerithDatabaseTableColumn::
                                                       MONTANT_TVA)));

        tableau_nouveau_recu.setItem(row, 7,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerithDatabaseTableColumn::
                                                       MONTANT_TOTAL_VENTE)));

        ++row;
    }

    QString factureTexTable;

    YerithUtils::getFactureTexTableString(factureTexTable,
                                          tableau_nouveau_recu,
                                          quantite_totale_vendue,
                                          montant_tva, montant_total_vente);


    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVille_LATEX());

    YerithUtils::getCurrentLocaleDate(factureDate);

    YerithUtils::get_RECU_TexDocumentString(factureTexDocument,
                                            factureTexTable);

    QString typeDeVenteStr(YerithUtils::_typedeventeToUserViewString.
                           value(type_de_vente));

    factureTexDocument.replace("YERITHTYPEPAIEMENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (typeDeVenteStr));

    factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (reference_recu_vendu));

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
             YerithERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YERITHPAPERSPEC", "a4paper");
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER,
              YerithERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YERITHPAPERSPEC", "a5paper");
    }

    factureTexDocument.replace("YERITHPAIEMENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (typeDeVenteStr));
    factureTexDocument.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    factureTexDocument.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());
    factureTexDocument.replace("YERITHLOCALISATION_SORTIE",
                               infoEntreprise.getLocalisation_LATEX());
    factureTexDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    factureTexDocument.replace("YERITHEMAIL",
                               infoEntreprise.getEmail_LATEX());
    factureTexDocument.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());
    factureTexDocument.replace("YERITHDATE", factureDate);

#ifdef YERITH_FRANCAIS_LANGUAGE
    factureTexDocument.replace("YERITHHEUREVENTE",
                               QString("%1, %2").arg(heure_de_vente,
                                                     YerithUtils::
                                                     frenchLocale.
                                                     toString
                                                     (GET_DATE_FROM_STRING
                                                      (date_vente))));
#endif

#ifdef YERITH_ENGLISH_LANGUAGE
    factureTexDocument.replace("YERITHHEUREVENTE",
                               QString("%1, %2").arg(heure_de_vente,
                                                     YerithUtils::
                                                     englishLocale.
                                                     toString
                                                     (GET_DATE_FROM_STRING
                                                      (date_vente))));
#endif

    factureTexDocument.replace("YERITHVENDEUR", NOM_CAISSIER);
    factureTexDocument.replace("YERITHCOMPTEBANCAIRENR",
                               infoEntreprise.getNumeroCompteBancaire());
    factureTexDocument.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YERITHAGENCECOMPTEBANCAIRE",
                               infoEntreprise.getAgenceCompteBancaireTex());

    factureTexDocument.replace("YERITHSOMME",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (GET_CURRENCY_STRING_NUM
                                (montant_total_vente)));


    QString
    YERITHMONTANTLITERAL(QString("'%1' %2").arg
                         (GET_SPELLED_NUMBER(montant_total_vente),
                          YerithERPConfig::currency));

    //    QDEBUG_STRING_OUTPUT_2("YERITHMONTANTLITERAL", YERITHMONTANTLITERAL);

    factureTexDocument.replace("YERITHMONTANTLITERAL",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (YERITHMONTANTLITERAL.toUpper()));

    if (YerithUtils::isEqualCaseInsensitive
            (nomClient, YerithUtils::STRING_FRENCH_DIVERS))
    {
        factureTexDocument.replace("YERITHCLIENT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (nomClient));
        factureTexDocument.replace("CLIENTYERITHREPRESENTANT",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHCITY",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHPOBOX",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHMAIL",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHPHONE",
                                   YerithUtils::EMPTY_STRING);
    }
    else if (nomClient.isEmpty())
    {
        nomClient.append("Client \"DIVERS\"");
        factureTexDocument.replace("YERITHCLIENT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (nomClient));
        factureTexDocument.replace("CLIENTYERITHREPRESENTANT",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHCITY",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHPOBOX",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHMAIL",
                                   YerithUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYERITHPHONE",
                                   YerithUtils::EMPTY_STRING);
    }
    else
    {
        YerithSqlTableModel &clientTableModel =
                        _allWindows->getSqlTableModel_clients();

        clientTableModel.yerithSetFilter_WITH_where_clause(QString
                                                           ("nom_entreprise = '%1'").
                                                           arg(nomClient));
        factureTexDocument.replace("YERITHCLIENT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (nomClient));

        if (clientTableModel.easySelect("src/windows/yerith-erp-ventes-window.cpp", 703) > 0)
        {
            QSqlRecord record = clientTableModel.record(0);
            QString
            clientRepresentant(GET_SQL_RECORD_DATA
                               (record,
                                YerithDatabaseTableColumn::
                                NOM_REPRESENTANT));
            QString
            clientVille(GET_SQL_RECORD_DATA
                        (record, YerithDatabaseTableColumn::VILLE));
            QString
            clientPOBox(GET_SQL_RECORD_DATA
                        (record, YerithDatabaseTableColumn::BOITE_POSTALE));
            QString
            clientEmail(GET_SQL_RECORD_DATA
                        (record, YerithDatabaseTableColumn::EMAIL));
            QString
            clientTel(GET_SQL_RECORD_DATA
                      (record,
                       YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

            if (clientTel.isEmpty())
            {
                clientTel.append(GET_SQL_RECORD_DATA
                                 (record,
                                  YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
            }

            //qDebug() << "++ clientEmail: " << clientEmail;
            if (!clientRepresentant.isEmpty())
            {
                factureTexDocument.replace("CLIENTYERITHREPRESENTANT",
                                           YerithUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientRepresentant));
            }
            else
            {
                factureTexDocument.replace("CLIENTYERITHREPRESENTANT",
                                           YerithUtils::EMPTY_STRING);
            }

            if (!clientVille.isEmpty())
            {
                factureTexDocument.replace("CLIENTYERITHCITY",
                                           YerithUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientVille));
            }
            else
            {
                factureTexDocument.replace("CLIENTYERITHCITY",
                                           YerithUtils::EMPTY_STRING);
            }

            if (!clientPOBox.isEmpty())
            {
                factureTexDocument.replace("CLIENTYERITHPOBOX",
                                           YerithUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientPOBox));
            }
            else
            {
                factureTexDocument.replace("CLIENTYERITHPOBOX", "N/a");
            }

            if (!clientEmail.isEmpty())
            {
                factureTexDocument.replace("CLIENTYERITHMAIL",
                                           YerithUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientEmail));
            }
            else
            {
                factureTexDocument.replace("CLIENTYERITHMAIL",
                                           YerithUtils::EMPTY_STRING);
            }

            if (!clientTel.isEmpty())
            {
                factureTexDocument.replace("CLIENTYERITHPHONE",
                                           YerithUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientTel));
            }
            else
            {
                factureTexDocument.replace("CLIENTYERITHPHONE",
                                           YerithUtils::EMPTY_STRING);
            }
        }
        else
        {
            factureTexDocument.replace("YERITHCLIENT",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (nomClient));
            factureTexDocument.replace("CLIENTYERITHREPRESENTANT",
                                       YerithUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYERITHCITY",
                                       YerithUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYERITHPOBOX",
                                       YerithUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYERITHMAIL",
                                       YerithUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYERITHPHONE",
                                       YerithUtils::EMPTY_STRING);
        }

        clientTableModel.resetFilter();
    }

    QString prefixFileName;

#ifdef YERITH_FRANCAIS_LANGUAGE

    prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yerith-erp-vente-facture-grand-NOUVEAU"));

#endif

#ifdef YERITH_ENGLISH_LANGUAGE

    prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yerith-erp-sale-receipt-big-NEW"));

#endif //qDebug() << "++\n" << factureTexDocument;


    QFile tmpLatexFile(prefixFileName + "tex");

    if (tmpLatexFile.open(QFile::WriteOnly))
    {
        tmpLatexFile.write(factureTexDocument.toUtf8());
    }

    tmpLatexFile.close();


    QString
    pdfReceiptFileName(YerithERPProcess::compileLatex(prefixFileName));

    if (!pdfReceiptFileName.isEmpty())
    {
        YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    return;
}


void YerithVentesWindow::afficher_ventes()
{
    tabWidget_ventes->setCurrentIndex(TableauDesVentes);
    textChangedSearchLineEditsQCompleters();
}


void YerithVentesWindow::ENABLE_AfficherAnnulerCetteVente(bool enable)
{
    if (!enable)
    {
        actionAnnulerCetteVente->setVisible(false);
        return;
    }
    else
    {
        YerithPOSUser *aUser = _allWindows->getUser();

        if (0 == aUser)
        {
            actionAnnulerCetteVente->setVisible(false);
            return;
        }

        if (aUser->isManager())
        {
            actionAnnulerCetteVente->setVisible(true);
        }
        else
        {
            actionAnnulerCetteVente->setVisible(false);
        }
    }
}


void YerithVentesWindow::modifier_visibilite_actions_sur_cette_vente()
{
    if (tableView_ventes->rowCount() > 0)
    {
        actionAfficherVenteDetail->setVisible(true);
        ENABLE_AfficherAnnulerCetteVente(true);
    }
    else
    {
        actionAfficherVenteDetail->setVisible(false);
        ENABLE_AfficherAnnulerCetteVente(false);
    }
}


bool YerithVentesWindow::annuler_cette_vente()
{
    int ventesTableViewRowCount = tableView_ventes->rowCount();

    if (ventesTableViewRowCount <= 0)
    {
        return false;
    }

    tabWidget_ventes->setCurrentIndex(RetourDuneVente);

    QString msg;

    if (lineEdit_retour_vente_quantite_a_retourner->text().isEmpty())
    {
        msg =
                        QObject::tr
                        ("Veuillez saisir la quantité d'articles à retourner !");

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("saisir la quantité d'articles à retourner"),
                                       msg);

        lineEdit_retour_vente_quantite_a_retourner->checkField();

        return false;
    }

    double quantite_a_retourner =
                    lineEdit_retour_vente_quantite_a_retourner->text().toDouble();

//      qDebug() << QString("++ qte a retourner: %1")
//                                      .arg(QString::number(quantite_a_retourner));

    if (quantite_a_retourner <= 0.0)
    {
        msg =
                        QObject::tr
                        ("La quantité d'articles à retourner doit être supérieure zéro !");

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("quantité d'articles à retourner"),
                                       msg);

        return false;
    }


    if (YerithUtils::isEqualCaseInsensitive(YerithUtils::EMPTY_STRING,
                                            comboBox_retour_vente_nom_client->
                                            currentText()))
    {
        comboBox_retour_vente_nom_client->checkField();

        return false;
    }

    if (YerithUtils::isEqualCaseInsensitive(YerithUtils::STRING_FRENCH_DIVERS,
                                            comboBox_retour_vente_nom_client->
                                            currentText()))
    {
        msg = QObject::tr("VEUILLEZ SAISIR 1 NOM DU CLIENT 'valide' !");

        YerithQMessageBox::information(this,
                                       QObject::tr("NOM DU CLIENT"), msg);

        return false;
    }


    if (YerithUtils::isEqualCaseInsensitive(YerithUtils::EMPTY_STRING,
                                            lineEdit_retour_vente_numero_de_telephone_client->
                                            text()))
    {
        msg =
                        QObject::tr
                        ("veuillez saisir 1 numéro de téléphone du client !");

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("numéro de téléphone du client"),
                                       msg);

        lineEdit_retour_vente_numero_de_telephone_client->checkField();

        return false;
    }

    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord curStocksVenduRecord;

    _allWindows->_ventesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (curStocksVenduRecord);

    double curStocksVenduQuantiteVendue = 0.0;

    curStocksVenduQuantiteVendue =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::QUANTITE_VENDUE).
                    toDouble();

    if (quantite_a_retourner > curStocksVenduQuantiteVendue)
    {
        msg =
                        QObject::tr("La quantité d'articles à retourner doit être "
                                        "inférieure ou égale à la quantité d'articles qui a été vendu !");

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("quantité d'articles à retourner"),
                                       msg);
        return false;
    }

    QString curVenteReference = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerithDatabaseTableColumn::REFERENCE);

    QString
    curVenteReferenceRecuVendu(GET_SQL_RECORD_DATA
                               (curStocksVenduRecord,
                                YerithDatabaseTableColumn::
                                REFERENCE_RECU_VENDU));

    QString
    curStocksVenduDesignation(GET_SQL_RECORD_DATA
                              (curStocksVenduRecord,
                               YerithDatabaseTableColumn::DESIGNATION));

    msg = QObject::tr("Poursuivre avec l'annulation de "
                          "la vente avec la désignation '%1', et "
                          "avec la référence 'reçu de vente %2' ?").arg
          (curStocksVenduDesignation, curVenteReferenceRecuVendu);

    if (QMessageBox::Cancel ==
            YerithQMessageBox::question(this,
                                        QObject::tr
                                        ("poursuivre l'annulation de la vente"),
                                        msg, QMessageBox::Cancel, QMessageBox::Ok))
    {
        return false;
    }

    int typeDeVente = YerithUtils::VENTE_COMPTANT;

    QString curNomDuClient;

    int clients_id = -1;

    double curMontantTotalVente = 0.0;

    double curMontantARembourserAuClient = 0.0;

    bool rembourserAuCompteClient = false;

    bool successReinsertStock = false;

    double curStockNouvelleQuantiteTotal = 0.0;
    double curStockQuantiteTotal = 0.0;

    QSqlRecord curStockRecord;
    QString curStockTableFilter;

    QString curHistoriqueStock;
    QString curHistoriqueStockRetour;

    QString curStocksVenduID;
    QString curStocksVendu_stocksID;
    QString curStocksVenduNomDepartementProduit;
    QString curStocksVenduCategorie;

    curStocksVenduID =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::ID);

    curStocksVendu_stocksID =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::STOCKS_ID);

    curStocksVenduNomDepartementProduit =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    curStocksVenduDesignation =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::DESIGNATION);

    curStocksVenduCategorie =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::CATEGORIE);

    //Je verifie deja si le stock est encore existant
    //dans la base de donnees
    YerithSqlTableModel &curStockTableModel =
                    _allWindows->getSqlTableModel_stocks();

    curStockTableFilter =
                    QString("%1 = '%2'").arg(YerithDatabaseTableColumn::ID,
                                             curStocksVendu_stocksID);

    curStockTableModel.yerithSetFilter_WITH_where_clause(curStockTableFilter);

    int curStocksTableRowCount =
    		curStockTableModel.easySelect("src/windows/yerith-erp-ventes-window.cpp", 1112);

    curNomDuClient =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);

    clients_id =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::CLIENTS_ID).toInt();

    typeDeVente =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerithDatabaseTableColumn::TYPE_DE_VENTE).toInt();

    bool cur_historique_stock_actif = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                          YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                                      toInt();


    if (1 == curStocksTableRowCount)
    {
        curStockRecord = curStockTableModel.record(0);

        curStockQuantiteTotal =
                        GET_SQL_RECORD_DATA(curStockRecord,
                                            YerithDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        curStockNouvelleQuantiteTotal =
                        curStockQuantiteTotal + quantite_a_retourner;

        curMontantTotalVente =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        curStockRecord.setValue(YerithDatabaseTableColumn::QUANTITE_TOTALE,
                                curStockNouvelleQuantiteTotal);

        cur_historique_stock_actif =
                        GET_SQL_RECORD_DATA(curStockRecord,
                                            YerithDatabaseTableColumn::
                                            HISTORIQUE_STOCK_ACTIF).toInt();

        curHistoriqueStock =
                        GET_SQL_RECORD_DATA(curStockRecord,
                                            YerithDatabaseTableColumn::HISTORIQUE_STOCK);

        if (cur_historique_stock_actif)
        {
            curHistoriqueStockRetour =
                            YerithHistoriqueStock::creer_mouvement_stock
                            (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
                             curStockRecord.value(YerithDatabaseTableColumn::ID).toInt(),
                             GET_CURRENT_DATE, curStockQuantiteTotal, quantite_a_retourner,
                             curStockNouvelleQuantiteTotal);
        }

        curHistoriqueStock.append(YerithHistoriqueStock::SEPARATION_EXTERNE).
        append(curHistoriqueStockRetour);

        curStockRecord.
        setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                 cur_historique_stock_actif);

        curStockRecord.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                curHistoriqueStock);

        successReinsertStock =
                        curStockTableModel.updateRecord(0,
                        								curStockRecord,
														"src/windows/yerith-erp-ventes-window.cpp",
														1181);
    }
    else
    {
        curStockRecord = curStockTableModel.record();

        curStockRecord.setValue(YerithDatabaseTableColumn::ID,
                                curStocksVendu_stocksID);

        curStockRecord.
        setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                 cur_historique_stock_actif);

        curStockRecord.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerithDatabaseTableColumn::
                                                    HISTORIQUE_STOCK));

        curStockRecord.setValue(YerithDatabaseTableColumn::REFERENCE,
                                GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerithDatabaseTableColumn::
                                                    REFERENCE));

        curStockRecord.setValue(YerithDatabaseTableColumn::DESIGNATION,
                                curStocksVenduDesignation);

        curStockRecord.
        setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                 curStocksVenduNomDepartementProduit);

        curStockRecord.setValue(YerithDatabaseTableColumn::CATEGORIE,
                                curStocksVenduCategorie);

        curStockRecord.setValue(YerithDatabaseTableColumn::LOTS_ENTRANT, 1);

        curStockRecord.setValue(YerithDatabaseTableColumn::DATE_PEREMPTION,
                                curStocksVenduRecord.value
                                (YerithDatabaseTableColumn::
                                 DATE_PEREMPTION).toDate());

        curStockRecord.setValue(YerithDatabaseTableColumn::QUANTITE_PAR_LOT,
                                quantite_a_retourner);

        curStockRecord.setValue(YerithDatabaseTableColumn::QUANTITE_TOTALE,
                                quantite_a_retourner);

        curMontantTotalVente =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        //                  qDebug() << QString("++ a rembourser au client: %1")
        //                                              .arg(QString::number(curMontantARembourserAuClient));

        double montant_total_tva = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                       YerithDatabaseTableColumn::MONTANT_TVA).
                                   toDouble();

        double montant_total_tva_en_gros =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS).
                        toDouble();

        double montant_tva_unitaire =
                        montant_total_tva / curStocksVenduQuantiteVendue;

        double montant_tva_unitaire_en_gros =
                        montant_total_tva_en_gros / curStocksVenduQuantiteVendue;

        double prix_unitaire = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                   YerithDatabaseTableColumn::PRIX_UNITAIRE).
                               toDouble();

        double prix_unitaire_en_gros =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerithDatabaseTableColumn::
                                            PRIX_UNITAIRE_EN_GROS).toDouble();

        double prix_vente = prix_unitaire + montant_tva_unitaire;

        double prix_vente_en_gros =
                        prix_unitaire_en_gros + montant_tva_unitaire_en_gros;

        curStockRecord.
        setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                 prix_unitaire_en_gros);

        curStockRecord.setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                                prix_unitaire);

        curStockRecord.setValue(YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                                prix_vente_en_gros);

        curStockRecord.setValue(YerithDatabaseTableColumn::PRIX_VENTE,
                                prix_vente);

        curStockRecord.setValue(YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                                montant_tva_unitaire_en_gros);

        curStockRecord.setValue(YerithDatabaseTableColumn::MONTANT_TVA,
                                montant_tva_unitaire);

        curStockRecord.setValue(YerithDatabaseTableColumn::IS_SERVICE,
                                YerithUtils::MYSQL_FALSE_LITERAL);

        curStockRecord.
        setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                 GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                     YerithDatabaseTableColumn::
                                     NOM_ENTREPRISE_FOURNISSEUR));

        curStockRecord.setValue(YerithDatabaseTableColumn::LOCALISATION,
                                GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerithDatabaseTableColumn::
                                                    LOCALISATION));

        curStockRecord.setValue(YerithDatabaseTableColumn::DATE_ENTREE,
                                GET_CURRENT_DATE);


        if (cur_historique_stock_actif)
        {
            curHistoriqueStockRetour =
                            YerithHistoriqueStock::creer_mouvement_stock
                            (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
                             curStocksVendu_stocksID.toInt(), GET_CURRENT_DATE, 0.0,
                             quantite_a_retourner, quantite_a_retourner);
        }

        curHistoriqueStock.append(YerithHistoriqueStock::SEPARATION_EXTERNE)
        				  .append(curHistoriqueStockRetour);

        curStockRecord.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                curHistoriqueStock);

        successReinsertStock = curStockTableModel
        						.insertNewRecord(curStockRecord,
        										 this,
												 "src/windows/yerith-erp-ventes-window.cpp",
												 1312,
												 curStocksVenduDesignation);
    }

    curStockTableModel.resetFilter();

    if (successReinsertStock)
    {
        double curStocksVenduNouvelleQteVendue =
                        curStocksVenduQuantiteVendue - quantite_a_retourner;

        if (0 != curStocksVenduNouvelleQteVendue)
        {
            curMontantARembourserAuClient =
                            quantite_a_retourner * (curMontantTotalVente /
                                                    curStocksVenduQuantiteVendue);

            double curStocksVenduNouveauMontantTotalVente =
                            curMontantTotalVente - curMontantARembourserAuClient;

            double curStocksVendu_MARGE_BENEFICIAIRE =
                            GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                YerithDatabaseTableColumn::
                                                MARGE_BENEFICIAIRE).toDouble();

            if (curStocksVendu_MARGE_BENEFICIAIRE > 0)
            {
                double marge_beneficiaire_unitaire =
                                curStocksVendu_MARGE_BENEFICIAIRE /
                                curStocksVenduQuantiteVendue;

                double curStocksVendu_nouvelle_MARGE_BENEFICIAIRE =
                                marge_beneficiaire_unitaire * curStocksVenduNouvelleQteVendue;

                curStocksVenduRecord.
                setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE,
                         curStocksVendu_nouvelle_MARGE_BENEFICIAIRE);
            }

            double curStocksVenduMontantTVA =
                            GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                YerithDatabaseTableColumn::MONTANT_TVA).
                            toDouble();

            double curStocksVenduNouveauMontantTVA =
                            curStocksVenduNouvelleQteVendue * (curStocksVenduMontantTVA /
                                                               curStocksVenduQuantiteVendue);

//                      qDebug() << QString("++ curStocksVenduNouveauMontantTotalVente: %1, "
//                                                              "curStocksVenduMontantTVA: %2, "
//                                                              "curStocksVenduNouveauMontantTVA: %3, "
//                                                              "curMontantARembourserAuClient: %4")
//                                                      .arg(QString::number(curStocksVenduNouveauMontantTotalVente),
//                                                               QString::number(curStocksVenduMontantTVA),
//                                                               QString::number(curStocksVenduNouveauMontantTVA),
//                                                               QString::number(curMontantARembourserAuClient));

            curStocksVenduRecord.
            setValue(YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                     curStocksVenduNouveauMontantTotalVente);

            curStocksVenduRecord.
            setValue(YerithDatabaseTableColumn::QUANTITE_VENDUE,
                     curStocksVenduNouvelleQteVendue);

            curStocksVenduRecord.
            setValue(YerithDatabaseTableColumn::MONTANT_TVA,
                     curStocksVenduNouveauMontantTVA);

            _allWindows->
            _ventesWindow->SQL_UPDATE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
            (curStocksVenduRecord);
        }
        else
        {
            curMontantARembourserAuClient = curMontantTotalVente;

            QString
            removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'").arg
                           (YerithDatabase::STOCKS_VENDU,
                            YerithDatabaseTableColumn::ID,
                            curStocksVenduID));

            bool successRemoveRowQuery =
                            YerithUtils::execQuery(removeRowQuery);
        }

        QString STR_CLIENTS_ID = QString::number(clients_id);

        rembourserAuCompteClient =
                        handleCompteClient(STR_CLIENTS_ID,
                                           typeDeVente, curMontantARembourserAuClient);

        clients_id = STR_CLIENTS_ID.toInt();
    }

    if (successReinsertStock)
    {
//              QDEBUG_STRING_OUTPUT_2_N("new created clients_id", clients_id);

        curNomDuClient = comboBox_retour_vente_nom_client->currentText();

        if (rembourserAuCompteClient && -1 != clients_id)
        {
            msg =
                            QObject::tr
                            ("La vente (avec référence 'reçu de vente %1') a été "
                             "annulée avec succès !\n\n"
                             "(Montant remis (crédité) au compte du client '%2': '%3' !)").
                            arg(curVenteReferenceRecuVendu, curNomDuClient,
                                GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
        }
        else
        {
            msg =
                            QObject::tr
                            ("La vente (avec référence 'reçu de vente %1') a été "
                             "annulée avec succès !\n\n"
                             "(Montant à rembourser au client (comptant): '%2' !)").arg
                            (curVenteReferenceRecuVendu,
                             GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
        }

        YerithQMessageBox::information(this, QObject::tr("succès"), msg);

        //J'ajoute 1 entree dans le tableau de paiements afin d'indiquer
        //la somme remboursee au au compte client

        bool successPaiementsInsert = false;

        YerithSqlTableModel &paiementsSqlTableModel =
                        _allWindows->getSqlTableModel_paiements();

        QSqlRecord paiementsRecord = paiementsSqlTableModel.record();

        paiementsRecord.setValue(YerithDatabaseTableColumn::DATE_PAIEMENT,
                                 GET_CURRENT_DATE);
        paiementsRecord.setValue(YerithDatabaseTableColumn::HEURE_PAIEMENT,
                                 CURRENT_TIME);
        paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                 curNomDuClient);

        QString utilisateurCourantNomComplet;

        YerithPOSUser *aUser = _allWindows->getUser();

        if (0 != aUser)
        {
            utilisateurCourantNomComplet = aUser->nom_complet();

            paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENCAISSEUR,
                                     utilisateurCourantNomComplet);
        }
        else
        {
            paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENCAISSEUR,
                                     QObject::tr("inconnu(e)"));
        }

        //This amount is debited from company account to client account;
        //that is why we negate its value into the payments table.
        paiementsRecord.setValue(YerithDatabaseTableColumn::MONTANT_PAYE,
                                 -1 * curMontantARembourserAuClient);

        paiementsRecord.setValue(YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
                                 YerithUtils::
                                 DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT);

//              QDEBUG_STRING_OUTPUT_2("curNomDuClient", curNomDuClient);
//
//              QDEBUG_STRING_OUTPUT_2_N("clients_id", clients_id);

        if (-1 != clients_id)
        {
            paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                     curNomDuClient);
        }
        else
        {
            paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                     YerithUtils::STRING_FRENCH_DIVERS);
        }

        paiementsRecord.setValue(YerithDatabaseTableColumn::NOTES,
                                 QObject::tr
                                 ("RETOUR D'ACHAT de L'ARTICLE '%1' (quantité retournée: '%2') AYANT "
                                  "EU POUR RÉFÉRENCE DE REÇU DE VENTE: '%3' !"
                                  "\n\n" "NOM DU CLIENT: \"%4\" \n"
                                  "NUMÉRO DE TÉLÉPHONE DU client: \"%5\".").
                                 arg(curStocksVenduDesignation,
                                     QString::number(quantite_a_retourner),
                                     curVenteReferenceRecuVendu,
                                     curNomDuClient,
                                     lineEdit_retour_vente_numero_de_telephone_client->
                                     text()));

        int IDforReceipt =
                        YerithERPWindows::getNextIdSqlTableModel_paiements();

        QString
        referenceRecuPaiementClient
        (YerithUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT
         (QString::number(IDforReceipt)));

        paiementsRecord.
        setValue(YerithDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT,
                 referenceRecuPaiementClient);


        paiementsRecord.setValue(YerithDatabaseTableColumn::DESIGNATION,
                                 curStocksVenduDesignation);

        paiementsRecord.setValue(YerithDatabaseTableColumn::REFERENCE,
                                 curVenteReference);


        YerithSqlTableModel &clientsTableModel =
                        _allWindows->getSqlTableModel_clients();

        QString clientsTableFilter =
                        QString("%1 = '%2'").arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                                 curNomDuClient);

        clientsTableModel.yerithSetFilter_WITH_where_clause(clientsTableFilter);

        double nouveau_compte_client = 0.0;

        int rows = clientsTableModel
        			.easySelect("src/windows/yerith-erp-ventes-window.cpp",
        						1543);

        if (rows > 0)
        {
            QSqlRecord clientsRecord = clientsTableModel.record(0);

            //The client new account value has been updated previously in
            // method 'bool handleCompteClient(QString client_id,
            //                                                                 double curMontantARembourserAuClient)
            nouveau_compte_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::COMPTE_CLIENT).
                            toDouble();
        }

        clientsTableModel.resetFilter();

//              QDEBUG_STRING_OUTPUT_2_N("nouveau_compte_client", nouveau_compte_client);

        paiementsRecord.setValue(YerithDatabaseTableColumn::LOCALISATION,
                                 YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

        paiementsRecord.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT,
                                 nouveau_compte_client);

        paiementsRecord.setValue(YerithDatabaseTableColumn::ID, IDforReceipt);

        successPaiementsInsert =
                        paiementsSqlTableModel
							.insertNewRecord(paiementsRecord,
											 this,
											 "src/windows/yerith-erp-ventes-window.cpp",
											 1572,
											 curNomDuClient);

        if (!successPaiementsInsert)
        {
            YerithQMessageBox::warning(this,
                                       QObject::tr("paiements - échec"),
                                       QObject::tr
                                       ("1 entrée dans le tableau des paiements n'a pas pu "
                                        "être faites pour la vente annulée (du client %1), "
                                        "avec référence 'reçu de vente %2' !").
                                       arg(curNomDuClient,
                                           curVenteReferenceRecuVendu));
        }

        tabWidget_ventes->setCurrentIndex(TableauDesVentes);

        lineEdit_retour_vente_quantite_a_retourner->clear();

        reinitialiser_recherche();
    }
    else
    {
        msg = QObject::tr("Échec de l'annulation de la vente "
                              "(avec référence '%1') !").arg
              (curVenteReferenceRecuVendu);

        YerithQMessageBox::information(this, QObject::tr("échec"), msg);
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    return successReinsertStock;
}


//bool annuler_cette_vente_2()
//{
//      QString msg;
//
//      int ventesTableViewRowCount = tableView_ventes->rowCount();
//
//      QString curVenteReferenceRecuVendu(lineEdit_ventes_reference_recu_vendu->text());
//
//      if (curVenteReferenceRecuVendu.isEmpty() || ventesTableViewRowCount <= 0)
//      {
//              msg = QObject::tr("Veuillez saisir la référence du 'reçu de vente' à annuler !");
//
//              YerithQMessageBox::information(this,
//                                                                         QObject::tr("annuler une vente"),
//                                                                         msg);
//
//              return false;
//      }
//
//    msg = QObject::tr("Poursuivre avec l'annulation de "
//                                                        "la vente avec la référence 'reçu de vente "
//                                                        "%1' ?"))
//              .arg(curVenteReferenceRecuVendu);
//
//    if (QMessageBox::Cancel ==
//            YerithQMessageBox::question(this, QObject::tr("poursuivre l'annulation de la vente"),
//                                                                               msg,
//                                                                                       QMessageBox::Cancel,
//                                                                                       QMessageBox::Ok))
//    {
//      return false;
//    }
//
//      YERITH_ERP_3_0_START_DATABASE_TRANSACTION;
//
//      QString typeDeVente(QObject::tr("achat-comptant"));
//
//      QString curNomDuClient;
//
//      int clients_id = -1;
//
//      double curMontantARembourserAuClient = 0.0;
//
//      bool rembourserAuCompteClient = false;
//
//      bool successReinsertStock = false;
//
//      double curStocksVenduQuantiteVendue = 0.0;
//      double curStockNouvelleQuantiteTotal = 0.0;
//      double curStockQuantiteTotal = 0.0;
//
//      QSqlRecord curStockRecord;
//      QString curStockTableFilter;
//
//    QString curHistoriqueStock;
//    QString curHistoriqueStockRetour;
//
//      QString curStocksVenduID;
//      QString curStocksVendu_stocksID;
//      QString curStocksVenduDesignation;
//      QString curStocksVenduCategorie;
//
//      QSqlRecord curStocksVenduRecord;
//
//      for (int k = 0; k < ventesTableViewRowCount; ++k)
//      {
//              curStocksVenduRecord.clear();
//
//              curStocksVenduRecord = _curStocksVenduTableModel->record(k);
//
//              curStocksVenduID =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::ID);
//
//              curStocksVendu_stocksID =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::STOCKS_ID);
//
//              curStocksVenduDesignation =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::DESIGNATION);
//
//              curStocksVenduCategorie =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::CATEGORIE);
//
//              //Je verifie deja si le stock est encore existant
//              //dans la base de donnees
//              YerithSqlTableModel &curStockTableModel = _allWindows->getSqlTableModel_stocks();
//
//              curStockTableFilter = QString("%1 = '%2'")
//                                                              .arg(YerithDatabaseTableColumn::ID,
//                                                                       curStocksVendu_stocksID);
//
//              curStockTableModel.yerithSetFilter(curStockTableFilter);
//
//              int curStocksTableRowCount = curStockTableModel.easySelect();
//
//              if (0 == k)
//              {
//                      curNomDuClient =
//                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);
//
//                      clients_id =
//                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::CLIENTS_ID).toInt();
//
//                      typeDeVente =
//                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::TYPE_DE_VENTE);
//              }
//
//              curStocksVenduQuantiteVendue =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::QUANTITE_VENDUE).toDouble();
//
//              if (1 == curStocksTableRowCount)
//              {
//                      curStockRecord = curStockTableModel.record(0);
//
//                      curStockQuantiteTotal =
//                                      GET_SQL_RECORD_DATA(curStockRecord, YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();
//
//                      curStockNouvelleQuantiteTotal = curStockQuantiteTotal + curStocksVenduQuantiteVendue;
//
//                      curMontantARembourserAuClient +=
//                                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();
//
//                      curStockRecord.setValue(YerithDatabaseTableColumn::QUANTITE_TOTALE, curStockNouvelleQuantiteTotal);
//
//                      curHistoriqueStock =
//                              GET_SQL_RECORD_DATA(curStockRecord, YerithDatabaseTableColumn::HISTORIQUE_STOCK);
//
//              curHistoriqueStockRetour = YerithHistoriqueStock::creer_mouvement_stock
//                                      (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
//                                       curStockRecord.value(YerithDatabaseTableColumn::ID).toInt(),
//                                               GET_CURRENT_DATE,
//                                               curStockQuantiteTotal,
//                                               curStocksVenduQuantiteVendue,
//                                               curStockNouvelleQuantiteTotal);
//
//              curHistoriqueStock.append(YerithHistoriqueStock::SEPARATION_EXTERNE)
//                                         .append(curHistoriqueStockRetour);
//
//              curStockRecord.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);
//
//                      successReinsertStock = curStockTableModel.updateRecord(0, curStockRecord);
//              }
//              else
//              {
//                      curStockRecord = curStockTableModel.record();
//
//                      curStockRecord.setValue(YerithDatabaseTableColumn::ID, curStocksVendu_stocksID);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::HISTORIQUE_STOCK));
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::REFERENCE,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::REFERENCE));
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::DESIGNATION, curStocksVenduDesignation);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::CATEGORIE, curStocksVenduCategorie);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::QUANTITE_TOTALE, curStocksVenduQuantiteVendue);
//
//                  curMontantARembourserAuClient +=
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();
//
////                qDebug() << QString("++ a rembourser au client: %1")
////                                            .arg(QString::number(curMontantARembourserAuClient));
//
//                  double quantite_totale = curStocksVenduQuantiteVendue;
//
//                  double montant_total_tva =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::MONTANT_TVA).toDouble();
//
//                  double montant_tva_unitaire = montant_total_tva / quantite_totale;
//
//                  double prix_unitaire =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::PRIX_UNITAIRE).toDouble();
//
//                  double prix_vente = prix_unitaire + montant_tva_unitaire;
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::PRIX_VENTE, prix_vente);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::MONTANT_TVA, montant_tva_unitaire);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::IS_SERVICE,
//                              YerithUtils::MYSQL_FALSE_LITERAL);
//
//                  QString curStocksVenduDatePeremption =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::DATE_PEREMPTION);
//
//                  QDate formatee(GET_DATE_FROM_STRING(curStocksVenduDatePeremption));
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::DATE_PEREMPTION, DATE_TO_DB_FORMAT_STRING(formatee));
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::LOCALISATION,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerithDatabaseTableColumn::LOCALISATION));
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::LOTS_ENTRANT, 1);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::QUANTITE_PAR_LOT, quantite_totale);
//
//                  curStockRecord.setValue(YerithDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);
//
//                      curHistoriqueStock =
//                              GET_SQL_RECORD_DATA(curStockRecord, YerithDatabaseTableColumn::HISTORIQUE_STOCK);
//
//              curHistoriqueStockRetour = YerithHistoriqueStock::creer_mouvement_stock
//                                      (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
//                                       curStocksVendu_stocksID.toInt(),
//                                               GET_CURRENT_DATE,
//                                               0.0,
//                                               curStocksVenduQuantiteVendue,
//                                               curStocksVenduQuantiteVendue);
//
//              curHistoriqueStock.append(YerithHistoriqueStock::SEPARATION_EXTERNE)
//                                         .append(curHistoriqueStockRetour);
//
//              curStockRecord.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);
//
//                  successReinsertStock = curStockTableModel.insertNewRecord(curStockRecord);
//              }
//
//              curStockTableModel.resetFilter();
//
//              if (successReinsertStock)
//              {
//                      QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
//                                      .arg(YerithDatabase::STOCKS_VENDU,
//                                                      YerithDatabaseTableColumn::ID,
//                                                      curStocksVenduID));
//
//                      bool successRemoveRowQuery = YerithUtils::execQuery(removeRowQuery);
//
//                      if (successRemoveRowQuery &&
//                              -1 != clients_id          &&
//                              YerithUtils::isEqualCaseInsensitive(QObject::tr("achat-compte-client"), typeDeVente))
//                      {
//                              rembourserAuCompteClient = handleCompteClient(QString::number(clients_id),
//                                                                                                                        curMontantARembourserAuClient);
//                      }
//
//                      successReinsertStock = successReinsertStock && successRemoveRowQuery;
//              }
//      } //for
//
//      YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
//
//      if (successReinsertStock)
//      {
//              if (rembourserAuCompteClient)
//              {
//                      msg = QObject::tr("La vente (avec référence 'reçu de vente %1') a été "
//                                                                                "annulée avec succès !\n\n"
//                                      "(Montant crédité au compte du client '%2': '%3' !)"))
//                                              .arg(curVenteReferenceRecuVendu,
//                                                       curNomDuClient,
//                                                       GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
//              }
//              else
//              {
//                      msg = QObject::tr("La vente (avec référence 'reçu de vente %1') a été "
//                                                                                "annulée avec succès !\n\n"
//                                                                                "(Montant à rembourser au client (comptant): '%2' !)"))
//                                              .arg(curVenteReferenceRecuVendu,
//                                                       GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
//              }
//
//              reinitialiser_recherche();
//
//              YerithQMessageBox::information(this,
//                                                                         QObject::tr("succès"),
//                                                                         msg);
//      }
//      else
//      {
//              msg = QObject::tr("Échec de l'annulation de la vente "
//                                                                        "(avec référence '%1') !"))
//                              .arg(curVenteReferenceRecuVendu);
//
//              YerithQMessageBox::information(this,
//                                                                         QObject::tr("échec"),
//                                                                         msg);
//      }
//
//      return successReinsertStock;
//}


bool YerithVentesWindow::filtrer_ventes()
{
    QString
    ventesTableColumnValue(lineEdit_ventes_element_de_vente_resultat->text
                           ());

    if (ventesTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;

    }

    QString
    ventesTableColumnProperty(comboBox_ventes_element_de_vente->currentText
                              ());

    QString
    mathOperator(comboBox_ventes_element_de_vente_condition->currentText());

    QString
    REAL_DB_ID_NAME_ventesTableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.key
     (ventesTableColumnProperty));

    QString filterString(QString("%1 AND (%2 %3 %4)").arg(_ventesDateFilter,
                                                          REAL_DB_ID_NAME_ventesTableColumnProperty,
                                                          mathOperator,
                                                          ventesTableColumnValue));

    //qDebug() << "++ " << _searchFilter;

    if (!_searchFilter.isEmpty())
    {
        _searchFilter.append(QString(" AND %1").arg(filterString));
    }
    else
    {
        _searchFilter.append(filterString);
    }

    //qDebug() << QString("++ %1").arg(_searchFilter);

    setCurrentlyFiltered(true);

    _curStocksVenduTableModel->yerithSetFilter_WITH_where_clause
    (_searchFilter);

    int resultRows =
    		_curStocksVenduTableModel
				->easySelect("src/windows/yerith-erp-ventes-window.cpp", 1949);

    lister_les_elements_du_tableau(*_curStocksVenduTableModel);

    if (resultRows > 0)
    {
        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "ventes - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "ventes - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerithVentesWindow::setCurrentUser_NOM_CAISSIER(const QString &
                                                     aUserNomComplet)
{
    lineEdit_ventes_nom_caissier->setYerithEnabled(false);
    lineEdit_ventes_nom_caissier->setText(aUserNomComplet);
}


void YerithVentesWindow::disableNomCaissier()
{
    lineEdit_ventes_nom_caissier->setYerithEnabled(false);
    lineEdit_ventes_nom_caissier->myClear();
}


void YerithVentesWindow::enableNomCaissier_ONLY_MANAGER()
{
    lineEdit_ventes_nom_caissier->setYerithEnabled(true);
    lineEdit_ventes_nom_caissier->myClear();
}


bool YerithVentesWindow::handleCompteClient(QString &client_id_IN_OUT,
                                            int type_de_vente,
                                            double curMontantARembourserAuClient)
{
    bool result = false;

    YerithSqlTableModel &clientsSqlTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString
    clientsFilter(QString("%1 = '%2'").arg
                  (YerithDatabaseTableColumn::ID, client_id_IN_OUT));

    clientsSqlTableModel.yerithSetFilter_WITH_where_clause(clientsFilter);

    QString a_new_client = YerithUtils::STRING_FRENCH_DIVERS;

    if (!comboBox_retour_vente_nom_client->isEmpty())
    {
        a_new_client = comboBox_retour_vente_nom_client->currentText();
    }

    bool new_client_record_created = false;

    int clientsRowCount =
    		clientsSqlTableModel.easySelect("src/windows/yerith-erp-ventes-window.cpp", 2021);

    if (clientsRowCount <= 0)
    {
        new_client_record_created =
                        YerithUtils::creerNouveauClient(a_new_client, this);

        assert(true == new_client_record_created);
    }

    if (new_client_record_created)
    {
        clientsSqlTableModel.resetFilter();

        QString
        aNewClientsFilter(QString("%1 = '%2'").arg
                          (YerithDatabaseTableColumn::NOM_ENTREPRISE,
                           a_new_client));

        clientsSqlTableModel.yerithSetFilter_WITH_where_clause
        (aNewClientsFilter);

        int newClientsRowCount =
        		clientsSqlTableModel.easySelect("src/windows/yerith-erp-ventes-window.cpp", 2044);

        assert(newClientsRowCount > 0);
    }


    QSqlRecord clientsRecord = clientsSqlTableModel.record(0);

    QString a_client_to_be_reimbursed = GET_SQL_RECORD_DATA(clientsRecord,
                                                            YerithDatabaseTableColumn::NOM_ENTREPRISE);

    client_id_IN_OUT.clear();

    client_id_IN_OUT.append(GET_SQL_RECORD_DATA
                            (clientsRecord, YerithDatabaseTableColumn::ID));

    bool client_account_to_be_updated =
                    (type_de_vente == YerithUtils::VENTE_COMPTE_CLIENT);

    if (client_account_to_be_updated)
    {
        double curCompteClient =
                        clientsRecord.value(YerithDatabaseTableColumn::COMPTE_CLIENT).
                        toDouble();

        double nouveauCompteClient =
                        curCompteClient + curMontantARembourserAuClient;

        clientsRecord.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT,
                               nouveauCompteClient);

        result = clientsSqlTableModel.updateRecord(0,
        										   clientsRecord,
												   "src/windows/yerith-erp-ventes-window.cpp",
												   2086);
    }

    clientsSqlTableModel.resetFilter();

    return result;
}


void YerithVentesWindow::populateComboBoxes()
{
    _logger->log("populateComboBoxes");


    YerithUtils::RELOAD_VIEW_STRINGS();


    comboBox_ventes_type_de_vente->clear();

    comboBox_ventes_type_de_vente
        ->setupPopulateNORawString(YerithDatabase::TYPE_DE_VENTE,
                                   YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                   &YerithUtils::_typedeventeToUserViewString);


    comboBox_ventes_type_de_vente->populateComboBox();


    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::REFERENCE);
    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_PEREMPTION);
    aQStringList.removeAll(YerithDatabaseTableColumn::HISTORIQUE_STOCK);
    aQStringList.removeAll(YerithDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER);
    aQStringList.removeAll(YerithDatabaseTableColumn::NOM_CAISSIER);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::MARGE_BENEFICIAIRE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::QUANTITE_VENDUE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::PRIX_VENTE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::PRIX_UNITAIRE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::MONTANT_TVA));


    comboBox_ventes_element_de_vente->clear();

    comboBox_ventes_element_de_vente->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");


    comboBox_ventes_element_de_vente_condition->clear();

    comboBox_ventes_element_de_vente_condition->addItems(aQStringList);
}


void YerithVentesWindow::setupLineEdits()
{
    _logger->log("setupLineEdits");

    _QLINEEDIT_nombre_de_lignes_par_page = lineEdit_nombre_de_lignes_par_page;

    lineEdit_retour_vente_numero_de_telephone_client->setValidator
    (&YerithUtils::UINT_VALIDATOR_GREATER_THAN_ZERO);

    lineEdit_ventes_terme_recherche->enableForSearch(QObject::tr
                                                     ("terme à rechercher (référence, fournisseur)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_ventes_nom_caissier->enableForSearch(QObject::tr
                                                  ("nom du caissier (caissière)"));

    lineEdit_ventes_quantite_vendue->setYerithEnabled(false);
    lineEdit_ventes_BENEFICE->setYerithEnabled(false);
    lineEdit_ventes_CAISSE->setYerithEnabled(false);
    lineEdit_ventes_recette_totale->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_nombre_de_lignes_par_page,
                                        tableView_ventes);


    lineEdit_details_type_de_vente->setYerithEnabled(false);
    lineEdit_details_nom_client->setYerithEnabled(false);
    lineEdit_details_reference_produit->setYerithEnabled(false);
    lineEdit_details_designation->setYerithEnabled(false);
    lineEdit_details_nom_entreprise_fournisseur->setYerithEnabled(false);
    lineEdit_details_categorie->setYerithEnabled(false);
    lineEdit_details_remise_prix->setYerithEnabled(false);
    lineEdit_details_remise_pourcentage->setYerithEnabled(false);
    lineEdit_details_tva->setYerithEnabled(false);
    lineEdit_details_montant_total->setYerithEnabled(false);
    lineEdit_details_nom_caissier->setYerithEnabled(false);
    lineEdit_details_localisation->setYerithEnabled(false);
    lineEdit_details_quantite_vendue->setYerithEnabled(false);
    lineEdit_details_heure_vente->setYerithEnabled(false);
    lineEdit_details_reference_recu_de_vente->setYerithEnabled(false);
    lineEdit_details_prix_unitaire->setYerithEnabled(false);

    lineEdit_retour_vente_quantite_a_retourner->
    setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_retour_vente_type_de_vente->setYerithEnabled(false);
    lineEdit_retour_vente_reference_produit->setYerithEnabled(false);
    lineEdit_retour_vente_designation->setYerithEnabled(false);
    lineEdit_retour_vente_nom_entreprise_fournisseur->setYerithEnabled(false);
    lineEdit_retour_vente_categorie->setYerithEnabled(false);
    lineEdit_retour_vente_remise_prix->setYerithEnabled(false);
    lineEdit_retour_vente_remise_pourcentage->setYerithEnabled(false);
    lineEdit_retour_vente_tva->setYerithEnabled(false);
    lineEdit_retour_vente_montant_total->setYerithEnabled(false);
    lineEdit_retour_vente_nom_caissier->setYerithEnabled(false);
    lineEdit_retour_vente_localisation->setYerithEnabled(false);
    lineEdit_retour_vente_quantite_vendue->setYerithEnabled(false);
    lineEdit_retour_vente_numero_de_telephone_client->setYerithEnabled(true);
    lineEdit_retour_vente_quantite_a_retourner->setYerithEnabled(true);
    lineEdit_retour_vente_heure_vente->setYerithEnabled(false);
    lineEdit_retour_vente_reference_recu_de_vente->setYerithEnabled(false);
    lineEdit_retour_vente_prix_unitaire->setYerithEnabled(false);

    lineEdit_ventes_terme_recherche->setFocus();
}


void YerithVentesWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionAnnulerCetteVente->setShortcut(Qt::Key_F2);
}


void YerithVentesWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
        _pushButton_ventes_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_ventes_filtrer_font->setUnderline(false);
    }

    pushButton_ventes_filtrer->setFont(*_pushButton_ventes_filtrer_font);
}


void YerithVentesWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    lister_les_elements_du_tableau();
}


void YerithVentesWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_ventes_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
//              qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("(%1 OR %2)").arg
                                 (GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::REFERENCE,
                                   partSearchTerm),
                                  GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::
                                   NOM_ENTREPRISE_FOURNISSEUR,
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


    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    {
        YerithLineEdit *aYerithLineEdit = 0;

        QMapIterator < YerithLineEdit **,
                     QString > it(_lineEditsToANDContentForSearch);

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
    }

    {
        QString curSearchDBStr;

        YerithComboBox *aYerithComboBox = 0;

        QMapIterator < YerithComboBox **,
                     QString > it(_comboBoxesToANDContentForSearch);

        while (it.hasNext())
        {
            it.next();

            aYerithComboBox = *it.key();

            correspondingDBFieldKeyValue = it.value();

            if (0 != aYerithComboBox)
            {
                aTableColumnFieldContentForANDSearch =
                                aYerithComboBox->currentText();

                if (!correspondingDBFieldKeyValue.isEmpty() &&
                        !aTableColumnFieldContentForANDSearch.isEmpty())
                {
                    if (!_searchFilter.isEmpty())
                    {
                        _searchFilter.append(" AND ");
                    }

                    if (aYerithComboBox == comboBox_ventes_type_de_vente)
                    {
                        int typedevente =
                                        YerithUtils::getComboBoxDatabaseQueryValue
                                        (aTableColumnFieldContentForANDSearch,
                                         YerithUtils::_typedeventeToUserViewString);

                        curSearchDBStr = QString::number(typedevente);
                    }

                    _searchFilter.append(GENERATE_SQL_IS_STMT
                                         (correspondingDBFieldKeyValue,
                                          curSearchDBStr));
                }
            }
        }
    }

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER
    QString finalSearchFilter(_ventesDateFilter);

    if (!_searchFilter.isEmpty())
    {
        QString
        searchFilterWithDate(QString("%1 AND (%2)").arg
                             (_ventesDateFilter, _searchFilter));

        finalSearchFilter = searchFilterWithDate;
    }

    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(finalSearchFilter);

    if (_yerithSqlTableModel->select())
    {
        lister_les_elements_du_tableau();
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithVentesWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithVentesWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT
            << YerithDatabaseTableColumn::LOCALISATION
            << YerithDatabaseTableColumn::DATE_VENTE
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT
            << YerithDatabaseTableColumn::QUANTITE_VENDUE
            << YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE;
}


void YerithVentesWindow::resizeEvent(QResizeEvent *event)
{
    lineEdit_ventes_terme_recherche->setFixedWidth(event->size().width() - 4);

    tabWidget_ventes->setFixedWidth(event->size().width() - 4);

    tableView_ventes->setFixedWidth(tabWidget_ventes->width() - 4);
}


void YerithVentesWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tableView_ventes->rowCount() > 0)
    {
        QMenu menu(this);
        menu.addAction(actionAfficherVenteDetail);
        menu.addAction(action_regenerer_recu_client_vente);
        menu.setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        menu.exec(event->globalPos());
    }
}


void YerithVentesWindow::clear_all_fields()
{
    comboBox_retour_vente_nom_client->clearField();

    lineEdit_details_nom_client->clearField();
    lineEdit_details_reference_produit->clearField();
    lineEdit_details_designation->clearField();
    lineEdit_details_nom_entreprise_fournisseur->clearField();
    lineEdit_details_categorie->clearField();
    lineEdit_details_remise_prix->clearField();
    lineEdit_details_remise_pourcentage->clearField();
    lineEdit_details_tva->clearField();
    lineEdit_details_montant_total->clearField();
    lineEdit_details_nom_caissier->clearField();
    lineEdit_details_localisation->clearField();
    lineEdit_details_quantite_vendue->clearField();
    lineEdit_details_reference_recu_de_vente->clearField();
    lineEdit_details_heure_vente->clearField();
    lineEdit_details_prix_unitaire->clearField();


    lineEdit_retour_vente_numero_de_telephone_client->clear();

    lineEdit_retour_vente_quantite_a_retourner->clear();

    lineEdit_retour_vente_reference_produit->clearField();
    lineEdit_retour_vente_designation->clearField();
    lineEdit_retour_vente_nom_entreprise_fournisseur->clearField();
    lineEdit_retour_vente_categorie->clearField();
    lineEdit_retour_vente_remise_prix->clearField();
    lineEdit_retour_vente_remise_pourcentage->clearField();
    lineEdit_retour_vente_tva->clearField();
    lineEdit_retour_vente_montant_total->clearField();
    lineEdit_retour_vente_nom_caissier->clearField();
    lineEdit_retour_vente_localisation->clearField();
    lineEdit_retour_vente_quantite_vendue->clearField();
    lineEdit_retour_vente_reference_recu_de_vente->clearField();
    lineEdit_retour_vente_heure_vente->clearField();
    lineEdit_retour_vente_prix_unitaire->clearField();

    lineEdit_ventes_BENEFICE->clearField();
    lineEdit_ventes_CAISSE->clearField();
    lineEdit_ventes_recette_totale->clearField();
    lineEdit_ventes_quantite_vendue->clearField();
}


void YerithVentesWindow::setupDateTimeEdits()
{
    dateEdit_details_date_vente->setYerithEnabled(false);

    dateEdit_details_date_peremption->setYerithEnabled(false);

    dateEdit_retour_vente_date_vente->setYerithEnabled(false);

    dateEdit_retour_vente_date_peremption->setYerithEnabled(false);

    dateEdit_ventes_debut->setStartDate(GET_CURRENT_DATE);

    dateEdit_ventes_fin->setStartDate(GET_CURRENT_DATE);

    _ventesDateFilter.clear();

    _ventesDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    							.arg(YerithDatabaseTableColumn::DATE_VENTE,
    								 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->date()),
									 YerithDatabaseTableColumn::DATE_VENTE,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->date())));

    connect(dateEdit_ventes_debut,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerithLineEdits()));

    connect(dateEdit_ventes_fin,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerithLineEdits()));
}


void YerithVentesWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerithWindowsCommons::deconnecter_utilisateur();
}


void YerithVentesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    tabWidget_ventes->removeTab(RetourDuneVente);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    (tableView_ventes);

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}

void YerithVentesWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    if (tabWidget_ventes->count() < 3)
    {
        tabWidget_ventes->insertTab(RetourDuneVente,
                                    _retourVenteTabWidget,
                                    _retourVenteTabWidgetTitle);
    }


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksVenduTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));

    pushButton_ventes_reinitialiser_filtre
        ->enable(this, SLOT(reinitialiser_elements_filtrage()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_retour_details->enable(this, SLOT(retourVentes()));

    pushButton_retour_annuler->enable(this, SLOT(retourVentes()));

    pushButton_annuler_vente->enable(this, SLOT(annuler_cette_vente()));
}


void YerithVentesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    tabWidget_ventes->removeTab(RetourDuneVente);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksVenduTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));

    pushButton_ventes_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_retour_details->enable(this, SLOT(retourVentes()));

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}


void YerithVentesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}


void YerithVentesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


void YerithVentesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    tabWidget_ventes->removeTab(RetourDuneVente);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    (tableView_ventes);

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}


bool YerithVentesWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_ventes,
                                               tableColumnsToIgnore,
                                               "yerith-erp-cashier-journal-csv-format",
                                               "journal of sales");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_ventes,
                                               tableColumnsToIgnore,
                                               "yerith-erp-journal-ventes-format-csv",
                                               "journal des ventes");
    }

    return success;
}


QString YerithVentesWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _latex_template_print_pdf_content = YerithUtils::EN_template_journal_des_ventes_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_journal_des_ventes_tex;
    }

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESDEBUT",
				DATE_TO_STRING(dateEdit_ventes_debut->date()));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESFIN",
				DATE_TO_STRING(dateEdit_ventes_fin->date()));


    return _latex_template_print_pdf_content;
}


void YerithVentesWindow::resetFilter(YerithSqlTableModel *stocksVenduTableModel)
{
    _curStocksVenduTableModel = stocksVenduTableModel;

    if (0 != _curStocksVenduTableModel)
    {
        _curStocksVenduTableModel->resetFilter();
    }

    lineEdit_ventes_terme_recherche->myClear();


    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enableNomCaissier_ONLY_MANAGER();
        }
        else
        {
            setCurrentUser_NOM_CAISSIER(aUser->nom_complet());
        }
    }
    else
    {
        disableNomCaissier();
    }

    dateEdit_ventes_debut->reset();
    dateEdit_ventes_fin->reset();
}


void YerithVentesWindow::handleTabEnabled()
{
    if (tableView_ventes->rowCount() > 0)
    {
        tabWidget_ventes->setTabEnabled(RetourDuneVente, true);
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, true);
    }
    else
    {
        tabWidget_ventes->setTabEnabled(RetourDuneVente, false);
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, false);
    }
}


void YerithVentesWindow::handleCurrentTabChanged(int index)
{
    //_logger->log("handleCurrentChanged(int)",
    //                  QString("handleCurrentChanged]. index: %1").arg(index));

    switch (index)
    {
    case TableauDesVentes:
        lister_les_elements_du_tableau();
        enable_on_print_actions_SET_VISIBILITY();
        break;

    case AfficherVenteAuDetail:
        if (afficher_vente_detail())
        {
            disable_on_print_actions_SET_VISIBILITY();
        }
        break;

    case RetourDuneVente:
        lineEdit_retour_vente_quantite_a_retourner->setFocus();

        if (afficher_retour_vente())
        {
            disable_on_print_actions_SET_VISIBILITY();
        }
        break;

    default:
        break;
    }

    modifier_visibilite_actions_sur_cette_vente();
}


void YerithVentesWindow::lister_les_elements_du_tableau(YerithSqlTableModel &
                                                        stocksVenduTableModel)
{
    int curStocksVenduTableModelRowCount =
    		_curStocksVenduTableModel->easySelect("src/windows/yerith-erp-ventes-window.cpp", 2914);

    double quantite_vendue = 0;
    double quantite_vendue_total = 0.0;


    QString reference_recu_vendu;

    QMap < QString, double >reference_recu_vendu_TO_remise_FIDELITE_CLIENTS;

    double remise_FIDELITE_CLIENTS = 0.0;
    double remise_totale_FIDELITE_CLIENTS_fcfa = 0.0;


    double remise_prix = 0.0;
    double remise_total_fcfa = 0.0;

    double montant_total = 0.0;
    double montant_total_vente = 0.0;

    double benefice = 0.0;
    double benefice_total = 0.0;

    double montant_tva = 0.0;
    double total_montant_tva = 0.0;


    QSqlRecord aRecord;

    for (int k = 0; k < curStocksVenduTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = _curStocksVenduTableModel->record(k);

        reference_recu_vendu =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::REFERENCE_RECU_VENDU);

        quantite_vendue =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::QUANTITE_VENDUE).
                        toDouble();

        quantite_vendue_total += quantite_vendue;

        montant_total_vente =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        montant_total += montant_total_vente;


        if (!reference_recu_vendu_TO_remise_FIDELITE_CLIENTS.contains
                (reference_recu_vendu))
        {
            remise_FIDELITE_CLIENTS =
                            GET_SQL_RECORD_DATA(aRecord,
                                                YerithDatabaseTableColumn::
                                                MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS).
                            toDouble();

//              QDEBUG_STRING_OUTPUT_2_N("remise_FIDELITE_CLIENTS", remise_FIDELITE_CLIENTS);

            reference_recu_vendu_TO_remise_FIDELITE_CLIENTS.insert
            (reference_recu_vendu, remise_FIDELITE_CLIENTS);
        }


        remise_prix =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::REMISE_PRIX).
                        toDouble();

        remise_total_fcfa += (quantite_vendue * remise_prix);

        benefice =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::MARGE_BENEFICIAIRE).
                        toDouble();

        benefice_total += benefice;
    }

    if (_allWindows->getUser()->isManager())
    {
        lineEdit_ventes_BENEFICE->setText(GET_CURRENCY_STRING_NUM
                                          (benefice_total));
    }
    else
    {
        lineEdit_ventes_BENEFICE->clear();
    }

    QMapIterator < QString,
                 double >it(reference_recu_vendu_TO_remise_FIDELITE_CLIENTS);

    while (it.hasNext())
    {
        it.next();
        remise_totale_FIDELITE_CLIENTS_fcfa += it.value();
    }

    double total_caisse = montant_total - remise_totale_FIDELITE_CLIENTS_fcfa;

    lineEdit_ventes_CAISSE->setText(GET_CURRENCY_STRING_NUM(total_caisse));
    lineEdit_ventes_quantite_vendue->setText(GET_DOUBLE_STRING
                                             (quantite_vendue_total));
    lineEdit_ventes_recette_totale->setText(GET_CURRENCY_STRING_NUM
                                            (montant_total));

    tableView_ventes->queryYerithTableViewCurrentPageContentRow
    (stocksVenduTableModel);

    tableView_show_or_hide_columns(*tableView_ventes);

    handleTabEnabled();
}


void YerithVentesWindow::rendreInvisible()
{
    lineEdit_retour_vente_quantite_a_retourner->clear();
    lineEdit_retour_vente_numero_de_telephone_client->clear();

    YerithWindowsCommons::rendreInvisible();
}


void YerithVentesWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
    retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    _curStocksTableModel = stocksTableModel;

    _curStocksVenduTableModel = &_allWindows->getSqlTableModel_stocks_vendu();

    setYerithSqlTableModel(_curStocksVenduTableModel);

    setupLineEditsQCompleters((QObject *) this);

    populateComboBoxes();

    tabWidget_ventes->setCurrentIndex(TableauDesVentes);

    label_details_tva->setText(QObject::tr("TVA (%1)").
                               arg(YerithERPConfig::currency));

    label_details_remise_prix->setText(QObject::tr("remise (%1)").
                                       arg(YerithERPConfig::currency));

    label_retour_vente_remise_prix->setText(QObject::tr("remise (%1)").
                                            arg(YerithERPConfig::currency));


    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enableNomCaissier_ONLY_MANAGER();
        }
        else
        {
            setCurrentUser_NOM_CAISSIER(aUser->nom_complet());
        }
    }
    else
    {
        disableNomCaissier();
    }

    modifier_visibilite_actions_sur_cette_vente();

    setVisible(true);

    afficher_ventes();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("ventes"),
    								 _curStocksVenduTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    lineEdit_ventes_terme_recherche->setFocus();
}


bool YerithVentesWindow::afficher_retour_vente()
{
    if (tableView_ventes->rowCount() <= 0)
    {
        return false;
    }

    lineEdit_retour_vente_quantite_a_retourner->checkField();

    lineEdit_retour_vente_numero_de_telephone_client->checkField();


    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord record;

    _allWindows->
    _ventesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    bool isService = GET_SQL_RECORD_DATA(record,
                                         YerithDatabaseTableColumn::IS_SERVICE).
                     toInt();

    if (isService)
    {
        QString msg(QObject::tr
                    ("Cette fonctionnalité n'est pas disponible pour les 'services' !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("fonctionnalité pas disponible"),
                                       msg);

        tabWidget_ventes->setCurrentIndex(TableauDesVentes);

        return false;
    }

    lineEdit_retour_vente_reference_produit->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerithDatabaseTableColumn::REFERENCE));
    lineEdit_retour_vente_designation->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::DESIGNATION));
    lineEdit_retour_vente_nom_entreprise_fournisseur->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_retour_vente_categorie->setText(GET_SQL_RECORD_DATA
                                             (record,
                                              YerithDatabaseTableColumn::CATEGORIE));

    double quantite_vendue = GET_SQL_RECORD_DATA(record,
                                                 YerithDatabaseTableColumn::QUANTITE_VENDUE).
                             toDouble();

    lineEdit_retour_vente_quantite_vendue->setText(GET_NUM_STRING
                                                   (quantite_vendue));
    lineEdit_retour_vente_prix_unitaire->setText(GET_CURRENCY_STRING_NUM
                                                 (GET_SQL_RECORD_DATA
                                                  (record,
                                                   "prix_unitaire").toDouble
                                                  ()));
    lineEdit_retour_vente_remise_prix->
    setText(GET_CURRENCY_STRING_NUM
            (GET_SQL_RECORD_DATA
             (record, YerithDatabaseTableColumn::REMISE_PRIX).toDouble()));

    QString remise_pourcentage(QString("%1 %").arg(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerithDatabaseTableColumn::
                                                    REMISE_POURCENTAGE)));

    lineEdit_retour_vente_remise_pourcentage->setText(remise_pourcentage);
    lineEdit_retour_vente_tva->setText(GET_CURRENCY_STRING_NUM
                                       (GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::MONTANT_TVA).
                                        toDouble()));
    lineEdit_retour_vente_montant_total->
    setText(GET_CURRENCY_STRING_NUM
            (GET_SQL_RECORD_DATA
             (record,
              YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble()));

    dateEdit_retour_vente_date_peremption->setDate(record.value
                                                   (YerithDatabaseTableColumn::
                                                    DATE_PEREMPTION).
                                                   toDate());
    dateEdit_retour_vente_date_vente->
    setDate(GET_DATE_FROM_STRING
            (GET_SQL_RECORD_DATA
             (record, YerithDatabaseTableColumn::DATE_VENTE)));

    lineEdit_retour_vente_heure_vente->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::HEURE_VENTE));

    lineEdit_retour_vente_reference_recu_de_vente->setText(GET_SQL_RECORD_DATA
                                                           (record,
                                                            YerithDatabaseTableColumn::REFERENCE_RECU_VENDU));

    lineEdit_retour_vente_nom_caissier->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::NOM_CAISSIER));
    lineEdit_retour_vente_localisation->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::LOCALISATION));


    comboBox_retour_vente_nom_client->populateComboBoxRawString
    (YerithDatabase::CLIENTS, YerithDatabaseTableColumn::NOM_ENTREPRISE);


    YerithSqlTableModel &clientsTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString clientsIdFilter;

    clientsIdFilter.append(QString("%1 = '%2'").arg
                           (YerithDatabaseTableColumn::ID,
                            GET_SQL_RECORD_DATA(record,
                                                YerithDatabaseTableColumn::
                                                CLIENTS_ID)));

    clientsTableModel.yerithSetFilter_WITH_where_clause(clientsIdFilter);

    int clientsTableModelRowCount =
    		clientsTableModel.easySelect("src/windows/yerith-erp-ventes-window.cpp", 3228);

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);

        comboBox_retour_vente_nom_client->setCurrentIndex
        (comboBox_retour_vente_nom_client->findText
         (GET_SQL_RECORD_DATA
          (clientsRecord, YerithDatabaseTableColumn::NOM_ENTREPRISE)));
    }
    else
    {
        comboBox_retour_vente_nom_client->resetYerithComboBox();
    }

    clientsTableModel.resetFilter();

    QString _nom_entreprise_client = GET_SQL_RECORD_DATA(record,
                                                         YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);

    if (-1 ==
            comboBox_retour_vente_nom_client->findText(_nom_entreprise_client))
    {
        comboBox_retour_vente_nom_client->addItem(_nom_entreprise_client);

        comboBox_retour_vente_nom_client->setCurrentIndex
        (comboBox_retour_vente_nom_client->findText(_nom_entreprise_client));
    }


    int typeDeVente = GET_SQL_RECORD_DATA(record,
                                          YerithDatabaseTableColumn::TYPE_DE_VENTE).
                      toInt();

    lineEdit_retour_vente_type_de_vente->
    setText(YerithUtils::_typedeventeToUserViewString.value(typeDeVente));

    tabWidget_ventes->setCurrentIndex(RetourDuneVente);

    return true;
}


bool YerithVentesWindow::afficher_vente_detail()
{
    if (tableView_ventes->rowCount() <= 0)
    {
        return false;
    }

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord record;

    _allWindows->
    _ventesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    lineEdit_details_reference_produit->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::REFERENCE));
    lineEdit_details_designation->setText(GET_SQL_RECORD_DATA
                                          (record,
                                           YerithDatabaseTableColumn::DESIGNATION));
    lineEdit_details_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_details_categorie->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::CATEGORIE));

    double quantite_vendue = GET_SQL_RECORD_DATA(record,
                                                 YerithDatabaseTableColumn::QUANTITE_VENDUE).
                             toDouble();

    lineEdit_details_quantite_vendue->setText(GET_NUM_STRING
                                              (quantite_vendue));
    lineEdit_details_prix_unitaire->setText(GET_CURRENCY_STRING_NUM
                                            (GET_SQL_RECORD_DATA
                                             (record,
                                              "prix_unitaire").toDouble()));
    lineEdit_details_remise_prix->setText(GET_CURRENCY_STRING_NUM
                                          (GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::REMISE_PRIX).
                                           toDouble()));

    QString remise_pourcentage(QString("%1 %").arg(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerithDatabaseTableColumn::
                                                    REMISE_POURCENTAGE)));

    lineEdit_details_remise_pourcentage->setText(remise_pourcentage);
    lineEdit_details_tva->setText(GET_CURRENCY_STRING_NUM
                                  (GET_SQL_RECORD_DATA
                                   (record,
                                    YerithDatabaseTableColumn::
                                    MONTANT_TVA).toDouble()));
    lineEdit_details_montant_total->
    setText(GET_CURRENCY_STRING_NUM
            (GET_SQL_RECORD_DATA
             (record,
              YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble()));

    dateEdit_details_date_peremption->setDate(record.value
                                              (YerithDatabaseTableColumn::
                                               DATE_PEREMPTION).toDate());
    dateEdit_details_date_vente->
    setDate(GET_DATE_FROM_STRING
            (GET_SQL_RECORD_DATA
             (record, YerithDatabaseTableColumn::DATE_VENTE)));

    lineEdit_details_heure_vente->setText(GET_SQL_RECORD_DATA
                                          (record,
                                           YerithDatabaseTableColumn::HEURE_VENTE));

    lineEdit_details_reference_recu_de_vente->setText(GET_SQL_RECORD_DATA
                                                      (record,
                                                       YerithDatabaseTableColumn::REFERENCE_RECU_VENDU));

    lineEdit_details_nom_caissier->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::NOM_CAISSIER));
    lineEdit_details_localisation->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::LOCALISATION));

    YerithSqlTableModel &clientsTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString clientsIdFilter;

    clientsIdFilter.append(QString("%1 = '%2'").arg
                           (YerithDatabaseTableColumn::ID,
                            GET_SQL_RECORD_DATA(record,
                                                YerithDatabaseTableColumn::
                                                CLIENTS_ID)));

    clientsTableModel.yerithSetFilter_WITH_where_clause(clientsIdFilter);

    int clientsTableModelRowCount =
    		clientsTableModel.easySelect("src/windows/yerith-erp-ventes-window.cpp", 3368);

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);
        lineEdit_details_nom_client->setText(GET_SQL_RECORD_DATA
                                             (clientsRecord,
                                              YerithDatabaseTableColumn::NOM_ENTREPRISE));
    }
    else
    {
        lineEdit_details_nom_client->setText(YerithUtils::EMPTY_STRING);
    }

    clientsTableModel.resetFilter();

    lineEdit_details_nom_client->setText(GET_SQL_RECORD_DATA
                                         (record,
                                          YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT));

    int typeDeVente =
            GET_SQL_RECORD_DATA(record,
                                YerithDatabaseTableColumn::TYPE_DE_VENTE).toInt();


    lineEdit_details_type_de_vente
        ->setText(YerithUtils::_typedeventeToUserViewString.value(typeDeVente));

    tabWidget_ventes->setCurrentIndex(AfficherVenteAuDetail);

    return true;
}


void YerithVentesWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_ventes_element_de_vente_resultat->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER;

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithVentesWindow::reinitialiser_recherche()
{
    //  _logger->log("reinitialiser_recherche");
    lineEdit_ventes_element_de_vente_resultat->clear();

    lineEdit_nom_element_string_db->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_stocks_vendu());

    resetLineEditsQCompleters((QObject *) this);

    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser && aUser->isVendeur())
    {
        lineEdit_ventes_nom_caissier->setYerithEnabled(false);
        lineEdit_ventes_nom_caissier->setText(aUser->nom_complet());
    }
    else
    {
        lineEdit_ventes_nom_caissier->clear();
        lineEdit_ventes_nom_caissier->setYerithEnabled(true);
    }

    afficher_ventes();

    lineEdit_ventes_terme_recherche->setFocus();
}


void YerithVentesWindow::refineYerithLineEdits()
{
    _ventesDateFilter.clear();

    _ventesDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                             (YerithDatabaseTableColumn::DATE_VENTE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->
                                                       date()),
                              YerithDatabaseTableColumn::DATE_VENTE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->
                                                       date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_ventes();
}
