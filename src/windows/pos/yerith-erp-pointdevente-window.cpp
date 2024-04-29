/*
   * yerith-erp-pointdevente-window.cpp
   *
   *      Author: Prof Dr Xavier Noundou
   */

#include "yerith-erp-pointdevente-window.hpp"

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/windows/pos/yerith-erp-pointdevente-liststocks-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/windows/stocks/yerith-erp-stock-detail-window.hpp"

#include "src/widgets/yerith-erp-line-edit.hpp"

#include "src/widgets/table-view/yerith-erp-table-view.hpp"

#include "src/utils/yerith-erp-spell-numbers.hpp"

#include "src/utils/yerith-erp-payment-processing-information.hpp"

#include "src/utils/yerith-erp-map-COMPLEX-ITEM.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-info-entreprise.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/users/yerith-erp-users.hpp"


#include <QtCore/QDate>

#include <QtCore/QtMath>

#include <QtCore/QThread>

#include <QtCore/QFile>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

#include <unistd.h>


bool YerithPointDeVenteWindow::_qteChangeCodeBar(false);


YerithPointDeVenteWindow::YerithPointDeVenteWindow()
:YerithQTableWidgetWindowsCommons(),
 _logger(new YerithLogger("YerithPointDeVenteWindow")),
 _paiement_carte_credit_carte_debit(false),
 _paiment_comptant(false),
 _paiement_compteclient_AVANCE_DE_CASH(false),
 _currentFocusSearchBar(0),
 _currentCreditCardInfo(0),
 _barcodeReaderActivated(false),
 _updateItemConversionError(false),
 _previousPressedQteValue("1"),
 _tvaCheckBoxPreviousState(false),
 _currentStocksID(YerithUtils::EMPTY_STRING),
 _TOTAL_PRIX_VENTE__POUR__COMPTECLIENT_AVANCE_DE_CASH(0.0),
 _sommeTotal_HORS_TAXES(0.0),
 _sommeTotal(0.0),
 _remise_somme_total_prix(0.0),
 _remise_somme_total_pourcentage(0.0),
 _SERVICE(0.0),
 _tva(0.0),
 _montantRecu(0.0),
 _montantARembourser(0.0),
 _quantiteAVendre(0),
 _currentTabView(0),
 actionRechercheArticle(0),
 actionRechercheArticleCodebar(0)
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("point de vente"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    _a_CURRENT_table_widget = tableWidget_articles;


    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_articles->font());



    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
        << pushButton_minus;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
        << actionAfficherPDF;




    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                            		   "QMessageBox QLabel {color: rgb(%2);}")
                            	.arg(COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                            		 COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    tableWidget_articles->resizeColumnsToContents();

    label_total_ttc->setStyleSheet(YerithPosStyle::getColorStyleSheetString
                                   (QColor
                                    (COLOUR_RGB_YERITH_ORANGE_243_162_0)));

    _currentFocusSearchBar = INITIAL_FOCUS_SEARCHBAR_VENTE_WINDOW;

    _currentCreditCardInfo = new YerithPOSCreditCardInfo;

    setRechercheLineEditFocus();

    checkBox_enregistrer_client->setChecked(false);

    checkBox_imprimer_recu_vendu->setChecked(true);

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_lecteur_de_code_barres->setVisible(_barcodeReaderActivated);

    pushButton_minus->disable(this);

    // Menu actions


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
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		(YerithQTableWidgetWindowsCommons *) this,
            SLOT(incrementFontSize__OF_TABLE__WIDGET()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		(YerithQTableWidgetWindowsCommons *) this,
            SLOT(decrementFontSize__OF_TABLE__WIDGET()));


    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(afficher_recu_vendu_pdf()));

    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionBasculerLecteurDeCodebarres, SIGNAL(triggered()), this,
            SLOT(handleBasculerLecteurDeCodebarres()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annuler()));

    connect(actionRETIRERARTICLE, SIGNAL(triggered()), this,
            SLOT(retirer_un_article()));

    connect(actionVendre, SIGNAL(triggered()), this,
            SLOT(choisir_methode_paiment()));

    connect(this, SIGNAL(SELLING()),
            _allWindows->_pdVenteMethodePaiementCarteCreditDialog,
            SLOT(reset_all_fields()));

    connect(_allWindows->_pdVenteMethodePaiementCarteCreditDialog,
            SIGNAL(SIGNAL_CREDIT_CARD_INFO_ALL_PROCESSED()),
            this, SLOT(PRE__PROCESS_CREDIT_CARD_PAYMENT()));

    connect(_allWindows->_pdVenteMethodePaiementComptantEntreeDialog->
            lineEdit_vente_montant_recu,
            SIGNAL(textChanged(const QString &)), this,
            SLOT(handleMontantRecu()));

    connect(_allWindows->_pdVenteMethodePaiementComptantEntreeDialog->
            lineEdit_vente_montant_recu, SIGNAL(textEdited(const QString)),
            this, SLOT(handleMontantRecu()));

    connect(tableWidget_articles,
    		SIGNAL(itemSelectionChanged()),
            this,
			SLOT(handle_VIEW_FONT_SIZE_change_buttons()));

    connect(tableWidget_articles, SIGNAL(itemPressed(QTableWidgetItem *)),
            this, SLOT(handleQtePressed(QTableWidgetItem *)));

    connect(tableWidget_articles, SIGNAL(itemChanged(QTableWidgetItem *)),
            this, SLOT(handleQteChange(QTableWidgetItem *)));

    connect(tabWidget_vente, SIGNAL(currentChanged(int)), this,
            SLOT(handleCurrentTABChanged(int)));

    connect(tableWidget_articles, SIGNAL(addedYerithTableWidget()), this,
            SLOT(handleAddedArticle()));

    connect(tableWidget_articles,
            SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this,
            SLOT(afficher_vente_detail(QTableWidgetItem *)));

    /* Signals-slots connection for the second tab 'Article au détail' */
    connect(checkBox_tva, SIGNAL(clicked(bool)), this,
            SLOT(handleTVACheckBox(bool)));

    connect(textEdit_MEMO_SALE_for_SEARCH,
            SIGNAL(textChanged()),
            this,
			SLOT(update_text_memo_SALE()));

    connect(lineEdit_article_detail_quantite_a_vendre,
            SIGNAL(textEdited(const QString &)),
            this,
			SLOT(updateQuantiteAVendre()));

    connect(lineEdit_article_detail_AJOUT_COMMERCIAL,
            SIGNAL(editingFinished()),
            this, SLOT(calculate_details_window_AJOUT_COMMERCIAL()));

    connect(lineEdit_article_detail_remise_prix,
            SIGNAL(textEdited(const QString)),
            this, SLOT(calculate_details_window_remise_prix()));

    connect(lineEdit_article_detail_remise_pourcentage,
            SIGNAL(textEdited(const QString)), this,
            SLOT(calculate_details_window_remise_pourcentage()));

    connect(radioButton_article_detail_AJOUT_COMMERCIAL, SIGNAL(toggled(bool)),
            this, SLOT(activateLineEdit_AJOUT_COMMERCIAL(bool)));

    connect(radioButton_article_detail_remise_prix, SIGNAL(toggled(bool)),
            this, SLOT(activateLineEditRemisePrix(bool)));

    connect(radioButton_article_detail_remise_pourcentage,
            SIGNAL(toggled(bool)), this,
            SLOT(activateLineEditRemisePourcentage(bool)));

    connect(actionSet_reference_as_standard_input, SIGNAL(triggered()), this,
            SLOT(setBarcodeAsStandardInput()));

    connect(actionSet_stock_item_name_as_standard_input, SIGNAL(triggered()),
            this, SLOT(setStockItemNameAsStandardInput()));

    connect(checkBox_effectuer_1_vente_en_gros, SIGNAL(stateChanged(int)),
            this, SLOT(handle_effectuer_vente_en_gros(int)));


    connect(checkBox_DATE_EXTRA_VENTE,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_DATA_EXTRA_VENTE(int)));


    setupShortcuts();
}

YerithPointDeVenteWindow::~YerithPointDeVenteWindow()
{
    deleteArticleVenteInfos();

    _paiement_carte_credit_carte_debit = false;

    _paiment_comptant = false;

    _paiement_compteclient_AVANCE_DE_CASH = false;

    delete _currentCreditCardInfo;

    delete actionRechercheArticle;

    delete actionRechercheArticleCodebar;

    delete _logger;
}


void YerithPointDeVenteWindow::setup_AJOUT_COMMERCIAL(bool aValue)
{
	radioButton_article_detail_AJOUT_COMMERCIAL->setEnabled(aValue);
	lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(aValue);
}


void YerithPointDeVenteWindow::setupRemises(bool aValue)
{
    radioButton_article_detail_remise_prix->setEnabled(aValue);
    radioButton_article_detail_remise_pourcentage->setEnabled(aValue);
    lineEdit_article_detail_remise_prix->setEnabled(aValue);
    lineEdit_article_detail_remise_pourcentage->setEnabled(aValue);
}

void YerithPointDeVenteWindow::deleteArticleVenteInfos()
{
    QMapIterator < int,
                 YerithArticleVenteInfo * >itArticleItemVenteInfo(articleItemToVenteInfo);

    YerithArticleVenteInfo *v = 0;

    while (itArticleItemVenteInfo.hasNext())
    {
        itArticleItemVenteInfo.next();

        v = itArticleItemVenteInfo.value();

        if (0 != v)
        {
            delete v;
        }
    }

    articleItemToVenteInfo.clear();
}


void YerithPointDeVenteWindow::setBarcodeAsStandardInput()
{
    _currentFocusSearchBar = lineEdit_recherche_article_codebar;
    lineEdit_recherche_article_codebar->setFocus();
}


void YerithPointDeVenteWindow::setStockItemNameAsStandardInput()
{
    _currentFocusSearchBar = lineEdit_recherche_article;
    lineEdit_recherche_article->setFocus();
}


void YerithPointDeVenteWindow::updateLineEditQCompleterInput()
{
    if (!_barcodeReaderActivated)
    {
        connect_manual_selection_of_article_item();
    }
    else
    {
        connect_barcode_reader_selection_of_article_item();
    }
}


void YerithPointDeVenteWindow::handle_DATA_EXTRA_VENTE(int state)
{
    if (checkBox_DATE_EXTRA_VENTE->isChecked())
    {
        enable_DATA_EXTRA_VENTE(true);
    }
    else
    {
        enable_DATA_EXTRA_VENTE(false);
    }
}


void YerithPointDeVenteWindow::handle_effectuer_vente_en_gros(int state)
{
    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidget_articles->currentRow());

    if (0 == articleVenteInfo)
    {
        QDEBUG_STRING_OUTPUT_2
        ("YerithPointDeVenteWindow::handle_effectuer_vente_en_gros(int)",
         QString("invalid YerithArticleVenteInfo (NULL) !"));

        return;
    }

    if (checkBox_effectuer_1_vente_en_gros->isChecked())
    {
        articleVenteInfo->_effectuer_vente_en_gros = true;
    }
    else
    {
        articleVenteInfo->_effectuer_vente_en_gros = false;
    }

    actualiser_montant_remise();
}


void YerithPointDeVenteWindow::handleBasculerLecteurDeCodebarres()
{
    pushButton_lecteur_de_code_barres->setVisible(!_barcodeReaderActivated);

    if (!_barcodeReaderActivated)
    {
        _barcodeReaderActivated = true;
        connect_barcode_reader_selection_of_article_item();
    }
    else
    {
        _barcodeReaderActivated = false;
        connect_manual_selection_of_article_item();
    }
}


void YerithPointDeVenteWindow::handleRefreshSaleStrategy()
{
    YerithUtils::refreshSalesStrategy(*_curStocksTableModel,
                                      lineEdit_recherche_article,
                                      lineEdit_recherche_article_codebar);

    updateLineEditQCompleterInput();
}


double YerithPointDeVenteWindow::GET_CURRENT_CLIENT_AMOUNT_TO_BE_PAID()
{
    double somme_total_SANS_TVA = _sommeTotal;

    if (checkBox_tva->isChecked())
    {
        somme_total_SANS_TVA = _sommeTotal - _tva;
    }

    return (_remise_somme_total_prix < somme_total_SANS_TVA) ?
           (somme_total_SANS_TVA - _remise_somme_total_prix) : _sommeTotal;
}


void
YerithPointDeVenteWindow::
disconnect_all_objects_for_stock_article_item_selection()
{
    disconnect(lineEdit_recherche_article, 0, 0, 0);
    disconnect(lineEdit_recherche_article_codebar, 0, 0, 0);

    QCompleter *
    aQCompleter = lineEdit_recherche_article->getMyQCompleter();

    if (0 != aQCompleter)
    {
        disconnect(aQCompleter, 0, 0, 0);
    }

    aQCompleter = lineEdit_recherche_article_codebar->getMyQCompleter();

    if (0 != aQCompleter)
    {
        disconnect(aQCompleter, 0, 0, 0);
    }
}


void YerithPointDeVenteWindow::connect_manual_selection_of_article_item()
{
    disconnect_all_objects_for_stock_article_item_selection();

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
              YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
              YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else				//ALL == YerithConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
}


void YerithPointDeVenteWindow::connect_barcode_reader_selection_of_article_item()
{
    disconnect_all_objects_for_stock_article_item_selection();

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar,
                SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
              YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar,
                SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
              YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar,
                SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else				//ALL == YerithConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_stockname(const QString &)));

        connect(lineEdit_recherche_article_codebar,
                SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
}


void YerithPointDeVenteWindow::setupLineEdits()
{
    setupRemises(false);

    lineEdit_recherche_article->set_show_ASSET(false);

    lineEdit_articles_nom_client->setPlaceholderText(QObject::tr("Nom du client"));

    lineEdit_recherche_article_codebar->set_show_ASSET(false);

    lineEdit_recherche_article->enableForSearch(QObject::tr
                                                ("désignation [ focus avec F12 ]"));

    lineEdit_recherche_article_codebar->enableForSearch(QObject::tr
                                                        ("référence [ focus avec F11 ]"));

    lineEdit_articles_FIDELITE_RABAIS->setYerithEnabled(false);
    lineEdit_articles_FIDELITE_RABAIS->clear();

    lineEdit_articles_imprimante->setText(YerithERPConfig::printer);
    lineEdit_articles_imprimante->setReadOnly(true);

    lineEdit_articles_PDV->setYerithEnabled(false);
    lineEdit_article_detail_nom_client->setYerithEnabled(false);
    lineEdit_article_detail_reference_produit->setYerithEnabled(false);
    lineEdit_article_detail_designation->setYerithEnabled(false);
    lineEdit_article_detail_nom_entreprise_fournisseur->setYerithEnabled
    (false);
    lineEdit_article_detail_categorie->setYerithEnabled(false);
    lineEdit_article_detail_prix_unitaire->setYerithEnabled(false);
    lineEdit_article_detail_prix_unitaire_en_gros->setYerithEnabled(false);
    lineEdit_article_detail_tva->setYerithEnabled(false);
    lineEdit_article_detail_localisation->setYerithEnabled(false);
    lineEdit_article_detail_nom_caissier->setYerithEnabled(false);
    lineEdit_articles_nom_caissier->setYerithEnabled(false);
    lineEdit_articles_total->setYerithEnabled(false);
    lineEdit_articles_SERVICE->setYerithEnabled(false);
    lineEdit_articles_tva->setYerithEnabled(false);
    lineEdit_articles_tva->setYerithEnabled(false);
    lineEdit_articles_somme_total->setYerithEnabled(false);
    lineEdit_article_detail_quantite_en_stock->setYerithEnabled(false);
    lineEdit_articles_quantite_a_vendre->setYerithEnabled(false);
    lineEdit_articles_montant_a_rembourser->setYerithEnabled(false);
    lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM(0.0));

    lineEdit_article_detail_AJOUT_COMMERCIAL->
    	setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_article_detail_remise_prix->
    	setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_article_detail_remise_pourcentage->
    	setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(false);
    lineEdit_article_detail_remise_prix->setEnabled(false);
    lineEdit_article_detail_remise_pourcentage->setEnabled(false);
}


void YerithPointDeVenteWindow::setupLineEditsQCompleters(bool
                                                         ONLY_CLIENT_NAMES_LINE_EDIT
                                                         /* = false */)
{
    lineEdit_articles_nom_client->setupMyStaticQCompleter(YerithDatabase::
                                                          CLIENTS,
                                                          YerithDatabaseTableColumn::
                                                          NOM_ENTREPRISE);

    if (ONLY_CLIENT_NAMES_LINE_EDIT)
    {
        return;
    }

    connect(lineEdit_articles_nom_client,
            SIGNAL(textEdited(const QString &)),
            this, SLOT(actualiser_toutes_valeurs(const QString &)));

    connect(lineEdit_articles_nom_client->getMyQCompleter(),
            SIGNAL(activated(const QString &)),
            this, SLOT(actualiser_toutes_valeurs(const QString &)));
}


void YerithPointDeVenteWindow::setupDateTimeEdits()
{
    dateEdit_PDV_date_vente_extra->setMinimumDate(QDate
                                                  (GET_CURRENT_DATE_YEAR.toInt
                                                   (), 1, 1));

    dateEdit_PDV_date_vente_extra->setMaximumDate(GET_CURRENT_DATE);

    dateEdit_PDV_date_vente_extra->setStartDate(GET_CURRENT_DATE);

    enable_DATA_EXTRA_VENTE(false);

    dateEdit_article_detail_date_peremption->setEnabled(false);

    checkBox_DATE_EXTRA_VENTE->setChecked(false);
}


bool YerithPointDeVenteWindow::PROCESS_CREDIT_CARD_PAYMENT()
{
    _logger->log("PROCESS_CREDIT_CARD_PAYMENT");

    //QNetworkAccessManager
    //_currentCreditCardInfo;

    return true;
}


bool YerithPointDeVenteWindow::PRE__PROCESS_CREDIT_CARD_PAYMENT()
{
    _logger->log("PRE__PROCESS_CREDIT_CARD_PAYMENT");

    //QNetworkAccessManager
    //_currentCreditCardInfo;

    if (true)
    {

        if (QMessageBox::Ok ==
                YerithQMessageBox::warning(this,
                                           QObject::tr
                                           ("PRE__PROCESS_CREDIT_CARD_PAYMENT"),
                                           QObject::tr
                                           ("Le paiement par \"carte de crédit\""
                                            " tel quel, ne peut être effectué.")))
        {
        }
        else
        {
        }

        return false;
    }

    _allWindows->_pdVenteMethodePaiementCarteCreditDialog->rendreInvisible();

    effectuer_check_out_carte_credit_carte_debit();

    return true;
}


void YerithPointDeVenteWindow::hideEvent(QHideEvent *event)
{
    _allWindows->_pdVenteMethodePaiementComptantEntreeDialog->
    rendreInvisible();
    _allWindows->_pdVenteListStocksWindow->close();
}


void YerithPointDeVenteWindow::setupShortcuts()
{
    actionRechercheArticle = new QAction(this);

    actionRechercheArticleCodebar = new QAction(this);

    //actionListerStocks = new QAction(this);

    lineEdit_recherche_article->addAction(actionRechercheArticle);

    lineEdit_recherche_article_codebar->addAction
    (actionRechercheArticleCodebar);

    //lineEdit_recherche_article_codebar->      addAction(actionListerStocks);

    connect(actionRechercheArticle, SIGNAL(triggered()), this,
            SLOT(setRechercheDesignationArticleFocus()));

    connect(actionRechercheArticleCodebar, SIGNAL(triggered()), this,
            SLOT(setRechercheCodebarArticleFocus()));

    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));

    setupShortcutActionMessageDaide(*actionAppeler_aide);

    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    setupShortcutActionAfficherPDF(*actionAfficherPDF);

    actionRechercheArticle->setShortcut(Qt::Key_F12);

    actionRechercheArticleCodebar->setShortcut(Qt::Key_F11);

    actionStocks->setShortcut(YerithUtils::LISTER_STOCKS_QKEYSEQUENCE);
}

void YerithPointDeVenteWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    setupRemises(false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_ventes->enable(this,
                                                    SLOT(retourVentes()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_vendre->enable(this, SLOT(choisir_methode_paiment()));
}

void YerithPointDeVenteWindow::definirManager()
{
    _logger->log("definirManager");

    setupRemises(true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, true);


#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(true);
    lineEdit_article_detail_remise_prix->setEnabled(true);
    lineEdit_article_detail_remise_pourcentage->setEnabled(true);
    pushButton_article_detail_retour_ventes->enable(this,
                                                    SLOT(retourVentes()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_vendre->enable(this, SLOT(choisir_methode_paiment()));
}


void YerithPointDeVenteWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    setupRemises(false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(false);
    lineEdit_article_detail_remise_prix->setEnabled(false);
    lineEdit_article_detail_remise_pourcentage->setEnabled(false);

    pushButton_article_detail_retour_ventes->enable(this,
                                                    SLOT(retourVentes()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_vendre->enable(this, SLOT(choisir_methode_paiment()));
}


void YerithPointDeVenteWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    setupRemises(false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_article_detail_retour_ventes->disable(this);
    pushButton_annuler->disable(this);
    pushButton_vendre->disable(this);
}


void YerithPointDeVenteWindow::CHANGER_DE_LANGUE()
{
    YerithWindowsCommons::CHANGER_DE_LANGUE(this,
                                            action_TRADUIRE_EN_ANGLAIS);

    rendreVisible(_curStocksTableModel);
}


QString YerithPointDeVenteWindow::afficher_recu_vendu_pdf(QString referenceRecu /* = QString("") */)
{
    QString pdfReceiptFileName;

    if (YerithERPConfig::RECEIPT_FORMAT_PETIT == YerithERPConfig::receiptFormat)
    {
        pdfReceiptFileName.append(imprimer_recu_vendu_petit(referenceRecu));
    }
    else
    {
        pdfReceiptFileName.append(imprimer_recu_vendu_grand(referenceRecu));
    }

    if (!pdfReceiptFileName.isEmpty())
    {
        return YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    return "";
}


QString YerithPointDeVenteWindow::imprimer_recu_vendu(QString referenceRecu)
{
    if (!checkBox_imprimer_recu_vendu->isChecked())
    {
        return YerithUtils::EMPTY_STRING;
    }

    QString pdfReceiptFileName;

    if (YerithERPConfig::RECEIPT_FORMAT_PETIT == YerithERPConfig::receiptFormat)
    {
        pdfReceiptFileName.append(imprimer_recu_vendu_petit(referenceRecu));

        if (YerithUtils::isEqualCaseInsensitive(YerithERPConfig::printer,
                                                YerithUtils::IMPRIMANTE_PDF))
        {
            return YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
        }
    }
    else
    {
        pdfReceiptFileName.append(imprimer_recu_vendu_grand(referenceRecu));

        if (YerithUtils::isEqualCaseInsensitive(YerithERPConfig::printer,
                                                YerithUtils::IMPRIMANTE_PDF))
        {
            return YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
        }
    }

    if (YerithUtils::isEqualCaseInsensitive
            (YerithUtils::IMPRIMANTE_EPSON_TM_T20ii, YerithERPConfig::printer)
            || YerithUtils::isEqualCaseInsensitive(YerithUtils::
                                                   IMPRIMANTE_EPSON_TM_T20ii_RESEAU,
                                                   YerithERPConfig::printer))
    {
        QString thermalPrinterTxtFileEnding("yerith-erp-3-0-txt");

        QString pdfReceiptFileName_txt(QString("%1-%2")
        								.arg(pdfReceiptFileName,
                                             thermalPrinterTxtFileEnding));

        QStringList progOptions;

        progOptions << "-layout";
        progOptions << pdfReceiptFileName;
        progOptions << pdfReceiptFileName_txt;

        bool pdftotext_RETURN_VALUE =
        		YerithERPProcess::startAndWaitForFinished("/usr/bin/pdftotext",
                                                          progOptions,
                                                          YerithUtils::
                                                          PRINT_TIMEOUT);

        bool pdfReceiptFileName_txt_CREATED = false;

        if (true == pdftotext_RETURN_VALUE)
        {
            QThread::sleep(0.5);

            progOptions.clear();

            QString LINE_FEED_CHARACTERS_FOR_THERMAL_SMALL_PRINTING;

            for (unsigned int k = 0;
                 k < YerithERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING;
                 ++k)
            {
                LINE_FEED_CHARACTERS_FOR_THERMAL_SMALL_PRINTING.append("\n");
            }

            progOptions << "-c";

            progOptions << QString("/bin/echo -e \"%1\" >> %2")
            				.arg(LINE_FEED_CHARACTERS_FOR_THERMAL_SMALL_PRINTING,
            					 pdfReceiptFileName_txt);

            pdfReceiptFileName_txt_CREATED =
            		YerithERPProcess::startAndWaitForFinished("/bin/bash",
                                                              progOptions,
                                                              YerithUtils::PRINT_TIMEOUT);

            QThread::sleep(0.1);
        }

        if (true == pdfReceiptFileName_txt_CREATED)
        {
            if (YerithUtils::isEqualCaseInsensitive(YerithUtils::IMPRIMANTE_EPSON_TM_T20ii,
            										YerithERPConfig::printer))
            {
                progOptions.clear();

                progOptions << "-c";

                progOptions << QString("/bin/cat %1 >> %2")
                				.arg(pdfReceiptFileName_txt,
                					 YerithERPConfig::pathToThermalPrinterDeviceFile);


                YerithERPProcess::startAndWaitForFinished("/bin/bash",
                                                          progOptions,
                                                          YerithUtils::
                                                          PRINT_TIMEOUT);


                if (YerithERPConfig::ouvrirRegistreDeCaisse)
                {
                    QThread::sleep(0.5);

                    progOptions.clear();

                    progOptions << "-c";

                    progOptions << QString("/bin/echo -e -n \"\\x1b\\x70\\x00\\x19\\xfa\" >> %1")
                    				.arg(YerithERPConfig::pathToThermalPrinterDeviceFile);


                    QProcess::startDetached("/bin/bash", progOptions,
                                            YerithERPConfig::
                                            temporaryFilesDir);
                }
            }
            else if (YerithUtils::isEqualCaseInsensitive(YerithUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU,
                      	  	  	  	  	  	  	  	  	 YerithERPConfig::printer))
            {
                // now we send text file content to database table
                // for network printer spooling thread.

                YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

                YerithPOSUser *aUser = _allWindows->getUser();

                if (0 != aUser)
                {
                    YerithSqlTableModel &imprimantereseau_sql_table_model =
                                    _allWindows->getSqlTableModel_imprimantereseau_recus_petits();

                    QSqlRecord record = imprimantereseau_sql_table_model.record();


                    QString pdfReceiptFileName_txt_CONTENT;

                    QFile pdfReceiptFile(pdfReceiptFileName_txt);

                    YerithUtils::YERITH_READ_FILE_CONTENT(pdfReceiptFile,
                                                          pdfReceiptFileName_txt_CONTENT);


                    record.setValue(YerithDatabaseTableColumn::ID,
                                    YerithERPWindows::getNextIdSqlTableModel_imprimantereseau_recus_petits());

                    record.setValue(YerithDatabaseTableColumn::NOM_UTILISATEUR,
                                    aUser->nom_utilisateur());

                    record.setValue(YerithDatabaseTableColumn::NOM_COMPLET,
                                    aUser->nom_complet());

                    record.setValue(YerithDatabaseTableColumn::MESSAGE_PDV_RECU_PETIT,
                    				pdfReceiptFileName_txt_CONTENT);

                    record.setValue(YerithDatabaseTableColumn::ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE,
                    				YerithERPConfig::IP_ADDRESS_NETWORK_printer);

//                                      QDEBUG_STRING_OUTPUT_2("MESSAGE_PDV_RECU_PETIT", pdfReceiptFileName_txt);

//                                      QDEBUG_STRING_OUTPUT_2("MESSAGE_PDV_RECU_PETIT contenu", pdfReceiptFileName_txt_CONTENT);
//
//                              QDEBUG_STRING_OUTPUT_2("MESSAGE_PDV_RECU_PETIT length",
//                                              QString::number(pdfReceiptFileName_txt_CONTENT.length()));

                    bool success =
                    		imprimantereseau_sql_table_model
                    			.insertNewRecord(record,
                                    			 this,
												 "src/windows/pos/yerith-erp-pointdevente-window.cpp",
												 1128,
												 aUser->nom_utilisateur());

                    if (!success)
                    {
                        YerithQMessageBox::information(this,
                                                       QObject::tr("impression imprimante réseau"),
                                                       QObject::tr("L'impression du reçu sur l'imprimante réseau "
                                                    		   	   	   "ne pouvait pas être réalisée !"));
                    }
                }

                YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
            }

            return pdfReceiptFileName;
        }
    }

    return YerithUtils::EMPTY_STRING;
}


QString YerithPointDeVenteWindow::imprimer_recu_vendu_grand(QString
                                                            referenceRecuGRAND
                                                            /* = QString("") */)
{
    _logger->log("imprimer_facture_grand");

    QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerithQMessageBox::information(this, QObject::tr("impression"),
                                       QObject::tr
                                       ("Il n'y a pas de données à imprimer !"));

        return YerithUtils::EMPTY_STRING;
    }

    YerithUtils::getFactureTexTableString(factureTexTable,
                                          *tableWidget_articles,
                                          _quantiteAVendre,
										  _tva,
										  _sommeTotal);

    YerithPOSUser *yerithUser = _allWindows->getUser();

    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVille_LATEX());


    if (!checkBox_DATE_EXTRA_VENTE->isChecked())
    {
        YerithUtils::getCurrentLocaleDate(factureDate);
    }
    else
    {
        YerithUtils::getCurrentLocaleDate(factureDate,
                                          dateEdit_PDV_date_vente_extra->date());
    }


    YerithUtils::get_RECU_TexDocumentString(factureTexDocument,
                                            factureTexTable);

    QString typeDeVenteStr(YerithUtils::_typedeventeToUserViewString.
                           value(_typeDeVente));

    factureTexDocument.replace("YERITHTYPEPAIEMENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (typeDeVenteStr));

    if (referenceRecuGRAND.isEmpty())
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }
    else
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (referenceRecuGRAND));
    }

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
             YerithERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YERITHPAPERSPEC", "a4paper");
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER,
              YerithERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YERITHPAPERSPEC", "a5paper");
    }

    factureTexDocument.replace("YERITHPAIEMENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (typeDeVenteStr));
    factureTexDocument.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    factureTexDocument.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());
    factureTexDocument.replace("YERITHLOCALISATION_SORTIE",
                               infoEntreprise.getLocalisation_LATEX());
    factureTexDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    factureTexDocument.replace("YERITHEMAIL",
                               infoEntreprise.getEmail_LATEX());
    factureTexDocument.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());
    factureTexDocument.replace("YERITHDATE", factureDate);
    factureTexDocument.replace("YERITHHEUREVENTE", CURRENT_TIME);
    factureTexDocument.replace("YERITHVENDEUR", yerithUser->nom_completTex());
    factureTexDocument.replace("YERITHCOMPTEBANCAIRENR",
                               infoEntreprise.getNumeroCompteBancaire());
    factureTexDocument.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YERITHAGENCECOMPTEBANCAIRE",
                               infoEntreprise.getAgenceCompteBancaireTex());

    factureTexDocument.replace("YERITHSOMME",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (GET_CURRENCY_STRING_NUM(_sommeTotal)));


    QString
    YERITHMONTANTLITERAL(QString("'%1' %2").arg
                         (GET_SPELLED_NUMBER(_sommeTotal),
                          YerithERPConfig::currency));

//    QDEBUG_STRING_OUTPUT_2("YERITHMONTANTLITERAL", YERITHMONTANTLITERAL);

    factureTexDocument.replace("YERITHMONTANTLITERAL",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (YERITHMONTANTLITERAL.toUpper()));

    QString nomClient(lineEdit_articles_nom_client->text());

    bool NAME_ONLY_FOR_DISPLAY = true;

    if (nomClient.isEmpty())
    {
        nomClient.append("Client \"DIVERS\"");
        factureTexDocument.replace("YERITHCLIENT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (nomClient));
        factureTexDocument.replace("CLIENTYERITHREPRESENTANT", "");
        factureTexDocument.replace("CLIENTYERITHCITY", "");
        factureTexDocument.replace("CLIENTYERITHPOBOX", "");
        factureTexDocument.replace("CLIENTYERITHMAIL", "");
        factureTexDocument.replace("CLIENTYERITHPHONE", "");
    }
    else if (!nomClient.isEmpty())
    {
    	YerithSqlTableModel &clientTableModel =
    			_allWindows->getSqlTableModel_clients();

    	clientTableModel.yerithSetFilter_WITH_where_clause(QString
    			("nom_entreprise = '%1'").
				arg(nomClient));

    	factureTexDocument.replace("YERITHCLIENT",
    			YerithUtils::LATEX_IN_OUT_handleForeignAccents(nomClient));

    	if (checkBox_enregistrer_client->isChecked() ||
    		clientTableModel.easySelect("src/windows/pos/yerith-erp-pointdevente-windows.cpp", 1228) > 0)
    	{
    		NAME_ONLY_FOR_DISPLAY = false;

    		QSqlRecord record = clientTableModel.record(0);

    		QString
			clientRepresentant(GET_SQL_RECORD_DATA
					(record,
							YerithDatabaseTableColumn::
							NOM_REPRESENTANT));
    		QString
			clientVille(GET_SQL_RECORD_DATA
					(record, YerithDatabaseTableColumn::VILLE));
    		QString
			clientPOBox(GET_SQL_RECORD_DATA
					(record, YerithDatabaseTableColumn::BOITE_POSTALE));
    		QString
			clientEmail(GET_SQL_RECORD_DATA
					(record, YerithDatabaseTableColumn::EMAIL));
    		QString
			clientTel(GET_SQL_RECORD_DATA
					(record,
							YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));

    		if (clientTel.isEmpty())
    		{
    			clientTel.append(GET_SQL_RECORD_DATA
    					(record,
    							YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
    		}

    		//qDebug() << "++ clientEmail: " << clientEmail;
    		if (!clientRepresentant.isEmpty())
    		{
    			factureTexDocument.replace("CLIENTYERITHREPRESENTANT",
    					YerithUtils::
						LATEX_IN_OUT_handleForeignAccents
						(clientRepresentant));
    		}
    		else
    		{
    			factureTexDocument.replace("CLIENTYERITHREPRESENTANT", "");
    		}

    		if (!clientVille.isEmpty())
    		{
    			factureTexDocument.replace("CLIENTYERITHCITY",
    					YerithUtils::
						LATEX_IN_OUT_handleForeignAccents
						(clientVille));
    		}
    		else
    		{
    			factureTexDocument.replace("CLIENTYERITHCITY", "");
    		}

    		if (!clientPOBox.isEmpty())
    		{
    			factureTexDocument.replace("CLIENTYERITHPOBOX", clientPOBox);
    		}
    		else
    		{
    			factureTexDocument.replace("CLIENTYERITHPOBOX", "N/a");
    		}

    		if (!clientEmail.isEmpty())
    		{
    			factureTexDocument.replace("CLIENTYERITHMAIL",
    					YerithUtils::LATEX_IN_OUT_handleForeignAccents(clientEmail));
    		}
    		else
    		{
    			factureTexDocument.replace("CLIENTYERITHMAIL", "");
    		}

    		if (!clientTel.isEmpty())
    		{
    			factureTexDocument.replace("CLIENTYERITHPHONE",
    					YerithUtils::
						LATEX_IN_OUT_handleForeignAccents
						(clientTel));
    		}
    		else
    		{
    			factureTexDocument.replace("CLIENTYERITHPHONE", "");
    		}

    		clientTableModel.resetFilter();
    	}
    	else if (NAME_ONLY_FOR_DISPLAY)
    	{
    		// nomClient is ONLY HERE for display purposes
    		factureTexDocument.replace("YERITHCLIENT",
    									YerithUtils::LATEX_IN_OUT_handleForeignAccents(nomClient));
    		factureTexDocument.replace("CLIENTYERITHREPRESENTANT", "");
    		factureTexDocument.replace("CLIENTYERITHCITY", "");
    		factureTexDocument.replace("CLIENTYERITHPOBOX", "");
    		factureTexDocument.replace("CLIENTYERITHMAIL", "");
    		factureTexDocument.replace("CLIENTYERITHPHONE", "");
    	}
    }


    QString prefixFileName;

#ifdef YERITH_FRANCAIS_LANGUAGE

    prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yerith-erp-vente-facture-grand"));

#endif

#ifdef YERITH_ENGLISH_LANGUAGE

    prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yerith-erp-sale-receipt-big"));

#endif //qDebug() << "++\n" << factureTexDocument;


    QFile tmpLatexFile(prefixFileName + "tex");

    if (tmpLatexFile.open(QFile::WriteOnly))
    {
        tmpLatexFile.write(factureTexDocument.toUtf8());
    }

    tmpLatexFile.close();

    return YerithERPProcess::compileLatex(prefixFileName);
}


QString YerithPointDeVenteWindow::imprimer_recu_vendu_petit(QString
                                                            referenceRecuPETIT
                                                            /* = QString("") */)
{
    _logger->log("imprimer_facture_petit");

    QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerithQMessageBox::information(this, QObject::tr("impression"),
                                       QObject::tr
                                       ("Il n'y a pas de données à imprimer !"));
        return YerithUtils::EMPTY_STRING;
    }

    YerithPOSUser *yerithUser = _allWindows->getUser();

    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVille_LATEX());


    if (!checkBox_DATE_EXTRA_VENTE->isChecked())
    {
        YerithUtils::getCurrentSimplifiedDate(factureDate);
    }
    else
    {
        YerithUtils::getCurrentSimplifiedDate(factureDate,
                                              dateEdit_PDV_date_vente_extra->
                                              date());
    }


    YerithUtils::getFactureSmallTexTableString(factureTexTable,
                                               *tableWidget_articles,
                                               _quantiteAVendre,
                                               _tva,
                                               _sommeTotal,
                                               _montantRecu,
                                               _montantARembourser);


	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		factureTexTable.append(QString("Thank you!"));
	}
	else
	{
		factureTexTable.append(QString("MERCI!"));
	}

    YerithUtils::get_RECU_Small_TexDocumentString(factureTexDocument,
                                                  factureTexTable);


    if (referenceRecuPETIT.isEmpty())
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }
    else
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (referenceRecuPETIT));
    }

    //_logger->debug("imprimer_facture_petit",
    //QString("document: %1").arg(factureTexDocument));

    static const double INITIAL_INCH_SIZE = 4.05;

    static const double ITEM_INCH_SIZE = 0.23;

    int articleItemCount = tableWidget_articles->rowCount();

    double factureInchSize =
                    INITIAL_INCH_SIZE + (articleItemCount * ITEM_INCH_SIZE);

    QString
    minPaperHeight(QString("%1in").arg
                   (QString::number(factureInchSize, 'f', 2)));
    //qDebug() << "\t++minPaperHeight: " << minPaperHeight;

    QString typeDeVenteStr(YerithUtils::_typedeventeToUserViewString.
                           value(_typeDeVente));

    factureTexDocument.replace("YERITHPAIEMENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (typeDeVenteStr));
    factureTexDocument.replace("YERITHFACTURESMALLPAPERHEIGHT",
                               minPaperHeight);
    factureTexDocument.replace("YERITHENTREPRISE",
                               infoEntreprise.
                               getNomCommercial_truncated_FOR_SMALL_RECEIPT_LATEX
                               ());
    factureTexDocument.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());
    factureTexDocument.replace("YERITHLOCALISATION_SORTIE",
                               infoEntreprise.
                               getLocalisation_truncated_FOR_SMALL_RECEIPT_LATEX
                               ());
    factureTexDocument.replace("YERITHEMAIL",
                               infoEntreprise.getEmail_LATEX());
    factureTexDocument.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());
    factureTexDocument.replace("YERITHDATE", factureDate);
    factureTexDocument.replace("YERITHVENDEUR",
                               yerithUser->
                               nom_complet_truncated_FOR_SMALL_RECEIPT_Tex());

    QString nomClient(lineEdit_articles_nom_client->text());

    if (nomClient.isEmpty())
    {
        nomClient.append("\"DIVERS\"");
    }

    factureTexDocument.replace("YERITHCLIENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (nomClient));

    //qDebug() << "++\n" << factureTexDocument;

    QString prefixFileName;

#ifdef YERITH_FRANCAIS_LANGUAGE

    prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yerith-erp-vente-facture-petit"));

#endif

#ifdef YERITH_ENGLISH_LANGUAGE

    prefixFileName.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yerith-erp-sale-receipt-small"));

#endif //qDebug() << "++\n" << factureTexDocument;


    QFile tmpLatexFile(QString("%1tex").arg(prefixFileName));

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
                                                    factureTexDocument);

    return YerithERPProcess::compileLatex(prefixFileName);
}


void
YerithPointDeVenteWindow::
CLEAR_LOYALTY_PROGRAM_DATA_FOR_CURRENT_SESSION_SALE()
{
    _curClientName.clear();

    HANDLE_VISIBILITY_OF_LOYALTY_LABEL_AND_LINEEDIT();

    _client_group_program_TO_money_benefit.clear();

    _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT = 0.0;

    lineEdit_articles_nom_client->clear();
}


void YerithPointDeVenteWindow::annuler()
{
    CLEAR_LOYALTY_PROGRAM_DATA_FOR_CURRENT_SESSION_SALE();

    if (tableWidget_articles->rowCount() > 0)
    {
        tableWidget_articles->yerithClearTableWidgetContent();
        lineEdit_recherche_article->myClear();
        lineEdit_recherche_article_codebar->myClear();
        lineEdit_articles_total->setText(GET_CURRENCY_STRING_NUM(0.0));
        lineEdit_articles_somme_total->setText(GET_CURRENCY_STRING_NUM(0.0));
        label_total_ttc->setText(GET_CURRENCY_STRING_NUM(0.0));
        lineEdit_articles_quantite_a_vendre->clear();
        lineEdit_articles_SERVICE->setText(GET_CURRENCY_STRING_NUM(0.0));
        lineEdit_articles_tva->setText(GET_CURRENCY_STRING_NUM(0.0));
        lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM
                                                        (0.0));

        tableWidget_articles->resizeColumnsToContents();

        deleteArticleVenteInfos();

        handleTabViews();

        setRechercheLineEditFocus();

        YerithQMessageBox::information(this,
                                       QObject::tr("annulation de la vente"),
                                       QObject::tr
                                       ("Vous avez annulé la vente !"),
                                       QMessageBox::Ok);
    }

    checkBox_DATE_EXTRA_VENTE->setChecked(false);

    handle_VIEW_FONT_SIZE_change_buttons();

    disableImprimer();
}


void YerithPointDeVenteWindow::setRechercheLineEditFocus()
{
    lineEdit_recherche_article->clearQCompleterText();
    lineEdit_recherche_article_codebar->clearQCompleterText();

    if (_currentFocusSearchBar == lineEdit_recherche_article)
    {
        lineEdit_recherche_article_codebar->setFocus();
        lineEdit_recherche_article->setFocus();
    }
    else
    {
        lineEdit_recherche_article->setFocus();
        lineEdit_recherche_article_codebar->setFocus();
    }
}


void YerithPointDeVenteWindow::setRechercheDesignationArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article;
    setRechercheLineEditFocus();
}


void YerithPointDeVenteWindow::setRechercheCodebarArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article_codebar;
    setRechercheLineEditFocus();
}


void YerithPointDeVenteWindow::lister()
{
    _logger->log("lister");
    _allWindows->_pdVenteListStocksWindow->listStocks(*_curStocksTableModel);
}


void YerithPointDeVenteWindow::enable_DATA_EXTRA_VENTE(bool enable)
{
    dateEdit_PDV_date_vente_extra->setEnabled(enable);
    dateEdit_PDV_date_vente_extra->setVisible(enable);
}


void YerithPointDeVenteWindow::retourVentes()
{
    _currentTabView = TableauDesVentes;
    tabWidget_vente->setCurrentIndex(TableauDesVentes);
}


void YerithPointDeVenteWindow::handleAddedArticle()
{
    enableImprimer();
    tabWidget_vente->setTabEnabled(AfficherVenteAuDetail, true);
}


void YerithPointDeVenteWindow::handleCurrentTABChanged(int index)
{
    //_logger->debug("handleCurrentChanged(int)", QString("index: %1").arg(index));
    _currentTabView = index;
    switch (index)
    {
    case TableauDesVentes:
    	lineEdit_article_detail_AJOUT_COMMERCIAL->clear();
        afficher_tableau_ventes(tableWidget_articles->currentRow());
        break;
    case AfficherVenteAuDetail:
        afficher_vente_detail(tableWidget_articles->currentRow());
        disableImprimer();
        _logger->debug("handleCurrentChanged(int)",
                       QString("current table row: %1").arg
                       (tableWidget_articles->currentRow()));
        break;
    default:
        break;
    }
}


void YerithPointDeVenteWindow::cleanUpAfterVente()
{
    _logger->log("cleanUpAfterVente");

    set_paiement_par_carte_credit_carte_debit(false);

    set_paiement_comptant(false);

    set_paiment_compteclient_AVANCE_DE_CASH(false);

    checkBox_enregistrer_client->setChecked(false);

    resetCheckboxTVA();

    tableWidget_articles->yerithClearTableWidgetContent();
    articleItemToVenteInfo.clear();

    deleteArticleVenteInfos();

    CLEAR_LOYALTY_PROGRAM_DATA_FOR_CURRENT_SESSION_SALE();

    lineEdit_recherche_article->myClear();
    lineEdit_recherche_article_codebar->myClear();

    tabWidget_vente->setCurrentIndex(TableauDesVentes);

    handleTabViews();

    _allWindows->_pdVenteMethodePaiementComptantEntreeDialog->
    clearLineEditValue();

    _typeDeVente = YerithUtils::VENTE_INDEFINI;

    _TOTAL_PRIX_VENTE__POUR__COMPTECLIENT_AVANCE_DE_CASH = 0.0;

    lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM
                                                    (0.0));
    lineEdit_articles_SERVICE->setText(GET_CURRENCY_STRING_NUM(0.0));
    lineEdit_articles_tva->setText(GET_CURRENCY_STRING_NUM(0.0));
    lineEdit_articles_total->setText(GET_CURRENCY_STRING_NUM(0.0));
    lineEdit_articles_somme_total->setText(GET_CURRENCY_STRING_NUM(0.0));
    label_total_ttc->setText(GET_CURRENCY_STRING_NUM(0.0));

    lineEdit_articles_quantite_a_vendre->setText(GET_DOUBLE_STRING(0.0));

    lineEdit_article_detail_reference_produit->clear();
    lineEdit_article_detail_designation->clear();
    lineEdit_article_detail_nom_entreprise_fournisseur->clear();
    lineEdit_article_detail_categorie->clear();
    lineEdit_article_detail_prix_unitaire->clear();
    lineEdit_article_detail_prix_unitaire_en_gros->clear();
    lineEdit_articles_SERVICE->clear();
    lineEdit_article_detail_tva->clear();
    lineEdit_article_detail_localisation->clear();
    lineEdit_article_detail_nom_caissier->clear();
    lineEdit_article_detail_quantite_en_stock->clear();
    lineEdit_article_detail_quantite_a_vendre->clear();
    lineEdit_article_detail_AJOUT_COMMERCIAL->clear();
    lineEdit_article_detail_remise_prix->clear();
    lineEdit_article_detail_remise_pourcentage->clear();
    radioButton_article_detail_AJOUT_COMMERCIAL->clearFocus();
    radioButton_article_detail_remise_prix->clearFocus();
    radioButton_article_detail_remise_pourcentage->clearFocus();
    lineEdit_article_detail_quantite_a_vendre->setFocus();

    handleRefreshSaleStrategy();

    handle_VIEW_FONT_SIZE_change_buttons_FROM__SELLING_signal();

    setRechercheLineEditFocus();
}

void YerithPointDeVenteWindow::rendreInvisible()
{
    _logger->log("rendreInvisible");

    set_paiement_par_carte_credit_carte_debit(false);

    set_paiement_comptant(false);

    set_paiment_compteclient_AVANCE_DE_CASH(false);

    tableWidget_articles->yerithClearTableWidgetContent();

    articleItemToVenteInfo.clear();

    deleteArticleVenteInfos();

    lineEdit_articles_PDV->myClear();

    lineEdit_recherche_article->myClear();

    lineEdit_recherche_article_codebar->myClear();

    _allWindows->_pdVenteMethodePaiementComptantEntreeDialog->
    clearLineEditValue();

    _remise_somme_total_prix = 0.0;

    _remise_somme_total_pourcentage = 0.0;

    _montantRecu = 0.0;

    _montantARembourser = 0.0;

    _qteChangeCodeBar = false;

    _sommeTotal_HORS_TAXES = 0.0;

    _sommeTotal = 0.0;

    _SERVICE = 0.0;

    _tva = 0.0;

    _quantiteAVendre = 0;

    CLEAR_LOYALTY_PROGRAM_DATA_FOR_CURRENT_SESSION_SALE();

    lineEdit_articles_total->clear();

    lineEdit_articles_somme_total->clear();

    label_total_ttc->clear();

    lineEdit_articles_quantite_a_vendre->clear();

    lineEdit_articles_SERVICE->clear();

    lineEdit_articles_tva->clear();

    lineEdit_articles_nom_client->clear();

    lineEdit_articles_montant_a_rembourser->clear();


    if (0 != _curStocksTableModel)
    {
        _curStocksTableModel->resetFilter();
    }


    YerithWindowsCommons::rendreInvisible();
}


void YerithPointDeVenteWindow::handleTabViews()
{
    if (0 == tableWidget_articles->rowCount())
    {
        tabWidget_vente->setTabEnabled(AfficherVenteAuDetail, false);
    }
}


void YerithPointDeVenteWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	CHANGER_DE_LANGUE__text__TRANSLATION(this,
                                         action_TRADUIRE_EN_ANGLAIS);

	_allWindows->_mainWindow->write_AFTER_SETUP_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    _client_group_program_TO_money_benefit.clear();

    radioButton_article_detail_remise_prix
    	->setText(QObject::tr("remise (%1)")
    				.arg(YerithERPConfig::currency));

    lineEdit_articles_PDV
    	->setText(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    lineEdit_articles_imprimante->setText(YerithERPConfig::printer);

    label_articles_tva->setText(YerithUtils::getTVALabelStringWithPercent());

    _curStocksTableModel = stocksTableModel;

    _curStocksTableModel->setFilter(QString(" (%1 > 0) ").arg
                                    (YerithDatabaseTableColumn::PRIX_VENTE));

    setupLineEditsQCompleters();

    if (!_curClientName.isEmpty())
    {
        lineEdit_articles_nom_client->setText(_curClientName);

        _curClientName.clear();
    }
    else
    {
        lineEdit_articles_nom_client->clear();
    }

    resetCheckboxTVA();

    tabWidget_vente->setCurrentIndex(TableauDesVentes);

    lineEdit_articles_nom_caissier->setText(_allWindows->getUser()->nom_complet());

    handleRefreshSaleStrategy();

    handleTabViews();

    setRechercheLineEditFocus();

    QString aNewWindowTitle;

    aNewWindowTitle = YerithUtils::getWindowTitleWithStrategy(this);

    aNewWindowTitle = YerithUtils::appendPDFReceiptFormat(aNewWindowTitle);

    setWindowTitle(aNewWindowTitle);

    actualiser_toutes_valeurs();

    tableWidget_articles->resizeColumnsToContents();

    if (tableWidget_articles->rowCount() > 0)
    {
        enableImprimer();
    }
    else
    {
        disableImprimer();
    }


    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser && aUser->isManager())
    {
        label_PDV_date_vente_extra->setVisible(true);
        checkBox_DATE_EXTRA_VENTE->setVisible(true);
    }
    else
    {
        label_PDV_date_vente_extra->setVisible(false);
        checkBox_DATE_EXTRA_VENTE->setVisible(false);
    }


    setVisible(true);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("point de vente"),
      								 _curStocksTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS_QTABLEWIDGET(*this);
}


void YerithPointDeVenteWindow::activateLineEdit_AJOUT_COMMERCIAL(bool toggled)
{
    if (toggled)
    {
    	lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(true);
    }
    else
    {
    	lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(false);
    }
}


void YerithPointDeVenteWindow::activateLineEditRemisePrix(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_prix->setEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_prix->setEnabled(false);
    }
}


void YerithPointDeVenteWindow::activateLineEditRemisePourcentage(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_pourcentage->setEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_pourcentage->setEnabled(false);
    }
}


void YerithPointDeVenteWindow::calculate_details_window_AJOUT_COMMERCIAL()
{
	int tableWidgetRow = tableWidget_articles->currentRow();

	if (tableWidgetRow < 0)
	{
		return;
	}

	YerithArticleVenteInfo *articleVenteInfo =
			articleItemToVenteInfo.value(tableWidgetRow);

	if (articleVenteInfo
			&& radioButton_article_detail_AJOUT_COMMERCIAL->isChecked())
	{
		double AJOUT_pour_COMMERCIAL =
				lineEdit_article_detail_AJOUT_COMMERCIAL->text().toDouble();

		articleVenteInfo->setAJOUT_COMMERCIAL(AJOUT_pour_COMMERCIAL);
	}

	actualiser_montant_remise();
}


void YerithPointDeVenteWindow::calculate_details_window_remise_prix()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    if (articleVenteInfo
            && radioButton_article_detail_remise_prix->isChecked())
    {
        double remise_prix =
                        lineEdit_article_detail_remise_prix->text().toDouble();

        double remise_pourcentage =
                        (100.0 * remise_prix) / articleVenteInfo->prix_unitaire();

        if (remise_prix > articleVenteInfo->prix_unitaire())
        {
            lineEdit_article_detail_remise_prix->
            	setText(QString::number(0.0, 'f', 2));

            lineEdit_article_detail_remise_pourcentage->
            	setText(QString::number(0.0, 'f', 2));

            YerithQMessageBox::warning(this,
                                       QObject::tr("montant d'une remise"),
                                       QObject::tr
                                       ("Le montant d'une remise sur un article doit être inférieur "
                                        "au prix unitaire de cet article !"));
            return;
        }

        articleVenteInfo->setRemisePrix(remise_prix);

        articleVenteInfo->setRemisePourcentage(remise_pourcentage);

        lineEdit_article_detail_remise_pourcentage->setText(articleVenteInfo->
                                                            remisePourcentage
                                                            ());
    }

    actualiser_montant_remise();
}


void YerithPointDeVenteWindow::calculate_details_window_remise_pourcentage()
{
    //_logger->log("calculate_details_window_remise_pourcentage");
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    if (articleVenteInfo
            && radioButton_article_detail_remise_pourcentage->isChecked())
    {
        double remise_pourcentage =
                        lineEdit_article_detail_remise_pourcentage->text().toDouble();

        double remise_prix =
                        (articleVenteInfo->prix_unitaire() * remise_pourcentage) / 100.0;

        if (remise_prix > articleVenteInfo->prix_unitaire())
        {
            lineEdit_article_detail_remise_prix->
            setText(QString::number(0.0, 'f', 2));
            lineEdit_article_detail_remise_pourcentage->
            setText(QString::number(0.0, 'f', 2));

            YerithQMessageBox::warning(this,
                                       QObject::tr("montant d'une remise"),
                                       QObject::tr
                                       ("Le montant d'une remise sur un article doit être inférieur "
                                        "au prix unitaire de cet article !"));
            return;
        }

        articleVenteInfo->setRemisePourcentage(remise_pourcentage);

        articleVenteInfo->setRemisePrix(remise_prix);

        lineEdit_article_detail_remise_prix->setText(articleVenteInfo->
                                                     remisePrix());
    }

    actualiser_montant_remise();
}

void YerithPointDeVenteWindow::actualiser_montant_remise()
{
    //_logger->log("actualiser_montant_remise");
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    double prix_unitaire = articleVenteInfo->_prix_unitaire;

    double prix_unitaire_en_gros = articleVenteInfo->_prix_unitaire_en_gros;

    if (!checkBox_effectuer_1_vente_en_gros->isChecked())
    {
        prix_unitaire =
                        articleVenteInfo->_prix_unitaire - articleVenteInfo->remise_prix();
    }
    else
    {
        prix_unitaire_en_gros =
                        articleVenteInfo->_prix_unitaire_en_gros -
                        articleVenteInfo->remise_prix();
    }

    if (checkBox_tva->isChecked())
    {
        double montant_tva = prix_unitaire * YerithERPConfig::tva_value;

        double montant_tva_en_gros =
                        prix_unitaire_en_gros * YerithERPConfig::tva_value;

        articleVenteInfo->_montant_tva = montant_tva;

        articleVenteInfo->_montant_tva_en_gros = montant_tva_en_gros;
    }

    lineEdit_article_detail_tva->setText(articleVenteInfo->montantTva());

    lineEdit_article_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM
                                                   (prix_unitaire));


    lineEdit_article_detail_prix_unitaire_en_gros->setText
    	(GET_CURRENCY_STRING_NUM(prix_unitaire_en_gros));

    actualiser_toutes_valeurs();
}


void YerithPointDeVenteWindow::resetCheckboxTVA()
{
    _tvaCheckBoxPreviousState = false;
    checkBox_tva->setChecked(false);
}


void YerithPointDeVenteWindow::handleTVACheckBox(bool clicked)
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    QString rowStockID =
                    tableWidget_articles->get_DB_ELEMENT_db_ID(tableWidgetRow);

    QSqlQuery stockRecordQuery;

    QString
    stockRecordQueryStr(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg
                        (YerithDatabaseTableColumn::MONTANT_TVA,
                         YerithDatabase::STOCKS,
                         YerithDatabaseTableColumn::ID, rowStockID));

    double montant_tva = 0.0;

    int querySize =
                    YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
        montant_tva =
                        stockRecordQuery.value(YerithDatabaseTableColumn::MONTANT_TVA).
                        toDouble();
    }

    double prix_vente = articleVenteInfo->prix_vente();

    if (clicked)
    {
        if (false == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }
        if (montant_tva <= 0)
        {
            montant_tva = prix_vente * YerithERPConfig::tva_value;
        }

        articleVenteInfo->_montant_tva = montant_tva;
    }
    else
    {
        articleVenteInfo->_montant_tva = 0;

        if (true == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = false;
        }
    }

    actualiser_montant_remise();
}


void YerithPointDeVenteWindow::update_text_memo_SALE()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    if (!articleVenteInfo)
    {
        return;
    }

//    QDEBUG_STRING_OUTPUT_2("textEdit_MEMO_SALE_for_SEARCH",
//    						textEdit_MEMO_SALE_for_SEARCH->toPlainText());

    articleVenteInfo->_MEMO_SALE_for_SEARCH =
    		textEdit_MEMO_SALE_for_SEARCH->toPlainText();
}


void YerithPointDeVenteWindow::updateQuantiteAVendre()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    if (!articleVenteInfo)
    {
        return;
    }

    double itemNewQteDouble =
                    lineEdit_article_detail_quantite_a_vendre->text().toDouble();

    if (articleVenteInfo->_quantite_en_stock < itemNewQteDouble)
    {
        YerithQMessageBox::warning(this, QObject::tr("articles en stock"),
                                   QObject::tr
                                   ("Il n'y a pas assez de articles en stock |"));
    }
    else
    {
        articleVenteInfo->_quantite_a_vendre = itemNewQteDouble;
    }

    actualiser_montant_remise();
}

void YerithPointDeVenteWindow::afficher_tableau_ventes(const int tableWidgetRow)
{
    tableWidget_articles->yerithClearTableWidgetContent();

    QMapIterator < int,
                 YerithArticleVenteInfo * >itArticleItemVenteInfo(articleItemToVenteInfo);

    int tmp_SERVICE_COST = 0;

    while (itArticleItemVenteInfo.hasNext())
    {
        itArticleItemVenteInfo.next();

        YerithArticleVenteInfo *articleVenteInfo =
                        itArticleItemVenteInfo.value();

        if (articleVenteInfo->_quantite_a_vendre > 0)
        {
            tableWidget_articles->addArticleAfter(*articleVenteInfo);
        }

        tmp_SERVICE_COST = tmp_SERVICE_COST + articleVenteInfo->Get_ajout_pour_COMMERCIAL();
    }

    if (tableWidget_articles->rowCount() > 0)
    {
        if (tableWidgetRow >= tableWidget_articles->rowCount())
        {
            tableWidget_articles->selectRow(tableWidget_articles->rowCount() -
                                            1);
        }
        else
        {
            tableWidget_articles->selectRow(tableWidgetRow);
        }

        _SERVICE = tmp_SERVICE_COST;

        enableImprimer();
    }
    else
    {
        disableImprimer();
    }

    actualiser_toutes_valeurs();
}

void YerithPointDeVenteWindow::afficher_vente_detail(const int tableWidgetRow)
{
    _logger->log("afficher_vente_detail",
                 QString("row: %1").arg(tableWidgetRow));

    if (tableWidgetRow < 0)
    {
        return;
    }

    checkBox_tva->setText(YerithUtils::getTVALabelStringWithPercent());

    QString rowStockID =
                    tableWidget_articles->get_DB_ELEMENT_db_ID(tableWidgetRow);

    /**
     * I set the stock ID of the current article
     * in detail view !
     */
    _currentStocksID = rowStockID;

    QSqlQuery stockRecordQuery;

    QString
    stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'").arg
                        (YerithDatabase::STOCKS,
                         YerithDatabaseTableColumn::ID, rowStockID));

    QVariant img;

    QDate date_peremption;

    QString localisation;

    QString nom_entreprise_fournisseur;

    int querySize =
                    YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
        img = stockRecordQuery.value(YerithDatabaseTableColumn::IMAGE_PRODUIT);

        date_peremption =
                        stockRecordQuery.value(YerithDatabaseTableColumn::DATE_PEREMPTION).
                        toDate();

        localisation =
                        stockRecordQuery.value(YerithDatabaseTableColumn::LOCALISATION).
                        toString();

        nom_entreprise_fournisseur =
                        stockRecordQuery.
                        value(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).
                        toString();
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    if (articleVenteInfo->_effectuer_vente_en_gros)
    {
        checkBox_effectuer_1_vente_en_gros->setChecked(true);
    }
    else
    {
        checkBox_effectuer_1_vente_en_gros->setChecked(false);
    }

    lineEdit_article_detail_reference_produit->setText(articleVenteInfo->
                                                       _stockReference);

    lineEdit_article_detail_designation->setText(articleVenteInfo->_stockName);

    lineEdit_article_detail_nom_entreprise_fournisseur->setText
    	(nom_entreprise_fournisseur);

    lineEdit_article_detail_categorie->setText(articleVenteInfo->
                                               _stockCategorie);

    if (!img.isNull())
    {
        YerithUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);
    }


    lineEdit_article_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM
                                                   (articleVenteInfo->_prix_unitaire));

    lineEdit_article_detail_prix_unitaire_en_gros->setText
    	(GET_CURRENCY_STRING_NUM(articleVenteInfo->_prix_unitaire_en_gros));

    lineEdit_article_detail_quantite_en_stock->setText(articleVenteInfo->
                                                       quantiteEnStock());

    lineEdit_article_detail_quantite_a_vendre->setText(articleVenteInfo->
                                                       quantiteAVendre());

    lineEdit_article_detail_AJOUT_COMMERCIAL
		->setText(QString::number(articleVenteInfo->Get_ajout_pour_COMMERCIAL()));

    lineEdit_article_detail_remise_prix->setText(articleVenteInfo->
                                                 remisePrix());

    lineEdit_article_detail_remise_pourcentage->setText(articleVenteInfo->
                                                        remisePourcentage());

    if (articleVenteInfo->montant_tva() > 0)
    {
        if (checkBox_tva->isChecked())
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }

        checkBox_tva->setChecked(true);

        lineEdit_article_detail_tva->setText(articleVenteInfo->montantTva());
    }
    else
    {
        if (checkBox_tva->isChecked())
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }

        checkBox_tva->setChecked(false);

        lineEdit_article_detail_tva->setText(YerithUtils::EMPTY_STRING);
    }

    dateEdit_article_detail_date_peremption->setDate(date_peremption);

    lineEdit_article_detail_nom_caissier->setText(_allWindows->getUser()->
                                                  nom_complet());

	lineEdit_article_detail_localisation->setText(localisation);

    lineEdit_article_detail_nom_client->setText(lineEdit_articles_nom_client->
                                                text());

    actualiser_montant_remise();

    tabWidget_vente->setCurrentIndex(AfficherVenteAuDetail);
}


void YerithPointDeVenteWindow::
	HANDLE_VISIBILITY_OF_LOYALTY_LABEL_AND_LINEEDIT(const QString &text /* = YerithUtils::EMPTY_STRING */
)
{
    if (!text.isEmpty())
    {
        label_FIDELITE_RABAIS->setVisible(true);
        lineEdit_articles_FIDELITE_RABAIS->setText(text);
        lineEdit_articles_FIDELITE_RABAIS->setVisible(true);
    }
    else
    {
        label_FIDELITE_RABAIS->setVisible(false);
        lineEdit_articles_FIDELITE_RABAIS->clear();
        lineEdit_articles_FIDELITE_RABAIS->setVisible(false);
    }
}


void YerithPointDeVenteWindow::handle_VIEW_FONT_SIZE_change_buttons(bool CLEAR_AFTER_SELLING /* = false */)
{
	if (! CLEAR_AFTER_SELLING)
	{
		if (tableWidget_articles->itemCount() > 0)
		{
            pushButton_minus->enable(this, SLOT(retirer_un_article()));

            YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
            YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, true);
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);
		}
		else
		{
            pushButton_minus->disable(this);

            YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
            YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, false);
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);
		}
	}
	else
	{
        pushButton_minus->disable(this);

        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETIRERARTICLE, false);
		YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
		YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);
	}
}


void YerithPointDeVenteWindow::handleQteChange(QTableWidgetItem *itemChanged)
{
    if (itemChanged
            && itemChanged->column() == YerithPointDeVenteTableWidget::QTE_COLUMN)
    {
        _updateItemConversionError = false;

        double newQteValue =
                        itemChanged->text().toDouble(&_updateItemConversionError);

        if (newQteValue <= 0)
        {
            itemChanged->setText(_previousPressedQteValue);
            return;
        }
        //No conversion error occurred.
        if (true == _updateItemConversionError)
        {
            QString rowStockID =
                            tableWidget_articles->get_DB_ELEMENT_db_ID(itemChanged->row());

            QSqlQuery stockRecordQuery;

            QString
            stockRecordQueryStr(QString
                                ("SELECT * FROM %1 WHERE %2 = '%3'").arg
                                (YerithDatabase::STOCKS,
                                 YerithDatabaseTableColumn::ID, rowStockID));

            double montantTva = 0.0;
            double montantTva_en_gros = 0.0;

            double prixVente = 0.0;
            double prixVente_en_gros = 0.0;

            double qteEnStock = 0.0;

            QString stockReference;
            QString stockName;
            QString stockNomDepartementProduits;
            QString stockcategorie;

            int querySize =
                            YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr,
                                                   _logger);

            if (querySize > 0 && stockRecordQuery.next())
            {
                montantTva =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::MONTANT_TVA).toDouble();
                montantTva_en_gros =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS).
                                toDouble();

                prixVente =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::PRIX_VENTE).toDouble();
                prixVente_en_gros =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS).
                                toDouble();

                qteEnStock =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::QUANTITE_TOTALE).
                                toDouble();

                stockReference =
                                stockRecordQuery.value(YerithDatabaseTableColumn::REFERENCE).
                                toString();
                stockName =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::DESIGNATION).toString();
                stockNomDepartementProduits =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT).
                                toString();
                stockcategorie =
                                stockRecordQuery.value(YerithDatabaseTableColumn::CATEGORIE).
                                toString();
            }

            YerithArticleVenteInfo *articleVenteInfo =
                            articleItemToVenteInfo.value(itemChanged->row());

            bool qteEnStock_OK = true;

            if (0 == articleVenteInfo)
            {
                articleVenteInfo = new YerithArticleVenteInfo;

                articleVenteInfo->_stockReference = stockReference;
                articleVenteInfo->_stockName = stockName;
                articleVenteInfo->_stockNomDepartementProduits =
                                stockNomDepartementProduits;
                articleVenteInfo->_stockCategorie = stockcategorie;
                articleVenteInfo->_stockID = rowStockID;
                articleVenteInfo->_quantite_en_stock = qteEnStock;

                articleVenteInfo->_montant_tva = montantTva;
                articleVenteInfo->_montant_tva_en_gros = montantTva_en_gros;

                articleVenteInfo->_prix_unitaire = prixVente - montantTva;
                articleVenteInfo->_prix_unitaire_en_gros =
                                prixVente_en_gros - montantTva_en_gros;

                articleVenteInfo->_quantite_a_vendre = newQteValue;

                articleItemToVenteInfo.insert(itemChanged->row(),
                                              articleVenteInfo);
            }
            else
            {
                if (articleVenteInfo->_quantite_en_stock < newQteValue)
                {
                    qteEnStock_OK = false;

                    YerithQMessageBox::warning(this,
                                               QObject::tr
                                               ("articles en stock"),
                                               QObject::tr
                                               ("Il n'y a pas assez de articles en stock !"));

                    itemChanged->setText(articleVenteInfo->quantiteAVendre());
                }
                else
                {
                    articleVenteInfo->_quantite_a_vendre = newQteValue;
                }
            }

            if (qteEnStock_OK)
            {
                if (!_qteChangeCodeBar)
                {
                    actualiser_articles(itemChanged->row(), newQteValue);
                }
                else
                {
                    actualiser_articles_codebar(itemChanged->row(),
                                                newQteValue);
                }
            }
        }
        else
        {
            itemChanged->setText(YerithPointDeVenteTableWidget::QTE_1);
        }
    }

    setRechercheLineEditFocus();
}


bool YerithPointDeVenteWindow::article_exist(const QString codeBar,
                                             const QString designation)
{
    QTableWidgetItem *anItem = 0;

    int maxItems = tableWidget_articles->rowCount();

    for (int k = 0; k < maxItems; ++k)
    {
        anItem =
                        tableWidget_articles->item(k,
                                                   YerithPointDeVenteTableWidget::
                                                   REFERENCE_COLUMN);

        if (anItem
                && YerithUtils::isEqualCaseInsensitive(codeBar, anItem->text()))
        {
            anItem =
                            tableWidget_articles->item(k,
                                                       YerithPointDeVenteTableWidget::
                                                       DESIGNATION_COLUMN);

            if (anItem
                    && YerithUtils::isEqualCaseInsensitive(designation,
                                                           anItem->text()))
            {
                anItem =
                                tableWidget_articles->item(k,
                                                           YerithPointDeVenteTableWidget::
                                                           QTE_COLUMN);

                int previousItemQty = anItem->text().toInt();
                int newItemQty = previousItemQty + 1;

                anItem->setText(QString::number(newItemQty));

                return true;
            }
        }
    }

    return false;
}


void YerithPointDeVenteWindow::ajouter_article(const QString &aStockID)
{
    QSqlQuery stockRecordQuery;

    QString
    stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'").arg
                        (YerithDatabase::STOCKS,
                         YerithDatabaseTableColumn::ID, aStockID));

    double montantTva = 0.0;
    double prixVente = 0.0;
    double qteEnStock = 0.0;

    QString stockReference;
    QString stockName;
    QString stockCategorie;

    int querySize =
                    YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
        montantTva =
                        stockRecordQuery.value(YerithDatabaseTableColumn::MONTANT_TVA).
                        toDouble();

        prixVente =
                        stockRecordQuery.value(YerithDatabaseTableColumn::PRIX_VENTE).
                        toDouble();
        qteEnStock =
                        stockRecordQuery.value(YerithDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        stockName =
                        stockRecordQuery.value(YerithDatabaseTableColumn::DESIGNATION).
                        toString();
        stockReference =
                        stockRecordQuery.value(YerithDatabaseTableColumn::REFERENCE).
                        toString();
        stockCategorie =
                        stockRecordQuery.value(YerithDatabaseTableColumn::CATEGORIE).
                        toString();

        if (article_exist(stockReference, stockName))
        {
            return;
        }
    }
    else
    {
        return;
    }

    double prix_unitaire = prixVente - montantTva;

    if (prix_unitaire < 0)
    {
        QString warningMsg(QObject::tr
                           ("Prix unitaire inférieur à zéro pour l'article '%1' !").
                           arg(stockName));

        if (QMessageBox::Ok ==
                YerithQMessageBox::warning(this,
                                           QObject::tr
                                           ("prix unitaire d'un article"),
                                           warningMsg))
        {
            return;
        }
    }

    //Each call to YerithPointDeVenteTableWidget::setItem in method 'YerithPointDeVenteTableWidget::addArticle'
    //triggers a call to YerithPointDeVenteWindow::handleQteChange
    int lastCurRow = tableWidget_articles->addArticle(aStockID,
                                                      stockReference,
                                                      stockName,
                                                      stockCategorie,
                                                      QString::number(prix_unitaire, 'f', 2),
                                                      QString::number(montantTva, 'f', 2),
													  QString::number(_SERVICE, 'f', 2),
													  QString::number(prixVente, 'f', 2),
                                                      YerithPointDeVenteTableWidget::QTE_1,
                                                      QString::number(qteEnStock));
    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }
}


void YerithPointDeVenteWindow::ajouter_article_stockname(const QString &
                                                         aStockName)
{
    _qteChangeCodeBar = false;

    QMap < QString, QString > stockNameToStockID =
                    lineEdit_recherche_article->getStockNameToStockID();

    if (!stockNameToStockID.contains(aStockName))
    {
        return;
    }

    QString stockID = stockNameToStockID.value(aStockName);

    ajouter_article(stockID);
}


void YerithPointDeVenteWindow::ajouter_article_codebar(const QString &
                                                       aStockReference)
{
    QMap < QString, QString > stockReferenceToStockID =
                    lineEdit_recherche_article_codebar->getStockreferenceCodebarToStockID();

    if (!stockReferenceToStockID.contains(aStockReference))
    {
        return;
    }

    QString stockID = stockReferenceToStockID.value(aStockReference);

    ajouter_article(stockID);
}


/**
   * This method is called by 'YerithPointDeVenteWindow::handleQteChange'
   */

void YerithPointDeVenteWindow::actualiser_articles_codebar(int row,
                                                           unsigned newItemQte)
{
    _qteChangeCodeBar = true;

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;

    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
        YerithSqlTableModel &articleSqlTableModel =
                        *lineEdit_recherche_article_codebar->getMySqlTableModel();

        YerithArticleVenteInfo *articleVenteInfo =
                        articleItemToVenteInfo.value(k);

        if (row != k)
        {
            curTableWidgetItem =
                            tableWidget_articles->item(k,
                                                       YerithPointDeVenteTableWidget::
                                                       QTE_COLUMN);
            curTableWidgetItemQte = curTableWidgetItem->text().toDouble();
        }
        else
        {
            //To update the table values after modifying the quantity value of an item
            curTableWidgetItemQte = newItemQte;

            QTableWidgetItem *totalTvaWidgetItem =
                            tableWidget_articles->item(k,
                                                       YerithPointDeVenteTableWidget::
                                                       TOTAL_TVA_COLUMN);

            QTableWidgetItem *totalWidgetItem = tableWidget_articles->item(k,
                                                                           YerithPointDeVenteTableWidget::
                                                                           TOTAL_COLUMN);

            if (totalTvaWidgetItem)
            {
                totalTvaWidgetItem->setText(articleVenteInfo->montantTva());
            }

            //We must always keep this goto break to avoid false updates
            if (!totalWidgetItem)
            {
                goto myItemBreak;
            }

            totalWidgetItem->setText(GET_CURRENCY_STRING_NUM
                                     (articleVenteInfo->prix_vente()));
        }

myItemBreak:
        quantiteVendue += curTableWidgetItemQte;
        tva += (curTableWidgetItemQte * articleVenteInfo->montant_tva());
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteAVendre = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva;

    update_lineedits_and_labels(total);

    tableWidget_articles->resizeColumnsToContents();
}

/**
  * This method is called by 'YerithPointDeVenteWindow::handleQteChange'
  */

void YerithPointDeVenteWindow::actualiser_articles(int row, unsigned newItemQte)
{
    _logger->log("actualiser_articles(int, unsigned)",
                 QString("row: %1, quantite: %2").arg(QString::number(row),
                                                      QString::number
                                                      (newItemQte)));

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
        YerithSqlTableModel &articleSqlTableModel =
                        *lineEdit_recherche_article->getMySqlTableModel();

        YerithArticleVenteInfo *articleVenteInfo =
                        articleItemToVenteInfo.value(k);

        if (row != k)
        {
            curTableWidgetItem =
                            tableWidget_articles->item(k,
                                                       YerithPointDeVenteTableWidget::
                                                       QTE_COLUMN);
            curTableWidgetItemQte = curTableWidgetItem->text().toDouble();
        }
        else
        {
            //To update the table values after modifying the quantity value of an item
            curTableWidgetItemQte = newItemQte;

            QTableWidgetItem *totalTvaWidgetItem =
                            tableWidget_articles->item(k,
                                                       YerithPointDeVenteTableWidget::
                                                       TOTAL_TVA_COLUMN);

            QTableWidgetItem *totalWidgetItem = tableWidget_articles->item(k,
                                                                           YerithPointDeVenteTableWidget::
                                                                           TOTAL_COLUMN);

            if (totalTvaWidgetItem)
            {
                totalTvaWidgetItem->setText(articleVenteInfo->montantTva());
            }

            //We must always keep this goto break to avoid false updates
            if (!totalWidgetItem)
            {
                goto myItemBreak;
            }

            totalWidgetItem->setText(QString::number
                                     (articleVenteInfo->prix_vente(), 'f',
                                      2));
        }

myItemBreak:
        quantiteVendue += curTableWidgetItemQte;
        tva += (curTableWidgetItemQte * articleVenteInfo->montant_tva());
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteAVendre = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva;

    update_lineedits_and_labels(total);

    tableWidget_articles->resizeColumnsToContents();
}

void YerithPointDeVenteWindow::actualiser_tableau_vente()
{
    int tableRowCount = tableWidget_articles->rowCount();

    _logger->log("actualiser_tableau_vente",
                 QString("tableRowCount: %1").arg(tableRowCount));

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableRowCount; ++k)
    {
        YerithArticleVenteInfo *articleVenteInfo =
                        articleItemToVenteInfo.value(k);

        curTableWidgetItem =
                        tableWidget_articles->item(k,
                                                   YerithPointDeVenteTableWidget::
                                                   QTE_COLUMN);

        curTableWidgetItemQte = curTableWidgetItem->text().toDouble();

        quantiteVendue += curTableWidgetItemQte;

        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);

        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteAVendre = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva;

    update_lineedits_and_labels(total);

    tableWidget_articles->resizeColumnsToContents();
}

void YerithPointDeVenteWindow::actualiser_toutes_valeurs()
{
    int tableRowCount = tableWidget_articles->rowCount();

    _logger->log("actualiser_toutes_valeurs",
                 QString("tableRowCount: %1").arg(tableRowCount));

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;

    double SERVICE_COST = 0.0;

    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableRowCount; ++k)
    {
        YerithArticleVenteInfo *articleVenteInfo =
                        articleItemToVenteInfo.value(k);

        curTableWidgetItem =
                        tableWidget_articles->item(k,
                                                   YerithPointDeVenteTableWidget::
                                                   QTE_COLUMN);
        curTableWidgetItemQte = curTableWidgetItem->text().toDouble();

        quantiteVendue += curTableWidgetItemQte;

        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);

        SERVICE_COST += articleVenteInfo->Get_ajout_pour_COMMERCIAL();

        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteAVendre = quantiteVendue;

    _tva = tva;

    _sommeTotal = sommeTotal;

    double total = _sommeTotal - _tva - SERVICE_COST;

    if (_remise_somme_total_prix < sommeTotal)
    {
        _sommeTotal = _sommeTotal - _remise_somme_total_prix;
    }

    update_lineedits_and_labels(total);
}


void YerithPointDeVenteWindow::actualiser_toutes_valeurs(const QString &
                                                         a_client_nom_entreprise)
{
    _curClientName = a_client_nom_entreprise;

    actualiser_toutes_valeurs();
}


void YerithPointDeVenteWindow::update_lineedits_and_labels(double total)
{
    _sommeTotal_HORS_TAXES = total;

    updateCompteClient_PROGRAMME_DE_FIDELITE_LOYALTY(_curClientName);

    lineEdit_articles_quantite_a_vendre->setText(GET_NUM_STRING
                                                 (_quantiteAVendre));
    lineEdit_articles_SERVICE->setText(GET_NUM_STRING(_SERVICE));
    lineEdit_articles_tva->setText(GET_NUM_STRING(_tva));
    lineEdit_articles_total->setText(GET_CURRENCY_STRING_NUM
                                     (_sommeTotal_HORS_TAXES));
    lineEdit_articles_somme_total->setText(GET_CURRENCY_STRING_NUM
                                           (_sommeTotal));
    label_total_ttc->setText(GET_CURRENCY_STRING_NUM(_sommeTotal));

    handleMontantRecu();
}

void YerithPointDeVenteWindow::handleMontantRecu()
{
    _montantRecu =
                    _allWindows->
                    _pdVenteMethodePaiementComptantEntreeDialog->lineEdit_vente_montant_recu->
                    text().toDouble();

    _montantARembourser = _montantRecu - _sommeTotal;

    if (_montantARembourser > 0)
    {
        lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM
                                                        (_montantARembourser));
    }
    else
    {
        lineEdit_articles_montant_a_rembourser->setText(GET_CURRENCY_STRING_NUM
                                                        (0.0));
    }
}

void YerithPointDeVenteWindow::retirer_un_article()
{
    _logger->log("enlever_article");

    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow > -1)
    {
        tableWidget_articles->removeArticle(tableWidgetRow);
        {
            YerithArticleVenteInfo *article =
                            articleItemToVenteInfo.value(tableWidgetRow);
            articleItemToVenteInfo.remove(tableWidgetRow);
            delete article;
        }

        QMap < int, YerithArticleVenteInfo * >newArticleItemToVenteInfo;

        QMapIterator < int,
                     YerithArticleVenteInfo * >i(articleItemToVenteInfo);

        int k = 0;

        while (i.hasNext())
        {
            i.next();
            newArticleItemToVenteInfo.insert(k, i.value());
            _logger->debug("enlever_article",
                           QString("key: %1, value: %2").
                           arg(QString::number(k), i.value()->_stockName));
            ++k;
        }

        articleItemToVenteInfo.clear();

        articleItemToVenteInfo = newArticleItemToVenteInfo;

        actualiser_tableau_vente();

        handle_VIEW_FONT_SIZE_change_buttons();

        setRechercheLineEditFocus();
    }
}


void YerithPointDeVenteWindow::choisir_methode_paiment()
{
    _logger->log("choisir_methode_paiment");

    _allWindows->_pdVenteMethodePaiementDialog->rendreInvisible();

    double total_prix_vente = 0.0;

    if (tableWidget_articles->itemCount() <= 0)
    {
        YerithQMessageBox::warning(this, QObject::tr("vendre"),
                                   QObject::tr
                                   ("Vous n'avez pas choisi d'articles à vendre !"));
        return;
    }
    else
    {
        YerithArticleVenteInfo *articleVenteInfo = 0;
        for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
        {
            articleVenteInfo = articleItemToVenteInfo.value(j);
            if (0 != articleVenteInfo)
            {
                total_prix_vente += articleVenteInfo->prix_vente();
            }
        }
    }


    if (_sommeTotal > 0 && !lineEdit_articles_nom_client->text().isEmpty())
    {
        _TOTAL_PRIX_VENTE__POUR__COMPTECLIENT_AVANCE_DE_CASH = total_prix_vente;

        QString query_customer_cash_advanced_account =
                        QString
                        ("SELECT compte_client_AVANCE_DE_CASH from %1 WHERE %2='%3'").arg
                        (YerithDatabase::CLIENTS,
                         YerithDatabaseTableColumn::NOM_ENTREPRISE,
                         lineEdit_articles_nom_client->text());

        QSqlQuery a_query;

        double customer_account_CASH_ADVANCED = 0.0;

        int results =
                        YerithUtils::execQuery(a_query, query_customer_cash_advanced_account,
                                               _logger);

        if (results > 0 && a_query.next())
        {
            customer_account_CASH_ADVANCED = a_query.value(0).toDouble();

            if (customer_account_CASH_ADVANCED > 0 &&
                    customer_account_CASH_ADVANCED >=
                    _TOTAL_PRIX_VENTE__POUR__COMPTECLIENT_AVANCE_DE_CASH)
            {
                _allWindows->_pdVenteMethodePaiementDialog->
                setPushbuttonCompteClient__AVANCE_DE_CASH_Enabled(true);
            }
            else
            {
                _allWindows->_pdVenteMethodePaiementDialog->
                setPushbuttonCompteClient__AVANCE_DE_CASH_Enabled(false);
            }
        }
        else
        {
            _allWindows->_pdVenteMethodePaiementDialog->
            setPushbuttonCompteClient__AVANCE_DE_CASH_Enabled(false);
        }
    }
    else
    {
        _allWindows->_pdVenteMethodePaiementDialog->
        setPushbuttonCompteClient__AVANCE_DE_CASH_Enabled(false);
    }

    _allWindows->_pdVenteMethodePaiementDialog->
    showAsModalDialogWithParent(*this);
}


//TODO
unsigned int YerithPointDeVenteWindow::effectuer_check_out_carte_credit_carte_debit()
{

    return 0;
}


void YerithPointDeVenteWindow::executer_la_vente_comptant()
{
    int IDforReceipt = YerithERPWindows::getNextIdSqlTableModel_stocks_vendu();

    QString
    referenceRecuVendu(YerithUtils::GET_REFERENCE_RECU_VENDU
                       (QString::number(IDforReceipt)));

    int stocksVenduID = -1;

    double nouveau_compteClient_FIDELITE = 0.0;

    bool is_compte_client_FIDELITE_calculated = false;

    for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
    {
        YerithArticleVenteInfo *articleVenteInfo =
                        articleItemToVenteInfo.value(j);

        QSqlQuery stockRecordQuery;

        QString
        stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'").arg
                            (YerithDatabase::STOCKS,
                             YerithDatabaseTableColumn::ID,
                             articleVenteInfo->_stockID));

        bool historique_stock_actif = true;

        bool isService = false;

        double quantite_totale_actuelle = 0.0;

        QDate datePeremtion;

        QString historiqueStock;

        QString localisation_stock;

        QString localisation;

        QString nomEntrepriseFournisseur;

        int querySize =
                        YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr,
                                               _logger);

        if (querySize > 0 && stockRecordQuery.next())
        {
            isService =
                            stockRecordQuery.value(YerithDatabaseTableColumn::IS_SERVICE).
                            toBool();

            quantite_totale_actuelle =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();

            datePeremtion =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::DATE_PEREMPTION).toDate();

            historique_stock_actif =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                            toBool();

            historiqueStock =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::HISTORIQUE_STOCK).toString();

            localisation_stock =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::LOCALISATION_STOCK).toString();

            localisation =
                            stockRecordQuery.value(YerithDatabaseTableColumn::LOCALISATION).
                            toString();

            nomEntrepriseFournisseur =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).
                            toString();
        }

        YerithSqlTableModel &stocksVenduTableModel =
                        _allWindows->getSqlTableModel_stocks_vendu();

        QSqlRecord stocksVenduRecord = stocksVenduTableModel.record();

        _typeDeVente = YerithUtils::VENTE_COMPTANT;

        stocksVenduID =
                        YerithERPWindows::getNextIdSqlTableModel_stocks_vendu();

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::ID,
                                   stocksVenduID);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::TYPE_DE_VENTE,
                                   _typeDeVente);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::IS_SERVICE,
                                   isService);

        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::REFERENCE_RECU_VENDU,
                 referenceRecuVendu);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::memo_sale_for_sale,
                                   articleVenteInfo->_MEMO_SALE_for_SEARCH);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::REFERENCE,
                                   articleVenteInfo->_stockReference);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::DESIGNATION,
                                   articleVenteInfo->_stockName);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::DATE_PEREMPTION,
                                   datePeremtion);

        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                 articleVenteInfo->_stockNomDepartementProduits);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::CATEGORIE,
                                   articleVenteInfo->_stockCategorie);
        stocksVenduRecord.setValue(YerithDatabaseTableColumn::QUANTITE_VENDUE,
                                   articleVenteInfo->_quantite_a_vendre);
        stocksVenduRecord.setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                                   articleVenteInfo->_prix_unitaire);
        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                 articleVenteInfo->prix_vente());

        stocksVenduRecord.setValue
        (YerithDatabaseTableColumn::
         MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS,
         GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS());

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
                                   articleVenteInfo->Get_ajout_pour_COMMERCIAL());

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::REMISE_PRIX,
                                   articleVenteInfo->remise_prix());
        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::REMISE_POURCENTAGE,
                 articleVenteInfo->remise_pourcentage());
        stocksVenduRecord.setValue(YerithDatabaseTableColumn::MONTANT_TVA,
                                   articleVenteInfo->montant_tva());

        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::LOCALISATION_STOCK,
                 localisation_stock);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::LOCALISATION,
                                   localisation);
        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                 nomEntrepriseFournisseur);

        YerithPOSUser *user = _allWindows->getUser();

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::NOM_CAISSIER,
                                   user->nom_complet());
        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER,
                 user->nom_utilisateur());


        if (checkBox_DATE_EXTRA_VENTE->isChecked())
        {
            stocksVenduRecord.setValue(YerithDatabaseTableColumn::DATE_VENTE,
                                       dateEdit_PDV_date_vente_extra->date());
        }
        else
        {
            stocksVenduRecord.setValue(YerithDatabaseTableColumn::DATE_VENTE,
                                       GET_CURRENT_DATE);
        }


        stocksVenduRecord.setValue(YerithDatabaseTableColumn::HEURE_VENTE,
                                   CURRENT_TIME);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::STOCKS_ID,
                                   articleVenteInfo->_stockID);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::MONTANT_RECU,
                                   _montantRecu);

        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_A_REMBOURSER,
                 lineEdit_articles_montant_a_rembourser->text().
                 toDouble());

        if (articleVenteInfo->_effectuer_vente_en_gros)
        {
            stocksVenduRecord.
            setValue(YerithDatabaseTableColumn::IS_VENTE_EN_GROS, true);
        }
        else
        {
            stocksVenduRecord.
            setValue(YerithDatabaseTableColumn::IS_VENTE_EN_GROS, false);
        }

        double articleVenteInfo_prix_dachat =
                        YerithUtils::get_prix_dachat_wheter_exists(articleVenteInfo->
                                                                   get_stocksID());

        if (articleVenteInfo_prix_dachat > 0)
        {
            double marge_beneficiaire =
                            YerithUtils::getMargeBeneficiaire(articleVenteInfo->
                                                              prix_vente(),
                                                              (articleVenteInfo_prix_dachat *
                                                               articleVenteInfo->
                                                               _quantite_a_vendre),
                                                              articleVenteInfo->
                                                              montant_tva());

            if (marge_beneficiaire > 0)
            {
                stocksVenduRecord.
                setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE,
                         marge_beneficiaire);
            }
        }

        double nouvelle_quantite_totale =
                        quantite_totale_actuelle - articleVenteInfo->_quantite_a_vendre;

        QString historiqueStockVendu;

        if (historique_stock_actif)
        {
            historiqueStockVendu = YerithHistoriqueStock::creer_mouvement_stock
                                   (MOUVEMENT_DE_STOCK_VENTE,
                                    stocksVenduID,
                                    GET_CURRENT_DATE,
                                    quantite_totale_actuelle,
                                    articleVenteInfo->_quantite_a_vendre, nouvelle_quantite_totale);

            historiqueStockVendu.
            prepend(YerithHistoriqueStock::SEPARATION_EXTERNE);

            QString historiqueStockVendu_NEW = historiqueStock;

            historiqueStockVendu_NEW.append(historiqueStockVendu);

            if (YerithUtils::IS_SQL_TABLE_COLUMN_MAXIMUM_SIZE_REACHED
                    (historiqueStock.length(), historiqueStockVendu_NEW.length(),
                     6500))
            {
                historiqueStockVendu = YerithUtils::EMPTY_STRING;
            }
        }

        historiqueStock.append(historiqueStockVendu);

        stocksVenduRecord.
        setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                 historique_stock_actif);

        stocksVenduRecord.setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                   historiqueStock);

        //qDebug() << QString("++ test: %1")
        //                     .arg(historiqueStock);

        QString nom_entreprise_client = lineEdit_articles_nom_client->text();

        if (checkBox_enregistrer_client->isChecked())
        {
            YerithUtils::creerNouveauClient(nom_entreprise_client, this);
        }

        if (!nom_entreprise_client.isEmpty())
        {
            YerithSqlTableModel &clientsTableModel =
                            _allWindows->getSqlTableModel_clients();

            QString
            clientFilter(QString("%1 = '%2'").arg
                         (YerithDatabaseTableColumn::NOM_ENTREPRISE,
                          nom_entreprise_client));

            clientsTableModel.yerithSetFilter_WITH_where_clause(clientFilter);

            int clientsTableModelRowCount =
            		clientsTableModel.easySelect("src/windows/pos/yerith-erp-pointdevente-windows.cpp", 3645);

            //QDEBUG_STRING_OUTPUT_2_N("clientsTableModelRowCount", clientsTableModelRowCount);

            QSqlRecord clientsRecord = clientsTableModel.record(0);

            QString
            clients_id(GET_SQL_RECORD_DATA
                       (clientsRecord, YerithDatabaseTableColumn::ID));

            //QDEBUG_STRING_OUTPUT_2("clients_id", clients_id);

            stocksVenduRecord.setValue(YerithDatabaseTableColumn::CLIENTS_ID,
                                       clients_id);

            stocksVenduRecord.
            setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                     lineEdit_articles_nom_client->text());

            QString quartier_entreprise_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::QUARTIER);

            QString ville_entreprise_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::VILLE);

            QString region_etat_entreprise_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::PROVINCE_ETAT);


            stocksVenduRecord.
            setValue(YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
                     quartier_entreprise_client);

            stocksVenduRecord.
            setValue(YerithDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT,
                     ville_entreprise_client);

            stocksVenduRecord.
            setValue(YerithDatabaseTableColumn::
                     REGION_ETAT_ENTREPRISE_CLIENT,
                     region_etat_entreprise_client);

            if (!is_compte_client_FIDELITE_calculated)
            {
                double compteClient_FIDELITE =
                		GET_SQL_RECORD_DATA(clientsRecord,
                                            YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS)
										.toDouble();

                nouveau_compteClient_FIDELITE =
                                compteClient_FIDELITE +
                                (-1 * GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS());

                _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT =
                                nouveau_compteClient_FIDELITE;

                stocksVenduRecord
					.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
							  _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT);

                clientsRecord
					.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
							  nouveau_compteClient_FIDELITE);

                is_compte_client_FIDELITE_calculated = true;
            }
            else
            {
                stocksVenduRecord
					.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                			  _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT);

                clientsRecord
					.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
							  nouveau_compteClient_FIDELITE);
            }

            clientsTableModel.updateRecord(0,
            							   clientsRecord,
										   "src/windows/pos/yerith-erp-pointdevente-window.cpp",
										   3726,
										   nom_entreprise_client);


            clientsTableModel.resetFilter();
        }
        else
        {
            stocksVenduRecord.setValue(YerithDatabaseTableColumn::CLIENTS_ID,
                                       -1);

            stocksVenduRecord.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                     	 	 	 	   YerithUtils::STRING_FRENCH_DIVERS);
        }

        bool success1 = stocksVenduTableModel
        					.insertNewRecord(stocksVenduRecord,
        									 this,
											 "src/windows/pos/yerith-erp-pointdevente-window.cpp",
											 3744,
											 articleVenteInfo->_stockName);

        if (success1)
        {
            if (nouvelle_quantite_totale < 0)
            {
                nouvelle_quantite_totale = 0;
            }

            if (0 == nouvelle_quantite_totale)
            {
                QString
                removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'").arg
                               (YerithDatabase::STOCKS,
                                YerithDatabaseTableColumn::ID,
                                articleVenteInfo->_stockID));
                YerithUtils::execQuery(removeRowQuery);
            }

            stockRecordQueryStr.clear();
            stockRecordQueryStr.append(QString
                                       ("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'").
                                       arg(YerithDatabase::STOCKS,
                                           YerithDatabaseTableColumn::QUANTITE_TOTALE,
                                           QString::number
                                           (nouvelle_quantite_totale),
                                           YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                           historiqueStock,
                                           YerithDatabaseTableColumn::ID,
                                           articleVenteInfo->_stockID));

            //qDebug() << QString("++ quantiteQueryStr: %1")
            //                      .arg(quantiteQueryStr);

            bool success2 =
                            YerithUtils::execQuery(stockRecordQueryStr, _logger);

            QString
            sMsg(QObject::tr("La quantité en stock de l'article '"));

            sMsg.append(articleVenteInfo->_stockName).append("'").
            append(QObject::tr(" (%1 pièce(s))").
                   arg(articleVenteInfo->_quantite_a_vendre));

            if (success2)
            {
                sMsg.append(QObject::tr
                            (" a été actualisée avec succès."));
            }
            else
            {
                sMsg.append(QObject::tr(" n'a pas pu être actualisée!\n"
                                            "Contacter 'YERITH'"));
            }

            _logger->log("vendre", sMsg);
        }
        else
        {
            //TODO MESSAGE D'ERREUR DANS LE FICHIER DE LOGS
        }
    }

    handle_CLIENT_LOYALTY_PROGRAM_PAYMENTS_ENTRIES(referenceRecuVendu);

    emit SELLING();

    QString vMsg(QObject::tr("La vente de '"));

    vMsg.append(QString::number(_quantiteAVendre)).append(QObject::tr
                                                          ("' articles a été éffectuée avec succès."));

    if (QMessageBox::Ok ==
            YerithQMessageBox::information(this,
                                           QObject::tr("succès d'une vente"),
                                           vMsg))
    {
        imprimer_recu_vendu(referenceRecuVendu);
    }

    QThread::sleep(0.0001);

    setupLineEditsQCompleters();
}


unsigned int YerithPointDeVenteWindow::effectuer_check_out_comptant()
{
    _logger->log("vendre");

    if (_montantRecu < 0 || _montantRecu < _sommeTotal)
    {
        if (QMessageBox::Ok ==
                YerithQMessageBox::warning(this,
                                           QObject::tr
                                           ("montant reçu du client"),
                                           QObject::tr
                                           ("Le montant reçu du client doit être supérieure"
                                            " à la somme total !")))
        {
            return -1;
        }
        else
        {
        }
    }

    _allWindows->_pdVenteMethodePaiementComptantEntreeDialog->
    rendreInvisible();

    handleMontantRecu();

    QString msgVente(QObject::tr("Poursuivre avec la vente de "));

    msgVente.append(QString::number(_quantiteAVendre, 'f', 0));
    msgVente.append(QObject::tr(" articles (comptant) ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this, QObject::tr("vendre"),
                                        msgVente,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        executer_la_vente_comptant();

        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        cleanUpAfterVente();
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("annulation d'une vente"),
                                       QObject::tr
                                       ("Vous avez annulé la vente !"),
                                       QMessageBox::Ok);
    }

    tableWidget_articles->resizeColumnsToContents();

    return 0;
}


void YerithPointDeVenteWindow::updateCompteClient(double nouveau_compte_client)
{
    QString queryStr;

    queryStr.append(QString("UPDATE %1 SET %2 = '%3' WHERE %4 = '%5'").arg
                    (YerithDatabase::CLIENTS,
                     YerithDatabaseTableColumn::COMPTE_CLIENT,
                     QString::number(nouveau_compte_client),
                     YerithDatabaseTableColumn::NOM_ENTREPRISE,
                     lineEdit_articles_nom_client->text()));

    bool success = YerithUtils::execQuery(queryStr, _logger);
}


double YerithPointDeVenteWindow::GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS()
{
    QString client_best_loyalty_program;

    return GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS(client_best_loyalty_program);
}


double YerithPointDeVenteWindow::
        GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS(QString &client_best_loyalty_program_IN_OUT)
{
    Yerith_MAP_COMPLEX_Item *a_map_COMPLEX_item = 0;

    if (!_client_group_program_TO_money_benefit.isEmpty())
    {
        a_map_COMPLEX_item =
            _client_group_program_TO_money_benefit.q_list().last();
    }

    if (0 == a_map_COMPLEX_item)
    {
        return 0.0;
    }

    client_best_loyalty_program_IN_OUT = a_map_COMPLEX_item->_itemName;

    HANDLE_VISIBILITY_OF_LOYALTY_LABEL_AND_LINEEDIT
        (QString("%1   (%2)")
            .arg(a_map_COMPLEX_item->_itemName,
         GET_CURRENCY_STRING_NUM(a_map_COMPLEX_item->_itemValue)));

    return a_map_COMPLEX_item->_itemValue;
}


double YerithPointDeVenteWindow::calculate_LOYALTY_PROGRAM_MONEY_BENEFITS(const
                                                                          QString &
                                                                          a_loyalty_program)
{
    double money_BENEFITS = 0.0;

    QString
    SELECT_PROGRAMME_DE_FIDELITE_CLIENTS(QString
                                         ("select * from %1 where %2='%3'").
                                         arg(YerithDatabase::
                                             PROGRAMMES_DE_FIDELITE_CLIENTS,
                                             YerithDatabaseTableColumn::
                                             DESIGNATION,
                                             a_loyalty_program));

//      QDEBUG_STRING_OUTPUT_2("SELECT_PROGRAMME_DE_FIDELITE_CLIENTS", SELECT_PROGRAMME_DE_FIDELITE_CLIENTS);

    QSqlQuery a_qsql_query;

    int query_size = YerithUtils::execQuery(a_qsql_query,
                                            SELECT_PROGRAMME_DE_FIDELITE_CLIENTS);

    if (query_size <= 0)
    {
        return 0.0;
    }

    a_qsql_query.next();

    QSqlRecord aQSqlRecord = a_qsql_query.record();

    double AMOUNT_TO_BE_PAID_BY_CLIENT__NO__MONEY_BENEFITS =
                    GET_CURRENT_CLIENT_AMOUNT_TO_BE_PAID();

//    QDEBUG_STRING_OUTPUT_2_N("original amount to be paid by client", _sommeTotal);

    double pourcentage_rabais = GET_SQL_RECORD_DATA(aQSqlRecord,
                                                    YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS).
                                toDouble();

    money_BENEFITS =
                    (pourcentage_rabais *
                     AMOUNT_TO_BE_PAID_BY_CLIENT__NO__MONEY_BENEFITS) / 100.0;

//      QDEBUG_STRING_OUTPUT_1(QString("MONEY BENEFITS FOR client loyalty program '%1': %2 (ORIGINAL AMOUNT: %3)")
//                                                              .arg(a_loyalty_program,
//                                                                       QString::number(money_BENEFITS),
//                                                                       QString::number(AMOUNT_TO_BE_PAID_BY_CLIENT__NO__MONEY_BENEFITS)));

    return money_BENEFITS;
}


void YerithPointDeVenteWindow::
        updateCompteClient_PROGRAMME_DE_FIDELITE_LOYALTY(const QString  &a_nom_entreprise_client,
                                                         bool           CALL_update_lineedits_and_labels /* = false */)
{
    if (a_nom_entreprise_client.isEmpty())
    {
        HANDLE_VISIBILITY_OF_LOYALTY_LABEL_AND_LINEEDIT();

        return;
    }

    /*
     * I search all client group of customer 'a_nom_entreprise_client'.
     */
    QString SELECT_CLIENT_GROUPS
                (QString("select %1 from %2 where %3='%4'")
                    .arg(YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
                         YerithDatabase::CLIENTS,
                         YerithDatabaseTableColumn::NOM_ENTREPRISE,
                         a_nom_entreprise_client));


    QSqlQuery a_qsql_query;

    int query_size = YerithUtils::execQuery(a_qsql_query,
                                            SELECT_CLIENT_GROUPS);

    if (query_size <= 0)
    {
        _client_group_program_TO_money_benefit.clear();

        HANDLE_VISIBILITY_OF_LOYALTY_LABEL_AND_LINEEDIT();

        return;
    }


    QString client_group_id;

    a_qsql_query.next();

    client_group_id = a_qsql_query
                        .value(YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID).toString();

//      QDEBUG_STRING_OUTPUT_2("client_group_id", client_group_id);

    if (client_group_id.isEmpty())
    {
        _client_group_program_TO_money_benefit.clear();

        HANDLE_VISIBILITY_OF_LOYALTY_LABEL_AND_LINEEDIT();

        return;
    }


    QStringList clientGroupId_list;

    YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING(clientGroupId_list,
                                                client_group_id);


//      QDEBUG_QSTRINGLIST_OUTPUT("clientGroupId_list", clientGroupId_list);


    /*
     * select the best client fidelity program for this customer (i.e.:
     * the 'client fidelity program' where this customer earns maximal
     * money !
     */
    _client_group_program_TO_money_benefit.clear();


//      QDEBUG_STRING_OUTPUT_2("YerithERPConfig::THIS_SITE_LOCALISATION_NAME",
//                                                 YerithERPConfig::THIS_SITE_LOCALISATION_NAME);


    QString clientGroup_db_ID;

    QString client_group_loyalty_program_LOCALISATION;

    QString client_group_loyalty_program;

    QString CUR_SELECT_CLIENT_LOYALTY_PROGRAM;

    double cur_client_group_loyalty_program_money_BENEFITS = 0.0;


    for (uint k = 0; k < clientGroupId_list.size(); ++k)
    {
        clientGroup_db_ID = clientGroupId_list.at(k);

        if (clientGroup_db_ID.isEmpty())
        {
            continue;
        }

        CUR_SELECT_CLIENT_LOYALTY_PROGRAM =
                        QString("select %1 from %2 where %3='%4'")
                            .arg(YerithDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
                                 YerithDatabase::GROUPES_DE_CLIENTS,
                                 YerithDatabaseTableColumn::ID,
                                 clientGroup_db_ID);

//              QDEBUG_STRING_OUTPUT_2("cur_select_client_loyalty_program",
//                                                         cur_select_client_loyalty_program);

        a_qsql_query.clear();

        query_size = YerithUtils::execQuery(a_qsql_query,
                                            CUR_SELECT_CLIENT_LOYALTY_PROGRAM);


        if (query_size > 0   &&
            a_qsql_query.next())
        {
            client_group_loyalty_program = a_qsql_query.value(0).toString();

            if (client_group_loyalty_program.isEmpty())
            {
                continue;
            }

            client_group_loyalty_program_LOCALISATION =
                            YerithUtils::GET_LOCALISATION_FOR_CLIENT_LOYALTY_PROGRAM
                                (client_group_loyalty_program);

//                      QDEBUG_STRING_OUTPUT_2("client_group_loyalty_program_LOCALISATION",
//                                                                 client_group_loyalty_program_LOCALISATION);

            if (!client_group_loyalty_program_LOCALISATION.isEmpty() &&
                !YerithUtils::isEqualCaseInsensitive(YerithERPConfig::THIS_SITE_LOCALISATION_NAME,
                                                     client_group_loyalty_program_LOCALISATION))
            {
                continue;
            }

//                      QDEBUG_STRING_OUTPUT_1(QString("clientGroup ID (%1), client_group_loyalty_program => %2")
//                                                                      .arg(clientGroup_db_ID,
//                                                                               client_group_loyalty_program));

            cur_client_group_loyalty_program_money_BENEFITS =
                calculate_LOYALTY_PROGRAM_MONEY_BENEFITS(client_group_loyalty_program);
                calculate_LOYALTY_PROGRAM_MONEY_BENEFITS(client_group_loyalty_program);


            _client_group_program_TO_money_benefit
                .insert_item(client_group_loyalty_program,
                             cur_client_group_loyalty_program_money_BENEFITS);
        }
    }


    _client_group_program_TO_money_benefit.q_sort();

//      _client_group_program_TO_money_benefit.print("_client_group_program_TO_money_benefit");

    double best_current_lotaly_program_money_benefits =
            GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS();

    double somme_TOTAL_SANS_TVA = GET_CURRENT_CLIENT_AMOUNT_TO_BE_PAID();

//      QDEBUG_STRING_OUTPUT_2_N("somme_TOTAL_SANS_TVA **", somme_TOTAL_SANS_TVA);

    if (somme_TOTAL_SANS_TVA >= best_current_lotaly_program_money_benefits)
    {
        somme_TOTAL_SANS_TVA =
            somme_TOTAL_SANS_TVA - best_current_lotaly_program_money_benefits;
    }

//      QDEBUG_STRING_OUTPUT_2_N(QString("somme_TOTAL_SANS_TVA - best_current_lotaly_program_money_benefits (%1)")
//                                                              .arg(best_current_lotaly_program_money_benefits),
//                                                       somme_TOTAL_SANS_TVA);

//      QDEBUG_STRING_OUTPUT_2_N("_sommeTotal *", _sommeTotal);

    double _TVA_SOMME_TOTAL_MONEY_BENEFITS = 0.0;

    if (checkBox_tva->isChecked())
    {
        _TVA_SOMME_TOTAL_MONEY_BENEFITS = somme_TOTAL_SANS_TVA * YerithERPConfig::tva_value;
    }

//      QDEBUG_STRING_OUTPUT_2_N("_TVA_SOMME_TOTAL_MONEY_BENEFITS",
//                                                       _TVA_SOMME_TOTAL_MONEY_BENEFITS);

    _sommeTotal_HORS_TAXES = somme_TOTAL_SANS_TVA;

    _tva = _TVA_SOMME_TOTAL_MONEY_BENEFITS;

    _sommeTotal = somme_TOTAL_SANS_TVA + _TVA_SOMME_TOTAL_MONEY_BENEFITS;

//      QDEBUG_STRING_OUTPUT_2_N("_sommeTotal ***", _sommeTotal);
}


void YerithPointDeVenteWindow::
        handle_CLIENT_LOYALTY_PROGRAM_PAYMENTS_ENTRIES(const QString &a_product_reference)
{
    if (_curClientName.isEmpty()                      ||
        _client_group_program_TO_money_benefit.isEmpty())
    {
        return;
    }

    /*
     * WE FIRST CREATE A PAYMENT ENTRY.
     */
    QString client_loyalty_best_program;


    YerithERPPaymentProcessingInformation a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT;

    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._nom_entreprise =
                    _curClientName;

    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._designation =
                    a_product_reference;

    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._reference =
                    a_product_reference;

    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._montant_paye =
                    GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS
                    (client_loyalty_best_program);

    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._type_de_paiement =
                    YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT;

    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT.
    _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS =
                    _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT;


    {
        QString compte_client_inchange =
                    QString("select %1 from %2 where %3='%4'")
                        .arg(YerithDatabaseTableColumn::COMPTE_CLIENT,
                             YerithDatabase::CLIENTS,
                             YerithDatabaseTableColumn::NOM_ENTREPRISE,
                             a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._nom_entreprise);

        QSqlQuery a_qsql_query;

        int query_size = YerithUtils::execQuery(a_qsql_query,
                                                compte_client_inchange);


        if (query_size > 0 && a_qsql_query.next())
        {
            a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._nouveau_compte_client =
                a_qsql_query.value(YerithDatabaseTableColumn::COMPTE_CLIENT).toDouble();
        }
    }


    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._NOTES =
        QObject::tr("PAYEMENT POUR PROGRAMME DE FIDÉLITÉ CLIENTS: '%1' (montant: %2)")
            .arg(client_loyalty_best_program,
                 QString::number(a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT._montant_paye));


    a_payment_info_record_FOR_CLIENT_LOYALTY_PAYMENT.save_payment_info_record();
}


void YerithPointDeVenteWindow::executer_la_vente_compteclient_FIDELITE()
{
    int IDforReceipt = YerithERPWindows::getNextIdSqlTableModel_stocks_vendu();


    QString referenceRecuVenduCompteClient =
        YerithUtils::GET_REFERENCE_RECU_VENDU(QString::number(IDforReceipt));


    int stocksVenduID = -1;

    double nouveau_compteClient_FIDELITE = 0.0;

    bool is_compte_client_FIDELITE_calculated = false;

    double total_prix_vente = 0.0;


    for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
    {
        YerithArticleVenteInfo *articleVenteInfo = articleItemToVenteInfo.value(j);

        QSqlQuery stockRecordQuery;

        QString stockRecordQueryStr =
            QString("SELECT * FROM %1 WHERE %2 = '%3'")
                .arg(YerithDatabase::STOCKS,
                     YerithDatabaseTableColumn::ID,
                     articleVenteInfo->_stockID);


        bool historique_stock_actif = true;

        bool isService = false;

        double quantite_totale_actuelle = 0.0;

        QDate datePeremtion;

        QString historiqueStock;

        QString localisation_stock;

        QString localisation;

        QString nomEntrepriseFournisseur;


        int querySize = YerithUtils::execQuery(stockRecordQuery,
                                               stockRecordQueryStr,
                                               _logger);


        if (querySize > 0 && stockRecordQuery.next())
        {
            isService = stockRecordQuery.value(YerithDatabaseTableColumn::IS_SERVICE).toBool();

            quantite_totale_actuelle =
                stockRecordQuery.value(YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();


            datePeremtion =
                stockRecordQuery.value(YerithDatabaseTableColumn::DATE_PEREMPTION).toDate();


            historique_stock_actif =
                stockRecordQuery.value(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF)
                                        .toBool();


            historiqueStock = stockRecordQuery.value(YerithDatabaseTableColumn::HISTORIQUE_STOCK).toString();

            localisation_stock = stockRecordQuery.value(YerithDatabaseTableColumn::LOCALISATION_STOCK).toString();

            localisation = stockRecordQuery.value(YerithDatabaseTableColumn::LOCALISATION).toString();


            nomEntrepriseFournisseur =
                stockRecordQuery.value(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).toString();
        }

        //qDebug() << QString("++ previousHistoriquestock: %1")
        //                      .arg(historiqueStock);

        YerithSqlTableModel &stocksVenduTableModel =
            _allWindows->getSqlTableModel_stocks_vendu();


        QSqlRecord stocksVenduCompteClientRecord = stocksVenduTableModel.record();


        _typeDeVente = YerithUtils::VENTE_COMPTE_CLIENT;


        stocksVenduID = YerithERPWindows::getNextIdSqlTableModel_stocks_vendu();

        stocksVenduCompteClientRecord.setValue(YerithDatabaseTableColumn::ID,
                                               stocksVenduID);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::TYPE_DE_VENTE,
                      _typeDeVente);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::IS_SERVICE,
                      isService);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::REFERENCE_RECU_VENDU,
                      referenceRecuVenduCompteClient);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::REFERENCE,
                      articleVenteInfo->_stockReference);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::DESIGNATION,
                      articleVenteInfo->_stockName);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::DATE_PEREMPTION,
                      datePeremtion);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                      articleVenteInfo->_stockNomDepartementProduits);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::CATEGORIE,
                      articleVenteInfo->_stockCategorie);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::QUANTITE_VENDUE,
                      articleVenteInfo->_quantite_a_vendre);

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                      articleVenteInfo->_prix_unitaire);


        total_prix_vente += articleVenteInfo->prix_vente();


        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                      articleVenteInfo->prix_vente());


        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS,
                      GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS());

        stocksVenduCompteClientRecord
            .setValue(YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
        			  articleVenteInfo->Get_ajout_pour_COMMERCIAL());

        stocksVenduCompteClientRecord.
        	setValue(YerithDatabaseTableColumn::REMISE_PRIX,
        			 articleVenteInfo->remise_prix());

        stocksVenduCompteClientRecord.
        	setValue(YerithDatabaseTableColumn::REMISE_POURCENTAGE,
        			 articleVenteInfo->remise_pourcentage());

        stocksVenduCompteClientRecord.
        	setValue(YerithDatabaseTableColumn::MONTANT_TVA,
        			 articleVenteInfo->montant_tva());

        stocksVenduCompteClientRecord.
        	setValue(YerithDatabaseTableColumn::LOCALISATION_STOCK,
        			 localisation_stock);

        stocksVenduCompteClientRecord.
        	setValue(YerithDatabaseTableColumn::LOCALISATION, localisation);

        stocksVenduCompteClientRecord.
        	setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
        			 nomEntrepriseFournisseur);

        YerithPOSUser *user = _allWindows->getUser();

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::NOM_CAISSIER,
                 user->nom_complet());

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER,
                 user->nom_utilisateur());


        if (checkBox_DATE_EXTRA_VENTE->isChecked())
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::DATE_VENTE,
                     dateEdit_PDV_date_vente_extra->date());
        }
        else
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::DATE_VENTE,
                     GET_CURRENT_DATE);
        }


        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::HEURE_VENTE, CURRENT_TIME);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::STOCKS_ID,
                 articleVenteInfo->_stockID);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_RECU, _montantRecu);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_A_REMBOURSER,
                 lineEdit_articles_montant_a_rembourser->text().
                 toDouble());

        if (articleVenteInfo->_effectuer_vente_en_gros)
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::IS_VENTE_EN_GROS, true);
        }
        else
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::IS_VENTE_EN_GROS, false);
        }

        double articleVenteInfo_prix_dachat =
                        YerithUtils::get_prix_dachat_wheter_exists(articleVenteInfo->
                                                                   get_stocksID());

        if (articleVenteInfo_prix_dachat > 0)
        {
            double marge_beneficiaire =
                            YerithUtils::getMargeBeneficiaire(articleVenteInfo->
                                                              prix_vente(),
                                                              articleVenteInfo_prix_dachat,
                                                              articleVenteInfo->
                                                              montant_tva());

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE,
                     (marge_beneficiaire *
                      articleVenteInfo->_quantite_a_vendre));
        }

        double nouvelle_quantite_totale =
                        quantite_totale_actuelle - articleVenteInfo->_quantite_a_vendre;


        QString historiqueStockVendu;

        if (historique_stock_actif)
        {
            historiqueStockVendu = YerithHistoriqueStock::creer_mouvement_stock
                                   (MOUVEMENT_DE_STOCK_VENTE,
                                    stocksVenduID,
                                    GET_CURRENT_DATE,
                                    quantite_totale_actuelle,
                                    articleVenteInfo->_quantite_a_vendre, nouvelle_quantite_totale);
        }

        historiqueStock.append(QString("%1%2").arg
                               (YerithHistoriqueStock::SEPARATION_EXTERNE,
                                historiqueStockVendu));

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                 historique_stock_actif);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                 historiqueStock);

        //qDebug() << QString("++ test: %1")
        //                     .arg(historiqueStock);

        YerithSqlTableModel &clientsTableModel =
                        _allWindows->getSqlTableModel_clients();

        QString
        clientFilter(QString("%1 = '%2'").arg
                     (YerithDatabaseTableColumn::NOM_ENTREPRISE,
                      lineEdit_articles_nom_client->text()));
        clientsTableModel.yerithSetFilter_WITH_where_clause(clientFilter);

        int clientsTableModelRowCount =
        		clientsTableModel.easySelect("src/windows/pos/yerith-erp-pointdevente-windows.cpp", 4424);

        if (clientsTableModelRowCount > 0)
        {
            QSqlRecord clientsRecord = clientsTableModel.record(0);

            QString
            clients_id(GET_SQL_RECORD_DATA
                       (clientsRecord, YerithDatabaseTableColumn::ID));

            QString quartier_entreprise_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::QUARTIER);

            QString ville_entreprise_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::VILLE);

            QString region_etat_entreprise_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::PROVINCE_ETAT);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
                     quartier_entreprise_client);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT,
                     ville_entreprise_client);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::
                     REGION_ETAT_ENTREPRISE_CLIENT,
                     region_etat_entreprise_client);

            if (!is_compte_client_FIDELITE_calculated)
            {
                double compteClient_FIDELITE =
                                GET_SQL_RECORD_DATA(clientsRecord,
                                                    YerithDatabaseTableColumn::
                                                    COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS).
                                toDouble();

                nouveau_compteClient_FIDELITE =
                                compteClient_FIDELITE +
                                (-1 * GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS());

                _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT =
                                nouveau_compteClient_FIDELITE;

                stocksVenduCompteClientRecord.setValue
                (YerithDatabaseTableColumn::
                 COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                 _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT);

                clientsRecord.setValue
                (YerithDatabaseTableColumn::
                 COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                 nouveau_compteClient_FIDELITE);

                is_compte_client_FIDELITE_calculated = true;
            }
            else
            {
                stocksVenduCompteClientRecord.setValue
                (YerithDatabaseTableColumn::
                 COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                 _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT);

                clientsRecord.setValue
                (YerithDatabaseTableColumn::
                 COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                 nouveau_compteClient_FIDELITE);
            }

            clientsTableModel.updateRecord(0,
            							   clientsRecord,
										   "src/windows/pos/yerith-erp-pointdevente-window.cpp",
										   4506);

            double compteClient = GET_SQL_RECORD_DATA(clientsRecord,
                                                      YerithDatabaseTableColumn::COMPTE_CLIENT).
                                  toDouble();

            double nouveau_compte_client = compteClient - total_prix_vente;

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::COMPTE_CLIENT,
                     nouveau_compte_client);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::CLIENTS_ID, clients_id);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                     lineEdit_articles_nom_client->text());

            clientsTableModel.resetFilter();

            updateCompteClient(nouveau_compte_client);
        }
        else
        {
            stocksVenduCompteClientRecord.setValue(YerithDatabaseTableColumn::CLIENTS_ID, -1);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                     YerithUtils::STRING_FRENCH_DIVERS);
        }

        bool success1 = stocksVenduTableModel
        					.insertNewRecord(stocksVenduCompteClientRecord,
                                             this,
											 "src/windows/pos/yerith-erp-pointdevente-window.cpp",
											 4684,
											 articleVenteInfo->_stockName);

        if (success1)
        {
            if (nouvelle_quantite_totale < 0)
            {
                nouvelle_quantite_totale = 0;
            }

            if (0 == nouvelle_quantite_totale)
            {
                QString
                removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'").arg
                               (YerithDatabase::STOCKS,
                                YerithDatabaseTableColumn::ID,
                                articleVenteInfo->_stockID));
                YerithUtils::execQuery(removeRowQuery);
            }

            stockRecordQueryStr.clear();
            stockRecordQueryStr.append(QString
                                       ("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'").
                                       arg(YerithDatabase::STOCKS,
                                           YerithDatabaseTableColumn::QUANTITE_TOTALE,
                                           QString::number
                                           (nouvelle_quantite_totale),
                                           YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                           historiqueStock,
                                           YerithDatabaseTableColumn::ID,
                                           articleVenteInfo->_stockID));

            //qDebug() << QString("++ quantiteQueryStr: %1")
            //                      .arg(quantiteQueryStr);

            bool success2 =
                            YerithUtils::execQuery(stockRecordQueryStr, _logger);

            QString
            sMsg(QObject::tr("La quantité en stock de l'article '"));

            sMsg.append(articleVenteInfo->_stockName).append("'").
            append(QObject::tr(" (%1 pièce(s))").
                   arg(articleVenteInfo->_quantite_a_vendre));

            if (success2)
            {
                sMsg.append(QObject::tr
                            (" a été actualisée avec succès."));
            }
            else
            {
                sMsg.append(QObject::tr
                            (" n'a pas pu être actualisée !\n"
                             "Contacter 'YERITH'"));
            }

            _logger->log("vendre", sMsg);
        }
        else
        {
            //TODO MESSAGE D'ERREUR DANS LE FICHIER DE LOGS
        }
    }

    handle_CLIENT_LOYALTY_PROGRAM_PAYMENTS_ENTRIES
    (referenceRecuVenduCompteClient);

    emit SELLING();

    QString vMsg(QObject::tr
                 ("La vente de '%1' articles a été éffectuée avec succès !").
                 arg(QString::number(_quantiteAVendre)));

    if (QMessageBox::Ok ==
            YerithQMessageBox::information(this,
                                           QObject::tr("succès d'une vente"),
                                           vMsg))
    {
        imprimer_recu_vendu(referenceRecuVenduCompteClient);
    }
}


unsigned int YerithPointDeVenteWindow::effectuer_check_out_compte_client()
{
    QString msgVente(QObject::tr("Poursuivre avec la vente de "));

    msgVente.append(QString::number(_quantiteAVendre, 'f', 0));
    msgVente.
    append(QObject::tr(" articles (compte client FIDÉLITÉ) ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this, QObject::tr("vendre"),
                                        msgVente,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        executer_la_vente_compteclient_FIDELITE();

        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        cleanUpAfterVente();
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("annulation d'une vente"),
                                       QObject::tr
                                       ("Vous avez annulé la vente !"),
                                       QMessageBox::Ok);
    }

    tableWidget_articles->resizeColumnsToContents();

    return 0;
}


unsigned int YerithPointDeVenteWindow::effectuer_checkout_compteclient_AVANCE_DE_CASH()
{
    QString msgVente(QObject::tr("Poursuivre avec la vente de "));

    msgVente.append(QString::number(_quantiteAVendre, 'f', 0));
    msgVente.
    append(QObject::tr(" articles (compte client [AVANCE DE CASH]) ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this, QObject::tr("vendre"),
                                        msgVente,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        executer_la_vente_compteclient_AVANCE_DE_CASH
        (_TOTAL_PRIX_VENTE__POUR__COMPTECLIENT_AVANCE_DE_CASH);

        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        cleanUpAfterVente();
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("annulation d'une vente"),
                                       QObject::tr
                                       ("Vous avez annulé la vente !"),
                                       QMessageBox::Ok);
    }

    tableWidget_articles->resizeColumnsToContents();

    return 0;
}


void
YerithPointDeVenteWindow::executer_la_vente_compteclient_AVANCE_DE_CASH
(double TOTAL_PRIX_DE_VENTE)
{
    QString
    proposedNonEntrepriseClient(lineEdit_articles_nom_client->text());

    if (checkBox_enregistrer_client->isChecked())
    {
        if (!YerithUtils::creerNouveauClient(proposedNonEntrepriseClient))
        {
            return;
        }
    }

    int
    IDforReceipt = YerithERPWindows::getNextIdSqlTableModel_stocks_vendu();

    QString
    referenceRecuVenduCompteClient_AVANCE_DE_CASH
    (YerithUtils::GET_REFERENCE_RECU_VENDU(QString::number(IDforReceipt)));

    int
    stocksVenduID = -1;

    double
    nouveau_compteClient_AVANCE_DE_CASH = 0.0;

    for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
    {
        YerithArticleVenteInfo *
        articleVenteInfo = articleItemToVenteInfo.value(j);

        QSqlQuery
        stockRecordQuery;

        QString
        stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'").arg
                            (YerithDatabase::STOCKS,
                             YerithDatabaseTableColumn::ID,
                             articleVenteInfo->_stockID));

        bool
        isService = false;

        double
        quantite_totale_actuelle = 0.0;

        QDate
        datePeremtion;

        bool
        historique_stock_actif = true;

        QString
        historiqueStock;

        QString
        localisation_stock;

        QString
        localisation;

        QString
        nomEntrepriseFournisseur;

        int
        querySize =
                        YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr,
                                               _logger);

        if (querySize > 0 && stockRecordQuery.next())
        {
            isService =
                            stockRecordQuery.value(YerithDatabaseTableColumn::IS_SERVICE).
                            toBool();

            quantite_totale_actuelle =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();

            datePeremtion =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::DATE_PEREMPTION).toDate();

            historique_stock_actif =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                            toBool();

            historiqueStock =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::HISTORIQUE_STOCK).toString();

            localisation_stock =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::LOCALISATION_STOCK).toString();

            localisation =
                            stockRecordQuery.value(YerithDatabaseTableColumn::LOCALISATION).
                            toString();

            nomEntrepriseFournisseur =
                            stockRecordQuery.
                            value(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).
                            toString();
        }

        //qDebug() << QString("++ previousHistoriquestock: %1")
        //                      .arg(historiqueStock);

        YerithSqlTableModel &stocksVenduTableModel =
                        _allWindows->getSqlTableModel_stocks_vendu();

        QSqlRecord
        stocksVenduCompteClientRecord = stocksVenduTableModel.record();

        _typeDeVente = YerithUtils::VENTE_COMPTECLIENT_AVANCE_DE_CASH;

        stocksVenduID =
                        YerithERPWindows::getNextIdSqlTableModel_stocks_vendu();

        stocksVenduCompteClientRecord.setValue(YerithDatabaseTableColumn::ID,
                                               stocksVenduID);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::TYPE_DE_VENTE, _typeDeVente);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::IS_SERVICE, isService);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::REFERENCE_RECU_VENDU,
                 referenceRecuVenduCompteClient_AVANCE_DE_CASH);
        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::REFERENCE,
                 articleVenteInfo->_stockReference);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::DESIGNATION,
                 articleVenteInfo->_stockName);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::DATE_PEREMPTION, datePeremtion);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                 articleVenteInfo->_stockNomDepartementProduits);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::CATEGORIE,
                 articleVenteInfo->_stockCategorie);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::QUANTITE_VENDUE,
                 articleVenteInfo->_quantite_a_vendre);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                 articleVenteInfo->_prix_unitaire);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                 articleVenteInfo->prix_vente());

        stocksVenduCompteClientRecord.
                setValue(YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
                         articleVenteInfo->Get_ajout_pour_COMMERCIAL());

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::REMISE_PRIX,
                 articleVenteInfo->remise_prix());

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::REMISE_POURCENTAGE,
                 articleVenteInfo->remise_pourcentage());

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_TVA,
                 articleVenteInfo->montant_tva());

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::LOCALISATION_STOCK,
                 localisation_stock);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::LOCALISATION, localisation);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                 nomEntrepriseFournisseur);

        YerithPOSUser *
        user = _allWindows->getUser();

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::NOM_CAISSIER,
                 user->nom_complet());

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER,
                 user->nom_utilisateur());


        if (checkBox_DATE_EXTRA_VENTE->isChecked())
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::DATE_VENTE,
                     dateEdit_PDV_date_vente_extra->date());
        }
        else
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::DATE_VENTE,
                     GET_CURRENT_DATE);
        }


        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::HEURE_VENTE, CURRENT_TIME);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::STOCKS_ID,
                 articleVenteInfo->_stockID);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_RECU, _montantRecu);

        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::MONTANT_A_REMBOURSER,
                 lineEdit_articles_montant_a_rembourser->text().toDouble());

        if (articleVenteInfo->_effectuer_vente_en_gros)
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::IS_VENTE_EN_GROS, true);
        }
        else
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::IS_VENTE_EN_GROS, false);
        }

        double
        articleVenteInfo_prix_dachat =
                        YerithUtils::get_prix_dachat_wheter_exists(articleVenteInfo->
                                                                   get_stocksID());

        if (articleVenteInfo_prix_dachat > 0)
        {
            double
            marge_beneficiaire =
                            YerithUtils::getMargeBeneficiaire(articleVenteInfo->
                                                              prix_vente(),
                                                              articleVenteInfo_prix_dachat,
                                                              articleVenteInfo->
                                                              montant_tva());

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::MARGE_BENEFICIAIRE,
                     (marge_beneficiaire *
                      articleVenteInfo->_quantite_a_vendre));
        }

        double
        nouvelle_quantite_totale =
                        quantite_totale_actuelle - articleVenteInfo->_quantite_a_vendre;

        QString
        historiqueStockVendu;

        if (historique_stock_actif)
        {
            historiqueStockVendu = YerithHistoriqueStock::creer_mouvement_stock
                                   (MOUVEMENT_DE_STOCK_VENTE,
                                    stocksVenduID,
                                    GET_CURRENT_DATE,
                                    quantite_totale_actuelle,
                                    articleVenteInfo->_quantite_a_vendre, nouvelle_quantite_totale);

            historiqueStock.append(QString("%1%2").arg
                                   (YerithHistoriqueStock::SEPARATION_EXTERNE,
                                    historiqueStockVendu));
        }


        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                 historique_stock_actif);


        stocksVenduCompteClientRecord.
        setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                 historiqueStock);

        //qDebug() << QString("++ test: %1")
        //                     .arg(historiqueStock);

        YerithSqlTableModel &clientsTableModel =
                        _allWindows->getSqlTableModel_clients();

        QString
        clientFilter(QString("%1 = '%2'").arg
                     (YerithDatabaseTableColumn::NOM_ENTREPRISE,
                      lineEdit_articles_nom_client->text()));

        clientsTableModel.yerithSetFilter_WITH_where_clause(clientFilter);

        int clientsTableModelRowCount =
        		clientsTableModel.easySelect("src/windows/pos/yerith-erp-pointdevente-windows.cpp", 5155);

        if (clientsTableModelRowCount > 0)
        {
            QSqlRecord clientsRecord = clientsTableModel.record(0);

            QString
            clients_id(GET_SQL_RECORD_DATA
                       (clientsRecord, YerithDatabaseTableColumn::ID));


            QString
            quartier_entreprise_client = GET_SQL_RECORD_DATA(clientsRecord,
                                                             YerithDatabaseTableColumn::QUARTIER);

            QString
            ville_entreprise_client = GET_SQL_RECORD_DATA(clientsRecord,
                                                          YerithDatabaseTableColumn::VILLE);

            QString
            region_etat_entreprise_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerithDatabaseTableColumn::PROVINCE_ETAT);


            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
                     quartier_entreprise_client);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT,
                     ville_entreprise_client);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::
                     REGION_ETAT_ENTREPRISE_CLIENT,
                     region_etat_entreprise_client);


            bool is_compte_client_AVANCE_DE_CASH_calculated = false;

            if (!is_compte_client_AVANCE_DE_CASH_calculated)
            {
                double
                compteClient_AVANCE_DE_CASH =
                                GET_SQL_RECORD_DATA(clientsRecord,
                                                    YerithDatabaseTableColumn::
                                                    COMPTECLIENT_AVANCE_DE_CASH).toDouble();

                nouveau_compteClient_AVANCE_DE_CASH =
                                compteClient_AVANCE_DE_CASH - articleVenteInfo->prix_vente();

                stocksVenduCompteClientRecord.setValue
                (YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
                 nouveau_compteClient_AVANCE_DE_CASH);

                clientsRecord.setValue
                (YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
                 nouveau_compteClient_AVANCE_DE_CASH);

                is_compte_client_AVANCE_DE_CASH_calculated = true;
            }
            else
            {
                stocksVenduCompteClientRecord.setValue
                (YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
                 nouveau_compteClient_AVANCE_DE_CASH);

                clientsRecord.setValue
                (YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
                 nouveau_compteClient_AVANCE_DE_CASH);
            }

            clientsTableModel.updateRecord(0,
            							   clientsRecord,
										   "src/windows/pos/yerith-erp-pointdevente-window.cpp",
										   5226,
										   lineEdit_articles_nom_client->text());

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::CLIENTS_ID, clients_id);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                     lineEdit_articles_nom_client->text());

            clientsTableModel.resetFilter();

//                      updateCompteClient(nouveau_compte_client);
        }
        else
        {
            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::CLIENTS_ID, -1);

            stocksVenduCompteClientRecord.
            setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                     YerithUtils::STRING_FRENCH_DIVERS);
        }

        bool success1 = stocksVenduTableModel
        					.insertNewRecord(stocksVenduCompteClientRecord,
                                             this,
											 "src/windows/pos/yerith-erp-pointdevente-window.cpp",
											 5256,
											 articleVenteInfo->_stockName);

        if (success1)
        {
            if (nouvelle_quantite_totale < 0)
            {
                nouvelle_quantite_totale = 0;
            }

            if (0 == nouvelle_quantite_totale)
            {
                QString
                removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'").arg
                               (YerithDatabase::STOCKS,
                                YerithDatabaseTableColumn::ID,
                                articleVenteInfo->_stockID));
                YerithUtils::execQuery(removeRowQuery);
            }

            stockRecordQueryStr.clear();
            stockRecordQueryStr.append(QString
                                       ("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'").
                                       arg(YerithDatabase::STOCKS,
                                           YerithDatabaseTableColumn::QUANTITE_TOTALE,
                                           QString::number
                                           (nouvelle_quantite_totale),
                                           YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                           historiqueStock,
                                           YerithDatabaseTableColumn::ID,
                                           articleVenteInfo->_stockID));

            //qDebug() << QString("++ quantiteQueryStr: %1")
            //                      .arg(quantiteQueryStr);

            bool
            success2 = YerithUtils::execQuery(stockRecordQueryStr, _logger);

            QString
            sMsg(QObject::tr("La quantité en stock de l'article '"));

            sMsg.append(articleVenteInfo->_stockName).append("'").append(QObject::tr(" (%1 pièce(s))").arg(nouvelle_quantite_totale));	//TODO: CHECK IF THIS CORRECT BY EXAMPLE!

            if (success2)
            {
                sMsg.append(QObject::tr
                            (" a été actualisée avec succès."));
            }
            else
            {
                sMsg.append(QObject::tr
                            (" n'a pas pu être actualisée !\n"
                             "Contacter 'YERITH'"));
            }

            _logger->log("vendre", sMsg);
        }
        else
        {
            //TODO MESSAGE D'ERREUR DANS LE FICHIER DE LOGS
        }
    }

    emit
    SELLING();

    QString
    vMsg(QObject::tr
         ("La vente de '%1' articles a été éffectuée avec succès !").arg
         (QString::number(_quantiteAVendre)));

    if (QMessageBox::Ok ==
            YerithQMessageBox::information(this,
                                           QObject::tr("succès d'une vente"),
                                           vMsg))
    {
        imprimer_recu_vendu(referenceRecuVenduCompteClient_AVANCE_DE_CASH);
    }
}
