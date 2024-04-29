/*
 * yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_SUPPLIER_DETAILS_DUN_GROUPE_DE_PAIE_hr_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_SUPPLIER_DETAILS_DUN_GROUPE_DE_PAIE_hr_WINDOW_HPP_

#include "../../../ui_yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.h"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithDetailsDunGroupeDePaieWindow : public YerithWindowsCommons,
    									   private Ui_YerithDetailsDunGroupeDePaieWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithDetailsDunGroupeDePaieWindow();

    inline ~YerithDetailsDunGroupeDePaieWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerithDetailsDunGroupeDePaieWindow;
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
                                       QObject::tr("Vous pouvez modifier les détails de ce programme de fidélité clients "
                                    		   	   "en cliquant sur 'Modifier' !"));
    }

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showPayGroup_PROGRAM_DETAIL();


    YerithLogger *_logger;

    YerithSqlTableModel *_curPAY_GROUP_TableModel;
};


#endif /* SRC_WINDOWS_YERITH_SUPPLIER_DETAILS_DUN_GROUPE_DE_PAIE_hr_WINDOW_HPP_ */
