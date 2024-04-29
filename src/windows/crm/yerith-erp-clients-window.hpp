/*
 * yerith-erp-clients-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_ERP_CLIENTS_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_ERP_CLIENTS_WINDOW_HPP_

#include "../../../ui_yerith-erp-clients-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerithSqlTableModel;

class YerithERPClientsWindow : public YerithWindowsCommons,
    						   private Ui_YerithERPClientsWindow,
							   public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithERPClientsWindow();

    virtual ~YerithERPClientsWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_clientsWindow;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_clients_numero_page_derniere,
    												  label_clients_numero_page_courante)

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);


public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_clients)

	virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Sélectionner un compte client dans le tableau "
                                        "des comptes clients, ensuite cliquer sur l'opération que vous souhaitez réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    virtual void afficherClients(YerithSqlTableModel &clientSqlTableModel);

    inline void afficherClients()
    {
        afficherClients(*_curClientsTableModel);
    }

    void afficher_nom_entreprise_selectioner(const QString &nomEntreprise);


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

    virtual void set_filtrer_font();


protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


private slots:

    void afficher_groupes_dun_client();

    void private_payer_au_compteclient();

    void private_payer_au_compteclient(const QModelIndex &aModelIndex);

    void modifierCompteClient();

    void supprimer_PLUSIEURS_Clients(YerithSqlTableModel &aClientsTableModel);

    void supprimerCompteClient();

    void afficher_au_detail();

    inline void afficher_au_detail(const QModelIndex &modelIndex)
    {
        afficher_au_detail();
    }

    bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void handle_filtres_checkBox(int state);


private:

    void initialiser_FILTRES();

    void populateClientsComboBoxes();

    void setupLineEdits();


    YerithLogger *_logger;

    QFont *_pushButton_filtrer_font;

    YerithSqlTableModel *_curClientsTableModel;
};

#endif /* SRC_WINDOWS_YERITH_ERP_CLIENTS_WINDOW_HPP_ */
