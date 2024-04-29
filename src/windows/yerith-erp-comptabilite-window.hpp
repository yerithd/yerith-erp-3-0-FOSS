/*
 * yerith-erp-comptabilite-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_COMPTES_BANCAIRES_WINDOW_HPP_
#define SRC_YERITH_COMPTES_BANCAIRES_WINDOW_HPP_

#include "../../ui_yerith-erp-comptabilite-window.h"

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "yerith-erp-qtable-widget-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>


class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;
class YerithUtils;
class YerithWindowsCommons;
class YerithPOSUser;

class YerithComptabiliteWindow : public YerithQTableWidgetWindowsCommons,
    							 private Ui_YerithComptabiliteWindow,
								 public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS


	YerithComptabiliteWindow();


    virtual ~YerithComptabiliteWindow();


    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_comptabilite_Window;
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


    virtual inline void definirAdministrateur()
    {
        definirPasDeRole();
    }


    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }


    virtual void definirPasDeRole();


    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_comptabilite_numero_page_derniere,
    												  label_comptabilite_numero_page_courante)


    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);


public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_comptes_doperations_comptables)


	virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }


    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("1. CRÉEZ des lignes budgetaires qui apparaîtront"
                                        " en tant que comptes d'opérations commerciales et comptables !\n\n"
                                        "2. Vous avez accès aux informations sur tous les"
                                        " comptes bancaires enregistrés dans YERITH-ERP-3.0,"
                                        " ainsi que toutes les transactions financières"
                                        " éffectuées sur ses comptes bancaires !"));
    }


    bool export_csv_file();


    virtual QString get_latex_template_print_pdf_content();


    virtual void deconnecter_utilisateur();


    void afficher_ASSOCIATIONS_OperationsCommerciales__compte_bancaires();


    void afficher_comptes_bancaires(YerithSqlTableModel &aYerithSqlTableModel);


    inline void afficher_comptes_bancaires()
    {
        textChangedSearchLineEditsQCompleters();
    }


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();


protected slots:

    virtual bool imprimer_UNE_LISTE(bool *full_list = 0);


    double GET_Human_RESOURCE_EXPENSE_Amount();


    double GET_FINANCIAL_EXPENSE_Amount();


    double GET_asset_PURCHASE_Amount();


    double GET_merchandise_PURCHASE_Amount();


    double GET_sales_cash_AND_sales_client_account_Amount();


    bool ON_CLICKED_Show_paid_in_paid_out_VALUES_amount(int a_modelIndex_ROW);


    inline bool ON_CLICKED_Show_paid_in_paid_out_VALUES_amount(const QModelIndex &modelIndex)
    {
        ON_CLICKED_Show_paid_in_paid_out_VALUES_amount(modelIndex.row());
    }


    bool ON_CLICKED_SELECT_COMMERCIAL_OPERATION_TYPE(const QModelIndex &modelIndex);


    inline void EXECUTER_DB_SLOT_Dissocier_TypeDoperation__CompteDoperation()
    {
        YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK
            (this,
             &YerithComptabiliteWindow::SLOT_Dissocier_TypeDoperation__CompteDoperation);
    }


    inline void EXECUTER_DB_SLOT_associer_TypeDoperation__CompteDoperation()
    {
        YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK
            (this,
             &YerithComptabiliteWindow::SLOT_associer_TypeDoperation__CompteDoperation);
    }


    QString SLOT_get_Budget_Line_SALES_and_SALES_client_account(int *code_IN_db_OUT = 0);


    virtual void SLOT_calculer_TOUTES_les_lignes_budgetaires();

    virtual bool SLOT_Dissocier_TypeDoperation__CompteDoperation();

    virtual bool SLOT_associer_TypeDoperation__CompteDoperation();

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


private slots:

    void disable_yerith_widgets();

    void enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();


    void handle_Enable_Disable_BUTTONS();


    void handle_filtres_checkBox(int state);

    void handle_change_tab(int);

    void set_filtrer_font();

    void refineYerithLineEdits();


private:

    enum FINANCIAL_ACCOUNTING_WINDOW_TAB
	{
    	FINANCIAL_Operation_Type__Financial_Operation_Type_Account = 0,
    	BUGET_Line__Operation_Type_Account = 1
	};


    void get_PRINT_OUT_TexTableString_LOCAL(QString   &texTable_IN_OUT,
                                            int        row_MAX_TO_GO_export = -1);

    void updateComboBoxes();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();


    YerithLogger        *_logger;

    YerithSqlTableModel *_curCompte_Lignes_Budgetaires_SqlTableModel;

    QFont               *_pushButton_comptabilite_filtrer_font;

};

#endif /* SRC_YERITH_COMPTES_BANCAIRES_WINDOW_HPP_ */
