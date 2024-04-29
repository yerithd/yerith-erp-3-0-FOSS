/*
 * yerith-erp-main-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-main-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-user-administrateur.hpp"

#include "src/users/yerith-erp-user-caissier.hpp"

#include "src/users/yerith-erp-user-magasinier.hpp"

#include "src/users/yerith-erp-user-manager.hpp"

#include "src/utils/yerith-erp-info-entreprise.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/dialogs/yerith-erp-changer-utilisateur-dialog.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>



#ifdef YERITH_FRANCAIS_LANGUAGE
	bool YerithMainWindow::LANGUE_ANGLAISE = false;
#endif

#ifdef YERITH_ENGLISH_LANGUAGE
	bool YerithMainWindow::LANGUE_ANGLAISE = true;
#endif


YerithMainWindow::YerithMainWindow()
:YerithWindowsCommons(),
_logger(new YerithLogger("YerithMainWindow"))
{
    _windowName = QString("%1 - %2").arg(YerithMainWindow::GET_YERITH_ERP_WINDOW_TITLE(),
                                         QObject::tr("fenêtre de l'utilisateur"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    QMESSAGE_BOX_STYLE_SHEET =
                    QString
                    ("QMessageBox {messagebox-text-interaction-flags: 5; background-color: rgb(%1);}"
                     "QMessageBox QLabel {color: rgb(%2);} ").arg
                    (COLOUR_RGB_STRING_YERITH_BLUE_53_132_228,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();


    label_display_yerith->setStyleSheet(YerithUtils::STRING_STYLESHEETS_YERITH_ERP_3_0);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);


    _main_window_button_TO_page_position.insert(0, std::make_tuple(193, 122));
    _main_window_button_TO_page_position.insert(1, std::make_tuple(300, 122));
    _main_window_button_TO_page_position.insert(2, std::make_tuple(407, 122));
    _main_window_button_TO_page_position.insert(3, std::make_tuple(514, 122));

    _main_window_button_TO_page_position.insert(4, std::make_tuple(193, 215));
    _main_window_button_TO_page_position.insert(5, std::make_tuple(300, 215));
    _main_window_button_TO_page_position.insert(6, std::make_tuple(407, 215));
    _main_window_button_TO_page_position.insert(7, std::make_tuple(514, 215));

    _main_window_button_TO_page_position.insert(8, std::make_tuple(193, 308));
    _main_window_button_TO_page_position.insert(9, std::make_tuple(300, 308));
    _main_window_button_TO_page_position.insert(10,
                                                std::make_tuple(407, 308));
    _main_window_button_TO_page_position.insert(11,
                                                std::make_tuple(514, 308));


    _main_window_buttons.push_back(pushButton_fournisseurs);
    _main_window_buttons.push_back(pushButton_charges_financieres);
    _main_window_buttons.push_back(pushButton_entrer);
    _main_window_buttons.push_back(pushButton_ventes);
    _main_window_buttons.push_back(pushButton_CRM_clients);
    _main_window_buttons.push_back(pushButton_afficher_stocks);
    _main_window_buttons.push_back(pushButton_marchandises_SERVICES);
    _main_window_buttons.push_back(pushButton_sortir);
    _main_window_buttons.push_back(pushButton_paiements);
    _main_window_buttons.push_back(pushButton_rapports);
    _main_window_buttons.push_back(pushButton_mouvementsDeStocks);
    _main_window_buttons.push_back(pushButton_vendre);


    connect(action_START_YR_QVGE_dynamic_analysis_tool,
            SIGNAL(triggered()),
            this,
            SLOT(ON_action_START_YR_QVGE_dynamic_analysis_tool()));


    connect(action_TRADUIRE_EN_ANGLAIS,
    		SIGNAL(triggered()),
			this,
            SLOT(CHANGER_DE_LANGUE()));


    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionGuide_PRATIQUE_DE_LUTILISATEUR, SIGNAL(triggered()), this,
            SLOT(getManuelUtilisateurPDF()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionMarchandises, SIGNAL(triggered()), this,
            SLOT(afficherMarchandises()));
    connect(actionCHARGES_FINANCIERES, SIGNAL(triggered()), this,
            SLOT(charges_financieres()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionTableauxDeBords, SIGNAL(triggered()), this,
            SLOT(tableaux_de_bords()));
    connect(actionVente, SIGNAL(triggered()), this, SLOT(ventes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionPaiements, SIGNAL(triggered()), this, SLOT(paiements()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionMouvementsDeStocks, SIGNAL(triggered()), this,
            SLOT(mouvements_de_stocks()));
    connect(actionFournisseurs, SIGNAL(triggered()), this,
            SLOT(fournisseurs()));
    connect(actionAchatsStocks, SIGNAL(triggered()), this,
            SLOT(achats_de_stocks()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(courrier()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

#ifdef YERITH_FEATURES_COMPTABILITE_yes
    connect(action_acceder_aux_comptes_de_lentreprise, SIGNAL(triggered()),
            this, SLOT(comptabilite()));
#endif

    setupShortcuts();
}


void YerithMainWindow::calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW()
{
//      QDEBUG_STRING_OUTPUT_1("calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW");

    YerithPushButton *next_to_draw_push_button = 0;

    uint next_position_to_draw = 0;

    for (uint k = 0; k < _main_window_buttons.size(); ++k)
    {
        next_to_draw_push_button = _main_window_buttons.at(k);

        if (0 != next_to_draw_push_button)
        {
            if (next_to_draw_push_button->isAuthorizedForCurrentUser())
            {
//                              QDEBUG_STRING_OUTPUT_2(next_to_draw_push_button->text(),
//                                                                         QString("(%1, %2)")
//                                                                                      .arg(QString::number(std::get<0>(_main_window_button_TO_page_position.value(next_position_to_draw))),
//                                                                                               QString::number(std::get<1>(_main_window_button_TO_page_position.value(next_position_to_draw)))));

                next_to_draw_push_button->setGeometry(std::get < 0 >
                                                      (_main_window_button_TO_page_position.
                                                       value
                                                       (next_position_to_draw)),
                                                      std::get < 1 >
                                                      (_main_window_button_TO_page_position.
                                                       value
                                                       (next_position_to_draw)),
                                                      next_to_draw_push_button->
                                                      width(),
                                                      next_to_draw_push_button->
                                                      height());

                ++next_position_to_draw;
            }
        }
    }
}


void YerithMainWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithMainWindow::definirCaissier()
{
    _logger->log("definirCaissier");


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yerith->setVisible(false);
}

void YerithMainWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, true);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);

#ifdef YERITH_FEATURES_COMPTABILITE_yes
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, true);
#endif

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_CRM_clients->enable(this, SLOT(clients()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_charges_financieres->enable(this,
                                           SLOT(charges_financieres()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->enable(this,
                                             SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_rapports->enable(this, SLOT(tableaux_de_bords()));
    pushButton_mouvementsDeStocks->enable(this,
                                          SLOT(mouvements_de_stocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yerith->setVisible(false);
}


void YerithMainWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#endif

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->enable(this, SLOT(clients()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->enable(this,
                                             SLOT(afficherMarchandises()));
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yerith->setVisible(false);
}


void YerithMainWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#endif

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->enable(this,
                                             SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_mouvementsDeStocks->enable(this,
                                          SLOT(mouvements_de_stocks()));

    label_display_yerith->setVisible(false);
}

void YerithMainWindow::definirAdministrateur()
{

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION

    _logger->log("definirAdministrateur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    _allWindows->_adminWindow->definirAdministrateur();

#endif

    label_display_yerith->setVisible(false);
}

void YerithMainWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_rapports->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_mouvementsDeStocks->enable(this,
                                          SLOT(mouvements_de_stocks()));
    label_display_yerith->setVisible(false);
}

void YerithMainWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_START_YR_QVGE_dynamic_analysis_tool, false);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);

    label_display_yerith->setVisible(true);
}


void YerithMainWindow::setup_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY()
{
    YerithUtils::YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY =
                    QString("database ip address: \"%1\" - "
                            "database table: \"%2\" - "
                            "database options: \"%3\"").arg(_allWindows->getDatabase().
                                                            db_ip_address(),
                                                            _allWindows->getDatabase().
                                                            db_name(),
                                                            _allWindows->getDatabase().
                                                            db_connection_options());
}


void YerithMainWindow::write_AFTER_SETUP_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY()
{
    setup_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    if (_allWindows->getDatabase().isOpen())
    {
        label_display_yerith_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
							 .arg(COLOUR_RGB_STRING_YERITH_BLUE_53_132_228));

        _allWindows->_adminWindow
			->label_ADMIN_WINDOW_display_yerith_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YERITH_BLUE_53_132_228));

        _allWindows->_pdVenteWindow
			->label_PDV_display_yerith_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YERITH_BLUE_53_132_228));
    }
    else
    {
        label_display_yerith_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
							 .arg(COLOUR_RGB_STRING_YERITH_FIREBRICK_RED_255_48_48));

        _allWindows->_adminWindow
			->label_ADMIN_WINDOW_display_yerith_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YERITH_FIREBRICK_RED_255_48_48));

        _allWindows->_pdVenteWindow
        	->label_PDV_display_yerith_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YERITH_FIREBRICK_RED_255_48_48));
    }


    label_display_yerith_erp_3_0_server_parameters
		->setText(YerithUtils::YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);

    _allWindows->_adminWindow
		->label_ADMIN_WINDOW_display_yerith_erp_3_0_server_parameters
			->setText(YerithUtils::YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);

    _allWindows->_pdVenteWindow
		->label_PDV_display_yerith_erp_3_0_server_parameters
			->setText(YerithUtils::YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);
}


void YerithMainWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	CHANGER_DE_LANGUE__text__TRANSLATION(this,
                                         action_TRADUIRE_EN_ANGLAIS);

    calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW();

    YerithWindowsCommons::rendreVisible(stocksTableModel);

    rendreVisibleLocalTOCLASS(_allWindows->getDatabase().isOpen());
}


void YerithMainWindow::rendreVisibleLocalTOCLASS(bool isDatabaseOpened /* = false */)
{
    retranslateUi(this);

	CHANGER_DE_LANGUE__text__TRANSLATION(this,
                                         action_TRADUIRE_EN_ANGLAIS);

	write_AFTER_SETUP_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    show();
}


void YerithMainWindow::CHANGER_DE_LANGUE()
{
    YerithWindowsCommons::CHANGER_DE_LANGUE(this,
                                            action_TRADUIRE_EN_ANGLAIS);

	write_AFTER_SETUP_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();
}


void YerithMainWindow::ON_action_START_YR_QVGE_dynamic_analysis_tool()
{
    _allWindows->_adminWindow->START_YRI_DB_RUNTIME_VERIF_GUI();
}


void YerithMainWindow::help()
{
//#define YERITH_DEVEL_TARGET

#ifndef YERITH_DEVEL_TARGET
    YerithPOSUser *currentUser = _allWindows->getUser();

    if (0 != currentUser && !currentUser->isPasDeRole())
    {
        YerithQMessageBox::information(this, QObject::tr("aide"),
                                       QObject::tr
                                       ("1) Veuillez vous enregistrer en cliquant sur le premier icône !\n\n"
                                        "2) Déconnectez vous en cliquant sur le deuxième icône !\n\n"
                                        "3) Lorsque vous êtes enregistré, choisissez une opération "
                                        "à réaliser en cliquant "
                                        "sur le bouton correspondant !"));
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("aide"),
                                       QObject::tr
                                       ("1) Veuillez vous enregistrer en cliquant sur le premier icône !\n\n"
                                        "2) Recevez un message d'aide en cliquant sur le deuxieme icone !"));
    }
#else

    QDEBUG_STRING_OUTPUT_2
    ("YerithERPConfig::YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER",
     YerithERPConfig::YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);

    YerithUtils::YERITH_CREATE_FOLDER
    (YerithERPConfig::YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);

    YerithERPConfig::THIS_SITE_LOCALISATION_NAME = "YERITH_RD_TEST";

    _allWindows->_adminWindow->start_TESTING_MAINTENANCE();

    //YerithUserLAdministrateur *u = new YerithUserLAdministrateur(_allWindows);
    //YerithUserLeCaissier *u = new YerithUserLeCaissier(_allWindows);
    //YerithUserLeMagasinier *u = new YerithUserLeMagasinier(_allWindows);

    YerithERPConfig::tva_value = 7.25 / 100.0;

    YerithERPConfig::currency = "F";

    YerithERPConfig::receiptFormat =
                    YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER;

    YerithERPConfig::printer = YerithUtils::IMPRIMANTE_PDF;

    YerithPOSUserManager *u = new YerithPOSUserManager(_allWindows);
    u->set_titre("Prof");
    u->set_prenom("Xavier");
    u->set_nom("Noundou");
    u->set_nom_utilisateur("yerith-nissi");
    u->set_localisation("Yaoundé");
    u->set_nom_complet("Prof Dr Xavier Noundou");

    _allWindows->setUser(u);

#endif
}


void YerithMainWindow::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();

    YerithPOSUser *currentUser = _allWindows->getUser();

    if (0 != currentUser)
    {
        currentUser->setRole(YerithUtils::ROLE_INDEFINI);
    }
}


void YerithMainWindow::cleanupAllOtherWindows()
{
    if (0 != _allWindows->getUser())
    {
        definirPasDeRole();
        _allWindows->hideAllWindows();
        show();
    }
}
