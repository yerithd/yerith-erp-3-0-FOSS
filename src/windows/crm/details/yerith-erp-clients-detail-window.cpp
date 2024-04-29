/*
 * yerith-erp-clients-detail-window.cpp
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-clients-detail-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/widgets/yerith-erp-progress-bar.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


const int YerithClientsDetailWindow::TRUNCATE_POS_FOR_NOM_ENTREPRISE = 23;


YerithClientsDetailWindow::YerithClientsDetailWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithClientsDetailWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("détails d'un client"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
                        .arg(COLOUR_RGB_STRING_YERITH_GOLD_245_194_17,
                             COLOUR_RGB_STRING_YERITH_BLUE_53_132_228);

    setupLineEdits();

    textEdit_client_details_description_du_client->setYerithEnabled(false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGenererCARTE_DE_FIDELITE_ClIENTS, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_clients->disable(this);
    pushButton_groupes_du_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_compteclient->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionGenererCARTE_DE_FIDELITE_ClIENTS, SIGNAL(triggered()),
            this,
            SLOT(YERITH_PROGRESS_BAR_generer_la_carte_de_fidelite_du_client
                 ()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionModifierCompteClient, SIGNAL(triggered()), this,
            SLOT(modifierCompteClient()));
    connect(actionSupprimerCompteClient, SIGNAL(triggered()), this,
            SLOT(supprimerCompteClient()));
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


bool YerithClientsDetailWindow::generer_la_carte_de_fidelite_du_client()
{
    _logger->log("generer_la_carte_de_fidelite_du_client");


    YerithInfoEntreprise::load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO
    ();


    QString
    yerithCustomerAccountImageTmpFile(QString("%1JPG").arg
                                      (YerithUtils::
                                       getUniquePrefixFileInTemporaryFilesDir
                                       ("yerith")));

    const QPixmap *label_image_produit_pixmap = label_image_produit->pixmap();

    if (0 != label_image_produit_pixmap)
    {
        YerithUtils::savePixmapToFile(yerithCustomerAccountImageTmpFile,
                                      *label_image_produit_pixmap, "JPG");
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(7);


    QString latexFileNamePrefix("yerith-erp-carte-de-fidelite-client-LOYALTY");

#ifdef YERITH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yerith-erp-client-LOYALTY-program-card");
#endif

    QString texDocument;

    YerithUtils::getLatexCLIENT_LOYALTY_CARD_template(texDocument);


    texDocument.replace("YERITHCARTEDEFIDELITELOGO",
                        YerithERPConfig::pathToPdfCOMPANY_LOGO);


    if (0 != label_image_produit_pixmap)
    {
        texDocument.replace("YERITHCHEMINCOMPLETIMAGECOMPTECLIENT",
                            yerithCustomerAccountImageTmpFile);
    }
    else
    {
        texDocument.replace("YERITHCHEMINCOMPLETIMAGECOMPTECLIENT", "");
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);


    texDocument.replace("YERITHNOMDUPROGRAMMEDEFIDELITEUN",
                        YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (_curClientGroups.at(0).toLower().trimmed()));


    YerithInfoEntreprise &infoEntreprise =
                    YerithUtils::getAllWindows()->getInfoEntreprise();

    QString
    fileDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
             (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YERITHENTREPRISE",
                        infoEntreprise.
                        get_CARTE_DE_FIDELITE_nom_entreprise_FOR_LATEX());

    texDocument.replace("YERITHNOMDUCLIENT",
                        QString("%1").arg
                        (lineEdit_clients_details_nom_entreprise->text_LATEX
                         (TRUNCATE_POS_FOR_NOM_ENTREPRISE)));

    texDocument.replace("YERITHNOMDUPROGRAMMEDEFIDELITE", "");

    texDocument.replace("YERITHCLIENTNUMEROCOMPTE",
                        YerithUtils::
                        GET_STRING_WITH_NUMBERS_WITHIN_KEPT_FOR_LATEX
                        (lineEdit_clients_details_reference_client->text()));

    texDocument.replace("YERITHCLIENTRUEOUQUARTIER",
                        YerithUtils::
                        GET_STRING_WITH_NUMBERS_WITHIN_KEPT_FOR_LATEX
                        (lineEdit_clients_details_quartier->text_LATEX()));

    texDocument.replace("YERITHCLIENTVILLE",
                        lineEdit_clients_details_ville->text_LATEX());

    texDocument.replace("YERITHCLIENTETATOUPROVINCE",
                        lineEdit_clients_details_province_etat->text_LATEX());

    texDocument.replace("YERITHCLIENTPAYS",
                        lineEdit_clients_details_pays->text_LATEX());

    texDocument.replace("YERITHSERVICEGESTIONRELATIONCLIENTELLE",
                        infoEntreprise.
                        get_CARTE_DE_FIDELITE_service_de_gestion_clientele_FOR_LATEX
                        ());

    texDocument.replace("YERITHSERVICEDUPROGRAMMEDEFIDELITECLIENTS",
                        infoEntreprise.
                        get_CARTE_DE_FIDELITE_service_du_programme_de_fidelite_clients_FOR_LATEX
                        ());

    texDocument.replace("YERITHSIEGEDUSERVICEGESTIONRELATIONCLIENTELLE",
                        infoEntreprise.get_CARTE_DE_FIDELITE_siege_FOR_LATEX
                        ());

    texDocument.replace("YERITHEMAILDUSERVICEGESTIONRELATIONCLIENTELLE",
                        infoEntreprise.get_CARTE_DE_FIDELITE_email_FOR_LATEX
                        ());

    texDocument.replace("YERITHTELEPHONEDUSERVICEGESTIONRELATIONCLIENTELLE",
                        infoEntreprise.
                        get_CARTE_DE_FIDELITE_telephone_FOR_LATEX());

    QString yerithPrefixFileName;

    yerithPrefixFileName.append
    (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
     (latexFileNamePrefix));

    //qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex").arg(yerithPrefixFileName));

    emit SIGNAL_INCREMENT_PROGRESS_BAR(48);

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(96);

    QString pdfCustomerDataFileName(YerithERPProcess::compileWITH_LUATEX_Latex
                                    (yerithPrefixFileName));

    if (pdfCustomerDataFileName.isEmpty())
    {
        return false;
    }

//    YERITH_DELETE_FREE_POINTER_NOW(a_label_company_logo_pixmap)


    YerithERPProcess::startPdfViewerProcess(pdfCustomerDataFileName);


    emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

    return true;
}


void YerithClientsDetailWindow::afficher_groupes_dun_client()
{
    rendreInvisible();

    _allWindows->_groupesDunClientWindow->rendreVisible(_curClientTableModel,
                                                        _curStocksTableModel);
}


void YerithClientsDetailWindow::private_payer_au_compteclient()
{
    rendreInvisible();

    _allWindows->_payerAuCompteClientWindow->rendreVisible(_curClientTableModel, _curStocksTableModel);
}


void YerithClientsDetailWindow::modifierCompteClient()
{
    rendreInvisible();

    bool show_TabDESCRIPTION_CLIENT = (1 == tabWidget_details_client->currentIndex());

    _allWindows->_modifierCompteClientWindow->rendreVisible(_curClientTableModel,
    														_curStocksTableModel,
															show_TabDESCRIPTION_CLIENT);
}


void YerithClientsDetailWindow::supprimerCompteClient()
{
    QSqlRecord record;

    _allWindows->_clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression du compte client '%1' ?").
                         arg(GET_SQL_RECORD_DATA
                             (record,
                              YerithDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression d'un compte client"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool resRemoved =
                        _allWindows->
                        _clientsWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW();

        //qDebug() << "YerithModifierWindow::supprimer_ce_stock() " << resRemoved;
        clients();
        if (resRemoved)
        {
            msgSupprimer =
                            QObject::tr("Le compte client '%1' a été supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("supprimer - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("Le compte client '%1' ne pouvait pas être supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("supprimer - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerithClientsDetailWindow::setupLineEdits()
{
    lineEdit_clients_details_reference_registre_du_commerce->setYerithEnabled
    (false);
    lineEdit_clients_details_reference_client->setYerithEnabled(false);
    lineEdit_clients_details_nom_entreprise->setYerithEnabled(false);
    lineEdit_clients_details_nom_representant->setYerithEnabled(false);
    lineEdit_clients_details_quartier->setYerithEnabled(false);
    lineEdit_clients_details_ville->setYerithEnabled(false);
    lineEdit_clients_details_province_etat->setYerithEnabled(false);
    lineEdit_clients_details_pays->setYerithEnabled(false);
    lineEdit_clients_details_boite_postale->setYerithEnabled(false);
    lineEdit_clients_details_siege_social->setYerithEnabled(false);
    lineEdit_clients_details_email->setYerithEnabled(false);
    lineEdit_clients_details_numero_telephone_1->setYerithEnabled(false);
    lineEdit_clients_details_numero_telephone_2->setYerithEnabled(false);
    lineEdit_clients_details_refereur->setYerithEnabled(false);
    lineEdit_clients_details_numero_contribuable->setYerithEnabled(false);
    lineEdit_clients_details_dette_maximale->setYerithEnabled(false);
    lineEdit_clients_details_compte_client->setYerithEnabled(false);
    lineEdit_clients_details_compte_client_AVANCE_DE_CASH->setYerithEnabled
    (false);
}


void YerithClientsDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGenererCARTE_DE_FIDELITE_ClIENTS, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_groupes_du_client->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_payer_au_compteclient->disable(this);
}

void YerithClientsDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole");

    definirPasDeRole();
}

void YerithClientsDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGenererCARTE_DE_FIDELITE_ClIENTS, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_groupes_du_client->enable(this,
                                         SLOT(afficher_groupes_dun_client
                                              ()));
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->enable(this, SLOT(supprimerCompteClient()));

    pushButton_payer_au_compteclient->enable(this,
                                             SLOT
                                             (private_payer_au_compteclient
                                              ()));

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif
}


void YerithClientsDetailWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGenererCARTE_DE_FIDELITE_ClIENTS, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_groupes_du_client->enable(this,
                                         SLOT(afficher_groupes_dun_client
                                              ()));
    pushButton_modifier->enable(this, SLOT(modifierCompteClient()));
    pushButton_supprimer->disable(this);

    pushButton_payer_au_compteclient->enable(this,
                                             SLOT
                                             (private_payer_au_compteclient
                                              ()));
}


void YerithClientsDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole");

    definirPasDeRole();
}

void YerithClientsDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole");

    definirPasDeRole();
}


bool YerithClientsDetailWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");

    QString yerithCustomerAccountImage("yerith");

    QString
    yerithCustomerAccountImageTmpFile(QString("%1JPG").arg
                                      (YerithUtils::
                                       getUniquePrefixFileInTemporaryFilesDir
                                       (yerithCustomerAccountImage)));

    const QPixmap *label_image_produit_pixmap = label_image_produit->pixmap();

    if (0 != label_image_produit_pixmap)
    {
        YerithUtils::savePixmapToFile(yerithCustomerAccountImageTmpFile,
                                      *label_image_produit_pixmap, "JPG");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);

    QString latexFileNamePrefix("yerith-erp-fiche-client");

#ifdef YERITH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yerith-erp-customer-account-file");
#endif

    QString texDocument;

    YerithUtils::getLatexCustomerData(texDocument);

    QString data;

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Référence client: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_reference_client->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Nom de l'entreprise -- client: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_nom_entreprise->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Référeur: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_refereur->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Nom du Représentant: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_nom_representant->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Quartier: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_quartier->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Ville: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_ville->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Province / État: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_province_etat->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text(QObject::tr("Pays: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_pays->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Boîte postale: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_boite_postale->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Siège social: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_siege_social->text_LATEX()));

    data.
    append(YerithUtils::get_latex_bold_text(QObject::tr("Émail: ")));
    data.append(QString("%1\\\\\n").
                arg(YerithUtils::
                    LATEX_IN_OUT_handleForeignAccents
                    (lineEdit_clients_details_email->text().toLower())));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Numéro de téléphone 1: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_numero_telephone_1->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Numéro de téléphone 2: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_numero_telephone_2->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Numéro de contribuable: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_numero_contribuable->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("RCCM N\\textsuperscript{o}: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_reference_registre_du_commerce->
                 text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Dette maximale: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_dette_maximale->text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Compte client (AVANCE DE CASH): ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_compte_client_AVANCE_DE_CASH->
                 text_LATEX()));

    data.append(YerithUtils::get_latex_bold_text
                (QObject::tr("Compte client: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_clients_details_compte_client->text_LATEX()));

    data.append("\n\n\\vspace{0.3cm}\n\n");

    data.append(QString("%1").arg
                (YerithUtils::get_latex_bold_text
                 (QObject::tr("DESCRIPTION CLIENT:"))));
    data.append("\n\n\\vspace{0.3cm}\n\n");

    texDocument.replace("YERITHDETAILSCOMPTECLIENT", data);

    data.clear();
    data.append(QString("%1\\\\").arg
                (textEdit_client_details_description_du_client->
                 toPlainTextForLatex()));

    texDocument.replace("YERITHDESCRIPTIONCOMPTECLIENT", data);

    if (0 != label_image_produit_pixmap)
    {
        texDocument.replace("YERITHCHEMINCOMPLETIMAGECOMPTECLIENT",
                            yerithCustomerAccountImageTmpFile);
    }
    else
    {
        texDocument.replace("YERITHCHEMINCOMPLETIMAGECOMPTECLIENT", "");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(24);

    YerithInfoEntreprise &infoEntreprise =
                    YerithUtils::getAllWindows()->getInfoEntreprise();

    QString
    fileDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
             (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YERITHPAPERSPEC", "a4paper");

    texDocument.replace("YERITHCLIENT",
                        QString("%1").arg
                        (lineEdit_clients_details_nom_entreprise->text_LATEX
                         ()));
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

    QString yerithPrefixFileName;

    yerithPrefixFileName.append
    (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
     (latexFileNamePrefix));

    //qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex").arg(yerithPrefixFileName));

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(92);

    QString
    pdfCustomerDataFileName(YerithERPProcess::compileLatex
                            (yerithPrefixFileName));

    if (pdfCustomerDataFileName.isEmpty())
    {
        return false;
    }

    YerithERPProcess::startPdfViewerProcess(pdfCustomerDataFileName);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

    return true;
}


void YerithClientsDetailWindow::rendreInvisible()
{
    lineEdit_clients_details_reference_registre_du_commerce->clear();
    lineEdit_clients_details_reference_client->clear();
    lineEdit_clients_details_nom_entreprise->clear();
    lineEdit_clients_details_nom_representant->clear();
    lineEdit_clients_details_quartier->clear();
    lineEdit_clients_details_ville->clear();
    lineEdit_clients_details_province_etat->clear();
    lineEdit_clients_details_pays->clear();
    lineEdit_clients_details_boite_postale->clear();
    lineEdit_clients_details_siege_social->clear();
    lineEdit_clients_details_email->clear();
    lineEdit_clients_details_numero_telephone_1->clear();
    lineEdit_clients_details_numero_telephone_2->clear();
    lineEdit_clients_details_refereur->clear();
    lineEdit_clients_details_numero_contribuable->clear();
    lineEdit_clients_details_dette_maximale->clear();
    lineEdit_clients_details_compte_client_AVANCE_DE_CASH->clear();
    lineEdit_clients_details_compte_client->clear();

    textEdit_client_details_description_du_client->clear();

    YerithUtils::supprimer_image(*label_image_produit);


    YerithWindowsCommons::rendreInvisible();
}


void YerithClientsDetailWindow::rendreVisible(YerithSqlTableModel *clientTableModel,
                                              YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curClientTableModel = clientTableModel;

    //qDebug() << "++ last selected row: " << YerithERPWindows::get_last_lister_selected_row_db_ID();

    setVisible(true);

    showClientDetail();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'un client"),
    								 _curClientTableModel);
}


void YerithClientsDetailWindow::showClientDetail()
{
    QSqlRecord record;

    _allWindows->_clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);


    QString groupes_du_client(
    			GET_SQL_RECORD_DATA(record,
    								YerithDatabaseTableColumn::GROUPES_DU_CLIENT));


    _curClientGroups.clear();

    _curClientGroups.append(groupes_du_client.split(YerithUtils::STAR_CHAR));


    lineEdit_clients_details_reference_client
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::REFERENCE_CLIENT));

    lineEdit_clients_details_reference_registre_du_commerce
		->setText(GET_SQL_RECORD_DATA(record,
									  YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

    lineEdit_clients_details_nom_entreprise->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerithDatabaseTableColumn::NOM_ENTREPRISE));

    lineEdit_clients_details_nom_representant->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerithDatabaseTableColumn::NOM_REPRESENTANT));

    lineEdit_clients_details_quartier->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::QUARTIER));

    lineEdit_clients_details_ville->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::VILLE));

    lineEdit_clients_details_province_etat->setText(GET_SQL_RECORD_DATA
                                                    (record,
                                                     YerithDatabaseTableColumn::PROVINCE_ETAT));

    lineEdit_clients_details_pays->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::PAYS));

    lineEdit_clients_details_boite_postale->setText(GET_SQL_RECORD_DATA
                                                    (record,
                                                     YerithDatabaseTableColumn::BOITE_POSTALE));

    lineEdit_clients_details_siege_social->setText(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerithDatabaseTableColumn::SIEGE_SOCIAL));

    lineEdit_clients_details_email->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::EMAIL));

    lineEdit_clients_details_numero_telephone_1->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

    lineEdit_clients_details_numero_telephone_2->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));

    lineEdit_clients_details_refereur->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::REFEREUR_CLIENT));

    lineEdit_clients_details_numero_contribuable->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerithDatabaseTableColumn::NUMERO_CONTRIBUABLE));

    double dette_maximale = GET_SQL_RECORD_DATA(record,
                                                YerithDatabaseTableColumn::
                                                DETTE_MAXIMALE_COMPTE_CLIENT).toDouble
                            ();

    lineEdit_clients_details_dette_maximale->setText(GET_CURRENCY_STRING_NUM
                                                     (dette_maximale));

    double compteclient_AVANCE_DE_CASH = GET_SQL_RECORD_DATA(record,
                                                             YerithDatabaseTableColumn::
                                                             COMPTECLIENT_AVANCE_DE_CASH).toDouble
                                         ();

    lineEdit_clients_details_compte_client_AVANCE_DE_CASH->setText
    (GET_CURRENCY_STRING_NUM(compteclient_AVANCE_DE_CASH));

    double compte_client = GET_SQL_RECORD_DATA(record,
                                               YerithDatabaseTableColumn::COMPTE_CLIENT).
                           toDouble();

    lineEdit_clients_details_compte_client->setText(GET_CURRENCY_STRING_NUM
                                                    (compte_client));

    textEdit_client_details_description_du_client->setText(GET_SQL_RECORD_DATA
                                                           (record,
                                                            YerithDatabaseTableColumn::DESCRIPTION_CLIENT));

    QVariant
    img(record.value(YerithDatabaseTableColumn::IMAGE_COMPTE_CLIENT));

    if (!img.isNull())
    {
        YerithUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        YerithUtils::supprimer_image(*label_image_produit);
    }
}


void YerithClientsDetailWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
}
