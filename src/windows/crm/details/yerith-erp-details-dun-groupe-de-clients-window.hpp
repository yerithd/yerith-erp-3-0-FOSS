/*
 * yerith-erp-details-dun-groupe-de-clients-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CLIENTS_DETAILS_DUN_GROUPE_DE_CLIENTS_WINDOW_HPP_
#define YERITH_CLIENTS_DETAILS_DUN_GROUPE_DE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yerith-erp-details-dun-groupe-de-clients-window.h"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithDetailsDunGroupeDeClientsWindow : public YerithWindowsCommons,
    										  private Ui_YerithDetailsDunGroupeDeClientsWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithDetailsDunGroupeDeClientsWindow();

    inline ~YerithDetailsDunGroupeDeClientsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_detailsDunGroupeDeClientsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

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

    virtual void rendreVisible(YerithSqlTableModel *aClientTableModel,
                               YerithSqlTableModel *aClientGroupTableModel,
                               YerithSqlTableModel *stocksTableModel,
                               const QString &clientGroup_db_ID =
                                               YerithUtils::EMPTY_STRING);

    virtual void rendreVisible(YerithSqlTableModel *clientTableModel,
                               YerithSqlTableModel *stocksTableModel,
                               const QString &clientGroup_db_ID =
                                               YerithUtils::EMPTY_STRING);

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

    void afficher_groupes_dun_client();

    void modifier_un_groupe_de_clients();

private:

    void setupLineEdits();

    void showClientGroup_DETAIL(const QString &clientGroup_db_ID = YerithUtils::EMPTY_STRING);


    YerithLogger *_logger;

    YerithSqlTableModel *_curClientTableModel;

    YerithSqlTableModel *_curClientGroupTableModel;
};


#endif /* YERITH_CLIENTS_DETAILS_DUN_GROUPE_DE_CLIENTS_WINDOW_HPP_ */
