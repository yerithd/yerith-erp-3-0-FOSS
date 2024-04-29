/*
 * yerith-erp-tableau-des-transactions-du-fournisseur-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_COMPTES_CLIENTS_TRANSACTIONS_DUN_FOURNISSEUR_WINDOW_HPP_
#define SRC_YERITH_COMPTES_CLIENTS_TRANSACTIONS_DUN_FOURNISSEUR_WINDOW_HPP_

#include "../../../ui_yerith-erp-tableau-des-transactions-du-fournisseur-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


class YerithTableauDesTransactionsDuFournisseurWindow : public YerithWindowsCommons,
    													private Ui_YerithTableauDesTransactionsDuFournisseurWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithTableauDesTransactionsDuFournisseurWindow();

    inline ~YerithTableauDesTransactionsDuFournisseurWindow()
    {
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_tableauDesTransactionDuFournisseur;
    }

    void listerTransactionsDunFournisseur(QDate 	dateDebutTransactions,
                                          QDate 	dateFinTransactions,
                                          QString 	fournisseurCompanyName,
                                          QSqlQuery &sqlFournisseurTransactionsUnionQuery);

public slots:

    bool export_csv_file();

    virtual bool imprimer_pdf_document();

protected:

    virtual void setupShortcuts();

    inline void virtual resizeEvent(QResizeEvent *event)
    {
        tableView_tableau_des_transactions_du_fournisseur->resize(event->
                                                                  size());
    }

private:


    QDate _curDateDebutTransactions;

    QDate _curDateFinTransactions;

    QString _fournisseurCompanyName;
};

#endif /* SRC_YERITH_COMPTES_CLIENTS_TRANSACTIONS_DUN_FOURNISSEUR_WINDOW_HPP_ */
