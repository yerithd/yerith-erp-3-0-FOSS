/*
 * yerith-erp-details-dun-programme-de-fidelite-clients-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yerith-erp-details-dun-programme-de-fidelite-clients-window.h"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithDetailsDunProgrammeDeFideliteClientsWindow : public YerithWindowsCommons,
    												     private Ui_YerithDetailsDunProgrammeDeFideliteClientsWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithDetailsDunProgrammeDeFideliteClientsWindow();

    inline ~YerithDetailsDunProgrammeDeFideliteClientsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerithDetailsDunProgrammeDeFideliteClientsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual inline void definirVendeur()
    {
        definirManager();
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

    virtual void rendreInvisible();

    virtual void rendreVisible(YerithSqlTableModel *curClient_LOYALTY_PROGRAM_TableModel,
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
                                       ("Vous pouvez modifier les détails de ce programme de fidélité clients "
                                        "en cliquant sur 'Modifier' !"));
    }

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showClientLOYALTY_PROGRAM_DETAIL();


    YerithLogger *_logger;

    YerithSqlTableModel *_curClient_LOYALTY_PROGRAM_TableModel;
};


#endif /* SRC_WINDOWS_YERITH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
