/*
 * yerith-erp-achats-detail-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-achats-detail-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include <QtSql/QSqlRecord>


YerithAchatsDetailWindow::YerithAchatsDetailWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithAchatsDetailWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("détails d'1 achat de stock"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_DARK_GRAY_60_60_60,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    setupLineEdits();

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    dateEdit_date_peremption->setYerithEnabled(false);

    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_menu->disable(this);
    pushButton_retour->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
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

void YerithAchatsDetailWindow::setupLineEdits()
{
    lineEdit_reference_produit->setYerithEnabled(false);
    lineEdit_designation->setYerithEnabled(false);
    lineEdit_quantite_restante->setYerithEnabled(false);
    lineEdit_prix_dachat->setYerithEnabled(false);
    lineEdit_prix_unitaire->setYerithEnabled(false);
    lineEdit_prix_vente->setYerithEnabled(false);
    lineEdit_categorie_produit->setYerithEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerithEnabled(false);
}


void YerithAchatsDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_menu->disable(this);
    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_retour->disable(this);
}


void YerithAchatsDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_lister->enable(this, SLOT(stocks()));
    pushButton_menu->disable(this);
    pushButton_retour->disable(this);
}

void YerithAchatsDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_lister->enable(this, SLOT(stocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(achats_de_stocks()));
}


void YerithAchatsDetailWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_lister->enable(this, SLOT(stocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(achats_de_stocks()));
}


void YerithAchatsDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_lister->enable(this, SLOT(stocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(achats_de_stocks()));
}

void YerithAchatsDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_lister->enable(this, SLOT(stocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->disable(this);
}


void YerithAchatsDetailWindow::rendreInvisible()
{
    lineEdit_reference_produit->clear();
    lineEdit_designation->clear();
    lineEdit_categorie_produit->clear();
    lineEdit_quantite_restante->clear();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_prix_dachat->clear();
    lineEdit_prix_unitaire->clear();
    lineEdit_prix_vente->clear();

    YerithWindowsCommons::rendreInvisible();
}


void YerithAchatsDetailWindow::rendreVisible(YerithSqlTableModel *stocksTableModel,
                                             YerithSqlTableModel *achatStocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curAchatStocksTableModel = achatStocksTableModel;

    setVisible(true);

    //qDebug() << "++ last selected row: " << YerithERPWindows::get_last_lister_selected_row_db_ID();
    showItem();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("détails d'1 achat de stock"),
    								 _curAchatStocksTableModel);
}


void YerithAchatsDetailWindow::showItem()
{
    _curAchatStocksTableModel->yerithSetFilter_WITH_where_clause(QString
                                                                 ("%1 = '%2'").
                                                                 arg
                                                                 (YerithDatabaseTableColumn::ID,
                                                                  YerithERPWindows::
                                                                  get_last_lister_selected_row_db_ID
                                                                  ()));

    QSqlRecord record = _curAchatStocksTableModel->record(0);

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::REFERENCE));
    lineEdit_designation->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerithDatabaseTableColumn::DESIGNATION));
    lineEdit_categorie_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::CATEGORIE));
    lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

    double prix_unitaire = GET_SQL_RECORD_DATA(record,
                                               YerithDatabaseTableColumn::PRIX_UNITAIRE).
                           toDouble();

    lineEdit_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));

    double prix_dachat = 0.0;

    YerithPOSUser *currentUser = YerithUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
        if (currentUser->isManager() || currentUser->isGestionaireDesStocks())
        {
            prix_dachat =
                            GET_SQL_RECORD_DATA(record,
                                                YerithDatabaseTableColumn::PRIX_DACHAT).
                            toDouble();
        }
    }

    lineEdit_prix_dachat->setText(GET_CURRENCY_STRING_NUM(prix_dachat));

    double prix_vente = GET_SQL_RECORD_DATA(record,
                                            YerithDatabaseTableColumn::PRIX_VENTE).
                        toDouble();

    lineEdit_prix_vente->setText(GET_CURRENCY_STRING_NUM(prix_vente));

    double quantite_restante = GET_SQL_RECORD_DATA(record,
                                                   YerithDatabaseTableColumn::QUANTITE_TOTALE).
                               toDouble();

    lineEdit_quantite_restante->setText(GET_DOUBLE_STRING_P
                                        (quantite_restante, 0));


    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::LOCALISATION_STOCK));

    dateEdit_date_peremption->setDate(record.
                                      value(YerithDatabaseTableColumn::
                                            DATE_PEREMPTION).toDate());

    lineEdit_reference_recu_dachat->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT));

    _curAchatStocksTableModel->resetFilter();
}


void YerithAchatsDetailWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
