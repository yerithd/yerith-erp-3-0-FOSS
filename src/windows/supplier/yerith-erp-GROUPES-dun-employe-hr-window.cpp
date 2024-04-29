/*
 * yerith-erp-GROUPES-dun-employe-hr-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-GROUPES-dun-employe-hr-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "yerith-erp-calcul-salaire-EMPLOYE-window.hpp"

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


YerithGROUPES_DUN_EMPLOYE_Window::YerithGROUPES_DUN_EMPLOYE_Window()
:YerithQTableWidgetWindowsCommons("yerith-erp-GROUPES-dun-employe-hr"),
 _logger(new YerithLogger("YerithGROUPES_DUN_EMPLOYE_Window")),
 _curEMPLOYEEgroups_TableModel(0),
 _somme_SALAIRE_CUMULE_A_DATE(0.0)
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("groupes d'1 employé"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
        << action_IMPRIMER_UNE_LISTE_de_groupe_demployes_JUSQUAU_selectionne;


    tableWidget_Groupes_Dun_Employe->setSortingEnabled(true);


    YerithQTableWidgetWindowsCommons::connect_Qtable_Widgets(*tableWidget_Groupes_Dun_Employe);



    _a_CURRENT_table_widget = tableWidget_Groupes_Dun_Employe;

    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_Groupes_Dun_Employe->font());



    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                            		   "QMessageBox QLabel {color: rgb(%2);}")
                            	 .arg(COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                            		  COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    _curEMPLOYEEgroups_TableModel = &_allWindows->getSqlTableModel_fournisseurs();


    dateEdit_groupe_dun_employe_date_begin->setStartDate(GET_CURRENT_DATE);

    dateEdit_groupe_dun_employe_date_end->setStartDate(GET_CURRENT_DATE);


    dateEdit_groupe_dun_employe_date_begin->setDate(GET_CURRENT_DATE);

    dateEdit_groupe_dun_employe_date_end->setDate(GET_CURRENT_DATE);


    dateEdit_groupe_dun_employe_date_begin->setYerithEnabled(false);

    dateEdit_groupe_dun_employe_date_end->setYerithEnabled(false);


    setupLineEdits();


    populateComboBoxes();


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_IMPRIMER_UNE_LISTE_de_groupe_demployes_JUSQUAU_selectionne, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_MANUAL_pay_for_selected_PAY_GROUP->disable(this);


    pushButton_VALIDER->disable(this);
    pushButton_paiements->disable(this);
    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);


    connect(action_parametrer_les_impressions,
            SIGNAL(triggered()),
            this,
            SLOT(setup_print()));


    connect(actionAfficherPDF,
            SIGNAL(triggered()),
            this,
            SLOT(imprimer_UNE_LISTE_WITH_A_YERITH_PROGRESS_BAR()));

    connect(action_IMPRIMER_UNE_LISTE_de_groupe_demployes_JUSQUAU_selectionne,
            SIGNAL(triggered()),
            this,
            SLOT(imprimer_UNE_LISTE_JUSQUAU_selectionne_WITH_A_YERITH_PROGRESS_BAR()));


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
            SIGNAL(triggered()),
            (YerithQTableWidgetWindowsCommons *) this,
            SLOT(incrementFontSize__OF_TABLE__WIDGET()));

    connect(actiondiminuer_la_police_du_tableau,
            SIGNAL(triggered()),
            (YerithQTableWidgetWindowsCommons *) this,
            SLOT(decrementFontSize__OF_TABLE__WIDGET()));


    connect(actionExporter_au_format_csv,
    		SIGNAL(triggered()),
			this,
			SLOT(export_csv_file()));


    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionListerDesGroupesDemployes, SIGNAL(triggered()), this, SLOT(GROUPES_demployes_hr()));
    connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAfficher_ce_groupe_DEMPLOYES_au_detail, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


    connect(checkBox_activer_DATES_DAPPARTENANCE,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(handle_activer_DATES_DAPPARTENANCE(int)));


    connect(this,
            SIGNAL(YRI_ROW_count_changed()),
            this,
            SLOT(handle_payer_manuellement_Button()));


    connect(tableWidget_Groupes_Dun_Employe,
            SIGNAL(addedYerithTableWidget()),
            this,
            SLOT(enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()));

    connect(tableWidget_Groupes_Dun_Employe,
            SIGNAL(clicked(const QModelIndex &)),
            this,
			SLOT(handle_DATE_DEBUT_et_DATE_FIN_dappartenance(const QModelIndex &)));

    connect(tableWidget_Groupes_Dun_Employe,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
			SLOT(afficher_au_detail(const QModelIndex &)));


    connect(comboBox_Groupes_Dun_Employe_recherche,
            SIGNAL(activated(const QString &)),
            this,
			SLOT(ajouter_appartenance(const QString &)));


    setupShortcuts();

    comboBox_Groupes_Dun_Employe_recherche->setYerithEditable(true);

    comboBox_Groupes_Dun_Employe_recherche->setFocus();


    emit YRI_ROW_count_changed();


    handle_activer_DATES_DAPPARTENANCE(Qt::Unchecked);
}


bool YerithGROUPES_DUN_EMPLOYE_Window::export_csv_file()
{
    bool success = false;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success =
            YerithUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_Groupes_Dun_Employe,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "Employee group export listing");
    }
    else
    {
        success =
            YerithUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_Groupes_Dun_Employe,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "FICHE D'EXPORTATION DES groupes d'1 employé");
    }

    return success;
}


bool YerithGROUPES_DUN_EMPLOYE_Window::imprimer_pdf_document()
{
//	QDEBUG_STRING_OUTPUT_1("YerithGROUPES_DUN_EMPLOYE_Window::imprimer_pdf_document");

	if (tableWidget_Groupes_Dun_Employe->rowCount() <= 0)
	{
		YerithQMessageBox::information(this,
				QObject::tr("impression"),
				QObject::tr("Il n'y a pas de données à imprimer !"));

		return false;
	}

	QString GROUPES_DUN_EMPLOYE__tex_table;

	//int pageNumber = qCeil(tableWidget_Groupes_Dun_Employe->rowCount() / 20);


	YerithPOSUser *yerithUser = _allWindows->getUser();

	YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

	QString GROUPES_DUN_EMPLOYE__TexDocument;

	QString factureDate(infoEntreprise.getVille_LATEX());

	YerithUtils::getCurrentSimplifiedDate(factureDate);

	get_PRINT_OUT_TexTableString(GROUPES_DUN_EMPLOYE__tex_table);

	emit SIGNAL_INCREMENT_PROGRESS_BAR(18);

	YerithUtils::get_GROUPES_DUN_EMPLOYE_TexDocumentString(GROUPES_DUN_EMPLOYE__TexDocument,
														   GROUPES_DUN_EMPLOYE__tex_table);

	emit SIGNAL_INCREMENT_PROGRESS_BAR(50);

	QString NOM_EMPLOYE(_curEMPLOYEE_NOM_ENTREPRISE);

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		GROUPES_DUN_EMPLOYE__TexDocument
		.replace("YERITHSUBJECT",
				QString("Employee '%1' belonging GROUPS")
				.arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_EMPLOYE)));
	}
	else
	{
		GROUPES_DUN_EMPLOYE__TexDocument
		.replace("YERITHSUBJECT",
				QString("Groupes d'appartenance d'1 Employé ('%1')")
				.arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_EMPLOYE)));
	}

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHPAPERSPEC",
											 get_PRINTING_PARAMETER_printing_position());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHBOITEPOSTALE", infoEntreprise.getBoitePostal());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHDATE", factureDate);

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHHEUREVENTE", CURRENT_TIME);

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());


	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHNOMUTILISATEUR",
			QString("%1 %2")
			.arg(YerithUtils::getAllWindows()->getUser()->titre(),
					YerithUtils::getAllWindows()->getUser()->nom_completTex()));

	GROUPES_DUN_EMPLOYE__TexDocument
	.replace("YERITHSUCCURSALE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents
			(YerithERPConfig::THIS_SITE_LOCALISATION_NAME));

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


	YerithSqlTableModel &employee_TableModel = _allWindows->getSqlTableModel_fournisseurs();

	employee_TableModel.yerithSetFilter_WITH_where_clause(
			QString("%1 = '%2' AND %3 != 0")
			.arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                 NOM_EMPLOYE,
                 YerithDatabaseTableColumn::EMPLOYE));

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YERITHEMPLOYE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(NOM_EMPLOYE));

	if (employee_TableModel.easySelect("src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp", 359) > 0)
	{
		QSqlRecord record = employee_TableModel.record(0);

		QString employee_Ville(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::VILLE));

		QString employee_POBox(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::BOITE_POSTALE));

		QString employee_Email(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::EMAIL));

		QString employee_Tel(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

		if (employee_Tel.isEmpty())
		{
			employee_Tel.append(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
		}

		//qDebug() << "++ employee_Email: " << employee_Email;

		if (!employee_Ville.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument
			.replace("EMPLOYEYERITHCITY",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(employee_Ville));
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYERITHCITY", "");
		}

		if (!employee_POBox.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYERITHPOBOX", employee_POBox);
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYERITHPOBOX", "N/a");
		}

		if (!employee_Email.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYERITHMAIL",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(employee_Email));
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYERITHMAIL", "");
		}

		if (!employee_Tel.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument
			.replace("EMPLOYEYERITHPHONE",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(employee_Tel));
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYERITHPHONE", "");
		}

		employee_TableModel.resetFilter();
	}


	QString prefixFileName;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir("yerith-erp-employee-Group-hr"));
	}
	else
	{
		prefixFileName.append(
				YerithUtils::getUniquePrefixFileInTemporaryFilesDir("yerith-erp-GROUPES-DUN-Employe-HR"));
	}

	//qDebug() << "++\n" << GROUPES_DUN_EMPLOYE__TexDocument;

	QFile tmpLatexFile(prefixFileName + "tex");

	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(GROUPES_DUN_EMPLOYE__TexDocument.toUtf8());
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


void YerithGROUPES_DUN_EMPLOYE_Window::
        handle_DATE_DEBUT_et_DATE_FIN_dappartenance(int row)
{
    QTableWidgetItem *item_designation =
        tableWidget_Groupes_Dun_Employe->item(row,
                                              0);

    QTableWidgetItem *item_GROUPE_DE_PAIE_HR =
        tableWidget_Groupes_Dun_Employe->item(row,
                                              1);

    if (0 != item_designation &&
        0 != item_GROUPE_DE_PAIE_HR)
    {
        QString designation_GROUPE_DEMPLOYES_HR = item_designation->text();


        //QDEBUG_STRING_OUTPUT_2("_curEMPLOYEE_NOM_ENTREPRISE", _curEMPLOYEE_NOM_ENTREPRISE);

        //QDEBUG_STRING_OUTPUT_2("designation_GROUPE_DEMPLOYES_HR", designation_GROUPE_DEMPLOYES_HR);

        //QDEBUG_STRING_OUTPUT_2("_curEMPLOYEE_groupe_de_paie_hr", _curEMPLOYEE_groupe_de_paie_hr);

        _curEMPLOYEE_groupe_de_paie_hr = item_GROUPE_DE_PAIE_HR->text();


        lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
            ->setText(GET_CURRENCY_STRING_NUM
                        (_map_GROUPE_SELECTIONNE__to__salaire
                            .value(_curEMPLOYEE_groupe_de_paie_hr)));


        if (!_curEMPLOYEE_NOM_ENTREPRISE.isEmpty() &&
            !_curEMPLOYEE_groupe_de_paie_hr.isEmpty())
        {
            YerithSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_TableModel
                = _allWindows->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


            QString CONDITION = QString("(%1) AND (%2)")
                                    .arg(GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                                              _curEMPLOYEE_NOM_ENTREPRISE),
                                         GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                                              _curEMPLOYEE_groupe_de_paie_hr));


            periodes_dappartenance_groupes_de_paie_hr_TableModel
                .yerithSetFilter_WITH_where_clause(CONDITION);


            int query_size = periodes_dappartenance_groupes_de_paie_hr_TableModel
                                .easySelect("src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp", 491);


            //QDEBUG_STRING_OUTPUT_2_N("query_size", query_size);


            if (0 < query_size)
            {
                QSqlRecord a_record =
                    periodes_dappartenance_groupes_de_paie_hr_TableModel.record(0);

                QString dateEdit_groupe_dun_employe_date_begin_STRING =
                    GET_SQL_RECORD_DATA(a_record,
                                        YerithDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE);

                QString dateEdit_groupe_dun_employe_date_end_STRING =
                    GET_SQL_RECORD_DATA(a_record,
                                        YerithDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE);


                dateEdit_groupe_dun_employe_date_begin
                    ->setDate(GET_DATE_FROM_STRING(dateEdit_groupe_dun_employe_date_begin_STRING));

                dateEdit_groupe_dun_employe_date_end
                    ->setDate(GET_DATE_FROM_STRING(dateEdit_groupe_dun_employe_date_end_STRING));
            }
            else
            {
                dateEdit_groupe_dun_employe_date_begin->reset();
                dateEdit_groupe_dun_employe_date_end->reset();
            }

            periodes_dappartenance_groupes_de_paie_hr_TableModel
                .resetFilter("src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp", 524);
        }
    }
    else
    {
        lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));
    }

}


bool YerithGROUPES_DUN_EMPLOYE_Window::imprimer_UNE_LISTE(bool *full_list /*= 0 */)
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


	YerithUtils::get_GROUPES_DUN_EMPLOYE_TexDocumentString
                                (a_yerith_qtable_widget__TexDocument_In_Out,
                                 a_yerith_qtable_widget__tex_table);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(23);


	QString NomEmploye(_curEMPLOYEE_NOM_ENTREPRISE);


    YerithSqlTableModel &supplierTableModel = _allWindows->getSqlTableModel_fournisseurs();

    supplierTableModel.yerithSetFilter_WITH_where_clause(
        QString("%1 = '%2' && %3 = '1'")
        .arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
             NomEmploye,
             YerithDatabaseTableColumn::EMPLOYE));

    a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHEMPLOYE",
            YerithUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye));

	if (supplierTableModel.easySelect("src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp", 398) > 0)
	{
		QSqlRecord record = supplierTableModel.record(0);

		QString EMPLOYE_Ville(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::VILLE));

		QString EMPLOYE_POBox(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::BOITE_POSTALE));

		QString EMPLOYE_Email(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::EMAIL));

		QString EMPLOYE_Tel(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

		if (EMPLOYE_Tel.isEmpty())
		{
			EMPLOYE_Tel.append(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
		}

		//qDebug() << "++ EMPLOYE_Email: " << EMPLOYE_Email;

		if (!EMPLOYE_Ville.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out
			.replace("EMPLOYEYERITHCITY",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(EMPLOYE_Ville));
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("EMPLOYEYERITHCITY", "");
		}

		if (!EMPLOYE_POBox.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("EMPLOYEYERITHPOBOX", EMPLOYE_POBox);
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("EMPLOYEYERITHPOBOX", "N/a");
		}

		if (!EMPLOYE_Email.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("EMPLOYEYERITHMAIL",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(EMPLOYE_Email));
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("EMPLOYEYERITHMAIL", "");
		}

		if (!EMPLOYE_Tel.isEmpty())
		{
			a_yerith_qtable_widget__TexDocument_In_Out
			.replace("EMPLOYEYERITHPHONE",
					YerithUtils::LATEX_IN_OUT_handleForeignAccents(EMPLOYE_Tel));
		}
		else
		{
			a_yerith_qtable_widget__TexDocument_In_Out.replace("EMPLOYEYERITHPHONE", "");
		}

		supplierTableModel.resetFilter();
	}



	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


    QString file_name_Subject_Title_FRENCH =
        (*full_list) ? QString("Groupes d'appartenance de l'EMPLOYÉ '%1'")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye))
                     : QString("Groupes d'appartenance de l'EMPLOYÉ '%1' (PARTIELLE)")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye));


    QString file_name_Subject_Title_ENGLISH =
        (*full_list) ? QString("EMPLOYEE '%1' belonging GROUPS")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye))
                     : QString("EMPLOYEE '%1' belonging GROUPS (EXCERPT)")
                        .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye));


    QString prefix_file_name_FRENCH =
        (*full_list) ? "yerith-erp-groupes-dappartenances-employe"
                     : "yerith-erp-groupes-dappartenances-employe-PARTIELLE";


    QString prefix_file_name_ENGLISH =
        (*full_list) ? "yerith-erp-BELONGING-GROUPS-employee"
                     : "yerith-erp-BELONGING-GROUPS-employee-PARTIAL";


	return
        YerithQTableWidgetWindowsCommons::
                imprimer_UNE_LISTE(*tableWidget_Groupes_Dun_Employe,
                                   a_yerith_qtable_widget__TexDocument_In_Out,
                                   file_name_Subject_Title_FRENCH,
                                   file_name_Subject_Title_ENGLISH,
                                   prefix_file_name_FRENCH,
                                   prefix_file_name_ENGLISH);
}


void YerithGROUPES_DUN_EMPLOYE_Window::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_menuGROUPES_DE_PAIE_DUN_EMPLOYE_Window->palette());
    menu.addAction(action_IMPRIMER_UNE_LISTE_de_groupe_demployes_JUSQUAU_selectionne);
    menu.addAction(actionAfficher_ce_groupe_DEMPLOYES_au_detail);
    menu.exec(event->globalPos());
}


void YerithGROUPES_DUN_EMPLOYE_Window::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


//bool YerithGROUPES_DUN_EMPLOYE_Window::imprimer_UNE_LISTE()
//{
//	QString a_yerith_qtable_widget__tex_table;
//
//	QString a_yerith_qtable_widget__TexDocument_In_Out;
//
//
//	get_PRINT_OUT_TexTableString(a_yerith_qtable_widget__tex_table);
//
//
//	emit SIGNAL_INCREMENT_PROGRESS_BAR(11);
//
//
//	YerithUtils::get_TYPEDOPERATION_UNELIGNEBUDGETAIRE_TexDocumentString
//                                (a_yerith_qtable_widget__TexDocument_In_Out,
//                                 a_yerith_qtable_widget__tex_table);
//
//
//	emit SIGNAL_INCREMENT_PROGRESS_BAR(23);
//
//
//    QString file_name_Subject_Title_FRENCH = "GROUPES D'APPARTENANCE d'1 employé";
//
//    QString file_name_Subject_Title_ENGLISH = "BELONGING GROUPS of an employee";
//
//    QString prefix_file_name_FRENCH = "yerith-erp-groupes-dappartenances-employe";
//
//    QString prefix_file_name_ENGLISH = "yerith-erp-BELONGING-GROUPS-employee";
//
//
//	return
//        YerithQTableWidgetWindowsCommons::
//                imprimer_UNE_LISTE(*tableWidget_Groupes_Dun_Employe,
//                                   a_yerith_qtable_widget__TexDocument_In_Out,
//                                   file_name_Subject_Title_FRENCH,
//                                   file_name_Subject_Title_ENGLISH,
//                                   prefix_file_name_FRENCH,
//                                   prefix_file_name_ENGLISH);
//}


bool YerithGROUPES_DUN_EMPLOYE_Window::CREATE_A_NEW_PERIODE_DE_TEMPS_APPARTENANCE()
{
    bool successBeginEndDataSaved = false;


    QDate begin_DATE = dateEdit_groupe_dun_employe_date_begin->date();

    QDate end_DATE = dateEdit_groupe_dun_employe_date_end->date();


    YerithSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_SqlTableModel =
        _allWindows->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.resetFilter();


    QSqlRecord periodes_dappartenance_Record =
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.record();


    periodes_dappartenance_Record
        .setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                  _curEMPLOYEE_NOM_ENTREPRISE);

    periodes_dappartenance_Record
        .setValue(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                  _curEMPLOYEE_groupe_de_paie_hr);

    periodes_dappartenance_Record
        .setValue(YerithDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE,
                  begin_DATE);

    periodes_dappartenance_Record
        .setValue(YerithDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE,
                  end_DATE);


    int IDforPERIODESdappartenance =
        YerithERPWindows::getNextIdSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    periodes_dappartenance_Record.setValue(YerithDatabaseTableColumn::ID,
                                           IDforPERIODESdappartenance);


    successBeginEndDataSaved =
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
            .insertNewRecord(periodes_dappartenance_Record,
                             0,
                             "src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp",
                             602);



    return successBeginEndDataSaved;
}


void YerithGROUPES_DUN_EMPLOYE_Window::handle_activer_DATES_DAPPARTENANCE(int state)
{
    if (checkBox_activer_DATES_DAPPARTENANCE->isChecked())
    {
        dateEdit_groupe_dun_employe_date_begin->setYerithEnabled(true);

        dateEdit_groupe_dun_employe_date_end->setYerithEnabled(true);

        pushButton_MANUAL_pay_for_selected_PAY_GROUP->setVisible(false);
    }
    else
    {
        dateEdit_groupe_dun_employe_date_begin->setYerithEnabled(false);

        dateEdit_groupe_dun_employe_date_end->setYerithEnabled(false);

        pushButton_MANUAL_pay_for_selected_PAY_GROUP->setVisible(true);
    }
}


bool YerithGROUPES_DUN_EMPLOYE_Window::EXECUTER_handle_MANUAL_pay_for_selected_PAY_GROUP_button()
{
    QString msgPayerManuellement =
        QObject::tr("Exécuter 1 paye manuelle pour l'employé(e) : (%1) du groupe d'employés \"%2\" ?")
            .arg(_curEMPLOYEE_NOM_ENTREPRISE,
                 _curEMPLOYEE_groupe_de_paie_hr);

    if (QMessageBox::Cancel ==
            YerithQMessageBox::question(this,
                                        QObject::tr("Payer (manuellement) 1 employé(e) d'1 groupe d'employés"),
                                        msgPayerManuellement,
                                        QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        return false;
    }


    bool success_Manual_EMPLOYEE_payment_for_selected_PAY_GROUP = false;


    YerithERPCalculSalaireEMPLOYE manual_pay_for_EMPLOYEE_group;


    if (_map_GROUPE_SELECTIONNE__to__salaire.contains(_curEMPLOYEE_groupe_de_paie_hr))
    {
        double current_pay_group_salary =
                    _map_GROUPE_SELECTIONNE__to__salaire
                        .value(_curEMPLOYEE_groupe_de_paie_hr);

        YerithERPPaymentProcessingInformation payment_processing_info_IN_Out;


        double New_Calculated_current_pay_group_salary =
                manual_pay_for_EMPLOYEE_group
                    .calculate_PAY_GROUP_MONEY_BENEFITS(_curEMPLOYEE_NOM_ENTREPRISE,
                                                        _curEMPLOYEE_groupe_de_paie_hr);


        if (New_Calculated_current_pay_group_salary > 0                      &&
            current_pay_group_salary == New_Calculated_current_pay_group_salary)
        {
            payment_processing_info_IN_Out._nom_entreprise = _curEMPLOYEE_NOM_ENTREPRISE;


            QString EMPLOYEE_group_PAY_GR0UP_LigneBUDGETAIRE =
                        YerithUtils::Get_INTITULE_DE_LA_LIGNE_BUDGETAIRE_FOR_GROUPE_DE_PAIE_Hr
                            (_curEMPLOYEE_groupe_de_paie_hr);

            payment_processing_info_IN_Out._paiement_INTITULE_DE_LA_LIGNE_BUDGETAIRE =
                EMPLOYEE_group_PAY_GR0UP_LigneBUDGETAIRE;



            payment_processing_info_IN_Out._paiement_intitule_compte_bancaire =
                YerithUtils::Get_INTITULE_COMPTE_BANCAIRE_FOR_intitule_de_la_ligne_budgetaire
                                (EMPLOYEE_group_PAY_GR0UP_LigneBUDGETAIRE);



            payment_processing_info_IN_Out._reference =
                QString("%1-SALAIRE-%2")
                    .arg(DATE_TO_STRING(GET_CURRENT_DATE),
                         payment_processing_info_IN_Out._nom_entreprise);


            payment_processing_info_IN_Out._designation =
                payment_processing_info_IN_Out._reference;


            payment_processing_info_IN_Out._type_de_paiement =
                YerithUtils::DECAISSEMENT_COMPTANT;


            payment_processing_info_IN_Out._NOTES =
                QObject::tr("PAYEMENT DU SALAIRE pour l'employé(e) '%1' \n\n"
                            "Groupe salarial: %2.")
                    .arg(_curEMPLOYEE_NOM_ENTREPRISE,
                         _curEMPLOYEE_groupe_de_paie_hr);


            payment_processing_info_IN_Out._montant_paye =
                New_Calculated_current_pay_group_salary;



            success_Manual_EMPLOYEE_payment_for_selected_PAY_GROUP =
                manual_pay_for_EMPLOYEE_group
                    .INSERT_employee_Paiement_AS_PAYMENT_ENTRY_DB_TABLE
                        (payment_processing_info_IN_Out);
        }
    }


    if (success_Manual_EMPLOYEE_payment_for_selected_PAY_GROUP)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("PAIEMENT MANUEL affecté Pour Le Groupe Sélectionné"),
                                       QObject::tr("PAIEMENT MANUEL affecté Pour "
                                               "Le Groupe Sélectionné '%1' effectué avec SUCCÈS !")
                                        .arg(_curEMPLOYEE_groupe_de_paie_hr));
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("PAIEMENT MANUEL affecté Pour Le Groupe Sélectionné"),
                                   QObject::tr("échec du PAIEMENT MANUEL affecté Pour "
                                               "Le Groupe Sélectionné '%1' !")
                                    .arg(_curEMPLOYEE_groupe_de_paie_hr));
    }


    return success_Manual_EMPLOYEE_payment_for_selected_PAY_GROUP;
}


void YerithGROUPES_DUN_EMPLOYE_Window::handle_VALIDER_button()
{
    if (!dateEdit_groupe_dun_employe_date_begin->isEnabled() ||
        !dateEdit_groupe_dun_employe_date_end->isEnabled())
    {
        return ;
    }


    QDate begin_DATE = dateEdit_groupe_dun_employe_date_begin->date();

    QDate end_DATE = dateEdit_groupe_dun_employe_date_end->date();


    if (end_DATE < begin_DATE)
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("date de fin AVANT date de début !"),
                                   QObject::tr("La date de fin [\"%1\"] d'appartenance au "
                                               "groupe de paie (HR) '%2' DEVRAIT ÊTRE avant "
                                               "la date de début [\"%3\"] !")
                                       .arg(DATE_TO_STRING(end_DATE),
                                            _curEMPLOYEE_groupe_de_paie_hr,
                                            DATE_TO_STRING(begin_DATE)));
        return ;
    }

    if (begin_DATE < GET_CURRENT_DATE)
    {

        YerithQMessageBox::warning(this,
                                   QObject::tr("date de début PRÉ-DATÉE !"),
                                   QObject::tr("La date de début [\"%1\"] d'appartenance au "
                                               "groupe de paie (HR) '%2' DEVRAIT ÊTRE daté "
                                               "au minimum à partir de ce jour [\"%3\"] !")
                                       .arg(DATE_TO_STRING(begin_DATE),
                                            _curEMPLOYEE_groupe_de_paie_hr,
                                            DATE_TO_STRING(GET_CURRENT_DATE)));

        YerithQMessageBox::information(this,
                                       QObject::tr("NOUVELLES DATES D'APPARTENACE"),
                                       QObject::tr("Pour faire de nouvelles dates d'appartenance au "
                                                   "groupe de paie (HR) '%1' pour cet employé [\"%2\"], "
                                                   "VOUS DEVEZ LE RETIRER DE CE GROUPE, et ensuite l'y remettre !")
                                         .arg(_curEMPLOYEE_groupe_de_paie_hr,
                                              _curEMPLOYEE_NOM_ENTREPRISE));
        return ;
    }


    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    YerithSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_SqlTableModel =
        _allWindows->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    QString condition_exist_already =
        QString("(%1) AND (%2)")
            .arg(GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                      _curEMPLOYEE_NOM_ENTREPRISE),
                 GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                      _curEMPLOYEE_groupe_de_paie_hr));


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .yerithSetFilter_WITH_where_clause(condition_exist_already);


    int query_size = periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
                        .easySelect("src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp", 701);


    //QDEBUG_STRING_OUTPUT_2_N("query_size", query_size);
    bool successBeginEndDataSaved = false;


    if (0 < query_size)
    {
        QSqlRecord a_record =
            periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.record(0);


        QSqlRecord a_periodes_dappartenance_UpdateRecord(a_record);


        a_periodes_dappartenance_UpdateRecord
            .setValue(YerithDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE,
                      begin_DATE);


        a_periodes_dappartenance_UpdateRecord
            .setValue(YerithDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE,
                      end_DATE);


        successBeginEndDataSaved =
            periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
                .updateRecord(0,
                              a_periodes_dappartenance_UpdateRecord,
                              "src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp",
                              728,
                              _curEMPLOYEE_NOM_ENTREPRISE);
    }
    else
    {
        successBeginEndDataSaved =
            CREATE_A_NEW_PERIODE_DE_TEMPS_APPARTENANCE();
    }


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .resetFilter("src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp", 743);


    if (successBeginEndDataSaved)
    {
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        YerithQMessageBox::information(this,
                                       QObject::tr("ajouter périodes d'appartenance"),
                                       QObject::tr("LES DATES D'APPARTENANCE AU GROUPE DE PAIE (HR) '%1' "
                                                   "POUR L'EMPLOYÉ [%2] ont été enregistrées avec succès !")
                                         .arg(_curEMPLOYEE_groupe_de_paie_hr,
                                              _curEMPLOYEE_NOM_ENTREPRISE));

    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("ajouter périodes d'appartenance"),
                                       QObject::tr("LES DATES D'APPARTENANCE AU GROUPE DE PAIE (HR) '%1' "
                                                   "POUR L'EMPLOYÉ [%2] N'ONT PAS PU ÊTRE ENREGISTRÉES !")
                                         .arg(_curEMPLOYEE_groupe_de_paie_hr,
                                              _curEMPLOYEE_NOM_ENTREPRISE));

        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;
    }

    checkBox_activer_DATES_DAPPARTENANCE->setCheckState(Qt::Unchecked);

    afficher_tous_les_groupes_DUN_EMPLOYE_hr();
}


void YerithGROUPES_DUN_EMPLOYE_Window::ajouter_appartenance(const QString &un_groupe_DEMPLOYES_hr)
{
    YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION
    (this,
     &un_groupe_DEMPLOYES_hr,
     &YerithGROUPES_DUN_EMPLOYE_Window::executer_ajouter_appartenance);

    comboBox_Groupes_Dun_Employe_recherche->resetYerithComboBox();
}


void YerithGROUPES_DUN_EMPLOYE_Window::
	executer_ajouter_appartenance(const QString *un_groupe_DEMPLOYES_hr)
{
    if (0 == un_groupe_DEMPLOYES_hr)
    {
        return;
    }

    QString SELECT_GROUP_FROM_DB_TABLE
		(QString("select %1, %2, %3, %4 from %5 where %6='%7'")
			.arg(YerithDatabaseTableColumn::ID,
                 YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                 YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                 YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                 YerithDatabase::GROUPES_DEMPLOYES_hr,
                 YerithDatabaseTableColumn::DESIGNATION,
                 *un_groupe_DEMPLOYES_hr));

    QSqlQuery aQSqlQuery;

    int query_size = YerithUtils::execQuery(aQSqlQuery, SELECT_GROUP_FROM_DB_TABLE);

    if (query_size <= 0)
    {
        return;
    }

    QSqlRecord AN_EMPLOYEE_GroupRecordInfo;

    if (!aQSqlQuery.next())
    {
        return;
    }

    AN_EMPLOYEE_GroupRecordInfo = aQSqlQuery.record();

    int maximum_de_membres =
        GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                            YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES).toInt();

    QString membres_du_groupe_db_ID =
                    GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                        YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

    QStringList membres_du_groupe_db_ID_LIST;

    YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
                                                membres_du_groupe_db_ID);

    if (membres_du_groupe_db_ID_LIST.size() >= maximum_de_membres)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("ajouter"),
                                       QObject::tr("LE GROUPE D'EMPLOYÉS '%1' a déjà atteint son nombre maximum de membres (%2) !")
                                         .arg(*un_groupe_DEMPLOYES_hr,
                                              QString::number(maximum_de_membres)));

        return;
    }


    if (YerithUtils::
    		APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(_curEmployeeDBID, membres_du_groupe_db_ID))
    {

        QString INSERT_UPDATE_employee_WITHIN_GROUP
			(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
				.arg(YerithDatabase::GROUPES_DEMPLOYES_hr,
					 YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                     membres_du_groupe_db_ID,
                     YerithDatabaseTableColumn::DESIGNATION,
                     *un_groupe_DEMPLOYES_hr));

        bool insert_update_success =
        		YerithUtils::execQuery(INSERT_UPDATE_employee_WITHIN_GROUP);


        if (!insert_update_success)
        {
            YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                           QObject::tr
                                           ("L'EMPLOYÉ '%1' n'a pas pu être ajouté "
                                            "dans le groupe des employés '%2' !").
                                           arg(_curEMPLOYEE_NOM_ENTREPRISE,
                                               *un_groupe_DEMPLOYES_hr));

            return;
        }
    }

    QSqlRecord record;

    _allWindows->_fournisseursWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString groupes_dun_employe =
    		GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE);

    bool update_DB_GROUPES_DUN_EMPLOYE_TABLE =
                    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (*un_groupe_DEMPLOYES_hr,
                     groupes_dun_employe);

    QString groupes_dun_employe_id =
    		GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID);

    QString un_groupe_DEMPLOYES_db_ID =
                    GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                        YerithDatabaseTableColumn::ID);


    bool update_groupes_dun_employe_id =
                    YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (un_groupe_DEMPLOYES_db_ID,
                     groupes_dun_employe_id);

    update_DB_GROUPES_DUN_EMPLOYE_TABLE =
                    update_DB_GROUPES_DUN_EMPLOYE_TABLE || update_groupes_dun_employe_id;


    if (!update_DB_GROUPES_DUN_EMPLOYE_TABLE)
    {
        YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr("L'EMPLOYÉ '%1' EST DÉJÀ MEMBRE DU GROUPE D'EMPLOYÉS '%2' !")
									   	   .arg(_curEMPLOYEE_NOM_ENTREPRISE,
									   			*un_groupe_DEMPLOYES_hr));
        return;
    }


    QString UPDATE_employee_GROUP_INFORMATION
		(QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
				.arg(YerithDatabase::FOURNISSEURS,
                     YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                     groupes_dun_employe,
                     YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                     groupes_dun_employe_id,
                     YerithDatabaseTableColumn::ID,
                     _curEmployeeDBID));


    bool UPDATE_EMPLOYEE_GROUP_SUCCESS =
                    YerithUtils::execQuery(UPDATE_employee_GROUP_INFORMATION);

    if (!UPDATE_EMPLOYEE_GROUP_SUCCESS)
    {
        YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr("L'employé '%1' n'a pas pu être ajouté "
                                    		   	   	   "dans le GROUPE D'EMPLOYÉS '%2' !")
												.arg(_curEMPLOYEE_NOM_ENTREPRISE,
													 *un_groupe_DEMPLOYES_hr));

        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

        return;
    }

    int last_row_INSERTED =
        tableWidget_Groupes_Dun_Employe
            ->insert_group(GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo, YerithDatabaseTableColumn::ID),
                           *un_groupe_DEMPLOYES_hr,
                           GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                               YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR),
                            QString::number(maximum_de_membres));

    handle_VALIDER_button();


    emit YRI_ROW_count_changed();


    lineEdit_Groupes_Dun_Employe_nombre_de_groupes
		->setText(QString::number(tableWidget_Groupes_Dun_Employe->rowCount()));


    YerithQMessageBox::information(this, QObject::tr("ajouter"),
                                   QObject::tr("L'EMPLOYÉ '%1' a été ajouté dans le groupe des employés '%2' !")
								   	   .arg(_curEMPLOYEE_NOM_ENTREPRISE,
								   			*un_groupe_DEMPLOYES_hr));
}


bool YerithGROUPES_DUN_EMPLOYE_Window::EXECUTER_retirer_cet_employe_du_groupe_selectionne()
{
    QString SELECT_GROUPES_DUN_EMPLOYE_INFO(
    			QString("select %1, %2 from %3 where %4='%5'")
					.arg(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                         YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                         YerithDatabase::FOURNISSEURS,
                         YerithDatabaseTableColumn::ID,
                         _curEmployeeDBID));

    QSqlQuery aQSqlQuery;

    int query_size = YerithUtils::execQuery(aQSqlQuery, SELECT_GROUPES_DUN_EMPLOYE_INFO);

    if (query_size <= 0)
    {
        return false;
    }

    QSqlRecord ANEMPLOYEERecordGroupInfo;

    if (!aQSqlQuery.next())
    {
        return false;
    }

    ANEMPLOYEERecordGroupInfo = aQSqlQuery.record();

    QString groupes_dun_EMPLOYE_ID =
    		GET_SQL_RECORD_DATA(ANEMPLOYEERecordGroupInfo,
                                YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID);

    QString groupes_dun_EMPLOYE =
    		GET_SQL_RECORD_DATA(ANEMPLOYEERecordGroupInfo,
                                YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE);


    QString msgRETIRER =
        QObject::tr("Retirer l'employé : (%1) du groupe d'employés \"%2\" ?")
            .arg(_curEMPLOYEE_NOM_ENTREPRISE,
                 _curEMPLOYEE_groupe_de_paie_hr);


    if (QMessageBox::Cancel ==
            YerithQMessageBox::question(this,
                                        QObject::tr("Retirer l'employé d'1 groupe d'employés"),
                                        msgRETIRER,
                                        QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        return false;
    }


    int currentRow = tableWidget_Groupes_Dun_Employe->currentRow();


    QString EMPLOYEE_Group_db_ID = tableWidget_Groupes_Dun_Employe->get_DB_ELEMENT_db_ID(currentRow);


    YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(groupes_dun_EMPLOYE_ID,
    															   EMPLOYEE_Group_db_ID);


    QTableWidgetItem *item = tableWidget_Groupes_Dun_Employe->item(currentRow, 0);

    if (0 == item)
    {
        return false;
    }

    QString employeeGroup_designation = item->text().trimmed();

    YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(groupes_dun_EMPLOYE,
    														       employeeGroup_designation);

    bool result = true;

    {
        QString UPDATE_GROUPES_DUN_EMPLOYE_INFO =
        			QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
						.arg(YerithDatabase::FOURNISSEURS,
                             YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                             groupes_dun_EMPLOYE,
                             YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                             groupes_dun_EMPLOYE_ID,
                             YerithDatabaseTableColumn::ID,
                             _curEmployeeDBID);

        result = result && YerithUtils::execQuery(UPDATE_GROUPES_DUN_EMPLOYE_INFO);
    }

    {
        QString UPDATE_PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR =
        			QString("DELETE FROM %1 WHERE %2='%3' AND %4='%5'")
						.arg(YerithDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR,
                             YerithDatabaseTableColumn::NOM_ENTREPRISE,
                             _curEMPLOYEE_NOM_ENTREPRISE,
                             YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                             _curEMPLOYEE_groupe_de_paie_hr);

        result = result && YerithUtils::execQuery(UPDATE_PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR);
    }

    {
        aQSqlQuery.clear();

        QSqlRecord an_employee_group_record_info;

        SELECT_GROUPES_DUN_EMPLOYE_INFO.clear();

        SELECT_GROUPES_DUN_EMPLOYE_INFO =
                        QString("select %1 from %2 where %3='%4'").arg
                        (YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                         YerithDatabase::GROUPES_DEMPLOYES_hr,
                         YerithDatabaseTableColumn::DESIGNATION,
						 employeeGroup_designation);

        result = result && YerithUtils::execQuery(aQSqlQuery, SELECT_GROUPES_DUN_EMPLOYE_INFO);

        if (!aQSqlQuery.next())
        {
            return false;
        }

        an_employee_group_record_info = aQSqlQuery.record();

        QString membres_du_groupe_db_ID =
        			GET_SQL_RECORD_DATA(an_employee_group_record_info,
                                        YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

        YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID,
        															   _curEmployeeDBID);

        QString UPDATE_GROUPES_DUN_EMPLOYE_DB_TABLE
					(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
						.arg(YerithDatabase::GROUPES_DEMPLOYES_hr,
                             YerithDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                             membres_du_groupe_db_ID,
                             YerithDatabaseTableColumn::DESIGNATION,
                             employeeGroup_designation));

        result = result && YerithUtils::execQuery(UPDATE_GROUPES_DUN_EMPLOYE_DB_TABLE);
    }

    if (result)
    {
        tableWidget_Groupes_Dun_Employe->removeArticle(currentRow);
    }

    int tableRowCount = tableWidget_Groupes_Dun_Employe->rowCount();

    if (tableRowCount > 0)
    {
        enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
    }
    else // <= 0
    {
        lineEdit_SALAIRE_CUMULE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));


        lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));


        disable_yerith_widgets();
    }


    lineEdit_Groupes_Dun_Employe_nombre_de_groupes
		->setText(QString::number(tableWidget_Groupes_Dun_Employe->rowCount()));


    afficher_tous_les_groupes_DUN_EMPLOYE_hr();


    tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();


    msgRETIRER.clear();

    msgRETIRER.append(
        QObject::tr("L'employé : (%1) a été retiré du groupe d'employés \"%2\" !")
            .arg(_curEMPLOYEE_NOM_ENTREPRISE,
                 employeeGroup_designation));


    YerithQMessageBox::information(this,
                                   QObject::tr("Retirer l'employé d'1 groupe d'employés"),
                                   msgRETIRER);


    emit YRI_ROW_count_changed();


    return result;
}


void YerithGROUPES_DUN_EMPLOYE_Window::afficher_au_detail(const QModelIndex &modelIndex)
{
    if (_curEMPLOYEEgroups_TableModel->rowCount() > 0)
    {
        _allWindows->_DETAILS_DUN_GROUPE_DEMPLOYES_hr
						->rendreVisible(_curEMPLOYEEgroups_TableModel,
                                        _curStocksTableModel,
                                        tableWidget_Groupes_Dun_Employe->get_DB_ELEMENT_db_ID(modelIndex.row()));

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 Profile d'Employé à afficher les détails."));
    }
}


void YerithGROUPES_DUN_EMPLOYE_Window::afficher_tous_les_groupes_DUN_EMPLOYE_hr()
{
    _somme_SALAIRE_CUMULE_A_DATE = 0.0;

    _map_GROUPE_SELECTIONNE__to__salaire.clear();

    tableWidget_Groupes_Dun_Employe->yerithClearTableWidgetContent();

    QSqlRecord record;

    _allWindows->_fournisseursWindow
		->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    _curEMPLOYEE_NOM_ENTREPRISE =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NOM_ENTREPRISE);

    _curEmployeeDBID =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID);


    groupBox_Groupes_Dun_Employe->setTitle(QObject::tr("Groupes d'appartenance DE L'EMPLOYÉ (%1)")
    										.arg(_curEMPLOYEE_NOM_ENTREPRISE));


    QString groupes_de_lemploye
		(GET_SQL_RECORD_DATA(record,
    						 YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE));


    QStringList allEmployeeGroups(groupes_de_lemploye.split(YerithUtils::STAR_CHAR));

    QString current_group_db_ID;

    QString maximum_de_membres;

    QString current_EMPLOYEE_PAY_GROUP;

    int curQuerySize = -1;

    QSqlQuery aSqlGroupTableModelQUERY;

    QString employeeGroupTableModelQUERY_STR;


    double CALCULATED_SALARY_for_EMPLOYEE = 0.0;

    YerithERPCalculSalaireEMPLOYE CHECK_VALIDITY_DATES_for_belonging_of_EMPLOYEE;


    for (int k = 0; k < allEmployeeGroups.size(); ++k)
    {
        employeeGroupTableModelQUERY_STR =
                        QString("select %1, %2, %3 from %4 where %5='%6'")
							.arg(YerithDatabaseTableColumn::ID,
								 YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
								 YerithDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
								 YerithDatabase::GROUPES_DEMPLOYES_hr,
								 YerithDatabaseTableColumn::DESIGNATION,
								 allEmployeeGroups.at(k).trimmed());

        aSqlGroupTableModelQUERY.clear();

        curQuerySize = YerithUtils::execQuery(aSqlGroupTableModelQUERY,
                                              employeeGroupTableModelQUERY_STR,
                                              _logger);

        if (curQuerySize > 0)
        {
            if (aSqlGroupTableModelQUERY.next())
            {
                current_group_db_ID =
                                QString::number(aSqlGroupTableModelQUERY.value(0).toInt());

                current_EMPLOYEE_PAY_GROUP =
                                aSqlGroupTableModelQUERY.value(1).toString();

                maximum_de_membres = aSqlGroupTableModelQUERY.value(2).toString();


                CALCULATED_SALARY_for_EMPLOYEE =
                    CHECK_VALIDITY_DATES_for_belonging_of_EMPLOYEE
                        .calculate_PAY_GROUP_MONEY_BENEFITS(_curEMPLOYEE_NOM_ENTREPRISE,
                                                            current_EMPLOYEE_PAY_GROUP);


                _map_GROUPE_SELECTIONNE__to__salaire
                    .insert(current_EMPLOYEE_PAY_GROUP,
                            CALCULATED_SALARY_for_EMPLOYEE);


                _somme_SALAIRE_CUMULE_A_DATE += CALCULATED_SALARY_for_EMPLOYEE;


                tableWidget_Groupes_Dun_Employe
					->insert_group(current_group_db_ID,
                                   allEmployeeGroups.at(k).trimmed(),
								   current_EMPLOYEE_PAY_GROUP,
								   maximum_de_membres,
								   (0.0 == CALCULATED_SALARY_for_EMPLOYEE));
            }

//              qDebug() << tableWidget_Groupes_Dun_Employe->get_mapListIdxToElement_db_ID();
        }
    }

    if (tableWidget_Groupes_Dun_Employe->rowCount() > 0)
    {
        lineEdit_SALAIRE_CUMULE
            ->setText(GET_CURRENCY_STRING_NUM(_somme_SALAIRE_CUMULE_A_DATE));

        tableWidget_Groupes_Dun_Employe->selectRow(0);
    }
    else
    {
        lineEdit_SALAIRE_CUMULE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));
    }


    emit YRI_ROW_count_changed();
}


void YerithGROUPES_DUN_EMPLOYE_Window::enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()
{
	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    actionAfficher_ce_groupe_DEMPLOYES_au_detail->setVisible(true);


    label_groupe_dun_employe_date_begin->setVisible(true);

    label_groupe_dun_employe_date_end->setVisible(true);

    label_activer_DATES_DAPPARTENANCE->setVisible(true);

    checkBox_activer_DATES_DAPPARTENANCE->setVisible(true);

    dateEdit_groupe_dun_employe_date_begin->setVisible(true);

    dateEdit_groupe_dun_employe_date_end->setVisible(true);


    pushButton_VALIDER->setVisible(true);

    pushButton_retirer->setVisible(true);
}



void YerithGROUPES_DUN_EMPLOYE_Window::
        get_PRINT_OUT_TexTableString(QString    &texTable_IN_OUT,
                                           int         row_MAX_TO_GO_export /* = -1 */)
{
	QString Header_TEMPLATE_texTable_In =
                "\\begin{table*}[!htbp]\n"
                "\\centering\n"
                "\\resizebox{\\textwidth}{!}{\n"
                "\\begin{tabular}"
                "{|c|l|l|r|} \\hline";


    QString ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In =
                "& & &				                \\\\ \n"
                "\\textbf{N\\textsuperscript{o}}    & 	 \n"
                "GROUP NAME	    	                & 	 \n"
                "PAY GROUP  		                & 	 \n"
                "MAX MEMBERS		                \\\\ \n"
                "& & &				                \\\\ \\hline \\hline \n";


   QString FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In =
                "& & & 				                \\\\ \n"
                "\\textbf{N\\textsuperscript{o}}    & 	 \n"
                "DÉSIGNATION DU GROUPE		        & 	 \n"
                "GROUP DE PAIE 				        & 	 \n"
                "NOMBRE DE MEMBRES MAXIMUM	        \\\\ \n"
                "& & &							    \\\\ \\hline \\hline \n";


    YerithQTableWidgetWindowsCommons::
        get_PRINT_OUT_TexTableString(*tableWidget_Groupes_Dun_Employe,
                                     texTable_IN_OUT,
                                     Header_TEMPLATE_texTable_In,
                                     FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In,
                                     ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In,
                                     row_MAX_TO_GO_export);
}


void YerithGROUPES_DUN_EMPLOYE_Window::disable_yerith_widgets()
{
	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    actionAfficher_ce_groupe_DEMPLOYES_au_detail->setVisible(false);


    dateEdit_groupe_dun_employe_date_begin->reset();

    dateEdit_groupe_dun_employe_date_end->reset();


    label_groupe_dun_employe_date_begin->setVisible(false);

    label_groupe_dun_employe_date_end->setVisible(false);

    label_activer_DATES_DAPPARTENANCE->setVisible(false);

    checkBox_activer_DATES_DAPPARTENANCE->setVisible(false);

    dateEdit_groupe_dun_employe_date_begin->setVisible(false);

    dateEdit_groupe_dun_employe_date_end->setVisible(false);


    pushButton_VALIDER->setVisible(false);

    pushButton_retirer->setVisible(false);
}


void YerithGROUPES_DUN_EMPLOYE_Window::setupLineEdits()
{
    lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
        ->setText(GET_CURRENCY_STRING_NUM(0.0));

    lineEdit_SALAIRE_CUMULE
        ->setText(GET_CURRENCY_STRING_NUM(0.0));


    lineEdit_SALAIRE_CUMULE->setYerithEnabled(false);

    lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE->setYerithEnabled(false);

    lineEdit_Groupes_Dun_Employe_nombre_de_groupes->setYerithEnabled(false);
}


void YerithGROUPES_DUN_EMPLOYE_Window::populateComboBoxes()
{
	comboBox_Groupes_Dun_Employe_recherche
    	->populateComboBoxRawString(YerithDatabase::GROUPES_DEMPLOYES_hr,
                              	    YerithDatabaseTableColumn::DESIGNATION);

	comboBox_Groupes_Dun_Employe_recherche->setYerithEnabled(true);
}


void YerithGROUPES_DUN_EMPLOYE_Window::rendreInvisible()
{
    _map_GROUPE_SELECTIONNE__to__salaire.clear();

    _somme_SALAIRE_CUMULE_A_DATE = 0.0;

    dateEdit_groupe_dun_employe_date_begin->reset();

    dateEdit_groupe_dun_employe_date_end->reset();


    dateEdit_groupe_dun_employe_date_begin->setYerithEnabled(false);

    dateEdit_groupe_dun_employe_date_end->setYerithEnabled(false);

    tableWidget_Groupes_Dun_Employe->yerithClearTableWidgetContent();


    YerithWindowsCommons::rendreInvisible();
}


void YerithGROUPES_DUN_EMPLOYE_Window::rendreVisible(YerithSqlTableModel *EMPLOYEE_TableModel,
                                                 	 YerithSqlTableModel *stocksTableModel)
{
    static bool FIRST_time_execution = true;

    if (FIRST_time_execution)
    {
        tableWidget_Groupes_Dun_Employe->a_current_window = this;

        FIRST_time_execution = false;
    }

	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curEMPLOYEEgroups_TableModel = EMPLOYEE_TableModel;


    {
        QSqlRecord a_qsql_record;

        _allWindows->_fournisseursWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(a_qsql_record);


        QString position_function =
            a_qsql_record.value(YerithDatabaseTableColumn::POSITION_FONCTION).toString();


        label_groupe_dun_employe_POSITION_FONCTION->setText(position_function);


        QVariant img(a_qsql_record.value(YerithDatabaseTableColumn::IMAGE_FOURNISSEUR));

        if (!img.isNull())
        {
            YerithUtils::loadPixmapFromDB(*label_image_Employe, img, "JPG");
        }
        else
        {
            YerithWindowsCommons::supprimer_image(*label_image_Employe);
        }
    }


    populateComboBoxes();

    tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();

    afficher_tous_les_groupes_DUN_EMPLOYE_hr();


    groupBox_Image_de_lemploye
        ->setTitle(QObject::tr("PORTRAIT DE L'EMPLOYÉ(E) \"%1\"")
                    .arg(_curEMPLOYEE_NOM_ENTREPRISE));


    int tableRowCount = tableWidget_Groupes_Dun_Employe->rowCount();

    if (tableRowCount > 0)
    {
        enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
    }
    else
    {
        disable_yerith_widgets();
    }

    setVisible(true);


    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("groupes d'1 employé"),
    								 _curEMPLOYEEgroups_TableModel);


    APPLY_USER_LOCAL_SETTINGS_PARAMETERS_QTABLEWIDGET(*this);


    lineEdit_Groupes_Dun_Employe_nombre_de_groupes->setText(GET_NUM_STRING(tableRowCount));

    tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();


    handle_some_actions_tools_enabled();


    emit YRI_ROW_count_changed();
}


void YerithGROUPES_DUN_EMPLOYE_Window::definirManager()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_IMPRIMER_UNE_LISTE_de_groupe_demployes_JUSQUAU_selectionne, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_Groupes_Dun_Employe_recherche->setVisible(true);


    pushButton_MANUAL_pay_for_selected_PAY_GROUP
        ->enable(this, SLOT(handle_MANUAL_pay_for_selected_PAY_GROUP_button()));

    pushButton_VALIDER->enable(this, SLOT(handle_VALIDER_button()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_groupes_demployes->enable(this, SLOT(GROUPES_demployes_hr()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_RETOUR->enable(this, SLOT(fournisseurs()));
    pushButton_retirer->enable(this, SLOT(retirer_CET_EMPLOYE_du_groupe_selectionne()));
}


void YerithGROUPES_DUN_EMPLOYE_Window::definirVendeur()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_IMPRIMER_UNE_LISTE_de_groupe_demployes_JUSQUAU_selectionne, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_Groupes_Dun_Employe_recherche->setVisible(false);


    pushButton_MANUAL_pay_for_selected_PAY_GROUP->disable(this);

    pushButton_VALIDER->disable(this);
    pushButton_paiements->disable(this);
    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_RETOUR->enable(this, SLOT(fournisseurs()));
    pushButton_retirer->disable(this);
}


void YerithGROUPES_DUN_EMPLOYE_Window::definirPasDeRole()
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_IMPRIMER_UNE_LISTE_de_groupe_demployes_JUSQUAU_selectionne, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    comboBox_Groupes_Dun_Employe_recherche->setVisible(false);


    pushButton_MANUAL_pay_for_selected_PAY_GROUP->disable(this);

    pushButton_VALIDER->disable(this);
    pushButton_paiements->disable(this);
    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);
}
