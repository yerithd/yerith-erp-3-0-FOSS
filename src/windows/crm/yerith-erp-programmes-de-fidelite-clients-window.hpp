/*
 * yerith-erp-programmes-de-fidelite-clients-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_PROGRAMMES_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define YERITH_ERP_PROGRAMMES_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../ui_yerith-erp-programmes-de-fidelite-clients-window.h"

#include <QtCore/QDebug>

#include <QtCore/QVector>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "src/windows/yerith-erp-window-commons.hpp"


class YerithPOSDialogCommons;
class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerithSqlTableModel;

class YerithERPProgrammesDeFideliteClientsWindow : public YerithWindowsCommons,
    											   private Ui_YerithERPProgrammesDeFideliteClientsWindow,
												   public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithERPProgrammesDeFideliteClientsWindow();

    virtual ~YerithERPProgrammesDeFideliteClientsWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_programmes_de_fidelite_clientsWindow;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_programmes_de_fidelite_clients_numero_page_derniere,
    												  label_programmes_de_fidelite_clients_numero_page_courante)

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);


public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_programmes_de_fidelite_clients)

	virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("1) Sélectionner un 'programme de fidélité clients' dans le"
                                    		   	   " tableau de cette page, ensuite cliquer sur"
                                    		   	   " l'opération que vous souhaitez"
                                    		   	   " réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    void afficher_au_detail();

    inline void afficher_au_detail(const QModelIndex &modelIndex)
    {
        afficher_au_detail();
    }

    void refineYerithLineEdits();

    bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();


private:

    void initialiser_FILTRES();


private slots:

    void handle_filtres_checkBox(int state);

    void supprimer_PLUSIEURS_programmes_de_fidelite_clients(YerithSqlTableModel &a_LOYALTY_PROGRAM_TableModel);

    void supprimer_un_programme_de_fidelite_clients();

    void afficher_programmes_de_fidelite_clients(YerithSqlTableModel &client_LOYALTY_PROGRAM_TableModel);

    inline void afficher_programmes_de_fidelite_clients()
    {
        afficher_programmes_de_fidelite_clients(*_curClient_LOYALTY_PROGRAM_TableModel);
    }

    bool filtrer_programmes_de_fidelite_client();


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual inline void hideEvent(QHideEvent *hideEvent)
    {
        YerithWindowsCommons::CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
    }

    virtual void setupShortcuts();

    void resetFilter();


protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


private:

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    QString _localStrategy;

    YerithLogger *_logger;

    QStringList _historiqueStock;

    QString _client_LOYALTY_PROGRAM_DateCreationFilter;

    QFont *_pushButton_filtrer_font;

    YerithSqlTableModel *_curClient_LOYALTY_PROGRAM_TableModel;
};

#endif /* YERITH_ERP_PROGRAMMES_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
