/*
 * yerith-erp-groupes-dun-client-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-groupes-dun-client-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"


#include <QtWidgets/QTableWidgetItem>

#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerithGroupesDunClientWindow::YerithGroupesDunClientWindow()
:YerithQTableWidgetWindowsCommons("yerith-erp-groupes-dun-client"),
 _Last_SelectedRow_Row_INDEX(0),
 _logger(new YerithLogger("YerithGroupesDunClientWindow")),
 _curClientsTableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("groupes d'1 client"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);



    _a_CURRENT_table_widget = tableWidget_groupes_dun_client;

    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_groupes_dun_client->font());


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
        << pushButton_retirer;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
        << action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionAfficher_ce_groupe_au_detail;


    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}")
    						     .arg(COLOUR_RGB_STRING_YERITH_GOLD_245_194_17);

    _curClientsTableModel = &_allWindows->getSqlTableModel_clients();

    setupLineEdits();

    populateComboBoxes();

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    connect(action_parametrer_les_impressions,
    		SIGNAL(triggered()),
			this,
            SLOT(setup_print()));

    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		(YerithQTableWidgetWindowsCommons *) this,
            SLOT(incrementFontSize__OF_TABLE__WIDGET()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		(YerithQTableWidgetWindowsCommons *) this,
            SLOT(decrementFontSize__OF_TABLE__WIDGET()));


    connect(action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne,
            SIGNAL(triggered()),
            this,
            SLOT(imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne_WITH_A_YERITH_PROGRESS_BAR()));


    connect(actionAfficherPDF,
    		SIGNAL(triggered()),
			this,
			SLOT(imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()));

    connect(actionExporter_au_format_csv,
    		SIGNAL(triggered()),
			this,
			SLOT(export_csv_file()));


    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionListerDesGroupesDeClients, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAfficher_ce_groupe_au_detail, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


    connect(tableWidget_groupes_dun_client,
            SIGNAL(clicked(const QModelIndex &)),
            this,
            SLOT(setLast_SelectedRow_Row_ID(const QModelIndex &)));

    connect(tableWidget_groupes_dun_client,
            SIGNAL(pressed(const QModelIndex &)),
            this,
            SLOT(setLast_SelectedRow_Row_ID(const QModelIndex &)));


    connect(tableWidget_groupes_dun_client,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(afficher_au_detail(const QModelIndex &)));

    connect(comboBox_groupes_dun_client_recherche,
            SIGNAL(activated(const QString &)),
            this,
			SLOT(ajouter_appartenance(const QString &)));

    setupShortcuts();

    comboBox_groupes_dun_client_recherche->setYerithEditable(true);

    comboBox_groupes_dun_client_recherche->setFocus();
}


bool YerithGroupesDunClientWindow::imprimer_UNE_LISTE(bool *full_list /*= 0 */)
{
	QString a_yerith_qtable_widget__tex_table;

	QString a_yerith_qtable_widget__TexDocument_In_Out;


    if (0 != full_list)
    {
        if (*full_list)
        {
            get_PRINT_OUT_TexTableString(a_yerith_qtable_widget__tex_table);
        }
        else
        {
            int a_row_FOR_pdf_printing_max = getLast_SelectedRow_Row_ID() + 1;

            get_PRINT_OUT_TexTableString(a_yerith_qtable_widget__tex_table,
                                         a_row_FOR_pdf_printing_max);
        }
    }
    else
    {
        return false;
    }


	emit SIGNAL_INCREMENT_PROGRESS_BAR(11);


	YerithUtils::get_GROUPES_DUN_client_TexDocumentString
                                (a_yerith_qtable_widget__TexDocument_In_Out,
                                 a_yerith_qtable_widget__tex_table);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(23);


	QString NOM_Client(_curClient_NOM_ENTREPRISE);


    YerithSqlTableModel &CLIENT_TableModel = _allWindows->getSqlTableModel_clients();

    CLIENT_TableModel.yerithSetFilter_WITH_where_clause(
        QString("%1 = '%2'")
        .arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
             NOM_Client));

    a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHCLIENT",
            YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client));

	if (CLIENT_TableModel.easySelect("src/windows/crm/yerith-erp-groupes-dun-client-window.cpp", 224) > 0)
	{
		QSqlRecord record = CLIENT_TableModel.record(0);

		QString Client_Ville(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::VILLE));

		QString Client_POBox(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::BOITE_POSTALE));

		QString Client_Email(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::EMAIL));

		QString Client_Tel(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

		if (Client_Tel.isEmpty())
		{
			Client_Tel.append(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
		}

		//qDebug() << "++ Client_Email: " << Client_Email;

		if (!Client_Ville.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out
			.replace("CLIENTYERITHCITY",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Ville));
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("CLIENTYERITHCITY", "");
		}

		if (!Client_POBox.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("CLIENTYERITHPOBOX", Client_POBox);
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("CLIENTYERITHPOBOX", "N/a");
		}

		if (!Client_Email.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("CLIENTYERITHMAIL",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Email));
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("CLIENTYERITHMAIL", "");
		}

		if (!Client_Tel.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out
			.replace("CLIENTYERITHPHONE",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Tel));
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("CLIENTYERITHPHONE", "");
		}

		CLIENT_TableModel.resetFilter();
	}



	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


    QString file_name_Subject_Title_FRENCH =
        (*full_list) ? QString("Groupes d'appartenance du CLIENT '%1'")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client))
                     : QString("Groupes d'appartenance du CLIENT '%1' (PARTIELLE)")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client));


    QString file_name_Subject_Title_ENGLISH =
        (*full_list) ? QString("client '%1' belonging GROUPS")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client))
                     : QString("client '%1' belonging GROUPS (EXCERPT)")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client));


    QString prefix_file_name_FRENCH =
        (*full_list) ? "yerith-erp-GROUPES-DUN-client-HR"
                     : "yerith-erp-GROUPES-DUN-client-HR-PARTIELLE";


    QString prefix_file_name_ENGLISH =
        (*full_list) ? "yerith-erp-CLIENT-group-hr"
                     : "yerith-erp-CLIENT-group-hr-PARTIAL";


	return
        YerithQTableWidgetWindowsCommons::
                imprimer_UNE_LISTE(*tableWidget_groupes_dun_client,
                                   a_yerith_qtable_widget__TexDocument_In_Out,
                                   file_name_Subject_Title_FRENCH,
                                   file_name_Subject_Title_ENGLISH,
                                   prefix_file_name_FRENCH,
                                   prefix_file_name_ENGLISH);
}


void YerithGroupesDunClientWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_menuGroupesDunClientWindow->palette());
    menu.addAction(action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne);
    menu.addAction(actionAfficher_ce_groupe_au_detail);
    menu.exec(event->globalPos());
}


void YerithGroupesDunClientWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithGroupesDunClientWindow::ajouter_appartenance(const QString &un_groupe_de_clients)
{
    YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION(this,
                                                          &un_groupe_de_clients,
                                                          &YerithGroupesDunClientWindow::executer_ajouter_appartenance);

    comboBox_groupes_dun_client_recherche->resetYerithComboBox();

    handle_some_actions_tools_enabled();
}


void YerithGroupesDunClientWindow::executer_ajouter_appartenance(const QString *un_groupe_de_clients)
{
    if (0 == un_groupe_de_clients)
    {
        return;
    }

    QString
    SELECT_GROUP_FROM_DB_TABLE(QString
                               ("select %1, %2, %3, %4 from %5 where %6='%7'").
                               arg(YerithDatabaseTableColumn::ID,
                                   YerithDatabaseTableColumn::
                                   REFERENCE_GROUPE,
                                   YerithDatabaseTableColumn::
                                   MEMBRES_DU_GROUPE_db_ID,
                                   YerithDatabaseTableColumn::
                                   MAXIMUM_DE_MEMBRES,
                                   YerithDatabase::GROUPES_DE_CLIENTS,
                                   YerithDatabaseTableColumn::DESIGNATION,
                                   *un_groupe_de_clients));

    QSqlQuery aQSqlQuery;

    int query_size =
                    YerithUtils::execQuery(aQSqlQuery, SELECT_GROUP_FROM_DB_TABLE);

    if (query_size <= 0)
    {
        return;
    }

    QSqlRecord aClientGroupRecordInfo;

    if (!aQSqlQuery.next())
    {
        return;
    }

    aClientGroupRecordInfo = aQSqlQuery.record();

    int maximum_de_membres = GET_SQL_RECORD_DATA(aClientGroupRecordInfo,
                                                 YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES).
                             toInt();

    QString membres_du_groupe_db_ID =
                    GET_SQL_RECORD_DATA(aClientGroupRecordInfo,
                                        YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

    QStringList membres_du_groupe_db_ID_LIST;

    YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
                                                membres_du_groupe_db_ID);

    if (membres_du_groupe_db_ID_LIST.size() >= maximum_de_membres)
    {
        YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr
                                       ("Le groupe de clients '%1' a déjà atteint son nombre maximum de membres (%2) !").
                                       arg(*un_groupe_de_clients,
                                           QString::
                                           number(maximum_de_membres)));

        return;
    }


    if (YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
            (_curClientDBID, membres_du_groupe_db_ID))
    {

        QString
        INSERT_UPDATE_CLIENT_WITHIN_GROUP(QString
                                          ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                          arg(YerithDatabase::
                                              GROUPES_DE_CLIENTS,
                                              YerithDatabaseTableColumn::
                                              MEMBRES_DU_GROUPE_db_ID,
                                              membres_du_groupe_db_ID,
                                              YerithDatabaseTableColumn::
                                              DESIGNATION,
                                              *un_groupe_de_clients));

        bool insert_update_success =
                        YerithUtils::execQuery(INSERT_UPDATE_CLIENT_WITHIN_GROUP);

//              QDEBUG_STRING_OUTPUT_2("insert_update_success - 1", BOOL_TO_STRING(insert_update_success));

        if (!insert_update_success)
        {
            YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                           QObject::tr
                                           ("Le client '%1' n'a pas pu être ajouté "
                                            "dans le groupe de clients '%2' !").
                                           arg(_curClient_NOM_ENTREPRISE,
                                               *un_groupe_de_clients));

            return;
        }
    }

    QSqlRecord record;

    _allWindows->
    _clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString groupes_du_client = GET_SQL_RECORD_DATA(record,
                                                    YerithDatabaseTableColumn::GROUPES_DU_CLIENT);

    bool update_DB_GROUPES_DU_CLIENT_TABLE =
                    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (*un_groupe_de_clients,
                     groupes_du_client);

    QString groupes_du_client_id = GET_SQL_RECORD_DATA(record,
                                                       YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID);

    QString un_groupe_de_clients_db_ID =
                    GET_SQL_RECORD_DATA(aClientGroupRecordInfo,
                                        YerithDatabaseTableColumn::ID);


    bool update_groupes_du_client_id =
                    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (un_groupe_de_clients_db_ID,
                     groupes_du_client_id);

    update_DB_GROUPES_DU_CLIENT_TABLE =
                    update_DB_GROUPES_DU_CLIENT_TABLE || update_groupes_du_client_id;


    if (!update_DB_GROUPES_DU_CLIENT_TABLE)
    {
        YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr
                                       ("Le client '%1' est déjà membre du groupe de clients '%2' !").
                                       arg(_curClient_NOM_ENTREPRISE,
                                           *un_groupe_de_clients));
        return;
    }


    QString
    UPDATE_CLIENT_GROUP_INFORMATION(QString
                                    ("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").
                                    arg(YerithDatabase::CLIENTS,
                                        YerithDatabaseTableColumn::
                                        GROUPES_DU_CLIENT,
                                        groupes_du_client,
                                        YerithDatabaseTableColumn::
                                        GROUPES_DU_CLIENT_ID,
                                        groupes_du_client_id,
                                        YerithDatabaseTableColumn::ID,
                                        _curClientDBID));

    bool update_client_group_success =
                    YerithUtils::execQuery(UPDATE_CLIENT_GROUP_INFORMATION);

    if (!update_client_group_success)
    {
        YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr
                                       ("Le client '%1' n'a pas pu être ajouté "
                                        "dans le groupe de clients '%2' !").arg
                                       (_curClient_NOM_ENTREPRISE,
                                        *un_groupe_de_clients));

        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

        return;
    }

    tableWidget_groupes_dun_client->insert_group(GET_SQL_RECORD_DATA
                                                 (aClientGroupRecordInfo,
                                                  YerithDatabaseTableColumn::
                                                  ID), *un_groupe_de_clients,
                                                 GET_SQL_RECORD_DATA
                                                 (aClientGroupRecordInfo,
                                                  YerithDatabaseTableColumn::
                                                  REFERENCE_GROUPE),
                                                 QString::
                                                 number(maximum_de_membres));


    lineEdit_groupes_dun_client_nombre_de_groupes->setText(QString::number
                                                           (tableWidget_groupes_dun_client->rowCount
                                                            ()));


    YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                   QObject::tr
                                   ("Le client '%1' a été ajouté dans le groupe de clients '%2' !").
                                   arg(_curClient_NOM_ENTREPRISE,
                                       *un_groupe_de_clients));
}


void YerithGroupesDunClientWindow::retirer_ce_client_du_groupe_selectionne()
{
    YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK(this,
            &YerithGroupesDunClientWindow::EXECUTER_retirer_ce_client_du_groupe_selectionne);

    comboBox_groupes_dun_client_recherche->resetYerithComboBox();

    handle_some_actions_tools_enabled();
}


bool YerithGroupesDunClientWindow::EXECUTER_retirer_ce_client_du_groupe_selectionne()
{
    QString SELECT_GROUP_DU_CLIENT_INFO =
                QString("select %1, %2 from %3 where %4='%5'")
                    .arg(YerithDatabaseTableColumn::GROUPES_DU_CLIENT,
                         YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
                         YerithDatabase::CLIENTS,
                         YerithDatabaseTableColumn::ID,
                         _curClientDBID);


    QSqlQuery aQSqlQuery;

    int query_size = YerithUtils::execQuery(aQSqlQuery,
                                            SELECT_GROUP_DU_CLIENT_INFO);

    if (query_size <= 0)
    {
        return false;
    }


    QSqlRecord aClientRecordGroupInfo;

    if (!aQSqlQuery.next())
    {
        return false;
    }

    aClientRecordGroupInfo = aQSqlQuery.record();

    QString groupes_du_client_ID =
                GET_SQL_RECORD_DATA(aClientRecordGroupInfo,
                                    YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID);


    QString groupes_du_client =
                GET_SQL_RECORD_DATA(aClientRecordGroupInfo,
                                    YerithDatabaseTableColumn::GROUPES_DU_CLIENT);


    int currentRow = tableWidget_groupes_dun_client->currentRow();

//      QDEBUG_STRING_OUTPUT_1(QString("groupes_du_client_ID: %1, currentRow: %2")
//                                                              .arg(groupes_du_client_ID,
//                                                                       QString::number(currentRow)));

    QString clientGroup_db_ID =
                tableWidget_groupes_dun_client->get_DB_ELEMENT_db_ID(currentRow);

//      QDEBUG_STRING_OUTPUT_1(QString("clientGroup_db_ID: %1, groupes_du_client_ID: %2")
//                                                              .arg(clientGroup_db_ID,
//                                                                       groupes_du_client_ID));

    YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
                    (groupes_du_client_ID,
                     clientGroup_db_ID);

//      QDEBUG_STRING_OUTPUT_1(QString("clientGroup_db_ID: %1, groupes_du_client_ID: %2")
//                                                              .arg(clientGroup_db_ID,
//                                                                       groupes_du_client_ID));

    QTableWidgetItem *item =
        tableWidget_groupes_dun_client->item(currentRow,
                                             0);

    if (0 == item)
    {
        return false;
    }


    QString clientGroup_designation = item->text().trimmed();


    QString msgRETIRER =
        QObject::tr("Retirer le client : (%1) du groupe de clients \"%2\" ?")
            .arg(_curClient_NOM_ENTREPRISE,
                 clientGroup_designation);


    if (QMessageBox::Cancel ==
            YerithQMessageBox::question(this,
                                        QObject::tr("Retirer le client d'1 groupe de clients"),
                                        msgRETIRER,
                                        QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        return false;
    }


    YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
                    (groupes_du_client,
                     clientGroup_designation);


    bool result = true;

    {
        QString UPDATE_GROUPES_DU_CLIENT_INFO =
            QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
                .arg(YerithDatabase::CLIENTS,
                     YerithDatabaseTableColumn::GROUPES_DU_CLIENT,
                     groupes_du_client,
                     YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
                     groupes_du_client_ID,
                     YerithDatabaseTableColumn::ID,
                     _curClientDBID);

        result = result && YerithUtils::execQuery(UPDATE_GROUPES_DU_CLIENT_INFO);
    }

    {
        aQSqlQuery.clear();

        QSqlRecord a_client_group_record_info;

        SELECT_GROUP_DU_CLIENT_INFO.clear();

        SELECT_GROUP_DU_CLIENT_INFO =
            QString("select %1 from %2 where %3='%4'").arg
            (YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
             YerithDatabase::GROUPES_DE_CLIENTS,
             YerithDatabaseTableColumn::DESIGNATION, clientGroup_designation);

//              QDEBUG_STRING_OUTPUT_2("SELECT_GROUP_DU_CLIENT_INFO",
//                                                         SELECT_GROUP_DU_CLIENT_INFO);

        result = result
                 && YerithUtils::execQuery(aQSqlQuery, SELECT_GROUP_DU_CLIENT_INFO);

        if (!aQSqlQuery.next())
        {
            return false;
        }

        a_client_group_record_info = aQSqlQuery.record();

        QString
        membres_du_groupe_db_ID =
            GET_SQL_RECORD_DATA(a_client_group_record_info,
                                YerithDatabaseTableColumn::
                                MEMBRES_DU_GROUPE_db_ID);


//              QDEBUG_STRING_OUTPUT_2("membres_du_groupe_db_ID *",
//                                                         membres_du_groupe_db_ID);


        YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
        (membres_du_groupe_db_ID, _curClientDBID);


//              QDEBUG_STRING_OUTPUT_2("membres_du_groupe_db_ID ***",
//                                                         membres_du_groupe_db_ID);


        QString
        UPDATE_GROUPES_DE_CLIENT_DB_TABLE(QString
                                          ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                          arg(YerithDatabase::
                                              GROUPES_DE_CLIENTS,
                                              YerithDatabaseTableColumn::
                                              MEMBRES_DU_GROUPE_db_ID,
                                              membres_du_groupe_db_ID,
                                              YerithDatabaseTableColumn::
                                              DESIGNATION,
                                              clientGroup_designation));

        result = result
                 && YerithUtils::execQuery(UPDATE_GROUPES_DE_CLIENT_DB_TABLE);
    }


    if (result)
    {
        tableWidget_groupes_dun_client->removeArticle(currentRow);

        int tableWidget_groupes_dun_client_RowCount =
            tableWidget_groupes_dun_client->rowCount();

        lineEdit_groupes_dun_client_nombre_de_groupes
            ->setText(QString::number(tableWidget_groupes_dun_client_RowCount));


        msgRETIRER.clear();

        msgRETIRER.append(
            QObject::tr("Le client : (%1) a été retiré du groupe de clients \"%2\" !")
                .arg(_curClient_NOM_ENTREPRISE,
                     clientGroup_designation));


        YerithQMessageBox::information(this,
                                       QObject::tr("Retirer le client d'1 groupe de clients"),
                                       msgRETIRER);
    }

    return result;
}


void YerithGroupesDunClientWindow::afficher_au_detail(const QModelIndex &modelIndex)
{
    if (_curClientsTableModel->rowCount() > 0)
    {
        _allWindows->
        _detailsGroupeDeClientsWindow->rendreVisible(_curClientsTableModel,
                                                     _curStocksTableModel,
                                                     tableWidget_groupes_dun_client->
                                                     get_DB_ELEMENT_db_ID
                                                     (modelIndex.row()));

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client à afficher les détails."));
    }
}


void YerithGroupesDunClientWindow::afficher_tous_les_groupes_du_client()
{
    tableWidget_groupes_dun_client->yerithClearTableWidgetContent();

    QSqlRecord record;

    _allWindows->
    _clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    _curClient_NOM_ENTREPRISE =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NOM_ENTREPRISE);

    _curClientDBID =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID);


    groupBox_groupes_dun_client->setTitle(QObject::tr
                                          ("Groupes d'appartenance du client (%1)").
                                          arg(_curClient_NOM_ENTREPRISE));


    QString
    groupes_du_client(GET_SQL_RECORD_DATA
                      (record,
                       YerithDatabaseTableColumn::GROUPES_DU_CLIENT));


    QStringList
    allClientGroups(groupes_du_client.split(YerithUtils::STAR_CHAR));

    QString current_group_db_ID;

    QString maximum_de_membres;

    QString current_client_group_reference;

    int curQuerySize = -1;

    QSqlQuery aSqlGroupTableModelQUERY;

    QString clientGroupTableModelQUERY_STR;

    for (int k = 0; k < allClientGroups.size(); ++k)
    {
        clientGroupTableModelQUERY_STR =
                        QString("select %1, %2, %3 from %4 where %5='%6'").arg
                        (YerithDatabaseTableColumn::ID,
                         YerithDatabaseTableColumn::REFERENCE_GROUPE,
                         YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                         YerithDatabase::GROUPES_DE_CLIENTS,
                         YerithDatabaseTableColumn::DESIGNATION,
                         allClientGroups.at(k).trimmed());

        aSqlGroupTableModelQUERY.clear();

        curQuerySize =
                        YerithUtils::execQuery(aSqlGroupTableModelQUERY,
                                               clientGroupTableModelQUERY_STR, _logger);

        if (curQuerySize > 0)
        {
            if (aSqlGroupTableModelQUERY.next())
            {
                current_group_db_ID =
                                QString::number(aSqlGroupTableModelQUERY.value(0).toInt());

                current_client_group_reference =
                                aSqlGroupTableModelQUERY.value(1).toString();

                maximum_de_membres =
                                aSqlGroupTableModelQUERY.value(2).toString();

                tableWidget_groupes_dun_client->insert_group
                (current_group_db_ID, allClientGroups.at(k).trimmed(),
                 current_client_group_reference, maximum_de_membres);
            }

//              qDebug() << tableWidget_groupes_dun_client->get_mapListIdxToElement_db_ID();
        }
    }

    if (tableWidget_groupes_dun_client->rowCount() > 0)
    {
        tableWidget_groupes_dun_client->selectRow(0);
    }
}


void YerithGroupesDunClientWindow::get_PRINT_OUT_TexTableString(QString       &texTable_IN_OUT,
                                                                int           row_MAX_TO_GO_export /* = -1 */)
{
	texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
                           "\\centering\n"
						   "\\resizebox{\\textwidth}{!}{\n"
						   "\\begin{tabular}"
						   "{|c|l|l|r|} \\hline");

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texTable_IN_OUT.append("& & &				        \\\\ \n"
                               "\\textbf{ID}			    & 	 \n"
							   "\\textbf{GROUP NAME}		& 	 \n"
							   "\\textbf{GROUP REFERENCE} 	& 	 \n"
							   "\\textbf{MAX MEMBERS}		\\\\ \n"
							   "& &	&				        \\\\ \\hline \\hline \n");
	}
	else //FRANCAIS
	{
		texTable_IN_OUT.append("& & &						        \\\\ \n"
                               "\\textbf{ID}			            & 	 \n"
							   "\\textbf{DÉSIGNATION DU GROUPE}		& 	 \n"
							   "\\textbf{RÉFÉRENCE DU GROUPE} 		& 	 \n"
							   "\\textbf{NOMBRE DE MEMBRES MAXIMUM}	\\\\ \n"
							   "& &	&						        \\\\ \\hline \\hline \n");
	}


	int rowCount = tableWidget_groupes_dun_client->rowCount();

	int columnCount = tableWidget_groupes_dun_client->columnCount();


    int MAX_TABLE_MODDEL_ROW_COUNT__to_export = rowCount;

	if (row_MAX_TO_GO_export > -1)
	{
        MAX_TABLE_MODDEL_ROW_COUNT__to_export = row_MAX_TO_GO_export;
	}



	bool color_this_row_grey = true;


	int LINE_COUNT_PER_PDF_PAGE = 41;


	QString cell_text;


    int TABLE_COUNT = qFloor(rowCount / LINE_COUNT_PER_PDF_PAGE);

    int current_table_count = 0;

	int current_pdf_page_line_count = 0;

	uint Visual_ID_counter = 0;

	//Tex table body
	for (int i = 0;
         i < MAX_TABLE_MODDEL_ROW_COUNT__to_export && current_pdf_page_line_count <= LINE_COUNT_PER_PDF_PAGE;
         ++i)
	{
        color_this_row_grey = (0 == i%2);

        if (color_this_row_grey)
        {
            texTable_IN_OUT.append(QString("\\rowcolor{yerithColorGray}"));
        }
        else
        {
            texTable_IN_OUT.append(QString("\\rowcolor{white}"));
        }


        //We add a cell for row numbering wioth an ID number.
        {
            ++Visual_ID_counter;

            QTableWidgetItem *an_item_ID = new QTableWidgetItem(QString::number(Visual_ID_counter));

            if (0 != an_item_ID)
            {
                YerithUtils::handleTexTableItemText(columnCount,
                                                    texTable_IN_OUT,
                                                    0,
                                                    an_item_ID->text());
            }
        }


        for (int j = 0; j < columnCount; ++j)
        {
            QTableWidgetItem *an_item = 0;

            an_item = tableWidget_groupes_dun_client->item(i, j);

            if (0 != an_item)
            {
                if (j != 4)
                {
                    cell_text = an_item->text();
                }
                else
                {
                    cell_text = GET_NUM_STRING(an_item->text().toDouble());

                }
            }

            YerithUtils::handleTexTableItemText(columnCount,
                                                texTable_IN_OUT,
                                                j,
                                                cell_text);
        }

        if (i < rowCount - 1)
        {
            texTable_IN_OUT.append("\\hline\n");
        }


        ++current_pdf_page_line_count;


        if (LINE_COUNT_PER_PDF_PAGE - 1 == current_pdf_page_line_count)
        {
            current_pdf_page_line_count = 0;

            if (current_table_count < TABLE_COUNT)
            {
                texTable_IN_OUT.append("\\hline\n"
                                       "\\end{tabular}}\n"
                                       "\\end{table*}\n");

                texTable_IN_OUT.append("\\newpage\n");

                texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
                                       "\\centering\n"
                                       "\\resizebox{\\textwidth}{!}{\n"
                                       "\\begin{tabular}"
                                       "{|c|l|l|r|} \\hline");

                if (YerithMainWindow::LANGUE_ANGLAISE)
                {
                    texTable_IN_OUT.append("& & &				        \\\\ \n"
                                           "\\textbf{ID}			    & 	 \n"
                                           "\\textbf{GROUP NAME}		& 	 \n"
                                           "\\textbf{GROUP REFERENCE} 	& 	 \n"
                                           "\\textbf{MAX MEMBERS}		\\\\ \n"
                                           "& &	&				        \\\\ \\hline \\hline \n");
                }
                else //FRANCAIS
                {
                    texTable_IN_OUT.append("& & &						        \\\\ \n"
                                           "\\textbf{ID}			            & 	 \n"
                                           "\\textbf{DÉSIGNATION DU GROUPE}		& 	 \n"
                                           "\\textbf{RÉFÉRENCE DU GROUPE} 		& 	 \n"
                                           "\\textbf{NOMBRE DE MEMBRES MAXIMUM}	\\\\ \n"
                                           "& &	&						        \\\\ \\hline \\hline \n");
                }
            }

            ++current_table_count;

            if (current_table_count > 0)
            {
                LINE_COUNT_PER_PDF_PAGE = 57;
            }
        }

    } //for-i


    texTable_IN_OUT.append("\\hline\n"
                           "\\end{tabular}}\n"
                           "\\end{table*}\n");

}


void YerithGroupesDunClientWindow::populateComboBoxes()
{
	comboBox_groupes_dun_client_recherche
    	->populateComboBoxRawString(YerithDatabase::GROUPES_DE_CLIENTS,
                              	    YerithDatabaseTableColumn::DESIGNATION);

	comboBox_groupes_dun_client_recherche->setYerithEnabled(true);
}


void YerithGroupesDunClientWindow::rendreInvisible()
{
    tableWidget_groupes_dun_client->yerithClearTableWidgetContent();

    YerithWindowsCommons::rendreInvisible();
}


void YerithGroupesDunClientWindow::rendreVisible(YerithSqlTableModel *clientTableModel,
                                                 YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    populateComboBoxes();

    tableWidget_groupes_dun_client->resizeColumnsToContents();

    afficher_tous_les_groupes_du_client();

    int tableRowCount = tableWidget_groupes_dun_client->rowCount();

    setVisible(true);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("groupes d'1 client"),
    								 _curClientsTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS_QTABLEWIDGET(*this);

    lineEdit_groupes_dun_client_nombre_de_groupes
		->setText(GET_NUM_STRING(tableRowCount));


    handle_some_actions_tools_enabled();
}


bool YerithGroupesDunClientWindow::export_csv_file()
{
    bool success = false;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success =
            YerithUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_groupes_dun_client,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "Client group export listing");
    }
    else
    {
        success =
            YerithUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_groupes_dun_client,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "FICHE D'EXPORTATION DES groupes de fidélité d'1 clients");
    }

    return success;
}


bool YerithGroupesDunClientWindow::
        imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne()
{
	if (tableWidget_groupes_dun_client->rowCount() <= 0)
	{
		YerithQMessageBox::information(this,
				QObject::tr("Impression d'1 LISTE de groupes d'1 client"),
				QObject::tr("PAS DE DONNÉES À Imprimer maintenant !"));

		return false;
	}


    int a_row_FOR_pdf_printing_max = getLast_SelectedRow_Row_ID() + 1;


	QString partial_GROUPES_DUN_client__tex_table;


	QString PARTIAL__GROUPES_DUN_CLIENT__TexDocument;


	get_PRINT_OUT_TexTableString(partial_GROUPES_DUN_client__tex_table,
                                 a_row_FOR_pdf_printing_max);





	YerithUtils::get_GROUPES_DUN_client_TexDocumentString(PARTIAL__GROUPES_DUN_CLIENT__TexDocument,
														  partial_GROUPES_DUN_client__tex_table);


    emit SIGNAL_INCREMENT_PROGRESS_BAR(18);



    QString NOM_Client(_curClient_NOM_ENTREPRISE);



	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
        PARTIAL__GROUPES_DUN_CLIENT__TexDocument
            .replace("YERITHSUBJECT",
                     QString("client '%1' belonging GROUPS (Excerpt)")
                        .arg(NOM_Client));
	}
	else
	{
        PARTIAL__GROUPES_DUN_CLIENT__TexDocument
            .replace("YERITHSUBJECT",
                     QString("Groupes d'appartenance du CLIENT '%1' (Partiel)")
                        .arg(NOM_Client));
	}


	//qDebug() << PARTIAL__GROUPES_DUN_CLIENT__TexDocument;


	emit SIGNAL_INCREMENT_PROGRESS_BAR(40);


    PARTIAL__GROUPES_DUN_CLIENT__TexDocument
            .append(partial_GROUPES_DUN_client__tex_table)
            .append("\n")
            .append("\\end{document}");


	emit SIGNAL_INCREMENT_PROGRESS_BAR(50);


	YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();


	QString factureDate(infoEntreprise.getVille_LATEX());


	YerithUtils::getCurrentSimplifiedDate(factureDate);


    PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHPAPERSPEC",
											get_PRINTING_PARAMETER_printing_position());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHBOITEPOSTALE", infoEntreprise.getBoitePostal());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHDATE", factureDate);

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHHEUREVENTE", CURRENT_TIME);

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());


	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHNOMUTILISATEUR",
			QString("%1 %2")
			.arg(YerithUtils::getAllWindows()->getUser()->titre(),
					YerithUtils::getAllWindows()->getUser()->nom_completTex()));


	PARTIAL__GROUPES_DUN_CLIENT__TexDocument
        .replace("YERITHSUCCURSALE",
                 YerithUtils::LATEX_IN_OUT_handleForeignAccents
                    (YerithERPConfig::THIS_SITE_LOCALISATION_NAME));


	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);



	YerithSqlTableModel &CLIENT_TableModel = _allWindows->getSqlTableModel_clients();

	CLIENT_TableModel.yerithSetFilter_WITH_where_clause(
			QString("%1 = '%2'")
			.arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
					NOM_Client));

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YERITHCLIENT",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client));


	emit SIGNAL_INCREMENT_PROGRESS_BAR(60);


	if (CLIENT_TableModel.easySelect("src/windows/crm/yerith-erp-groupes-dun-client-window.cpp", 114) > 0)
	{
		QSqlRecord record = CLIENT_TableModel.record(0);

		QString Client_Ville(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::VILLE));

		QString Client_POBox(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::BOITE_POSTALE));

		QString Client_Email(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::EMAIL));

		QString Client_Tel(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

		if (Client_Tel.isEmpty())
		{
			Client_Tel.append(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
		}

		//qDebug() << "++ Client_Email: " << Client_Email;

		if (!Client_Ville.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument
			.replace("CLIENTYERITHCITY",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Ville));
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHCITY", "");
		}

		if (!Client_POBox.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHPOBOX", Client_POBox);
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHPOBOX", "N/a");
		}

		if (!Client_Email.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHMAIL",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Email));
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHMAIL", "");
		}

		if (!Client_Tel.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument
			.replace("CLIENTYERITHPHONE",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Tel));
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHPHONE", "");
		}

		CLIENT_TableModel.resetFilter();
	}


	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


    QString prefixFileName;


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        prefixFileName
            .append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                        ("yerith-erp-CLIENT-group-hr-PARTIAL-"));
    }
    else
    {
        prefixFileName
            .append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                        ("yerith-erp-Groupe-dun-Client-hr-PARTIAL-"));
    }

	//qDebug() << "++\n" << PARTIAL__GROUPES_DUN_CLIENT__TexDocument;

	QFile tmpLatexFile(prefixFileName + "tex");

	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(PARTIAL__GROUPES_DUN_CLIENT__TexDocument.toUtf8());
	}

	tmpLatexFile.close();

	QString pdfReceiptFileName = YerithERPProcess::compileLatex(prefixFileName);

	if (!pdfReceiptFileName.isEmpty())
	{
		YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
	}

	emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

	return true;
}


//bool YerithGroupesDunClientWindow::imprimer_pdf_document()
//{
////	QDEBUG_STRING_OUTPUT_1("YerithGroupesDunClientWindow::imprimer_pdf_document");
//
//	if (tableWidget_groupes_dun_client->rowCount() <= 0)
//	{
//		YerithQMessageBox::information(this,
//				QObject::tr("impression"),
//				QObject::tr("Il n'y a pas de données à imprimer !"));
//
//		return false;
//	}
//
//	QString GROUPES_DUN_client__tex_table;
//
//	//int pageNumber = qCeil(tableWidget_groupes_dun_client->rowCount() / 20);
//
//
//	YerithPOSUser *yerithUser = _allWindows->getUser();
//
//	YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();
//
//	QString GROUPES_DUN_CLIENT__TexDocument;
//
//	QString factureDate(infoEntreprise.getVille_LATEX());
//
//	YerithUtils::getCurrentSimplifiedDate(factureDate);
//
//	get_PRINT_OUT_TexTableString(GROUPES_DUN_client__tex_table);
//
//	emit SIGNAL_INCREMENT_PROGRESS_BAR(18);
//
//	YerithUtils::get_GROUPES_DUN_client_TexDocumentString(GROUPES_DUN_CLIENT__TexDocument,
//														  GROUPES_DUN_client__tex_table);
//
//	emit SIGNAL_INCREMENT_PROGRESS_BAR(50);
//
//	QString NOM_Client(_curClient_NOM_ENTREPRISE);
//
//	if (YerithMainWindow::LANGUE_ANGLAISE)
//	{
//		GROUPES_DUN_CLIENT__TexDocument
//		.replace("YERITHSUBJECT",
//				QString("client '%1' belonging GROUPS")
//				.arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client)));
//	}
//	else
//	{
//		GROUPES_DUN_CLIENT__TexDocument
//		.replace("YERITHSUBJECT",
//				QString("Groupes d'appartenance du CLIENT '%1'")
//				.arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client)));
//	}
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHPAPERSPEC",
//											get_PRINTING_PARAMETER_printing_position());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHBOITEPOSTALE", infoEntreprise.getBoitePostal());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHDATE", factureDate);
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHHEUREVENTE", CURRENT_TIME);
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());
//
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHNOMUTILISATEUR",
//			QString("%1 %2")
//			.arg(YerithUtils::getAllWindows()->getUser()->titre(),
//					YerithUtils::getAllWindows()->getUser()->nom_completTex()));
//
//	GROUPES_DUN_CLIENT__TexDocument
//	.replace("YERITHSUCCURSALE",
//			YerithUtils::LATEX_IN_OUT_handleForeignAccents
//			(YerithERPConfig::THIS_SITE_LOCALISATION_NAME));
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);
//
//
//	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);
//
//
//	YerithSqlTableModel &CLIENT_TableModel = _allWindows->getSqlTableModel_clients();
//
//	CLIENT_TableModel.yerithSetFilter_WITH_where_clause(
//			QString("%1 = '%2'")
//			.arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
//					NOM_Client));
//
//	GROUPES_DUN_CLIENT__TexDocument.replace("YERITHCLIENT",
//			YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client));
//
//	if (CLIENT_TableModel.easySelect("src/windows/crm/yerith-erp-groupes-dun-client-window.cpp", 894) > 0)
//	{
//		QSqlRecord record = CLIENT_TableModel.record(0);
//
//		QString Client_Ville(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::VILLE));
//
//		QString Client_POBox(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::BOITE_POSTALE));
//
//		QString Client_Email(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::EMAIL));
//
//		QString Client_Tel(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
//
//		if (Client_Tel.isEmpty())
//		{
//			Client_Tel.append(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
//		}
//
//		//qDebug() << "++ Client_Email: " << Client_Email;
//
//		if (!Client_Ville.isEmpty())
//		{
//			GROUPES_DUN_CLIENT__TexDocument
//			.replace("CLIENTYERITHCITY",
//					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Ville));
//		}
//		else
//		{
//			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHCITY", "");
//		}
//
//		if (!Client_POBox.isEmpty())
//		{
//			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHPOBOX", Client_POBox);
//		}
//		else
//		{
//			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHPOBOX", "N/a");
//		}
//
//		if (!Client_Email.isEmpty())
//		{
//			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHMAIL",
//					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Email));
//		}
//		else
//		{
//			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHMAIL", "");
//		}
//
//		if (!Client_Tel.isEmpty())
//		{
//			GROUPES_DUN_CLIENT__TexDocument
//			.replace("CLIENTYERITHPHONE",
//					YerithUtils::LATEX_IN_OUT_handleForeignAccents(Client_Tel));
//		}
//		else
//		{
//			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYERITHPHONE", "");
//		}
//
//		CLIENT_TableModel.resetFilter();
//	}
//
//
//	QString prefixFileName;
//
//	if (YerithMainWindow::LANGUE_ANGLAISE)
//	{
//		prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir("yerith-erp-CLIENT-group-hr"));
//	}
//	else
//	{
//		prefixFileName.append(
//				YerithUtils::getUniquePrefixFileInTemporaryFilesDir("yerith-erp-GROUPES-DUN-client-HR"));
//	}
//
//	//qDebug() << "++\n" << GROUPES_DUN_CLIENT__TexDocument;
//
//	QFile tmpLatexFile(prefixFileName + "tex");
//
//	if (tmpLatexFile.open(QFile::WriteOnly))
//	{
//		tmpLatexFile.write(GROUPES_DUN_CLIENT__TexDocument.toUtf8());
//	}
//
//	tmpLatexFile.close();
//
//	QString pdfReceiptFileName = YerithERPProcess::compileLatex(prefixFileName);
//
//	if (!pdfReceiptFileName.isEmpty())
//	{
//		YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
//	}
//
//	emit SIGNAL_INCREMENT_PROGRESS_BAR(98);
//
//	return true;
//}


void YerithGroupesDunClientWindow::definirManager()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_groupes_dun_client_recherche->setVisible(true);

    pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(clients()));
    pushButton_retirer->enable(this,
                               SLOT(retirer_ce_client_du_groupe_selectionne
                                    ()));
}


void YerithGroupesDunClientWindow::definirVendeur()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, true);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_groupes_dun_client_recherche->setVisible(false);

    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(clients()));
    pushButton_retirer->disable(this);
}


void YerithGroupesDunClientWindow::definirPasDeRole()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    comboBox_groupes_dun_client_recherche->setVisible(false);

    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);
}
