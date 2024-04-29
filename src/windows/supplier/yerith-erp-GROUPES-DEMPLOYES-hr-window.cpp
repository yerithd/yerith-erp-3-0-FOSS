/*
 * yerith-erp-GROUPES-DEMPLOYES-hr-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-GROUPES-DEMPLOYES-hr-window.hpp"

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


YerithERPGroupesDemployesHRWindow::YerithERPGroupesDemployesHRWindow()
:YerithWindowsCommons("yerith-erp-liste-groupes-DEMPLOYES"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::GROUPES_DEMPLOYES_hr),
 _logger(new YerithLogger("YerithERPGroupesDemployesHRWindow")),
 _pushButton_filtrer_font(0),
 _curEMPLOYEEGroupTableModel(0)
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("groupes d'employés [hr]"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_GROUPESDEMPLOYES);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
            << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionAfficher_les_membres_de_ce_groupe
        << actionSupprimerGroupeDEMPLOYES
		<< actionAfficher_ce_groupe_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_GROUPESDEMPLOYES);


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1); color: white;}"
                            "QMessageBox QLabel {color: rgb(%2);}")
                        .arg(COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                             COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(YerithDatabase::GROUPES_DEMPLOYES_hr);


    _lineEditsToANDContentForSearch.insert(&lineEdit_terme_recherche,
    									   YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString.insert(YerithDatabaseTableColumn::REFERENCE_GROUPE);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION
    (YerithDatabase::GROUPES_DEMPLOYES_hr);

    reinitialiser_colones_db_visibles();

    _curEMPLOYEEGroupTableModel =
                    &_allWindows->getSqlTableModel_groupes_demployes_hr();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_GROUPESDEMPLOYES->setSqlTableName(&YerithDatabase::GROUPES_DEMPLOYES_hr);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_les_membres_de_ce_groupe, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupeDEMPLOYES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_add_condition_SEARCH->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_creer_groupe->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_GROUPES_DE_PAIE->disable(this);
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
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficher_les_membres_de_ce_groupe, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionAfficher_ce_groupe_au_detail, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionReinitialiserChampsDBVisible,
    		SIGNAL(triggered()),
			this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible,
    		SIGNAL(triggered()),
			this,
            SLOT(selectionner_champs_db_visibles()));

    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this,
			SLOT(handle_filtres_checkBox(int)));

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
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
#endif

    connect(actionSupprimerGroupeDEMPLOYES, SIGNAL(triggered()),
            this, SLOT(supprimer_GROUPE_DEMPLOYES()));

    connect(tableView_GROUPESDEMPLOYES,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}


YerithERPGroupesDemployesHRWindow::~YerithERPGroupesDemployesHRWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithERPGroupesDemployesHRWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    afficher_GROUPESDEMPLOYES(*_curEMPLOYEEGroupTableModel);
}


void YerithERPGroupesDemployesHRWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("(%1)").arg(GENERATE_SQL_LIKE_STMT
                                                     (YerithDatabaseTableColumn::
                                                      DESCRIPTION_GROUPE,
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
    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(_searchFilter);

    if (_yerithSqlTableModel->select())
    {
        afficher_GROUPESDEMPLOYES(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithERPGroupesDemployesHRWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithERPGroupesDemployesHRWindow::clear_all_fields()
{
}


void YerithERPGroupesDemployesHRWindow::populateComboBoxes()
{
    _logger->log("populateComboBoxes");

    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_CREATION);
    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_GROUPE_DEMPLOYES_HR);
    aQStringList.removeAll(YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES);
    aQStringList.removeAll(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);

    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES));


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


void YerithERPGroupesDemployesHRWindow::setupLineEdits()
{
    lineEdit_terme_recherche->setFocus();


    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_nombre_de_lignes_par_page;


    lineEdit_terme_recherche
		->enableForSearch(QObject::tr("terme à rechercher (DESCRIPTION DU GROUPE D'EMPLOYÉS)"));


    lineEdit_nom_element_string_db
		->enableForSearch(QObject::tr("valeur à rechercher"));


    lineEdit_nombre_de_groupes->setYerithEnabled(false);


    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_nombre_de_lignes_par_page,
    									tableView_GROUPESDEMPLOYES);
}


void YerithERPGroupesDemployesHRWindow::setupDateTimeEdits()
{
    dateEdit_debut
		->setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_fin
		->setStartDate(GET_CURRENT_DATE);

    _EmployeeGroupDateCreationFilter.clear();

    _EmployeeGroupDateCreationFilter
		.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
                   .arg(YerithDatabaseTableColumn::DATE_CREATION,
                        DATE_TO_DB_FORMAT_STRING(dateEdit_debut->date()),
                        YerithDatabaseTableColumn::DATE_CREATION,
                        DATE_TO_DB_FORMAT_STRING(dateEdit_fin->date())));


    connect(dateEdit_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));

    connect(dateEdit_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));
}


void YerithERPGroupesDemployesHRWindow::initialiser_FILTRES()
{
    comboBox_element->setVisible(false);
    comboBox_condition->setVisible(false);
    lineEdit_resultat_filtre->setVisible(false);
    pushButton_reinitialiser_filtre->setVisible(false);
    pushButton_filtrer->setVisible(false);
}


void YerithERPGroupesDemployesHRWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element->setVisible(IS_VISIBLE);
    comboBox_condition->setVisible(IS_VISIBLE);
    lineEdit_resultat_filtre->setVisible(IS_VISIBLE);
    pushButton_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_filtrer->setVisible(IS_VISIBLE);
}


void YerithERPGroupesDemployesHRWindow::
		supprimer_PLUSIEURS_GROUPESDEMPLOYES(YerithSqlTableModel &anEmployeeGroupTableModel)
{
    QString YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING;

    QMapIterator<QString, QString> j(tableView_GROUPESDEMPLOYES->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();

        YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING
			.append(QString("DELETE FROM %1 WHERE %2 = '%3';")
						.arg(YerithDatabase::GROUPES_DEMPLOYES_hr,
                             YerithDatabaseTableColumn::ID,
                             j.value()));
    }


    QString msgConfirmation = QObject::tr("SUPPRIMER LES GROUPE D'EMPLOYÉS SÉLECTIONÉS ?");


    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        bool success = YerithUtils::execQuery(YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING);


        QString msg = QObject::tr("LES GROUPE D'EMPLOYÉS sélectionés");


        if (success && anEmployeeGroupTableModel.select())
        {
            setupLineEditsQCompleters((QObject *) this);

            msg.append(QObject::tr(" ont été supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr("suppression - succès"),
										   msg,
                                           QMessageBox::Ok);
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
}


void YerithERPGroupesDemployesHRWindow::supprimer_GROUPE_DEMPLOYES()
{
    YerithSqlTableModel *groupeDemployesTableModel = 0;

    if (_curEMPLOYEEGroupTableModel &&
            YerithUtils::isEqualCaseInsensitive(YerithDatabase::GROUPES_DEMPLOYES_hr,
                                                _curEMPLOYEEGroupTableModel->sqlTableName()))
    {
        groupeDemployesTableModel = _curEMPLOYEEGroupTableModel;
    }
    else
    {
        return;
    }


    if (tableView_GROUPESDEMPLOYES->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_GROUPESDEMPLOYES(*groupeDemployesTableModel);

        tableView_GROUPESDEMPLOYES->clearSelection();

        afficher_GROUPESDEMPLOYES();

        return;
    }


    QSqlRecord record;


    _allWindows
		->_groupe_demployes_window
			->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);


    if (record.isEmpty() 								   ||
    	record.isNull(YerithDatabaseTableColumn::DESIGNATION))
    {
        return;
    }


    QString GROUP_db_ID =
    		GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID);


    QString designation =
    		GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DESIGNATION);


    QString msgConfirmation =
    		QObject::tr("Supprimer le GROUPE D'EMPLOYÉS '%1' ?")
				.arg(designation);


    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel,
										QMessageBox::Ok))
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        QString SELECT_ALL_EMPLOYEE_GROUP_MEMBER_ID =
                        QString("select %1 from %2").
                        arg(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                            YerithDatabase::GROUPES_DEMPLOYES_hr);

        QSqlQuery a_qsql_query;

        int query_size = YerithUtils::execQuery(a_qsql_query,
                                                SELECT_ALL_EMPLOYEE_GROUP_MEMBER_ID);

        if (query_size > 0 && a_qsql_query.next())
        {
            QString membres_du_groupe_db_ID = a_qsql_query.value(0).toString();

            QStringList membres_du_groupe_db_ID_LIST;

            YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
            											membres_du_groupe_db_ID);

            QString cur_employee_db_ID;

            QSqlQuery a_qsql_query_2;

            int query_size_2 = 0;

            for (uint j = 0; j < membres_du_groupe_db_ID_LIST.size(); ++j)
            {
                cur_employee_db_ID = membres_du_groupe_db_ID_LIST.at(j);

                QString SELECT_EMPLOYEE_GROUP_FROM_EMPLOYEE_DATA =
                                QString("SELECT %1, %2, %3 FROM %4 WHERE %5='%6' AND %7 = '1'")
								  .arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
									   YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
									   YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
									   YerithDatabase::FOURNISSEURS,
									   YerithDatabaseTableColumn::ID,
									   cur_employee_db_ID,
									   YerithDatabaseTableColumn::EMPLOYE);

                a_qsql_query_2.clear();


                query_size_2 =
                		YerithUtils::execQuery(a_qsql_query_2,
                                               SELECT_EMPLOYEE_GROUP_FROM_EMPLOYEE_DATA);


                if (query_size_2 > 0 && a_qsql_query_2.next())
                {
                    QString nom_entreprise_employe =
                    		a_qsql_query_2
								.value(YerithDatabaseTableColumn::NOM_ENTREPRISE).toString();

                    QString GROUPES_DUN_EMPLOYE =
                    		a_qsql_query_2
								.value(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE).toString();

                    QString groupes_dun_employe_ID =
                    		a_qsql_query_2
								.value(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID).toString();


//                                      QDEBUG_STRING_OUTPUT_2("nom_entreprise_CLIENT", nom_entreprise_CLIENT);

//                                      QDEBUG_STRING_OUTPUT_2("GROUP_db_ID", GROUP_db_ID);

                    YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(GROUPES_DUN_EMPLOYE,
                    															   designation);

                    YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(groupes_dun_employe_ID,
                    															   GROUP_db_ID);

//                                      QDEBUG_STRING_OUTPUT_2("groupes_du_client_ID", groupes_du_client_ID);


                    QString update_employee_group_data =
                    		QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7' AND %8 = '1'")
								.arg(YerithDatabase::FOURNISSEURS,
                                     YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                                     GROUPES_DUN_EMPLOYE,
                                     YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                                     groupes_dun_employe_ID,
                                     YerithDatabaseTableColumn::ID,
                                     cur_employee_db_ID,
									 YerithDatabaseTableColumn::EMPLOYE);

//                                      QDEBUG_STRING_OUTPUT_2("UPDATE_CLIENT_GROUP_DATA", UPDATE_CLIENT_GROUP_DATA);


                    bool SUCCESS_UPDATED_EMPLOYEE_GROUP_DATA =
                    		YerithUtils::execQuery(update_employee_group_data);


                    if (!SUCCESS_UPDATED_EMPLOYEE_GROUP_DATA)
                    {
                        YerithQMessageBox::information(this,
                                                       QObject::tr("GROUPE DE L'EMPLOYÉ - suppression - échec"),
                                                       QObject::tr("LES DONNÉES DE GROUPE DE L'EMPLOYÉ '%1'"
                                                    		   	   " NE POUVAIENT PAS ÊTRE ACTUALISÉES !")
														  .arg(nom_entreprise_employe),
													   QMessageBox::Ok);
                    }
                }
            }
        }


        bool success = _allWindows
        					->_groupe_demployes_window
								->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW();


        QString msg = QObject::tr("LE GROUPE D'EMPLOYÉS '%1")
        				.arg(designation);


        if (success && groupeDemployesTableModel->select())
        {
            setupLineEditsQCompleters((QObject *) this);

            tableView_GROUPESDEMPLOYES->clearSelection();

            afficher_GROUPESDEMPLOYES();

            msg.append(QObject::tr("' a été supprimé de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr("suppression - succès"),
										   msg,
                                           QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::tr(" n'a pas été supprimé de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr("suppression - échec"),
										   msg,
                                           QMessageBox::Ok);
        }


        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


void YerithERPGroupesDemployesHRWindow::
		afficher_GROUPESDEMPLOYES(YerithSqlTableModel& anEMPLOYEE_GroupTableModel)
{
    tableView_GROUPESDEMPLOYES
		->queryYerithTableViewCurrentPageContentRow(anEMPLOYEE_GroupTableModel);

    tableView_show_or_hide_columns(*tableView_GROUPESDEMPLOYES);

    int rowCount = tableView_GROUPESDEMPLOYES->rowCount();

    lineEdit_nombre_de_groupes->setText(GET_NUM_STRING(rowCount));

    if (rowCount > 0)
    {
        if (get_INT_last_selected_row_number() > -1)
        {
            tableView_GROUPESDEMPLOYES->selectRow(get_INT_last_selected_row_number());
        }
        else
        {
            tableView_GROUPESDEMPLOYES->selectRow(0);
        }
    }
}


bool YerithERPGroupesDemployesHRWindow::filtrer_GROUPESDEMPLOYES()
{
    return false;
}


void YerithERPGroupesDemployesHRWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DATE_CREATION
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::REFERENCE_GROUPE
            << YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS
            << YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES;
}


void YerithERPGroupesDemployesHRWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_GROUPESDEMPLOYESWindow->palette());

    menu.addAction(actionAfficher_ce_groupe_au_detail);
    menu.addAction(actionAfficher_les_membres_de_ce_groupe);
    menu.addAction(actionSupprimerGroupeDEMPLOYES);

    menu.exec(event->globalPos());
}


void YerithERPGroupesDemployesHRWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionReinitialiserRecherche->
    setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithERPGroupesDemployesHRWindow::resetFilter()
{
}


void YerithERPGroupesDemployesHRWindow::rendreInvisible()
{
    clear_all_fields();

    YerithWindowsCommons::rendreInvisible();
}


void YerithERPGroupesDemployesHRWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    populateComboBoxes();

    setYerithSqlTableModel(_curEMPLOYEEGroupTableModel);

    _curStocksTableModel = stocksTableModel;

    setVisible(true);

    afficher_GROUPESDEMPLOYES(*_curEMPLOYEEGroupTableModel);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("groupes d'employés [hr]"),
    								 _curEMPLOYEEGroupTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerithERPGroupesDemployesHRWindow::definirManager()
{
    _logger->log("definirManager");

    tableView_GROUPESDEMPLOYES->setVisible(true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupeDEMPLOYES,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curEMPLOYEEGroupTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_filtrer->enable(this, SLOT(filtrer_GROUPESDEMPLOYES()));
    pushButton_GROUPES_DE_PAIE->enable(this,
                                                      SLOT
                                                      (GROUPES_DE_PAIE_hr
                                                       ()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_creer_groupe->enable(this, SLOT(CREER_GROUPE_demployes_hr()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
}


void YerithERPGroupesDemployesHRWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupeDEMPLOYES,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    tableView_GROUPESDEMPLOYES->setVisible(false);

    pushButton_add_condition_SEARCH->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_GROUPES_DE_PAIE->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_creer_groupe->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerithERPGroupesDemployesHRWindow::afficher_au_detail()
{
    if (_curEMPLOYEEGroupTableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_DETAILS_DUN_GROUPE_DEMPLOYES_hr->rendreVisible(0,
                                                                  _curEMPLOYEEGroupTableModel,
                                                                  _curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("DÉTAILS D'1 GROUPE D'EMPLOYÉS"),
                                   QObject::tr
                                   ("Sélectionnez un GROUPE D'EMPLOYÉS à afficher les détails !"));
    }
}


void YerithERPGroupesDemployesHRWindow::refineYerithLineEdits()
{
    _EmployeeGroupDateCreationFilter.clear();

    _EmployeeGroupDateCreationFilter.append(QString
                                          (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                          arg
                                          (YerithDatabaseTableColumn::DATE_CREATION,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_debut->
                                            date()),
                                           YerithDatabaseTableColumn::DATE_CREATION,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_fin->
                                            date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_GROUPESDEMPLOYES(*_curEMPLOYEEGroupTableModel);
}


bool YerithERPGroupesDemployesHRWindow::filtrer()
{
    QString groupes_demployes_TableColumnValue = lineEdit_resultat_filtre->text();

    if (groupes_demployes_TableColumnValue.isEmpty())
    {
        QString msg =
        		QObject::tr("Veuillez saisir une valeur numérique pour la recherche.");

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);

        return false;
    }

    QString groupes_demployes_TableColumnProperty = comboBox_element->currentText();

    QString mathOperator = comboBox_condition->currentText();

    QString filterString;

    QString REAL_DB_ID_NAME_groupes_demployes_TableColumnProperty
				(YerithDatabaseTableColumn::_tableColumnToUserViewString
							.key(groupes_demployes_TableColumnProperty));

    filterString.append(QString("%2 %3 '%4'")
    						.arg(REAL_DB_ID_NAME_groupes_demployes_TableColumnProperty,
    							 mathOperator,
								 groupes_demployes_TableColumnValue));

    //qDebug() << QString("filterString: %1")
    //                              .arg(filterString);

    _curEMPLOYEEGroupTableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curEMPLOYEEGroupTableModel
				->easySelect("src/windows/crm/yerith-erp-groupes-de-clients-window.cpp", 1076);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        afficher_GROUPESDEMPLOYES(*_curEMPLOYEEGroupTableModel);

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


void YerithERPGroupesDemployesHRWindow::reinitialiser_elements_filtrage()
{
    lineEdit_resultat_filtre->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithERPGroupesDemployesHRWindow::reinitialiser_recherche()
{
    lineEdit_nom_element_string_db->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *) this);

    afficher_GROUPESDEMPLOYES();
}


void YerithERPGroupesDemployesHRWindow::set_filtrer_font()
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


bool YerithERPGroupesDemployesHRWindow::export_csv_file()
{
    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_GROUPESDEMPLOYES,
                                               tableColumnsToIgnore,
                                               "yerith-erp-EMPLOYEE-group-listing-csv-format",
                                               "employee group listing");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_GROUPESDEMPLOYES,
                                               tableColumnsToIgnore,
                                               "yerith-erp-liste-DEMPLOYES-groupes-format-csv",
                                               "fiche des groupes des EMPLOYES");
    }

    return success;
}


QString YerithERPGroupesDemployesHRWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_EMPLOYEE_GROUP_LISTING_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_EMPLOYEE_GROUP_LISTING_tex;
    }

    return _latex_template_print_pdf_content;
}
