/*
 * yerith-erp-point-de-vente-table-widget.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-point-de-vente-table-widget.hpp"

#include "src/utils/yerith-erp-logger.hpp"


const unsigned int YerithPointDeVenteTableWidget::REFERENCE_COLUMN(0);

const unsigned int YerithPointDeVenteTableWidget::DESIGNATION_COLUMN(1);

const unsigned int YerithPointDeVenteTableWidget::CATEGORIE_COLUMN(2);

const unsigned int YerithPointDeVenteTableWidget::TOTAL_TVA_COLUMN(6);

const unsigned int YerithPointDeVenteTableWidget::SERVICE_COST_COLUMN(7);

const unsigned int YerithPointDeVenteTableWidget::TOTAL_COLUMN(8);

const unsigned int YerithPointDeVenteTableWidget::QTE_COLUMN(3);

const unsigned int YerithPointDeVenteTableWidget::QTE_TOTAL_STOCK(4);


int YerithPointDeVenteTableWidget::addArticleAfter(YerithArticleVenteInfo &
                                                   articleVenteInfo)
{
    _logger->log("addArticleAfter(YerithArticleVenteInfo &)");

    setRowCount(_curRow + 1);

    double prix_unitaire = articleVenteInfo.prix_unitaire();

    if (articleVenteInfo.remise_prix() > 0)
    {
        prix_unitaire =
                        articleVenteInfo.prix_unitaire() - articleVenteInfo.remise_prix();
    }

    _stockReferenceItem =
                    new YerithQTableWidgetItem(articleVenteInfo._stockReference);
    _stockNameItem = new YerithQTableWidgetItem(articleVenteInfo._stockName);
    _stockCategorieItem =
                    new YerithQTableWidgetItem(articleVenteInfo._stockCategorie);
    _prixUnitaireItem =
                    new YerithQTableWidgetItem(QString::number(prix_unitaire, 'f', 2));
    _montantTvaItem =
                    new YerithQTableWidgetItem(articleVenteInfo.montantTva());

    _SERVICECOSTItem =
                    new YerithQTableWidgetItem(articleVenteInfo.Get_ajout_pour_COMMERCIAL_text());

    _prixVenteItem =
                    new
    YerithQTableWidgetItem(GET_CURRENCY_STRING_NUM(articleVenteInfo.prix_vente()));

    _qteItem = new YerithQTableWidgetItem(articleVenteInfo.quantiteAVendre());
    _qteTotalStockItem =
                    new YerithQTableWidgetItem(articleVenteInfo.quantiteEnStock());


    _mapListIdxToElement_db_ID.insert(_curRow, articleVenteInfo._stockID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerithPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _stockReferenceItem);
    setItem(_curRow, idx++, _stockNameItem);
    setItem(_curRow, idx++, _stockCategorieItem);
    setItem(_curRow, idx++, _qteItem);
    setItem(_curRow, idx++, _qteTotalStockItem);
    setItem(_curRow, idx++, _prixUnitaireItem);
    setItem(_curRow, idx++, _montantTvaItem);
    setItem(_curRow, idx++, _SERVICECOSTItem);
    setItem(_curRow, idx, _prixVenteItem);

    _stockReferenceItem->setFlags(_myQStandardItemFlags);
    _stockNameItem->setFlags(_myQStandardItemFlags);
    _stockCategorieItem->setFlags(_myQStandardItemFlags);
    _qteItem->setFlags(_myQStandardItemFlags | Qt::ItemIsEditable);
    _qteTotalStockItem->setFlags(_myQStandardItemFlags);
    _prixUnitaireItem->setFlags(_myQStandardItemFlags);
    _montantTvaItem->setFlags(_myQStandardItemFlags);
    _SERVICECOSTItem->setFlags(_myQStandardItemFlags);
    _prixVenteItem->setFlags(_myQStandardItemFlags);

    setColours_QTE_QStandardItem();

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    ++_curRow;

    emit addedYerithTableWidget();

    return lastCurRow;
}


int YerithPointDeVenteTableWidget::addArticle(QString stockID,
                                              QString stockReference,
                                              QString stockName,
                                              QString stockCategorie,
                                              QString prix_unitaire,
											  QString montant_tva,
											  QString service_cost,
                                              QString prix_vente,
                                              QString quantite_a_vendre,
                                              QString quantite_total_stock)
{
    _logger->log("addArticle");

    setRowCount(_curRow + 1);

    _stockReferenceItem = new YerithQTableWidgetItem(stockReference);
    _stockNameItem = new YerithQTableWidgetItem(stockName);
    _stockCategorieItem = new YerithQTableWidgetItem(stockCategorie);
    _prixUnitaireItem = new YerithQTableWidgetItem(prix_unitaire);
    _montantTvaItem = new YerithQTableWidgetItem(montant_tva);
    _SERVICECOSTItem = new YerithQTableWidgetItem(service_cost);
    _prixVenteItem = new YerithQTableWidgetItem(prix_vente);
    _qteItem = new YerithQTableWidgetItem(quantite_a_vendre);
    _qteTotalStockItem = new YerithQTableWidgetItem(quantite_total_stock);


    _mapListIdxToElement_db_ID.insert(_curRow, stockID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerithPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _stockReferenceItem);
    setItem(_curRow, idx++, _stockNameItem);
    setItem(_curRow, idx++, _stockCategorieItem);
    setItem(_curRow, idx++, _qteItem);
    setItem(_curRow, idx++, _qteTotalStockItem);
    setItem(_curRow, idx++, _prixUnitaireItem);
    setItem(_curRow, idx++, _montantTvaItem);
    setItem(_curRow, idx++, _SERVICECOSTItem);
    setItem(_curRow, idx, _prixVenteItem);

    _stockReferenceItem->setFlags(_myQStandardItemFlags);
    _stockNameItem->setFlags(_myQStandardItemFlags);
    _stockCategorieItem->setFlags(_myQStandardItemFlags);
    _qteItem->setFlags(_myQStandardItemFlags | Qt::ItemIsEditable);
    _qteTotalStockItem->setFlags(_myQStandardItemFlags);
    _prixUnitaireItem->setFlags(_myQStandardItemFlags);
    _montantTvaItem->setFlags(_myQStandardItemFlags);
    _SERVICECOSTItem->setFlags(_myQStandardItemFlags);
    _prixVenteItem->setFlags(_myQStandardItemFlags);

    setColours_QTE_QStandardItem();

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    ++_curRow;

    emit addedYerithTableWidget();

    return lastCurRow;
}
