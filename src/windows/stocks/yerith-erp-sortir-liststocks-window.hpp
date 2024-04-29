/*
 * yerith-erp-sortir-liststocks-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_SORTIR_LISTSTOCKS_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_SORTIR_LISTSTOCKS_WINDOW_HPP_

#include "../../../ui_yerith-erp-sortir-liststocks-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


class YerithERPWindows;
class YerithSqlTableModel;
class YerithPOSUser;

class YerithSortirListStocksWindow : public YerithWindowsCommons,
    								 private Ui_YerithSortirListStocksWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithSortirListStocksWindow();

    inline ~YerithSortirListStocksWindow()
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
        tableView_list_stocks->resize(event->size());
    }

private:

    YerithLogger *_logger;
};

#endif /* SRC_WINDOWS_YERITH_SORTIR_LISTSTOCKS_WINDOW_HPP_ */
