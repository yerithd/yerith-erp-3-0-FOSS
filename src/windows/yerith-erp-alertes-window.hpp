/*
 * yerith-erp-alertes-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_ALERTES_WINDOW_HPP_
#define YERITH_ERP_ALERTES_WINDOW_HPP_

#include "../../ui_yerith-erp-alertes-window.h"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


#include "yerith-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerithSqlTableModel;

class YerithAlertesWindow : public YerithWindowsCommons,
    						private Ui_YerithAlertesWindow,
							public YerithAbstractClassYerithSearchWindow
{
    Q_OBJECT

	enum TabIndex
    {
        TableauDesAlertes = 0,
        AfficherAlerteAuDetail = 1
    };

public:

    YERITH_CLASS_OPERATORS

	YerithAlertesWindow();

    virtual ~YerithAlertesWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_alertesWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_alertes_numero_page_derniere,
    												  label_alertes_numero_page_courante)

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);


public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_alertes)


    virtual void CHANGER_DE_LANGUE();


    virtual inline void ___retranslateUi()
    {
        retranslateUi(this);
    }


	virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Sélectionner une alerte dans le tableau des alertes, ensuite cliquer "
                                        "sur l'opération que vous souhaitez réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    void resetFilter(YerithSqlTableModel *alertesSqlTableModel);

    virtual void lister_les_elements_du_tableau(YerithSqlTableModel &alertesSqlTableModel);

    inline void lister_les_elements_du_tableau()
    {
        lister_les_elements_du_tableau(*_curAlertesSqlTableModel);
    }

    void afficher_alerte_selectioner(const QString &alerteDesignation);

    bool afficher_au_detail();

    void reinitialiser_recherche();

    void handle_visibilite_autres_elements(bool aBoolValue);


protected slots:

    void handleCurrentTabChanged(int index);

    void handleComboBoxClients_courriers_alertes_resolue_oui_OU_non(const QString &currentText);


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual inline void hideEvent(QHideEvent *hideEvent)
    {
        YerithWindowsCommons::CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
    }

    virtual void setupShortcuts();


protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

    virtual inline void disableImprimer()
    {
        actionAfficherPDF->setVisible(false);
    }

    virtual inline void enableImprimer()
    {
        actionAfficherPDF->setVisible(true);
    }


private slots:

    void afficher_alertes();

    void marquer_resolue();

    void supprimer();


private:

    void setCurrentUser_NOM_UTILISATEUR(const QString &aUserNomUtilisateur);

    void disable_NOM_UTILISATEUR();

    void enable_NOM_UTILISATEUR_ONLY_MANAGER();

    void clear_all_fields();

    void populateComboBoxes();

    void setupLineEdits();


    YerithLogger *_logger;

    YerithSqlTableModel *_curAlertesSqlTableModel;
};

#endif /* YERITH_ERP_ALERTES_WINDOW_HPP_ */
