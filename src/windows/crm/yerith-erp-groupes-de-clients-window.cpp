/*
 * yerith-erp-groupes-de-clients-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-groupes-de-clients-window.hpp"

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


YerithGroupesDeClientsWindow::YerithGroupesDeClientsWindow()
:YerithWindowsCommons("yerith-erp-liste-groupes-de-clients"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::GROUPES_DE_CLIENTS),
 _logger(new YerithLogger("YerithGroupesDeClientsWindow")),
 _pushButton_filtrer_font(0),
 _curClientGroupTableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("groupes de clients"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_groupes_de_clients);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
            << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionAfficher_les_membres_de_ce_groupe
        << actionSupprimerGroupeDeClients
		<< actionAfficher_ce_groupe_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION
    (tableView_groupes_de_clients);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(YerithDatabase::GROUPES_DE_CLIENTS);


    _lineEditsToANDContentForSearch.insert
    (&lineEdit_groupes_de_clients_terme_recherche, YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString.
    insert(YerithDatabaseTableColumn::REFERENCE_GROUPE);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION
    (YerithDatabase::GROUPES_DE_CLIENTS);

    reinitialiser_colones_db_visibles();

    _curClientGroupTableModel =
                    &_allWindows->getSqlTableModel_groupes_de_clients();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_groupes_de_clients->
    setSqlTableName(&YerithDatabase::GROUPES_DE_CLIENTS);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_les_membres_de_ce_groupe, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupeDeClients,
                                           false);
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
    pushButton_creer_groupe->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_programmes_de_fidelite_clients->disable(this);
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
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficher_les_membres_de_ce_groupe, SIGNAL(triggered()),
            this, SLOT(afficher_au_detail()));
    connect(actionAfficher_ce_groupe_au_detail, SIGNAL(triggered()), this,
            SLOT(afficher_au_detail()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));


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

    connect(actionSupprimerGroupeDeClients, SIGNAL(triggered()),
            this, SLOT(supprimer_groupe_de_clients()));

    connect(tableView_groupes_de_clients,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setupShortcuts();
}


YerithGroupesDeClientsWindow::~YerithGroupesDeClientsWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithGroupesDeClientsWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    afficher_groupes_de_clients(*_curClientGroupTableModel);
}


void YerithGroupesDeClientsWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_groupes_de_clients_terme_recherche->text());

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
        afficher_groupes_de_clients(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithGroupesDeClientsWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithGroupesDeClientsWindow::clear_all_fields()
{
    lineEdit_groupes_de_clients_compte_fidelite_clients->clear();

    _CLIENT_GROUP_TO_compte_FIDELITE_CLIENTS_total_fcfa.clear();
}


void YerithGroupesDeClientsWindow::populateComboBoxes()
{
    _logger->log("populateClientsComboBoxes");

    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_CREATION);
    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_GROUPE);
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


void YerithGroupesDeClientsWindow::setupLineEdits()
{
    lineEdit_groupes_de_clients_terme_recherche->setFocus();

    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_groupes_de_clients_nombre_de_lignes_par_page;

    lineEdit_groupes_de_clients_terme_recherche->enableForSearch(QObject::
                                                                 trUtf8
                                                                 ("terme à rechercher (description du groupe de clients)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_nombre_de_groupes->setYerithEnabled(false);

    lineEdit_groupes_de_clients_compte_fidelite_clients->setYerithEnabled
    (false);


    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_groupes_de_clients_nombre_de_lignes_par_page,
     tableView_groupes_de_clients);
}


void YerithGroupesDeClientsWindow::setupDateTimeEdits()
{
    dateEdit_groupes_de_clients_debut->
    setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_groupes_de_clients_fin->setStartDate(GET_CURRENT_DATE);

    _clientGroupDateCreationFilter.clear();

    _clientGroupDateCreationFilter.append(QString
                                          (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                          arg
                                          (YerithDatabaseTableColumn::DATE_CREATION,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_groupes_de_clients_debut->
                                            date()),
                                           YerithDatabaseTableColumn::DATE_CREATION,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_groupes_de_clients_fin->
                                            date())));

    connect(dateEdit_groupes_de_clients_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));

    connect(dateEdit_groupes_de_clients_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));
}


void YerithGroupesDeClientsWindow::initialiser_FILTRES()
{
    comboBox_element->setVisible(false);
    comboBox_condition->setVisible(false);
    lineEdit_resultat_filtre->setVisible(false);
    pushButton_reinitialiser_filtre->setVisible(false);
    pushButton_filtrer->setVisible(false);
}


void YerithGroupesDeClientsWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element->setVisible(IS_VISIBLE);
    comboBox_condition->setVisible(IS_VISIBLE);
    lineEdit_resultat_filtre->setVisible(IS_VISIBLE);
    pushButton_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_filtrer->setVisible(IS_VISIBLE);
}


void
YerithGroupesDeClientsWindow::supprimer_PLUSIEURS_groupes_de_clients
(YerithSqlTableModel &aClientGroupTableModel)
{
    QString
    YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING;

    QMapIterator < QString,
                 QString > j(tableView_groupes_de_clients->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();

        YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING.append(QString
                                                                   ("DELETE FROM %1 WHERE %2 = '%3';").
                                                                   arg
                                                                   (YerithDatabase::GROUPES_DE_CLIENTS,
                                                                    YerithDatabaseTableColumn::ID,
                                                                    j.value
                                                                    ()));
    }

    QString
    msgConfirmation(QObject::tr
                    ("Supprimer les groupes de clients sélectionés ?"));

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
        msg(QObject::tr("Les groupes de clients sélectionés"));

        if (success && aClientGroupTableModel.select())
        {
            setupLineEditsQCompleters((QObject *) this);

            msg.append(QObject::tr
                       (" ont été supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"), msg,
                                           QMessageBox::Ok);
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
}


void YerithGroupesDeClientsWindow::supprimer_groupe_de_clients()
{
    YerithSqlTableModel *groupeDeClientsTableModel = 0;

    if (_curClientGroupTableModel &&
            YerithUtils::isEqualCaseInsensitive(YerithDatabase::GROUPES_DE_CLIENTS,
                                                _curClientGroupTableModel->
                                                sqlTableName()))
    {
        groupeDeClientsTableModel = _curClientGroupTableModel;
    }
    else
    {
        return;
    }


    if (tableView_groupes_de_clients->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_groupes_de_clients(*groupeDeClientsTableModel);

        tableView_groupes_de_clients->clearSelection();

        afficher_groupes_de_clients();

        return;
    }


    QSqlRecord record;

    _allWindows->
    _groupesDeClientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    if (record.isEmpty()
            || record.isNull(YerithDatabaseTableColumn::DESIGNATION))
    {
        return;
    }

    QString
    GROUP_db_ID(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID));

    QString
    designation(GET_SQL_RECORD_DATA
                (record, YerithDatabaseTableColumn::DESIGNATION));

    QString
    msgConfirmation(QObject::tr
                    ("Supprimer le groupe de clients '%1' ?").arg
                    (designation));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        QString SELECT_ALL_CLIENT_GROUP_MEMBER_ID =
                        QString("select %1 from %2").
                        arg(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                            YerithDatabase::GROUPES_DE_CLIENTS);

        QSqlQuery a_qsql_query;

        int query_size = YerithUtils::execQuery(a_qsql_query,
                                                SELECT_ALL_CLIENT_GROUP_MEMBER_ID);

        if (query_size > 0 && a_qsql_query.next())
        {
            QString membres_du_groupe_db_ID =
                            a_qsql_query.value(0).toString();

            QStringList membres_du_groupe_db_ID_LIST;

            YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING
            (membres_du_groupe_db_ID_LIST, membres_du_groupe_db_ID);

            QString cur_CLIENT_db_ID;
            QSqlQuery a_qsql_query_2;
            int query_size_2 = 0;

            for (uint j = 0; j < membres_du_groupe_db_ID_LIST.size(); ++j)
            {
                cur_CLIENT_db_ID = membres_du_groupe_db_ID_LIST.at(j);

                QString SELECT_CLIENT_GROUP_FROM_CLIENT_DATA =
                                QString("SELECT %1, %2, %3 FROM %4 WHERE %5='%6'").arg
                                (YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                 YerithDatabaseTableColumn::GROUPES_DU_CLIENT,
                                 YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
                                 YerithDatabase::CLIENTS,
                                 YerithDatabaseTableColumn::ID,
                                 cur_CLIENT_db_ID);

                a_qsql_query_2.clear();

                query_size_2 =
                                YerithUtils::execQuery(a_qsql_query_2,
                                                       SELECT_CLIENT_GROUP_FROM_CLIENT_DATA);

                if (query_size_2 > 0 && a_qsql_query_2.next())
                {
                    QString nom_entreprise_CLIENT =
                                    a_qsql_query_2.
                                    value(YerithDatabaseTableColumn::NOM_ENTREPRISE).
                                    toString();

                    QString groupes_du_client =
                                    a_qsql_query_2.
                                    value(YerithDatabaseTableColumn::GROUPES_DU_CLIENT).
                                    toString();

                    QString groupes_du_client_ID =
                                    a_qsql_query_2.
                                    value(YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID).
                                    toString();


//                                      QDEBUG_STRING_OUTPUT_2("nom_entreprise_CLIENT", nom_entreprise_CLIENT);

//                                      QDEBUG_STRING_OUTPUT_2("GROUP_db_ID", GROUP_db_ID);

                    YerithUtils::
                    REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
                    (groupes_du_client, designation);

                    YerithUtils::
                    REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
                    (groupes_du_client_ID, GROUP_db_ID);

//                                      QDEBUG_STRING_OUTPUT_2("groupes_du_client_ID", groupes_du_client_ID);

                    QString UPDATE_CLIENT_GROUP_DATA =
                                    QString
                                    ("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").arg
                                    (YerithDatabase::CLIENTS,
                                     YerithDatabaseTableColumn::GROUPES_DU_CLIENT,
                                     groupes_du_client,
                                     YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
                                     groupes_du_client_ID,
                                     YerithDatabaseTableColumn::ID,
                                     cur_CLIENT_db_ID);

//                                      QDEBUG_STRING_OUTPUT_2("UPDATE_CLIENT_GROUP_DATA", UPDATE_CLIENT_GROUP_DATA);

                    bool success_updated_client_group_data =
                                    YerithUtils::execQuery(UPDATE_CLIENT_GROUP_DATA);

                    if (!success_updated_client_group_data)
                    {
                        YerithQMessageBox::information(this,
                                                       QObject::tr
                                                       ("groupe du client - suppression - échec"),
                                                       QObject::tr
                                                       ("LES DONNÉES DE GROUPE DU CLIENT '%1'"
                                                        " NE POUVAIENT PAS ÊTRE ACTUALISÉES !").
                                                       arg
                                                       (nom_entreprise_CLIENT),
                                                       QMessageBox::Ok);
                    }
                }
            }
        }


        bool success =
                        _allWindows->
                        _groupesDeClientsWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
                        ();

        QString msg(QObject::tr("Le groupe de clients '%1").
                    arg(designation));

        if (success && groupeDeClientsTableModel->select())
        {
            setupLineEditsQCompleters((QObject *) this);

            tableView_groupes_de_clients->clearSelection();

            afficher_groupes_de_clients();

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

        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


void YerithGroupesDeClientsWindow::afficher_groupes_de_clients(YerithSqlTableModel
                                                               &
                                                               aClientGroupTableModel)
{
    _CLIENT_GROUP_TO_compte_FIDELITE_CLIENTS_total_fcfa.clear();

    int client_group_row_count =
    		aClientGroupTableModel.easySelect("src/windows/crm/yerith-erp-groupes-de-clients-window.cpp", 747);

    QString client_group_designation;

    QString client_group_members_db_ID;

    QStringList client_group_members_db_ID_LIST;


    QString SELECT_CLIENT_FOR_LOYALTY_PAYMENT_ACCOUNT;

    double previous_client_group_compte_totale_FIDELITE_CLIENTS_fcfa = 0.0;

    double compte_FIDELITE_CLIENTS = 0.0;
    double compte_totale_FIDELITE_CLIENTS_fcfa = 0.0;

    int querySize = -1;

    QSqlQuery a_qsql_query;

    QSqlRecord aClient_LOYALTY_ACCOUNT_record;

    QSqlRecord aRecord;

    for (int k = 0; k < client_group_row_count; ++k)
    {
        querySize = -1;

        aRecord.clear();

        aRecord = aClientGroupTableModel.record(k);

        client_group_designation =
                        GET_SQL_RECORD_DATA(aRecord, YerithDatabaseTableColumn::DESIGNATION);

        client_group_members_db_ID =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::
                                            MEMBRES_DU_GROUPE_db_ID);

        client_group_members_db_ID_LIST.clear();

        YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING
        (client_group_members_db_ID_LIST, client_group_members_db_ID);

        compte_totale_FIDELITE_CLIENTS_fcfa = 0.0;

        for (uint j = 0; j < client_group_members_db_ID_LIST.size(); ++j)
        {
            a_qsql_query.clear();

            aClient_LOYALTY_ACCOUNT_record.clear();

            SELECT_CLIENT_FOR_LOYALTY_PAYMENT_ACCOUNT =
                            QString("select %1 from %2 where %3='%4' AND %5!=''").arg
                            (YerithDatabaseTableColumn::
                             COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                             YerithDatabase::CLIENTS, YerithDatabaseTableColumn::ID,
                             client_group_members_db_ID_LIST.at(j),
                             YerithDatabaseTableColumn::GROUPES_DU_CLIENT);

            querySize =
                            YerithUtils::execQuery(a_qsql_query,
                                                   SELECT_CLIENT_FOR_LOYALTY_PAYMENT_ACCOUNT);

            if (querySize > 0 && a_qsql_query.next())
            {
                aClient_LOYALTY_ACCOUNT_record = a_qsql_query.record();

                compte_FIDELITE_CLIENTS =
                                GET_SQL_RECORD_DATA(aClient_LOYALTY_ACCOUNT_record,
                                                    YerithDatabaseTableColumn::
                                                    COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS).
                                toDouble();

                compte_totale_FIDELITE_CLIENTS_fcfa += compte_FIDELITE_CLIENTS;
            }
        }

        if (querySize > 0)
        {
            _CLIENT_GROUP_TO_compte_FIDELITE_CLIENTS_total_fcfa.insert
            (client_group_designation, compte_totale_FIDELITE_CLIENTS_fcfa);
        }
    }


    tableView_groupes_de_clients->queryYerithTableViewCurrentPageContentRow
    (aClientGroupTableModel);

    tableView_show_or_hide_columns(*tableView_groupes_de_clients);

    int rowCount = tableView_groupes_de_clients->rowCount();

    lineEdit_nombre_de_groupes->setText(GET_NUM_STRING(rowCount));
}


bool YerithGroupesDeClientsWindow::filtrer_groupes_de_clients()
{
    return false;
}


void YerithGroupesDeClientsWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DATE_CREATION
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::REFERENCE_GROUPE
            << YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS
            << YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES;
}


void YerithGroupesDeClientsWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_groupes_de_clientsWindow->palette());

    menu.addAction(actionAfficher_ce_groupe_au_detail);
    menu.addAction(actionAfficher_les_membres_de_ce_groupe);
    menu.addAction(actionSupprimerGroupeDeClients);

    menu.exec(event->globalPos());
}


void YerithGroupesDeClientsWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionReinitialiserRecherche->
    setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithGroupesDeClientsWindow::resetFilter()
{
}


void YerithGroupesDeClientsWindow::rendreInvisible()
{
    clear_all_fields();

    YerithWindowsCommons::rendreInvisible();
}


void YerithGroupesDeClientsWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setYerithSqlTableModel(_curClientGroupTableModel);

    _curStocksTableModel = stocksTableModel;

    setVisible(true);

    afficher_groupes_de_clients(*_curClientGroupTableModel);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("groupes de clients"),
    								 _curClientGroupTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerithGroupesDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    tableView_groupes_de_clients->setVisible(true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupeDeClients,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curClientGroupTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_filtrer->enable(this, SLOT(filtrer_groupes_de_clients()));
    pushButton_programmes_de_fidelite_clients->enable(this,
                                                      SLOT
                                                      (programmes_de_fidelite_clients
                                                       ()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_creer_groupe->enable(this, SLOT(creerUnGroupeDeClients()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
}


void YerithGroupesDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerGroupeDeClients,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    tableView_groupes_de_clients->setVisible(false);

    pushButton_add_condition_SEARCH->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_programmes_de_fidelite_clients->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_creer_groupe->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerithGroupesDeClientsWindow::afficher_au_detail()
{
    if (_curClientGroupTableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_detailsGroupeDeClientsWindow->rendreVisible(0,
                                                                  _curClientGroupTableModel,
                                                                  _curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("détails d'un groupe de clients"),
                                   QObject::tr
                                   ("Sélectionnez un groupe de clients à afficher les détails !"));
    }
}


void YerithGroupesDeClientsWindow::refineYerithLineEdits()
{
    _clientGroupDateCreationFilter.clear();

    _clientGroupDateCreationFilter.append(QString
                                          (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                          arg
                                          (YerithDatabaseTableColumn::DATE_CREATION,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_groupes_de_clients_debut->
                                            date()),
                                           YerithDatabaseTableColumn::DATE_CREATION,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_groupes_de_clients_fin->
                                            date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_groupes_de_clients(*_curClientGroupTableModel);
}


bool YerithGroupesDeClientsWindow::filtrer()
{
    QString
    groupes_de_clients_TableColumnValue(lineEdit_resultat_filtre->text());

    if (groupes_de_clients_TableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche."));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;
    }

    QString
    groupes_de_clients_TableColumnProperty(comboBox_element->currentText());

    QString mathOperator(comboBox_condition->currentText());

    QString filterString;

    QString
    REAL_DB_ID_NAME_groupes_de_clients_TableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.key
     (groupes_de_clients_TableColumnProperty));

    filterString.append(QString("%2 %3 '%4'").arg
                        (REAL_DB_ID_NAME_groupes_de_clients_TableColumnProperty,
                         mathOperator, groupes_de_clients_TableColumnValue));

    //qDebug() << QString("filterString: %1")
    //                              .arg(filterString);

    _curClientGroupTableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curClientGroupTableModel->easySelect("src/windows/crm/yerith-erp-groupes-de-clients-window.cpp", 1092);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        afficher_groupes_de_clients(*_curClientGroupTableModel);

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


void YerithGroupesDeClientsWindow::reinitialiser_elements_filtrage()
{
    lineEdit_resultat_filtre->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithGroupesDeClientsWindow::reinitialiser_recherche()
{
    lineEdit_nom_element_string_db->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *) this);

    afficher_groupes_de_clients();
}


void YerithGroupesDeClientsWindow::set_filtrer_font()
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


bool YerithGroupesDeClientsWindow::export_csv_file()
{
    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_groupes_de_clients,
                                               tableColumnsToIgnore,
                                               "yerith-erp-client-group-listing-csv-format",
                                               "client group listing");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_groupes_de_clients,
                                               tableColumnsToIgnore,
                                               "yerith-erp-liste-de-clients-groupes-format-csv",
                                               "fiche des groupes des clients");
    }

    return success;
}


QString YerithGroupesDeClientsWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_CLIENT_LOYALTY_GROUP_LISTING_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_CLIENT_LOYALTY_GROUP_LISTING_tex;
    }

    return _latex_template_print_pdf_content;
}
