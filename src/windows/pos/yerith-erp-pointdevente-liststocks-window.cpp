/*
 * yerith-erp-pointdevente-liststocks-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-pointdevente-liststocks-window.hpp"


#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/users/yerith-erp-users.hpp"


#include <QtWidgets/QDesktopWidget>


YerithPointDeVenteListStocksWindow::YerithPointDeVenteListStocksWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithVenteListStocksWindow"))
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("stocks à vendre"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_PDV_list_stocks);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::REFERENCE
            << YerithDatabaseTableColumn::DESCRIPTION_PRODUIT
            << YerithDatabaseTableColumn::PRIX_UNITAIRE
            << YerithDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS
            << YerithDatabaseTableColumn::MONTANT_TVA
            << YerithDatabaseTableColumn::MONTANT_TVA_EN_GROS
            << YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS
            << YerithDatabaseTableColumn::STOCK_DALERTE
            << YerithDatabaseTableColumn::LOCALISATION_STOCK
            << YerithDatabaseTableColumn::IMAGE_PRODUIT
            << YerithDatabaseTableColumn::DATE_ENTREE
            << YerithDatabaseTableColumn::DATE_PEREMPTION
            << YerithDatabaseTableColumn::LOCALISATION
            << YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR
            << YerithDatabaseTableColumn::LOTS_ENTRANT
            << YerithDatabaseTableColumn::QUANTITE_PAR_LOT
            << YerithDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF
            << YerithDatabaseTableColumn::HISTORIQUE_STOCK
            << YerithDatabaseTableColumn::PRIX_DACHAT
            << YerithDatabaseTableColumn::REFERENCE_RECU_DACHAT
            << YerithDatabaseTableColumn::ENREGISTREUR_STOCK
            << YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT
            << YerithDatabaseTableColumn::IS_SERVICE;


    setup_select_configure_dbcolumn(YerithDatabase::STOCKS);

    enableResizing();
}


void YerithPointDeVenteListStocksWindow::listStocks(YerithSqlTableModel &aSqlTableModel)
{
	retranslateUi(this);

	tableView_PDV_list_stocks->_currentViewWindow = this;

    tableView_PDV_list_stocks->lister_les_elements_du_tableau(aSqlTableModel,
                                                              YerithERPConfig::salesStrategy,
															  YerithUtils::EMPTY_STRING,
															  false);

    if (YerithUtils::isEqualCaseInsensitive(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             	 	 	 	 	 	 	 	YerithERPConfig::salesStrategy)					||
			YerithUtils::isEqualCaseInsensitive(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
                                                YerithERPConfig::salesStrategy)			||
			YerithUtils::isEqualCaseInsensitive(YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
                                                YerithERPConfig::salesStrategy))
    {
        tableView_PDV_list_stocks->setSortingEnabled(false);
    }
    else				//YerithConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
        tableView_PDV_list_stocks->setSortingEnabled(true);
    }

    yerith_hide_columns();

    tableView_PDV_list_stocks->selectRow(0);

    char *aWindowTitleWithStrategy = const_cast<char *>
    			(YerithUtils::getWindowTitleWithStrategy(this).toStdString().c_str());

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName(aWindowTitleWithStrategy),
      								 _curStocksTableModel);
    show();
}
