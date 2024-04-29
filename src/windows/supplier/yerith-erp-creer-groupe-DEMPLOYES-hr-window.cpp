/*
 * yerith-erp-creer-groupe-DEMPLOYES-hr-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-creer-groupe-DEMPLOYES-hr-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"


YerithCreerGroupeDemployesWindow::YerithCreerGroupeDemployesWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithCreerGroupeDemployesWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("créer un groupe D'EMPLOYÉS"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


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
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDEMPLOYES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionListerLesGroupesDemployes, SIGNAL(triggered()), this,
            SLOT(GROUPES_demployes_hr()));
    connect(actionEMPLOYES, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionSupprimerUnMembreDuGroupeDEMPLOYES, SIGNAL(triggered()),
            this, SLOT(supprimerUnMembreDunGroupeDemployes_HR()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerUnGroupeDEMPLOYES_hr()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));


    connect(tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe,
            SIGNAL(itemClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));

    connect(tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe,
            SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));


#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();

    comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->setYerithEditable(true);

    lineEdit_creer_groupe_DEMPLOYES_designation->setFocus();
}

void YerithCreerGroupeDemployesWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerithWindowsCommons::deconnecter_utilisateur();
}


void YerithCreerGroupeDemployesWindow::setupLineEditsQCompleters()
{
    lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux
		->setupMyStaticQCompleter(YerithDatabase::FOURNISSEURS,
								  YerithDatabaseTableColumn::NOM_ENTREPRISE,
								  QString("%1 = '1'")
								  	  .arg(YerithDatabaseTableColumn::EMPLOYE));

    connect((QObject *) lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux->getMyQCompleter(),
			SIGNAL(activated(const QString &)),
			this,
            SLOT(ajouter_un_membre_au_groupe_DEMPLOYES(const QString &)));
}


void YerithCreerGroupeDemployesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithCreerGroupeDemployesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDEMPLOYES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerithCreerGroupeDemployesWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDEMPLOYES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_demployes
                                    ()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDemployes_HR
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDEMPLOYES_hr
                                        ()));
}


void YerithCreerGroupeDemployesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDEMPLOYES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YERITH_SERVER

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_demployes
                                    ()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDemployes_HR
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDEMPLOYES_hr
                                        ()));
}


void YerithCreerGroupeDemployesWindow::
		update_EMPLOYEE_GROUP_MEMBER_DATA(const QString &new_EMPLOYEE_GROUP_designation,
                                          const QString &new_EMPLOYEE_GROUP_db_ID,
                                          const QString &employee_db_ID)
{
//	QDEBUG_STRING_OUTPUT_2("new_EMPLOYEE_GROUP_db_ID", new_EMPLOYEE_GROUP_db_ID);

//	QDEBUG_STRING_OUTPUT_2("employee_db_ID", employee_db_ID);

    QString strQueryEmployeeGroupMemberDATA =
    		QString("select %1, %2 from %3 where %4='%5' and %6 = '1'")
				.arg(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                     YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                     YerithDatabase::FOURNISSEURS,
                     YerithDatabaseTableColumn::ID,
                     employee_db_ID,
					 YerithDatabaseTableColumn::EMPLOYE);


//    QDEBUG_STRING_OUTPUT_2("strQueryEmployeeGroupMemberDATA",
//    					   strQueryEmployeeGroupMemberDATA);


    QSqlQuery query;

    int querySize = YerithUtils::execQuery(query,
    									   strQueryEmployeeGroupMemberDATA,
										   _logger);

//    QDEBUG_STRING_OUTPUT_2_N("querySize", querySize);

    if (querySize <= 0)
    {
        return;
    }

    QString GROUPES_DUN_EMPLOYE;

    QString GROUPES_DUN_EMPLOYE_ID;

    if (query.next())
    {
        GROUPES_DUN_EMPLOYE = query.value(0).toString();
        GROUPES_DUN_EMPLOYE_ID = query.value(1).toString();
    }

    query.clear();

    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
		(new_EMPLOYEE_GROUP_designation, GROUPES_DUN_EMPLOYE);

    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
    	(new_EMPLOYEE_GROUP_db_ID, GROUPES_DUN_EMPLOYE_ID);

    QString strQuery_UPDATE_EmployeeGroupMemberDATA =
    			QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6 = '%7'")
					.arg(YerithDatabase::FOURNISSEURS,
                         YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                         GROUPES_DUN_EMPLOYE,
                         YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                         GROUPES_DUN_EMPLOYE_ID,
                         YerithDatabaseTableColumn::ID,
                         employee_db_ID);

//    QDEBUG_STRING_OUTPUT_2("strQuery_UPDATE_EmployeeGroupMemberDATA",
//    					   strQuery_UPDATE_EmployeeGroupMemberDATA);

    YerithUtils::execQuery(query,
    					   strQuery_UPDATE_EmployeeGroupMemberDATA,
                           _logger);
}


bool YerithCreerGroupeDemployesWindow::employeeGroupAlreadyExists()
{
    bool result = false;

    YerithSqlTableModel &groupes_DEMPLOYES_TableModel =
                    _allWindows->getSqlTableModel_groupes_demployes_hr();

    // ** check if customer account with same name exist
    result = YerithUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                         groupes_DEMPLOYES_TableModel,
                                                         *lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux,
                                                         QObject::tr("1 GROUPE D'EMPLOYÉS"),
                                                         YerithDatabaseTableColumn::NOM_ENTREPRISE);

    return result;
}


bool YerithCreerGroupeDemployesWindow::check_fields()
{
    bool result;

    bool nom_du_groupe_de_EMPLOYES_CREER =
                    lineEdit_creer_groupe_DEMPLOYES_designation->checkField();

    bool nombre_maximum_de_membres =
                    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->checkField();

    bool GROUPE_DE_PAIE_HR =
    		comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->checkField();


    result = nom_du_groupe_de_EMPLOYES_CREER 			&&
    		 nombre_maximum_de_membres					&&
             nombre_maximum_de_membres && GROUPE_DE_PAIE_HR;

    return result;
}


void YerithCreerGroupeDemployesWindow::clear_all_fields()
{
    lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux->clear();

    lineEdit_creer_groupe_DEMPLOYES_reference_groupe->clear();
    lineEdit_creer_groupe_DEMPLOYES_designation->clearField();
    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->clearField();
    comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->clearField();

    textEdit_creer_groupe_DEMPLOYES_description_groupe->clear();

    tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe
		->yerithClearTableWidgetContent();
}


void YerithCreerGroupeDemployesWindow::rendreInvisible()
{
    clear_all_fields();
    YerithWindowsCommons::rendreInvisible();
}


void YerithCreerGroupeDemployesWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    populateComboBoxes();

    setupLineEditsQCompleters();

    check_fields();

    lineEdit_creer_groupe_DEMPLOYES_designation->setFocus();

    setVisible(true);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("créer un groupe D'EMPLOYÉS"),
    								 _curStocksTableModel);
}


bool YerithCreerGroupeDemployesWindow::
		IS_client_present_dans_un_groupe_DEMPLOYES(const QString &anEmployeeGroupMemberName)
{
    QTableWidgetItem *anItem = 0;

    for (unsigned int k = 0;
         k < tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->rowCount();
         ++k)
    {
        anItem = tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->item(k, 1);

        if (0 != anItem)
        {
            if (YerithUtils::isEqualCaseInsensitive(anEmployeeGroupMemberName,
                                                    anItem->text()))
            {
                return true;
            }
        }
    }

    return false;
}


void YerithCreerGroupeDemployesWindow::
		ajouter_un_membre_au_groupe_DEMPLOYES(const QString &anEmployeeGroupMemberName)
{
    if (IS_client_present_dans_un_groupe_DEMPLOYES(anEmployeeGroupMemberName))
    {
        return;
    }

    int MAXIMUM_DE_MEMBRES_DU_GROUPE =
    		lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->text().toInt();

    if (tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->rowCount()
            == MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("NOMBRE MAXIMUM DE MEMBRES"),
                                       QObject::tr("NOMBRE MAXIMUM DE MEMBRES ('%1') INITIAUX ATTEINT !")
                                       	   .arg(QString::number(MAXIMUM_DE_MEMBRES_DU_GROUPE)));

        return;
    }

    YerithSqlTableModel &anEMPLOYEESqlTableModel = _allWindows->getSqlTableModel_fournisseurs();

    anEMPLOYEESqlTableModel.setFilter(QString("%1='%2'")
    									.arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
    										 anEmployeeGroupMemberName));


    QSqlRecord anEMPLOYEERecord = anEMPLOYEESqlTableModel.record(0);

    QString anEMPLOYEE_db_ID = GET_SQL_RECORD_DATA(anEMPLOYEERecord, YerithDatabaseTableColumn::ID);

    QString anEMPLOYEEReference = GET_SQL_RECORD_DATA(anEMPLOYEERecord,
                                           	   	   YerithDatabaseTableColumn::REFERENCE_FOURNISSEUR);


    tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe
		->addAClientGroupMember(anEMPLOYEE_db_ID,
								anEMPLOYEEReference,
                          	  	anEmployeeGroupMemberName);

    anEMPLOYEESqlTableModel.resetFilter();

    lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux->myClear();
}


void YerithCreerGroupeDemployesWindow::annuler_la_creation_dun_groupe_demployes()
{
    clear_all_fields();

    GROUPES_demployes_hr();
}


bool YerithCreerGroupeDemployesWindow::creerEnregistrerUnGroupeDEMPLOYES_hr()
{
    if (!check_fields())
    {
        if (QMessageBox::Ok ==
                YerithQMessageBox::warning(this,
                                           QObject::tr("groupe d'EMPLOYÉS (hr)"),
                                           QObject::tr("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }


    int NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES =
                    tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->rowCount();

    int MAXIMUM_DE_MEMBRES_DU_GROUPE =
                    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->text().toInt();

    if (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES > MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        uint difference = (uint) qFabs(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES
                                       - MAXIMUM_DE_MEMBRES_DU_GROUPE);

        YerithQMessageBox::information(this,
                                       QObject::tr("nombre maximum de membres"),
                                       QObject::tr("nombre maximum de membres: %1\n\n"
                                    		   	   "nombre de membres initiaux du groupe d'employés: %2\n\n"
                                    		   	   "VEUILLEZ CORRIGER LE NOMBRE MAXIMUM DE MEMBRES À '%3' ou "
                                    		   	   "DIMINUER SON NOMBRE DE MEMBRES INITIAUX DE ('%4 MEMBRES') !")
											.arg(QString::number(MAXIMUM_DE_MEMBRES_DU_GROUPE),
												 QString::number(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES),
												 QString::number(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES),
												 QString::number(difference)));

        return false;
    }


    if (employeeGroupAlreadyExists())
    {
        return false;
    }


    QString retMsg(QObject::tr("Le groupe D'EMPLOYÉS '"));

    YerithSqlTableModel &EMPLOYEE_GroupTableModel =
                    _allWindows->getSqlTableModel_groupes_demployes_hr();


    QSqlRecord record = EMPLOYEE_GroupTableModel.record();


    QString new_employee_group_designation =
                    lineEdit_creer_groupe_DEMPLOYES_designation->text();

    int new_EMPLOYEE_group_db_ID =
                    YerithERPWindows::getNextIdSqlTableModel_groupes_demployes_hr();


    record.setValue(YerithDatabaseTableColumn::ID, new_EMPLOYEE_group_db_ID);

    record.setValue(YerithDatabaseTableColumn::DATE_CREATION,
                    DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

    record.setValue(YerithDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr,
                    lineEdit_creer_groupe_DEMPLOYES_reference_groupe->text());

    record.setValue(YerithDatabaseTableColumn::DESIGNATION,
                    new_employee_group_designation);

    record.setValue(YerithDatabaseTableColumn::DESCRIPTION_GROUPE_DEMPLOYES_HR,
                    textEdit_creer_groupe_DEMPLOYES_description_groupe->toPlainText());

    record.setValue(YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->text().toInt());

    record.setValue(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                    comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->currentText());


    QMapIterator<int, QString> it_MapListIdxToElement_db_ID
								(tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe
										->get_mapListIdxToElement_db_ID());


    QString a_cur_EMPLOYEE_db_ID;

    QString membres_du_groupe_db_ID;

    while (it_MapListIdxToElement_db_ID.hasNext())
    {
        it_MapListIdxToElement_db_ID.next();

        a_cur_EMPLOYEE_db_ID = it_MapListIdxToElement_db_ID.value();

        update_EMPLOYEE_GROUP_MEMBER_DATA(new_employee_group_designation,
                                          QString::number(new_EMPLOYEE_group_db_ID),
										  a_cur_EMPLOYEE_db_ID);

        YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
			(a_cur_EMPLOYEE_db_ID,
			 membres_du_groupe_db_ID);
    }

    record.setValue(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                    membres_du_groupe_db_ID);


    retMsg.append(lineEdit_creer_groupe_DEMPLOYES_designation->text());

    bool success = EMPLOYEE_GroupTableModel
    					.insertNewRecord(record,
    									 this,
										 "src/windows/supplier/yerith-erp-creer-groupe-DEMPLOYES-hr-window.cpp",
										 616,
										 new_employee_group_designation);

    if (!success)
    {
        retMsg.append(QObject::tr("' n'a pas pu être créer !"));

        YerithQMessageBox::warning(this,
                                   QObject::tr("créer 1 groupe D'EMPLOYÉS"), retMsg);
        return false;
    }

    EMPLOYEE_GroupTableModel.select();

    retMsg.append(QObject::tr("' a été créer avec succès !"));

    YerithQMessageBox::information(this,
                                   QObject::tr("créer 1 groupe D'EMPLOYÉS"), retMsg);

    clear_all_fields();

    GROUPES_demployes_hr();

    return true;
}
