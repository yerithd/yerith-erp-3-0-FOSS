/*
 * yerith-erp-details-dun-programme-de-fidelite-clients-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-details-dun-programme-de-fidelite-clients-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerithDetailsDunProgrammeDeFideliteClientsWindow::YerithDetailsDunProgrammeDeFideliteClientsWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithDetailsDunProgrammeDeFideliteClientsWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("détails d'un programme de fidélité clients"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
						.arg(COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0,
							 COLOUR_RGB_STRING_YERITH_BLACK_0_0_0);

    doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais->setMaximum(100.0);

    doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais->setYerithEnabled(false);

    setupLineEdits();

    textEdit_details_dun_programme_de_fidelite_clients_description->setYerithEnabled(false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenuClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionRETOUR, SIGNAL(triggered()), this,
            SLOT(programmes_de_fidelite_clients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
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

    setupShortcuts();
}


void YerithDetailsDunProgrammeDeFideliteClientsWindow::setupLineEdits()
{
    lineEdit_details_dun_programme_de_fidelite_clients_reference->
    setYerithEnabled(false);
    lineEdit_details_dun_programme_de_fidelite_clients_designation->
    setYerithEnabled(false);
}


void YerithDetailsDunProgrammeDeFideliteClientsWindow::definirPasDeRole()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
}


void YerithDetailsDunProgrammeDeFideliteClientsWindow::definirManager()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(programmes_de_fidelite_clients()));
}


void YerithDetailsDunProgrammeDeFideliteClientsWindow::rendreInvisible()
{
    comboBox_details_un_programme_de_fidelite_clients_localisation->clear();
    YerithWindowsCommons::rendreInvisible();
}


void YerithDetailsDunProgrammeDeFideliteClientsWindow::
		rendreVisible(YerithSqlTableModel *curClient_LOYALTY_PROGRAM_TableModel,
					  YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

	_curStocksTableModel = stocksTableModel;

    _curClient_LOYALTY_PROGRAM_TableModel =
                    curClient_LOYALTY_PROGRAM_TableModel;

    //qDebug() << "++ last selected row: " << YerithERPWindows::get_last_lister_selected_row_db_ID();

    setVisible(true);

    showClientLOYALTY_PROGRAM_DETAIL();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'un programme de fidélité clients"),
      								 _curStocksTableModel);
}


void YerithDetailsDunProgrammeDeFideliteClientsWindow::showClientLOYALTY_PROGRAM_DETAIL()
{
    QSqlRecord record;

    QString designation_programme_de_fidelite_clients;

    _allWindows-> _programmesDeFideliteClientsWindow
						->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    lineEdit_details_dun_programme_de_fidelite_clients_reference
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS));

    designation_programme_de_fidelite_clients =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DESIGNATION);

    lineEdit_details_dun_programme_de_fidelite_clients_designation
		->setText(designation_programme_de_fidelite_clients);

    comboBox_details_un_programme_de_fidelite_clients_localisation
		->addItem(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::LOCALISATION));


    doubleSpinBox_details_dun_programme_de_fidelite_clients_pourcentage_du_rabais
		->setValue(GET_SQL_RECORD_DATA(record,
									   YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble());


    textEdit_details_dun_programme_de_fidelite_clients_description
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS));

    tableWidget_details_dun_programe_de_fidelite_clients_groupes_de_clients_participants
		->yerithClearTableWidgetContent();

    QString SELECT_ALL_CLIENT_GROUPS_PARTICIPATING
				(QString("select %1, %2, %3, %4 from %5 where %6='%7'")
					.arg(YerithDatabaseTableColumn::ID,
                         YerithDatabaseTableColumn::DESIGNATION,
                         YerithDatabaseTableColumn::REFERENCE_GROUPE,
                         YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                         YerithDatabase::GROUPES_DE_CLIENTS,
                         YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
                         designation_programme_de_fidelite_clients));


    QSqlQuery aQSqlQuery;

    int query_size = YerithUtils::execQuery(aQSqlQuery,
                                            SELECT_ALL_CLIENT_GROUPS_PARTICIPATING);

    if (query_size <= 0)
    {
        return;
    }

    QString client_group_db_ID;
    QString designation;
    QString reference_groupe;
    QStringList client_group_member_db_ID;

    while (aQSqlQuery.next())
    {
        client_group_db_ID = aQSqlQuery.value(YerithDatabaseTableColumn::ID).toString();

        designation = aQSqlQuery.value(YerithDatabaseTableColumn::DESIGNATION).toString();

        reference_groupe = aQSqlQuery.value(YerithDatabaseTableColumn::REFERENCE_GROUPE).toString();

        client_group_member_db_ID.clear();

        YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING
						(client_group_member_db_ID,
                         aQSqlQuery.value(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID).toString());

        tableWidget_details_dun_programe_de_fidelite_clients_groupes_de_clients_participants
			->insert_group(client_group_db_ID,
						   designation,
						   reference_groupe,
                     	   QString::number(client_group_member_db_ID.size()));
    }
}


void YerithDetailsDunProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
