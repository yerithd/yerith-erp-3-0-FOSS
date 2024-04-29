/*
 * yerith-erp-achats-detail-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_ACHATS_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_ACHATS_DETAIL_WINDOW_HPP_

#include "../../../ui_yerith-erp-achats-detail-window.h"

#include "src/utils/yerith-erp-logger.hpp"

#include <QtWidgets/QMessageBox>
#include "src/windows/yerith-erp-window-commons.hpp"

class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithAchatsDetailWindow : public YerithWindowsCommons,
    							 private Ui_YerithAchatsDetailWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithAchatsDetailWindow();

    inline ~YerithAchatsDetailWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_detailWindow;
    }

    void definirCaissier();

    void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    void definirMagasinier();

    void definirPasDeRole();

    void rendreInvisible();

    void rendreVisible(YerithSqlTableModel *achatStocksTableModel,
                       YerithSqlTableModel *stocksTableModel);

public slots:

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

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showItem();

    void checkCourrierAlerts();

    int _achatLastSelectedRow;

    YerithSqlTableModel *_curAchatStocksTableModel;

    YerithLogger *_logger;
};


#endif /* SRC_WINDOWS_YERITH_ACHATS_DETAIL_WINDOW_HPP_ */
