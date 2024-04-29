/*
 * yerith-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_FOURNISSEURS_DETAILS_DUN_GROUPE_DEMPLOYES_HR_WINDOW_HPP_
#define YERITH_FOURNISSEURS_DETAILS_DUN_GROUPE_DEMPLOYES_HR_WINDOW_HPP_

#include "../../../ui_yerith-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.h"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithDetailsDunGroupeDemployesWindow : public YerithWindowsCommons,
    										  private Ui_YerithDetailsDunGroupeDemployesWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithDetailsDunGroupeDemployesWindow();

    inline ~YerithDetailsDunGroupeDemployesWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_detailsDunGroupeDemployesWindow;
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

    virtual inline void rendreInvisible()
    {
        YerithWindowsCommons::rendreInvisible();
    }

    virtual void rendreVisible(YerithSqlTableModel *aClientTableModel,
                               YerithSqlTableModel *aClientGroupTableModel,
                               YerithSqlTableModel *stocksTableModel,
                               const QString &clientGroup_db_ID = YerithUtils::EMPTY_STRING);

    virtual void rendreVisible(YerithSqlTableModel *clientTableModel,
                               YerithSqlTableModel *stocksTableModel,
                               const QString &EMPLOYEE_Group_db_ID = YerithUtils::EMPTY_STRING);

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

private slots:

    void afficher_groupes_dun_EMPLOYE();

private:

    void setupLineEdits();

    void showEmployeeGroup_DETAIL(const QString &clientGroup_db_ID =
                                                YerithUtils::EMPTY_STRING);


    YerithLogger *_logger;

    YerithSqlTableModel *_curEmployeeTableModel;

    YerithSqlTableModel *_curEmployeeGroupTableModel;
};


#endif /* YERITH_FOURNISSEURS_DETAILS_DUN_GROUPE_DEMPLOYES_HR_WINDOW_HPP_ */
