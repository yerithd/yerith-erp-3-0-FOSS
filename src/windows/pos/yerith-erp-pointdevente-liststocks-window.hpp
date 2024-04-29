/*
 * yerith-erp-pointdevente-liststocks-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_VENTE_LISTSTOCKS_WINDOW_HPP_
#define SRC_YERITH_VENTE_LISTSTOCKS_WINDOW_HPP_

#include "../../../ui_yerith-erp-pointdevente-liststocks-window.h"


#include "src/yerith-erp-windows.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"


class YerithERPWindows;
class YerithSqlTableModel;
class YerithPOSUser;


class YerithPointDeVenteListStocksWindow : public YerithWindowsCommons,
    									   private Ui_YerithVenteListStocksWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithPointDeVenteListStocksWindow();

    inline ~YerithPointDeVenteListStocksWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return 0;
    }

    void listStocks(YerithSqlTableModel &aSqlTableModel);

protected:

    inline void virtual resizeEvent(QResizeEvent *event)
    {
        tableView_PDV_list_stocks->resize(event->size());
    }

private:

    YerithLogger *_logger;
};

#endif /* SRC_YERITH_VENTE_LISTSTOCKS_WINDOW_HPP_ */
