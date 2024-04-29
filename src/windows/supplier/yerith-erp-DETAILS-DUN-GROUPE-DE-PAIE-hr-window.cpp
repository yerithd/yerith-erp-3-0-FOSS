/*
 * yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerithDetailsDunGroupeDePaieWindow::YerithDetailsDunGroupeDePaieWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithDetailsDunGroupeDePaieWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("DÉTAILS D'UN GROUPE DE PAIE"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    QMESSAGE_BOX_STYLE_SHEET =
    		QString("QMessageBox {background-color: rgb(%1);}"
                    "QMessageBox QLabel {color: rgb(%2);}")
				.arg(COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YERITH_BLACK_0_0_0);


    doubleSpinBox_details_DUN_GROUPE_DE_PAIE_pourcentage_du_rabais->setMaximum(100.0);

    doubleSpinBox_details_DUN_GROUPE_DE_PAIE_pourcentage_du_rabais->setYerithEnabled(false);


    setupLineEdits();


    textEdit_details_DUN_GROUPE_DE_PAIE_description->setYerithEnabled(false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDemployes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, false);


    pushButton_groupes_de_paie->disable(this);
    pushButton_menu_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionGroupesDemployes, SIGNAL(triggered()), this, SLOT(GROUPES_DE_PAIE_hr()));
    connect(actionMenuFOURNISSEURS, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(GROUPES_DE_PAIE_hr()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif

    setupShortcuts();
}


void YerithDetailsDunGroupeDePaieWindow::setupLineEdits()
{
    lineEdit_details_DUN_GROUPE_DE_PAIE_reference->setYerithEnabled(false);
    lineEdit_details_DUN_GROUPE_DE_PAIE_designation->setYerithEnabled(false);
    lineEdit_details_DUN_GROUPE_DE_PAIE_ligne_budgetaire->setYerithEnabled(false);
    lineEdit_details_dun_GroupeDePaie_MONTANT_A_PAYER->setYerithEnabled(false);
}


void YerithDetailsDunGroupeDePaieWindow::definirPasDeRole()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDemployes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, false);

    pushButton_groupes_de_paie->disable(this);
    pushButton_menu_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
}


void YerithDetailsDunGroupeDePaieWindow::definirManager()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDemployes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, true);

    pushButton_groupes_de_paie->enable(this, SLOT(GROUPES_DE_PAIE_hr()));
    pushButton_menu_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_RETOUR->enable(this, SLOT(GROUPES_DE_PAIE_hr()));
}


void YerithDetailsDunGroupeDePaieWindow::rendreInvisible()
{
    comboBox_DETAILS_UN_GROUPE_DE_PAIE_localisation->clear();
    YerithWindowsCommons::rendreInvisible();
}


void YerithDetailsDunGroupeDePaieWindow::
		rendreVisible(YerithSqlTableModel *curClient_LOYALTY_PROGRAM_TableModel,
				      YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curPAY_GROUP_TableModel =
                    curClient_LOYALTY_PROGRAM_TableModel;

    //qDebug() << "++ last selected row: " << YerithERPWindows::get_last_lister_selected_row_db_ID();

    setVisible(true);

    showPayGroup_PROGRAM_DETAIL();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("DÉTAILS D'UN GROUPE DE PAIE"),
    								 _curPAY_GROUP_TableModel);
}


void YerithDetailsDunGroupeDePaieWindow::showPayGroup_PROGRAM_DETAIL()
{
    QSqlRecord record;


    QString designation_GROUPE_DE_PAIE;

    QString ligne_budgetaire_GROUPE_DE_PAIE;

    QString montant_a_payer;


    _allWindows->_GROUPES_DE_PAIE_Window
					->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);


    lineEdit_details_DUN_GROUPE_DE_PAIE_reference
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr));

    designation_GROUPE_DE_PAIE =
        GET_SQL_RECORD_DATA(record,
                            YerithDatabaseTableColumn::DESIGNATION);

    ligne_budgetaire_GROUPE_DE_PAIE  =
        GET_SQL_RECORD_DATA(record,
                            YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire);

    montant_a_payer =
        GET_SQL_RECORD_DATA(record,
                            YerithDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL);



    lineEdit_details_DUN_GROUPE_DE_PAIE_designation
		->setText(designation_GROUPE_DE_PAIE);

    lineEdit_details_DUN_GROUPE_DE_PAIE_ligne_budgetaire
        ->setText(ligne_budgetaire_GROUPE_DE_PAIE);

    lineEdit_details_dun_GroupeDePaie_MONTANT_A_PAYER
		->setText(GET_CURRENCY_STRING_NUM(montant_a_payer.toDouble()));



    comboBox_DETAILS_UN_GROUPE_DE_PAIE_localisation
		->addItem(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::LOCALISATION));


    doubleSpinBox_details_DUN_GROUPE_DE_PAIE_pourcentage_du_rabais
		->setValue(GET_SQL_RECORD_DATA(record,
									   YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble());


    textEdit_details_DUN_GROUPE_DE_PAIE_description
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS));

    tableWidget_details_dun_groupe_de_paie_groupes_demployes_participants
		->yerithClearTableWidgetContent();

    QString SELECT_ALL_employee_GROUP_PARTICIPATING
				(QString("select %1, %2, %3, %4 from %5 where %6='%7'")
					.arg(YerithDatabaseTableColumn::ID,
                         YerithDatabaseTableColumn::DESIGNATION,
                         YerithDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr,
                         YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                         YerithDatabase::GROUPES_DEMPLOYES_hr,
                         YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                         designation_GROUPE_DE_PAIE));


    QSqlQuery aQSqlQuery;

    int query_size = YerithUtils::execQuery(aQSqlQuery,
                                        	SELECT_ALL_employee_GROUP_PARTICIPATING);

    if (query_size <= 0)
    {
        return;
    }

    QString EMPLOYEE_group_db_ID;
    QString designation;
    QString reference_GROUPE_DEMPLOYES;
    QStringList EMPLOYEE_group_member_db_ID;

    while (aQSqlQuery.next())
    {
        EMPLOYEE_group_db_ID = aQSqlQuery.value(YerithDatabaseTableColumn::ID).toString();

        designation = aQSqlQuery.value(YerithDatabaseTableColumn::DESIGNATION).toString();

        reference_GROUPE_DEMPLOYES =
        		aQSqlQuery.value(YerithDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr).toString();

        EMPLOYEE_group_member_db_ID.clear();

        YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING
						(EMPLOYEE_group_member_db_ID,
                         aQSqlQuery.value(YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID).toString());

        tableWidget_details_dun_groupe_de_paie_groupes_demployes_participants
			->insert_group(EMPLOYEE_group_db_ID,
						   designation,
						   reference_GROUPE_DEMPLOYES,
						   QString::number(EMPLOYEE_group_member_db_ID.size()));
    }
}


void YerithDetailsDunGroupeDePaieWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
