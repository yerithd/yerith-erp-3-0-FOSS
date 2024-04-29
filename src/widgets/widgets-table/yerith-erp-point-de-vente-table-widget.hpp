/*
 * yerith-erp-point-de-vente-table-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_POINT_DE_VENTE_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YERITH_POINT_DE_VENTE_TABLE_WIDGET_HPP_

#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"

class YerithPointDeVenteTableWidget: public YerithTableWidget
{
Q_OBJECT
public:

	YERITH_CLASS_OPERATORS
	inline YerithPointDeVenteTableWidget(QWidget *parent = 0)
	:YerithTableWidget(parent),
	 _stockReferenceItem(0),
	 _stockNameItem(0),
	 _stockCategorieItem(0),
	 _prixUnitaireItem(0),
	 _montantTvaItem(0),
	 _SERVICECOSTItem(0),
	 _prixVenteItem(0),
	 _qteItem(0),
	 _qteTotalStockItem(0)
	{
	}

	virtual inline ~YerithPointDeVenteTableWidget()
	{
	}

	inline void setColours_QTE_QStandardItem()
	{
		_qteItem->setForeground(YerithUtils::YERITH_QBRUSH_ORANGE);
	}

	int addArticleAfter(YerithArticleVenteInfo &articleVenteInfo);

	int addArticle(QString stockID,
				   QString stockReference,
				   QString stockName,
				   QString stockCategorie,
				   QString prix_unitaire,
				   QString montant_tva,
				   QString service_cost,
				   QString prix_vente,
				   QString quantite_a_vendre,
				   QString quantite_total_stock);

	static const unsigned int REFERENCE_COLUMN;

	static const unsigned int CATEGORIE_COLUMN;

	static const unsigned int DESIGNATION_COLUMN;

	static const unsigned int TOTAL_TVA_COLUMN;

	static const unsigned int SERVICE_COST_COLUMN;

	static const unsigned int TOTAL_COLUMN;

	static const unsigned int QTE_COLUMN;

	static const unsigned int QTE_TOTAL_STOCK;

private:

	YerithQTableWidgetItem *_stockReferenceItem;

	YerithQTableWidgetItem *_stockNameItem;

	YerithQTableWidgetItem *_stockCategorieItem;

	YerithQTableWidgetItem *_prixUnitaireItem;

	YerithQTableWidgetItem *_montantTvaItem;

	YerithQTableWidgetItem *_SERVICECOSTItem;

	YerithQTableWidgetItem *_prixVenteItem;

	YerithQTableWidgetItem *_qteItem;

	YerithQTableWidgetItem *_qteTotalStockItem;
};

#endif /* SRC_WIDGETS_YERITH_POINT_DE_VENTE_TABLE_WIDGET_HPP_ */
