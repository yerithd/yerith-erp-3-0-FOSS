/*
 * yerith-erp-creer-groupe-de-clients-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-creer-groupe-de-clients-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"


YerithCreerGroupeDeClientsWindow::YerithCreerGroupeDeClientsWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithCreerGroupeDeClientsWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("créer un groupe de clients"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YERITH_BLACK_0_0_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(YerithDatabase::GROUPES_DE_CLIENTS);

    setupLineEdits();

    setupLineEditsQCompleters();


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this,
            SLOT(clients()));
    connect(actionListerLesGroupesDeClients, SIGNAL(triggered()), this,
            SLOT(groupes_de_clients()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionSupprimerUnMembreDuGroupeDeClients, SIGNAL(triggered()),
            this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerUnGroupeDeClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));


    connect(tableWidget_creer_groupe_clients_membres_initiaux_du_groupe,
            SIGNAL(itemClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));

    connect(tableWidget_creer_groupe_clients_membres_initiaux_du_groupe,
            SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));


#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();

    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->setYerithEditable(true);

    lineEdit_creer_groupe_clients_designation->setFocus();
}

void YerithCreerGroupeDeClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerithWindowsCommons::deconnecter_utilisateur();
}


void YerithCreerGroupeDeClientsWindow::setupLineEditsQCompleters()
{
    lineEdit_creer_groupe_clients_recherche_client_initiaux
		->setupMyStaticQCompleter(YerithDatabase::CLIENTS,
								  YerithDatabaseTableColumn::NOM_ENTREPRISE);

    connect((QObject *) lineEdit_creer_groupe_clients_recherche_client_initiaux->getMyQCompleter(),
			SIGNAL(activated(const QString &)),
			this,
            SLOT(ajouter_un_membre_au_groupe_de_clients(const QString &)));
}


void YerithCreerGroupeDeClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithCreerGroupeDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerithCreerGroupeDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_de_clients
                                    ()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDeClients
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDeClients
                                        ()));
}


void YerithCreerGroupeDeClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_de_clients
                                    ()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDeClients
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDeClients
                                        ()));
}


void YerithCreerGroupeDeClientsWindow::update_client_group_member_data(const
                                                                       QString &
                                                                       new_client_group_designation,
                                                                       const
                                                                       QString &
                                                                       new_client_group_db_ID,
                                                                       const
                                                                       QString &
                                                                       client_db_ID)
{
    QString
    strQueryClientGroupMemberDATA(QString
                                  ("select %1, %2 from %3 where %4='%5'").arg
                                  (YerithDatabaseTableColumn::
                                   GROUPES_DU_CLIENT,
                                   YerithDatabaseTableColumn::
                                   GROUPES_DU_CLIENT_ID,
                                   YerithDatabase::CLIENTS,
                                   YerithDatabaseTableColumn::ID,
                                   client_db_ID));

    QSqlQuery query;

    int querySize =
                    YerithUtils::execQuery(query, strQueryClientGroupMemberDATA, _logger);

    if (querySize <= 0)
    {
        return;
    }

    QString groupes_du_clients;
    QString groupes_du_clients_ID;

    if (query.next())
    {
        groupes_du_clients = query.value(0).toString();
        groupes_du_clients_ID = query.value(1).toString();
    }

    query.clear();

    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
    (new_client_group_designation, groupes_du_clients);

    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
    (new_client_group_db_ID, groupes_du_clients_ID);

    QString
    strQuery_UPDATE_ClientGroupMemberDATA(QString
                                          ("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").
                                          arg(YerithDatabase::CLIENTS,
                                              YerithDatabaseTableColumn::
                                              GROUPES_DU_CLIENT,
                                              groupes_du_clients,
                                              YerithDatabaseTableColumn::
                                              GROUPES_DU_CLIENT_ID,
                                              groupes_du_clients_ID,
                                              YerithDatabaseTableColumn::ID,
                                              client_db_ID));

//      QDEBUG_STRING_OUTPUT_2("strQuery_UPDATE_ClientGroupMemberDATA", strQuery_UPDATE_ClientGroupMemberDATA);

    YerithUtils::execQuery(query, strQuery_UPDATE_ClientGroupMemberDATA,
                           _logger);
}


bool YerithCreerGroupeDeClientsWindow::clientGroupAlreadyExists()
{
    bool result = false;

    YerithSqlTableModel &groupes_de_clients_TableModel =
                    _allWindows->getSqlTableModel_groupes_de_clients();

    // ** check if customer account with same name exist
    result =
                    YerithUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                                groupes_de_clients_TableModel,
                                                                *lineEdit_creer_groupe_clients_recherche_client_initiaux,
                                                                QObject::tr
                                                                ("Un groupe de client"),
                                                                YerithDatabaseTableColumn::
                                                                NOM_ENTREPRISE);

    return result;
}


bool YerithCreerGroupeDeClientsWindow::check_fields()
{
    bool result;

    bool nom_du_groupe_de_clients_creer =
                    lineEdit_creer_groupe_clients_designation->checkField();

    bool nombre_maximum_de_membres =
                    lineEdit_creer_groupe_clients_maximum_de_membres->checkField();

    bool programme_de_fidelite_clients =
                    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->checkField();


    result = nom_du_groupe_de_clients_creer &&
             nombre_maximum_de_membres 		&&
			 programme_de_fidelite_clients;

    return result;
}


void YerithCreerGroupeDeClientsWindow::clear_all_fields()
{
    lineEdit_creer_groupe_clients_recherche_client_initiaux->clear();

    lineEdit_creer_groupe_clients_reference_groupe->clear();
    lineEdit_creer_groupe_clients_designation->clearField();
    lineEdit_creer_groupe_clients_maximum_de_membres->clearField();
    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->clearField();

    textEdit_creer_groupe_clients_description_groupe->clear();

    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->
    yerithClearTableWidgetContent();
}


void YerithCreerGroupeDeClientsWindow::rendreInvisible()
{
    clear_all_fields();
    YerithWindowsCommons::rendreInvisible();
}


void YerithCreerGroupeDeClientsWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    populateComboBoxes();

    setupLineEditsQCompleters();

    check_fields();

    lineEdit_creer_groupe_clients_designation->setFocus();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("créer un groupe de clients"),
      								 _curStocksTableModel);

    setVisible(true);
}


bool YerithCreerGroupeDeClientsWindow::
	IS_client_present_dans_un_groupe_de_clients(const QString &aClientGroupMemberName)
{
    QTableWidgetItem *anItem = 0;

    for (unsigned int k = 0;
         k < tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->rowCount();
         ++k)
    {
        anItem = tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->item(k, 1);

        if (0 != anItem)
        {
            if (YerithUtils::isEqualCaseInsensitive(aClientGroupMemberName,
                                                    anItem->text()))
            {
                return true;
            }
        }
    }

    return false;
}


void
YerithCreerGroupeDeClientsWindow::ajouter_un_membre_au_groupe_de_clients
(const QString &aClientGroupMemberName)
{
    if (IS_client_present_dans_un_groupe_de_clients(aClientGroupMemberName))
    {
        return;
    }

    int
    MAXIMUM_DE_MEMBRES_DU_GROUPE =
                    lineEdit_creer_groupe_clients_maximum_de_membres->text().toInt();

    if (tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->rowCount()
            == MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES"),
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES ('%1') INITIAUX ATTEINT !").
                                       arg(QString::
                                           number
                                           (MAXIMUM_DE_MEMBRES_DU_GROUPE)));

        return;
    }

    YerithSqlTableModel &aClientSqlTableModel =
                    _allWindows->getSqlTableModel_clients();

    aClientSqlTableModel.setFilter(QString("%1='%2'").arg
                                   (YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                    aClientGroupMemberName));

    QSqlRecord
    aClientRecord = aClientSqlTableModel.record(0);

    QString
    aClient_db_ID =
                    GET_SQL_RECORD_DATA(aClientRecord, YerithDatabaseTableColumn::ID);

    QString
    aClientReference = GET_SQL_RECORD_DATA(aClientRecord,
                                           YerithDatabaseTableColumn::REFERENCE_CLIENT);


    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->
    addAClientGroupMember(aClient_db_ID, aClientReference,
                          aClientGroupMemberName);

    aClientSqlTableModel.resetFilter();

    lineEdit_creer_groupe_clients_recherche_client_initiaux->myClear();
}


void YerithCreerGroupeDeClientsWindow::annuler_la_creation_dun_groupe_de_clients()
{
    clear_all_fields();

    groupes_de_clients();
}


void YerithCreerGroupeDeClientsWindow::supprimerUnMembreDunGroupeDeClients()
{
    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->removeArticle
    (_last_selected_table_widget_row);
}


bool YerithCreerGroupeDeClientsWindow::creerEnregistrerUnGroupeDeClients()
{
    if (!check_fields())
    {
        if (QMessageBox::Ok ==
                YerithQMessageBox::warning(this,
                                           QObject::tr
                                           ("groupe de FIDÉLITÉ clients"),
                                           QObject::tr
                                           ("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }


    int NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS =
                    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->rowCount();

    int MAXIMUM_DE_MEMBRES_DU_GROUPE =
                    lineEdit_creer_groupe_clients_maximum_de_membres->text().toInt();

    if (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS >
            MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        uint difference =
                        (uint) qFabs(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS
                                     - MAXIMUM_DE_MEMBRES_DU_GROUPE);

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES"),
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES: %1\n\n"
                                        "NOMBRE DE MEMBRES INITIAUX DU GROUPE DE FIDÉLITÉ CLIENT: %2\n\n"
                                        "Veuillez corriger le nombre maximum de membres à '%3' ou "
                                        "diminuer son nombre de membres initiaux de ('%4 MEMBRES') !").
                                       arg(QString::
                                           number
                                           (MAXIMUM_DE_MEMBRES_DU_GROUPE),
                                           QString::
                                           number
                                           (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS),
                                           QString::
                                           number
                                           (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS),
                                           QString::number(difference)));

        return false;
    }


    if (clientGroupAlreadyExists())
    {
        return false;
    }


    QString retMsg(QObject::tr("Le groupe de clients '"));

    YerithSqlTableModel &clientGroupTableModel =
                    _allWindows->getSqlTableModel_groupes_de_clients();


    QSqlRecord record = clientGroupTableModel.record();


    QString new_client_group_designation =
                    lineEdit_creer_groupe_clients_designation->text();

    int new_client_group_db_ID =
                    YerithERPWindows::getNextIdSqlTableModel_groupes_de_clients();


    record.setValue(YerithDatabaseTableColumn::ID, new_client_group_db_ID);

    record.setValue(YerithDatabaseTableColumn::DATE_CREATION,
                    DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

    record.setValue(YerithDatabaseTableColumn::REFERENCE_GROUPE,
                    lineEdit_creer_groupe_clients_reference_groupe->text());

    record.setValue(YerithDatabaseTableColumn::DESIGNATION,
                    new_client_group_designation);

    record.setValue(YerithDatabaseTableColumn::DESCRIPTION_GROUPE,
                    textEdit_creer_groupe_clients_description_groupe->toPlainText());

    record.setValue(YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                    lineEdit_creer_groupe_clients_maximum_de_membres->text().toInt());

    record.setValue(YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
                    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->currentText());


    QMapIterator<int, QString> it_MapListIdxToElement_db_ID
								(tableWidget_creer_groupe_clients_membres_initiaux_du_groupe
										->get_mapListIdxToElement_db_ID());


    QString a_cur_client_db_ID;

    QString membres_du_groupe_db_ID;

    while (it_MapListIdxToElement_db_ID.hasNext())
    {
        it_MapListIdxToElement_db_ID.next();

        a_cur_client_db_ID = it_MapListIdxToElement_db_ID.value();

        update_client_group_member_data(new_client_group_designation,
                                        QString::number(new_client_group_db_ID),
                                        a_cur_client_db_ID);

        YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(a_cur_client_db_ID,
        															membres_du_groupe_db_ID);
    }

    record.setValue(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                    membres_du_groupe_db_ID);


    retMsg.append(lineEdit_creer_groupe_clients_designation->text());

    bool success = clientGroupTableModel.insertNewRecord(record,
    													 this,
														 "src/windows/crm/creer/yerith-erp-creer-groupe-de-clients-window.cpp",
														 647,
														 new_client_group_designation);

    if (!success)
    {
        retMsg.append(QObject::tr("' n'a pas pu être créer !"));

        YerithQMessageBox::warning(this,
                                   QObject::tr("créer 1 groupe de clients"),
								   retMsg);
        return false;
    }

    clientGroupTableModel.select();

    retMsg.append(QObject::tr("' a été créer avec succès !"));

    YerithQMessageBox::information(this,
                                   QObject::tr("créer 1 groupe de clients"),
								   retMsg);

    clear_all_fields();

    groupes_de_clients();

    return true;
}
