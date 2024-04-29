/*
 * yerith-erp-modifier-window.cpp
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-modifier-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"


#include <unistd.h>


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtGui/QContextMenuEvent>


YerithModifierWindow::YerithModifierWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithModifierWindow")),
 _montantTva(0.0),
 _IS_AN_ASSET(false),
 _tvaCheckBoxPreviousState(false),
 _tvaPercent(YerithUtils::getTvaStringWithPercent())
{
	_windowName = YerithMainWindow::get_TRANSLATED_WindowName("modifier (ré-approvisionner) un(e) stock / immobilisation");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    textEdit_description->setYerithEnabled(false);


    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);



    connect(checkBox_HISTORIQUE,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_HISTORIQUE_CHECKBOX(int)));


    connect(checkBox_achat, SIGNAL(stateChanged(int)), this,
            SLOT(handle_achat_checkBox(int)));

    connect(checkBox_re_approvisionnement, SIGNAL(stateChanged(int)),
            this, SLOT(handle_re_approvisionnement_checkBox(int)));

    connect(checkBox_tva, SIGNAL(clicked(bool)), this,
            SLOT(handleTVACheckBox(bool)));

    connect(lineEdit_prix_dachat,
            SIGNAL(textChanged(const QString &)),
            this,
            SLOT(calculate_and_display_ALL_benefit_buying_price_percentage
                 ()));

    connect(lineEdit_prix_vente, SIGNAL(textChanged(const QString &)), this,
            SLOT(edited_prix_vente(const QString &)));

    connect(lineEdit_prix_vente, SIGNAL(editingFinished()), this,
            SLOT(display_prix_vente()));

    connect(lineEdit_prix_vente_EN_GROS,
            SIGNAL(textChanged(const QString &)), this,
            SLOT(edited_prix_vente_EN_GROS(const QString &)));

    connect(lineEdit_prix_vente_EN_GROS, SIGNAL(editingFinished()), this,
            SLOT(display_prix_vente_EN_GROS()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionActualiser, SIGNAL(triggered()), this,
            SLOT(actualiser_stock()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionSupprimer, SIGNAL(triggered()), this,
            SLOT(supprimer_ce_stock()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this,
            SLOT(supprimer_image_stock()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
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

void YerithModifierWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithModifierWindow::handle_HISTORIQUE_CHECKBOX(int aState)
{
    if (checkBox_HISTORIQUE->isChecked())
    {
        checkBox_HISTORIQUE->setText(QObject::tr("historique actif"));
    }
    else
    {
        checkBox_HISTORIQUE->
        setText(QObject::tr("historique désactiver"));
    }
}


void YerithModifierWindow::handle_achat_checkBox(int aState)
{
    checkBox_achat->setReadOnly(!checkBox_re_approvisionnement->isChecked());

    if (checkBox_achat->isChecked())
    {
        if (checkBox_re_approvisionnement->isChecked())
        {
            lineEdit_prix_dachat->setYerithEnabled(true);
        }
        else
        {
            lineEdit_prix_dachat->setYerithEnabled(false);
        }
    }

    handleVisibilityPrixDachat(checkBox_achat->isChecked());

    calculate_and_display_benefit_buying_price_percentage();
}


void YerithModifierWindow::handle_re_approvisionnement_checkBox(int state)
{
    checkBox_achat->setReadOnly(!checkBox_re_approvisionnement->isChecked());

    if (checkBox_re_approvisionnement->isChecked())
    {
        checkBox_achat->setVisible(true);


        lineEdit_prix_dachat->setYerithEnabled(checkBox_achat->isChecked());

        handleVisibilityPrixDachat(checkBox_achat->isChecked());


        lineEdit_quantite_restante->setYerithEnabled(true);
        lineEdit_reference_recu_dachat->setYerithEnabled(true);
        lineEdit_nom_entreprise_fournisseur->setYerithEnabled(true);
    }
    else
    {
        int achatQuerySize =
                        YerithUtils::STOCK_PURCHASE_RECORDS_QUANTITY(_currentStockID);

        if (achatQuerySize > 0)
        {
            checkBox_achat->setVisible(true);
            checkBox_achat->setChecked(true);
        }
        else
        {
            checkBox_achat->setVisible(false);
            checkBox_achat->setChecked(false);
        }

        handleVisibilityPrixDachat(checkBox_achat->isChecked());

        lineEdit_prix_dachat->setText(_currentPrixDachat);

        lineEdit_prix_dachat->setYerithEnabled(false);

        lineEdit_quantite_restante->setYerithEnabled(false);
        lineEdit_reference_recu_dachat->setYerithEnabled(false);
        lineEdit_nom_entreprise_fournisseur->setYerithEnabled(false);
    }
}


void YerithModifierWindow::actualiser_stock()
{
    if (!check_fields())
    {
        return;
    }

    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_vente_EN_GROS =
                    lineEdit_prix_vente_EN_GROS->text().toDouble();

    double prix_dachat = lineEdit_prix_dachat->text().toDouble();

    if (!_IS_AN_ASSET
            && !YerithUtils::isProfitable(prix_vente, prix_dachat, _montantTva))
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("pas profitable"),
                                   QObject::tr
                                   ("Le prix de vente doit être "
                                    "supérieure ou égal au prix d'achat !"));

        return;
    }


    bool correctDatePeremption = true;

    if (dateEdit_date_peremption->date() <= GET_CURRENT_DATE)
    {
        QString warnMsg(QObject::tr
                        ("La date de péremption n'est pas postdatée !\n\n"
                         "Continuer avec l'actualisation des données d'1 immobilisation OU stock ?"));

        if (QMessageBox::Cancel ==
                YerithQMessageBox::question(this,
                                            QObject::tr
                                            ("actualiser les détails d'1 immobilisation OU stock"),
                                            warnMsg, QMessageBox::Cancel,
                                            QMessageBox::Ok))
        {
            correctDatePeremption = false;
        }
    }

    if (!correctDatePeremption)
    {
        return;
    }


    bool CONTINUE_AVEC_PRIX_DACHAT_ZERO = true;

    if (prix_dachat <= 0)
    {
        QString warnMsg(QObject::tr
                        ("Le prix d'achat est égal à zéro (0) %1 !\n\n"
                         "Continuer avec l'actualisation des données d'1 immobilisation OU stock ?").
                        arg(YerithERPConfig::currency));

        if (QMessageBox::Cancel ==
                YerithQMessageBox::question(this,
                                            QObject::tr
                                            ("actualiser les détails d'1 immobilisation OU stock"),
                                            warnMsg, QMessageBox::Cancel,
                                            QMessageBox::Ok))
        {
            CONTINUE_AVEC_PRIX_DACHAT_ZERO = false;
        }
    }

    if (!CONTINUE_AVEC_PRIX_DACHAT_ZERO)
    {
        return;
    }


    QString msgEnregistrer(QObject::tr("Poursuivre avec la "
                                           "modification (le ré-approvisionnement) "
                                           "d'1 immobilisation OU stock '%1' ?").
                           arg(lineEdit_designation->text()));

    if (QMessageBox::Ok !=
            YerithQMessageBox::question(this,
                                        QObject::tr("modification"),
                                        msgEnregistrer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        msgEnregistrer.clear();

        msgEnregistrer.append(QObject::tr
                              ("Vous avez annulé la modification "
                               "des détails d'1 immobilisation OU stock '%1' !").
                              arg(lineEdit_designation->text()));

        YerithQMessageBox::information(this, QObject::tr("annulation"),
                                       msgEnregistrer, QMessageBox::Ok);

        return;
    }

    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    description_produit(GET_SQL_RECORD_DATA
                        (record,
                         YerithDatabaseTableColumn::DESCRIPTION_PRODUIT));

    if (!YerithUtils::isEqualCaseInsensitive(description_produit,
                                             textEdit_description->toPlainText
                                             ()))
    {
        record.setValue(YerithDatabaseTableColumn::DESCRIPTION_PRODUIT,
                        textEdit_description->toPlainText());
    }

    if (checkBox_HISTORIQUE->isChecked())
    {
        record.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF, 1);
    }
    else
    {
        record.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF, 0);
    }

    record.setValue(YerithDatabaseTableColumn::LOCALISATION_STOCK,
                    lineEdit_localisation_produit->text());

    record.setValue(YerithDatabaseTableColumn::MONTANT_TVA, _montantTva);

    record.setValue(YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                    _montantTva_EN_GROS);


    double prix_unitaire_ht = prix_vente - _montantTva;

    double prix_unitaire_en_gros_ht = prix_vente_EN_GROS - _montantTva_EN_GROS;


    record.setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                    prix_unitaire_ht);

    record.setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                    prix_unitaire_en_gros_ht);

    record.setValue(YerithDatabaseTableColumn::PRIX_DACHAT, prix_dachat);


    if (!_IS_AN_ASSET)
    {
        record.setValue(YerithDatabaseTableColumn::PRIX_VENTE, prix_vente);

        record.setValue(YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                        prix_vente_EN_GROS);
    }

    record.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                    lineEdit_nom_entreprise_fournisseur->text());

    record.setValue(YerithDatabaseTableColumn::STOCK_DALERTE,
                    lineEdit_stock_dalerte->text().toDouble());

    record.setValue(YerithDatabaseTableColumn::DATE_PEREMPTION,
                    dateEdit_date_peremption->date());

    if (label_image_produit->pixmap())
    {
        QByteArray bytes;
        YerithUtils::savePixmapToByteArray(bytes,
                                           *label_image_produit->pixmap(),
                                           "JPG");
        record.setValue(YerithDatabaseTableColumn::IMAGE_PRODUIT, bytes);
    }

    double quantite_en_re_approvisionement = 0;

    if (checkBox_re_approvisionnement->isChecked())
    {
        double ancienne_quantite_totale = GET_SQL_RECORD_DATA(record,
                                                              YerithDatabaseTableColumn::QUANTITE_TOTALE).
                                          toDouble();

        double quantite_totale =
                        lineEdit_quantite_restante->text().toDouble();

        quantite_en_re_approvisionement =
                        quantite_totale - ancienne_quantite_totale;

        if (quantite_en_re_approvisionement > 0)
        {
            record.setValue(YerithDatabaseTableColumn::QUANTITE_TOTALE,
                            quantite_totale);

            record.setValue(YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT,
                            lineEdit_reference_recu_dachat->text());

            bool historique_stock_actif = GET_SQL_RECORD_DATA(record,
                                                              YerithDatabaseTableColumn::
                                                              HISTORIQUE_STOCK_ACTIF).toInt
                                          ();

            QString historiqueStock = GET_SQL_RECORD_DATA(record,
                                                          YerithDatabaseTableColumn::HISTORIQUE_STOCK);

            QString historiqueStockReApprovisionement;

            if (historique_stock_actif)
            {
                historiqueStockReApprovisionement =
                                YerithHistoriqueStock::creer_mouvement_stock
                                (MOUVEMENT_DE_STOCK_RE_APPROVISIONNEMENT, _currentStockID,
                                 GET_CURRENT_DATE, ancienne_quantite_totale,
                                 quantite_en_re_approvisionement, quantite_totale);
            }

            historiqueStock.append(YerithHistoriqueStock::SEPARATION_EXTERNE).
            append(historiqueStockReApprovisionement);

            record.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                            historique_stock_actif);

            record.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                            historiqueStock);
        }
        else
        {
            QString retMsg(QObject::tr
                           ("En mode ré-approvisionnement, la quantité "
                            "restante doit être supérieure à "
                            "l'ancienne quantité restante !"));

            YerithQMessageBox::warning(this, QObject::tr("échec"),
                                       retMsg);

            lineEdit_quantite_restante->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerithDatabaseTableColumn::
                                                 QUANTITE_TOTALE));

            return;
        }
    }

    bool success =
                    _allWindows->
                    _stocksWindow->SQL_UPDATE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    retMsg(QObject::tr
           ("Les détails d'1 immobilisation OU stock '%1").arg
           (lineEdit_designation->text()));

    if (success)
    {
        YerithERPStockMarchandiseData a_stock_data;

        if (!_IS_AN_ASSET)
        {
            a_stock_data._designation = lineEdit_designation->text();
            a_stock_data._prix_dachat_precedent = lineEdit_prix_dachat->text();
            a_stock_data._prix_vente_precedent = lineEdit_prix_vente->text();
            a_stock_data._prix_vente_en_gros_precedent =
                            lineEdit_prix_vente_EN_GROS->text();

            YerithUtils::UPDATE_PREVIOUS_SELLING_PRICE_IN_ProductList
            (a_stock_data, this);
        }

        if (checkBox_achat->isChecked())
        {
            if (!_IS_AN_ASSET)
            {
                YerithUtils::UPDATE_PREVIOUS_BUYING_PRICE_IN_ProductList
                (a_stock_data, this);
            }

            if (checkBox_re_approvisionnement->isChecked() &&
                    quantite_en_re_approvisionement > 0)
            {
                ajouter_nouveau_re_approvisionnement_achat
                (quantite_en_re_approvisionement, _currentStockID);
            }
        }

        update_achat_deja_existant(record,
                                   prix_unitaire_ht, prix_unitaire_en_gros_ht);


        retMsg.
        append(QObject::tr("' ont été actualisés avec succès !"));

        YerithQMessageBox::information(this, QObject::tr("succès"),
                                       retMsg);
    }
    else
    {
        retMsg.append(QObject::tr
                      ("' n'ont pas pu être actualisés avec succès !"));

        YerithQMessageBox::warning(this, QObject::tr("échec"), retMsg);
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);

    rendreInvisible();
}


void YerithModifierWindow::edited_prix_vente(const QString &newPrixVente)
{
    _lastEditedPrixVente = newPrixVente;

    calculate_and_display_benefit_buying_price_percentage();
}


void YerithModifierWindow::edited_prix_vente_EN_GROS(const QString &
                                                     newPrixVente_en_gros)
{
    _lastEditedPrixVente_EN_GROS = newPrixVente_en_gros;

    calculate_and_display_benefit_buying_price_percentage_EN_GROS();
}


void
YerithModifierWindow::
calculate_and_display_ALL_benefit_buying_price_percentage()
{
    calculate_and_display_benefit_buying_price_percentage();
    calculate_and_display_benefit_buying_price_percentage_EN_GROS();
}


void YerithModifierWindow::calculate_and_display_benefit_buying_price_percentage()
{
    if (checkBox_achat->isChecked())
    {
        double prix_dachat = lineEdit_prix_dachat->text().toDouble();
        double prix_vente = lineEdit_prix_vente->text().toDouble();
        //double montantTva = 0.0;

        if (YerithUtils::isProfitable(prix_vente, prix_dachat, _montantTva))
        {
            double pourcentage_benefice_prix_dachat =
                            POURCENTAGE_YERITH_GET_VALUE(YerithUtils::getMargeBeneficiaire
                                                         (prix_vente,
                                                          prix_dachat,
                                                          _montantTva),
                                                         prix_dachat);

            lineEdit_pourcentage_prix_dachat_prix_de_vente->setText(QString
                                                                    ("%1%").arg
                                                                    (QString::
                                                                     number
                                                                     (pourcentage_benefice_prix_dachat,
                                                                      'f',
                                                                      2)));
        }
        else
        {
            lineEdit_pourcentage_prix_dachat_prix_de_vente->setText("0.00 %");
        }
    }
    else
    {
        lineEdit_pourcentage_prix_dachat_prix_de_vente->setText("0.00 %");
    }
}


void
YerithModifierWindow::
calculate_and_display_benefit_buying_price_percentage_EN_GROS()
{
    if (checkBox_achat->isChecked())
    {
        double
        prix_dachat = lineEdit_prix_dachat->text().toDouble();

        double
        prix_vente_EN_GROS = lineEdit_prix_vente_EN_GROS->text().toDouble();

        if (YerithUtils::isProfitable
                (prix_vente_EN_GROS, prix_dachat, _montantTva_EN_GROS))
        {
            double
            pourcentage_benefice_prix_dachat_EN_GROS =
                            POURCENTAGE_YERITH_GET_VALUE(YerithUtils::getMargeBeneficiaire
                                                         (prix_vente_EN_GROS,
                                                          prix_dachat,
                                                          _montantTva_EN_GROS),
                                                         prix_dachat);

            lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText
            (QString("%1%").arg
             (QString::number
              (pourcentage_benefice_prix_dachat_EN_GROS, 'f', 2)));
        }
        else
        {
            lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText
            ("0.00 %");
        }
    }
    else
    {
        lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText
        ("0.00 %");
    }
}


void YerithModifierWindow::handleVisibilityPrixDachat(bool anEditable)
{
    label_prix_dachat->setVisible(anEditable);

    lineEdit_prix_dachat->setVisible(anEditable);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setVisible(anEditable);

    lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setVisible
    (anEditable);
}


void YerithModifierWindow::ajouter_nouveau_re_approvisionnement_achat(double
                                                                      quantiteEnReApprovisionement,
                                                                      int stockId)
{
    YerithSqlTableModel &achatSqlTableModel =
                    _allWindows->getSqlTableModel_achats();

    QSqlRecord nouveauAchatRecord = achatSqlTableModel.record();

    int next_achat_id = YerithERPWindows::getNextIdSqlTableModel_achats();

    QString utilisateurCourrantNomComplet;

    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        utilisateurCourrantNomComplet.append(aUser->nom_complet());
    }

    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::ID, next_achat_id);
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::STOCKS_ID, stockId);
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::REFERENCE,
                                lineEdit_reference_produit->text());
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::DESIGNATION,
                                lineEdit_designation->text());
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::CATEGORIE,
                                lineEdit_categorie_produit->text());
    nouveauAchatRecord.
    setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
             lineEdit_nom_departement_produit->text());

    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::ENREGISTREUR_STOCK,
                                utilisateurCourrantNomComplet);
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::QUANTITE_TOTALE,
                                quantiteEnReApprovisionement);

    QString reference_recu_dachat = lineEdit_reference_recu_dachat->text();

    nouveauAchatRecord.
    setValue(YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT,
             reference_recu_dachat);

    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_vente_en_gros =
                    lineEdit_prix_vente_EN_GROS->text().toDouble();

    double prix_dachat = lineEdit_prix_dachat->text().toDouble();

    if (prix_dachat < 0)
    {
        prix_dachat = 0;
    }

    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::PRIX_DACHAT,
                                prix_dachat);

    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::PRIX_VENTE,
                                prix_vente);
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                                prix_vente_en_gros);

    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::MONTANT_TVA,
                                _montantTva);
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                                _montantTva_EN_GROS);

    double prix_unitaire_ht = prix_vente - _montantTva;

    double prix_unitaire_en_gros_ht = prix_vente_en_gros - _montantTva_EN_GROS;


    if (!_IS_AN_ASSET)
    {
        double marge_beneficiaire =
                        YerithUtils::getMargeBeneficiaire(prix_vente, prix_dachat,
                                                          _montantTva);

        nouveauAchatRecord.
        setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE,
                 marge_beneficiaire);
    }
    else
    {
        nouveauAchatRecord.
        setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE, 0.0);
    }

    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                                prix_unitaire_ht);

    nouveauAchatRecord.
    setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
             prix_unitaire_en_gros_ht);

    nouveauAchatRecord.
    setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
             lineEdit_nom_entreprise_fournisseur->text());
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::LOCALISATION,
                                YerithERPConfig::THIS_SITE_LOCALISATION_NAME);
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::LOCALISATION_STOCK,
                                lineEdit_localisation_produit->text());
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::DATE_ENTREE,
                                GET_CURRENT_DATE);
    nouveauAchatRecord.setValue(YerithDatabaseTableColumn::DATE_PEREMPTION,
                                dateEdit_date_peremption->date());

    bool achatSuccess = achatSqlTableModel
    						.insertNewRecord(nouveauAchatRecord,
    										 this,
											 "src/windows/stocks/yerith-erp-modifier-window.cpp",
											 776);

    QString achatRetMsg(QObject::tr
                        ("1 nouveau achat a été enregistré dans la base de données !"));

    if (!achatSuccess)
    {
        achatRetMsg =
                        QObject::tr
                        ("Échec de l'enregistrement d'1 nouveau achat dans la base de données !");
    }

    YerithQMessageBox::information(this,
                                   QObject::tr("ajout d'1 nouveau achat"),
                                   achatRetMsg);
}


void YerithModifierWindow::update_achat_deja_existant(const QSqlRecord &aStockRecord,
                                                      double aPrixUnitaireHT,
                                                      double aPrixUnitaire_EN_GROS_HT)
{
    //Handling of table "achats"
    YerithSqlTableModel &achatSqlTableModel =
                    _allWindows->getSqlTableModel_achats();

    achatSqlTableModel.yerithSetFilter_WITH_where_clause(QString("%1='%2'").
                                                         arg
                                                         (YerithDatabaseTableColumn::
                                                          STOCKS_ID,
                                                          QString::
                                                          number
                                                          (_currentStockID)));

    int achatSqlTableModel_RESULT_RowCount =
    		achatSqlTableModel.easySelect("src/windows/stocks/yerith-erp-modifier-windows.cpp", 814);

    QString curDatePeremption = GET_SQL_RECORD_DATA(aStockRecord,
                                                    YerithDatabaseTableColumn::DATE_PEREMPTION);

    QDate
    curDatePeremptionFormatedFORDB(GET_DATE_FROM_STRING(curDatePeremption));

    double marge_beneficiaire = 0.0;
    double prix_vente = lineEdit_prix_vente->text().toDouble();
    double anAchatPrixDachat = 0.0;

    QSqlRecord anAchatRecord;

    for (int k = 0; k < achatSqlTableModel_RESULT_RowCount; ++k)
    {
        anAchatRecord = achatSqlTableModel.record(k);

        anAchatRecord.setValue(YerithDatabaseTableColumn::DATE_PEREMPTION,
                               DATE_TO_DB_FORMAT_STRING
                               (curDatePeremptionFormatedFORDB));

        anAchatRecord.setValue(YerithDatabaseTableColumn::LOCALISATION_STOCK,
                               lineEdit_localisation_produit->text());

        anAchatPrixDachat =
                        GET_SQL_RECORD_DATA(anAchatRecord,
                                            YerithDatabaseTableColumn::PRIX_DACHAT).
                        toDouble();

        anAchatRecord.setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                               aPrixUnitaireHT);

        anAchatRecord.
        setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                 aPrixUnitaire_EN_GROS_HT);

        anAchatRecord.setValue(YerithDatabaseTableColumn::PRIX_VENTE,
                               prix_vente);

        if (!_IS_AN_ASSET)
        {
            marge_beneficiaire = YerithUtils::getMargeBeneficiaire(prix_vente,
                                                                   anAchatPrixDachat,
                                                                   _montantTva);

            anAchatRecord.
             setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE, marge_beneficiaire);
        }
        else
        {
            anAchatRecord.
             setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE, 0.0);
        }


        achatSqlTableModel.updateRecord(k,
        								anAchatRecord,
										"src/windows/stocks/yerith-erp-modifier-window.cpp",
										871);
    }

    achatSqlTableModel.resetFilter();
}


void YerithModifierWindow::setupLineEdits()
{
    lineEdit_quantite_restante->setValidator(&YerithUtils::UintValidator);
    lineEdit_stock_dalerte->
    setValidator(&YerithUtils::POSITIVE_DoubleValidator);
    lineEdit_prix_vente->setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    if (checkBox_re_approvisionnement->isChecked())
    {
        lineEdit_quantite_restante->setYerithEnabled(true);
        lineEdit_reference_recu_dachat->setYerithEnabled(true);
        lineEdit_nom_entreprise_fournisseur->setYerithEnabled(true);
    }
    else
    {
        lineEdit_quantite_restante->setYerithEnabled(false);
        lineEdit_reference_recu_dachat->setYerithEnabled(false);
        lineEdit_nom_entreprise_fournisseur->setYerithEnabled(false);
    }


    lineEdit_localisation_produit->setYerithEnabled(true);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setYerithEnabled(false);

    lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setYerithEnabled
    (false);

    lineEdit_prix_dachat->setYerithEnabled(false);
    lineEdit_prix_vente->setYerithEnabled(false);
    lineEdit_prix_vente_EN_GROS->setYerithEnabled(false);
    lineEdit_reference_produit->setYerithEnabled(false);
    lineEdit_designation->setYerithEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerithEnabled(false);
    lineEdit_nom_departement_produit->setYerithEnabled(false);
    lineEdit_categorie_produit->setYerithEnabled(false);
    lineEdit_stock_dalerte->setYerithEnabled(true);
    lineEdit_tva->setYerithEnabled(false);

    lineEdit_tva->setText(YerithUtils::getTvaStringWithPercent());

    calculate_and_display_benefit_buying_price_percentage();
}


void YerithModifierWindow::setupLineEditsQCompleters()
{
    lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter
    (YerithDatabase::FOURNISSEURS, YerithDatabaseTableColumn::NOM_ENTREPRISE);
}


void YerithModifierWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerithWindowsCommons::deconnecter_utilisateur();
}


void YerithModifierWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
}


void YerithModifierWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(stocks()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
    pushButton_enregistrer->enable(this, SLOT(actualiser_stock()));
    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT(supprimer_image_stock
                                                      ()));
    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
}


void YerithModifierWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));

    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(stocks()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
    pushButton_enregistrer->enable(this, SLOT(actualiser_stock()));
    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT(supprimer_image_stock
                                                      ()));
    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
}


bool YerithModifierWindow::check_fields()
{
    bool prix_dachat = lineEdit_prix_dachat->checkField();

    bool prix_vente = lineEdit_prix_vente->checkField();

    if (_IS_AN_ASSET)
    {
        prix_vente = true;
    }

    return prix_dachat && prix_vente;
}


void YerithModifierWindow::clear_all_fields()
{
    checkBox_re_approvisionnement->setChecked(false);

    checkBox_tva->setChecked(false);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setText("0.00 %");

    lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText("0.00 %");

    lineEdit_reference_produit->clearField();
    lineEdit_designation->clearField();
    lineEdit_nom_departement_produit->clearField();
    lineEdit_categorie_produit->clearField();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_stock_dalerte->clearField();
    lineEdit_prix_dachat->clearField();
    lineEdit_prix_vente->clearField();
    lineEdit_prix_vente_EN_GROS->clear();
    lineEdit_localisation_produit->clear();
    lineEdit_tva->clearField();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);

    textEdit_description->clear();
}


void YerithModifierWindow::display_prix_vente()
{
    if (_lastEditedPrixVente != lineEdit_prix_vente->text())
    {
        return;
    }

    if (checkBox_tva->isChecked())
    {
        double prix_vente = lineEdit_prix_vente->text().toDouble();

        _montantTva = prix_vente * YerithERPConfig::tva_value;

        prix_vente = prix_vente + _montantTva;

        lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
    }
}


void YerithModifierWindow::display_prix_vente_EN_GROS()
{
    if (_lastEditedPrixVente_EN_GROS != lineEdit_prix_vente_EN_GROS->text())
    {
        return;
    }

    if (checkBox_tva->isChecked())
    {
        double prix_vente_EN_GROS =
                        lineEdit_prix_vente_EN_GROS->text().toDouble();

        _montantTva_EN_GROS = prix_vente_EN_GROS * YerithERPConfig::tva_value;

        prix_vente_EN_GROS = prix_vente_EN_GROS + _montantTva_EN_GROS;

        lineEdit_prix_vente_EN_GROS->
        setText(QString::number(prix_vente_EN_GROS, 'f', 2));
    }
}


void YerithModifierWindow::handleTVACheckBox(bool clicked)
{
    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_vente_EN_GROS =
                    lineEdit_prix_vente_EN_GROS->text().toDouble();

    if (clicked && checkBox_tva->isChecked())
    {
        if (false == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }
        //qDebug() << "\t ++_montantTva: " << _montantTva;
        _montantTva = prix_vente * YerithERPConfig::tva_value;

        _montantTva_EN_GROS = prix_vente_EN_GROS * YerithERPConfig::tva_value;

        prix_vente = prix_vente + _montantTva;

        prix_vente_EN_GROS = prix_vente_EN_GROS + _montantTva_EN_GROS;
    }
    else
    {
        //qDebug() << "\t ++_montantTva: " << _montantTva;
        _montantTva = 0;

        _montantTva_EN_GROS = 0;

        if (true == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = false;

            prix_vente = prix_vente / (1 + YerithERPConfig::tva_value);

            prix_vente_EN_GROS =
                            prix_vente_EN_GROS / (1 + YerithERPConfig::tva_value);
        }
    }

    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));

    lineEdit_prix_vente_EN_GROS->
    setText(QString::number(prix_vente_EN_GROS, 'f', 2));
}


void YerithModifierWindow::supprimer_ce_stock()
{
    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    msgSupprimer(QObject::tr
                 ("Poursuivre avec la suppression du stock '%1' ?").arg
                 (GET_SQL_RECORD_DATA
                  (record, YerithDatabaseTableColumn::DESIGNATION)));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this, QObject::tr("suppression"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool resRemoved =
                        _allWindows->
                        _stocksWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW();

        //qDebug() << "YerithModifierWindow::supprimer_ce_stock() " << resRemoved;

        stocks();

        if (resRemoved)
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("Le stock '%1' a été supprimé !").arg
                                (GET_SQL_RECORD_DATA
                                 (record,
                                  YerithDatabaseTableColumn::DESIGNATION)));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("1 immobilisation OU stock '%1' ne pouvait pas être supprimé(e) !").
                                arg(GET_SQL_RECORD_DATA
                                    (record,
                                     YerithDatabaseTableColumn::
                                     DESIGNATION)));

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


void YerithModifierWindow::supprimer_image_stock()
{
    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    stockName(GET_SQL_RECORD_DATA
              (record, YerithDatabaseTableColumn::DESIGNATION));

    QVariant
    image_produit(record.value(YerithDatabaseTableColumn::IMAGE_PRODUIT));

    if (image_produit.toByteArray().isEmpty())
    {
        QString
        msg(QObject::tr
            ("Le stock '%1' n'a pas d'image enregistrée !").arg
            (stockName));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("suppression de l'image d'1 immobilisation OU stock"),
                                       msg);

        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);

        return;
    }


    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression de l'image du stock \""));

    msgSupprimer.append(stockName);
    msgSupprimer.append("\" ?");

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression de l'image d'1 immobilisation OU stock"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        record.setValue(YerithDatabaseTableColumn::IMAGE_PRODUIT,
                        QVariant(QVariant::ByteArray));

        bool success_removed_STOCK_IMAGE =
                        _allWindows->
                        _stocksWindow->SQL_UPDATE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
                        (record);

        label_image_produit->clear();

        label_image_produit->setAutoFillBackground(false);

        if (success_removed_STOCK_IMAGE)
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("L'image du stock '%1' a été supprimée !").
                                arg(stockName));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression de l'image du stock avec succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("L'image du stock '%1' ne pouvait pas être supprimée !").
                                arg(stockName));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("échec de la suppression de l'image d'1 immobilisation OU stock"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerithModifierWindow::rendreInvisible()
{
    clear_all_fields();

    _lastEditedPrixVente.clear();

    _montantTva = 0;

    _tvaPercent = YerithUtils::getTvaStringWithPercent();

    _tvaCheckBoxPreviousState = false;

    dateEdit_date_peremption->reset();

    YerithWindowsCommons::rendreInvisible();
}


void YerithModifierWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    checkBox_tva->setReadOnly(true);

    setupLineEditsQCompleters();

    showItem();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("modifier (ré-approvisionner) un(e) stock / immobilisation"),
      								 _curStocksTableModel);

    setVisible(true);
}


void YerithModifierWindow::setStockSpecificWidgetVisible(bool visible)
{
    label_reference_recu_dachat->setVisible(visible);
    lineEdit_reference_recu_dachat->setVisible(visible);

    label_stock_dalerte->setVisible(visible);
    lineEdit_stock_dalerte->setVisible(visible);

    label_localisation_du_stock->setVisible(visible);
    lineEdit_localisation_produit->setVisible(visible);

    dateEdit_date_peremption->setVisible(visible);
    label_date_peremption->setVisible(visible);
}


void YerithModifierWindow::showItem()
{
    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    bool is_service = GET_SQL_RECORD_DATA(record,
                                          YerithDatabaseTableColumn::IS_SERVICE).
                      toInt();

    if (is_service)
    {
        return;
    }

    QString recordID =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID);

    _currentStockID = recordID.toInt();

    int achatQuerySize =
                    YerithUtils::STOCK_PURCHASE_RECORDS_QUANTITY(_currentStockID);

    checkBox_achat->setVisible((achatQuerySize > 0));

    checkBox_achat->setChecked((achatQuerySize > 0));

    if (achatQuerySize > 0)
    {
        handle_achat_checkBox(Qt::Checked);
    }
    else
    {
        handle_achat_checkBox(Qt::Unchecked);
    }


    bool historique_stock_actif = GET_SQL_RECORD_DATA(record,
                                                      YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                                  toInt();


    if (historique_stock_actif)
    {
        checkBox_HISTORIQUE->setYerithChecked(true,
                                              QObject::tr
                                              ("historique actif"));
    }
    else
    {
        checkBox_HISTORIQUE->setYerithChecked(false,
                                              QObject::tr
                                              ("historique désactiver"));
    }


    setStockSpecificWidgetVisible(true);


    if (!YerithUtils::IS_QSQL_RECORD_AN_ASSET(record))
    {
        _IS_AN_ASSET = false;

        lineEdit_prix_vente->setYerithEnabled(true);

        lineEdit_prix_vente_EN_GROS->setYerithEnabled(true);
    }
    else
    {
        _IS_AN_ASSET = true;

        lineEdit_prix_vente->setYerithEnabled(false);

        lineEdit_prix_vente_EN_GROS->setYerithEnabled(false);
    }


    label_fournisseur->setText(QObject::tr("fournisseur"));

    lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::REFERENCE));

    lineEdit_designation->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerithDatabaseTableColumn::DESIGNATION));

    lineEdit_stock_dalerte->setText(GET_SQL_RECORD_DATA
                                    (record,
                                     YerithDatabaseTableColumn::STOCK_DALERTE));

    lineEdit_reference_recu_dachat->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT));


    _currentPrixDachat =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::PRIX_DACHAT);

    lineEdit_prix_dachat->setText(_currentPrixDachat);

    YerithPOSUser *currentUser = YerithUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
        if (!currentUser->isManager() ||
                !currentUser->isGestionaireDesStocks())
        {
            lineEdit_reference_recu_dachat->setYerithEnabled(false);
            lineEdit_prix_dachat->setYerithEnabled(false);
        }
        else
        {
            lineEdit_reference_recu_dachat->setYerithEnabled(true);
            lineEdit_prix_dachat->setYerithEnabled(true);
        }
    }

    double prix_vente = GET_SQL_RECORD_DATA(record,
                                            YerithDatabaseTableColumn::
                                            PRIX_VENTE).toDouble();

    double prix_vente_EN_GROS = GET_SQL_RECORD_DATA(record,
                                                    YerithDatabaseTableColumn::
                                                    PRIX_VENTE_EN_GROS).
                                toDouble();

    _montantTva =
                    GET_SQL_RECORD_DATA(record,
                                        YerithDatabaseTableColumn::MONTANT_TVA).toDouble();
    //qDebug() << "\t++ showItem, _montantTva: " << _montantTva;
    if (_montantTva > 0)
    {
        checkBox_tva->setChecked(true);
        _tvaCheckBoxPreviousState = true;
    }

    lineEdit_tva->setText(YerithUtils::getTvaStringWithPercent());

    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));

    lineEdit_prix_vente_EN_GROS->
    setText(QString::number(prix_vente_EN_GROS, 'f', 2));

    double quantite_restante = GET_SQL_RECORD_DATA(record,
                                                   YerithDatabaseTableColumn::QUANTITE_TOTALE).
                               toDouble();

    lineEdit_quantite_restante->
    setText(QString::number(quantite_restante, 'f', 2));
    textEdit_description->
    setText(GET_SQL_RECORD_DATA
            (record, YerithDatabaseTableColumn::DESCRIPTION_PRODUIT));

    QString
    date_peremption(GET_SQL_RECORD_DATA
                    (record, YerithDatabaseTableColumn::DATE_PEREMPTION));

    dateEdit_date_peremption->setYerithEnabled(true);
    dateEdit_date_peremption->setMyDate(GET_DATE_FROM_STRING
                                        (date_peremption));
    lineEdit_nom_departement_produit->setText(GET_SQL_RECORD_DATA
                                              (record,
                                               YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));
    lineEdit_categorie_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::CATEGORIE));
    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::LOCALISATION_STOCK));

    QVariant img(record.value(YerithDatabaseTableColumn::IMAGE_PRODUIT));

    if (!img.isNull())
    {
        YerithUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}
