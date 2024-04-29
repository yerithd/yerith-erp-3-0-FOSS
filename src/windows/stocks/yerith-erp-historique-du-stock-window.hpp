/*
 * yerith-erp-pointdevente-liststocks-window..hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_STOCKS_HISTORIQUE_DU_STOCK_WINDOW_HPP_
#define SRC_YERITH_STOCKS_HISTORIQUE_DU_STOCK_WINDOW_HPP_

#include "../../../ui_yerith-erp-historique-du-stock-window.h"

#include "src/yerith-erp-windows.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"



class YerithHistoriqueDuStockWindow : public YerithWindowsCommons,
    								  private Ui_YerithHistoriqueDuStockWindow,
									  public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithHistoriqueDuStockWindow();

    ~YerithHistoriqueDuStockWindow();

    void clear_CURRENT_STOCK_HISTORY();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_historiqueDunStock;
    }

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_numero_page_derniere,
    												  label_numero_page_courante)

public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_historique_du_stock)


	void listHistoriqueDuStock(const QStringList &aMouvementStockList,
                               const QString 	 stockReference,
							   const QString 	 stockDesignation,
							   bool 			 show_ASSET = false);

    bool export_csv_file();

    virtual bool imprimer_pdf_document();


protected slots:

    virtual void textChangedSearchLineEditsQCompleters();

private slots:

    inline uint GET_ROW_NUMBER_PER_VIEWING_PAGE()
    {
        return lineEdit_nombre_de_lignes_par_page->text().toUInt();
    }

    void SLOT_line_per_pagte_MODIFIED(const QString &a_line_per_page_NR);

    void appliquer_bouton_PREMIERE_PAGE();

    void appliquer_bouton_PRECEDENTE_PAGE();

    void appliquer_bouton_SUIVANTE_PAGE();

    void appliquer_bouton_DERNIERE_PAGE();


protected:

    virtual void setupShortcuts();


public:

    //static const uint ROW_SIZE_PER_VIEWING_PAGE;


private:

    void setupLineEdits();


    bool _show_ASSET;

    uint _last_page_nr;

    uint _CURRENT_PAGING_NR;

    QMap <uint, QString> _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST;


    QStringList _a_previous_mouvement_stock_list;

    QString _currentStockID;

    QString _previousStockID;

    QString _currentStockReference;

    QString _currentStockDesignation;

    YerithLogger *_logger;
};

#endif /* SRC_YERITH_STOCKS_HISTORIQUE_DU_STOCK_WINDOW_HPP_ */
