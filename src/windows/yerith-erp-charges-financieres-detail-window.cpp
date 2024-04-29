/*
 * yerith-erp-charges-financieres-detail-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-charges-financieres-detail-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include <QtSql/QSqlRecord>


YerithChargesFinancieresDetailsWindow::YerithChargesFinancieresDetailsWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithChargesFinancieresDetailsWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("détails d'1 CHARGE FINANCIÈRE"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_DARK_GRAY_60_60_60,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    setupLineEdits();

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_detail_charges_financieres_CREER->disable(this);
    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_menu->disable(this);
    pushButton_retour->disable(this);

    /** Menu actions */

    connect(action_CREER_A_PARTIR_DE_CECI,
    		SIGNAL(triggered()),
			this,
			SLOT(SLOT_CREER_A_PARTIR_DE_CECI()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
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

void YerithChargesFinancieresDetailsWindow::setupLineEdits()
{
    dateEdit_date_de_reception->setYerithEnabled(false);
    dateEdit_date_de_commande->setYerithEnabled(false);

    lineEdit_departement->setYerithEnabled(false);
    lineEdit_reference_produit->setYerithEnabled(false);
    lineEdit_designation->setYerithEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerithEnabled(false);
    lineEdit_LIGNE_BUDGETAIRE->setYerithEnabled(false);
    lineEdit_quantite->setYerithEnabled(false);
    lineEdit_prix_dachat->setYerithEnabled(false);
    lineEdit_prix_unitaire->setYerithEnabled(false);

    lineEdit_STATUT_DE_LACHAT->setYerithEnabled(false);
	lineEdit_MONTANT_TVA->setYerithEnabled(false);
	lineEdit_ref_RECU_DACHAT->setYerithEnabled(false);
	lineEdit_LOCALISATION->setYerithEnabled(false);
	lineEdit_ID_commandeur->setYerithEnabled(false);

    textEdit_une_CHARGE_FINANCIERE->setYerithEnabled(false);
}


void YerithChargesFinancieresDetailsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_detail_charges_financieres_CREER->disable(this);

    pushButton_menu->disable(this);
    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_retour->disable(this);
}


void YerithChargesFinancieresDetailsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_detail_charges_financieres_CREER
		->enable(this, SLOT(SLOT_CREER_A_PARTIR_DE_CECI()));

    pushButton_entrer->enable(this, SLOT(entrer_une_charge_financiere()));
    pushButton_lister->enable(this, SLOT(stocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(charges_financieres()));
}


void YerithChargesFinancieresDetailsWindow::rendreInvisible()
{
	dateEdit_date_de_commande->reset();
    lineEdit_reference_produit->clear();
    lineEdit_designation->clear();
    lineEdit_LIGNE_BUDGETAIRE->clear();
    lineEdit_quantite->clear();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_prix_dachat->clear();
    lineEdit_prix_unitaire->clear();

    YerithWindowsCommons::rendreInvisible();
}


void YerithChargesFinancieresDetailsWindow::rendreVisible(YerithSqlTableModel *stocksTableModel,
                                                          YerithSqlTableModel *achatStocksTableModel)
{
    retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _cur_CHARGES_FINANCIERESTableModel = achatStocksTableModel;

    setVisible(true);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'1 CHARGE FINANCIÈRE"),
    								 _cur_CHARGES_FINANCIERESTableModel);

    //qDebug() << "++ last selected row: " << YerithERPWindows::get_last_lister_selected_row_db_ID();
    showItem();
}


void YerithChargesFinancieresDetailsWindow::showItem()
{
    _cur_CHARGES_FINANCIERESTableModel
		->yerithSetFilter_WITH_where_clause
			(QString("%1 = '%2'")
				.arg(YerithDatabaseTableColumn::ID,
					 YerithERPWindows::get_last_lister_selected_row_db_ID()));


    QSqlRecord record = _cur_CHARGES_FINANCIERESTableModel->record(0);

    dateEdit_date_de_reception
	->setDate(record.value(YerithDatabaseTableColumn::DATE_DE_RECEPTION).toDate());

    dateEdit_date_de_commande
	->setDate(record.value(YerithDatabaseTableColumn::DATE_DE_COMMANDE).toDate());


    lineEdit_departement
	->setText(GET_SQL_RECORD_DATA(record,
			YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    lineEdit_reference_produit
	->setText(GET_SQL_RECORD_DATA(record,
			YerithDatabaseTableColumn::REFERENCE));

    lineEdit_designation
	->setText(GET_SQL_RECORD_DATA(record,
			YerithDatabaseTableColumn::DESIGNATION));

    lineEdit_LIGNE_BUDGETAIRE
	->setText(GET_SQL_RECORD_DATA(record,
			YerithDatabaseTableColumn::CATEGORIE));

    lineEdit_nom_entreprise_fournisseur
	->setText(GET_SQL_RECORD_DATA(record,
			YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));


    double prix_unitaire = GET_SQL_RECORD_DATA(record,
    		YerithDatabaseTableColumn::PRIX_UNITAIRE).toDouble();


    lineEdit_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));


    double prix_dachat = 0.0;


    YerithPOSUser *currentUser = YerithUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
    	if (currentUser->isManager() ||
    		currentUser->isGestionaireDesStocks())
    	{
    		prix_dachat =
    				GET_SQL_RECORD_DATA(record,
    						YerithDatabaseTableColumn::PRIX_DACHAT).toDouble();
    	}
    }


    lineEdit_prix_dachat->setText(GET_CURRENCY_STRING_NUM(prix_dachat));


    double quantite_restante =
    		GET_SQL_RECORD_DATA(record,
    				YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();


    lineEdit_quantite->setText(GET_DOUBLE_STRING_P(quantite_restante, 0));


    lineEdit_STATUT_DE_LACHAT->setText(GET_SQL_RECORD_DATA(record,
    		YerithDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR));

    lineEdit_MONTANT_TVA->setText(GET_SQL_RECORD_DATA(record,
    		YerithDatabaseTableColumn::MONTANT_TVA));

    lineEdit_ref_RECU_DACHAT->setText(GET_SQL_RECORD_DATA(record,
    		YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT));

    lineEdit_LOCALISATION->setText(GET_SQL_RECORD_DATA(record,
    		YerithDatabaseTableColumn::LOCALISATION));

    lineEdit_ID_commandeur->setText(GET_SQL_RECORD_DATA(record,
    		YerithDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT));


    textEdit_une_CHARGE_FINANCIERE->setText(GET_SQL_RECORD_DATA(record,
    		YerithDatabaseTableColumn::DESCRIPTION_charge_financiere));


    _cur_CHARGES_FINANCIERESTableModel->resetFilter();
}


bool YerithChargesFinancieresDetailsWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");


    QString latexFileNamePrefix("yerith-erp-BON-DE-COMMANDE");


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        latexFileNamePrefix.clear();
        latexFileNamePrefix.append("yerith-erp-ORDER-SHEET");
    }


    QString texDocument;

    //TODO
    YerithUtils::getLatex_FINANCIALexpense_Data(texDocument);

    QString data;

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("NOM DE L'employé commandeur: ")));
    data.append(QString("%1\\\\\n").arg
                (_allWindows->getUser()->nom_completTex()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("DATE DE COMMANDE: ")));
    data.append(QString("%1\\\\\n").arg
                (dateEdit_date_de_commande->dateTime().toString("dd.MM.yyyy")));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("DATE DE réception: ")));

    //QDEBUG_STRING_OUTPUT_2_N("dateEdit_date_de_reception >= dateEdit_date_de_commande",
    //                         dateEdit_date_de_reception >= dateEdit_date_de_commande);

    data.append(QString("%1\\\\\n")
                 .arg(dateEdit_date_de_commande->dateTime()
                        .toString("dd.MM.yyyy")));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Département : ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_departement->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Référence: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_reference_produit->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Désignation: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_designation->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("FOURNISSEUR: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_nom_entreprise_fournisseur->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("LIGNE BUDGÉTAIRE: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_LIGNE_BUDGETAIRE->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Quantité: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_quantite->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("PRIX D'ACHAT: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_prix_dachat->text_LATEX()));

    data.
    append(YerithUtils::get_latex_bold_text(QObject::tr("PRIX UNITAIRE: ")));
    data.append(QString("%1\\\\\n").
                arg(lineEdit_prix_unitaire->text_LATEX()));

    data.append("\n\n\\vspace{0.3cm}\n\n");

    texDocument.replace("YERITHDETAILSBONDECOMMANDE", data);


    data.clear();

    data.append(QString("%1")
                 .arg(YerithUtils::get_latex_bold_text
                    (QObject::tr("Description de la charge financière: "))));

    data.append(QString("%1\\\\").arg
                (textEdit_une_CHARGE_FINANCIERE->toPlainTextForLatex()));

    texDocument.replace("YERITHDESCRIPTIONBONDECOMMANDE", data);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(78);

    YerithInfoEntreprise &infoEntreprise =
                    YerithUtils::getAllWindows()->getInfoEntreprise();

    QString
    fileDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
             (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YERITHPAPERSPEC", "a4paper");

    texDocument.replace("YERITHENTREPRISE",
                        infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YERITHACTIVITESENTREPRISE",
                        infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YERITHBOITEPOSTALE",
                        infoEntreprise.getBoitePostal());
    texDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YERITHDATE", fileDate);
    texDocument.replace("YERITHFOURNISSEUR",
                        lineEdit_nom_entreprise_fournisseur->text_LATEX());
    texDocument.replace("YERITHDESIGNATIONBONDECOMMANDE",
                        lineEdit_designation->text_LATEX());
    texDocument.replace("YERITHNOMUTILISATEUR",
                        _allWindows->getUser()->nom_completTex());
    texDocument.replace("YERITHSUCCURSALE",
                        YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerithERPConfig::THIS_SITE_LOCALISATION_NAME));
    texDocument.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YERITHCOMPTEBANCAIRENR",
                        infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YERITHCONTRIBUABLENR",
                        infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YERITHAGENCECOMPTEBANCAIRE",
                        infoEntreprise.getAgenceCompteBancaireTex());


    QString yerithPrefixFileName;

    yerithPrefixFileName
        .append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

    //qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex").arg(yerithPrefixFileName));

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(92);

    QString
    pdfSupplierDataFileName(YerithERPProcess::compileLatex
                            (yerithPrefixFileName));

    if (pdfSupplierDataFileName.isEmpty())
    {
        return false;
    }

    YerithERPProcess::startPdfViewerProcess(pdfSupplierDataFileName);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

    return true;
}


void YerithChargesFinancieresDetailsWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_CHARGE_FINANCIERE_detailWindow->palette());
    menu.addAction(action_CREER_A_PARTIR_DE_CECI);
    menu.exec(event->globalPos());
}


void YerithChargesFinancieresDetailsWindow::hideEvent(QHideEvent *hideEvent)
{
    YerithWindowsCommons::
    CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
}


void YerithChargesFinancieresDetailsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
