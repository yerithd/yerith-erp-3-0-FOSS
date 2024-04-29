/*
 * yerith-erp-programmes-de-fidelite-clients-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-programmes-de-fidelite-clients-window.hpp"

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


YerithERPProgrammesDeFideliteClientsWindow::YerithERPProgrammesDeFideliteClientsWindow()
:YerithWindowsCommons("yerith-erp-liste-programmes-de-fidelite-clients"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS),
 _logger(new YerithLogger("YerithERPProgrammesDeFideliteClientsWindow")),
 _pushButton_filtrer_font(0),
 _curClient_LOYALTY_PROGRAM_TableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("programmes de fidélité clients"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_programmes_de_fidelite_clients);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
            << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionAfficher_ce_programmeDeFideliteClients_au_detail
        << actionSupprimerUnProgrammeDeFideliteClients;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION
    (tableView_programmes_de_fidelite_clients);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn
    (YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS);


    _lineEditsToANDContentForSearch.insert
    (&lineEdit_programmes_de_fidelite_clients_terme_recherche,
     YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString.insert
    (YerithDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION
    (YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS);

    reinitialiser_colones_db_visibles();

    _curClient_LOYALTY_PROGRAM_TableModel =
                    &_allWindows->getSqlTableModel_programmes_de_fidelite_clients();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_programmes_de_fidelite_clients->
    setSqlTableName(&YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_ce_programmeDeFideliteClients_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnProgrammeDeFideliteClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_afficher->disable(this);
    pushButton_groupe_de_clients->disable(this);
    pushButton_creer_un_programme_de_fidelite_clients->disable(this);
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
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenuClients, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficher_ce_programmeDeFideliteClients_au_detail,
            SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionSupprimerUnProgrammeDeFideliteClients, SIGNAL(triggered()),
            this, SLOT(supprimer_un_programme_de_fidelite_clients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));

    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
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

    connect(actionAfficher_ce_programmeDeFideliteClients_au_detail,
            SIGNAL(triggered()), this, SLOT(afficher_au_detail()));

    connect(tableView_programmes_de_fidelite_clients,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}


YerithERPProgrammesDeFideliteClientsWindow::
~YerithERPProgrammesDeFideliteClientsWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void
YerithERPProgrammesDeFideliteClientsWindow::
slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    afficher_programmes_de_fidelite_clients
    (*_curClient_LOYALTY_PROGRAM_TableModel);
}


void
YerithERPProgrammesDeFideliteClientsWindow::
textChangedSearchLineEditsQCompleters()
{
    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString
    searchTerm(lineEdit_programmes_de_fidelite_clients_terme_recherche->text
               ());

    if (!searchTerm.isEmpty())
    {
        QStringList
        searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString
        partSearchTerm;

        int
        lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("(%1 OR %2 OR %3)").arg
                                 (GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::
                                   DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS,
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


    YerithLineEdit *
    aYerithLineEdit = 0;

    QString
    correspondingDBFieldKeyValue;

    QString
    aTableColumnFieldContentForANDSearch;

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
        afficher_programmes_de_fidelite_clients(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithERPProgrammesDeFideliteClientsWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithERPProgrammesDeFideliteClientsWindow::populateComboBoxes()
{
    _logger->log("populateClientsComboBoxes");

    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    YerithUtils::REMOVE_QSTRING_LIST(aQStringList,
                                     _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME);

    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_CREATION);
    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS);
    aQStringList.removeAll(YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);



    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS));

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


void YerithERPProgrammesDeFideliteClientsWindow::setupLineEdits()
{
    lineEdit_programmes_de_fidelite_clients_terme_recherche->setFocus();

    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_programmes_de_fidelite_clients_nombre_de_lignes_par_page;

    lineEdit_programmes_de_fidelite_clients_terme_recherche->enableForSearch
    (QObject::tr
     ("terme à rechercher (description du programme fidélité de clients)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_nombre_de_programmes->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_programmes_de_fidelite_clients_nombre_de_lignes_par_page,
     tableView_programmes_de_fidelite_clients);
}


void YerithERPProgrammesDeFideliteClientsWindow::setupDateTimeEdits()
{
    dateEdit_programmes_de_fidelite_clients_debut->setStartDate
    (YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_programmes_de_fidelite_clients_fin->setStartDate
    (GET_CURRENT_DATE);

    _client_LOYALTY_PROGRAM_DateCreationFilter.clear();

    _client_LOYALTY_PROGRAM_DateCreationFilter.append(QString
                                                      (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                                      arg
                                                      (YerithDatabaseTableColumn::DATE_CREATION,
                                                       DATE_TO_DB_FORMAT_STRING
                                                       (dateEdit_programmes_de_fidelite_clients_debut->date
                                                        ()),
                                                       YerithDatabaseTableColumn::DATE_CREATION,
                                                       DATE_TO_DB_FORMAT_STRING
                                                       (dateEdit_programmes_de_fidelite_clients_fin->date
                                                        ())));

    connect(dateEdit_programmes_de_fidelite_clients_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));

    connect(dateEdit_programmes_de_fidelite_clients_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));
}


void YerithERPProgrammesDeFideliteClientsWindow::initialiser_FILTRES()
{
    comboBox_element->setVisible(false);
    comboBox_condition->setVisible(false);
    lineEdit_resultat_filtre->setVisible(false);
    pushButton_reinitialiser_filtre->setVisible(false);
    pushButton_filtrer->setVisible(false);
}


void YerithERPProgrammesDeFideliteClientsWindow::handle_filtres_checkBox(int
                                                                         state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element->setVisible(IS_VISIBLE);
    comboBox_condition->setVisible(IS_VISIBLE);
    lineEdit_resultat_filtre->setVisible(IS_VISIBLE);
    pushButton_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_filtrer->setVisible(IS_VISIBLE);
}


void
YerithERPProgrammesDeFideliteClientsWindow::supprimer_PLUSIEURS_programmes_de_fidelite_clients
(YerithSqlTableModel &a_LOYALTY_PROGRAM_TableModel)
{
    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    QString cur_client_group_designation;

    QString SELECT_CURRENT_CLIENT_GROUP_DESIGNATION;

    QString YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING;

    QString UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING;


    int cur_query_size = -1;

    QSqlQuery a_sql_query;


    QMapIterator < QString,
                 QString >
                 j(tableView_programmes_de_fidelite_clients->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();


        YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING.append(QString
                                                                   ("DELETE FROM %1 WHERE %2 = '%3'; ").
                                                                   arg
                                                                   (YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                                                                    YerithDatabaseTableColumn::ID,
                                                                    j.value
                                                                    ()));


        SELECT_CURRENT_CLIENT_GROUP_DESIGNATION
            =
                            QString("select %1 from %2 where %3='%4'").arg
                            (YerithDatabaseTableColumn::DESIGNATION,
                             YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                             YerithDatabaseTableColumn::ID, j.value());

        cur_query_size =
                        YerithUtils::execQuery(a_sql_query,
                                               SELECT_CURRENT_CLIENT_GROUP_DESIGNATION);

        if (cur_query_size > 0 && a_sql_query.next())
        {
            cur_client_group_designation =
                            a_sql_query.value(YerithDatabaseTableColumn::DESIGNATION).
                            toString();

            UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING.append(QString
                                                                  ("UPDATE %1 SET %2='' WHERE %3='%4'; ").
                                                                  arg
                                                                  (YerithDatabase::GROUPES_DE_CLIENTS,
                                                                   YerithDatabaseTableColumn::
                                                                   PROGRAMME_DE_FIDELITE_CLIENTS,
                                                                   YerithDatabaseTableColumn::
                                                                   PROGRAMME_DE_FIDELITE_CLIENTS,
                                                                   cur_client_group_designation));
        }
    }

    QString
    msgConfirmation(QObject::tr
                    ("Supprimer les programmes de fidélité clients sélectionés ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool
        success =
                        YerithUtils::execQuery
                        (YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING);

        QString
        msg(QObject::tr
            ("Les programmes fidélité de clients sélectionés"));

        if (success && a_LOYALTY_PROGRAM_TableModel.select())
        {
//              QDEBUG_STRING_OUTPUT_2("UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING",
//                                                         UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING);

            bool
            success_update_client_group =
                            YerithUtils::execQuery
                            (UPDATE_CLIENT_GROUP_SELECTED_ROWS_QUERY_STRING);

            setupLineEditsQCompleters((QObject *) this);

            msg.append(QObject::tr
                       (" ont été supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"), msg,
                                           QMessageBox::Ok);

            if (!success_update_client_group)
            {
                YerithQMessageBox::warning(this,
                                           QObject::tr
                                           ("actualisation - échec"),
                                           QObject::tr
                                           ("les données des groupes de clients "
                                            "N'ONT pas pu être actualisées avec succès !"));

                YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

                return;
            }
        }
        else
        {
            msg.append(QObject::tr
                       (" n'ont pas pu être supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - échec"), msg,
                                           QMessageBox::Ok);
        }
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


void
YerithERPProgrammesDeFideliteClientsWindow::
supprimer_un_programme_de_fidelite_clients()
{
    YerithSqlTableModel *
    programmeDeFideliteClientsTableModel = 0;

    if (_curClient_LOYALTY_PROGRAM_TableModel &&
            YerithUtils::isEqualCaseInsensitive(YerithDatabase::
                                                PROGRAMMES_DE_FIDELITE_CLIENTS,
                                                _curClient_LOYALTY_PROGRAM_TableModel->
                                                sqlTableName()))
    {
        programmeDeFideliteClientsTableModel =
                        _curClient_LOYALTY_PROGRAM_TableModel;
    }
    else
    {
        return;
    }


    if (tableView_programmes_de_fidelite_clients->
            lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_programmes_de_fidelite_clients
        (*programmeDeFideliteClientsTableModel);

        tableView_programmes_de_fidelite_clients->clearSelection();

        afficher_programmes_de_fidelite_clients();

        return;
    }


    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    QSqlRecord
    record;

    _allWindows->
    _programmesDeFideliteClientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    if (record.isEmpty()
            || record.isNull(YerithDatabaseTableColumn::DESIGNATION))
    {
        return;
    }

    QString
    designation(GET_SQL_RECORD_DATA
                (record, YerithDatabaseTableColumn::DESIGNATION));

    QString
    msgConfirmation(QObject::tr
                    ("Supprimer le programme de fidélité clients '%1' ?").arg
                    (designation));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool
        success =
                        _allWindows->
                        _programmesDeFideliteClientsWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
                        ();

        QString
        msg(QObject::tr("Le programme de fidélité clients '%1").arg
            (designation));

        if (success && programmeDeFideliteClientsTableModel->select())
        {
            QString
            UPDATE_FROM_CLIENT_GROUP_QUERY_STRING(QString
                                                  ("UPDATE %1 SET %2='' WHERE %3='%4'").
                                                  arg(YerithDatabase::
                                                      GROUPES_DE_CLIENTS,
                                                      YerithDatabaseTableColumn::
                                                      PROGRAMME_DE_FIDELITE_CLIENTS,
                                                      YerithDatabaseTableColumn::
                                                      PROGRAMME_DE_FIDELITE_CLIENTS,
                                                      designation));

            bool
            success_update_client_group =
                            YerithUtils::execQuery(UPDATE_FROM_CLIENT_GROUP_QUERY_STRING);

            setupLineEditsQCompleters((QObject *) this);

            tableView_programmes_de_fidelite_clients->clearSelection();

            afficher_programmes_de_fidelite_clients();

            msg.append(QObject::tr
                       ("' a été supprimée de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"), msg,
                                           QMessageBox::Ok);

            if (!success_update_client_group)
            {
                msg =
                                QObject::tr("les données du groupe de client ('%1') "
                                                "N'ONT pas pu être actualisées avec succès !").
                                arg(designation);

                YerithQMessageBox::warning(this,
                                           QObject::tr
                                           ("actualisation - échec"), msg);
            }
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

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


void
YerithERPProgrammesDeFideliteClientsWindow::

afficher_programmes_de_fidelite_clients(YerithSqlTableModel &
                                        client_LOYALTY_PROGRAM_TableModel)
{
    tableView_programmes_de_fidelite_clients->
    queryYerithTableViewCurrentPageContentRow
    (client_LOYALTY_PROGRAM_TableModel);

    tableView_show_or_hide_columns(*tableView_programmes_de_fidelite_clients);

    int
    rowCount = tableView_programmes_de_fidelite_clients->rowCount();

    lineEdit_nombre_de_programmes->setText(GET_NUM_STRING(rowCount));
}


bool
YerithERPProgrammesDeFideliteClientsWindow::
filtrer_programmes_de_fidelite_client()
{
    return false;
}


void
YerithERPProgrammesDeFideliteClientsWindow::
reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DATE_CREATION
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::LOCALISATION
            << YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS;
}


void
YerithERPProgrammesDeFideliteClientsWindow::contextMenuEvent
(QContextMenuEvent *event)
{
    QMenu
    menu(this);

    menu.setPalette(toolBar_programmes_de_fidelite_clientsWindow->palette());
    menu.addAction(actionAfficher_ce_programmeDeFideliteClients_au_detail);
    menu.addAction(actionSupprimerUnProgrammeDeFideliteClients);
    menu.exec(event->globalPos());
}


void YerithERPProgrammesDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithERPProgrammesDeFideliteClientsWindow::resetFilter()
{
}


void YerithERPProgrammesDeFideliteClientsWindow::rendreVisible(YerithSqlTableModel* stocksTableModel)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setYerithSqlTableModel(_curClient_LOYALTY_PROGRAM_TableModel);

    _curStocksTableModel = stocksTableModel;

    setVisible(true);

    afficher_programmes_de_fidelite_clients(*_curClient_LOYALTY_PROGRAM_TableModel);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("programmes de fidélité clients"),
    								 _curClient_LOYALTY_PROGRAM_TableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerithERPProgrammesDeFideliteClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_ce_programmeDeFideliteClients_au_detail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnProgrammeDeFideliteClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curClient_LOYALTY_PROGRAM_TableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_filtrer->enable(this,
                               SLOT(filtrer_programmes_de_fidelite_client
                                    ()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_groupe_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_creer_un_programme_de_fidelite_clients->enable(this,
                                                              SLOT
                                                              (creer_un_programme_de_fidelite_clients
                                                               ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
}


void YerithERPProgrammesDeFideliteClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_ce_programmeDeFideliteClients_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnProgrammeDeFideliteClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_afficher->disable(this);
    pushButton_groupe_de_clients->disable(this);
    pushButton_creer_un_programme_de_fidelite_clients->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerithERPProgrammesDeFideliteClientsWindow::afficher_au_detail()
{
    if (_curClient_LOYALTY_PROGRAM_TableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_detailsDunProgrammeDeFideliteClientsWindow->rendreVisible
        (_curClient_LOYALTY_PROGRAM_TableModel, _curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("détails d'un \"programme de fidélité clients\""),
                                   QObject::tr
                                   ("Sélectionnez un \"programme de fidélité clients\" à afficher les détails !"));
    }
}


void YerithERPProgrammesDeFideliteClientsWindow::refineYerithLineEdits()
{
    _client_LOYALTY_PROGRAM_DateCreationFilter.clear();

    _client_LOYALTY_PROGRAM_DateCreationFilter.append(QString
                                                      (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                                      arg
                                                      (YerithDatabaseTableColumn::DATE_CREATION,
                                                       DATE_TO_DB_FORMAT_STRING
                                                       (dateEdit_programmes_de_fidelite_clients_debut->date
                                                        ()),
                                                       YerithDatabaseTableColumn::DATE_CREATION,
                                                       DATE_TO_DB_FORMAT_STRING
                                                       (dateEdit_programmes_de_fidelite_clients_fin->date
                                                        ())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_programmes_de_fidelite_clients
    (*_curClient_LOYALTY_PROGRAM_TableModel);
}


bool YerithERPProgrammesDeFideliteClientsWindow::filtrer()
{
    QString client_group_TableColumnValue(lineEdit_resultat_filtre->text());

    if (client_group_TableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche."));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;
    }

    QString client_group_TableColumnProperty(comboBox_element->currentText());

    QString mathOperator(comboBox_condition->currentText());

    QString filterString;

    QString
    REAL_DB_ID_NAME_client_group_TableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.
     key(client_group_TableColumnProperty));

    filterString.append(QString("%2 %3 '%4'").arg
                        (REAL_DB_ID_NAME_client_group_TableColumnProperty,
                         mathOperator, client_group_TableColumnValue));

    //qDebug() << QString("filterString: %1")
    //                              .arg(filterString);

    _curClient_LOYALTY_PROGRAM_TableModel->yerithSetFilter_WITH_where_clause
    (filterString);

    int resultRows =
    		_curClient_LOYALTY_PROGRAM_TableModel
				->easySelect("src/windows/crm/yerith-erp-programmes-de-fidelite-clients-windows.cpp", 1004);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        afficher_programmes_de_fidelite_clients
        (*_curClient_LOYALTY_PROGRAM_TableModel);

        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "clients - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "clients - filtrer");
    }

    return false;
}


void YerithERPProgrammesDeFideliteClientsWindow::reinitialiser_elements_filtrage()
{
    lineEdit_resultat_filtre->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithERPProgrammesDeFideliteClientsWindow::reinitialiser_recherche()
{
    lineEdit_nom_element_string_db->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *) this);

    afficher_programmes_de_fidelite_clients();
}


void YerithERPProgrammesDeFideliteClientsWindow::set_filtrer_font()
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


bool YerithERPProgrammesDeFideliteClientsWindow::export_csv_file()
{
    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_programmes_de_fidelite_clients,
                                               tableColumnsToIgnore,
                                               "yerith-erp-client-LOYALTY-PROGRAM-group-listing-csv-format",
                                               "client LOYALTY PROGRAM listing");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_programmes_de_fidelite_clients,
                                               tableColumnsToIgnore,
                                               "yerith-erp-liste-des-programmes-de-fidelite-clients-format-csv",
                                               "fiche des programmes de fidélité clients");
    }

    return success;
}


QString YerithERPProgrammesDeFideliteClientsWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _latex_template_print_pdf_content = YerithUtils::EN_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex;
    }
    else
    {
        _latex_template_print_pdf_content = YerithUtils::FR_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex;
    }

    return _latex_template_print_pdf_content;
}
