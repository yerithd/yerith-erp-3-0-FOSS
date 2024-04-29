/*
 * yerith-erp-tableau-des-transactions-du-client-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_
#define SRC_YERITH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_

#include "../../../ui_yerith-erp-tableau-des-transactions-du-client-window.h"

#include "src/yerith-erp-windows.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"


class YerithTableauDesTransactionsDuClientWindow : public YerithWindowsCommons,
    											   private Ui_YerithTableauDesTransactionsDuClientWindow,
												   public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithTableauDesTransactionsDuClientWindow();

    inline ~YerithTableauDesTransactionsDuClientWindow()
    {
        clear_current_client_financial_transactions();
    }

    void clear_current_client_financial_transactions();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_tableauDesTransactionDuClient;
    }

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_numero_page_derniere,
    												  label_numero_page_courante)

    inline uint GET_ROW_NUMBER_PER_VIEWING_PAGE()
    {
        return lineEdit_nombre_de_lignes_par_page->text().toUInt();
    }

    void listerTransactionsDunClient_WITH_ONE_LOCALISATION(QDate 		dateDebutTransactions,
                                                           QDate 		dateFinTransactions,
                                                           QString 		clientCompanyName,
                                                           QSqlQuery 	&sqlClientTransactionsUnionQuery);

    void listerTransactionsDunClient(QDate 		dateDebutTransactions,
                                     QDate 		dateFinTransactions,
                                     QString 	clientCompanyName,
                                     QSqlQuery 	&sqlClientTransactionsUnionQuery);

public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_tableau_des_transactions_du_client)

	bool export_csv_file();

    virtual bool imprimer_pdf_document();

protected slots:

    virtual void textChangedSearchLineEditsQCompleters();

private slots:

    void appliquer_bouton_PREMIERE_PAGE();

    void appliquer_bouton_PRECEDENTE_PAGE();

    void appliquer_bouton_SUIVANTE_PAGE();

    void appliquer_bouton_DERNIERE_PAGE();


protected:

    virtual void setupShortcuts();


private:

    void setupLineEdits();


    uint _CURRENT_PAGING_NR;

    QMap<uint, QString> _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST;


    QDate _curDateDebutTransactions;

    QDate _curDateFinTransactions;

    QString _clientCompanyName;
};

#endif /* SRC_YERITH_COMPTES_CLIENTS_TRANSACTIONS_DUN_CLIENT_WINDOW_HPP_ */
