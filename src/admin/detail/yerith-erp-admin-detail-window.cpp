/*
 * yerith-erp-admin-detail-window.cpp
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-admin-detail-window.hpp"

#include "src/yerith-erp-windows.hpp"


#include "src/process/yerith-erp-process.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QDesktopWidget>

#include <QtSql/QSqlError>

#include <QtSql/QSqlRecord>


YerithAdminDetailWindow::YerithAdminDetailWindow()
:YerithPOSAdminWindowsCommons(QObject::tr("administration ~ détail")),
 _logger(new YerithLogger("YerithAdminDetailWindow"))
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
						.arg(COLOUR_RGB_STRING_YERITH_DARK_GREEN_47_67_67,
							 COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    setupLineEdits();

    dateEdit_detail_alerte_date_debut->setYerithEnabled(false);

    dateEdit_detail_alerte_date_fin->setYerithEnabled(false);

    dateEdit_date_de_reception->setYerithEnabled(false);

    dateEdit_date_de_commande->setYerithEnabled(false);

    dateEdit_detail_utilisateur_date_naissance->setEnabled(false);

    dateEdit_detail_localisation_date_ouverture->setEnabled(false);

    textEdit_detail_departements_de_produits_description->setYerithEnabled(false);
    textEdit_detail_categorie_description->setYerithEnabled(false);
    textEdit_une_CHARGE_FINANCIERE->setYerithEnabled(false);
    textEdit_detail_dune_LIGNE_budgetaire->setYerithEnabled(false);
    textEdit_detail_compte_bancaire_description_du_compte->setYerithEnabled(false);
    textEdit_detail_localisation_description_lieu->setYerithEnabled(false);
    textEdit_detail_alerte_message->setYerithEnabled(false);

    radioButton_detail_alerte_date_periode_temps->setEnabled(false);
    radioButton_detail_alerte_quantite->setEnabled(false);


    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);


    pushButton_detail_charges_financieres_CREER->disable(this);

    pushButton_creer->enable(this, SLOT(creer()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(lister()));
    pushButton_modifier->enable(this, SLOT(modifier()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));

    connect(actionCreer, SIGNAL(triggered()), this, SLOT(creer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionModifier, SIGNAL(triggered()), this, SLOT(modifier()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));

    pushButton_detail_utilisateur_retour->enable(this,
                                                 SLOT
                                                 (retourListerCompteUtilisateur
                                                  ()));
    pushButton_detail_localisation_retour->enable(this,
                                                  SLOT
                                                  (retourListerLocalisation
                                                   ()));
    pushButton_detail_departements_de_produits_retour->enable(this,
                                                              SLOT
                                                              (retourListerDepartementsDeProduits
                                                               ()));
    pushButton_detail_categorie_retour->enable(this,
                                               SLOT(retourListerCategorie()));

    pushButton_detail_LIGNE_budgetaire_retour->enable(this,
                                               	      SLOT(retourLister_LIGNE_BUDGETAIRE()));

    pushButton_detail_compte_bancaire_retour->enable(this,
                                                     SLOT
                                                     (retourListerCompteBancaire
                                                      ()));
    pushButton_detail_remise_retour->enable(this,
                                            SLOT(retourListerRemise()));

    pushButton_detail_charges_financieres_retour->enable(this,
    													 SLOT(retourListerChargeFinanciere()));

    pushButton_detail_alerte_retour->enable(this,
                                            SLOT(retourListerAlerte()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));

    connect(tabWidget_detail,
    		SIGNAL(currentChanged(int)),
			this,
            SLOT(handleCurrentChanged(int)));

    connect(actionAfficherPDF,
            SIGNAL(triggered()),
            this,
            SLOT(imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()));

    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this,
            SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
}


void YerithAdminDetailWindow::setupLineEdits()
{
	//FINANCIAL EXPENSE WIDGET SETUP
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


    lineEdit_detail_utilisateur_prenom->setYerithEnabled(false);
    lineEdit_detail_utilisateur_nom->setYerithEnabled(false);
    lineEdit_detail_utilisateur_lieu_naissance->setYerithEnabled(false);
    lineEdit_detail_utilisateur_email->setYerithEnabled(false);
    lineEdit_detail_utilisateur_ville->setYerithEnabled(false);
    lineEdit_detail_utilisateur_province_etat->setYerithEnabled(false);
    lineEdit_detail_utilisateur_pays->setYerithEnabled(false);
    lineEdit_detail_utilisateur_boite_postale->setYerithEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_1->setYerithEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_2->setYerithEnabled(false);
    lineEdit_detail_utilisateur_id->setYerithEnabled(false);
    lineEdit_detail_utilisateur_mot_passe->setYerithEnabled(false);


    lineEdit_detail_departements_de_produits_nom->setYerithEnabled(false);

    lineEdit_detail_categorie_nom_departement_produit->setYerithEnabled(false);

    lineEdit_detail_categorie_nom->setYerithEnabled(false);

    lineEdit_detail_INTITULE_de_la_LIGNE_BUDGETAIRE->setYerithEnabled(false);

    lineEdit_detail_montant_de_la_LIGNE_BUDGETAIRE->setYerithEnabled(false);

    lineEdit_detail_montant_RESTANT_de_la_LIGNE_BUDGETAIRE->setYerithEnabled(false);

    lineEdit_detail_INTITULE_DU_COMPTE_BANCAIRE->setYerithEnabled(false);


    lineEdit_detail_compte_bancaire_reference_du_compte_bancaire
		->setYerithEnabled(false);

    lineEdit_detail_compte_bancaire_intitule_du_compte_bancaire
		->setYerithEnabled(false);

    lineEdit_detail_compte_bancaire_institut_bancaire
		->setYerithEnabled(false);


    lineEdit_detail_localisation_adresse_ip->setYerithEnabled(false);
    lineEdit_detail_localisation_nom->setYerithEnabled(false);
    lineEdit_detail_localisation_numero_unique->setYerithEnabled(false);
    lineEdit_detail_localisation_quartier->setYerithEnabled(false);
    lineEdit_detail_localisation_ville->setYerithEnabled(false);
    lineEdit_detail_localisation_province_etat->setYerithEnabled(false);
    lineEdit_detail_localisation_pays->setYerithEnabled(false);
    lineEdit_detail_localisation_boite_postale->setYerithEnabled(false);
    lineEdit_detail_localisation_email->setYerithEnabled(false);
    lineEdit_detail_localisation_numero_telephone_1->setYerithEnabled(false);
    lineEdit_detail_localisation_numero_telephone_2->setYerithEnabled(false);

    lineEdit_detail_alerte_designation_article->setYerithEnabled(false);
    lineEdit_detail_alerte_quantite->setYerithEnabled(false);
}


void YerithAdminDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

    pushButton_detail_charges_financieres_CREER->disable(this);
}


void YerithAdminDetailWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

    pushButton_detail_charges_financieres_CREER->disable(this);
}


void YerithAdminDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

    pushButton_detail_charges_financieres_CREER
		->enable(this, SLOT(SLOT_CREER_A_PARTIR_DE_CECI()));
}


void YerithAdminDetailWindow::creer()
{
    _allWindows->_adminCreateWindow
        ->rendreVisible(_curStocksTableModel,
                        tabWidget_detail->currentIndex());
    rendreInvisible();
}


void YerithAdminDetailWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
    rendreInvisible();
}


void YerithAdminDetailWindow::modifier()
{
    _logger->log("modifier");
    switch (tabWidget_detail->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_REMISE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_ALERTE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	_allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }

    rendreInvisible();
}


void YerithAdminDetailWindow::supprimer()
{
    _logger->log("supprimer");
    _allWindows->_adminListerWindow->supprimer();
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
    rendreInvisible();
}


void YerithAdminDetailWindow::rendreInvisible()
{
    _logger->log("rendreInvisible");
    lineEdit_detail_alerte_id_destinataire->clear();
    lineEdit_detail_alerte_nom_destinataire->clear();
    lineEdit_detail_alerte_designation_article->clear();
    clear_set_edit_comboBoxes();
    setVisible(false);
}


bool YerithAdminDetailWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");


    QString latexFileNamePrefix("yerith-erp-ADMINISTRATION-BON-DE-COMMANDE");


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        latexFileNamePrefix.clear();
        latexFileNamePrefix.append("yerith-erp-ADMINISTRATION-ORDER-SHEET");
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


void YerithAdminDetailWindow::handleCurrentChanged(int index)
{
	_allWindows->_adminWindow->_curAdminSujetAction =
			(enum AdminSujetAction) tabWidget_detail->currentIndex();

    switch (tabWidget_detail->currentIndex())
    {
    case SUJET_ACTION_CHARGE_FINANCIERE:

    	disconnect(actionAfficherPDF, 0, 0, 0);

        connect(actionAfficherPDF,
                SIGNAL(triggered()),
                this,
                SLOT(imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()));

        break;

    default:
        YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
        disconnect(actionAfficherPDF, 0, 0, 0);

        break;
    }
}


void YerithAdminDetailWindow::enableOtherTabs(enum AdminSujetAction curAction,
                                              bool enabled)
{
    tabWidget_detail->setTabEnabled(curAction, !enabled);
    for (int k = 0; k <= 7; ++k)
    {
        if (k != curAction)
        {
            tabWidget_detail->setTabEnabled(k, enabled);
        }
    }
}


void YerithAdminDetailWindow::rendreVisibleCompteUtilisateur(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ utilisateurs"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_COMPTE_UTILISATEUR);

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *usersSqlTableModel = lw->getCurSearchSqlTableModel();

    if (!usersSqlTableModel)
    {
        usersSqlTableModel = &_allWindows->getSqlTableModel_users();
    }
    else if (usersSqlTableModel &&
             !YerithUtils::isEqualCaseInsensitive(usersSqlTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::USERS))
    {
        usersSqlTableModel = &_allWindows->getSqlTableModel_users();
    }

    //YerithSqlTableModel &usersSqlTableRecord = _allWindows->getSqlTableModel_users();

    QSqlRecord record = usersSqlTableModel->record(sqlTableRow);

    lineEdit_detail_utilisateur_prenom
		->setText(GET_SQL_RECORD_DATA(record,
				  YerithDatabaseTableColumn::PRENOM));

    lineEdit_detail_utilisateur_nom
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerithDatabaseTableColumn::NOM));

    comboBox_detail_utilisateur_titre
		->addItem(YerithUtils::_titreToUserViewString
					.value(GET_SQL_RECORD_DATA(record,
											   YerithDatabaseTableColumn::TITRE).toInt()));

    lineEdit_detail_utilisateur_lieu_naissance
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::LIEU_NAISSANCE));

    dateEdit_detail_utilisateur_date_naissance
		->setDate(GET_DATE_FROM_STRING
					(GET_SQL_RECORD_DATA(record,
										 YerithDatabaseTableColumn::DATE_NAISSANCE)));

    lineEdit_detail_utilisateur_email->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::EMAIL));
    lineEdit_detail_utilisateur_pays->setText(GET_SQL_RECORD_DATA
                                              (record,
                                               YerithDatabaseTableColumn::PAYS));
    lineEdit_detail_utilisateur_ville->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::VILLE));
    lineEdit_detail_utilisateur_province_etat->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerithDatabaseTableColumn::PROVINCE_ETAT));
    lineEdit_detail_utilisateur_boite_postale->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerithDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_detail_utilisateur_numero_telephone_1->setText(GET_SQL_RECORD_DATA
                                                            (record,
                                                             YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_utilisateur_numero_telephone_2->setText(GET_SQL_RECORD_DATA
                                                            (record,
                                                             YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));

    comboBox_detail_utilisateur_role->
    addItem(YerithUtils::_roleToUserViewString.
            value(GET_SQL_RECORD_DATA
                  (record, YerithDatabaseTableColumn::ROLE).toInt()));

    lineEdit_detail_utilisateur_id->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::NOM_UTILISATEUR));

    lineEdit_detail_utilisateur_mot_passe->setText("****");

    lineEdit_detail_utilisateur_localisation->setYerithEnabled(false);

    lineEdit_detail_utilisateur_localisation->
    setText(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    enableOtherTabs(SUJET_ACTION_COMPTE_UTILISATEUR, false);

    setVisible(true);
}


void YerithAdminDetailWindow::rendreVisibleLocalisation(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ localisations"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_LOCALISATION);

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *localisationSqlTableModel =
                    lw->getCurSearchSqlTableModel();

    if (!localisationSqlTableModel)
    {
        localisationSqlTableModel =
                        &_allWindows->getSqlTableModel_localisations();
    }
    else if (localisationSqlTableModel
             &&
             !YerithUtils::
             isEqualCaseInsensitive(localisationSqlTableModel->sqlTableName(),
                                    YerithDatabase::LOCALISATIONS))
    {
        localisationSqlTableModel =
                        &_allWindows->getSqlTableModel_localisations();
    }

    //YerithSqlTableModel &localisationSqlTableModel = _allWindows->getSqlTableModel_localisations();
    QSqlRecord record = localisationSqlTableModel->record(sqlTableRow);

    lineEdit_detail_localisation_adresse_ip->setText(GET_SQL_RECORD_DATA
                                                     (record, "adresse_ip"));
    lineEdit_detail_localisation_nom->setText(GET_SQL_RECORD_DATA
                                              (record, "nom_localisation"));
    lineEdit_detail_localisation_numero_unique->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         "numero_unique"));
    lineEdit_detail_localisation_quartier->setText(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerithDatabaseTableColumn::QUARTIER));
    lineEdit_detail_localisation_ville->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::VILLE));
    lineEdit_detail_localisation_province_etat->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         "province_etat"));
    lineEdit_detail_localisation_pays->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::PAYS));
    lineEdit_detail_localisation_boite_postale->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerithDatabaseTableColumn::BOITE_POSTALE));

    dateEdit_detail_localisation_date_ouverture->setDate(QDate::fromString
                                                         (GET_SQL_RECORD_DATA
                                                          (record,
                                                           "date_ouverture"),
                                                          YerithUtils::
                                                          DATE_FORMAT));

    lineEdit_detail_localisation_email->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::EMAIL));
    lineEdit_detail_localisation_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_localisation_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_detail_localisation_base_donnees->setText(GET_SQL_RECORD_DATA
                                                       (record, "dbms"));
    textEdit_detail_localisation_description_lieu->setText(GET_SQL_RECORD_DATA
                                                           (record,
                                                            "description_lieu"));

    enableOtherTabs(SUJET_ACTION_LOCALISATION, false);

    setVisible(true);
}


void YerithAdminDetailWindow::rendreVisibleDepartementsDeProduits(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ départements"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *departementsDeProduitsTableModel =
                    lw->getCurSearchSqlTableModel();

    if (0 == departementsDeProduitsTableModel)
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }
    else if (0 != departementsDeProduitsTableModel
             &&
             !YerithUtils::isEqualCaseInsensitive
             (departementsDeProduitsTableModel->sqlTableName(),
              YerithDatabase::DEPARTEMENTS_PRODUITS))
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }

    QSqlRecord record = departementsDeProduitsTableModel->record(sqlTableRow);

    lineEdit_detail_departements_de_produits_nom->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerithDatabaseTableColumn::
                                                           NOM_DEPARTEMENT_PRODUIT));

    textEdit_detail_departements_de_produits_description->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT));

    enableOtherTabs(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS, false);

    setVisible(true);
}


void YerithAdminDetailWindow::rendreVisibleCategorie(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ catégories"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_CATEGORIE);


    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;


    YerithSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();


    if (!categoriesTableModel)
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }
    else if (categoriesTableModel &&
    		 !YerithUtils::isEqualCaseInsensitive(categoriesTableModel->sqlTableName(),
                                                  YerithDatabase::CATEGORIES))
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }


    QSqlRecord record = categoriesTableModel->record(sqlTableRow);

    lineEdit_detail_categorie_nom_departement_produit
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    lineEdit_detail_categorie_nom
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerithDatabaseTableColumn::NOM_CATEGORIE));

    textEdit_detail_categorie_description
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE));

    enableOtherTabs(SUJET_ACTION_CATEGORIE, false);

    setVisible(true);
}


void YerithAdminDetailWindow::rendreVisibleLigneBudgetaire(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ lignes budgétaires"));

    setWindowTitle(_windowName);

	tabWidget_detail->setCurrentIndex(SUJET_ACTION_ligne_budgetaire);

	YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

	YerithSqlTableModel *ligneBudgetaireTableModel =
			lw->getCurSearchSqlTableModel();

	if (!ligneBudgetaireTableModel)
	{
		ligneBudgetaireTableModel =
				&_allWindows->getSqlTableModel_lignes_budgetaires();
	}
	else if (ligneBudgetaireTableModel				&&
			!YerithUtils::isEqualCaseInsensitive(ligneBudgetaireTableModel->sqlTableName(),
												 YerithDatabase::LIGNES_BUDGETAIRES))
	{
		ligneBudgetaireTableModel =
				&_allWindows->getSqlTableModel_lignes_budgetaires();
	}

	QSqlRecord record = ligneBudgetaireTableModel->record(sqlTableRow);

	lineEdit_detail_INTITULE_de_la_LIGNE_BUDGETAIRE->setText
	(GET_SQL_RECORD_DATA
			(record, YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire));

	lineEdit_detail_montant_de_la_LIGNE_BUDGETAIRE
		->setText(GET_DOUBLE_STRING(
				GET_SQL_RECORD_DATA(record,
									YerithDatabaseTableColumn::montant_de_la_ligne_budgetaire).toDouble()));

	lineEdit_detail_montant_RESTANT_de_la_LIGNE_BUDGETAIRE
		->setText(GET_DOUBLE_STRING(
				GET_SQL_RECORD_DATA(record,
									YerithDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire).toDouble()));

	lineEdit_detail_INTITULE_DU_COMPTE_BANCAIRE->setText
	(GET_SQL_RECORD_DATA
			(record, YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

	textEdit_detail_dune_LIGNE_budgetaire->setText
	(GET_SQL_RECORD_DATA
			(record, YerithDatabaseTableColumn::DESCRIPTION_ligne_budgetaire));

	enableOtherTabs(SUJET_ACTION_ligne_budgetaire, false);

	setVisible(true);
}


void YerithAdminDetailWindow::rendreVisibleCompteBancaire(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ comptes bancaires"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_COMPTE_BANCAIRE);

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *comptesBancairesTableModel =
                    lw->getCurSearchSqlTableModel();

    if (!comptesBancairesTableModel)
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (comptesBancairesTableModel
             &&
             !YerithUtils::isEqualCaseInsensitive
             (comptesBancairesTableModel->sqlTableName(),
              YerithDatabase::COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record = comptesBancairesTableModel->record(sqlTableRow);

    lineEdit_detail_compte_bancaire_reference_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));

    lineEdit_detail_compte_bancaire_intitule_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    lineEdit_detail_compte_bancaire_institut_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::INSTITUT_BANCAIRE));

    textEdit_detail_compte_bancaire_description_du_compte->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE));

    enableOtherTabs(SUJET_ACTION_COMPTE_BANCAIRE, false);

    setVisible(true);
}


void YerithAdminDetailWindow::rendreVisibleRemise(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ remises"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_REMISE);

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *remiseTableModel = lw->getCurSearchSqlTableModel();

    if (!remiseTableModel)
    {
        remiseTableModel = &_allWindows->getSqlTableModel_remises();
    }
    else if (remiseTableModel &&
             !YerithUtils::isEqualCaseInsensitive(remiseTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::REMISES))
    {
        remiseTableModel = &_allWindows->getSqlTableModel_remises();
    }

    QSqlRecord record = remiseTableModel->record(sqlTableRow);

    lineEdit_detail_remise_nom->setYerithEnabled(false);
    lineEdit_detail_remise_designation_article->setYerithEnabled(false);
    lineEdit_detail_remise_montant->setYerithEnabled(false);

    spinBox_detail_remise_pourcentage->setEnabled(false);

    dateEdit_detail_remise_date_debut->setYerithEnabled(false);
    dateEdit_detail_remise_date_fin->setYerithEnabled(false);

    textEdit_detail_remise_message->setYerithEnabled(false);

    lineEdit_detail_remise_nom->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::DESIGNATION_REMISE));
    lineEdit_detail_remise_designation_article->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerithDatabaseTableColumn::DESIGNATION));

    spinBox_detail_remise_pourcentage->setValue(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::REMISE_POURCENTAGE).toDouble
                                                ());

    QString
    date_debut(GET_SQL_RECORD_DATA
               (record, YerithDatabaseTableColumn::DATE_DEBUT));
    dateEdit_detail_remise_date_debut->setDate(GET_DATE_FROM_STRING
                                               (date_debut));

    QString
    date_fin(GET_SQL_RECORD_DATA
             (record, YerithDatabaseTableColumn::DATE_FIN));
    dateEdit_detail_remise_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));

    textEdit_detail_remise_message->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::REMISE_NOTES));

    enableOtherTabs(SUJET_ACTION_REMISE, false);
    setVisible(true);
}


void YerithAdminDetailWindow::rendreVisible_CHARGE_FINANCIERE(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ CHARGE FINANCIÈRE"));

    setWindowTitle(_windowName);

    YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_CHARGE_FINANCIERE);


    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *CHARGES_FINANCIERES_TableModel = lw->getCurSearchSqlTableModel();

    if (0 == CHARGES_FINANCIERES_TableModel)
    {
        CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_charges_financieres();
    }
    else if ((0 != CHARGES_FINANCIERES_TableModel) &&
             !YerithUtils::isEqualCaseInsensitive(CHARGES_FINANCIERES_TableModel->sqlTableName(),
                                                  YerithDatabase::CHARGES_FINANCIERES))
    {
        CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_charges_financieres();
    }


    QSqlRecord record = CHARGES_FINANCIERES_TableModel->record(sqlTableRow);

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
        if (currentUser->isManager())
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



    enableOtherTabs(SUJET_ACTION_CHARGE_FINANCIERE, false);


    setVisible(true);
}


void YerithAdminDetailWindow::rendreVisibleAlerte(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ alertes"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_ALERTE);

    YerithAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerithSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == alertesTableModel)
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }
    else if ((0 != alertesTableModel) &&
             !YerithUtils::isEqualCaseInsensitive(alertesTableModel->
                                                  sqlTableName(),
                                                  YerithDatabase::ALERTES))
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record = alertesTableModel->record(sqlTableRow);

    lineEdit_detail_alerte_designation_alerte->setYerithEnabled(false);
    lineEdit_detail_alerte_id_destinataire->setYerithEnabled(false);
    lineEdit_detail_alerte_nom_destinataire->setYerithEnabled(false);
    lineEdit_detail_alerte_designation_article->setYerithEnabled(false);

    lineEdit_detail_alerte_designation_alerte->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerithDatabaseTableColumn::DESIGNATION_ALERTE));

    lineEdit_detail_alerte_id_destinataire->setText(GET_SQL_RECORD_DATA
                                                    (record,
                                                     YerithDatabaseTableColumn::DESTINATAIRE));

    lineEdit_detail_alerte_nom_destinataire->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerithDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));

    lineEdit_detail_alerte_designation_article->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerithDatabaseTableColumn::DESIGNATION));

    comboBox_detail_alerte_condition->addItem(GET_SQL_RECORD_DATA
                                              (record,
                                               YerithDatabaseTableColumn::CONDITION_ALERTE));

    double quantite = GET_SQL_RECORD_DATA(record,
                                          YerithDatabaseTableColumn::
                                          QUANTITE).toDouble();

    if (quantite > -1)
    {
        radioButton_detail_alerte_quantite->setChecked(true);

        lineEdit_detail_alerte_quantite->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerithDatabaseTableColumn::QUANTITE));
    }
    else
    {
        radioButton_detail_alerte_date_periode_temps->setChecked(true);

        QString
        date_debut(GET_SQL_RECORD_DATA
                   (record, YerithDatabaseTableColumn::DATE_DEBUT));
        dateEdit_detail_alerte_date_debut->setDate(GET_DATE_FROM_STRING
                                                   (date_debut));

        QString
        date_fin(GET_SQL_RECORD_DATA
                 (record, YerithDatabaseTableColumn::DATE_FIN));
        dateEdit_detail_alerte_date_fin->setDate(GET_DATE_FROM_STRING
                                                 (date_fin));
    }

    textEdit_detail_alerte_message->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::MESSAGE_ALERTE));

    enableOtherTabs(SUJET_ACTION_ALERTE, false);

    setVisible(true);
}


void YerithAdminDetailWindow::retourListerCompteUtilisateur()
{
    _allWindows->_adminListerWindow->
    rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
    rendreInvisible();
}


void YerithAdminDetailWindow::retourListerLocalisation()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
    rendreInvisible();
}


void YerithAdminDetailWindow::retourListerDepartementsDeProduits()
{
    _allWindows->_adminListerWindow->
    rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
    rendreInvisible();
}


void YerithAdminDetailWindow::retourListerCategorie()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
    rendreInvisible();
}


void YerithAdminDetailWindow::retourLister_LIGNE_BUDGETAIRE()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
    rendreInvisible();
}


void YerithAdminDetailWindow::retourListerCompteBancaire()
{
    _allWindows->_adminListerWindow->
    rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
    rendreInvisible();
}

void YerithAdminDetailWindow::retourListerRemise()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_REMISE);
    rendreInvisible();
}

void YerithAdminDetailWindow::retourListerChargeFinanciere()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CHARGE_FINANCIERE);
    rendreInvisible();
}

void YerithAdminDetailWindow::retourListerAlerte()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);
    rendreInvisible();
}

void YerithAdminDetailWindow::clear_set_edit_utilisateur_comboBoxes()
{
    comboBox_detail_utilisateur_role->clear();
    comboBox_detail_utilisateur_titre->clear();
}

void YerithAdminDetailWindow::clear_set_edit_alerte_comboBoxes()
{
    comboBox_detail_alerte_condition->clear();
}

void YerithAdminDetailWindow::clear_set_edit_comboBoxes()
{
    clear_set_edit_utilisateur_comboBoxes();
    clear_set_edit_alerte_comboBoxes();
}
