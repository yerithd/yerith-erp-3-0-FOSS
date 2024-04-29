/*
 * yerith-erp-fournisseurs-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-fournisseurs-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerithERPFournisseursWindow::YerithERPFournisseursWindow()
:YerithWindowsCommons("yerith-erp-fournisseurs"),
 _SHOW_EMPLOYEE(true),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::FOURNISSEURS),
 _logger(new YerithLogger("YerithERPFournisseursWindow")),
 _pushButton_filtrer_font(0),
 _curFournisseursTableModel(0)
{
    _windowName = QString("%1 - %2")
                    .arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("EMPLOYÉS / fournisseurs"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_fournisseurs);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
        << pushButton_modifier
        << pushButton_groupes_de_paies_de_CET_EMPLOYE;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionModifierFournisseur
        << actionSupprimerFournisseur
		<< actionAfficherDetailsFournisseur;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_fournisseurs);


    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                                       "QMessageBox QLabel {color: rgb(%2);}")
                                    .arg(COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                                         COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
        << YerithDatabaseTableColumn::EMPLOYE
        << YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE
        << YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID;


    setup_select_configure_dbcolumn(YerithDatabase::FOURNISSEURS);


    _lineEditsToANDContentForSearch
        .insert(&lineEdit_fournisseurs_terme_recherche,
                YerithUtils::EMPTY_STRING);


    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerithDatabase::FOURNISSEURS);


    reinitialiser_colones_db_visibles();


    _curFournisseursTableModel = &_allWindows->getSqlTableModel_fournisseurs();


    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    populateFournisseursComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_fournisseurs->setSqlTableName(&YerithDatabase::FOURNISSEURS);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_DE_LEMPLOYE,
                                           false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS;


    pushButton_add_condition_SEARCH->disable(this);
    pushButton_GROUPE_DEMPLOYES->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_groupes_de_paies_de_CET_EMPLOYE->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));


    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    //Menu actions
    connect(actionReinitialiserChampsDBVisible,
            SIGNAL(triggered()),
            this,
            SLOT(slot_reinitialiser_colones_db_visibles()));


    connect(action_afficher_groupes_DE_LEMPLOYE,
    		SIGNAL(triggered()),
			this,
            SLOT(afficher_groupes_dun_employe()));


    connect(actionChampsDBVisible,
            SIGNAL(triggered()),
            this,
            SLOT(selectionner_champs_db_visibles()));


    connect(action_AFFICHER_DES_fournisseurs_OU_des_EMPLOYES,
            SIGNAL(triggered()),
            this,
			SLOT(handle_ACTION_CHOICE_FOURNISSEURS_EMPLOYES()));


    connect(radioButton_lister_des_employes,
            SIGNAL(toggled(bool)),
            this,
			SLOT(handle_RADIO_BUTTONS_FOURNISSEURS_employes(bool)));


    connect(radioButton_lister_des_FOURNISSEURS,
            SIGNAL(toggled(bool)),
            this,
			SLOT(handle_RADIO_BUTTONS_FOURNISSEURS_employes(bool)));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this,
			SLOT(handle_filtres_checkBox(int)));


    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionCreerFournisseur, SIGNAL(triggered()), this,
            SLOT(creerFournisseur()));
    connect(actionModifierFournisseur, SIGNAL(triggered()), this,
            SLOT(modifierFournisseur()));
    connect(actionSupprimerFournisseur, SIGNAL(triggered()), this,
            SLOT(supprimerFournisseur()));
    connect(actionAfficherDetailsFournisseur, SIGNAL(triggered()), this,
            SLOT(afficher_au_detail()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));

    connect(actionAfficherPDF,
    		SIGNAL(triggered()),
			this,
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

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif


    connect(tableView_fournisseurs,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}

YerithERPFournisseursWindow::~YerithERPFournisseursWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR
    delete _pushButton_filtrer_font;
    delete _logger;
}


void YerithERPFournisseursWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::LOCALISATION
            << YerithDatabaseTableColumn::REFERENCE_FOURNISSEUR
            << YerithDatabaseTableColumn::NOM_ENTREPRISE
            << YerithDatabaseTableColumn::VILLE
            << YerithDatabaseTableColumn::EMAIL
            << YerithDatabaseTableColumn::NUMERO_TELEPHONE_1;
}


void YerithERPFournisseursWindow::contextMenuEvent(QContextMenuEvent *event)
{
	if (false == _SHOW_EMPLOYEE)
	{
	    QMenu menu(this);
	    menu.setPalette(toolBar_fournisseursWindow->palette());
	    menu.addAction(actionAfficherDetailsFournisseur);
	    menu.addAction(actionModifierFournisseur);
	    menu.addAction(actionSupprimerFournisseur);
	    menu.exec(event->globalPos());
	}
	else
	{
	    QMenu menu(this);
	    menu.setPalette(toolBar_fournisseursWindow->palette());
	    menu.addAction(actionAfficherDetailsFournisseur);
	    menu.addAction(actionModifierFournisseur);
	    menu.addAction(action_afficher_groupes_DE_LEMPLOYE);
	    menu.addAction(actionSupprimerFournisseur);
	    menu.exec(event->globalPos());
	}
}


void YerithERPFournisseursWindow::hide_SHOW_EMPLOYEEEvent(QHideEvent *hideEvent)
{
    YerithWindowsCommons::
    CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();

    _allWindows->_transactionsDunFournisseurWindow->close();
}


void YerithERPFournisseursWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);

    actionReinitialiserRecherche->
    	setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithERPFournisseursWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
        _pushButton_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_filtrer_font->setUnderline(false);
    }

    pushButton_filtrer->setFont(*_pushButton_filtrer_font);
}


void YerithERPFournisseursWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    lister_les_elements_du_tableau();
}


void YerithERPFournisseursWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_fournisseurs_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("(%1 OR %2 OR %3)").arg
                                 (GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::EMAIL,
                                   partSearchTerm),
                                  GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::NOM_REPRESENTANT,
                                   partSearchTerm),
                                  GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::
                                   DESCRIPTION_FOURNISSEUR, partSearchTerm)));

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


	if (_SHOW_EMPLOYEE)
    {
        if (_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" %1 != 0").arg
                                 (YerithDatabaseTableColumn::EMPLOYE));
        }
        else
        {
            _searchFilter.append(QString(" AND %1 != 0").arg
                                 (YerithDatabaseTableColumn::EMPLOYE));
        }
    }
    else
    {
        if (_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" %1 = 0").arg
                                 (YerithDatabaseTableColumn::EMPLOYE));
        }
        else
        {
            _searchFilter.append(QString(" AND %1 = 0").arg
                                 (YerithDatabaseTableColumn::EMPLOYE));
        }
    }

    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(_searchFilter);

    if (_yerithSqlTableModel->select())
    {
        lister_les_elements_du_tableau(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString("++ YerithERPFournisseursWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithERPFournisseursWindow::initialiser_FILTRES()
{
    comboBox_element->setVisible(false);
    comboBox_condition->setVisible(false);
    lineEdit_resultat_filtre->setVisible(false);
    pushButton_reinitialiser_filtre->setVisible(false);
    pushButton_filtrer->setVisible(false);
}


void YerithERPFournisseursWindow::afficher_groupes_dun_employe()
{
    if (_curFournisseursTableModel->rowCount() > 0)
    {
        rendreInvisible();

        _allWindows->_groupes_DUN_EMPLOYE_window
			->rendreVisible(_curFournisseursTableModel, _curStocksTableModel);
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("groupes d'EMPLOYÉS d'appartenance"),
                                       QObject::tr
                                       ("Sélectionnez 1 employé afin de visualiser ses GROUPES d'EMPLOYÉS d'appartenance."));
    }
}


void YerithERPFournisseursWindow::handle_ACTION_CHOICE_FOURNISSEURS_EMPLOYES()
{
    if (radioButton_lister_des_employes->isChecked())
    {
    	_SHOW_EMPLOYEE = true;

    	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_DE_LEMPLOYE, true);

    	radioButton_lister_des_FOURNISSEURS->toggle();
    }
    else if (radioButton_lister_des_FOURNISSEURS->isChecked())
    {
    	_SHOW_EMPLOYEE = false;

    	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_DE_LEMPLOYE, false);

    	radioButton_lister_des_employes->toggle();
    }


    pushButton_groupes_de_paies_de_CET_EMPLOYE->setVisible(_SHOW_EMPLOYEE);
}


void YerithERPFournisseursWindow::handle_RADIO_BUTTONS_FOURNISSEURS_employes(bool is_toggle)
{
    if (radioButton_lister_des_FOURNISSEURS->isChecked())
    {
        _windowName = QString("%1 - %2")
                        .arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                             QObject::tr("fournisseurs"));

    	action_AFFICHER_DES_fournisseurs_OU_des_EMPLOYES->setText(QObject::tr("AFFICHER DES EMPLOYÉS"));

        _SHOW_EMPLOYEE = false;

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_DE_LEMPLOYE, false);
    }
    else if (radioButton_lister_des_employes->isChecked())
    {
        _windowName = QString("%1 - %2")
                        .arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                             QObject::tr("EMPLOYÉS"));

    	action_AFFICHER_DES_fournisseurs_OU_des_EMPLOYES->setText(QObject::tr("AFFICHER DES fournisseurs"));

        _SHOW_EMPLOYEE = true;

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_DE_LEMPLOYE, true);
    }


    setWindowTitle(_windowName);


    textChangedSearchLineEditsQCompleters();


    pushButton_groupes_de_paies_de_CET_EMPLOYE->setVisible(_SHOW_EMPLOYEE);
}


void YerithERPFournisseursWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element->setVisible(IS_VISIBLE);
    comboBox_condition->setVisible(IS_VISIBLE);
    lineEdit_resultat_filtre->setVisible(IS_VISIBLE);
    pushButton_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_filtrer->setVisible(IS_VISIBLE);
}


void YerithERPFournisseursWindow::modifierFournisseur()
{
    if (_curFournisseursTableModel->rowCount() > 0)
    {
        rendreInvisible();

        if (!_SHOW_EMPLOYEE)
        {
            _allWindows->_modifierFournisseurWindow
				->rendreVisible(_curFournisseursTableModel,
								_curStocksTableModel,
								false,
								false);
        }
        else
        {
            _allWindows->_modifierFournisseurWindow
				->rendreVisible(_curFournisseursTableModel,
								_curStocksTableModel,
								false,
								true);
        }

    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("modifier un fournisseur"),
                                   QObject::tr
                                   ("Sélectionnez un fournisseur à modifier !"));
    }
}


void
YerithERPFournisseursWindow::supprimer_PLUSIEURS_Fournisseurs
(YerithSqlTableModel &aFournisseursTableModel)
{
    QString
    YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING;

    QMapIterator < QString,
                 QString > j(tableView_fournisseurs->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();

        YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING.append(QString
                                                                   ("DELETE FROM %1 WHERE %2 = '%3';").
                                                                   arg
                                                                   (YerithDatabase::FOURNISSEURS,
                                                                    YerithDatabaseTableColumn::ID,
                                                                    j.value
                                                                    ()));
    }

    QString
    msgConfirmation(QObject::tr
                    ("Supprimer les fournisseurs sélectionés ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression de plusieurs fournisseurs"),
                                        msgConfirmation, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool
        success =
                        YerithUtils::execQuery
                        (YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING);

        QString
        msg(QObject::tr("Les fournisseurs sélectionés"));

        if (success && aFournisseursTableModel.select())
        {
            setupLineEditsQCompleters((QObject *) this);

            msg.append(QObject::tr
                       (" ont été supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression de fournisseurs - succès"),
                                           msg, QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'ont pas pu être supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression de fournisseurs - échec"),
                                           msg, QMessageBox::Ok);
        }
    }
}


void YerithERPFournisseursWindow::supprimerFournisseur()
{
    YerithSqlTableModel *fournisseursTableModel = 0;

    if (_curFournisseursTableModel
            && YerithUtils::isEqualCaseInsensitive(YerithDatabase::FOURNISSEURS,
                                                   _curFournisseursTableModel->
                                                   sqlTableName()))
    {
        fournisseursTableModel = _curFournisseursTableModel;
    }
    else
    {
        return;
    }

    if (tableView_fournisseurs->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_Fournisseurs(*fournisseursTableModel);

        tableView_fournisseurs->clearSelection();

        lister_les_elements_du_tableau();

        return;
    }

    QSqlRecord record;

    _allWindows->_fournisseursWindow->
    SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    if (record.isEmpty()
            || record.isNull(YerithDatabaseTableColumn::NOM_ENTREPRISE))
    {
        return;
    }

    QString
    nom_entreprise(GET_SQL_RECORD_DATA
                   (record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

    QString
    msgConfirmation(QObject::tr("Supprimer le fournisseur '%1' ?").arg
                    (nom_entreprise));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool success =
                        _allWindows->
                        _fournisseursWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
                        ();

        QString msg(QObject::tr("Le fournisseur '%1").arg(nom_entreprise));

        if (success && fournisseursTableModel->select())
        {
            setupLineEditsQCompleters((QObject *) this);

            tableView_fournisseurs->clearSelection();

            lister_les_elements_du_tableau();

            msg.append(QObject::tr
                       ("' a été supprimée de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"), msg,
                                           QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'a pas été supprimée de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - échec"), msg,
                                           QMessageBox::Ok);
        }
    }
}


void YerithERPFournisseursWindow::afficher_au_detail()
{
    if (_curFournisseursTableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();

        if (!_SHOW_EMPLOYEE)
        {
            _allWindows->_fournisseurDetailsWindow
    			->rendreVisible(_curFournisseursTableModel,
    							_curStocksTableModel);
        }
        else
		{
            _allWindows->_fournisseurDetailsWindow
    			->rendreVisible(_curFournisseursTableModel,
    							_curStocksTableModel,
								true);
		}

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::warning(this, QObject::tr("détails"),
                                   QObject::tr
                                   ("Sélectionnez un fournisseur à afficher les détails !"));
    }
}


bool YerithERPFournisseursWindow::filtrer()
{
    QString stockTableColumnValue(lineEdit_resultat_filtre->text());

    if (stockTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;
    }

    QString stockTableColumnProperty(comboBox_element->currentText());

    QString mathOperator(comboBox_condition->currentText());

    QString filterString;

    QString
    REAL_DB_ID_NAME_stockTableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.key
     (stockTableColumnProperty));

    filterString.append(QString("%2 %3 '%4'").arg
                        (REAL_DB_ID_NAME_stockTableColumnProperty,
                         mathOperator, stockTableColumnValue));

    //qDebug() << QString("filterString: %1")
    //                              .arg(filterString);

    _curFournisseursTableModel->yerithSetFilter_WITH_where_clause
    (filterString);

    int resultRows =
    		_curFournisseursTableModel->easySelect("src/windows/supplier/yerith-erp-fournisseurs-windows.cpp", 748);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        lister_les_elements_du_tableau(*_curFournisseursTableModel);

        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "filtrer");
    }

    return false;
}


void YerithERPFournisseursWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithERPFournisseursWindow::reinitialiser_recherche()
{
    lineEdit_nom_element_string_db->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *) this);

    lister_les_elements_du_tableau();
}


void YerithERPFournisseursWindow::populateFournisseursComboBoxes()
{
    _logger->log("populateFournisseursComboBoxes");

    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_FOURNISSEUR);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::_tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::COMPTE_FOURNISSEUR));


    comboBox_element->clear();

    comboBox_element->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");


    comboBox_condition->clear();

    comboBox_condition->addItems(aQStringList);
}


void YerithERPFournisseursWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_fournisseurs_nombre_de_lignes_par_page;

    lineEdit_fournisseurs_terme_recherche->enableForSearch(QObject::tr
                                                           ("terme à rechercher (émail, réprésentant, description de l'entreprise fournisseure)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_nombre_de_fournisseurs->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_fournisseurs_nombre_de_lignes_par_page, tableView_fournisseurs);

    lineEdit_resultat_filtre->setValidator(&YerithUtils::DoubleValidator);
}


void YerithERPFournisseursWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    _logger->log("rendreVisible");

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    populateFournisseursComboBoxes();

    setYerithSqlTableModel(_curFournisseursTableModel);

    _curStocksTableModel = stocksTableModel;

    if (!_SHOW_EMPLOYEE)
    {
    	radioButton_lister_des_FOURNISSEURS->toggle();
    	radioButton_lister_des_employes->toggle();
    	radioButton_lister_des_FOURNISSEURS->toggle();
    }
    else
    {
    	radioButton_lister_des_employes->toggle();
    	radioButton_lister_des_FOURNISSEURS->toggle();
    	radioButton_lister_des_employes->toggle();
    }


    setVisible(true);


    lister_les_elements_du_tableau(*_curFournisseursTableModel);


    if (radioButton_lister_des_FOURNISSEURS->isChecked())
    {
        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("fournisseurs"),
                                         _curFournisseursTableModel);
    }
    else if (radioButton_lister_des_employes->isChecked())
    {
    	YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("EMPLOYÉS"),
                                         _curFournisseursTableModel);
    }


    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();


    int row = tableView_fournisseurs
                     ->_map_dbID_TO_yerith_table_view_ROW_NUMBER
                            .value(get_last_lister_selected_row_db_ID());

    //tableView_fournisseurs->selectRow(row);

    pushButton_groupes_de_paies_de_CET_EMPLOYE->setVisible(_SHOW_EMPLOYEE);


    lineEdit_fournisseurs_terme_recherche->setFocus();
}


void YerithERPFournisseursWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_DE_LEMPLOYE, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curFournisseursTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT(AJOUTER_CONDITION_DE_RECHERCHE()));

    pushButton_GROUPE_DEMPLOYES->enable(this, SLOT(GROUPES_demployes_hr()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_fournisseur->enable(this, SLOT(creerFournisseur()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));

    pushButton_groupes_de_paies_de_CET_EMPLOYE
        ->enable(this, SLOT(afficher_groupes_dun_employe()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));

    pushButton_reinitialiser_filtre
        ->enable(this,
                 SLOT(reinitialiser_elements_filtrage()));
}


void YerithERPFournisseursWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_DE_LEMPLOYE,
                                           false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsFournisseur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);

    pushButton_GROUPE_DEMPLOYES->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_fournisseur->disable(this);
    pushButton_modifier->disable(this);
    pushButton_groupes_de_paies_de_CET_EMPLOYE->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerithERPFournisseursWindow::lister_les_elements_du_tableau()
{
    textChangedSearchLineEditsQCompleters();
}


void YerithERPFournisseursWindow::afficher_nom_entreprise_selectioner(const QString &nomEntreprise)
{
    _logger->log("afficher_nom_entreprise_selectioner(const QString &)");

    QString filter(GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::NOM_ENTREPRISE, nomEntreprise));

    //qDebug() << QString("filter: %1").arg(filter);

    _curFournisseursTableModel->yerithSetFilter_WITH_where_clause(filter);

    if (_curFournisseursTableModel->easySelect("src/windows/supplier/yerith-erp-fournisseurs-windows.cpp", 1064) > 0)
    {
        lister_les_elements_du_tableau(*_curFournisseursTableModel);
    }
}


void YerithERPFournisseursWindow::
        lister_les_elements_du_tableau(YerithSqlTableModel &fournisseurSqlTableModel)
{
	tableView_fournisseurs->queryYerithTableViewCurrentPageContentRow(fournisseurSqlTableModel);

    tableView_show_or_hide_columns(*tableView_fournisseurs);

    //tableView_fournisseurs
    //    ->selectRow(tableView_fournisseurs->currentIndex().row());

    int rowCount = tableView_fournisseurs->rowCount();

    lineEdit_nombre_de_fournisseurs->setText(GET_NUM_STRING(rowCount));
}


bool YerithERPFournisseursWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_fournisseurs,
                                               tableColumnsToIgnore,
                                               "yerith-erp-supplier-account-listing-csv-format",
                                               "supplier account listing file");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_fournisseurs,
                                               tableColumnsToIgnore,
                                               "yerith-erp-fiche-fournisseurs-format-csv",
                                               "fiche des fournisseurs");
    }

    return success;
}


QString YerithERPFournisseursWindow::get_latex_template_print_pdf_content()
{
    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	if (_SHOW_EMPLOYEE)
    	{
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHOBJECT", "Employee");

            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHSUBJECT", "EMPLOYEE Listing");
    	}
    	else
    	{
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHOBJECT", "Supplier");

            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHSUBJECT", "Supplier Listing");
    	}

        _latex_template_print_pdf_content = YerithUtils::EN_template_fournisseurs_tex;
    }
    else
    {
    	if (_SHOW_EMPLOYEE)
    	{
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHOBJECT", "d'EMPLOYÉS");

            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHSUBJECT", "$1$ LISTE D'EMPLOYÉS");
    	}
    	else
    	{
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHOBJECT", "de fournisseurs");

            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
    			.insert("YERITHSUBJECT", "$1$ LISTE DE FOURNISSEURS");
    	}

        _latex_template_print_pdf_content = YerithUtils::FR_template_fournisseurs_tex;
    }

    return _latex_template_print_pdf_content;
}
