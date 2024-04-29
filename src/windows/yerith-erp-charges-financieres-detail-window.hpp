/*
 * yerith-erp-charges-financieres-detail-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_CHARGES_FINANCIERES_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_CHARGES_FINANCIERES_DETAIL_WINDOW_HPP_

#include "../../ui_yerith-erp-charges-financieres-detail-window.h"

#include "src/utils/yerith-erp-logger.hpp"

#include <QtWidgets/QMessageBox>
#include "src/windows/yerith-erp-window-commons.hpp"

class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithChargesFinancieresDetailsWindow : public YerithWindowsCommons,
    										  private Ui_YerithChargesFinancieresDetailsWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithChargesFinancieresDetailsWindow();

    inline ~YerithChargesFinancieresDetailsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_CHARGE_FINANCIERE_detailWindow;
    }

    virtual inline void definirCaissier()
    {
    	definirPasDeRole();
    }

    virtual void definirManager();

    virtual inline void definirVendeur()
    {
    	definirPasDeRole();
    }

    virtual inline void definirGestionaireDesStocks()
    {
    	definirPasDeRole();
    }

    virtual inline void definirMagasinier()
    {
    	definirPasDeRole();
    }

    virtual void definirPasDeRole();


    void rendreInvisible();

    void rendreVisible(YerithSqlTableModel *achatStocksTableModel,
                       YerithSqlTableModel *stocksTableModel);


public slots:

    virtual bool imprimer_pdf_document();

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

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showItem();

    void checkCourrierAlerts();

    int _achatLastSelectedRow;

    YerithSqlTableModel *_cur_CHARGES_FINANCIERESTableModel;

    YerithLogger *_logger;
};


#endif /* SRC_WINDOWS_YERITH_CHARGES_FINANCIERES_DETAIL_WINDOW_HPP_ */
