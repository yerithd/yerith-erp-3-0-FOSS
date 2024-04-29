/*
 * yerith-erp-stock-detail-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_STOCK_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_STOCK_DETAIL_WINDOW_HPP_

#include "../../../ui_yerith-erp-stock-detail-window.h"


#include "src/utils/yerith-erp-logger.hpp"

#include "src/utils/windows/yerith-erp-abstract-class-detail-view-window.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithStockDetailWindow : public YerithWindowsCommons,
    							private Ui_YerithStockDetailWindow,
								public YerithERPAbstractClassDetailViewFOR_WINDOW
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithStockDetailWindow();

    inline ~YerithStockDetailWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_detailWindow;
    }

    void definirPasDeRole();

    void definirCaissier();

    void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    void definirMagasinier();

    void rendreInvisible();

    void rendreVisible(YerithSqlTableModel *stocksTableModel = 0);

public slots:

    virtual void modifier_les_articles();

    void supprimer_ce_stock();

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Vous pouvez retourner à la page précédente en "
                                        "cliquant sur 'Retour'!"));
    }

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void setStockAchatValuesVisibility(bool aVisibility);

    void setStockSpecificWidgetVisibility(bool aVisibility);

    virtual void showItem_YerithERPAbstractClassDetailViewFOR_WINDOW();

    void checkCourrierAlerts();

    YerithLogger *_logger;
};


#endif /* SRC_WINDOWS_YERITH_STOCK_DETAIL_WINDOW_HPP_ */
