/*
 * yerith-erp-fournisseur-details-window.cpp
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-fournisseur-details-window.hpp"

#include "yerith-erp-calcul-salaire-EMPLOYE-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerithFournisseurDetailsWindow::YerithFournisseurDetailsWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithFournisseurDetailsWindow")),
 _SHOW_EMPLOYEE(false),
 _sommeTotal(0.0)
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("détails d'un fournisseur"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    setupLineEdits();

    textEdit_fournisseur_details_description_du_fournisseur->setYerithEnabled
    (false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_EMPLOYEE_PAY_GROUP->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()));
    connect(actionFournisseurs, SIGNAL(triggered()), this,
            SLOT(fournisseurs()));
    connect(actionModifierFournisseur, SIGNAL(triggered()), this,
            SLOT(modifierFournisseur()));
    connect(actionSupprimerFournisseur, SIGNAL(triggered()), this,
            SLOT(supprimerFournisseur()));
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


void YerithFournisseurDetailsWindow::afficher_groupes_dun_employe()
{
    if (_curFournisseurTableModel->rowCount() > 0)
    {
        rendreInvisible();

        _allWindows->_groupes_DUN_EMPLOYE_window
			->rendreVisible(_curFournisseurTableModel,
                            _curStocksTableModel);
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("groupes d'EMPLOYÉS d'appartenance"),
                                       QObject::tr("Sélectionnez 1 employé afin de visualiser "
                                                   "ses GROUPES d'EMPLOYÉS d'appartenance."));
    }
}


void YerithFournisseurDetailsWindow::modifierFournisseur()
{
    rendreInvisible();

    bool show_TabDESCRIPTION_SUPPLIER =
                    (tabWidget_details_fournisseur->currentIndex() == 1);

    _allWindows->_modifierFournisseurWindow
		->rendreVisible(_curFournisseurTableModel,
						_curStocksTableModel,
						show_TabDESCRIPTION_SUPPLIER,
						_SHOW_EMPLOYEE);
}


void YerithFournisseurDetailsWindow::supprimerFournisseur()
{
    QSqlRecord record;

    _allWindows->
    _fournisseursWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression du compte fournisseur '%1' ?").
                         arg(GET_SQL_RECORD_DATA
                             (record,
                              YerithDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"), msgSupprimer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved =
                        _allWindows->
                        _fournisseursWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
                        ();

        //qDebug() << "YerithModifierWindow::supprimer_ce_stock() " << resRemoved;
        fournisseurs();
        if (resRemoved)
        {
            msgSupprimer =
                            QObject::tr
                            ("Le compte fournisseur '%1' a été supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("Le compte fournisseur '%1' ne pouvait pas être supprimé !").
                            arg(GET_SQL_RECORD_DATA
                                (record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerithFournisseurDetailsWindow::setupLineEdits()
{
    lineEdit_fournisseur_details_reference_registre_du_commerce->
    setYerithEnabled(false);
    lineEdit_fournisseur_details_reference_fournisseur->
    setYerithEnabled(false);
    lineEdit_fournisseur_details_nom_entreprise->setYerithEnabled(false);
    lineEdit_fournisseur_details_nom_representant->setYerithEnabled(false);
    lineEdit_fournisseur_details_quartier->setYerithEnabled(false);
    lineEdit_fournisseur_details_ville->setYerithEnabled(false);
    lineEdit_fournisseur_details_province_etat->setYerithEnabled(false);
    lineEdit_fournisseur_details_pays->setYerithEnabled(false);
    lineEdit_fournisseur_details_boite_postale->setYerithEnabled(false);
    lineEdit_fournisseur_details_siege_social->setYerithEnabled(false);
    lineEdit_fournisseur_details_email->setYerithEnabled(false);
    lineEdit_fournisseur_details_numero_telephone_1->setYerithEnabled(false);
    lineEdit_fournisseur_details_numero_telephone_2->setYerithEnabled(false);
    lineEdit_fournisseur_details_numero_contribuable->setYerithEnabled(false);
    lineEdit_fournisseur_details_fournisseur->setYerithEnabled(false);
    lineEdit_employe_qualite->setYerithEnabled(false);
    lineEdit_employe_SUPERIEUR_hierarchique->setYerithEnabled(false);
	lineEdit_employe_DEPARTEMENT->setYerithEnabled(false);
	lineEdit_EMPLOYE_SALAIRE_annuel->setYerithEnabled(false);
	lineEdit_EMPLOYE_SALAIRE_MENSUEL->setYerithEnabled(false);
	lineEdit_EMPLOYE_position_fonction->setYerithEnabled(false);
}


void YerithFournisseurDetailsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_EMPLOYEE_PAY_GROUP->disable(this);
}


void YerithFournisseurDetailsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));
    pushButton_EMPLOYEE_PAY_GROUP->enable(this, SLOT(afficher_groupes_dun_employe()));


#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif
}


bool YerithFournisseurDetailsWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");

    QString yerithSupplierAccountImage("yerith");

    QString
    yerithCustomerAccountImageTmpFile(QString("%1JPG").arg
                                      (YerithUtils::
                                       getUniquePrefixFileInTemporaryFilesDir
                                       (yerithSupplierAccountImage)));

    const QPixmap *label_image_produit_pixmap = label_image_produit->pixmap();

    if (0 != label_image_produit_pixmap)
    {
        YerithUtils::savePixmapToFile(yerithCustomerAccountImageTmpFile,
                                      *label_image_produit_pixmap, "JPG");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);

    QString latexFileNamePrefix("yerith-erp-liste-fournisseur");


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        latexFileNamePrefix.clear();
        latexFileNamePrefix.append("yerith-erp-supplier-file");
    }


    QString texDocument;

    YerithUtils::getLatexSupplierData(texDocument);

    QString data;


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	if (_SHOW_EMPLOYEE)
    	{
            data.append(YerithUtils::get_latex_bold_text("Employee FUNCTION: "));
            data.append(QString("%1\\\\\n")
                         .arg(lineEdit_EMPLOYE_position_fonction->text_LATEX()));

            data.append(YerithUtils::get_latex_bold_text("Employee reference: "));
    	}
    	else
    	{
    	    data.append(YerithUtils::get_latex_bold_text("Supplier reference: "));
    	}
    }
    else
    {
    	if (_SHOW_EMPLOYEE)
    	{
            data.append(YerithUtils::get_latex_bold_text("POSTE de l'employé: "));
            data.append(QString("%1\\\\\n")
                         .arg(lineEdit_EMPLOYE_position_fonction->text_LATEX()));

    	    data.append(YerithUtils::get_latex_bold_text("Référence employé: "));
    	}
    	else
    	{
    	    data.append(YerithUtils::get_latex_bold_text("Référence fournisseur: "));
    	}
    }

    data.append(QString("%1\\\\\n")
                  .arg(lineEdit_fournisseur_details_reference_fournisseur
                        ->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Nom de l'entreprise: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_nom_entreprise->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Nom du Représentant: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_nom_representant->
                 text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Quartier: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_quartier->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Ville: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_ville->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Province / État: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_province_etat->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Pays: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_pays->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Boîte postale: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_boite_postale->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Siège social: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_siege_social->text_LATEX()));

    data.
    append(YerithUtils::get_latex_bold_text(QObject::tr("Émail: ")));
    data.append(QString("%1\\\\\n").
                arg(lineEdit_fournisseur_details_email->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Numéro de téléphone 1: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_numero_telephone_1->
                 text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Numéro de téléphone 2: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_numero_telephone_2->
                 text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Numéro de contribuable: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_numero_contribuable->text_LATEX
                 ()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("RCCM N\\textsuperscript{o}: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_reference_registre_du_commerce->
                 text_LATEX()));


    if (!_SHOW_EMPLOYEE)
    {
        data.append(YerithUtils::get_latex_bold_text(QObject::tr("Compte (argent à reverser) au Fournisseur: ")));
        data.append(QString("%1\\\\\n")
                     .arg(lineEdit_fournisseur_details_fournisseur->text_LATEX()));
    }


    data.append("\n\n\\vspace{0.3cm}\n\n");



    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	if (_SHOW_EMPLOYEE)
    	{
            texDocument.replace("\captionof{figure}{Image associated for supplier",
                                "\captionof{figure}{Image associated for Employee",
                                Qt::CaseSensitive);

    	    data.append(
                QString("%1")
                    .arg(YerithUtils::get_latex_bold_text
                            ("Employee Description:")));
    	}
    	else
    	{
            texDocument.replace("\captionof{figure}{Image associated for Employee",
                                "\captionof{figure}{Image associated for supplier",
                                Qt::CaseSensitive);

    	    data.append(
                QString("%1")
                    .arg(YerithUtils::get_latex_bold_text
                            ("Supplier Description:")));
    	}
    }
    else
    {
    	if (_SHOW_EMPLOYEE)
    	{
            texDocument.replace("\captionof{figure}{L'image associée au fournisseur",
                                "\captionof{figure}{L'image associée à l'employé(e)",
                                Qt::CaseSensitive);

    	    data.append(
                QString("%1")
                    .arg(YerithUtils::get_latex_bold_text
                            ("Description de l'employé:")));
    	}
    	else
    	{
            texDocument.replace("\captionof{figure}{L'image associée à l'employé(e)",
                                "\captionof{figure}{L'image associée au fournisseur",
                                Qt::CaseSensitive);

    	    data.append(
                QString("%1")
                    .arg(YerithUtils::get_latex_bold_text
                            ("Description du fournisseur:")));
    	}
    }



    data.append("\n\n\\vspace{0.3cm}\n\n");

    texDocument.replace("YERITHDETAILSFOURNISSEUR", data);

    data.clear();
    data.append(QString("%1\\\\").arg
                (textEdit_fournisseur_details_description_du_fournisseur->
                 toPlainTextForLatex()));

    texDocument.replace("YERITHDESCRIPTIONFOURNISSEUR", data);

    if (0 != label_image_produit_pixmap)
    {
        texDocument.replace("YERITHCHEMINCOMPLETIMAGEFOURNISSEUR",
                            yerithCustomerAccountImageTmpFile);
    }
    else
    {
        texDocument.replace("YERITHCHEMINCOMPLETIMAGEFOURNISSEUR", "");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(24);

    YerithInfoEntreprise &infoEntreprise =
                    YerithUtils::getAllWindows()->getInfoEntreprise();

    QString
    fileDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
             (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YERITHPAPERSPEC", "a4paper");

    texDocument.replace("YERITHFOURNISSEUR",
                        QString("%1").arg
                        (lineEdit_fournisseur_details_nom_entreprise->
                         text_LATEX()));
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


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	if (_SHOW_EMPLOYEE)
    	{
    	    texDocument.replace("YERITHOBJECT", "Employee");
    	}
    	else
    	{
            texDocument.replace("YERITHOBJECT", "Supplier");
    	}
    }
    else
    {
        if (_SHOW_EMPLOYEE)
    	{
    	    texDocument.replace("YERITHOBJECT", "de l'EMPLOYÉ");
    	}
    	else
    	{
            texDocument.replace("YERITHOBJECT", "de fournisseur");
    	}
    }


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


void YerithFournisseurDetailsWindow::rendreInvisible()
{
    lineEdit_fournisseur_details_reference_registre_du_commerce->clear();
    lineEdit_fournisseur_details_reference_fournisseur->clear();
    lineEdit_fournisseur_details_nom_entreprise->clear();
    lineEdit_fournisseur_details_nom_representant->clear();
    lineEdit_fournisseur_details_quartier->clear();
    lineEdit_fournisseur_details_ville->clear();
    lineEdit_fournisseur_details_province_etat->clear();
    lineEdit_fournisseur_details_pays->clear();
    lineEdit_fournisseur_details_boite_postale->clear();
    lineEdit_fournisseur_details_siege_social->clear();
    lineEdit_fournisseur_details_email->clear();
    lineEdit_fournisseur_details_numero_telephone_1->clear();
    lineEdit_fournisseur_details_numero_telephone_2->clear();
    lineEdit_fournisseur_details_numero_contribuable->clear();
    lineEdit_fournisseur_details_fournisseur->clear();

    lineEdit_employe_qualite->clear();
	lineEdit_employe_SUPERIEUR_hierarchique->clear();
	lineEdit_employe_DEPARTEMENT->clear();
	lineEdit_EMPLOYE_SALAIRE_annuel->clear();
	lineEdit_EMPLOYE_SALAIRE_MENSUEL->clear();
	lineEdit_EMPLOYE_position_fonction->clear();

    textEdit_fournisseur_details_description_du_fournisseur->clear();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);

    _sommeTotal = 0.0;

    YerithWindowsCommons::rendreInvisible();
}


void YerithFournisseurDetailsWindow::rendreVisible(YerithSqlTableModel *fournisseurTableModel,
                                                   YerithSqlTableModel *stocksTableModel,
												   bool 			   employe /* = false */)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curFournisseurTableModel = fournisseurTableModel;

    _SHOW_EMPLOYEE = employe;
    //qDebug() << "++ last selected row: " << _allWindows->getLastSelectedListerRow();

    if (true == _SHOW_EMPLOYEE)
    {
        pushButton_EMPLOYEE_PAY_GROUP->setVisible(true);

    	label_employe_SUPERIEUR_hierarchique->setVisible(true);
    	label_employe_DEPARTEMENT->setVisible(true);
		label_EMPLOYE_SALAIRE_annuel->setVisible(true);
		label_EMPLOYE_SALAIRE_MENSUEL->setVisible(true);
		label_EMPLOYE_position_fonction->setVisible(true);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(true);
		lineEdit_employe_DEPARTEMENT->setVisible(true);
		lineEdit_EMPLOYE_SALAIRE_annuel->setVisible(true);
		lineEdit_EMPLOYE_SALAIRE_MENSUEL->setVisible(true);
		lineEdit_EMPLOYE_position_fonction->setVisible(true);
    }
    else
    {
        pushButton_EMPLOYEE_PAY_GROUP->setVisible(false);

    	label_employe_SUPERIEUR_hierarchique->setVisible(false);
    	label_employe_DEPARTEMENT->setVisible(false);
		label_EMPLOYE_SALAIRE_annuel->setVisible(false);
		label_EMPLOYE_SALAIRE_MENSUEL->setVisible(false);
		label_EMPLOYE_position_fonction->setVisible(false);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(false);
		lineEdit_employe_DEPARTEMENT->setVisible(false);
		lineEdit_EMPLOYE_SALAIRE_annuel->setVisible(false);
		lineEdit_EMPLOYE_SALAIRE_MENSUEL->setVisible(false);
		lineEdit_EMPLOYE_position_fonction->setVisible(false);
    }

    setVisible(true);

    showFournisseurDetail(_SHOW_EMPLOYEE);


//    if (_SHOW_EMPLOYEE)
//    {
//        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'un EMPLOYÉ"),
//                                         _curFournisseurTableModel);
//    }
//    else
//    {
        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'un fournisseur"),
                                         _curFournisseurTableModel);
//    }
}


void YerithFournisseurDetailsWindow::showFournisseurDetail(bool employe /* = false */)
{
    QSqlRecord record;


    _allWindows->_fournisseursWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);


    lineEdit_fournisseur_details_reference_fournisseur
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::REFERENCE_FOURNISSEUR));

    lineEdit_fournisseur_details_reference_registre_du_commerce
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

    lineEdit_fournisseur_details_nom_entreprise
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

    lineEdit_fournisseur_details_nom_representant
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NOM_REPRESENTANT));

    lineEdit_fournisseur_details_quartier
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::QUARTIER));

    lineEdit_fournisseur_details_ville
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::VILLE));

    lineEdit_fournisseur_details_province_etat
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::PROVINCE_ETAT));

    lineEdit_fournisseur_details_pays
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::PAYS));

    lineEdit_fournisseur_details_boite_postale
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::BOITE_POSTALE));

    lineEdit_fournisseur_details_siege_social
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::SIEGE_SOCIAL));

    lineEdit_fournisseur_details_email
        ->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::EMAIL));



    double sommeTotal_SALAIRE = 0.0;


    YerithERPCalculSalaireEMPLOYE CALCUL_SALAIRE_EMPLOYE;


    if (true == employe)
    {
        label_fournisseur_details_fournisseur->setVisible(false);
        lineEdit_fournisseur_details_fournisseur->setVisible(false);

        tabWidget_details_fournisseur->setTabText(0, QObject::tr("Détails de l'employé(e)"));

        tabWidget_details_fournisseur->setTabText(1, QObject::tr("Description de l'employé(e)"));

        groupBox_IMAge_du_FOURNISSEUR
            ->setTitle(QObject::tr("Image de l'employé"));

    	lineEdit_employe_qualite
			->setText(QObject::tr("Employé"));

    	lineEdit_employe_SUPERIEUR_hierarchique
			->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::MANAGER));

    	lineEdit_employe_DEPARTEMENT
			->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DEPARTEMENT));

        sommeTotal_SALAIRE =
            CALCUL_SALAIRE_EMPLOYE.CALCULATE_EMPLOYEE_SALARY
                (GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

    	lineEdit_EMPLOYE_position_fonction
			->setText(GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::POSITION_FONCTION));
    }
    else
    {
        label_fournisseur_details_fournisseur->setVisible(true);
        lineEdit_fournisseur_details_fournisseur->setVisible(true);

        tabWidget_details_fournisseur->setTabText(0, QObject::tr("Détails du Fournisseur"));

        tabWidget_details_fournisseur->setTabText(1, QObject::tr("Description du Fournisseur"));

        groupBox_IMAge_du_FOURNISSEUR
            ->setTitle(QObject::tr("Image du fournisseur"));

    	lineEdit_employe_qualite
			->setText(QObject::tr("FOURNISSEUR"));

    	lineEdit_employe_SUPERIEUR_hierarchique->clear();
    	lineEdit_employe_DEPARTEMENT->clear();
    	lineEdit_EMPLOYE_SALAIRE_annuel->clear();
    	lineEdit_EMPLOYE_SALAIRE_MENSUEL->clear();
    	lineEdit_EMPLOYE_position_fonction->clear();
    }

    lineEdit_fournisseur_details_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

    lineEdit_fournisseur_details_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));

    lineEdit_fournisseur_details_numero_contribuable->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE));


    double fournisseur = GET_SQL_RECORD_DATA(record,
                                             YerithDatabaseTableColumn::COMPTE_FOURNISSEUR).
                         toDouble();

    lineEdit_fournisseur_details_fournisseur->setText(GET_CURRENCY_STRING_NUM
                                                      (fournisseur));

    textEdit_fournisseur_details_description_du_fournisseur->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::DESCRIPTION_FOURNISSEUR));

    QVariant img(record.value(YerithDatabaseTableColumn::IMAGE_FOURNISSEUR));

    if (!img.isNull())
    {
        YerithUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }

    lineEdit_EMPLOYE_SALAIRE_MENSUEL
        ->setText(GET_CURRENCY_STRING_NUM(sommeTotal_SALAIRE));
}


void YerithFournisseurDetailsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
}
