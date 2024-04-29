/*
 * yerith-erp-stocks-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-stocks-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/dialogs/yerith-erp-generic-select-db-field-dialog.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/QThread>

#include <QtCore/QProcess>

#include <QtCore/qmath.h>

#include <QtWidgets/QCheckBox>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <unistd.h>


/**
 * yerith-erp-windows.hpp cannot be included in
 * the header file because it will caused a circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */


const QString YerithStocksWindow::_STOCKS_TEXT_STRING[2] =
{
		"STOCKS",
		"STOCKS"
};

const QString YerithStocksWindow::_STOCKS_LABEL_TEXT_STRING[2] =
{
		"#stocks",
		"#stocks"
};

const QString YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[2] =
{
		"ASSET",
		"IMMOBILISATIONS"
};

const QString YerithStocksWindow::_IMMOBILISATION_LABEL_TEXT_STRING[2] =
{
		"# assets",
		"# immobilisations"
};


YerithStocksWindow::YerithStocksWindow()
:YerithWindowsCommons("yerith-erp-fichier-stocks"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::STOCKS),
 _show_ASSET(false),
 _logger(new YerithLogger("YerithStocksWindow")),
 _pushButton_stocks_filtrer_font(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("fiche des IMMOBILISATIONS OU DE stocks"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_stocks);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
        << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionModifier_ce_stock
        << actionAfficher_lhistorique_de_ce_stock
        << actionAfficher_stock_au_detail
        << actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_stocks);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0,
                     COLOUR_RGB_STRING_YERITH_GREEN_0_255_0);

    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT
            << YerithDatabaseTableColumn::IMAGE_PRODUIT
            << YerithDatabaseTableColumn::IS_SERVICE
            << YerithDatabaseTableColumn::HISTORIQUE_STOCK
            << YerithDatabaseTableColumn::LOTS_ENTRANT
            << YerithDatabaseTableColumn::QUANTITE_PAR_LOT;

    setup_select_configure_dbcolumn(YerithDatabase::STOCKS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_stock_terme_recherche,
                                           YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString.insert(YerithDatabaseTableColumn::REFERENCE);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerithDatabase::STOCKS);

    reinitialiser_colones_db_visibles();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    _pushButton_stocks_filtrer_font =
                    new QFont(pushButton_stocks_filtrer->font());

    tableView_stocks->setSqlTableName(&YerithDatabase::STOCKS);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEffacerLhistorique, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    pushButton_add_condition_SEARCH->disable(this);
    pushButton_stocks_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_achats_stocks->disable(this);
    pushButton_reinitialiser->disable(this);

    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionMarchandises, SIGNAL(triggered()), this,
            SLOT(afficherMarchandises()));
    connect(actionAchatsStocks, SIGNAL(triggered()), this,
            SLOT(achats_de_stocks()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));

    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));

    connect(actionAfficher_lhistorique_de_ce_stock, SIGNAL(triggered()),
            this, SLOT(private_slot_afficher_historique_du_stock()));

    connect(actionModifier_ce_stock, SIGNAL(triggered()),
            this, SLOT(modifier_les_articles()));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));

    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this,
            SLOT(reinitialiser_elements_filtrage()));
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

    connect(actionAfficher_stock_au_detail, SIGNAL(triggered()),
            this, SLOT(afficher_au_detail()));

    connect(actionEffacerLhistorique, SIGNAL(triggered()), this,
            SLOT(EFFACER_MANUELLEMENT_LHISTORIQUE()));

    connect(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES,
            SIGNAL(triggered()), this, SLOT(supprimer_ce_stock()));

    connect(action_BASCULER_STOCK_IMMOBILISATION,
            SIGNAL(triggered()),
            this,
			SLOT(handle_ACTION_CHOICE_IMMOBILISATION_STOCK()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(handle_filtres_checkBox(int)));


    connect(comboBox_CHOICE_IMMOBILISATION_STOCK,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_combobox_CHOICE_IMMOBILISATION_STOCK(const QString &)));

    connect(comboBox_strategie_de_stocks,
            SIGNAL(currentTextChanged(const QString &)),
			this,
            SLOT(gererChoixStrategieGestionDesStocks()));


    connect(tableView_stocks,
    		SIGNAL(doubleClicked(const QModelIndex &)),
            this,
			SLOT(afficher_au_detail(const QModelIndex &)));

    connect(actionAfficher_les_stocks_en_alerte_de_stock,
            SIGNAL(triggered()),
			this,
            SLOT(filtrer_stocks_en_alerte_de_stock()));

    connect(actionAfficher_les_stocks_perimes,
    		SIGNAL(triggered()),
            this,
			SLOT(filtrer_stocks_perimes_seulement()));


    setupShortcuts();
}


YerithStocksWindow::~YerithStocksWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithStocksWindow::initialiser_FILTRES()
{
    comboBox_stocks_element_de_stock->setVisible(false);
    comboBox_stocks_element_de_stock_condition->setVisible(false);
    lineEdit_stocks_element_de_stock_resultat->setVisible(false);
    pushButton_stocks_reinitialiser_filtre->setVisible(false);
    pushButton_stocks_filtrer->setVisible(false);
}


void YerithStocksWindow::SWITCH_QACTION_TEXTS()
{
    QAction *a_child_widget_QACTION = 0;

    QString old_qaction_text;

    QString NEW_qaction_text;

    for (uint k = 0; k < _ALL_QACTIONS.size(); ++k)
    {
        a_child_widget_QACTION = _ALL_QACTIONS.at(k);

        if (actionAUGMENTER_LA_POLICE_DU_TABLEAU == a_child_widget_QACTION ||
        	actiondiminuer_la_police_du_tableau == a_child_widget_QACTION)
        {
        	continue;
        }

        old_qaction_text = a_child_widget_QACTION->text();

//        QDEBUG_STRING_OUTPUT_2("old_qaction_text", old_qaction_text);

        if (_show_ASSET)
        {
        	if (YerithMainWindow::LANGUE_ANGLAISE)
        	{
                NEW_qaction_text = old_qaction_text.replace("stocks", "ASSETS");
                NEW_qaction_text = NEW_qaction_text.replace("stock", "ASSET");
//                QDEBUG_STRING_OUTPUT_2("NEW_qaction_text", NEW_qaction_text);
        	}
        	else
        	{
                NEW_qaction_text = old_qaction_text.replace("stocks", "IMMOBILISATIONS");
                NEW_qaction_text = NEW_qaction_text.replace("stock", "IMMOBILISATION");
        	}

            if (!NEW_qaction_text.trimmed().isEmpty())
            {
            	a_child_widget_QACTION->setText(NEW_qaction_text);
            }
        }
        else
        {
        	if (YerithMainWindow::LANGUE_ANGLAISE)
        	{
                NEW_qaction_text = old_qaction_text.replace("ASSETS", "stocks");
                NEW_qaction_text = NEW_qaction_text.replace("ASSET", "stock");
//                QDEBUG_STRING_OUTPUT_2("NEW_qaction_text", NEW_qaction_text);
        	}
        	else
        	{
                NEW_qaction_text = old_qaction_text.replace("IMMOBILISATIONS", "stocks");
                NEW_qaction_text = NEW_qaction_text.replace("IMMOBILISATION", "stock");
        	}

            if (!NEW_qaction_text.trimmed().isEmpty())
            {
            	a_child_widget_QACTION->setText(NEW_qaction_text);
            }
        }
    }


    old_qaction_text = action_BASCULER_STOCK_IMMOBILISATION->text();

    if (_show_ASSET)
    {
    	if (YerithMainWindow::LANGUE_ANGLAISE)
    	{
            NEW_qaction_text = old_qaction_text.replace("ASSETS", "stocks");
            NEW_qaction_text = NEW_qaction_text.replace("ASSET", "stock");
    	}
    	else
    	{
            NEW_qaction_text = old_qaction_text.replace("IMMOBILISATIONS", "stocks");
            NEW_qaction_text = NEW_qaction_text.replace("IMMOBILISATION", "stock");
    	}

        if (!NEW_qaction_text.trimmed().isEmpty())
        {
        	action_BASCULER_STOCK_IMMOBILISATION->setText(NEW_qaction_text);
        }
    }
    else
    {
    	if (YerithMainWindow::LANGUE_ANGLAISE)
    	{
    		NEW_qaction_text = old_qaction_text.replace("stocks", "ASSETS");
    		NEW_qaction_text = NEW_qaction_text.replace("stock", "ASSET");
    		//                QDEBUG_STRING_OUTPUT_2("NEW_qaction_text", NEW_qaction_text);
    	}
    	else
    	{
    		NEW_qaction_text = old_qaction_text.replace("stocks", "IMMOBILISATIONS");
    		NEW_qaction_text = NEW_qaction_text.replace("stock", "IMMOBILISATION");
    	}

    	if (!NEW_qaction_text.trimmed().isEmpty())
    	{
    		action_BASCULER_STOCK_IMMOBILISATION->setText(NEW_qaction_text);
    	}
    }
}


void YerithStocksWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_stocks_element_de_stock->setVisible(IS_VISIBLE);
    comboBox_stocks_element_de_stock_condition->setVisible(IS_VISIBLE);
    lineEdit_stocks_element_de_stock_resultat->setVisible(IS_VISIBLE);
    pushButton_stocks_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_stocks_filtrer->setVisible(IS_VISIBLE);
}


void YerithStocksWindow::handle_ACTION_CHOICE_IMMOBILISATION_STOCK()
{
    QString stock_ASSET_text(comboBox_CHOICE_IMMOBILISATION_STOCK->currentText());

    QString stock_asset_choice;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
	    if (YerithUtils::isEqualCaseInsensitive(stock_ASSET_text,
	                                            YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[ENGLISH]))
	    {
	    	stock_asset_choice = YerithStocksWindow::_STOCKS_TEXT_STRING[ENGLISH];
	    }
	    else
	    {
	    	stock_asset_choice = YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[ENGLISH];
	    }
	}
	else
	{
	    if (YerithUtils::isEqualCaseInsensitive(stock_ASSET_text,
	                                            YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[FRENCH]))
	    {
	    	stock_asset_choice = YerithStocksWindow::_STOCKS_TEXT_STRING[FRENCH];
	    }
	    else
	    {
	    	stock_asset_choice = YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[FRENCH];
	    }
	}

	handle_combobox_CHOICE_IMMOBILISATION_STOCK(stock_asset_choice);
}


void YerithStocksWindow::handle_combobox_CHOICE_IMMOBILISATION_STOCK(const QString &choice_immobilisation_stock)
{
	_CURRENT_choice_immobilisation_stock = choice_immobilisation_stock;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
	    if (YerithUtils::isEqualCaseInsensitive(_CURRENT_choice_immobilisation_stock,
	             	 	 	 	 	 	 	 	YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[ENGLISH]))
	    {
	        _show_ASSET = true;
	    }
	    else
	    {
	        _show_ASSET = false;
	    }
	}
	else
	{
	    if (YerithUtils::isEqualCaseInsensitive(_CURRENT_choice_immobilisation_stock,
	             	 	 	 	 	 	 	    YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[FRENCH]))
	    {
	        _show_ASSET = true;
	    }
	    else
	    {
	        _show_ASSET = false;
	    }
	}


//    QDEBUG_STRING_OUTPUT_2("YerithStocksWindow::handle_combobox_CHOICE_IMMOBILISATION_STOCK, _show_ASSET",
//    			           BOOL_TO_STRING(_show_ASSET));

    SWITCH_QACTION_TEXTS();

    textChangedSearchLineEditsQCompleters();
}


void YerithStocksWindow::private_slot_afficher_historique_du_stock()
{
    //qDebug() << QString("lastSelectedRow__ID: %1")
    //                              .arg(QString::number(lastSelectedRow__ID));

    if (0 != _curStocksTableModel && _curStocksTableModel->rowCount() > 0)
    {
        QSqlRecord record;

        _allWindows->_stocksWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

        QString historiqueStockSelectionne
					(GET_SQL_RECORD_DATA(record,
                                    	 YerithDatabaseTableColumn::HISTORIQUE_STOCK));


        QString stockReference
					(GET_SQL_RECORD_DATA(record,
										 YerithDatabaseTableColumn::REFERENCE));

        QString stockDesignation
					(GET_SQL_RECORD_DATA(record,
										 YerithDatabaseTableColumn::DESIGNATION));

//          qDebug() << QString("++ historiqueStockSelectionne: %1")
//                                      .arg(historiqueStockSelectionne);

        YerithHistoriqueStock::listHistoriqueDuStock(historiqueStockSelectionne,
        											 stockReference,
													 stockDesignation,
													 _show_ASSET);
    }
    else
    {
        if (!_show_ASSET)
        {
            YerithQMessageBox::information(this,
                                           QObject::tr("historique du stock"),
                                           QObject::tr("Il n'y a pas de stocks listés !"));
        }
        else
        {
            YerithQMessageBox::information(this,
                                           QObject::tr("historique de L'IMMOBILISATION"),
                                           QObject::tr("Il n'y a pas d'immobilisations listées !"));
        }
    }
}


bool YerithStocksWindow::filtrer_stocks_en_alerte_de_stock()
{
    QString filterString;

    filterString.append(QString("%1 <= %2").arg
                        (YerithDatabaseTableColumn::QUANTITE_TOTALE,
                         YerithDatabaseTableColumn::STOCK_DALERTE));

//      qDebug() << QString("++ filtrer_stocks_en_alerte_de_stock | filterString: %1")
//                                              .arg(filterString);

    _curStocksTableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curStocksTableModel->easySelect("src/windows/stocks/yerith-erp-stocks-window.cpp", 609);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);
    }

    lister_les_elements_du_tableau();

    if (resultRows > 0)
    {
        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "stocks en état d'alerte de stocks");
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this,
                                                  "stocks en état d'alerte de stocks");
    }

    set_filtrer_font();

    return true;
}


bool YerithStocksWindow::filtrer_stocks_perimes_seulement()
{
    QString filterString;

    filterString.append(QString("%1 < '%2'").arg
                        (YerithDatabaseTableColumn::DATE_PEREMPTION,
                         DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE)));

    //qDebug() << QString("++ filtrer_stocks_perimes_seulement | filterString: %1")
    //                                          .arg(filterString);

    reinitialiser_recherche();

    _curStocksTableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curStocksTableModel->easySelect("src/windows/stocks/yerith-erp-stocks-window.cpp", 650);

    if (resultRows > 0)
    {
        setCurrentlyFiltered(true);
    }

    lister_les_elements_du_tableau();

    if (resultRows > 0)
    {
        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "stocks périmés");
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "stocks périmés");
    }

    set_filtrer_font();

    return true;
}


bool YerithStocksWindow::filtrer_stocks()
{
    QString
    stockTableColumnValue(lineEdit_stocks_element_de_stock_resultat->text());

    if (stockTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr("Veuillez saisir une valeur numérique pour la recherche !"));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;
    }

    reinitialiser_recherche();

    QString stockTableColumnProperty(comboBox_stocks_element_de_stock->currentText());

    QString mathOperator(comboBox_stocks_element_de_stock_condition->currentText());

    QString REAL_DB_ID_NAME_stockTableColumnProperty
    			(YerithDatabaseTableColumn::_tableColumnToUserViewString.key(stockTableColumnProperty));

    QString filterString;

    if (!_show_ASSET)
    {
        filterString.append(QString("%1 %2 %3 AND (%4 != 0)")
        						.arg(REAL_DB_ID_NAME_stockTableColumnProperty,
        							 mathOperator,
									 stockTableColumnValue,
									 YerithDatabaseTableColumn::PRIX_VENTE));
    }
    else
    {
        filterString.append(QString("(%1 %2 %3) AND (%4 = 0)")
        						.arg(REAL_DB_ID_NAME_stockTableColumnProperty,
        						     mathOperator,
									 stockTableColumnValue,
									 YerithDatabaseTableColumn::PRIX_VENTE));
    }

    //QDEBUG_STRING_OUTPUT_2("YerithStocksWindow::filtrer_stocks(), filterString", filterString);

    _curStocksTableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curStocksTableModel->easySelect("src/windows/stocks/yerith-erp-stocks-window.cpp", 722);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        lister_les_elements_du_tableau();

        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "stocks - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "stocks - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerithStocksWindow::resetFilter()
{
    if (0 != _curStocksTableModel)
    {
        _curStocksTableModel->resetFilter();
    }

    lineEdit_stock_terme_recherche->myClear();

    dateEdit_stocks_debut->reset();
    dateEdit_stocks_fin->reset();
}


void YerithStocksWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    lister_les_elements_du_tableau();
}


void YerithStocksWindow::activerComboBoxStrategieDeGestionDesStocks()
{
    comboBox_strategie_de_stocks->setVisible(true);
    comboBox_strategie_de_stocks->setYerithEnabled(true);
    lineEdit_stock_terme_recherche->setFixedWidth(656);
}


void YerithStocksWindow::desactiverComboBoxStrategieDeGestionDesStocks()
{
    comboBox_strategie_de_stocks->setVisible(false);
    comboBox_strategie_de_stocks->setYerithEnabled(false);
    lineEdit_stock_terme_recherche->setFixedWidth(778);
}


void YerithStocksWindow::disableValeurDinventaire()
{
    lineEdit_stocks_valeur_totale_dinventaire->setVisible(false);
    label_stocks_valeur_totale->setVisible(false);
}


void YerithStocksWindow::enableValeurDinventaire_ONLY_MANAGER()
{
    lineEdit_stocks_valeur_totale_dinventaire->setVisible(true);
    label_stocks_valeur_totale->setVisible(true);
}


void YerithStocksWindow::populateComboBoxes_TYPE_DINVENTAIRE()
{
	QStringList aQStringList;

	if (_show_ASSET)
	{
		if (YerithMainWindow::LANGUE_ANGLAISE)
		{
			aQStringList.append(_STOCKS_TEXT_STRING[ENGLISH]);

			aQStringList.append(_IMMOBILISATION_TEXT_STRING[ENGLISH]);

			_CURRENT_choice_immobilisation_stock = _IMMOBILISATION_TEXT_STRING[ENGLISH];
		}
	    else
	    {
	        aQStringList.append(_STOCKS_TEXT_STRING[FRENCH]);

	        aQStringList.append(_IMMOBILISATION_TEXT_STRING[FRENCH]);

	        _CURRENT_choice_immobilisation_stock = _IMMOBILISATION_TEXT_STRING[FRENCH];
	    }
	}
	else
	{
		if (YerithMainWindow::LANGUE_ANGLAISE)
		{
			aQStringList.append(_STOCKS_TEXT_STRING[ENGLISH]);

			aQStringList.append(_IMMOBILISATION_TEXT_STRING[ENGLISH]);

			_CURRENT_choice_immobilisation_stock = _STOCKS_TEXT_STRING[ENGLISH];
		}
	    else
	    {
	        aQStringList.append(_STOCKS_TEXT_STRING[FRENCH]);

	        aQStringList.append(_IMMOBILISATION_TEXT_STRING[FRENCH]);

	        _CURRENT_choice_immobilisation_stock = _STOCKS_TEXT_STRING[FRENCH];
	    }
	}


	disconnect(comboBox_CHOICE_IMMOBILISATION_STOCK, 0, 0, 0);


	comboBox_CHOICE_IMMOBILISATION_STOCK->clear();

    comboBox_CHOICE_IMMOBILISATION_STOCK->removeItem(0);

    comboBox_CHOICE_IMMOBILISATION_STOCK->addItems(aQStringList);


    comboBox_CHOICE_IMMOBILISATION_STOCK
		->find_AND_SET_CURRENT_INDEX(_CURRENT_choice_immobilisation_stock);


    connect(comboBox_CHOICE_IMMOBILISATION_STOCK,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_combobox_CHOICE_IMMOBILISATION_STOCK(const QString &)));
}


void YerithStocksWindow::populateComboBoxes()
{
    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    YerithUtils::REMOVE_QSTRING_LIST(aQStringList,
                                     _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME);

    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_PRODUIT);
    aQStringList.removeAll(YerithDatabaseTableColumn::ENREGISTREUR_STOCK);
    aQStringList.removeAll(YerithDatabaseTableColumn::HISTORIQUE_STOCK);
    aQStringList.removeAll(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);

//      qDebug() << "++ test: " << aQStringList;

    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::_tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerithDatabaseTableColumn::MONTANT_TVA));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerithDatabaseTableColumn::PRIX_VENTE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerithDatabaseTableColumn::PRIX_UNITAIRE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(YerithDatabaseTableColumn::QUANTITE_TOTALE));


    comboBox_stocks_element_de_stock->clear();

    comboBox_stocks_element_de_stock->addItems(aQStringList);



    comboBox_stocks_element_de_stock_condition->clear();

    comboBox_stocks_element_de_stock_condition
        ->addItems(_ALL_WINDOWS_FILTER_NUMERIC_CONDITIONS);



    comboBox_strategie_de_stocks->clear();

    comboBox_strategie_de_stocks->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

    //index 0 of enum enum_comboBoxStrategyIndex
    comboBox_strategie_de_stocks->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);
    comboBox_strategie_de_stocks->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
    comboBox_strategie_de_stocks->addItem(YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    desactiverComboBoxStrategieDeGestionDesStocks();

    aQStringList.clear();

    populateComboBoxes_TYPE_DINVENTAIRE();
}


void YerithStocksWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_stocks_nombre_de_lignes_par_page;

    lineEdit_stock_terme_recherche->enableForSearch(QObject::tr
                                                    ("terme à rechercher (description de l'article (ou service))"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_stocks_element_de_stock_resultat->setValidator(&YerithUtils::POSITIVE_DoubleValidator);

    lineEdit_stocks_nombre_darticles->setYerithEnabled(false);
    lineEdit_stocks_valeur_totale_dinventaire->setYerithEnabled(false);
    lineEdit_nombre_de_stocks_IMMOBILISATIONS->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_stocks_nombre_de_lignes_par_page,
    									tableView_stocks);
}


void YerithStocksWindow::setupDateTimeEdits()
{
    dateEdit_stocks_debut->setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_stocks_fin->setStartDate(GET_CURRENT_DATE);

    _stocksDateEntreeFilter.clear();

    _stocksDateEntreeFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
                                   	 .arg(YerithDatabaseTableColumn::DATE_ENTREE,
                                       DATE_TO_DB_FORMAT_STRING(dateEdit_stocks_debut->date()),
                                       YerithDatabaseTableColumn::DATE_ENTREE,
                                       DATE_TO_DB_FORMAT_STRING(dateEdit_stocks_fin->date())));

    connect(dateEdit_stocks_debut,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerithLineEdits()));

    connect(dateEdit_stocks_fin,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerithLineEdits()));
}


void YerithStocksWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionAfficher_lhistorique_de_ce_stock->setShortcut(Qt::Key_F2);
    actionReinitialiserRecherche->setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithStocksWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_stocks_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_stock_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("%1")
            						.arg(GENERATE_SQL_LIKE_STMT(YerithDatabaseTableColumn::DESCRIPTION_PRODUIT,
                                                    			partSearchTerm)));

            if (k != lastIdx)
            {
                _searchFilter.append(" AND ");
            }
        }
    }


    {
        bool GENERATE_Like_SQL_STMT = false;

        if (!checkBox_EXACT_STRING_MATCH->isChecked())
        {
            GENERATE_Like_SQL_STMT = true;
        }

        YerithWindowsCommons::setYerithLineEditQCompleterSearchFilter(_searchFilter,
                                                                      GENERATE_Like_SQL_STMT);
    }


    YerithLineEdit *aYerithLineEdit = 0;

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    QMapIterator<YerithLineEdit **, QString> it(_lineEditsToANDContentForSearch);

    while (it.hasNext())
    {
        it.next();

        aYerithLineEdit = *it.key();

        correspondingDBFieldKeyValue = it.value();

        if (0 != aYerithLineEdit)
        {
            aTableColumnFieldContentForANDSearch = aYerithLineEdit->text();

            if (!correspondingDBFieldKeyValue.isEmpty() 	&&
                !aTableColumnFieldContentForANDSearch.isEmpty())
            {
                if (!_searchFilter.isEmpty())
                {
                    _searchFilter.append(" AND ");
                }

                _searchFilter.append(GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
                                      	  	  	  	  	  aTableColumnFieldContentForANDSearch));
            }
        }
    }

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER

    QString CURRENT_ASSET_LANGUAGE_TEXT;

    if (_show_ASSET)
    {
    	if (YerithMainWindow::LANGUE_ANGLAISE)
    	{
            CURRENT_ASSET_LANGUAGE_TEXT =
            		YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[ENGLISH];
    	}
    	else
    	{
            CURRENT_ASSET_LANGUAGE_TEXT =
            		YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[FRENCH];
    	}
    }
    else
    {
    	if (YerithMainWindow::LANGUE_ANGLAISE)
    	{
            CURRENT_ASSET_LANGUAGE_TEXT =
            		YerithStocksWindow::_STOCKS_TEXT_STRING[ENGLISH];
    	}
    	else
    	{
            CURRENT_ASSET_LANGUAGE_TEXT =
            		YerithStocksWindow::_STOCKS_TEXT_STRING[FRENCH];
    	}
    }


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        if (YerithUtils::isEqualCaseInsensitive(CURRENT_ASSET_LANGUAGE_TEXT,
        		YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[ENGLISH]))
        {
            if (_searchFilter.isEmpty())
            {
                _searchFilter.append(QString(" %1 <= 0")
                						.arg(YerithDatabaseTableColumn::PRIX_VENTE));
            }
            else
            {
                _searchFilter.append(QString(" AND %1 <= 0")
                						.arg(YerithDatabaseTableColumn::PRIX_VENTE));
            }

            label_nombre_de_IMMOBILISATIONS
				->setText(YerithStocksWindow::_IMMOBILISATION_LABEL_TEXT_STRING[ENGLISH]);

            label_nombre_de_stocks->setVisible(false);

            label_nombre_de_IMMOBILISATIONS->setVisible(true);
        }
        else
        {
            if (_searchFilter.isEmpty())
            {
                _searchFilter.append(QString(" %1 > 0").arg
                                     (YerithDatabaseTableColumn::PRIX_VENTE));
            }
            else
            {
                _searchFilter.append(QString(" AND %1 > 0").arg
                                     (YerithDatabaseTableColumn::PRIX_VENTE));
            }

            label_nombre_de_stocks->setVisible(true);

            label_nombre_de_IMMOBILISATIONS->setVisible(false);

            label_nombre_de_stocks->setText(YerithStocksWindow::_STOCKS_LABEL_TEXT_STRING[ENGLISH]);
        }
    }
    else
    {
        if (YerithUtils::isEqualCaseInsensitive(CURRENT_ASSET_LANGUAGE_TEXT,
        		YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[FRENCH]))
        {
            if (_searchFilter.isEmpty())
            {
                _searchFilter.append(QString(" %1 <= 0")
                						.arg(YerithDatabaseTableColumn::PRIX_VENTE));
            }
            else
            {
                _searchFilter.append(QString(" AND %1 <= 0")
                						.arg(YerithDatabaseTableColumn::PRIX_VENTE));
            }

            label_nombre_de_IMMOBILISATIONS
				->setText(YerithStocksWindow::_IMMOBILISATION_LABEL_TEXT_STRING[FRENCH]);

            label_nombre_de_stocks->setVisible(false);

            label_nombre_de_IMMOBILISATIONS->setVisible(true);
        }
        else
        {
            if (_searchFilter.isEmpty())
            {
                _searchFilter.append(QString(" %1 > 0").arg
                                     (YerithDatabaseTableColumn::PRIX_VENTE));
            }
            else
            {
                _searchFilter.append(QString(" AND %1 > 0").arg
                                     (YerithDatabaseTableColumn::PRIX_VENTE));
            }

            label_nombre_de_stocks->setVisible(true);

            label_nombre_de_IMMOBILISATIONS->setVisible(false);

            label_nombre_de_stocks->setText(YerithStocksWindow::_STOCKS_LABEL_TEXT_STRING[FRENCH]);
        }
    }


//    QDEBUG_STRING_OUTPUT_2("_searchFilter",
//                            _searchFilter);


    QString finalSearchFilter(_stocksDateEntreeFilter);

    if (!_searchFilter.isEmpty())
    {
        QString searchFilterWithDate(QString("%1 AND (%2)")
        								.arg(_stocksDateEntreeFilter,
        									 _searchFilter));

        finalSearchFilter = searchFilterWithDate;
    }

//    QDEBUG_STRING_OUTPUT_2("finalSearchFilter", finalSearchFilter);

    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(finalSearchFilter);


    if (_yerithSqlTableModel->select())
    {
        comboBox_CHOICE_IMMOBILISATION_STOCK
			->find_AND_SET_CURRENT_INDEX(_CURRENT_choice_immobilisation_stock);

        lister_les_elements_du_tableau(*_yerithSqlTableModel, _localStrategy);
    }
    else
    {
        qDebug() << QString("++ YerithStocksWindow::textChangedSearchLineEditsQCompleters(): %1")
                 	 .arg(_yerithSqlTableModel->lastError().text());
    }
}


void YerithStocksWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::CATEGORIE
            << YerithDatabaseTableColumn::PRIX_VENTE
            << YerithDatabaseTableColumn::QUANTITE_TOTALE
            << YerithDatabaseTableColumn::LOCALISATION_STOCK
            << YerithDatabaseTableColumn::DATE_PEREMPTION;
}


void YerithStocksWindow::contextMenuEvent(QContextMenuEvent *event)
{
    YerithPOSUser *aUser = _allWindows->getUser();
    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.addAction(actionAfficher_lhistorique_de_ce_stock);
                menu.addAction(actionEffacerLhistorique);
                menu.addAction(actionModifier_ce_stock);
                menu.addAction(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES);
                menu.exec(event->globalPos());
            }
        }
        if (aUser->isVendeur())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.exec(event->globalPos());
            }
        }
        else if (aUser->isGestionaireDesStocks())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.addAction(actionAfficher_lhistorique_de_ce_stock);
                menu.addAction(actionModifier_ce_stock);
                menu.addAction(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES);
                menu.exec(event->globalPos());
            }
        }
        else if (aUser->isMagasinier())
        {
            if (tableView_stocks->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_stocksWindow->palette());
                menu.addAction(actionAfficher_stock_au_detail);
                menu.exec(event->globalPos());
            }
        }
    }
}


void YerithStocksWindow::hideEvent(QHideEvent *hideEvent)
{
    YerithWindowsCommons::CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();

    _selectExportDBQDialog->close();

    _allWindows->_historiqueDuStockWindow->close();
}


void YerithStocksWindow::rendreVisible(YerithSqlTableModel *stocksTableModel,
                                       bool 				show_ASSET)
{
	retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    _show_ASSET = show_ASSET;

//    QDEBUG_STRING_OUTPUT_2("_show_ASSET", BOOL_TO_STRING(_show_ASSET));

    static bool QACTION_first_execution = true;

    QAction *a_child_widget_QACTION = 0;

    if (QACTION_first_execution)
    {
        QList<QObject *> child_objects = children();

        QObject *a_child_object = 0;

        for (uint k = 0; k < child_objects.size(); ++k)
        {
            a_child_object = child_objects.at(k);

            if (0 == a_child_object)
            {
                continue;
            }

            a_child_widget_QACTION = dynamic_cast<QAction *>(a_child_object);

            if (0 != a_child_widget_QACTION)
            {
                _ALL_QACTIONS.append(a_child_widget_QACTION);
            }
        }

        QACTION_first_execution = false;
    }


    setYerithSqlTableModel(stocksTableModel);

    _curStocksTableModel = stocksTableModel;

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    SWITCH_QACTION_TEXTS();

    lineEdit_stock_terme_recherche->setFocus();


    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enableValeurDinventaire_ONLY_MANAGER();
        }
        else
        {
            disableValeurDinventaire();
        }
    }
    else
    {
        disableValeurDinventaire();
    }


    static bool first_execution = true;

    if (first_execution)
    {
        setComboBoxStrategieDeStocks();
        first_execution = false;
    }

    setVisible(true);

    afficher_stocks();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("fiche des IMMOBILISATIONS OU DE stocks"),
    								 _curStocksTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerithStocksWindow::imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()
{
    if (0 != _yerith_PRINT_UTILITIES_TEX_TABLE)
    {
        _yerith_PRINT_UTILITIES_TEX_TABLE->RESET_NOMBRE_DE_LIGNES_TABLEAU_STANDARD();
    }

    YerithProgressBar((YerithWindowsCommons *) this, QString("rgb(%1)").arg(COLOUR_RGB_STRING_YERITH_GREEN_0_255_0))
                     ((YerithWindowsCommons *) this,
                      &YerithWindowsCommons::imprimer_pdf_document);
}


void YerithStocksWindow::gererChoixStrategieGestionDesStocks()
{
    _localStrategy = comboBox_strategie_de_stocks->currentText();

    lister_les_elements_du_tableau(_localStrategy);
}


void YerithStocksWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_BASCULER_STOCK_IMMOBILISATION, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_stock_au_detail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEffacerLhistorique, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    activerComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_marchandises_SERVICES->enable(this,
                                             SLOT(afficherMarchandises()));
    pushButton_achats_stocks->enable(this, SLOT(achats_de_stocks()));
}


void YerithStocksWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_BASCULER_STOCK_IMMOBILISATION, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_stock_au_detail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEffacerLhistorique, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    activerComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksTableModel)

    pushButton_add_condition_SEARCH->enable(this, SLOT(AJOUTER_CONDITION_DE_RECHERCHE()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_achats_stocks->enable(this, SLOT(achats_de_stocks()));
}


void YerithStocksWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_BASCULER_STOCK_IMMOBILISATION, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_stock_au_detail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEffacerLhistorique, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    activerComboBoxStrategieDeGestionDesStocks();

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksTableModel)

    pushButton_add_condition_SEARCH->enable(this, SLOT(AJOUTER_CONDITION_DE_RECHERCHE()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));
    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_achats_stocks->enable(this, SLOT(achats_de_stocks()));
}

void YerithStocksWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_BASCULER_STOCK_IMMOBILISATION, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_stock_au_detail, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEffacerLhistorique, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    desactiverComboBoxStrategieDeGestionDesStocks();


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksTableModel)


    pushButton_add_condition_SEARCH->enable(this, SLOT(AJOUTER_CONDITION_DE_RECHERCHE()));

    pushButton_stocks_filtrer->enable(this, SLOT(filtrer_stocks()));

    pushButton_stocks_reinitialiser_filtre->enable(this, SLOT(reinitialiser_elements_filtrage()));
    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_achats_stocks->disable(this);
}

void YerithStocksWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_BASCULER_STOCK_IMMOBILISATION, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

	YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_stock_au_detail, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_lhistorique_de_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEffacerLhistorique, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier_ce_stock, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_ce_TOUS_LES_stockS_SELECTIONES, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    desactiverComboBoxStrategieDeGestionDesStocks();


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_add_condition_SEARCH->disable(this);
    pushButton_reinitialiser->disable(this);
    pushButton_stocks_filtrer->disable(this);
    pushButton_stocks_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_achats_stocks->disable(this);
    pushButton_menu_principal->disable(this);
}


void YerithStocksWindow::afficher_au_detail()
{
    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1 &&
        _curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_detailWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("détails d'un stock"),
                                       QObject::tr("Sélectionnez un stock à afficher les détails."));

        return;
    }
}


void YerithStocksWindow::afficher_au_detail(const QModelIndex &modelIndex)
{
    if (_curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_detailWindow->rendreVisible(_curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr("détails d'un stock"),
                                   QObject::tr("Sélectionnez un stock à afficher les détails."));
    }
}


void YerithStocksWindow::EFFACER_MANUELLEMENT_LHISTORIQUE()
{
    QSqlRecord record;

    SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    bool is_service = GET_SQL_RECORD_DATA(record,
                                          YerithDatabaseTableColumn::IS_SERVICE).
                      toInt();

    if (is_service)
    {
        return;
    }

    QString record_ID =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID);

    QString current_stock_name =
                    GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DESIGNATION);

    QString msgSupprimer =
                    QObject::tr("EFFACER MANUELLEMENT L'HISTORIQUE du stock '%1' AVEC ID: '%2' ?")
							  .arg(current_stock_name,
								   record_ID);


    if (QMessageBox::Cancel ==
            YerithQMessageBox::question(this,
                                        QObject::tr("EFFACER MANUELLEMENT L'HISTORIQUE"),
                                        			msgSupprimer,
													QMessageBox::Cancel,
													QMessageBox::Ok))
    {
        return;
    }
    else
    {
    }

    QString STOCK_SQL_STRING_QUERY =
                    QString("UPDATE %1 SET %2='' WHERE %3='%4'")
						.arg(YerithDatabase::STOCKS,
                             YerithDatabaseTableColumn::HISTORIQUE_STOCK,
                             YerithDatabaseTableColumn::ID,
                             record_ID);

    bool res_effacer_query = false;

    res_effacer_query = YerithUtils::execQuery(STOCK_SQL_STRING_QUERY);

    if (res_effacer_query)
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("EFFACER MANUELLEMENT L'HISTORIQUE"),
                                       QObject::tr
                                       ("L'HISTORIQUE du stock '%1' AVEC ID: '%2' A ÉTÉ EFFACÉ !").
                                       arg(current_stock_name, record_ID));
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("EFFACER MANUELLEMENT L'HISTORIQUE"),
                                   QObject::tr
                                   ("L'HISTORIQUE du stock '%1' AVEC ID: '%2' N'A PU ÊTRE EFFACÉ !").
                                   arg(current_stock_name, record_ID));
    }
}


/**
 * La suppression d'un stock entraine automatiquement la
 * suppression de l'achat correspondant
 */
void YerithStocksWindow::supprimer_PLUSIEURS_Stocks(YerithSqlTableModel &
                                                    aStocksTableModel)
{
    QString msgSupprimer =
                    QObject::tr("SUPPRIMER TOUS LES STOCKS "
                                    "SÉLECTIONÉS (sans confirmation de suppression) ?");


    if (QMessageBox::Cancel ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression de plusieurs"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("suppression de plusieurs"),
                                       QObject::tr
                                       ("ANNULATION DES SUPPRESSIONS DES STOCKS SÉLECTIONÉS !"));

        return;
    }
    else
    {
    }

    QMapIterator<QString, QString > j(tableView_stocks->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();

        supprimer_ce_stock_DE_PLUSIEURS(j.value());
    }

    setupLineEditsQCompleters((QObject *) this);

    lister_les_elements_du_tableau(*_curStocksTableModel);

    tableView_stocks->clearSelection();
}


/**
 * La suppression d'un stock entraine automatiquement la
 * suppression de l'achat correspondant.
 *
 * ICI NOUS SUPPRIMONS 1 STOCK SANS confirmation de SUPPRESSION.
 */
void YerithStocksWindow::supprimer_ce_stock_DE_PLUSIEURS(QString aStockID)
{
    QSqlRecord record;

    static QSqlQuery query;

    query.clear();

    QString
    QUERY_STOCK_DATA(QString("select * from %1 where %2='%3'").arg
                     (YerithDatabase::STOCKS, YerithDatabaseTableColumn::ID,
                      aStockID));

    int querySize = YerithUtils::execQuery(query, QUERY_STOCK_DATA);

    if (query.next())
    {
        record = query.record();
    }

    bool resRemoved = false;

    QString DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING
    (QString("DELETE FROM %1 WHERE %2 = '%3'").arg(YerithDatabase::STOCKS,
                                                   YerithDatabaseTableColumn::
                                                   ID, aStockID));

    resRemoved =
                    YerithUtils::execQuery
                    (DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

//              QDEBUG_STRING_OUTPUT_2(QString("resRemoved: %1, stocksID: %2")
//                                                                      .arg(BOOL_TO_STRING(resRemoved),
//                                                                               aStockID),
//                                                                                       DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

    if (resRemoved && _curStocksTableModel->select())
    {
        bool is_service = GET_SQL_RECORD_DATA(record,
                                              YerithDatabaseTableColumn::IS_SERVICE).
                          toInt();

        if (!is_service)
        {
            QString deleteAchatsRowQueryStr;

            deleteAchatsRowQueryStr.append
            (QString("DELETE FROM %1 WHERE %2='%3'").arg
             (YerithDatabase::ACHATS, YerithDatabaseTableColumn::STOCKS_ID,
              aStockID));

            bool deletedBuyings =
                            YerithUtils::execQuery(deleteAchatsRowQueryStr);
        }
    }
}


/**
 * La suppression d'un stock entraine automatiquement la
 * suppression de l'achat correspondant
 */
void YerithStocksWindow::supprimer_ce_stock(QString aStockID
                                            /* = YerithUtils::EMPTY_STRING */,
                                            bool _reEntrant /* = false */)
{
    if (getLastListerSelectedRow__ID_AS_INTEGER() <= -1 ||
            _curStocksTableModel->rowCount() <= 0)
    {
        YerithQMessageBox::information(this, QObject::tr("supprimer"),
                                       QObject::tr
                                       ("Sélectionnez 1 stock à supprimer."));

        return;
    }

    if (!_reEntrant && tableView_stocks->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_Stocks(*_curStocksTableModel);

        return;
    }

    QSqlRecord record;

    if (!aStockID.isEmpty())
    {
        static QSqlQuery query;

        query.clear();

        QString
        QUERY_STOCK_DATA(QString("select * from %1 where %2='%3'").arg
                         (YerithDatabase::STOCKS,
                          YerithDatabaseTableColumn::ID, aStockID));

        int querySize = YerithUtils::execQuery(query, QUERY_STOCK_DATA);

        if (query.next())
        {
            record = query.record();
        }
    }
    else
    {
        _allWindows->
        _stocksWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);
    }

    QString msgSupprimer;

    QString serviceOuArticle;

    bool is_service = GET_SQL_RECORD_DATA(record,
                                          YerithDatabaseTableColumn::IS_SERVICE).
                      toInt();

    if (is_service)
    {
        serviceOuArticle =
                        GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::REFERENCE);
        msgSupprimer.append(QObject::tr
                            ("Poursuivre avec la suppression du service '%1' ?").
                            arg(serviceOuArticle));
    }
    else
    {
        serviceOuArticle =
                        GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::DESIGNATION);

        msgSupprimer.append(QObject::tr
                            ("Poursuivre avec la suppression du stock '%1' ?").
                            arg(serviceOuArticle));
    }

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression d'un stock (service)"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool resRemoved = false;

        if (!aStockID.isEmpty())
        {
            QString DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING
            (QString("DELETE FROM %1 WHERE %2 = '%3'").arg
             (YerithDatabase::STOCKS, YerithDatabaseTableColumn::ID,
              aStockID));

            resRemoved =
                            YerithUtils::execQuery
                            (DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

//              QDEBUG_STRING_OUTPUT_2(QString("resRemoved: %1, stocksID: %2")
//                                                                      .arg(BOOL_TO_STRING(resRemoved),
//                                                                               aStockID),
//                                                                                       DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);
        }
        else
        {
            resRemoved =
                            _allWindows->
                            _stocksWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW();
        }

        //qDebug() << "YerithStocksWindow::supprimer_ce_stock() " << resRemoved;

        if (resRemoved && _curStocksTableModel->select())
        {
            if (!is_service)
            {
                QString deleteAchatsRowQueryStr;

                if (!aStockID.isEmpty())
                {
                    deleteAchatsRowQueryStr.append
                    (QString("DELETE FROM %1 WHERE %2='%3'").arg
                     (YerithDatabase::ACHATS,
                      YerithDatabaseTableColumn::STOCKS_ID, aStockID));
                }
                else
                {
                    deleteAchatsRowQueryStr.append
                    (QString("DELETE FROM %1 WHERE %2='%3'").arg
                     (YerithDatabase::ACHATS,
                      YerithDatabaseTableColumn::STOCKS_ID,
                      GET_SQL_RECORD_DATA(record,
                                          YerithDatabaseTableColumn::ID)));
                }

                bool deletedBuyings =
                                YerithUtils::execQuery(deleteAchatsRowQueryStr);

//                      if (!aStockID.isEmpty())
//                      {
//                      QDEBUG_STRING_OUTPUT_2(QString("deletedBuyings: %1, stocksID: %2")
//                                                                              .arg(BOOL_TO_STRING(deletedBuyings),
//                                                                                       aStockID),
//                                                                                               deleteAchatsRowQueryStr);
//                      }
//                      else
//                      {
//                      QDEBUG_STRING_OUTPUT_2(QString("deletedBuyings: %1, stocksID: %2")
//                                                                              .arg(BOOL_TO_STRING(deletedBuyings),
//                                                                                       GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::ID)),
//                                                                                               deleteAchatsRowQueryStr);
//                      }
            }

            msgSupprimer =
                            QObject::tr("Le stock (service) '%1' a été supprimé !").
                            arg(serviceOuArticle);

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression d'un stock (service)"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("Le stock (service) '%1' ne pouvait pas être supprimé !").arg
                            (serviceOuArticle);

            YerithQMessageBox::warning(this,
                                       QObject::tr
                                       ("suppression d'un stock (service)"),
                                       msgSupprimer);
        }

        setupLineEditsQCompleters((QObject *) this);

        lister_les_elements_du_tableau(*_curStocksTableModel);
    }
    else
    {
    }
}


void YerithStocksWindow::refineYerithLineEdits()
{
    _stocksDateEntreeFilter.clear();

    _stocksDateEntreeFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                   arg(YerithDatabaseTableColumn::DATE_ENTREE,
                                       DATE_TO_DB_FORMAT_STRING
                                       (dateEdit_stocks_debut->date()),
                                       YerithDatabaseTableColumn::DATE_ENTREE,
                                       DATE_TO_DB_FORMAT_STRING
                                       (dateEdit_stocks_fin->date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_stocks();
}


void YerithStocksWindow::reinitialiser_elements_filtrage()
{
    lineEdit_stocks_element_de_stock_resultat->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER;

    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithStocksWindow::reinitialiser_recherche()
{
    lineEdit_nom_element_string_db->clear();

    lineEdit_stocks_element_de_stock_resultat->clear();

    setCurrentlyFiltered(false);

    resetFilter();

    resetLineEditsQCompleters((QObject *) this);

    _localStrategy.clear();

    setComboBoxStrategieDeStocks();

    afficher_stocks();
}


void YerithStocksWindow::entrer(enum entrer_VISIBLE_CHOICE choice_insert
                                /* = set_choice_insert_STANDARD_INVENTORY_STOCK */
                               )
{
    if (_show_ASSET)
    {
        choice_insert = set_choice_insert_ASSET;
    }
    else
    {
        choice_insert = set_choice_insert_STANDARD_INVENTORY_STOCK;
    }

    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1 &&
    	_curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel,
                                                  true,
												  0,
												  choice_insert);
        rendreInvisible();
    }
    else
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel,
                                                  false,
												  0,
												  choice_insert);
        rendreInvisible();
    }
}


void YerithStocksWindow::modifier_les_articles()
{
    if (_curStocksTableModel->rowCount() > 0)
    {
        _allWindows->_modifierWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::warning(this, "modifier un stock",
                                   QObject::tr
                                   ("Sélectionnez un stock à modifier."));
    }
}


void YerithStocksWindow::lister_les_elements_du_tableau(YerithSqlTableModel &
                                                        sqlTableModel,
                                                        QString
                                                        localVisibleStrategy
                                                        /* = YerithUtils::EMPTY_STRING */
                                                       )
{
    _curStocksTableModel = &sqlTableModel;

    QString currentStockListingStrategy(localVisibleStrategy);

    if (localVisibleStrategy.isEmpty())
    {
        currentStockListingStrategy = YerithERPConfig::salesStrategy;
    }

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             currentStockListingStrategy)
            || YerithUtils::isEqualCaseInsensitive(YerithERPConfig::
                                                   STRATEGIE_VENTE_SORTIE_FIFO,
                                                   currentStockListingStrategy)
            || YerithUtils::isEqualCaseInsensitive(YerithERPConfig::
                                                   STRATEGIE_VENTE_SORTIE_LIFO,
                                                   currentStockListingStrategy))
    {
        tableView_stocks->setSortingEnabled(false);
    }
    else				//YerithConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
        tableView_stocks->setSortingEnabled(true);
    }

    int curStocksTableModelRowCount =
    		_curStocksTableModel->easySelect("src/windows/stocks/yerith-erp-stocks-window.cpp", 2149);

    double prix_vente = 0.0;

    uint TOTAL_ASSETS_STOCKS = 0;

    double quantite_stock = 0;
    double quantite_totale_ARTICLES_STOCK = 0;
    double prix_dachat = 0.0;
    double valeur_dinventaire = 0.0;
    double valeur_totale_dinventaire = 0.0;

    QSqlRecord aRecord;

    for (int k = 0; k < curStocksTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = _curStocksTableModel->record(k);

        quantite_stock =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        quantite_totale_ARTICLES_STOCK += quantite_stock;

        prix_dachat =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::PRIX_DACHAT).
                        toDouble();

        valeur_dinventaire = prix_dachat * quantite_stock;

        valeur_totale_dinventaire += valeur_dinventaire;
    }

    lineEdit_stocks_nombre_darticles->setText(GET_NUM_STRING
                                              (quantite_totale_ARTICLES_STOCK));

    lineEdit_nombre_de_stocks_IMMOBILISATIONS->setText(GET_NUM_STRING
                                                       (curStocksTableModelRowCount));

    lineEdit_stocks_valeur_totale_dinventaire->setText(GET_CURRENCY_STRING_NUM
                                                       (valeur_totale_dinventaire));


    tableView_stocks->queryYerithTableViewCurrentPageContentRow(*_curStocksTableModel,
    															currentStockListingStrategy);

    tableView_show_or_hide_columns(*tableView_stocks);


    comboBox_strategie_de_stocks
		->setCurrentIndex
			(YerithUtils::getComboBoxDatabaseQueryValue(localVisibleStrategy,
													    YerithUtils::_strategieindexToUserViewString));


	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
	    if (YerithUtils::isEqualCaseInsensitive(YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[ENGLISH],
	             	 	 	 	 	 	 	 	_CURRENT_choice_immobilisation_stock))
	    {
	        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("ASSET LISTING"),
	        								 _curStocksTableModel);
	    }
	    else
	    {
	        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("STOCK LISTING"),
	        								 _curStocksTableModel);
	    }
	}
	else
	{
	    if (YerithUtils::isEqualCaseInsensitive(YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[FRENCH],
	             	 	 	 	 	 	 	    _CURRENT_choice_immobilisation_stock))
	    {
	        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("Fiche DES Immobilisations"),
	        								 _curStocksTableModel);
	    }
	    else
	    {
	        YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("Fiche DES Stocks"),
	        								 _curStocksTableModel);
	    }
	}

	setWindowTitle(YerithUtils::getWindowTitleWithStrategy(this, localVisibleStrategy));
}


void YerithStocksWindow::lister_les_elements_du_tableau(QString strategieGlobale
                                                        /* = YerithUtils::EMPTY_STRING */
                                                       )
{
    QString curStrategy = strategieGlobale;

    if (curStrategy.isEmpty())
    {
        //set local visible strategy
        curStrategy = _localStrategy;
    }

    comboBox_strategie_de_stocks
		->setCurrentIndex(YerithUtils::getComboBoxDatabaseQueryValue(curStrategy,
																	 YerithUtils::_strategieindexToUserViewString));

    lister_les_elements_du_tableau(*_curStocksTableModel, curStrategy);
}


void YerithStocksWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_stocks_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_stocks_filtrer_font->setUnderline(false);
    }

    pushButton_stocks_filtrer->setFont(*_pushButton_stocks_filtrer_font);
}


bool YerithStocksWindow::export_csv_file()
{
    bool success = false;

    QList < int >tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_stocks,
                                               tableColumnsToIgnore,
                                               "yerith-erp-stocks-listing-csv-format",
                                               "stock listing");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_stocks,
                                               tableColumnsToIgnore,
                                               "yerith-erp-fichier-stocks-format-csv",
                                               "fiche des stocks");
    }


    return success;
}


QString YerithStocksWindow::get_latex_template_print_pdf_content()
{
    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	if (YerithUtils::isEqualCaseInsensitive(YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[ENGLISH],
    											_CURRENT_choice_immobilisation_stock))
    	{
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT",
						QString("ASSET LISTING \\_ %1")
							.arg(comboBox_strategie_de_stocks->currentText()));
    	}
    	else
    	{
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT",
						QString("STOCK LISTING \\_ %1")
							.arg(comboBox_strategie_de_stocks->currentText()));
    	}
    }
    else
    {
        if (YerithUtils::isEqualCaseInsensitive(YerithStocksWindow::_IMMOBILISATION_TEXT_STRING[FRENCH],
                 	 	 	 	 	 	 	 	_CURRENT_choice_immobilisation_stock))
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT",
						QString("Fiche DES Immobilisations \\_ %1")
							.arg(comboBox_strategie_de_stocks->currentText()));
        }
        else
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YERITHSUBJECT",
						QString("Fiche DES Stocks \\_ %1")
							.arg(comboBox_strategie_de_stocks->currentText()));
        }
    }


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_lister_stock_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_lister_stock_tex;
    }


    return _latex_template_print_pdf_content;
}
