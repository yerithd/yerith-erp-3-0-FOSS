/*
 * yerith-erp-tableau-des-transactions-du-client-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-tableau-des-transactions-du-client-window.hpp"


#include "src/process/yerith-erp-process.hpp"

#include "src/widgets/yerith-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


YerithTableauDesTransactionsDuClientWindow::YerithTableauDesTransactionsDuClientWindow()
:YerithWindowsCommons("yerith-erp-tableau-des-transactions-dun-client"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::CLIENTS),
 _CURRENT_PAGING_NR(1)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("transactions financières d'un client"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    enableResizing();

    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_tableau_des_transactions_du_client, false);


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
    		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
    		<< actiondiminuer_la_police_du_tableau;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_tableau_des_transactions_du_client)

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}")
								.arg(COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0);


    setupLineEdits();


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));


    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));


    pushButton_page_premiere->enable(this,
                                     SLOT(appliquer_bouton_PREMIERE_PAGE()));
    pushButton_page_precedente->enable(this,
                                       SLOT(appliquer_bouton_PRECEDENTE_PAGE
                                            ()));
    pushButton_page_suivante->enable(this,
                                     SLOT(appliquer_bouton_SUIVANTE_PAGE()));
    pushButton_page_derniere->enable(this,
                                     SLOT(appliquer_bouton_DERNIERE_PAGE()));


    setupShortcuts();

    setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING(0);
    setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING(1);
    setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING(2);
    setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING(5);
    setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING(6);
}


void YerithTableauDesTransactionsDuClientWindow::clear_current_client_financial_transactions()
{
    _CURRENT_PAGING_NR = 1;

    _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.clear();
}


void YerithTableauDesTransactionsDuClientWindow::
		listerTransactionsDunClient_WITH_ONE_LOCALISATION(QDate dateDebutTransactions,
														  QDate dateFinTransactions,
														  QString clientCompanyName,
														  QSqlQuery &sqlClientTransactionsUnionQuery)
{
	retranslateUi(this);

    disconnect(lineEdit_nombre_de_lignes_par_page, 0, 0, 0);

    connect(lineEdit_nombre_de_lignes_par_page,
            SIGNAL(textChanged(const QString &)),
            _allWindows->_payerAuCompteClientWindow,
            SLOT
            (PUBLIC_slot_afficher_les_transactions_dun_client_WITH_ONE_LOCALISATION
             ()));


    _curDateDebutTransactions = dateDebutTransactions;

    _curDateFinTransactions = dateFinTransactions;

    _clientCompanyName = clientCompanyName;

    QString curWindowTitle(QString("%1 - %2 - '%3'")
    						.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                                 QObject::tr("transactions financières d'un client"),
                                 clientCompanyName));

    setWindowTitle(curWindowTitle);

    tableView_tableau_des_transactions_du_client
		->lister_les_transactions_dun_client(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
                                       	     sqlClientTransactionsUnionQuery,
											 _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST);

    int tableViewSize = _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size();

    uint last_page_nr = (0 == tableViewSize ? 1 : tableViewSize);


//      QDEBUG_STRING_OUTPUT_2_N("tableViewSize", tableViewSize);
//
//      QDEBUG_STRING_OUTPUT_2_N("last_page_nr", last_page_nr);


    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));

    label_numero_page_derniere->setText(QString::number(last_page_nr));


    tableView_tableau_des_transactions_du_client
		->lister_les_transactions_dun_client
			(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
             _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));


    tableView_tableau_des_transactions_du_client->resizeColumnsToContents();

    if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
    {
        tableView_tableau_des_transactions_du_client->selectRow(0);
    }

    show();
}



void YerithTableauDesTransactionsDuClientWindow::
		listerTransactionsDunClient(QDate dateDebutTransactions,
                                    QDate dateFinTransactions,
                                    QString clientCompanyName,
                                    QSqlQuery &sqlClientTransactionsUnionQuery)
{
	retranslateUi(this);

    disconnect(lineEdit_nombre_de_lignes_par_page, 0, 0, 0);

    connect(lineEdit_nombre_de_lignes_par_page,
            SIGNAL(textChanged(const QString &)),
            _allWindows->_payerAuCompteClientWindow,
            SLOT(PUBLIC_slot_afficher_les_transactions_dun_client()));


    _curDateDebutTransactions = dateDebutTransactions;

    _curDateFinTransactions = dateFinTransactions;

    _clientCompanyName = clientCompanyName;

    QString curWindowTitle(QString("%1 - %2 - '%3'")
    						.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
    							 QObject::tr("transactions financières d'un client"),
								 clientCompanyName));

    setWindowTitle(curWindowTitle);

    tableView_tableau_des_transactions_du_client
		->lister_les_transactions_dun_client(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
                                       	     sqlClientTransactionsUnionQuery,
											 _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST);

    int tableViewSize = _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size();

    uint last_page_nr = (0 == tableViewSize ? 1 : tableViewSize);


//      QDEBUG_STRING_OUTPUT_2_N("tableViewSize", tableViewSize);
//
//      QDEBUG_STRING_OUTPUT_2_N("last_page_nr", last_page_nr);


    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));

    label_numero_page_derniere->setText(QString::number(last_page_nr));


    tableView_tableau_des_transactions_du_client
		->lister_les_transactions_dun_client
			(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
             _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));


    tableView_tableau_des_transactions_du_client->resizeColumnsToContents();

    if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
    {
        tableView_tableau_des_transactions_du_client->selectRow(0);
    }

    show();
}


bool YerithTableauDesTransactionsDuClientWindow::export_csv_file()
{
    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    QString aNewClientCompanyName =
                    YerithUtils::YERITH_REPLACE_ALL_EMPTYSPACES_WITH_UNDERSCORE_CHARACTER
                    (_clientCompanyName);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_tableau_des_transactions_du_client,
                                               tableColumnsToIgnore,
                                               QString
                                               ("yerith-erp-%1-client-transaction-csv-format").
                                               arg(aNewClientCompanyName),
                                               "client transaction");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_tableau_des_transactions_du_client,
                                               tableColumnsToIgnore,
                                               QString
                                               ("yerith-erp-transaction-client-%1-format-csv").
                                               arg(aNewClientCompanyName),
                                               "transactions d'un client");
    }

    return success;
}


bool YerithTableauDesTransactionsDuClientWindow::imprimer_pdf_document()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_transactions_dun_client_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_transactions_dun_client_tex;
    }

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHDEVISEFINANCIEREMONNAIE",
				QString("'%1'")
					.arg(YerithERPConfig::currency));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHCLIENT",
				YerithUtils::LATEX_IN_OUT_handleForeignAccents(_clientCompanyName));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESDEBUT", SET_DATE_TO_STRING(_curDateDebutTransactions));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESFIN",
                SET_DATE_TO_STRING(_curDateFinTransactions));

    return YerithWindowsCommons::imprimer_pdf_document();
}


void YerithTableauDesTransactionsDuClientWindow::textChangedSearchLineEditsQCompleters()
{
}


void YerithTableauDesTransactionsDuClientWindow::appliquer_bouton_PREMIERE_PAGE()
{
    _CURRENT_PAGING_NR = 1;

    tableView_tableau_des_transactions_du_client
		->lister_les_transactions_dun_client(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
                                       	     _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));

    tableView_tableau_des_transactions_du_client->resizeColumnsToContents();

    if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
    {
        tableView_tableau_des_transactions_du_client->selectRow(0);
    }

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerithTableauDesTransactionsDuClientWindow::appliquer_bouton_PRECEDENTE_PAGE()
{
    _CURRENT_PAGING_NR = _CURRENT_PAGING_NR - 1;

    if (_CURRENT_PAGING_NR <= 0)
    {
        _CURRENT_PAGING_NR = 1;
    }

    tableView_tableau_des_transactions_du_client
    	->lister_les_transactions_dun_client(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
                                       	     _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST
											 	 .value(_CURRENT_PAGING_NR));

    tableView_tableau_des_transactions_du_client->resizeColumnsToContents();


    if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
    {
        tableView_tableau_des_transactions_du_client->selectRow(0);
    }

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerithTableauDesTransactionsDuClientWindow::appliquer_bouton_SUIVANTE_PAGE()
{
    _CURRENT_PAGING_NR = _CURRENT_PAGING_NR + 1;

    if (_CURRENT_PAGING_NR > _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size())
    {
        _CURRENT_PAGING_NR = _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size();
    }

    if (_CURRENT_PAGING_NR <= 0)
    {
        _CURRENT_PAGING_NR = 1;
    }

    tableView_tableau_des_transactions_du_client->
    lister_les_transactions_dun_client(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
                                       _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.
                                       value(_CURRENT_PAGING_NR));

    tableView_tableau_des_transactions_du_client->resizeColumnsToContents();


    if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
    {
        tableView_tableau_des_transactions_du_client->selectRow(0);
    }

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerithTableauDesTransactionsDuClientWindow::appliquer_bouton_DERNIERE_PAGE()
{
    _CURRENT_PAGING_NR = _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size();

    if (_CURRENT_PAGING_NR <= 0)
    {
        _CURRENT_PAGING_NR = 1;
    }

    tableView_tableau_des_transactions_du_client->
    lister_les_transactions_dun_client(GET_ROW_NUMBER_PER_VIEWING_PAGE(),
                                       _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.
                                       value(_CURRENT_PAGING_NR));

    tableView_tableau_des_transactions_du_client->resizeColumnsToContents();


    if (tableView_tableau_des_transactions_du_client->rowCount() > 0)
    {
        tableView_tableau_des_transactions_du_client->selectRow(0);
    }

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerithTableauDesTransactionsDuClientWindow::setupShortcuts()
{
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
}


void YerithTableauDesTransactionsDuClientWindow::setupLineEdits()
{
    lineEdit_nombre_de_lignes_par_page
		->setValidator(&YerithUtils::UINT_VALIDATOR_GREATER_THAN_ZERO);

    lineEdit_nombre_de_lignes_par_page->setText(QString::number(9));
}
