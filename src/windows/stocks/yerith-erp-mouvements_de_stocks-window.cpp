/*
 * yerith-erp-mouvements_de_stocks-window.cpp
 *      Author: Prof Dr Xavier Noundou
 */
#include "yerith-erp-mouvements_de_stocks-window.hpp"


#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-utils.hpp"



#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcessEnvironment>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QDesktopWidget>


const QString YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH("CHECKED-OUT");

const QString YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_FRENCH("SORTIES");


const QString YerithMouvementsDeStocksWindow::TRANSFERT_DE_STOCKS_TEXT_STRING_ENGLISH("TRANSFERED");

const QString YerithMouvementsDeStocksWindow::TRANSFERT_DE_STOCKS_TEXT_STRING_FRENCH("TRANFERTS");


YerithMouvementsDeStocksWindow::YerithMouvementsDeStocksWindow()
:YerithWindowsCommons("yerith-erp-mouvements_de_stocks-stocks"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::STOCKS_SORTIES),
 _logger(new YerithLogger("YerithMouvementsDeStocksWindow")),
 _mouvements_de_stocks_DateFilter(YerithUtils::EMPTY_STRING),
 _curMouvementsDeStocksTableModel(&_allWindows->getSqlTableModel_stocks_sorties())
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("TRANSFERTS ET/OU SORTIES de IMMOBILISATIONS / stocks"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_mouvements_de_stocks);


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficher_lhistorique_de_ce_stock << actionAfficherPDF;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION
    (tableView_mouvements_de_stocks);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_BLUE_77_93_254,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::HISTORIQUE_STOCK;

    setup_select_configure_dbcolumn(YerithDatabase::STOCKS_SORTIES);


    _lineEditsToANDContentForSearch.insert
    (&lineEdit_mouvementsDeStocks_terme_recherche, YerithUtils::EMPTY_STRING);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION
    (YerithDatabase::STOCKS_SORTIES);

    reinitialiser_colones_db_visibles();

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    tableView_mouvements_de_stocks->
    setSqlTableName(&YerithDatabase::STOCKS_SORTIES);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    _pushButton_mouvements_de_stocks_filtrer_font =
                    new QFont(pushButton_mouvementsDeStocks_filtrer->font());

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_mouvementsDeStocks_filtrer->disable(this);

    pushButton_mouvementsDeStocks_reinitialiser_filtre->disable(this);


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


    connect(action_BASCULER_SORTIES_TRANSFERTS,
            SIGNAL(triggered()),
            this, SLOT(handle_ACTION_CHOICE_SORTIES_TRANSFERTS()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));


    connect(actionAfficher_lhistorique_de_ce_stock, SIGNAL(triggered()),
            this, SLOT(private_slot_afficher_historique_du_stock()));


    // Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#ifdef YERITH_SERVER
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(comboBox_type_de_mouvement_de_stocks,
            SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(handle_combobox_type_de_mouvement(const QString &)));

    setupShortcuts();
}


void YerithMouvementsDeStocksWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    lister_les_elements_du_tableau();
}


YerithMouvementsDeStocksWindow::~YerithMouvementsDeStocksWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithMouvementsDeStocksWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_mouvementsDeStocks_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_mouvementsDeStocks_terme_recherche->text());

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
                                                   (YerithDatabaseTableColumn::NOTES,
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

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER
    QString finalSearchFilter(_mouvements_de_stocks_DateFilter);

    if (!_searchFilter.isEmpty())
    {
        QString
        searchFilterWithDate(QString("%1 AND (%2)").arg
                             (_mouvements_de_stocks_DateFilter,
                              _searchFilter));

        finalSearchFilter = searchFilterWithDate;
    }

    if (_curMouvementsDeStocksTableModel->select())
    {
        lister_les_elements_du_tableau();
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithMouvementsDeStocksWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_curMouvementsDeStocksTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithMouvementsDeStocksWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DATE_SORTIE
            << YerithDatabaseTableColumn::HEURE_SORTIE
            << YerithDatabaseTableColumn::REFERENCE
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
            << YerithDatabaseTableColumn::CATEGORIE
            << YerithDatabaseTableColumn::QUANTITE_SORTIE
            << YerithDatabaseTableColumn::LOCALISATION_ENTREE
            << YerithDatabaseTableColumn::LOCALISATION_SORTIE;
}


void YerithMouvementsDeStocksWindow::hideEvent(QHideEvent *hideEvent)
{
    YerithWindowsCommons::
    CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
}


void YerithMouvementsDeStocksWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithMouvementsDeStocksWindow::setFilter()
{
    _curMouvementsDeStocksTableModel->yerithSetFilter_WITH_where_clause(_searchFilter);
}


void YerithMouvementsDeStocksWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    _curStocksTableModel = stocksTableModel;

    _curMouvementsDeStocksTableModel =
                    &_allWindows->getSqlTableModel_stocks_sorties();

    setYerithSqlTableModel(_curMouvementsDeStocksTableModel);

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    handle_combobox_type_de_mouvement(comboBox_type_de_mouvement_de_stocks->currentText());

    setVisible(true);

    afficher_stocks_sorties_OU_transferes();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("TRANSFERTS ET/OU SORTIES de IMMOBILISATIONS / stocks"),
    								 _curMouvementsDeStocksTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    lineEdit_mouvementsDeStocks_terme_recherche->setFocus();
}


void YerithMouvementsDeStocksWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_mouvements_de_stocks_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_mouvements_de_stocks_filtrer_font->setUnderline(false);
    }

    pushButton_mouvementsDeStocks_filtrer->setFont
    (*_pushButton_mouvements_de_stocks_filtrer_font);
}


void YerithMouvementsDeStocksWindow::private_slot_afficher_historique_du_stock()
{
    //qDebug() << QString("lastSelectedRow__ID: %1")
    //                              .arg(QString::number(lastSelectedRow__ID));

    if (0 != _curStocksTableModel && _curStocksTableModel->rowCount() > 0)
    {
        QSqlRecord record;

        _allWindows->
        _mouvementsDeStocksWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
        (record);

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


void YerithMouvementsDeStocksWindow::handle_ACTION_CHOICE_SORTIES_TRANSFERTS()
{
    QString type_de_mouvement;

#ifdef YERITH_FRANCAIS_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                            YerithMouvementsDeStocksWindow::
                                            SORTIE_DE_STOCKS_TEXT_STRING_FRENCH))
    {
        type_de_mouvement =
                        YerithMouvementsDeStocksWindow::
                        TRANSFERT_DE_STOCKS_TEXT_STRING_FRENCH;
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (_cur_type_de_mouvements_de_stocks,
              YerithMouvementsDeStocksWindow::
              TRANSFERT_DE_STOCKS_TEXT_STRING_FRENCH))
    {
        type_de_mouvement =
                        YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_FRENCH;
    }

#else //YERITH_ENGLISH_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                            YerithMouvementsDeStocksWindow::
                                            SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH))
    {
        type_de_mouvement =
                        YerithMouvementsDeStocksWindow::
                        TRANSFERT_DE_STOCKS_TEXT_STRING_ENGLISH;
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (_cur_type_de_mouvements_de_stocks,
              YerithMouvementsDeStocksWindow::
              TRANSFERT_DE_STOCKS_TEXT_STRING_ENGLISH))
    {
        type_de_mouvement =
                        YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH;
    }

#endif

    comboBox_type_de_mouvement_de_stocks->find_AND_SET_CURRENT_INDEX
    (type_de_mouvement);
}


void YerithMouvementsDeStocksWindow::handle_combobox_type_de_mouvement(const
                                                                       QString &
                                                                       type_de_mouvement)
{
    _cur_type_de_mouvements_de_stocks = type_de_mouvement;

#ifdef YERITH_FRANCAIS_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                            YerithMouvementsDeStocksWindow::
                                            SORTIE_DE_STOCKS_TEXT_STRING_FRENCH))
    {
        action_BASCULER_SORTIES_TRANSFERTS->setText("Afficher TRANSFERTS");

        label_quantite_mouvements->setText("# sorties");
        label_quantite_sortie->setText("# sortie");
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (_cur_type_de_mouvements_de_stocks,
              YerithMouvementsDeStocksWindow::
              TRANSFERT_DE_STOCKS_TEXT_STRING_FRENCH))
    {
        action_BASCULER_SORTIES_TRANSFERTS->setText("Afficher SORTIES");

        label_quantite_mouvements->setText("# transferts");
        label_quantite_sortie->setText("# transférée");
    }

#else //YERITH_ENGLISH_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                            YerithMouvementsDeStocksWindow::
                                            SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH))
    {
        action_BASCULER_SORTIES_TRANSFERTS->setText("VIEW TRANSFERS");

        label_quantite_sortie->setText("# checked-out");
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (_cur_type_de_mouvements_de_stocks,
              YerithMouvementsDeStocksWindow::
              TRANSFERT_DE_STOCKS_TEXT_STRING_ENGLISH))
    {
        action_BASCULER_SORTIES_TRANSFERTS->setText("VIEW CHECKED-OUT");

        label_quantite_sortie->setText("# transfered");
    }

#endif

    textChangedSearchLineEditsQCompleters();
}


void YerithMouvementsDeStocksWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_mouvementsDeStocks_element_de_sortie->setVisible(IS_VISIBLE);
    comboBox_mouvementsDeStocks_element_de_sorties_condition->setVisible
    (IS_VISIBLE);
    lineEdit_mouvementsDeStocks_resultat->setVisible(IS_VISIBLE);
    pushButton_mouvementsDeStocks_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_mouvementsDeStocks_filtrer->setVisible(IS_VISIBLE);
}


bool YerithMouvementsDeStocksWindow::filtrer_stocks_sorties_OU_transferes()
{
    QString
    sortie_de_stocks_table_column_value
    (lineEdit_mouvementsDeStocks_resultat->text());

    if (sortie_de_stocks_table_column_value.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;

    }

    QString
    sortie_de_stocks_table_column_property
    (comboBox_mouvementsDeStocks_element_de_sortie->currentText());

    QString
    mathOperator
    (comboBox_mouvementsDeStocks_element_de_sorties_condition->currentText
     ());

    QString
    REAL_DB_ID_NAME_sortie_de_stocks_table_column_property
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.key
     (sortie_de_stocks_table_column_property));

    QString
    filterString(QString("%1 AND (%2 %3 %4)").arg
                 (_mouvements_de_stocks_DateFilter,
                  REAL_DB_ID_NAME_sortie_de_stocks_table_column_property,
                  mathOperator, sortie_de_stocks_table_column_value));

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

    _curMouvementsDeStocksTableModel->yerithSetFilter_WITH_where_clause
    (_searchFilter);

    int resultRows =
    		_curMouvementsDeStocksTableModel
				->easySelect("src/windows/stocks/yerith-erp-mouvements_de_stocks-window.cpp", 651);

    lister_les_elements_du_tableau();

    if (resultRows > 0)
    {
        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "TRANSFERTS ET/OU SORTIES de stocks - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this,
                                                  "TRANSFERTS ET/OU SORTIES de stocks - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerithMouvementsDeStocksWindow::initialiser_FILTRES()
{
    comboBox_mouvementsDeStocks_element_de_sortie->setVisible(false);
    comboBox_mouvementsDeStocks_element_de_sorties_condition->setVisible
    (false);
    lineEdit_mouvementsDeStocks_resultat->setVisible(false);
    pushButton_mouvementsDeStocks_reinitialiser_filtre->setVisible(false);
    pushButton_mouvementsDeStocks_filtrer->setVisible(false);
}


void YerithMouvementsDeStocksWindow::populateComboBoxes()
{
    QStringList aQStringList;

#ifdef YERITH_FRANCAIS_LANGUAGE

    _cur_type_de_mouvements_de_stocks =
                    YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_FRENCH;

    aQStringList.append
    (YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_FRENCH);

    aQStringList.append
    (YerithMouvementsDeStocksWindow::TRANSFERT_DE_STOCKS_TEXT_STRING_FRENCH);

#else //YERITH_ENGLISH_LANGUAGE

    _cur_type_de_mouvements_de_stocks =
                    YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH;

    aQStringList.append
    (YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH);

    aQStringList.append
    (YerithMouvementsDeStocksWindow::TRANSFERT_DE_STOCKS_TEXT_STRING_ENGLISH);

#endif

    comboBox_type_de_mouvement_de_stocks->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::HISTORIQUE_STOCK);
    aQStringList.
    removeAll(YerithDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER);
    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_PEREMPTION);
    aQStringList.removeAll(YerithDatabaseTableColumn::NOTES);

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::QUANTITE_SORTIE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::PRIX_UNITAIRE));

    comboBox_mouvementsDeStocks_element_de_sortie->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");

    comboBox_mouvementsDeStocks_element_de_sorties_condition->addItems
    (aQStringList);
}


void YerithMouvementsDeStocksWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_mouvementsDeStocks_nombre_de_lignes_par_page;

    lineEdit_mouvementsDeStocks_terme_recherche->
    enableForSearch(QObject::tr("terme à rechercher (notes)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_mouvementsDeStocks_resultat->
    setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_mouvementsDeStocks_quantite_mouvements->setYerithEnabled(false);

    lineEdit_mouvementsDeStocks_quantite_sortie->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_mouvementsDeStocks_nombre_de_lignes_par_page,
     tableView_mouvements_de_stocks);
}


void YerithMouvementsDeStocksWindow::setupDateTimeEdits()
{
    dateEdit_mouvementsDeStocks_debut->
    setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_mouvementsDeStocks_fin->setStartDate(GET_CURRENT_DATE);

    _mouvements_de_stocks_DateFilter.clear();

    _mouvements_de_stocks_DateFilter.append(QString
                                            (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                            arg
                                            (YerithDatabaseTableColumn::DATE_SORTIE,
                                             DATE_TO_DB_FORMAT_STRING
                                             (dateEdit_mouvementsDeStocks_debut->
                                              date()),
                                             YerithDatabaseTableColumn::DATE_SORTIE,
                                             DATE_TO_DB_FORMAT_STRING
                                             (dateEdit_mouvementsDeStocks_fin->
                                              date())));

    connect(dateEdit_mouvementsDeStocks_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));

    connect(dateEdit_mouvementsDeStocks_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));
}


void YerithMouvementsDeStocksWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curMouvementsDeStocksTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_mouvementsDeStocks_filtrer->enable(this,
                                                  SLOT
                                                  (filtrer_stocks_sorties_OU_transferes
                                                   ()));

    pushButton_mouvementsDeStocks_reinitialiser_filtre->enable(this,
                                                               SLOT
                                                               (reinitialiser_elements_filtrage
                                                                ()));
}


void YerithMouvementsDeStocksWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curMouvementsDeStocksTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_mouvementsDeStocks_filtrer->enable(this,
                                                  SLOT
                                                  (filtrer_stocks_sorties_OU_transferes
                                                   ()));

    pushButton_mouvementsDeStocks_reinitialiser_filtre->enable(this,
                                                               SLOT
                                                               (reinitialiser_elements_filtrage
                                                                ()));
}

void YerithMouvementsDeStocksWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curMouvementsDeStocksTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_mouvementsDeStocks_filtrer->enable(this,
                                                  SLOT
                                                  (filtrer_stocks_sorties_OU_transferes
                                                   ()));

    pushButton_mouvementsDeStocks_reinitialiser_filtre->enable(this,
                                                               SLOT
                                                               (reinitialiser_elements_filtrage
                                                                ()));
}

void YerithMouvementsDeStocksWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    (tableView_mouvements_de_stocks);

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_mouvementsDeStocks_filtrer->disable(this);
    pushButton_mouvementsDeStocks_reinitialiser_filtre->disable(this);
}


bool YerithMouvementsDeStocksWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    int currentTableViewCurrentIndex = -1;

    currentTableViewCurrentIndex =
                    tabWidget_mouvementsDeStocks->currentIndex();

    YerithTableView *currentYerithTableView = 0;

    if (0 == currentTableViewCurrentIndex)
    {
        currentYerithTableView = tableView_mouvements_de_stocks;
    }
    else if (1 == currentTableViewCurrentIndex)
    {
        currentYerithTableView = tableView_mouvements_de_stocks;
    }
    else
    {
        //TODO: insert QDialog message box here
        return success;
    }

    QStandardItemModel *tableModel =
                    tableView_mouvements_de_stocks->getStandardItemModel();

    if (0 == tableModel)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("pas de données à exporter au format csv"),
                                       QObject::tr
                                       ("Il n'y a pas de données à exporter au format csv !"));
        return success;
    }

    QList < int >tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

#ifdef YERITH_FRANCAIS_LANGUAGE
    success = YerithUtils::export_csv_file(*this,
                                           *currentYerithTableView,
                                           tableColumnsToIgnore,
                                           "yerith-erp-fichier-stock-sortie-format-csv",
                                           "fiche des comptes clients");
#endif

#ifdef YERITH_ENGLISH_LANGUAGE
    success = YerithUtils::export_csv_file(*this,
                                           *currentYerithTableView,
                                           tableColumnsToIgnore,
                                           "yerith-erp-stock-transfert-csv-format",
                                           "customer account listing file");
#endif

    return success;
}


QString YerithMouvementsDeStocksWindow::get_latex_template_print_pdf_content()
{
    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        if (YerithUtils::isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                                YerithMouvementsDeStocksWindow::
                                                SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH))
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT", "Journal of outgoing articles");
        }
        else
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT", "Journal of article transfers");
        }

        _latex_template_print_pdf_content = YerithUtils::EN_template_liste_des_mouvements_de_stocks_tex;
    }
    else
    {
        if (YerithUtils::isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                                YerithMouvementsDeStocksWindow::
                                                SORTIE_DE_STOCKS_TEXT_STRING_FRENCH))
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT", "Journal des sorties d'articles");
        }
        else
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT", "Journal des transferts d'articles");
        }

        _latex_template_print_pdf_content = YerithUtils::FR_template_liste_des_mouvements_de_stocks_tex;
    }


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESDEBUT",
				DATE_TO_STRING(dateEdit_mouvementsDeStocks_debut->date()));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESFIN",
				DATE_TO_STRING(dateEdit_mouvementsDeStocks_fin->date()));


    return _latex_template_print_pdf_content;
}


void YerithMouvementsDeStocksWindow::lister_les_elements_du_tableau()
{
    _logger->log("lister_les_elements_du_tableau");

//    QDEBUG_STRING_OUTPUT_2("_searchFilter - *", _searchFilter);

    if (_searchFilter.isEmpty())
    {
        _searchFilter.append(QString(" %1 >= '%2' AND %3 <= '%4'").arg
                             (YerithDatabaseTableColumn::DATE_SORTIE,
                              DATE_TO_DB_FORMAT_STRING
                              (dateEdit_mouvementsDeStocks_debut->date()),
                              YerithDatabaseTableColumn::DATE_SORTIE,
                              DATE_TO_DB_FORMAT_STRING
                              (dateEdit_mouvementsDeStocks_fin->date())));
    }
    else
    {
        _searchFilter.append(QString(" AND (%1 >= '%2' AND %3 <= '%4')").arg
                             (YerithDatabaseTableColumn::DATE_SORTIE,
                              DATE_TO_DB_FORMAT_STRING
                              (dateEdit_mouvementsDeStocks_debut->date()),
                              YerithDatabaseTableColumn::DATE_SORTIE,
                              DATE_TO_DB_FORMAT_STRING
                              (dateEdit_mouvementsDeStocks_fin->date())));
    }


//    QDEBUG_STRING_OUTPUT_2("_searchFilter - ***", _searchFilter);


#ifdef YERITH_FRANCAIS_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive
            (comboBox_type_de_mouvement_de_stocks->currentText(),
             YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_FRENCH))
    {
        _searchFilter.append(" AND localisation_entree = ''");
    }
    else
    {
        _searchFilter.append(" AND localisation_entree != ''");
    }

#endif

#ifdef YERITH_ENGLISH_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive
            (comboBox_type_de_mouvement_de_stocks->currentText(),
             YerithMouvementsDeStocksWindow::SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH))
    {
        _searchFilter.append(" AND localisation_entree = ''");
    }
    else
    {
        _searchFilter.append(" AND localisation_entree != ''");
    }

#endif


    _curMouvementsDeStocksTableModel->yerithSetFilter_WITH_where_clause
    (_searchFilter);

    //qDebug() << "++ lister_les_elements_du_tableau, _searchFilter: " << _curMouvementsDeStocksTableModel->filter();

    tableView_mouvements_de_stocks->lister_les_elements_du_tableau
    (*_curMouvementsDeStocksTableModel);


    if (YerithUtils::isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                            YerithMouvementsDeStocksWindow::
                                            SORTIE_DE_STOCKS_TEXT_STRING_FRENCH)
            || YerithUtils::
            isEqualCaseInsensitive(_cur_type_de_mouvements_de_stocks,
                                   YerithMouvementsDeStocksWindow::
                                   SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH))
    {
        if (_visibleDBColumnNameStrList.contains
                (YerithDatabaseTableColumn::LOCALISATION_ENTREE))
        {
            _visibleDBColumnNameStrList.
            removeAll(YerithDatabaseTableColumn::LOCALISATION_ENTREE);
        }
    }
    else
    {
        if (!_visibleDBColumnNameStrList.contains
                (YerithDatabaseTableColumn::LOCALISATION_ENTREE))
        {
            _visibleDBColumnNameStrList.
            append(YerithDatabaseTableColumn::LOCALISATION_ENTREE);
        }
    }


    tableView_mouvements_de_stocks->lister_les_elements_du_tableau
    (*_curMouvementsDeStocksTableModel);

    tableView_show_or_hide_columns(*tableView_mouvements_de_stocks);

    double quantite_sortie_OU_transferee = 0;
    double quantite_sortie_OU_transferee_TOTAL = 0;

    int curMouvementsDeStocksTableModelRowCount =
    		_curMouvementsDeStocksTableModel->easySelect("src/windows/stocks/yerith-erp-mouvements_de_stocks-window.cpp", 1231);

    QSqlRecord aRecord;

    for (int j = 0; j < curMouvementsDeStocksTableModelRowCount; ++j)
    {
        aRecord = _curMouvementsDeStocksTableModel->record(j);

        quantite_sortie_OU_transferee =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::QUANTITE_SORTIE).
                        toDouble();

        quantite_sortie_OU_transferee_TOTAL += quantite_sortie_OU_transferee;
    }

    lineEdit_mouvementsDeStocks_quantite_mouvements->setText(QString::number
                                                             (curMouvementsDeStocksTableModelRowCount));

    lineEdit_mouvementsDeStocks_quantite_sortie->setText(GET_DOUBLE_STRING
                                                         (quantite_sortie_OU_transferee_TOTAL));

    tableView_mouvements_de_stocks->queryYerithTableViewCurrentPageContentRow
    (*_curMouvementsDeStocksTableModel);

    tableView_show_or_hide_columns(*tableView_mouvements_de_stocks);
}


void YerithMouvementsDeStocksWindow::resetFilter(YerithSqlTableModel *
                                                 sortie_stocks_table_model)
{
    _searchFilter.clear();

    if (0 != sortie_stocks_table_model)
    {
        sortie_stocks_table_model->resetFilter();
    }

    lineEdit_mouvementsDeStocks_terme_recherche->clear();

    if (_allWindows->getUser()->isManager()
            || _allWindows->getUser()->isMagasinier())
    {
        dateEdit_mouvementsDeStocks_debut->reset();

        dateEdit_mouvementsDeStocks_fin->reset();
    }
}


void YerithMouvementsDeStocksWindow::refineYerithLineEdits()
{
    _mouvements_de_stocks_DateFilter.clear();

    _mouvements_de_stocks_DateFilter.append(QString
                                            (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                            arg
                                            (YerithDatabaseTableColumn::DATE_SORTIE,
                                             DATE_TO_DB_FORMAT_STRING
                                             (dateEdit_mouvementsDeStocks_debut->
                                              date()),
                                             YerithDatabaseTableColumn::DATE_SORTIE,
                                             DATE_TO_DB_FORMAT_STRING
                                             (dateEdit_mouvementsDeStocks_fin->
                                              date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_stocks_sorties_OU_transferes();
}


void YerithMouvementsDeStocksWindow::reinitialiser_elements_filtrage()
{
    lineEdit_mouvementsDeStocks_resultat->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithMouvementsDeStocksWindow::reinitialiser_recherche()
{
    lineEdit_mouvementsDeStocks_resultat->clear();

    lineEdit_nom_element_string_db->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_stocks_sorties());

    resetLineEditsQCompleters((QObject *) this);

#ifdef YERITH_FRANCAIS_LANGUAGE

    comboBox_type_de_mouvement_de_stocks->find_AND_SET_CURRENT_INDEX
    (SORTIE_DE_STOCKS_TEXT_STRING_FRENCH);

#endif


#ifdef YERITH_ENGLISH_LANGUAGE

    comboBox_type_de_mouvement_de_stocks->find_AND_SET_CURRENT_INDEX
    (SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH);

#endif

    afficher_stocks_sorties_OU_transferes();

    lineEdit_mouvementsDeStocks_terme_recherche->setFocus();
}
