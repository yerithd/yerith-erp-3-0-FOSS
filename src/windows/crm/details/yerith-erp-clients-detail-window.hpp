/*
 * yerith-erp-clients-detail-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_CLIENTS_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_CLIENTS_DETAIL_WINDOW_HPP_

#include "../../../../ui_yerith-erp-clients-detail-window.h"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithClientsDetailWindow : public YerithWindowsCommons,
    							  private Ui_YerithClientsDetailWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithClientsDetailWindow();

    inline ~YerithClientsDetailWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_clientsDetailWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerithSqlTableModel *clientTableModel,
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
                                       QObject::tr("Vous pouvez modifier les détails de ce compte client "
                                    		   	   "en cliquant sur 'Modifier' !"));
    }

protected:

    virtual void setupShortcuts();

private slots:

    virtual inline void imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()
    {
        YerithProgressBar(this)(this,
        						&YerithClientsDetailWindow::imprimer_pdf_document);
    }

    inline void YERITH_PROGRESS_BAR_generer_la_carte_de_fidelite_du_client()
    {
        YerithProgressBar(this)(this,
        						&YerithClientsDetailWindow::generer_la_carte_de_fidelite_du_client);
    }

    bool generer_la_carte_de_fidelite_du_client();

    void afficher_groupes_dun_client();

    void private_payer_au_compteclient();

    void modifierCompteClient();

    void supprimerCompteClient();

private:

    void setupLineEdits();

    void showClientDetail();

    void checkCourrierAlerts();


    static const int TRUNCATE_POS_FOR_NOM_ENTREPRISE;


    YerithLogger *_logger;

    QStringList _curClientGroups;

    YerithSqlTableModel *_curClientTableModel;
};


#endif /* SRC_WINDOWS_YERITH_CLIENTS_DETAIL_WINDOW_HPP_ */
