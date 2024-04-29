/*
 * yerith-erp-fournisseurs-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_ERP_FOURNISSEURS_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_ERP_FOURNISSEURS_WINDOW_HPP_

#include "../../../ui_yerith-erp-fournisseurs-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerithSqlTableModel;

class YerithERPFournisseursWindow : public YerithWindowsCommons,
    								private Ui_YerithERPFournisseursWindow,
									public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithERPFournisseursWindow();

    virtual ~YerithERPFournisseursWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_fournisseursWindow;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_fournisseurs_numero_page_derniere,
    												  label_fournisseurs_numero_page_courante)

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

    inline bool is_Show_Employee()
    {
        return _SHOW_EMPLOYEE;
    }


public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_fournisseurs)

	virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("Sélectionner un fournisseur dans le tableau "
                                    		   	   "des fournisseurs, ensuite cliquer sur l'opération que vous souhaitez réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &fournisseurSqlTableModel);

    void lister_les_elements_du_tableau();

    void afficher_nom_entreprise_selectioner(const QString &nomEntreprise);


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void hide_SHOW_EMPLOYEEEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

    virtual void set_filtrer_font();


protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


private:

    void initialiser_FILTRES();


private slots:

	void afficher_groupes_dun_employe();

	void handle_ACTION_CHOICE_FOURNISSEURS_EMPLOYES();

	void handle_RADIO_BUTTONS_FOURNISSEURS_employes(bool is_toggle);

    void handle_filtres_checkBox(int state);

    void modifierFournisseur();

    void supprimer_PLUSIEURS_Fournisseurs(YerithSqlTableModel &aFournisseursTableModel);

    void supprimerFournisseur();

    virtual void afficher_au_detail();

    virtual inline void afficher_au_detail(const QModelIndex &modelIndex)
    {
        afficher_au_detail();
    }

    bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

private:

    void populateFournisseursComboBoxes();

    void setupLineEdits();


    bool _SHOW_EMPLOYEE;

    YerithLogger *_logger;

    QFont *_pushButton_filtrer_font;

    YerithSqlTableModel *_curFournisseursTableModel;
};

#endif /* SRC_WINDOWS_YERITH_ERP_FOURNISSEURS_WINDOW_HPP_ */
