/*
 * yerith-erp-achats-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-achats-window.hpp"


#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/QThread>

#include <QtCore/QProcess>

#include <QtCore/qmath.h>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <unistd.h>


/**
 * yerith-erp-windows.hpp cannot be included in
 * the header file because it will caused a circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */


unsigned int YerithAchatsWindow::PDF_LISTING_COLUMN_STOCKS_ID(1);

unsigned int YerithAchatsWindow::PDF_LISTING_COLUMN_REFERENCE(3);

unsigned int YerithAchatsWindow::PDF_LISTING_COLUMN_DESIGNATION(2);

unsigned int YerithAchatsWindow::PDF_LISTING_COLUMN_CATEGORIE(4);


YerithAchatsWindow::YerithAchatsWindow()
:YerithWindowsCommons("yerith-erp-achats"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::ACHATS),
 _logger(new YerithLogger("YerithAchatsWindow")),
 _pushButton_achats_filtrer_font(0),
 _show_ASSET(false),
 _curAchatSqlTableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("fiche des achats des IMMOBILISATIONS et des stocks"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_achats);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
            << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
		<< action_parametrer_les_impressions
		<< actionAfficherPDF << actionAfficher_achat_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_achats)


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_GREEN_2_160_170,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(YerithDatabase::ACHATS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_terme_recherche,
                                           YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString
        .insert(YerithDatabaseTableColumn::REFERENCE);

    _yerith_WINDOW_references_dbColumnString
        .insert(YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerithDatabase::ACHATS);

    reinitialiser_colones_db_visibles();

    _curAchatSqlTableModel = &_allWindows->getSqlTableModel_achats();

    if (!_show_ASSET)
    {
        radioButton_lister_des_stocks->setChecked(true);
    }
    else
    {
        radioButton_lister_des_IMMOBILISATIONS->setChecked(true);
    }

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    _pushButton_achats_filtrer_font =
                    new QFont(pushButton_achats_filtrer->font());

    tableView_achats->setSqlTableName(&YerithDatabase::ACHATS);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);
    pushButton_achats_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));


    //Menu actions

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


    connect(action_BASCULER_STOCK_IMMOBILISATION,
            SIGNAL(triggered()),
            this, SLOT(handle_ACTION_CHOICE_IMMOBILISATION_STOCK()));


    connect(radioButton_lister_des_stocks,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_RADIO_BUTTONS_immobilisations_STOCKS(bool)));

    connect(radioButton_lister_des_IMMOBILISATIONS,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_RADIO_BUTTONS_immobilisations_STOCKS(bool)));

    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));


    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionVentes, SIGNAL(triggered()), this, SLOT(ventes()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this,
            SLOT(reinitialiser_elements_filtrage()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
#endif

    connect(actionAfficher_achat_au_detail, SIGNAL(triggered()),
            this, SLOT(afficher_au_detail()));

    connect(tableView_achats, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(afficher_au_detail(const QModelIndex &)));


    setCenterAligned_FOR_YERITH_PDF_LATEX_PRINTING
    (YerithDatabaseTableColumn::STOCKS_ID);

    setupShortcuts();
}

YerithAchatsWindow::~YerithAchatsWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithAchatsWindow::handle_ACTION_CHOICE_IMMOBILISATION_STOCK()
{
    if (radioButton_lister_des_stocks->isChecked())
    {
        radioButton_lister_des_IMMOBILISATIONS->setChecked(true);
    }
    else if (radioButton_lister_des_IMMOBILISATIONS->isChecked())
    {
        radioButton_lister_des_stocks->setChecked(true);
    }
}


void YerithAchatsWindow::handle_RADIO_BUTTONS_immobilisations_STOCKS(bool
                                                                     is_toggle)
{
    if (radioButton_lister_des_stocks->isChecked())
    {
        action_BASCULER_STOCK_IMMOBILISATION->setText(QObject::tr
                                                      ("AFFICHER ACHATS IMMOBILISATIONS"));

        _show_ASSET = false;
    }
    else if (radioButton_lister_des_IMMOBILISATIONS->isChecked())
    {
        action_BASCULER_STOCK_IMMOBILISATION->setText(QObject::tr
                                                      ("AFFICHER ACHATS stocks"));

        _show_ASSET = true;
    }

    textChangedSearchLineEditsQCompleters();
}


void YerithAchatsWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element_achats->setVisible(IS_VISIBLE);
    comboBox_element_achats_condition->setVisible(IS_VISIBLE);
    lineEdit_element_achats_resultat->setVisible(IS_VISIBLE);
    pushButton_achats_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_achats_filtrer->setVisible(IS_VISIBLE);
}


bool YerithAchatsWindow::filtrer_achats()
{
    QString achatsTableColumnValue(lineEdit_element_achats_resultat->text());

    if (achatsTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);

        return false;
    }

    reinitialiser_recherche();

    QString achatsTableColumnProperty(comboBox_element_achats->currentText());

    QString mathOperator(comboBox_element_achats_condition->currentText());

    QString
    REAL_DB_ID_NAME_achatsTableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.key
     (achatsTableColumnProperty));

    QString filterString;

    filterString.append(QString("%1 %2 %3").arg
                        (REAL_DB_ID_NAME_achatsTableColumnProperty,
                         mathOperator, achatsTableColumnValue));

    _curAchatSqlTableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curAchatSqlTableModel->easySelect("src/windows/purchase/yerith-erp-achats-window.cpp", 329);

    if (resultRows > 0)
    {
        setCurrentlyFiltered(true);

        lister_les_elements_du_tableau(*_curAchatSqlTableModel);

        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "achats - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "achats - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerithAchatsWindow::initialiser_FILTRES()
{
    comboBox_element_achats->setVisible(false);
    comboBox_element_achats_condition->setVisible(false);
    lineEdit_element_achats_resultat->setVisible(false);
    pushButton_achats_reinitialiser_filtre->setVisible(false);
    pushButton_achats_filtrer->setVisible(false);
}


void YerithAchatsWindow::populateComboBoxes()
{
    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::ENREGISTREUR_STOCK);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::MARGE_BENEFICIAIRE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::MONTANT_TVA));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::PRIX_VENTE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::PRIX_UNITAIRE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::QUANTITE_TOTALE));


    comboBox_element_achats->clear();

    comboBox_element_achats->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");


    comboBox_element_achats_condition->clear();

    comboBox_element_achats_condition->addItems(aQStringList);
}


void YerithAchatsWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_achats_nombre_de_lignes_par_page;

    lineEdit_achats_terme_recherche->enableForSearch(QObject::tr
                                                     ("terme à rechercher (désignation de l'article)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_achats_total_achats->setYerithEnabled(false);

    lineEdit_achats_nombre_dachats->setYerithEnabled(false);

    lineEdit_element_achats_resultat->
    setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_achats_nombre_de_lignes_par_page, tableView_achats);
}


void YerithAchatsWindow::setupDateTimeEdits()
{
    dateEdit_achats_debut->
    setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_achats_fin->setStartDate(GET_CURRENT_DATE);

    _achatsDateFilter.clear();

    _achatsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                             (YerithDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_debut->
                                                       date()),
                              YerithDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_fin->
                                                       date())));

    connect(dateEdit_achats_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));

    connect(dateEdit_achats_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));
}


void YerithAchatsWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionReinitialiserRecherche->
    setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithAchatsWindow::resetFilter(YerithSqlTableModel *achatsTableModel)
{
    _curAchatSqlTableModel = achatsTableModel;

    if (0 != _curAchatSqlTableModel)
    {
        _curAchatSqlTableModel->resetFilter();
    }

    lineEdit_achats_terme_recherche->myClear();

    dateEdit_achats_debut->reset();
    dateEdit_achats_fin->reset();
}


void YerithAchatsWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();

    if (0 != _curAchatSqlTableModel)
    {
        lister_les_elements_du_tableau(*_curAchatSqlTableModel);
    }
}


void YerithAchatsWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_achats_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("%1").arg(GENERATE_SQL_LIKE_STMT
                                                   (YerithDatabaseTableColumn::DESIGNATION,
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

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER;

	if (_show_ASSET)
    {
        if (_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" %1 <= 0").arg
                                 (YerithDatabaseTableColumn::PRIX_VENTE));
        }
        else
        {
            _searchFilter.append(QString(" AND %1 <= 0").arg
                                 (YerithDatabaseTableColumn::PRIX_VENTE));
        }
    }
    else
    {
        if (_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" %1 > 0").arg
                                 (YerithDatabaseTableColumn::PRIX_VENTE));
        }
        else
        {
            _searchFilter.append(QString(" AND %1 > 0").arg
                                 (YerithDatabaseTableColumn::PRIX_VENTE));
        }
    }




    QString finalSearchFilter(_achatsDateFilter);

    if (!_searchFilter.isEmpty())
    {
        QString
        searchFilterWithDate(QString("%1 AND (%2)").arg
                             (_achatsDateFilter, _searchFilter));

        finalSearchFilter = searchFilterWithDate;
    }

    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(finalSearchFilter);

    if (_yerithSqlTableModel->select())
    {
        lister_les_elements_du_tableau(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithAchatsWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithAchatsWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
            << YerithDatabaseTableColumn::CATEGORIE
            << YerithDatabaseTableColumn::PRIX_DACHAT
            << YerithDatabaseTableColumn::PRIX_VENTE
            << YerithDatabaseTableColumn::MARGE_BENEFICIAIRE
            << YerithDatabaseTableColumn::DATE_ENTREE
            << YerithDatabaseTableColumn::QUANTITE_TOTALE;

}


void YerithAchatsWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tableView_achats->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_achatsWindow->palette());
        menu.addAction(actionAfficher_achat_au_detail);
        menu.exec(event->globalPos());
    }
}


void YerithAchatsWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setYerithSqlTableModel(_curAchatSqlTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_achats_terme_recherche->setFocus();

    setVisible(true);

    afficher_achats();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("fiche des achats des IMMOBILISATIONS et des stocks"),
    								 _curAchatSqlTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerithAchatsWindow::definirManager()
{
    _logger->log("definirManager");

    tableView_achats->setVisible(true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curAchatSqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
}


void YerithAchatsWindow::definirVendeur()
{
    _logger->log("definirVendeur - definirPasDeRole()");

    _logger->log("definirManager");

    tableView_achats->setVisible(true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curAchatSqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
}


void YerithAchatsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    tableView_achats->setVisible(true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curAchatSqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_ventes->disable(this);
}


void YerithAchatsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    tableView_achats->setVisible(false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);

    pushButton_achats_filtrer->disable(this);
    pushButton_achats_reinitialiser_filtre->disable(this);

    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerithAchatsWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1
            && _curAchatSqlTableModel->rowCount() > 0)
    {
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
                                                        _curAchatSqlTableModel);
        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 achat à afficher les détails."));

        return;
    }
}


void YerithAchatsWindow::afficher_au_detail(const QModelIndex &modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    if (_curAchatSqlTableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
                                                        _curAchatSqlTableModel);

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 achat à afficher les détails."));

        return;
    }
}


void YerithAchatsWindow::refineYerithLineEdits()
{
    _achatsDateFilter.clear();

    _achatsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                             (YerithDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_debut->
                                                       date()),
                              YerithDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_fin->
                                                       date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_achats();
}


void YerithAchatsWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_element_achats_resultat->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithAchatsWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_nom_element_string_db->clear();

    lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_achats());

    resetLineEditsQCompleters((QObject *) this);

    afficher_achats();
}


void YerithAchatsWindow::lister_les_elements_du_tableau(YerithSqlTableModel &
                                                        achatSqlTableModel)
{
    int curAchatsTableModelRowCount =
    		achatSqlTableModel.easySelect("src/windows/purchase/yerith-erp-achats-window.cpp", 972);

    double quantite_totale = 0.0;
    double prix_dachat = 0.0;
    double montant_total_achat = 0.0;

    double montant_total = 0.0;

    QSqlRecord aRecord;

    for (int k = 0; k < curAchatsTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = achatSqlTableModel.record(k);

        quantite_totale =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        prix_dachat =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::PRIX_DACHAT).
                        toDouble();

        montant_total_achat = prix_dachat * quantite_totale;

        montant_total += montant_total_achat;
    }

    lineEdit_achats_total_achats->setText(GET_CURRENCY_STRING_NUM
                                          (montant_total));


    tableView_achats->queryYerithTableViewCurrentPageContentRow
    (achatSqlTableModel);

    tableView_show_or_hide_columns(*tableView_achats);

    int rowCount = tableView_achats->rowCount();

    lineEdit_achats_nombre_dachats->setText(GET_NUM_STRING(rowCount));



    if (_show_ASSET)
    {
        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("fiche des achats des IMMOBILISATIONS"),
        								 _curAchatSqlTableModel);
    }
    else
    {
        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("fiche des achats de stocks"),
        								 _curAchatSqlTableModel);
    }
}


void YerithAchatsWindow::afficher_stock_selectioner(const QString &stockName)
{
    _logger->log("afficher_stock_selectioner(const QString &)");

    //qDebug() << QString("afficher_stock_selectioner(%1)").arg(stockName);

    QString
    filter(GENERATE_SQL_IS_STMT
           (YerithDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curAchatSqlTableModel->yerithSetFilter_WITH_where_clause(filter);

    if (_curAchatSqlTableModel->easySelect("src/windows/purchase/yerith-erp-achats-window.cpp", 1050) > 0)
    {
        lister_les_elements_du_tableau(*_curAchatSqlTableModel);
    }
}


void YerithAchatsWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_achats_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_achats_filtrer_font->setUnderline(false);
    }

    pushButton_achats_filtrer->setFont(*_pushButton_achats_filtrer_font);
}


bool YerithAchatsWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_achats,
                                               tableColumnsToIgnore,
                                               "yerith-erp-stock-purchase-listing",
                                               "stock purchase listing file");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_achats,
                                               tableColumnsToIgnore,
                                               "yerith-erp-fiche-achats",
                                               "fiche des achats");
    }

    return success;
}


QString YerithAchatsWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_lister_achats_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_lister_achats_tex;
    }

    return _latex_template_print_pdf_content;
}
