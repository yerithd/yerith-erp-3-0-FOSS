/*
 * yerith-erp-COMPTABILITE-table-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_COMPTABILITE_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YERITH_COMPTABILITE_TABLE_WIDGET_HPP_

#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"

class YerithComptabiliteTableWidget : public YerithTableWidget
{
    Q_OBJECT

public:

	YERITH_CLASS_OPERATORS

	inline YerithComptabiliteTableWidget(QWidget *parent = 0)
	:YerithTableWidget(parent),
	 _yr_type_doperation_financiere_item(0),
	 _yr_intitule_ligne_budgetaire_item(0)
	{
	}


	virtual inline ~YerithComptabiliteTableWidget()
	{
	}


    inline bool REMOVE_an_association(int tableWidgetRow)
    {
        YerithTableWidget::removeArticle(tableWidgetRow);
    }


	int add_an_association(QString yr_type_doperation_financiere_DB_ID,
                           QString yr_type_doperation_financiere,
                           QString yr_type_de_credit_debit,
                           QString yr_intitule_ligne_budgetaire);


    static const unsigned int ID_COLUMN;

    static const unsigned int TYPE_DOPERATION_FINANCIERE_COLUMN;

    static const unsigned int TYPE_DE_CREDIT_DEBIT_COLUMN;

	static const unsigned int INTITULE_LIGNE_BUDGETAIRE_COLUMN;


private:

    YerithQTableWidgetItem *_yr_DB_ID_item;

    YerithQTableWidgetItem *_yr_type_doperation_financiere_item;

    YerithQTableWidgetItem *_yr_type_de_credit_debit_item;

	YerithQTableWidgetItem *_yr_intitule_ligne_budgetaire_item;
};

#endif /* SRC_WIDGETS_YERITH_COMPTABILITE_TABLE_WIDGET_HPP_ */
