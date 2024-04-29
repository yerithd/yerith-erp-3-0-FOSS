/*
 * yerith-erp-tableau-des-transactions-du-fournisseur-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-tableau-des-transactions-du-fournisseur-window.hpp"


#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/widgets/yerith-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


YerithTableauDesTransactionsDuFournisseurWindow::YerithTableauDesTransactionsDuFournisseurWindow()
:YerithWindowsCommons("yerith-erp-tableau-des-transactions-dun-fournisseur")
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("transactions financières d'un fournisseur"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    enableResizing();

    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_tableau_des_transactions_du_fournisseur,
    										false);

    _DBFieldNamesToPrintLeftAligned.insert(0);
    _DBFieldNamesToPrintLeftAligned.insert(1);
    _DBFieldNamesToPrintLeftAligned.insert(2);
    _DBFieldNamesToPrintLeftAligned.insert(5);
    _DBFieldNamesToPrintLeftAligned.insert(6);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0);


    connect(actionExporter_au_format_csv,
    		SIGNAL(triggered()),
			this,
            SLOT(export_csv_file()));


    connect(action_parametrer_les_impressions,
    		SIGNAL(triggered()),
			this,
            SLOT(setup_print()));


    connect(actionAfficherPDF,
    		SIGNAL(triggered()),
    		this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));


    setupShortcuts();
}


void YerithTableauDesTransactionsDuFournisseurWindow::
		listerTransactionsDunFournisseur(QDate 		dateDebutTransactions,
                                 	 	 QDate 		dateFinTransactions,
										 QString 	fournisseurCompanyName,
										 QSqlQuery 	&sqlFournisseurTransactionsUnionQuery)
{
	retranslateUi(this);

    _curDateDebutTransactions = dateDebutTransactions;

    _curDateFinTransactions = dateFinTransactions;

    _fournisseurCompanyName = fournisseurCompanyName;

    QString curWindowTitle =
    			QString("%1 - %2 - '%3'")
					.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
						 QObject::tr("transactions d'un fournisseur"),
						 fournisseurCompanyName);

    setWindowTitle(curWindowTitle);

    tableView_tableau_des_transactions_du_fournisseur
		->lister_les_transactions_dun_fournisseur(sqlFournisseurTransactionsUnionQuery);

    tableView_tableau_des_transactions_du_fournisseur->resizeColumnsToContents();

    if (tableView_tableau_des_transactions_du_fournisseur->rowCount() > 0)
    {
        tableView_tableau_des_transactions_du_fournisseur->selectRow(0);
    }

    show();
}


bool YerithTableauDesTransactionsDuFournisseurWindow::export_csv_file()
{
    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    QString aNewFournisseurCompanyName =
    		YerithUtils::YERITH_REPLACE_ALL_EMPTYSPACES_WITH_UNDERSCORE_CHARACTER(_fournisseurCompanyName);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_tableau_des_transactions_du_fournisseur,
                                               tableColumnsToIgnore,
                                               QString("yerith-erp-%1-supplier-transaction-csv-format")
                                               	   .arg(aNewFournisseurCompanyName),
                                               "client transaction");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_tableau_des_transactions_du_fournisseur,
                                               tableColumnsToIgnore,
                                               QString("yerith-erp-transaction-fournisseur-%1-format-csv")
                                               	   .arg(aNewFournisseurCompanyName),
                                               "transactions d'un client");
    }

    return success;
}


bool YerithTableauDesTransactionsDuFournisseurWindow::imprimer_pdf_document()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_transactions_dun_fournisseur_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_transactions_dun_fournisseur_tex;
    }

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHFOURNISSEUR",
				YerithUtils::LATEX_IN_OUT_handleForeignAccents(_fournisseurCompanyName));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESDEBUT",
				SET_DATE_TO_STRING(_curDateDebutTransactions));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESFIN",
                SET_DATE_TO_STRING(_curDateFinTransactions));


    return YerithWindowsCommons::imprimer_pdf_document();
}


void YerithTableauDesTransactionsDuFournisseurWindow::setupShortcuts()
{
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
}
