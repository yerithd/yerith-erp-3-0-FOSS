/*
 * yerith-erp-sortir-window.cpp
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-sortir-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/windows/stocks/yerith-erp-stock-detail-window.hpp"

#include "src/widgets/table-view/yerith-erp-table-view.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-info-entreprise.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/users/yerith-erp-users.hpp"

#include <QtCore/QThread>

#include <QtCore/QFile>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

bool YerithSortirWindow::_qteChangeCodeBar(false);


YerithSortirWindow::YerithSortirWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithSortirWindow")),
 _currentFocusSearchBar(0),
 _barcodeReaderActivated(false),
 _linuxWhich(0),
 _updateItemConversionError(false),
 _previousPressedQteValue("1"),
 _tvaCheckBoxPreviousState(false),
 _sommeTotal(0.0),
 _tva(0.0),
 _quantiteVendue(0),
 _currentTabView(0),
 actionRechercheArticle(0),
 actionRechercheArticleCodebar(0)
{
	_windowName = YerithMainWindow::get_TRANSLATED_WindowName("sortir / transférer des IMMOBILISATIONS OU stocks");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_articles->font());

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0);

    tableWidget_articles->resizeColumnsToContents();

    _currentFocusSearchBar = INITIAL_FOCUS_SEARCHBAR_SORTIR_WINDOW;

    setRechercheLineEditFocus();

    checkBox_imprimer_recu_sortie->setChecked(false);

    comboBox_articles_nom_client->setYerithEditable(true);

    comboBox_articles_transfert->setYerithEditable(true);

    populateComboBoxes();

    setupLineEdits();

    setupDateTimeEdits();

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_lecteur_de_code_barres->setVisible(_barcodeReaderActivated);

    pushButton_minus->enable(this, SLOT(retirer_un_article()));

    // Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(afficher_facture_pdf()));
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

    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));

    connect(tableWidget_articles,
    		SIGNAL(itemSelectionChanged()),
            this,
			SLOT(handle_VIEW_FONT_SIZE_change_buttons()));

    connect(tableWidget_articles, SIGNAL(itemPressed(QTableWidgetItem *)),
            this, SLOT(handleQtePressed(QTableWidgetItem *)));

    connect(tableWidget_articles, SIGNAL(itemChanged(QTableWidgetItem *)),
            this, SLOT(handleQteChange(QTableWidgetItem *)));

    connect(tabWidget_sorties, SIGNAL(currentChanged(int)), this,
            SLOT(handleCurrentChanged(int)));

    connect(tableWidget_articles, SIGNAL(addedYerithTableWidget()), this,
            SLOT(handleAddedArticle()));

    connect(tableWidget_articles,
            SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this,
            SLOT(afficher_sortie_detail(QTableWidgetItem *)));

    /* Signals-slots connection for the second tab 'Article au détail' */
    connect(checkBox_tva, SIGNAL(clicked(bool)), this,
            SLOT(handleTVACheckBox(bool)));

    connect(lineEdit_article_detail_quantite_a_vendre,
            SIGNAL(textEdited(const QString &)), this,
            SLOT(updateQuantiteAVendre()));

    connect(lineEdit_article_detail_AJOUT_COMMERCIAL,
            SIGNAL(textEdited(const QString)),
            this, SLOT(calculate_details_window_AJOUT_COMMERCIAL()));

    connect(lineEdit_article_detail_remise_prix,
            SIGNAL(textChanged(const QString)), this,
            SLOT(calculate_details_window_remise_prix()));

    connect(lineEdit_article_detail_remise_pourcentage,
            SIGNAL(textChanged(const QString)), this,
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

    setupShortcuts();
}


YerithSortirWindow::~YerithSortirWindow()
{
    delete _logger;
    delete actionRechercheArticle;
    delete actionRechercheArticleCodebar;
    deleteArticleVenteInfos();
}


void YerithSortirWindow::deleteArticleVenteInfos()
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


void YerithSortirWindow::setBarcodeAsStandardInput()
{
    _currentFocusSearchBar = lineEdit_recherche_article_codebar;
    lineEdit_recherche_article_codebar->setFocus();
}


void YerithSortirWindow::setStockItemNameAsStandardInput()
{
    _currentFocusSearchBar = lineEdit_recherche_article;
    lineEdit_recherche_article->setFocus();
}


void YerithSortirWindow::updateLineEditQCompleterInput()
{
    if (_barcodeReaderActivated)
    {
        connect_barcode_reader_selection_of_article_item();
    }
    else
    {
        connect_manual_selection_of_article_item();
    }
}


void YerithSortirWindow::handleBasculerLecteurDeCodebarres()
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


void YerithSortirWindow::handleRefreshSaleStrategy()
{
    YerithUtils::refreshSalesStrategy(*_curStocksTableModel,
                                      lineEdit_recherche_article,
                                      lineEdit_recherche_article_codebar);

    updateLineEditQCompleterInput();
}


void YerithSortirWindow::disconnect_all_objects_for_stock_article_item_selection()
{
    disconnect(lineEdit_recherche_article, 0, 0, 0);
    disconnect(lineEdit_recherche_article_codebar, 0, 0, 0);
    disconnect(lineEdit_recherche_article->getMyQCompleter(), 0, 0, 0);
    disconnect(lineEdit_recherche_article_codebar->getMyQCompleter(), 0, 0,
               0);
}


void YerithSortirWindow::connect_manual_selection_of_article_item()
{
    disconnect_all_objects_for_stock_article_item_selection();

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

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
                SLOT(ajouter_article(const QString &)));

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
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else				//ALL == YerithConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
}


void YerithSortirWindow::connect_barcode_reader_selection_of_article_item()
{
    disconnect_all_objects_for_stock_article_item_selection();

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             YerithERPConfig::salesStrategy))
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

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
                SLOT(ajouter_article(const QString &)));

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
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar,
                SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
    else				//ALL == YerithConfig::salesStrategy
    {
        connect(lineEdit_recherche_article->getMyQCompleter(),
                SIGNAL(activated(const QString &)), this,
                SLOT(ajouter_article(const QString &)));

        connect(lineEdit_recherche_article_codebar,
                SIGNAL(textChanged(const QString &)), this,
                SLOT(ajouter_article_codebar(const QString &)));
    }
}


bool YerithSortirWindow::check_fields()
{
    bool nom_recepteur = lineEdit_articles_nom_recepteur->checkField();
    if (false == nom_recepteur)
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("nom du récepteur de(s) stock(s) sortant"),
                                   QObject::tr
                                   ("Entrez le nom du récepteur de(s) stock(s) sortant !"));
        return false;
    }
    QString currentLocalisation(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    bool destinationIsCorrect =
                    !YerithUtils::isEqualCaseInsensitive(currentLocalisation,
                                                         comboBox_articles_transfert->
                                                         currentText());

    if (!currentLocalisation.isEmpty() && !destinationIsCorrect)
    {
        YerithQMessageBox::warning(this, QObject::tr("localisation"),
                                   QObject::tr
                                   ("La localisation d'entrée doit"
                                    " être différente de la localisation de sortie !"));
        return false;
    }
    return true;
}


void YerithSortirWindow::setup_AJOUT_COMMERCIAL(bool aValue)
{
	radioButton_article_detail_AJOUT_COMMERCIAL->setEnabled(aValue);
	lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(aValue);
}


void YerithSortirWindow::clear_all_fields()
{
    tableWidget_articles->yerithClearTableWidgetContent();
    lineEdit_recherche_article->myClear();
    lineEdit_recherche_article_codebar->myClear();
    lineEdit_articles_quantite_a_transferer->clear();
    lineEdit_articles_quantite_a_transferer->setText(GET_DOUBLE_STRING(0.0));
    comboBox_articles_transfert->resetYerithComboBox();
    textEdit_articles_notes->clear();
    lineEdit_articles_nom_recepteur->clearField();
}


void YerithSortirWindow::populateComboBoxes()
{
    comboBox_articles_nom_client->
    populateComboBoxRawString(YerithDatabase::CLIENTS,
                              YerithDatabaseTableColumn::NOM_ENTREPRISE);

    comboBox_articles_transfert->
    populateComboBoxRawString(YerithDatabase::LOCALISATIONS,
                              YerithDatabaseTableColumn::NOM_LOCALISATION);
}


void YerithSortirWindow::setupLineEdits()
{
    lineEdit_recherche_article->set_show_ASSET(true);

    lineEdit_recherche_article_codebar->set_show_ASSET(true);

    lineEdit_recherche_article_codebar->enableForSearch(QObject::tr
                                                        ("référence [ focus avec F11 ]"));
    lineEdit_recherche_article->enableForSearch(QObject::tr
                                                ("désignation [ focus avec F12 ]"));

    lineEdit_articles_imprimante->setText(YerithERPConfig::printer);
    lineEdit_articles_imprimante->setReadOnly(true);

    lineEdit_article_detail_nom_client->setYerithEnabled(false);
    lineEdit_article_detail_reference_produit->setYerithEnabled(false);
    lineEdit_article_detail_designation->setYerithEnabled(false);
    lineEdit_article_detail_nom_entreprise_fournisseur->setYerithEnabled
    (false);
    lineEdit_article_detail_categorie->setYerithEnabled(false);
    lineEdit_article_detail_prix_unitaire->setYerithEnabled(false);
    lineEdit_article_detail_tva->setYerithEnabled(false);
    lineEdit_article_detail_destination->setYerithEnabled(false);
    lineEdit_article_detail_nom_magasinier->setYerithEnabled(false);
    lineEdit_article_detail_nom_recepteur->setYerithEnabled(false);
    lineEdit_article_detail_quantite_en_stock->setYerithEnabled(false);
    lineEdit_articles_nom_magasinier->setYerithEnabled(false);
    lineEdit_articles_quantite_a_transferer->setYerithEnabled(false);
    lineEdit_articles_quantite_a_transferer->setText(GET_DOUBLE_STRING(0.0));
    //qDebug() << "++ YerithSortirWindow::setupLineEdits(): " << YerithConfig::salesStrategy;

    lineEdit_article_detail_quantite_a_vendre->
    	setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_article_detail_AJOUT_COMMERCIAL->
      	setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_article_detail_remise_prix->
    	setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_article_detail_remise_pourcentage->
    	setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_article_detail_AJOUT_COMMERCIAL->setEnabled(false);

    lineEdit_article_detail_remise_prix->setYerithEnabled(false);

    lineEdit_article_detail_remise_pourcentage->setYerithEnabled(false);
}

void YerithSortirWindow::hideEvent(QHideEvent *hideEvent)
{
    _allWindows->_sortirListStocksWindow->close();
}

void YerithSortirWindow::setupShortcuts()
{
    actionRechercheArticle = new QAction(this);
    actionRechercheArticleCodebar = new QAction(this);
    lineEdit_recherche_article->addAction(actionRechercheArticle);
    lineEdit_recherche_article_codebar->addAction
    (actionRechercheArticleCodebar);
    connect(actionRechercheArticle, SIGNAL(triggered()), this,
            SLOT(setRechercheDesignationArticleFocus()));
    connect(actionRechercheArticleCodebar, SIGNAL(triggered()), this,
            SLOT(setRechercheCodebarArticleFocus()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionRechercheArticle->setShortcut(Qt::Key_F12);
    actionRechercheArticleCodebar->setShortcut(Qt::Key_F11);
    actionStocks->setShortcut(YerithUtils::LISTER_STOCKS_QKEYSEQUENCE);
}


void YerithSortirWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_sorties->enable(this,
                                                     SLOT(retourSorties()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}


void YerithSortirWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_sorties->enable(this,
                                                     SLOT(retourSorties()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerithSortirWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_article_detail_retour_sorties->enable(this,
                                                     SLOT(retourSorties()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}

void YerithSortirWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_article_detail_retour_sorties->disable(this);
    pushButton_annuler->disable(this);
    pushButton_sortir->disable(this);
}


void YerithSortirWindow::incrementFontSize__OF_TABLE()
{
	if (0 != tableWidget_articles &&
		0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		++_TABLE_FONT_SIZE;

		_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

		//QDEBUG_STRING_OUTPUT_2_N("incrementFontSize__OF_TABLE, _TABLE_FONT_SIZE",
		//						  _TABLE_FONT_SIZE);

		tableWidget_articles->hide();

		tableWidget_articles->resizeColumnsToContents();

		tableWidget_articles->show();

        YerithERPWindows *allWindows = YerithUtils::getAllWindows();

        YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
	}
}


void YerithSortirWindow::decrementFontSize__OF_TABLE()
{
	if (0 != tableWidget_articles &&
		0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		if (_TABLE_FONT_SIZE > 5)
		{
			--_TABLE_FONT_SIZE;

			_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

			//QDEBUG_STRING_OUTPUT_2_N("decrementFontSize__OF_TABLE, _TABLE_FONT_SIZE",
			//						  _TABLE_FONT_SIZE);

			tableWidget_articles->hide();

			tableWidget_articles->resizeColumnsToContents();

			tableWidget_articles->show();

            YerithERPWindows *allWindows = YerithUtils::getAllWindows();

            YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
		}
	}
}


QString YerithSortirWindow::afficher_facture_pdf(QString referenceRecuSORTIE
                                                 /* = QString("") */)
{
    QString
    pdfReceiptFileName;

    if (YerithERPConfig::RECEIPT_FORMAT_PETIT == YerithERPConfig::receiptFormat)
    {
        pdfReceiptFileName.append(imprimer_facture_petit
                                  (referenceRecuSORTIE));
    }
    else
    {
        pdfReceiptFileName.append(imprimer_facture_grand
                                  (referenceRecuSORTIE));
    }

    if (!pdfReceiptFileName.isEmpty())
    {
        return YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    return "";
}

QString YerithSortirWindow::imprimer_facture(QString referenceRecuSORTIE
                                             /* = QString("") */)
{
    if (!checkBox_imprimer_recu_sortie->isChecked())
    {
        return YerithUtils::EMPTY_STRING;
    }

    QString
    pdfReceiptFileName;

    if (YerithERPConfig::RECEIPT_FORMAT_PETIT == YerithERPConfig::receiptFormat)
    {
        pdfReceiptFileName.append(imprimer_facture_petit
                                  (referenceRecuSORTIE));

        if (YerithUtils::isEqualCaseInsensitive(YerithERPConfig::printer,
                                                YerithUtils::IMPRIMANTE_PDF))
        {
            return YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
        }
    }
    else
    {
        pdfReceiptFileName.append(imprimer_facture_grand
                                  (referenceRecuSORTIE));

        return YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    if (false ==
            YerithUtils::isEqualCaseInsensitive(YerithUtils::IMPRIMANTE_PDF,
                                                YerithERPConfig::printer))
    {
        QString
        thermalPrinterTxtFileEnding("yerith-erp-3-0-txt");

        QString
        pdfReceiptFileName_txt(QString("%1-%2").arg
                               (pdfReceiptFileName,
                                thermalPrinterTxtFileEnding));

        QStringList
        progOptions;

        progOptions << "-layout";
        progOptions << pdfReceiptFileName;
        progOptions << pdfReceiptFileName_txt;

        if (YerithERPProcess::startAndWaitForFinished("/usr/bin/pdftotext",
                                                      progOptions,
                                                      YerithUtils::
                                                      PRINT_TIMEOUT))
        {
            QThread::sleep(0.5);

            progOptions.clear();

            QString
            LINE_FEED_CHARACTERS_FOR_THERMAL_SMALL_PRINTING;
            for (unsigned int k = 0;
                    k <
                    YerithERPConfig::
                    LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING; ++k)
            {
                LINE_FEED_CHARACTERS_FOR_THERMAL_SMALL_PRINTING.append("\n");
            }

            progOptions << "-c";
            progOptions <<
                        QString("/bin/echo -e \"%1\" >> %2").arg
                        (LINE_FEED_CHARACTERS_FOR_THERMAL_SMALL_PRINTING,
                         pdfReceiptFileName_txt);

            YerithERPProcess::startAndWaitForFinished("/bin/bash",
                                                      progOptions,
                                                      YerithUtils::
                                                      PRINT_TIMEOUT);

            QThread::sleep(0.1);

            progOptions.clear();

            progOptions << "-c";
            progOptions <<
                        QString("/bin/cat %1 >> %2").arg(pdfReceiptFileName_txt,
                                                         YerithERPConfig::pathToThermalPrinterDeviceFile);

            YerithERPProcess::startAndWaitForFinished("/bin/bash",
                                                      progOptions,
                                                      YerithUtils::
                                                      PRINT_TIMEOUT);

            return pdfReceiptFileName;
        }
    }

    return "";
}


QString YerithSortirWindow::imprimer_facture_grand(QString referenceRecuSortieGRAND
                                                   /* = QString("") */)
{

    _logger->log("imprimer_facture_grand");

    QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("sortir des stocks - pas de données !"),
                                       QObject::tr
                                       ("Il n'y a pas de données à imprimer !"));
        return "";
    }

    YerithUtils::getFactureTexTableString(factureTexTable,
                                          *tableWidget_articles,
                                          _quantiteVendue, _tva, _sommeTotal);

    YerithPOSUser *yerithUser = _allWindows->getUser();

    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVille_LATEX());

    YerithUtils::getCurrentLocaleDate(factureDate);

    YerithUtils::getSortieDesStocksTexDocumentString(factureTexDocument,
                                                     factureTexTable);

    if (referenceRecuSortieGRAND.isEmpty())
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }
    else
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (referenceRecuSortieGRAND));
    }


    QString destinationTex(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                           (comboBox_articles_transfert->currentText()));

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

    QString recepteur(lineEdit_articles_nom_recepteur->text());

    factureTexDocument.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    factureTexDocument.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YERITHLOCALISATION_SORTIE",
                               infoEntreprise.getLocalisation_LATEX());
    factureTexDocument.replace("YERITHLOCALISATION_ENTREE", destinationTex);
    factureTexDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    factureTexDocument.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());
    factureTexDocument.replace("YERITHEMAIL",
                               infoEntreprise.getEmail_LATEX());
    factureTexDocument.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());
    factureTexDocument.replace("YERITHDATE", factureDate);
    factureTexDocument.replace("YERITHHEUREVENTE", CURRENT_TIME);
    factureTexDocument.replace("YERITHMAGASINIER",
                               yerithUser->nom_completTex());
    factureTexDocument.replace("YERITHRECEPTEUR",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (recepteur));
    factureTexDocument.replace("YERITHCOMPTEBANCAIRENR",
                               infoEntreprise.getNumeroCompteBancaire());
    factureTexDocument.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YERITHAGENCECOMPTEBANCAIRE",
                               infoEntreprise.getAgenceCompteBancaireTex());
    factureTexDocument.replace("YERITHSOMME",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (GET_CURRENCY_STRING_NUM(_sommeTotal)));

    QString nom_client(comboBox_articles_nom_client->currentText());

    QString nom_succursale(comboBox_articles_transfert->currentText());

    bool isClient = true;

    if (nom_client.isEmpty())
    {
        if (!nom_succursale.isEmpty())
        {
            isClient = false;
            handle_info_succursale(factureTexDocument, nom_succursale);
        }
        else
        {
            nom_client = "Client \"DIVERS\"";
            factureTexDocument.replace("YERITHCLIENT",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (nom_client));
            factureTexDocument.replace("CLIENTYERITHPOBOX", "");
            factureTexDocument.replace("CLIENTYERITHCITY", "");
            factureTexDocument.replace("CLIENTYERITHMAIL", "");
            factureTexDocument.replace("CLIENTYERITHPHONE", "");
        }
    }
    else
    {
        handle_info_client(factureTexDocument, nom_client);
    }

    //qDebug() << "++\n" << factureTexDocument;

    QString prefixFileName;

#ifdef YERITH_FRANCAIS_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Sortie");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-sortie-grand"));
    }
    else
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Transfert");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-transfert-grand"));
    }

#endif

#ifdef YERITH_ENGLISH_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Outgoing");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-outgoing-big"));
    }
    else
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Transfer");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-transfer-big"));
    }

#endif

    QFile tmpLatexFile(prefixFileName + "tex");

    if (tmpLatexFile.open(QFile::WriteOnly))
    {
        tmpLatexFile.write(factureTexDocument.toUtf8());
    }

    tmpLatexFile.close();

    return YerithERPProcess::compileLatex(prefixFileName);
}

QString YerithSortirWindow::imprimer_facture_petit(QString referenceRecuSortiePETIT
                                                   /* = QString("") */)
{
    _logger->log("imprimer_facture_petit");

    QString factureTexTable;

    if (tableWidget_articles->rowCount() <= 0)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("impression"),
                                       QObject::tr
                                       ("Il n'y a pas de données à imprimer !"));
        return "";
    }

    YerithPOSUser *yerithUser = _allWindows->getUser();

    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVille_LATEX());

    YerithUtils::getCurrentSimplifiedDate(factureDate);

    YerithUtils::getFactureSmallTexTableString(factureTexTable,
                                               *tableWidget_articles,
                                               _quantiteVendue, _tva,
                                               _sommeTotal);

    factureTexTable.append(QString("MERCI!\\"));

    YerithUtils::getSortieDesStocksSmallTexDocumentString(factureTexDocument,
                                                          factureTexTable);


    if (referenceRecuSortiePETIT.isEmpty())
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   QObject::tr("EXEMPLE (*NON VALIDE*)"));
    }
    else
    {
        factureTexDocument.replace("YERITHNUMEROSORTIETRANSFERT",
                                   YerithUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (referenceRecuSortiePETIT));
    }

    //_logger->debug("imprimer_facture_petit",
    //QString("document: %1").arg(factureTexDocument));

    static const double INITIAL_INCH_SIZE = 4.05;

    static const double ITEM_INCH_SIZE = 0.2;

    int articleItemCount = tableWidget_articles->rowCount();

    double factureInchSize =
                    INITIAL_INCH_SIZE + (articleItemCount * ITEM_INCH_SIZE);

    QString
    minPaperHeight(QString("%1in").arg
                   (QString::number(factureInchSize, 'f', 2)));
    //qDebug() << "\t++minPaperHeight: " << minPaperHeight;

    factureTexDocument.replace("YERITHFACTURESMALLPAPERHEIGHT",
                               minPaperHeight);

    QString destinationTex(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                           (comboBox_articles_transfert->currentText()));

    factureTexDocument.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    factureTexDocument.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YERITHLOCALISATION_SORTIE",
                               infoEntreprise.getLocalisation_LATEX());
    factureTexDocument.replace("YERITHLOCALISATION_ENTREE", destinationTex);
    factureTexDocument.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());
    factureTexDocument.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());
    factureTexDocument.replace("YERITHDATE", factureDate);
    factureTexDocument.replace("YERITHHEUREVENTE", CURRENT_TIME);
    factureTexDocument.replace("YERITHMAGASINIER",
                               yerithUser->nom_completTex());
    factureTexDocument.replace("YERITHRECEPTEUR",
                               lineEdit_articles_nom_recepteur->text());

    QString nom_client(comboBox_articles_nom_client->currentText());

    QString nom_succursale(comboBox_articles_transfert->currentText());

    bool isClient = true;

    if (nom_client.isEmpty())
    {
        if (!nom_succursale.isEmpty())
        {
            isClient = false;
        }
        else
        {
            nom_client.append("Client \"DIVERS\"");
            factureTexDocument.replace("YERITHCLIENT",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (nom_client));
            factureTexDocument.replace("CLIENTYERITHPOBOX", "");
            factureTexDocument.replace("CLIENTYERITHCITY", "");
            factureTexDocument.replace("CLIENTYERITHMAIL", "");
            factureTexDocument.replace("CLIENTYERITHPHONE", "");
        }
    }
    else
    {
        handle_info_client(factureTexDocument, nom_client);
    }

    //qDebug() << "++\n" << factureTexDocument;

    QString prefixFileName;

#ifdef YERITH_FRANCAIS_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Sortie");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-sortie-petit"));
    }
    else
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Transfert");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-transfert-petit"));
    }

#endif

#ifdef YERITH_ENGLISH_LANGUAGE

    if (isClient)
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Outgoing");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-outgoing-small"));
    }
    else
    {
        factureTexDocument.replace("YERITHSORTIETRANSFERT", "Transfer");
        prefixFileName.append
        (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
         ("yerith-erp-stock-transfer-small"));
    }

#endif


    QFile tmpLatexFile(QString("%1tex").arg(prefixFileName));

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
                                                    factureTexDocument);

    return YerithERPProcess::compileLatex(prefixFileName);
}


void YerithSortirWindow::handle_info_succursale(QString &factureTexDocument,
                                                const QString nom_succursale)
{
    QString succursale;
    succursale.append("Succursale de ").append("\"").append(nom_succursale).
    append("\"");
    YerithSqlTableModel &localisationTableModel =
                    _allWindows->getSqlTableModel_localisations();
    localisationTableModel.
    yerithSetFilter_WITH_where_clause(QString("nom_localisation = '%1'").
                                      arg(nom_succursale));
    //qDebug() << "++ nom_succursale: " << nom_succursale;

    factureTexDocument.replace("YERITHCLIENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (succursale));

    if (localisationTableModel.easySelect("src/windows/stocks/yerith-erp-sortir-windows.cpp", 1171) > 0)
    {
        QSqlRecord record = localisationTableModel.record(0);
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
                  (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
        if (clientTel.isEmpty())
        {
            clientTel.append(GET_SQL_RECORD_DATA
                             (record,
                              YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
        }
        if (!clientVille.isEmpty())
            factureTexDocument.replace("CLIENTYERITHCITY",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientVille));
        else
            factureTexDocument.replace("CLIENTYERITHCITY", "");
        if (!clientPOBox.isEmpty())
            factureTexDocument.replace("CLIENTYERITHPOBOX",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientPOBox));
        else
            factureTexDocument.replace("CLIENTYERITHPOBOX", "N/a");
        if (!clientEmail.isEmpty())
            factureTexDocument.replace("CLIENTYERITHMAIL",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientEmail));
        else
            factureTexDocument.replace("CLIENTYERITHMAIL", "");
        if (!clientTel.isEmpty())
            factureTexDocument.replace("CLIENTYERITHPHONE",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientTel));
        else
            factureTexDocument.replace("CLIENTYERITHPHONE", "");
    }
    localisationTableModel.resetFilter();
}

void YerithSortirWindow::handle_info_client(QString &factureTexDocument,
                                            const QString nom_client)
{
    YerithSqlTableModel &clientTableModel =
                    _allWindows->getSqlTableModel_clients();
    clientTableModel.yerithSetFilter_WITH_where_clause(QString
                                                       ("nom_entreprise = '%1'").
                                                       arg(nom_client));
    //qDebug() << "++ nom_client: " << nom_client;
    factureTexDocument.replace("YERITHCLIENT",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (nom_client));

    if (clientTableModel.easySelect("src/windows/stocks/yerith-erp-sortir-windows.cpp", 1237) > 0)
    {
        QSqlRecord record = clientTableModel.record(0);
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
                  (record, YerithDatabaseTableColumn::NUMERO_TELEPHONE_1));
        if (clientTel.isEmpty())
        {
            clientTel.append(GET_SQL_RECORD_DATA
                             (record,
                              YerithDatabaseTableColumn::NUMERO_TELEPHONE_2));
        }
        if (!clientVille.isEmpty())
            factureTexDocument.replace("CLIENTYERITHCITY",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientVille));
        else
            factureTexDocument.replace("CLIENTYERITHCITY", "");
        if (!clientPOBox.isEmpty())
            factureTexDocument.replace("CLIENTYERITHPOBOX",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientPOBox));
        else
            factureTexDocument.replace("CLIENTYERITHPOBOX", "N/a");
        if (!clientEmail.isEmpty())
            factureTexDocument.replace("CLIENTYERITHMAIL",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientEmail));
        else
            factureTexDocument.replace("CLIENTYERITHMAIL", "");
        if (!clientTel.isEmpty())
            factureTexDocument.replace("CLIENTYERITHPHONE",
                                       YerithUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (clientTel));
        else
            factureTexDocument.replace("CLIENTYERITHPHONE", "");
    }
    clientTableModel.resetFilter();
}


void YerithSortirWindow::annuler()
{
    if (tableWidget_articles->rowCount() > 0)
    {
        clear_all_fields();

        deleteArticleVenteInfos();

        setRechercheLineEditFocus();

        handleTabViews();

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("annulation de sortie"),
                                       QObject::tr
                                       ("Vous avez annulé la sortie !"),
                                       QMessageBox::Ok);

        handle_VIEW_FONT_SIZE_change_buttons();
    }
}

void YerithSortirWindow::lister()
{
    _logger->log("lister");
    _allWindows->_sortirListStocksWindow->listStocks(*_curStocksTableModel);
}

void YerithSortirWindow::retourSorties()
{
    _currentTabView = TableauDesSorties;
    tabWidget_sorties->setCurrentIndex(TableauDesSorties);
}

void YerithSortirWindow::handleAddedArticle()
{
    //_logger->debug("handleAddedArticle()");
    tabWidget_sorties->setTabEnabled(AfficherSortieAuDetail, true);
}


void YerithSortirWindow::handleCurrentChanged(int index)
{
    //_logger->debug("handleCurrentChanged(int)", QString("index: %1").arg(index));
    _currentTabView = index;
    switch (index)
    {
    case TableauDesSorties:
        afficher_tableau_sorties(tableWidget_articles->currentRow());
        break;
    case AfficherSortieAuDetail:
        afficher_sortie_detail(tableWidget_articles->currentRow());
        disableImprimer();
        _logger->debug("handleCurrentChanged(int)",
                       QString("current table row: %1").arg
                       (tableWidget_articles->currentRow()));
        break;
    default:
        break;
    }
}

void YerithSortirWindow::cleanUpAfterVente()
{
    _logger->log("cleanUpAfterVente");

    resetCheckboxTVA();

    tableWidget_articles->yerithClearTableWidgetContent();

    articleItemToVenteInfo.clear();

    deleteArticleVenteInfos();

    lineEdit_recherche_article->myClear();
    lineEdit_recherche_article_codebar->myClear();

    tabWidget_sorties->setCurrentIndex(TableauDesSorties);

    handleTabViews();

    lineEdit_articles_quantite_a_transferer->clear();
    comboBox_articles_transfert->resetYerithComboBox();
    comboBox_articles_nom_client->resetYerithComboBox();
    textEdit_articles_notes->clear();
    lineEdit_articles_nom_recepteur->clear();
    lineEdit_article_detail_reference_produit->clear();
    lineEdit_article_detail_designation->clear();
    lineEdit_article_detail_nom_entreprise_fournisseur->clear();
    lineEdit_article_detail_categorie->clear();
    lineEdit_article_detail_prix_unitaire->clear();
    lineEdit_article_detail_tva->clear();
    lineEdit_article_detail_destination->clear();
    lineEdit_article_detail_nom_magasinier->clear();
    lineEdit_article_detail_nom_magasinier->clear();
    lineEdit_article_detail_quantite_en_stock->clear();
    lineEdit_article_detail_quantite_a_vendre->clear();
    lineEdit_article_detail_AJOUT_COMMERCIAL->clear();
    lineEdit_article_detail_remise_prix->clear();
    lineEdit_article_detail_remise_pourcentage->clear();

    radioButton_article_detail_remise_prix->clearFocus();
    radioButton_article_detail_remise_pourcentage->clearFocus();

    lineEdit_article_detail_quantite_a_vendre->setFocus();

    handleRefreshSaleStrategy();

    handle_VIEW_FONT_SIZE_change_buttons_FROM__SELLING_signal();

    setRechercheLineEditFocus();
}


void YerithSortirWindow::rendreInvisible()
{
    _logger->log("rendreInvisible");
    _qteChangeCodeBar = false;
    tableWidget_articles->yerithClearTableWidgetContent();
    articleItemToVenteInfo.clear();
    deleteArticleVenteInfos();
    lineEdit_recherche_article->clear();
    lineEdit_recherche_article_codebar->clear();
    lineEdit_articles_quantite_a_transferer->clear();
    comboBox_articles_transfert->resetYerithComboBox();
    comboBox_articles_nom_client->resetYerithComboBox();
    lineEdit_articles_nom_recepteur->clear();
    textEdit_articles_notes->clear();
    YerithWindowsCommons::rendreInvisible();
}


void YerithSortirWindow::handleTabViews()
{
    if (0 == tableWidget_articles->rowCount())
    {
        tabWidget_sorties->setTabEnabled(AfficherSortieAuDetail, false);
    }
}


void YerithSortirWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    lineEdit_articles_imprimante->setText(YerithERPConfig::printer);

    //populateComboBoxes();

    tabWidget_sorties->setTabEnabled(AfficherSortieAuDetail, false);

    if (!_curClientName.isEmpty())
    {
        comboBox_articles_transfert->find_AND_SET_CURRENT_INDEX
        (_curClientName);
        _curClientName.clear();
    }
    else
    {
        comboBox_articles_transfert->resetYerithComboBox();
    }

    resetCheckboxTVA();

    tabWidget_sorties->setCurrentIndex(TableauDesSorties);

    lineEdit_articles_nom_magasinier->setText(_allWindows->getUser()->
                                              nom_complet());

    handleRefreshSaleStrategy();

    handleTabViews();

    setRechercheLineEditFocus();

    QString aNewWindowTitle;

    aNewWindowTitle = YerithUtils::getWindowTitleWithStrategy(this);

    aNewWindowTitle = YerithUtils::appendPDFReceiptFormat(aNewWindowTitle);

    YERITH_set_windowName_TRANSLATED(
    		YerithMainWindow::get_TRANSLATED_WindowName(const_cast<char *> (aNewWindowTitle.toStdString().c_str())),
      								 _curStocksTableModel);

    check_fields_entry();

    if (tableWidget_articles->rowCount() > 0)
    {
        enableImprimer();
    }
    else
    {
        disableImprimer();
    }

    setVisible(true);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS_QTABLEWIDGET(*this);
}


void YerithSortirWindow::setRechercheLineEditFocus()
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


void YerithSortirWindow::setRechercheDesignationArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article;
    setRechercheLineEditFocus();
}


void YerithSortirWindow::setRechercheCodebarArticleFocus()
{
    _currentFocusSearchBar = lineEdit_recherche_article_codebar;
    setRechercheLineEditFocus();
}


void YerithSortirWindow::activateLineEdit_AJOUT_COMMERCIAL(bool toggled)
{
    if (toggled)
    {
    	lineEdit_article_detail_AJOUT_COMMERCIAL->setYerithEnabled(true);
    }
    else
    {
    	lineEdit_article_detail_AJOUT_COMMERCIAL->setYerithEnabled(false);
    }
}


void YerithSortirWindow::activateLineEditRemisePrix(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_prix->setYerithEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_prix->setYerithEnabled(false);
    }
}


void YerithSortirWindow::activateLineEditRemisePourcentage(bool toggled)
{
    if (toggled)
    {
        lineEdit_article_detail_remise_pourcentage->setYerithEnabled(true);
    }
    else
    {
        lineEdit_article_detail_remise_pourcentage->setYerithEnabled(false);
    }
}


void YerithSortirWindow::calculate_details_window_AJOUT_COMMERCIAL()
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


void YerithSortirWindow::calculate_details_window_remise_prix()
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
                        (100.0 * remise_prix) / articleVenteInfo->_prix_unitaire;

        articleVenteInfo->setRemisePrix(remise_prix);

        articleVenteInfo->setRemisePourcentage(remise_pourcentage);

        lineEdit_article_detail_remise_pourcentage->setText(articleVenteInfo->
                                                            remisePourcentage
                                                            ());
    }

    actualiser_montant_remise();
}


void YerithSortirWindow::calculate_details_window_remise_pourcentage()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    if (0 != articleVenteInfo
            && radioButton_article_detail_remise_pourcentage->isChecked())
    {
        double remise_pourcentage =
                        lineEdit_article_detail_remise_pourcentage->text().toDouble();

        double remise_prix =
                        (articleVenteInfo->_prix_unitaire * remise_pourcentage) / 100.0;

        articleVenteInfo->setRemisePourcentage(remise_pourcentage);

        articleVenteInfo->setRemisePrix(remise_prix);

        lineEdit_article_detail_remise_prix->setText(articleVenteInfo->
                                                     remisePrix());
    }

    actualiser_montant_remise();
}


void YerithSortirWindow::actualiser_montant_remise()
{
    int tableWidgetRow = tableWidget_articles->currentRow();

    if (tableWidgetRow < 0)
    {
        return;
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

    double prix_unitaire =
                    articleVenteInfo->prix_unitaire() - articleVenteInfo->remise_prix();

    double montant_tva = prix_unitaire * YerithERPConfig::tva_value;

    if (checkBox_tva->isChecked())
    {
        articleVenteInfo->_montant_tva = montant_tva;
    }

    lineEdit_article_detail_tva->setText(articleVenteInfo->montantTva());

    lineEdit_article_detail_prix_unitaire->setText(GET_CURRENCY_STRING_NUM
                                                   (prix_unitaire));
}

void YerithSortirWindow::resetCheckboxTVA()
{
    _tvaCheckBoxPreviousState = false;
    checkBox_tva->setChecked(false);
}

void YerithSortirWindow::handleTVACheckBox(bool clicked)
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

void YerithSortirWindow::updateQuantiteAVendre()
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
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("pas assez d'articles en stock"),
                                   QObject::tr
                                   ("Il n'y a pas assez d'articles en stock !"));
    }
    else
    {
        articleVenteInfo->_quantite_a_vendre = itemNewQteDouble;
    }

    actualiser_montant_remise();
}


void YerithSortirWindow::afficher_tableau_sorties(const int tableWidgetRow)
{
    tableWidget_articles->yerithClearTableWidgetContent();

    QMapIterator < int,
                 YerithArticleVenteInfo * >itArticleItemVenteInfo(articleItemToVenteInfo);

    while (itArticleItemVenteInfo.hasNext())
    {
        itArticleItemVenteInfo.next();

        YerithArticleVenteInfo *articleVenteInfo =
                        itArticleItemVenteInfo.value();

        if (articleVenteInfo->_quantite_a_vendre > 0)
        {
            tableWidget_articles->addArticleAfter(*articleVenteInfo);
        }
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

        enableImprimer();
    }
    else
    {
        disableImprimer();
    }
}

void YerithSortirWindow::afficher_sortie_detail(const int tableWidgetRow)
{
    _logger->log("afficher_sortie_detail",
                 QString("row: %1").arg(tableWidgetRow));

    if (tableWidgetRow < 0)
    {
        return;
    }

    QString rowStockID =
                    tableWidget_articles->get_DB_ELEMENT_db_ID(tableWidgetRow);

    QSqlQuery stockRecordQuery;

    QString
    stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'").arg
                        (YerithDatabase::STOCKS,
                         YerithDatabaseTableColumn::ID, rowStockID));
    QVariant img;

    QDate date_peremption;

    QString nom_entreprise_fournisseur;

    int querySize =
                    YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr, _logger);

    if (querySize > 0 && stockRecordQuery.next())
    {
        img = stockRecordQuery.value(YerithDatabaseTableColumn::IMAGE_PRODUIT);

        date_peremption =
                        stockRecordQuery.value(YerithDatabaseTableColumn::DATE_PEREMPTION).
                        toDate();

        nom_entreprise_fournisseur =
                        stockRecordQuery.
                        value(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).
                        toString();
    }

    YerithArticleVenteInfo *articleVenteInfo =
                    articleItemToVenteInfo.value(tableWidgetRow);

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
    lineEdit_article_detail_quantite_en_stock->setText(articleVenteInfo->
                                                       quantiteEnStock());
    lineEdit_article_detail_quantite_a_vendre->setText(articleVenteInfo->
                                                       quantiteAVendre());

    lineEdit_article_detail_AJOUT_COMMERCIAL->setText(articleVenteInfo->Get_ajout_pour_COMMERCIAL_text());

    lineEdit_article_detail_remise_prix->setText(articleVenteInfo->
                                                 remisePrix());
    lineEdit_article_detail_remise_pourcentage->setText(articleVenteInfo->
                                                        remisePourcentage());

    if (articleVenteInfo->_montant_tva > 0)
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

    lineEdit_article_detail_nom_magasinier->setText(_allWindows->getUser()->
                                                    nom_complet());
    lineEdit_article_detail_nom_recepteur->
    setText(lineEdit_articles_nom_recepteur->text());
    lineEdit_article_detail_destination->setText(comboBox_articles_transfert->
                                                 currentText());
    lineEdit_article_detail_nom_client->setText(comboBox_articles_nom_client->
                                                currentText());

    tabWidget_sorties->setCurrentIndex(AfficherSortieAuDetail);
}


void YerithSortirWindow::handle_VIEW_FONT_SIZE_change_buttons(bool CLEAR_AFTER_SELLING /* = false */)
{
	if (! CLEAR_AFTER_SELLING)
	{
		if (tableWidget_articles->itemCount() > 0)
		{
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);
		}
		else
		{
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
			YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);
		}
	}
	else
	{
		YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
		YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);
	}
}


void YerithSortirWindow::handleQteChange(QTableWidgetItem *itemChanged)
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
            YerithSqlTableModel *articleSqlTableModel = 0;

            if (_qteChangeCodeBar)
            {
                articleSqlTableModel =
                                lineEdit_recherche_article_codebar->getMySqlTableModel();
            }
            else
            {
                articleSqlTableModel =
                                lineEdit_recherche_article->getMySqlTableModel();
            }

            QString rowStockID =
                            tableWidget_articles->get_DB_ELEMENT_db_ID(itemChanged->row());

            QSqlQuery stockRecordQuery;

            QString
            stockRecordQueryStr(QString
                                ("SELECT * FROM %1 WHERE %2 = '%3'").arg
                                (YerithDatabase::STOCKS,
                                 YerithDatabaseTableColumn::ID, rowStockID));

            double montantTva = 0.0;
            double prixVente = 0.0;
            double qteEnStock = 0.0;

            QString stockReference;
            QString stockName;
            QString stockNomDepartementProduits;
            QString stockCategorie;

            int querySize =
                            YerithUtils::execQuery(stockRecordQuery, stockRecordQueryStr,
                                                   _logger);

            if (querySize > 0 && stockRecordQuery.next())
            {
                montantTva =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::MONTANT_TVA).toDouble();
                prixVente =
                                stockRecordQuery.
                                value(YerithDatabaseTableColumn::PRIX_VENTE).toDouble();
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
                stockCategorie =
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
                articleVenteInfo->_stockCategorie = stockCategorie;
                articleVenteInfo->_stockID = rowStockID;
                articleVenteInfo->_quantite_en_stock = qteEnStock;
                articleVenteInfo->_montant_tva = montantTva;
                articleVenteInfo->_prix_unitaire = prixVente - montantTva;
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
                                               ("pas assez d'articles en stock"),
                                               QObject::tr
                                               ("Il n'y a pas assez d'articles en stock !"));

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


bool YerithSortirWindow::article_exist(const QString codeBar,
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


void YerithSortirWindow::ajouter_article(const QString &text)
{
    _qteChangeCodeBar = false;

    QMap < QString, QString > stockNameToStockID =
                    lineEdit_recherche_article->getStockNameToStockID();

    if (!stockNameToStockID.contains(text))
    {
        return;
    }

    QString stockID = stockNameToStockID.value(text);

    QSqlQuery stockRecordQuery;

    QString
    stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'").arg
                        (YerithDatabase::STOCKS,
                         YerithDatabaseTableColumn::ID, stockID));

    double montantTva = 0.0;
    double serviceCost = 0.0;
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

        serviceCost =
                        stockRecordQuery.value(YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL).
                        toDouble();

        prixVente =
                        stockRecordQuery.value(YerithDatabaseTableColumn::PRIX_VENTE).
                        toDouble();
        qteEnStock =
                        stockRecordQuery.value(YerithDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        stockReference =
                        stockRecordQuery.value(YerithDatabaseTableColumn::REFERENCE).
                        toString();
        stockName =
                        stockRecordQuery.value(YerithDatabaseTableColumn::DESIGNATION).
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

    //Each call to YerithPointDeVenteTableWidget::setItem in method 'YerithPointDeVenteTableWidget::addArticle'
    //triggers a call to YerithSortirWindow::handleQteChange
    int lastCurRow = tableWidget_articles->addArticle(stockID,
                                                      stockReference,
                                                      stockName,
                                                      stockCategorie,
                                                      QString::number(prix_unitaire, 'f', 2),
                                                      QString::number(montantTva, 'f', 2),
													  QString::number(serviceCost, 'f', 2),
                                                      QString::number(prixVente, 'f', 2),
                                                      YerithPointDeVenteTableWidget::QTE_1,
                                                      QString::number(qteEnStock));

    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }

    setRechercheDesignationArticleFocus();
}


void YerithSortirWindow::ajouter_article_codebar(const QString &text)
{
    QMap < QString, QString > stockReferenceToStockID =
                    lineEdit_recherche_article_codebar->getStockreferenceCodebarToStockID();

    if (!stockReferenceToStockID.contains(text))
    {
        return;
    }

    QString stockID = stockReferenceToStockID.value(text);

    QSqlQuery stockRecordQuery;

    QString
    stockRecordQueryStr(QString("SELECT * FROM %1 WHERE %2 = '%3'").arg
                        (YerithDatabase::STOCKS,
                         YerithDatabaseTableColumn::ID, stockID));

    double montantTva = 0.0;
    double serviceCost = 0.0;
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

        serviceCost =
                        stockRecordQuery.value(YerithDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL).
                        toDouble();
        prixVente =
                        stockRecordQuery.value(YerithDatabaseTableColumn::PRIX_VENTE).
                        toDouble();
        qteEnStock =
                        stockRecordQuery.value(YerithDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        stockReference =
                        stockRecordQuery.value(YerithDatabaseTableColumn::REFERENCE).
                        toString();
        stockName =
                        stockRecordQuery.value(YerithDatabaseTableColumn::DESIGNATION).
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

    //Each call to YerithPointDeVenteTableWidget::setItem in method 'YerithPointDeVenteTableWidget::addArticle'
    //triggers a call to YerithSortirWindow::handleQteChange
    int lastCurRow = tableWidget_articles->addArticle(stockID,
                                                      stockReference,
                                                      stockName,
                                                      stockCategorie,
                                                      QString::number(prix_unitaire, 'f', 2),
                                                      QString::number(montantTva, 'f', 2),
													  QString::number(serviceCost, 'f', 2),
													  QString::number(prixVente, 'f', 2),
                                                      YerithPointDeVenteTableWidget::QTE_1,
                                                      QString::number(qteEnStock));
    if (lastCurRow > -1)
    {
        tableWidget_articles->selectRow(lastCurRow);
    }

    setRechercheCodebarArticleFocus();
}


/**
 * This method is called by 'YerithSortirWindow::handleQteChange'
 */
void YerithSortirWindow::actualiser_articles(int row, unsigned newItemQte)
{
    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
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
        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteVendue = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    update_lineedits_and_labels();
    tableWidget_articles->resizeColumnsToContents();
}

/**
  * This method is called by 'YerithSortirWindow::handleQteChange'
  */
void YerithSortirWindow::actualiser_articles_codebar(int row, unsigned newItemQte)
{
    _qteChangeCodeBar = true;

    double quantiteVendue = 0.0;
    double sommeTotal = 0.0;
    double tva = 0.0;
    double curTableWidgetItemQte = 1;

    QTableWidgetItem *curTableWidgetItem = 0;

    for (int k = 0; k < tableWidget_articles->rowCount(); ++k)
    {
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
        tva += (curTableWidgetItemQte * articleVenteInfo->_montant_tva);
        sommeTotal += articleVenteInfo->prix_vente();
    }

    _quantiteVendue = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    update_lineedits_and_labels();
    tableWidget_articles->resizeColumnsToContents();
}

void YerithSortirWindow::actualiser_tableau_sortie()
{
    int tableRowCount = tableWidget_articles->rowCount();

    _logger->log("actualiser_tableau_sortie",
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

    _quantiteVendue = quantiteVendue;
    _tva = tva;
    _sommeTotal = sommeTotal;

    update_lineedits_and_labels();

    tableWidget_articles->resizeColumnsToContents();
}


void YerithSortirWindow::retirer_un_article()
{
    _logger->log("enlever_article");

    int tableWidgetRow = tableWidget_articles->currentRow();

    _logger->log("enlever_article", QString("row: %1").arg(tableWidgetRow));

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

        actualiser_tableau_sortie();

        handle_VIEW_FONT_SIZE_change_buttons();

        setRechercheLineEditFocus();
    }
}

void YerithSortirWindow::sortir()
{
    if (tableWidget_articles->itemCount() <= 0)
    {
        YerithQMessageBox::warning(this, QObject::tr("stocks à sortir"),
                                   QObject::tr
                                   ("Vous n'avez pas entré de stocks à sortir!"));
        return;
    }

    if (!check_fields())
    {
        return;
    }

    bool sortieSucces = false;

    QString msgVente(QObject::tr("Poursuivre avec la sortie de "));
    msgVente.append(QString::number(_quantiteVendue));
    msgVente.append(QObject::tr(" articles ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("sortie"),
                                        msgVente,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

        int stocksSortieID = -1;

        int IDforReceipt =
                        YerithERPWindows::getNextIdSqlTableModel_stocks_sorties();

        QString
        referenceRecuSortie(YerithUtils::GET_REFERENCE_RECU_SORTIE
                            (QString::number(IDforReceipt)));

        QString quantiteQueryStr;

        QSqlQuery quantiteQuery;

        QDate datePeremtion;

        QString historiqueStock;

        QString localisation_stock;

        QString localisation;

        QString nomEntrepriseFournisseur;

        bool historique_stock_actif = true;

        bool isService = false;

        double quantite_totale_actuelle = 0.0;

        YerithArticleVenteInfo *articleVenteInfo = 0;

        for (int j = 0; j < tableWidget_articles->itemCount(); ++j)
        {
            articleVenteInfo = articleItemToVenteInfo.value(j);

            if (0 == articleVenteInfo)
            {
                continue;
            }

            quantiteQueryStr =
                            QString("SELECT * FROM %1 WHERE %2 = '%3'").
                            arg(YerithDatabase::STOCKS, YerithDatabaseTableColumn::ID,
                                articleVenteInfo->_stockID);

            int querySize =
                            YerithUtils::execQuery(quantiteQuery, quantiteQueryStr, _logger);

            if (querySize > 0 && quantiteQuery.next())
            {
                isService =
                                quantiteQuery.value(YerithDatabaseTableColumn::IS_SERVICE).
                                toBool();

                quantite_totale_actuelle =
                                quantiteQuery.
                                value(YerithDatabaseTableColumn::QUANTITE_TOTALE).
                                toDouble();

                datePeremtion =
                                quantiteQuery.
                                value(YerithDatabaseTableColumn::DATE_PEREMPTION).toDate();

                historique_stock_actif =
                                quantiteQuery.
                                value(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                                toBool();

                historiqueStock =
                                quantiteQuery.
                                value(YerithDatabaseTableColumn::HISTORIQUE_STOCK).
                                toString();

                localisation_stock =
                                quantiteQuery.
                                value(YerithDatabaseTableColumn::LOCALISATION_STOCK).
                                toString();

                localisation =
                                quantiteQuery.value(YerithDatabaseTableColumn::LOCALISATION).
                                toString();

                nomEntrepriseFournisseur =
                                quantiteQuery.
                                value(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR).
                                toString();
            }

            //qDebug() << QString("++ previousHistoriqueStock: %1")
            //                  .arg(historiqueStock);

            YerithSqlTableModel &stocksSortiesTableModel =
                            _allWindows->getSqlTableModel_stocks_sorties();

            QSqlRecord stocksSortiesRecord = stocksSortiesTableModel.record();

            stocksSortieID =
                            YerithERPWindows::getNextIdSqlTableModel_stocks_sorties();

            stocksSortiesRecord.setValue(YerithDatabaseTableColumn::ID,
                                         stocksSortieID);

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::REFERENCE_RECU_SORTIE,
                     referenceRecuSortie);

            stocksSortiesRecord.setValue(YerithDatabaseTableColumn::REFERENCE,
                                         articleVenteInfo->_stockReference);

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::DESIGNATION,
                     articleVenteInfo->_stockName);
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::DATE_PEREMPTION,
                     datePeremtion);

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                     articleVenteInfo->_stockNomDepartementProduits);

            stocksSortiesRecord.setValue(YerithDatabaseTableColumn::CATEGORIE,
                                         articleVenteInfo->_stockCategorie);
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::QUANTITE_SORTIE,
                     articleVenteInfo->_quantite_a_vendre);
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::PRIX_UNITAIRE,
                     articleVenteInfo->_prix_unitaire);
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                     articleVenteInfo->prix_vente());
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::MONTANT_TVA,
                     articleVenteInfo->montant_tva());
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::LOCALISATION_SORTIE,
                     localisation);
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::LOCALISATION_ENTREE,
                     comboBox_articles_transfert->currentText());
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::NOM_RECEPTEUR,
                     lineEdit_articles_nom_recepteur->text());

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                     comboBox_articles_nom_client->currentText());

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                     nomEntrepriseFournisseur);

            YerithPOSUser *user = _allWindows->getUser();

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::NOM_MAGASINIER,
                     user->nom_complet());
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER,
                     user->nom_utilisateur());
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::DATE_SORTIE,
                     GET_CURRENT_DATE);
            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::HEURE_SORTIE, CURRENT_TIME);
            stocksSortiesRecord.setValue(YerithDatabaseTableColumn::NOTES,
                                         textEdit_articles_notes->
                                         toPlainText());
            stocksSortiesRecord.setValue(YerithDatabaseTableColumn::IS_SERVICE,
                                         isService);
            stocksSortiesRecord.setValue(YerithDatabaseTableColumn::STOCKS_ID,
                                         articleVenteInfo->_stockID);

            double nouvelle_quantite_totale =
                            quantite_totale_actuelle - articleVenteInfo->_quantite_a_vendre;

            QString historiqueStockSortie_transfert;

            if (historique_stock_actif)
            {
                if (comboBox_articles_transfert->isEmpty())
                {
                    historiqueStockSortie_transfert.append
                    (YerithHistoriqueStock::creer_mouvement_stock
                     (MOUVEMENT_DE_STOCK_SORTIE, stocksSortieID,
                      GET_CURRENT_DATE, quantite_totale_actuelle,
                      articleVenteInfo->_quantite_a_vendre,
                      nouvelle_quantite_totale));
                }
                else
                {
                    historiqueStockSortie_transfert.append
                    (YerithHistoriqueStock::creer_mouvement_stock
                     (MOUVEMENT_DE_STOCK_TRANSFERT, stocksSortieID,
                      GET_CURRENT_DATE, quantite_totale_actuelle,
                      articleVenteInfo->_quantite_a_vendre,
                      nouvelle_quantite_totale));
                }
            }

            historiqueStock.append(YerithHistoriqueStock::SEPARATION_EXTERNE).
            append(historiqueStockSortie_transfert);

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                     historique_stock_actif);

            stocksSortiesRecord.
            setValue(YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                     historiqueStock);

            //qDebug() << QString("++ historiqueStock: %1")
            //                  .arg(historiqueStock);

            bool success1 = stocksSortiesTableModel
            					.insertNewRecord(stocksSortiesRecord,
                                                 this,
												 "src/windows/stocks/yerith-erp-sortir-window.cpp",
												 2788);

            if (success1)
            {
                if (nouvelle_quantite_totale < 0)
                {
                    nouvelle_quantite_totale = 0;
                }

                if (0 == nouvelle_quantite_totale)
                {
                    QString
                    removeRowQuery(QString
                                   ("DELETE FROM %1 WHERE %2 = '%3'").arg
                                   (YerithDatabase::STOCKS,
                                    YerithDatabaseTableColumn::ID,
                                    articleVenteInfo->_stockID));
                    YerithUtils::execQuery(removeRowQuery);
                }

                quantiteQueryStr.clear();
                quantiteQueryStr.append(QString
                                        ("UPDATE %1 SET %2 = '%3', %4 = '%5' WHERE %6 = '%7'").
                                        arg(YerithDatabase::STOCKS,
                                            YerithDatabaseTableColumn::QUANTITE_TOTALE,
                                            QString::number
                                            (nouvelle_quantite_totale),
                                            YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                                            historiqueStock,
                                            YerithDatabaseTableColumn::ID,
                                            articleVenteInfo->_stockID));

                bool success2 =
                                YerithUtils::execQuery(quantiteQueryStr, _logger);

                QString sMsg(QObject::tr("Le stock '%1' (%2 pièce(s))").
                             arg(articleVenteInfo->_stockName,
                                 articleVenteInfo->_quantite_a_vendre));

                if (success2)
                {
                    sMsg.append(QObject::tr
                                (" à été sorti avec succès."));
                }
                else
                {
                    sMsg.append(QObject::tr(" n'a pas pu être sorti !\n"
                                                "Contacter l'administrateur de %1").
                                arg(YerithUtils::APPLICATION_NAME));
                }

                sortieSucces = success1 && success2;

                _logger->log("sortir", sMsg);
            }
        }
        if (sortieSucces)
        {
            QString vMsg(QObject::tr
                         ("La sortie de '%1' articles a été effectuée avec succès !").
                         arg(QString::number(_quantiteVendue, 'f', 0)));

            if (QMessageBox::Ok ==
                    YerithQMessageBox::information(this,
                                                   QObject::tr("succès"),
                                                   vMsg))
            {
                imprimer_facture(referenceRecuSortie);
            }
        }

        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        handle_VIEW_FONT_SIZE_change_buttons_FROM__SELLING_signal();

        cleanUpAfterVente();
    }
    else
    {
    	handle_VIEW_FONT_SIZE_change_buttons_FROM__SELLING_signal();

        YerithQMessageBox::information(this, QObject::tr("annulation"),
                                       QObject::tr
                                       ("Vous avez annulé la sortie des articles !"),
                                       QMessageBox::Ok);
    }

    tableWidget_articles->resizeColumnsToContents();
}
