/*
 * yerith-erp-GROUPES-DE-PAIE-hr-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-GROUPES-DE-PAIE-hr-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/dialogs/yerith-erp-generic-select-db-field-dialog.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/QThread>

#include <QtCore/QProcess>

#include <QtCore/qmath.h>

#include <QtWidgets/QCheckBox>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <unistd.h>


/**
 * yerith-erp-windows.hpp cannot be included in
 * the header file because it will caused a circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */


YerithERPGROUPES_DE_PAIE_Window::YerithERPGROUPES_DE_PAIE_Window()
:YerithWindowsCommons("yerith-erp-liste_GROUPES_DE_PAIE_hr"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::GROUPES_DE_PAIE_hr),
 _logger(new YerithLogger("YerithERPGROUPES_DE_PAIE_Window")),
 _pushButton_filtrer_font(0),
 _cur_PAY_GROUP_TableModel(0)
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("GROUPES DE PAIE (HR)"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_groupes_de_paie);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
            << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionAfficher_ce_GROUPEDEPAIE_au_detail
        << actionSupprimerUnGROUPEDEPAIE;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_groupes_de_paie);


    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                            		   "QMessageBox QLabel {color: rgb(%2);}")
                            	.arg(COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                            		 COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(YerithDatabase::GROUPES_DE_PAIE_hr);


    _lineEditsToANDContentForSearch.insert(&lineEdit_GROUPESDEPAIE_recherche,
    									   YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString
		.insert(YerithDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr);


    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerithDatabase::GROUPES_DE_PAIE_hr);


    reinitialiser_colones_db_visibles();

    _cur_PAY_GROUP_TableModel = &_allWindows->getSqlTableModel_groupes_de_paie_hr();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_groupes_de_paie
		->setSqlTableName(&YerithDatabase::GROUPES_DE_PAIE_hr);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_GROUPEDEPAIE_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnGROUPEDEPAIE, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_add_condition_SEARCH->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_afficher->disable(this);
    pushButton_groupes_DEMPLOYES->disable(this);
    pushButton_creer_un_GROUPEDEPAIE->disable(this);
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


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenuFOURNISSEURS, SIGNAL(triggered()), this, SLOT(menu()));

    connect(actionAfficher_ce_GROUPEDEPAIE_au_detail,
            SIGNAL(triggered()),
			this,
			SLOT(afficher_au_detail()));

    connect(actionSupprimerUnGROUPEDEPAIE,
    		SIGNAL(triggered()),
            this,
			SLOT(supprimer_un_GROUPE_DE_PAIE_hr()));

    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionReinitialiserChampsDBVisible,
    		SIGNAL(triggered()),
			this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible,
    		SIGNAL(triggered()),
			this,
            SLOT(selectionner_champs_db_visibles()));

    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));

    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this, SLOT(reinitialiser_recherche()));

    connect(actionReinitialiserElementsDeFiltrage,
    		SIGNAL(triggered()),
			this,
			SLOT(reinitialiser_elements_filtrage()));

    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
#endif

    connect(actionAfficher_ce_GROUPEDEPAIE_au_detail,
            SIGNAL(triggered()),
			this,
			SLOT(afficher_au_detail()));

    connect(tableView_groupes_de_paie,
            SIGNAL(doubleClicked(const QModelIndex &)),
			this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}


YerithERPGROUPES_DE_PAIE_Window::~YerithERPGROUPES_DE_PAIE_Window()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

	delete _logger;
}


void YerithERPGROUPES_DE_PAIE_Window::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();

    resetTableViewHorizontalHeader_DEFAULT_ORDERING();

    afficher_GROUPES_DE_PAIE_hr(*_cur_PAY_GROUP_TableModel);
}


void YerithERPGROUPES_DE_PAIE_Window::textChangedSearchLineEditsQCompleters()
{
    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_GROUPESDEPAIE_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append
				(QString("(%1 OR %2 OR %3)")
					.arg(GENERATE_SQL_LIKE_STMT(YerithDatabaseTableColumn::DESCRIPTION_GROUPE_DE_PAIE_HR,
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

            if (!correspondingDBFieldKeyValue.isEmpty() 	 &&
                !aTableColumnFieldContentForANDSearch.isEmpty())
            {
                if (!_searchFilter.isEmpty())
                {
                    _searchFilter.append(" AND ");
                }

                _searchFilter
					.append(GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
                                      	  	  	 aTableColumnFieldContentForANDSearch));
            }
        }
    }

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER


    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(_searchFilter);


    if (_yerithSqlTableModel->select())
    {
        afficher_GROUPES_DE_PAIE_hr(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() << QString("++ YerithERPGROUPES_DE_PAIE_Window::textChangedSearchLineEditsQCompleters(): %1")
        				.arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithERPGROUPES_DE_PAIE_Window::populateComboBoxes()
{
    _logger->log("populateComboBoxes");


    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());


    YerithUtils::REMOVE_QSTRING_LIST(aQStringList,
                                     _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME);


    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_CREATION);

    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_GROUPE_DE_PAIE_HR);

    aQStringList.removeAll(YerithDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL);

    aQStringList.removeAll(YerithDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES);


    comboBox_element_string_db->clear();


    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::_tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append
		(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
				(YerithDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL));

    aQStringList.append
		(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
				(YerithDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES));


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


void YerithERPGROUPES_DE_PAIE_Window::setupLineEdits()
{
    lineEdit_GROUPESDEPAIE_recherche->setFocus();

    _QLINEEDIT_nombre_de_lignes_par_page = lineEdit_nombre_de_lignes_par_page;

    lineEdit_GROUPESDEPAIE_recherche
		->enableForSearch(QObject::tr("terme à rechercher (description du programme fidélité de clients)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr("valeur à rechercher"));

    lineEdit_nombre_de_GROUPESDEPAIE->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_nombre_de_lignes_par_page,
    									tableView_groupes_de_paie);
}


void YerithERPGROUPES_DE_PAIE_Window::setupDateTimeEdits()
{
    dateEdit_debut->setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_fin->setStartDate(GET_CURRENT_DATE);

    _EMPLOYEE_pay_group_DateCreationFilter.clear();

    _EMPLOYEE_pay_group_DateCreationFilter
		.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
                  .arg(YerithDatabaseTableColumn::DATE_CREATION,
                       DATE_TO_DB_FORMAT_STRING(dateEdit_debut->date()),
                       YerithDatabaseTableColumn::DATE_CREATION,
                       DATE_TO_DB_FORMAT_STRING(dateEdit_fin->date())));

    connect(dateEdit_debut,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerithLineEdits()));

    connect(dateEdit_fin,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerithLineEdits()));
}


void YerithERPGROUPES_DE_PAIE_Window::initialiser_FILTRES()
{
    comboBox_element->setVisible(false);
    comboBox_condition->setVisible(false);
    lineEdit_resultat_filtre->setVisible(false);
    pushButton_reinitialiser_filtre->setVisible(false);
    pushButton_filtrer->setVisible(false);
}


void YerithERPGROUPES_DE_PAIE_Window::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element->setVisible(IS_VISIBLE);
    comboBox_condition->setVisible(IS_VISIBLE);
    lineEdit_resultat_filtre->setVisible(IS_VISIBLE);
    pushButton_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_filtrer->setVisible(IS_VISIBLE);
}


void YerithERPGROUPES_DE_PAIE_Window::
		supprimer_PLUSIEURS_GROUPES_DE_PAIE_hr(YerithSqlTableModel &EMPLOYEE_pay_group_TableModel)
{
    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    QString cur_client_group_designation;

    QString SELECT_CURRENT_CLIENT_GROUP_DESIGNATION;

    QString YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING;

    QString UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING;


    int cur_query_size = -1;

    QSqlQuery a_sql_query;


    QMapIterator<QString, QString> j(tableView_groupes_de_paie->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();

        YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING
			.append(QString("DELETE FROM %1 WHERE %2 = '%3'; ")
                        .arg(YerithDatabase::GROUPES_DE_PAIE_hr,
                             YerithDatabaseTableColumn::ID,
                             j.value()));


        SELECT_CURRENT_CLIENT_GROUP_DESIGNATION
            = QString("select %1 from %2 where %3='%4'")
				.arg(YerithDatabaseTableColumn::DESIGNATION,
                     YerithDatabase::GROUPES_DE_PAIE_hr,
                     YerithDatabaseTableColumn::ID,
					 j.value());


        cur_query_size = YerithUtils::execQuery(a_sql_query,
                                                SELECT_CURRENT_CLIENT_GROUP_DESIGNATION);


        if (cur_query_size > 0 && a_sql_query.next())
        {
            cur_client_group_designation =
                            a_sql_query.value(YerithDatabaseTableColumn::DESIGNATION).toString();

            UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING
				.append(QString("UPDATE %1 SET %2='' WHERE %3='%4'; ")
                           .arg(YerithDatabase::GROUPES_DEMPLOYES_hr,
                                YerithDatabase::GROUPES_DE_PAIE_hr,
                                YerithDatabase::GROUPES_DE_PAIE_hr,
                                cur_client_group_designation));
        }
    }

    QString msgConfirmation = QObject::tr("Supprimer les GROUPES DE PAIE sélectionés ?");

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success =
        		YerithUtils::execQuery(YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING);

        QString msg = QObject::tr("Les GROUPES DE PAIE sélectionés");

        if (success && EMPLOYEE_pay_group_TableModel.select())
        {
//              QDEBUG_STRING_OUTPUT_2("UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING",
//                                                         UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING);

            bool success_update_client_group =
            		YerithUtils::execQuery(UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING);

            setupLineEditsQCompleters((QObject *) this);

            msg.append(QObject::tr(" ont été supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr("suppression - succès"),
										   msg,
                                           QMessageBox::Ok);

            if (!success_update_client_group)
            {
                YerithQMessageBox::warning(this,
                                           QObject::tr("actualisation - échec"),
                                           QObject::tr("les données des groupes d'employés "
                                        		   	   "N'ONT pas pu être actualisées avec succès !"));

                YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

                return;
            }
        }
        else
        {
            msg.append(QObject::tr(" n'ont pas pu être supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr("suppression - échec"),
										   msg,
                                           QMessageBox::Ok);
        }
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


void YerithERPGROUPES_DE_PAIE_Window::supprimer_un_GROUPE_DE_PAIE_hr()
{
    YerithSqlTableModel *GROUPES_DE_PAIE_TableModel = 0;

    if (_cur_PAY_GROUP_TableModel &&
        YerithUtils::isEqualCaseInsensitive(YerithDatabase::GROUPES_DE_PAIE_hr,
                                            _cur_PAY_GROUP_TableModel->sqlTableName()))
    {
        GROUPES_DE_PAIE_TableModel = _cur_PAY_GROUP_TableModel;
    }
    else
    {
        return;
    }


    if (tableView_groupes_de_paie->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_GROUPES_DE_PAIE_hr(*GROUPES_DE_PAIE_TableModel);

        tableView_groupes_de_paie->clearSelection();

        afficher_GROUPES_DE_PAIE_hr();

        return;
    }


    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    QSqlRecord record;

    _allWindows->_GROUPES_DE_PAIE_Window->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    if (record.isEmpty() || record.isNull(YerithDatabaseTableColumn::DESIGNATION))
    {
        return;
    }

    QString designation(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DESIGNATION));

    QString msgConfirmation
		= QObject::tr("Supprimer le GROUPE DE PAIE (hr) '%1' ?")
			.arg(designation);

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success =
        		_allWindows->_GROUPES_DE_PAIE_Window->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW();


        QString msg = QObject::tr("Le GROUPE DE PAIE (hr) '%1").arg(designation);


        if (success && GROUPES_DE_PAIE_TableModel->select())
        {
            QString UPDATE_FROM_CLIENT_GROUP_QUERY_STRING =
            		QString("UPDATE %1 SET %2='' WHERE %3='%4'")
                        .arg(YerithDatabase::GROUPES_DEMPLOYES_hr,
                             YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                             YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                             designation);

            bool success_update_client_group =
            		YerithUtils::execQuery(UPDATE_FROM_CLIENT_GROUP_QUERY_STRING);


            setupLineEditsQCompleters((QObject *) this);


            tableView_groupes_de_paie->clearSelection();

            afficher_GROUPES_DE_PAIE_hr();

            msg.append(QObject::tr("' a été supprimée de la base de données !"));


            YerithQMessageBox::information(this,
                                           QObject::tr("suppression - succès"),
										   msg,
                                           QMessageBox::Ok);


            if (!success_update_client_group)
            {
                msg =
                                QObject::tr("les données du groupe d'EMPLOYÉS ('%1') "
                                            "N'ONT pas pu être actualisées avec succès !")
									.arg(designation);

                YerithQMessageBox::warning(this,
                                           QObject::tr("actualisation - échec"),
										   msg);
            }
        }
        else
        {
            msg.append(QObject::tr(" n'a pas été supprimée de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr("suppression - échec"),
										   msg,
                                           QMessageBox::Ok);
        }
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


void YerithERPGROUPES_DE_PAIE_Window::
	afficher_GROUPES_DE_PAIE_hr(YerithSqlTableModel &EMPLOYEE_pay_group_TableModel)
{
    tableView_groupes_de_paie
		->queryYerithTableViewCurrentPageContentRow(EMPLOYEE_pay_group_TableModel);

    tableView_show_or_hide_columns(*tableView_groupes_de_paie);

    int rowCount = tableView_groupes_de_paie->rowCount();

    lineEdit_nombre_de_GROUPESDEPAIE->setText(GET_NUM_STRING(rowCount));
}


bool YerithERPGROUPES_DE_PAIE_Window::filtrer_groupes_de_paie_hr()
{
    return false;
}


void YerithERPGROUPES_DE_PAIE_Window::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
		<< YerithDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr
		<< YerithDatabaseTableColumn::DATE_CREATION
        << YerithDatabaseTableColumn::DESIGNATION
        << YerithDatabaseTableColumn::LOCALISATION
        << YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS;
}


void YerithERPGROUPES_DE_PAIE_Window::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.setPalette(toolBar_GROUPES_DE_PAIE_Window->palette());
    menu.addAction(actionAfficher_ce_GROUPEDEPAIE_au_detail);
    menu.addAction(actionSupprimerUnGROUPEDEPAIE);
    menu.exec(event->globalPos());
}


void YerithERPGROUPES_DE_PAIE_Window::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithERPGROUPES_DE_PAIE_Window::resetFilter()
{
}


void YerithERPGROUPES_DE_PAIE_Window::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setYerithSqlTableModel(_cur_PAY_GROUP_TableModel);

    _curStocksTableModel = stocksTableModel;

    setVisible(true);

    afficher_GROUPES_DE_PAIE_hr(*_cur_PAY_GROUP_TableModel);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("GROUPES DE PAIE (HR)"),
    								 _cur_PAY_GROUP_TableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerithERPGROUPES_DE_PAIE_Window::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_GROUPEDEPAIE_au_detail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnGROUPEDEPAIE, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _cur_PAY_GROUP_TableModel)


    pushButton_add_condition_SEARCH->enable(this, SLOT(AJOUTER_CONDITION_DE_RECHERCHE()));

    pushButton_filtrer->enable(this, SLOT(filtrer_groupes_de_paie_hr()));

    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));

    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));

    pushButton_groupes_DEMPLOYES->enable(this, SLOT(GROUPES_demployes_hr()));

    pushButton_creer_un_GROUPEDEPAIE->enable(this, SLOT(CREER_UN_GROUPE_DE_SALARIAL_hr()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_filtrer->enable(this, SLOT(filtrer()));

    pushButton_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
}


void YerithERPGROUPES_DE_PAIE_Window::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_GROUPEDEPAIE_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnGROUPEDEPAIE, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_add_condition_SEARCH->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_afficher->disable(this);
    pushButton_groupes_DEMPLOYES->disable(this);
    pushButton_creer_un_GROUPEDEPAIE->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerithERPGROUPES_DE_PAIE_Window::afficher_au_detail()
{
    if (_cur_PAY_GROUP_TableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_DETAILS_dun_groupe_de_PAIE_hr
			->rendreVisible(_cur_PAY_GROUP_TableModel,
							_curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("détails d'un \"GROUPE DE PAIE (hr)\""),
                                   QObject::tr("Sélectionnez un \"GROUPE DE PAIE (hr)\" à afficher les détails !"));
    }
}


void YerithERPGROUPES_DE_PAIE_Window::refineYerithLineEdits()
{
    _EMPLOYEE_pay_group_DateCreationFilter.clear();

    _EMPLOYEE_pay_group_DateCreationFilter
		.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
					.arg(YerithDatabaseTableColumn::DATE_CREATION,
                         DATE_TO_DB_FORMAT_STRING(dateEdit_debut->date()),
                         YerithDatabaseTableColumn::DATE_CREATION,
                         DATE_TO_DB_FORMAT_STRING(dateEdit_fin->date())));


    setupLineEditsQCompleters((QObject *) this);


    afficher_GROUPES_DE_PAIE_hr(*_cur_PAY_GROUP_TableModel);
}


bool YerithERPGROUPES_DE_PAIE_Window::filtrer()
{
    QString Employee_group_TableColumnValue(lineEdit_resultat_filtre->text());

    if (Employee_group_TableColumnValue.isEmpty())
    {
        QString msg = QObject::tr("Veuillez saisir une valeur numérique pour la recherche.");

        YerithQMessageBox::information(this,
        							   QObject::tr("filtrer"),
									   msg);

        return false;
    }

    QString employee_group_TableColumnProperty(comboBox_element->currentText());

    QString mathOperator(comboBox_condition->currentText());

    QString filterString;


    QString REAL_DB_ID_NAME_EMPLOYEE_group_TableColumnProperty =
    			YerithDatabaseTableColumn::_tableColumnToUserViewString
					.key(employee_group_TableColumnProperty);


    filterString.append(QString("%2 %3 '%4'").arg
                        (REAL_DB_ID_NAME_EMPLOYEE_group_TableColumnProperty,
                         mathOperator, Employee_group_TableColumnValue));

    //qDebug() << QString("filterString: %1")
    //                              .arg(filterString);

    _cur_PAY_GROUP_TableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_cur_PAY_GROUP_TableModel
				->easySelect("src/windows/crm/yerith-erp-GROUPES_DE_PAIE-window.cpp", 936);


    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        afficher_GROUPES_DE_PAIE_hr(*_cur_PAY_GROUP_TableModel);

        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this,
        											resultRows,
                                                    "EMPLOYÉS - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "EMPLOYÉS - filtrer");
    }

    return false;
}


void YerithERPGROUPES_DE_PAIE_Window::reinitialiser_elements_filtrage()
{
    lineEdit_resultat_filtre->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER

	setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithERPGROUPES_DE_PAIE_Window::reinitialiser_recherche()
{
    lineEdit_nom_element_string_db->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *) this);

    afficher_GROUPES_DE_PAIE_hr();
}


void YerithERPGROUPES_DE_PAIE_Window::set_filtrer_font()
{
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


bool YerithERPGROUPES_DE_PAIE_Window::export_csv_file()
{
    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_groupes_de_paie,
                                               tableColumnsToIgnore,
                                               "yerith-erp-EMPLOYEE-pay-group-listing-csv-format",
                                               "EMPLOYEE PAY GROUP listing");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_groupes_de_paie,
                                               tableColumnsToIgnore,
                                               "yerith-erp-liste-des-groupes-de-paie-format-csv",
                                               "liste des groupes de paie des employés");
    }

    return success;
}


QString YerithERPGROUPES_DE_PAIE_Window::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _latex_template_print_pdf_content = YerithUtils::EN_template_employee_PAY_GROUP_LISTING_tex;
    }
    else
    {
        _latex_template_print_pdf_content = YerithUtils::FR_template_employee_PAY_GROUP_LISTING_tex;
    }

    return _latex_template_print_pdf_content;
}


