/*
 * yerith-erp-details-dun-groupe-de-clients-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-details-dun-groupe-de-clients-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerithDetailsDunGroupeDeClientsWindow::YerithDetailsDunGroupeDeClientsWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithDetailsDunGroupeDeClientsWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("détails d'un groupe de clients"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YERITH_BLACK_0_0_0);

    setupLineEdits();

    textEdit_details_dun_groupe_de_clients_description_groupe->setYerithEnabled
    (false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_programmes_de_fidelite_clients->disable(this);
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
            SLOT(groupes_de_clients()));
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


void YerithDetailsDunGroupeDeClientsWindow::afficher_groupes_dun_client()
{
    if (0 == _curClientTableModel)
    {
        groupes_de_clients();
    }

    rendreInvisible();

    _allWindows->_groupesDunClientWindow->rendreVisible(_curClientTableModel,
                                                        _curStocksTableModel);
}


void YerithDetailsDunGroupeDeClientsWindow::modifier_un_groupe_de_clients()
{

}


void YerithDetailsDunGroupeDeClientsWindow::setupLineEdits()
{
    lineEdit_details_dun_groupe_de_clients_reference_groupe->setYerithEnabled(false);
    lineEdit_details_dun_groupe_de_clients_designation->setYerithEnabled(false);
    lineEdit_details_dun_groupe_de_clients_maximum_de_membres->setYerithEnabled(false);
    lineEdit_details_dun_groupe_de_clients_programme_de_fidelite_de_clients->setYerithEnabled(false);
}


void YerithDetailsDunGroupeDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_programmes_de_fidelite_clients->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
}


void YerithDetailsDunGroupeDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_programmes_de_fidelite_clients->enable(this, SLOT(programmes_de_fidelite_clients()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(groupes_de_clients()));
}


void YerithDetailsDunGroupeDeClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_programmes_de_fidelite_clients->disable(this);
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(groupes_de_clients()));
}


void YerithDetailsDunGroupeDeClientsWindow::rendreInvisible()
{
    YerithWindowsCommons::rendreInvisible();
}


void YerithDetailsDunGroupeDeClientsWindow::
		rendreVisible(YerithSqlTableModel *aClientTableModel,
                      YerithSqlTableModel *aClientGroupTableModel,
                      YerithSqlTableModel *stocksTableModel,
                      const QString &clientGroup_db_ID /* = YerithUtils::EMPTY_STRING */)
{
	retranslateUi(this);

    disconnect(actionRETOUR, 0, 0, 0);

    pushButton_RETOUR->disable(this);

    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));

    pushButton_RETOUR->enable(this, SLOT(groupes_de_clients()));


    _curClientTableModel = 0;

    _curStocksTableModel = stocksTableModel;

    _curClientGroupTableModel = aClientGroupTableModel;

    setVisible(true);

    showClientGroup_DETAIL(clientGroup_db_ID);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'un groupe de clients"),
    								 _curClientGroupTableModel);
}


void YerithDetailsDunGroupeDeClientsWindow::
		rendreVisible(YerithSqlTableModel *clientTableModel,
					  YerithSqlTableModel *stocksTableModel,
                      const QString 	  &clientGroup_db_ID /* = YerithUtils::EMPTY_STRING */)
{
	retranslateUi(this);

    disconnect(actionRETOUR, 0, 0, 0);

    pushButton_RETOUR->disable(this);

    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(afficher_groupes_dun_client()));

    pushButton_RETOUR->enable(this, SLOT(afficher_groupes_dun_client()));


    _curClientTableModel = clientTableModel;

    _curStocksTableModel = stocksTableModel;

    _curClientGroupTableModel = &_allWindows->getSqlTableModel_groupes_de_clients();

    setVisible(true);

    showClientGroup_DETAIL(clientGroup_db_ID);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'un groupe de clients"),
    								 _curClientGroupTableModel);
}


void YerithDetailsDunGroupeDeClientsWindow::
		showClientGroup_DETAIL(const QString &clientGroup_db_ID /* = YerithUtils::EMPTY_STRING */)
{
    tableWidget_details_dun_groupe_de_clients_membres_initiaux_du_groupe->yerithClearTableWidgetContent();

    QSqlRecord record;

    if (clientGroup_db_ID.isEmpty())
    {
        _allWindows->_groupesDeClientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);
    }
    else
    {
        _curClientGroupTableModel->setFilter(QString("%1='%2'").arg
                                             (YerithDatabaseTableColumn::ID,
                                              clientGroup_db_ID));

        record = _curClientGroupTableModel->record(0);

        _curClientGroupTableModel->resetFilter();
    }


    QString membres_du_groupe_db_ID = GET_SQL_RECORD_DATA(record,
                                                          YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

    QStringList membres_du_groupe_db_ID_LIST;

    YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
                                                membres_du_groupe_db_ID);


    int curQuerySize = -1;

    QSqlQuery aSqlClientTableModelQUERY;

    QString clientTableModelQUERY_STR;


    QString aClientMember_nom_entreprise;

    QString aClientMember_reference;

    QString aClientMember_db_ID;

    for (unsigned int i = 0; i < membres_du_groupe_db_ID_LIST.size(); ++i)
    {
        aClientMember_db_ID = membres_du_groupe_db_ID_LIST.at(i).trimmed();

        if (aClientMember_db_ID.isEmpty())
        {
            continue;
        }

        clientTableModelQUERY_STR =
                        QString("select %1, %2 from %3 where %4='%5'").arg
                        (YerithDatabaseTableColumn::REFERENCE_CLIENT,
                         YerithDatabaseTableColumn::NOM_ENTREPRISE, YerithDatabase::CLIENTS,
                         YerithDatabaseTableColumn::ID, aClientMember_db_ID);

        aSqlClientTableModelQUERY.clear();

        curQuerySize =
                        YerithUtils::execQuery(aSqlClientTableModelQUERY,
                                               clientTableModelQUERY_STR, _logger);

        if (curQuerySize > 0)
        {
            if (aSqlClientTableModelQUERY.next())
            {
                aClientMember_reference =
                                aSqlClientTableModelQUERY.value(0).toString();

                aClientMember_nom_entreprise =
                                aSqlClientTableModelQUERY.value(1).toString();

                tableWidget_details_dun_groupe_de_clients_membres_initiaux_du_groupe->
                addAClientGroupMember(aClientMember_db_ID,
                                      aClientMember_reference,
                                      aClientMember_nom_entreprise);
            }
        }
    }

//      QDEBUG_STRING_OUTPUT_2("designation",
//                      GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DESIGNATION));

    lineEdit_details_dun_groupe_de_clients_reference_groupe->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::REFERENCE_GROUPE));

    lineEdit_details_dun_groupe_de_clients_designation->setText
    (GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DESIGNATION));

    lineEdit_details_dun_groupe_de_clients_maximum_de_membres->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES));

    lineEdit_details_dun_groupe_de_clients_programme_de_fidelite_de_clients->
    setText(GET_SQL_RECORD_DATA
            (record,
             YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS));

    textEdit_details_dun_groupe_de_clients_description_groupe->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::DESCRIPTION_GROUPE));
}


void YerithDetailsDunGroupeDeClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
