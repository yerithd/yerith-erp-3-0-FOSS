/*
 * yerith-erp-3-0.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#ifdef YERITH_SERVER

	#include "src/dbus/YERITH_RUNTIME_VERIFIER.hpp"

#endif /* YERITH_SERVER */

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QStringList>

#include "src/users/yerith-erp-user-manager.hpp"

#include "src/utils/yerith-erp-command-line-parser.hpp"

#include "src/utils/virtual-keyboard/yerith-erp-key-emitter.hpp"

#include "src/yerith-erp-windows.hpp"


#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

#include "src/tests/utils/yerith-erp-test-yerith-erp-utils.hpp"

#include "src/tests/yerith-erp-test-yerith-erp-windows.hpp"

#include "src/tests/yerith-erp-test-yerith-erp-table-view.hpp"

#endif


#include "src/utils/yerith-erp-command-line-parser.hpp"
#include "src/utils/yerith-erp-config.hpp"
#include "src/utils/yerith-erp-utils.hpp"
#include "src/utils/yerith-erp-database.hpp"
#include "src/utils/yerith-erp-logger.hpp"
#include "src/process/yerith-erp-process-info.hpp"

#include "src/users/yerith-erp-users.hpp"

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QtCore/QTranslator>
#include <QtCore/QList>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QCryptographicHash>
#include <QtCore/QLocale>

#include <QtCore/QTranslator>

#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QStylePlugin>

#include <QtNetwork/QNetworkInterface>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>

#include <cstdlib>

#ifdef YERITH_CHECK_MEMORY
	#include <mcheck.h>
#endif

class YerithMain
{
public:

    inline YerithMain(YerithLogger *logger)
    {
        _logger = logger;
    }

    inline ~YerithMain()
    {
    }

private:

    YerithLogger *_logger;
};


void readTexTemplateFiles(YerithLogger &logger)
{
    logger.log("main", "readTexTemplateFiles");


    {
    	// FR_template_Comptabilite_OPERATIONS_TYPES.tex
    	QFile file_FR_template_Comptabilite_comptes_Doperations(FILE_YERITH_ERP_3_0_TEMPLATE_COMPTABILITE_OPERATIONS_TYPES_FR);

    	YerithUtils::YERITH_READ_FILE_CONTENT(file_FR_template_Comptabilite_comptes_Doperations,
    			YerithUtils::FR_template_Comptabilite_comptes_Doperations_TEX);

    	// EN_template_Comptabilite_OPERATIONS_TYPES.tex
    	QFile file_EN_template_Comptabilite_comptes_Doperations(FILE_YERITH_ERP_3_0_TEMPLATE_COMPTABILITE_OPERATIONS_TYPES_EN);

    	YerithUtils::YERITH_READ_FILE_CONTENT(file_EN_template_Comptabilite_comptes_Doperations,
    			YerithUtils::EN_template_Comptabilite_comptes_Doperations_TEX);
    }

    {
    	// FR_template-lister-admin-objects.tex
    	QFile file_FR_template_bon_de_commande(FILE_YERITH_ERP_3_0_TEMPLATE_PURCHASE_ORDER_SHEET_FR);

    	YerithUtils::YERITH_READ_FILE_CONTENT(file_FR_template_bon_de_commande,
    			YerithUtils::FR_template_bon_de_commande_TEX);

    	// EN_template-lister-admin-objects.tex
    	QFile file_EN_template_bon_de_commande(FILE_YERITH_ERP_3_0_TEMPLATE_PURCHASE_ORDER_SHEET_EN);

    	YerithUtils::YERITH_READ_FILE_CONTENT(file_EN_template_bon_de_commande,
    			YerithUtils::EN_template_bon_de_commande_TEX);
    }

    {
    	// FR_template-lister-admin-objects.tex
    	QFile file_FR_template_lister_admin_objects(FILE_YERITH_ERP_3_0_TEMPLATE_LISTE_ADMIN_OBJECTS_FR);

    	YerithUtils::YERITH_READ_FILE_CONTENT(file_FR_template_lister_admin_objects,
    			YerithUtils::FR_template_lister_admin_objects_TEX);

    	// EN_template-lister-admin-objects.tex
    	QFile file_EN_template_lister_admin_objects(FILE_YERITH_ERP_3_0_TEMPLATE_LISTE_ADMIN_OBJECTS_EN);

    	YerithUtils::YERITH_READ_FILE_CONTENT(file_EN_template_lister_admin_objects,
    			YerithUtils::EN_template_lister_admin_objects_TEX);
    }

    {
    	// FR_template_employee_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_GroupTemplateFR(FILE_YERITH_ERP_3_0_TEMPLATE_EMPLOYEES_GROUPS_HR_FR);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileEMPLOYEE_GroupTemplateFR,
    			YerithUtils::FR_template_EMPLOYEE_GROUP_LISTING_tex);

    	// EN_template_employee_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_GroupTemplateEN(FILE_YERITH_ERP_3_0_TEMPLATE_EMPLOYEES_GROUPS_HR_EN);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileEMPLOYEE_GroupTemplateEN,
    			YerithUtils::EN_template_EMPLOYEE_GROUP_LISTING_tex);
    }

    {
    	// FR_template_employee_PAY_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_pay_GroupTemplateFR(FILE_YERITH_ERP_3_0_TEMPLATE_PAY_GROUP_HR_FR);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileEMPLOYEE_pay_GroupTemplateFR,
    			YerithUtils::FR_template_employee_PAY_GROUP_LISTING_tex);

    	// EN_template_employee_PAY_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_pay_GroupTemplateEN(FILE_YERITH_ERP_3_0_TEMPLATE_PAY_GROUP_HR_EN);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileEMPLOYEE_pay_GroupTemplateEN,
    			YerithUtils::EN_template_employee_PAY_GROUP_LISTING_tex);
    }

    {
    	// FR_template_GROUPES_DUN_CLIENT_tex
    	QFile fileCLIENTGroupTemplateFR(FILE_YERITH_ERP_3_0_TEMPLATE_CLIENT_GROUP_HR_FR);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCLIENTGroupTemplateFR,
    			YerithUtils::FR_template_GROUPES_DUN_CLIENT_tex);

    	// EN_template_GROUPES_DUN_CLIENT_tex
    	QFile fileCLIENTGroupTemplateEN(FILE_YERITH_ERP_3_0_TEMPLATE_CLIENT_GROUP_HR_EN);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCLIENTGroupTemplateEN,
    			YerithUtils::EN_template_GROUPES_DUN_CLIENT_tex);
    }

    {
    	// FR_template_GROUPES_DUN_EMPLOYE_tex
    	QFile fileEmployeeGroupTemplateFR(FILE_YERITH_ERP_3_0_TEMPLATE_EMPLOYEE_GROUP_HR_FR);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileEmployeeGroupTemplateFR,
    			YerithUtils::FR_template_GROUPES_DUN_EMPLOYE_tex);

    	// EN_template_GROUPES_DUN_EMPLOYE_tex
    	QFile fileEmployeeGroupTemplateEN(FILE_YERITH_ERP_3_0_TEMPLATE_EMPLOYEE_GROUP_HR_EN);

    	YerithUtils::YERITH_READ_FILE_CONTENT(fileEmployeeGroupTemplateEN,
    			YerithUtils::EN_template_GROUPES_DUN_EMPLOYE_tex);
    }

    {
    	QFile fileFinancialAccountingReportDataTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_FINANCIAL_ACCOUNTING_REPORT_DATA_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileFinancialAccountingReportDataTemplate,
    										  YerithUtils::FR_template_bilan_comptable_tex);

    	QFile fileFinancialAccountingReportDataENTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_FINANCIAL_ACCOUNTING_REPORT_DATA_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileFinancialAccountingReportDataENTemplate,
    										  YerithUtils::EN_template_bilan_comptable_tex);
    }

    {
    	QFile fileClientLOYALTY_GROUP_LISTING_FR(FILE_YERITH_CLIENT_LOYALTY_GROUPS_LISTING_TEMPLATE_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileClientLOYALTY_GROUP_LISTING_FR,
    										  YerithUtils::FR_template_CLIENT_LOYALTY_GROUP_LISTING_tex);

    	QFile fileClientLOYALTY_GROUP_LISTING_EN(FILE_YERITH_CLIENT_LOYALTY_GROUPS_LISTING_TEMPLATE_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileClientLOYALTY_GROUP_LISTING_EN,
    										  YerithUtils::EN_template_CLIENT_LOYALTY_GROUP_LISTING_tex);
    }

    {
    	QFile fileClientLOYALTYprogramLISTING_FR(FILE_YERITH_CLIENT_LOYALTY_PROGRAM_LISTING_TEMPLATE_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileClientLOYALTYprogramLISTING_FR,
    										  YerithUtils::FR_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex);

    	QFile fileClientLOYALTYprogramLISTING_EN(FILE_YERITH_CLIENT_LOYALTY_PROGRAM_LISTING_TEMPLATE_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileClientLOYALTYprogramLISTING_EN,
    										  YerithUtils::EN_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex);
    }

    {
    	QFile fileClientLOYALTYprogramTemplateFR(FILE_YERITH_LOYALTY_MEMBERSHIP_CARD_TEMPLATE_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileClientLOYALTYprogramTemplateFR,
    										   YerithUtils::FR_template_carte_de_fidelite_client_TEMPLATE_tex);

    	QFile fileClientLOYALTYprogramTemplateEN(FILE_YERITH_LOYALTY_MEMBERSHIP_CARD_TEMPLATE_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileClientLOYALTYprogramTemplateEN,
    										  YerithUtils::EN_template_carte_de_fidelite_client_TEMPLATE_tex);
    }

    {
    	QFile fileSupplierDataTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_SUPPLIER_DATA_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileSupplierDataTemplate,
    										  YerithUtils::FR_template_fiche_dun_fournisseur_tex);

    	QFile fileSupplierDataENTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_SUPPLIER_DATA_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileSupplierDataENTemplate,
    										  YerithUtils::EN_template_fiche_dun_fournisseur_tex);
    }

    {
    	QFile fileCustomerDataTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_CUSTOMER_DATA_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCustomerDataTemplate,
    										  YerithUtils::FR_template_fiche_client_tex);

    	QFile fileCustomerDataENTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_CUSTOMER_DATA_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCustomerDataENTemplate,
    										  YerithUtils::EN_template_fiche_client_tex);
    }

    {
    	QFile barDiagTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_BAR_DIAG_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(barDiagTemplate,
    										  YerithUtils::FR_bar_diag_tex);

    	QFile barDiagENTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_BAR_DIAG_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(barDiagENTemplate,
    										  YerithUtils::EN_bar_diag_tex);
    }

    {
    	QFile barChartTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_BAR_CHART_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(barChartTemplate,
    										  YerithUtils::FR_bar_chart_tex);

    	QFile barChartENTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_BAR_CHART_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(barChartENTemplate,
    										  YerithUtils::EN_bar_chart_tex);
    }

    {
    	QFile ZERO_ventes_Template(FILE_YERITH_ERP_3_0_TEMPLATE_ZERO_VENTES_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(ZERO_ventes_Template,
    										  YerithUtils::FR_ZERO_ventes_tex);

    	QFile ZERO_ventes_ENTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_ZERO_VENTES_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(ZERO_ventes_ENTemplate,
    										  YerithUtils::EN_ZERO_ventes_tex);
    }

    {
    	QFile pieChartTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_PIE_CHART_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(pieChartTemplate,
    										  YerithUtils::FR_pie_chart_tex);

    	QFile pieChartENTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_PIE_CHART_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(pieChartENTemplate,
    										  YerithUtils::EN_pie_chart_tex);
    }

    {
    	QFile fileFactureSmall_fr(FILE_YERITH_ERP_3_0_TEMPLATE_RECEIPT_SMALL_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileFactureSmall_fr,
    										  YerithUtils::FR_template_facture_petit_tex);

    	QFile fileFactureSmall_en(FILE_YERITH_ERP_3_0_TEMPLATE_RECEIPT_SMALL_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileFactureSmall_en,
    										  YerithUtils::EN_template_facture_petit_tex);
    }

    {
    	QFile file_FACTURE_grand_Fr(FILE_YERITH_ERP_3_0_TEMPLATE_INVOICE_LARGE_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_FACTURE_grand_Fr,
    										  YerithUtils::FR_template_FACTURE_GRAND_tex);

    	QFile file_FACTURE_grand_EN(FILE_YERITH_ERP_3_0_TEMPLATE_INVOICE_LARGE_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_FACTURE_grand_EN,
    										  YerithUtils::EN_template_FACTURE_GRAND_tex);
    }

    {
    	QFile file_three_D_bardiag_Fr(FILE_YERITH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_three_D_bardiag_Fr,
    										  YerithUtils::FR_template_THREE_D_bardiag_tex);

    	QFile file_three_D_bardiag_En(FILE_YERITH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_three_D_bardiag_En,
    										  YerithUtils::EN_template_THREE_D_bardiag_tex);
    }

    {
    	QFile file_RECU_grand_Fr(FILE_YERITH_ERP_3_0_TEMPLATE_RECEIPT_LARGE_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_RECU_grand_Fr,
    										  YerithUtils::FR_template_RECU_GRAND_tex);

    	QFile file_RECU_grand_En(FILE_YERITH_ERP_3_0_TEMPLATE_RECEIPT_LARGE_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_RECU_grand_En,
    										  YerithUtils::EN_template_RECU_GRAND_tex);
    }

    {
    	QFile fileStockHistory(FILE_YERITH_ERP_3_0_TEMPLATE_HISTORIQUE_DUN_STOCK_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileStockHistory,
    										  YerithUtils::FR_template_historique_dun_stock_tex);

    	QFile fileStockHistoryEN(FILE_YERITH_ERP_3_0_TEMPLATE_HISTORIQUE_DUN_STOCK_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileStockHistoryEN,
    										  YerithUtils::EN_template_historique_dun_stock_tex);
    }

    {

    	QFile filePayments(FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_PAYMENTS_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(filePayments,
    										  YerithUtils::FR_template_journal_des_paiements_tex);

    	QFile filePaymentsEn(FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_PAYMENTS_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(filePaymentsEn,
    										  YerithUtils::EN_template_journal_des_paiements_tex);
    }

    {
    	QFile fileLandscape(FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_SALES_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileLandscape,
    										  YerithUtils::FR_template_journal_des_ventes_tex);

    	QFile fileLandscapeEn(FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_SALES_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileLandscapeEn,
    										  YerithUtils::EN_template_journal_des_ventes_tex);
    }

    {
    	QFile fileLandscapeTransactions(FILE_YERITH_ERP_3_0_TEMPLATE_MOUVEMENTSDESTOCKS_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileLandscapeTransactions,
    										  YerithUtils::FR_template_liste_des_mouvements_de_stocks_tex);

    	QFile fileLandscapeTransactionsEN(FILE_YERITH_ERP_3_0_TEMPLATE_MOUVEMENTSDESTOCKS_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileLandscapeTransactionsEN,
    										  YerithUtils::EN_template_liste_des_mouvements_de_stocks_tex);
    }

    {
    	QFile fileSupplierTransactionListingFr(FILE_YERITH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_FOURNISSEUR_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileSupplierTransactionListingFr,
    										  YerithUtils::FR_template_transactions_dun_fournisseur_tex);

    	QFile fileSupplierTransactionListingEn(FILE_YERITH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_FOURNISSEUR_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileSupplierTransactionListingEn,
    										  YerithUtils::EN_template_transactions_dun_fournisseur_tex);
    }

    {
    	QFile fileCustomerTransactionListingFr(FILE_YERITH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCustomerTransactionListingFr,
    										  YerithUtils::FR_template_transactions_dun_client_tex);

    	QFile fileCustomerTransactionListingEn(FILE_YERITH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCustomerTransactionListingEn,
    										  YerithUtils::EN_template_transactions_dun_client_tex);
    }

    {
    	QFile fileSupplierListingFr(FILE_YERITH_ERP_3_0_TEMPLATE_FOURNISSEURS_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileSupplierListingFr,
    										  YerithUtils::FR_template_fournisseurs_tex);

    	QFile fileSupplierListingEn(FILE_YERITH_ERP_3_0_TEMPLATE_FOURNISSEURS_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileSupplierListingEn,
    										  YerithUtils::EN_template_fournisseurs_tex);
    }

    {
    	QFile fileCustomerAccountListingFr(FILE_YERITH_ERP_3_0_TEMPLATE_COMPTES_CLIENTS_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCustomerAccountListingFr,
    										  YerithUtils::FR_template_comptes_clients_tex);

    	QFile fileCustomerAccountListingEn(FILE_YERITH_ERP_3_0_TEMPLATE_COMPTES_CLIENTS_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileCustomerAccountListingEn,
    										  YerithUtils::EN_template_comptes_clients_tex);
    }

    {
    	QFile fileStockListingFr(FILE_YERITH_ERP_3_0_TEMPLATE_MARCHANDISES_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileStockListingFr,
    										  YerithUtils::FR_template_marchandises_tex);

    	QFile fileStockListingEn(FILE_YERITH_ERP_3_0_TEMPLATE_MARCHANDISES_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileStockListingEn,
    										  YerithUtils::EN_template_marchandises_tex);
    }

    {
    	QFile fileListeAlertesFr(FILE_YERITH_ERP_3_0_TEMPLATE_LISTE_ALERTES_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileListeAlertesFr,
    										  YerithUtils::FR_template_lister_alertes_tex);

    	QFile fileListeAlertesEn(FILE_YERITH_ERP_3_0_TEMPLATE_LISTE_ALERTES_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileListeAlertesEn,
    										  YerithUtils::EN_template_lister_alertes_tex);
    }

    {
    	QFile fileListe_CHARGES_FINANCIERES_FR(FILE_YERITH_ERP_3_0_TEMPLATE_CHARGES_FINANCIERES_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileListe_CHARGES_FINANCIERES_FR,
    										  YerithUtils::FR_template_CHARGES_FINANCIERES_tex);

    	QFile fileListe_FINANCIAL_EXPENSES_EN(FILE_YERITH_ERP_3_0_TEMPLATE_CHARGES_FINANCIERES_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileListe_FINANCIAL_EXPENSES_EN,
    										  YerithUtils::EN_template_CHARGES_FINANCIERES_tex);
    }

    {
    	QFile fileListeAchatsFr(FILE_YERITH_ERP_3_0_TEMPLATE_LISTE_ACHATS_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileListeAchatsFr,
    										  YerithUtils::FR_template_lister_achats_tex);

    	QFile fileListeAchatsEn(FILE_YERITH_ERP_3_0_TEMPLATE_LISTE_ACHATS_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileListeAchatsEn,
    										   YerithUtils::EN_template_lister_achats_tex);
    }

    {
    	QFile file_comptes_doperation_comptables_Fr(FILE_YERITH_ERP_3_0_TEMPLATE_COMPTABILITE_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_comptes_doperation_comptables_Fr,
											  YerithUtils::FR_template_comptabilite_tex);

    	QFile file_comptes_doperation_comptables_En(FILE_YERITH_ERP_3_0_TEMPLATE_COMPTABILITE_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(file_comptes_doperation_comptables_En,
											  YerithUtils::EN_template_comptabilite_tex);
    }

    {
    	QFile yerith_erp_3D_BARDIAG_TEMPLATEFr(FILE_YERITH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG);
    	YerithUtils::YERITH_READ_FILE_CONTENT(yerith_erp_3D_BARDIAG_TEMPLATEFr,
    										  YerithUtils::FR_template_yerith_erp_3D_BARDIAG_TEMPLATE_tex);

    	QFile yerith_erp_3D_BARDIAG_TEMPLATEEn(FILE_YERITH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG);
    	YerithUtils::YERITH_READ_FILE_CONTENT(yerith_erp_3D_BARDIAG_TEMPLATEEn,
    										  YerithUtils::EN_template_yerith_erp_3D_BARDIAG_TEMPLATE_tex);
    }

    {
    	QFile fileStockDeSecuriteFr(FILE_YERITH_ERP_3_0_TEMPLATE_FICHE_STOCKS_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileStockDeSecuriteFr,
    										  YerithUtils::FR_template_lister_stock_tex);

    	QFile fileStockDeSecuriteEn(FILE_YERITH_ERP_3_0_TEMPLATE_FICHE_STOCKS_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileStockDeSecuriteEn,
    										  YerithUtils::EN_template_lister_stock_tex);
    }

    {
    	QFile fileTransfertsTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_SMALL_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileTransfertsTemplate,
    										  YerithUtils::FR_template_sortie_des_stocks_petit_tex);

    	QFile fileTransfertsTemplateEn(FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_SMALL_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileTransfertsTemplateEn,
    										  YerithUtils::EN_template_sortie_des_stocks_petit_tex);
    }

    {
    	QFile fileTransfertsBigTemplate(FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_LARGE_FR);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileTransfertsBigTemplate,
    										  YerithUtils::FR_template_sortie_des_stocks_grand_tex);

    	QFile fileTransfertsBigTemplateEn (FILE_YERITH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_LARGE_EN);
    	YerithUtils::YERITH_READ_FILE_CONTENT(fileTransfertsBigTemplateEn,
    										  YerithUtils::EN_template_sortie_des_stocks_grand_tex);
    }

    {
    	QFile barDiagTemplate2(FILE_YERITH_ERP_3_0_TEMPLATE_1A_TEX);
    	YerithUtils::YERITH_READ_FILE_CONTENT(barDiagTemplate2,
    										  YerithUtils::_1a_tex);
    }
}


void read_system_local_yerith_configuration()
{
    QFile
    file
    (YerithERPConfig::
     FILE_ABSOLUTEPATH_YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE);

    if (!file.open(QFile::ReadOnly))
    {
        return;
    }

    QTextStream init_cfg(&file);
    QString aValue;
    QString line;
    QStringList list;

    do
    {
        line = init_cfg.readLine();

        list =
                        line.split(YERITH_ERP_3_0_CONFIGURATION_FILE_SEPARATION_OPERATOR);

//              QDEBUG_STRING_OUTPUT_2("line", line);

        if (YerithUtils::isEqualCaseInsensitive
                ("local_parameter_full_path_pdf_reader", list.at(0)))
        {
            YerithERPConfig::pathToPdfReader = list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_full_path_pdf_latex_executable_root_dir",
                  list.at(0)))
        {
            YerithERPConfig::pathToLatexSystemRootFolder =
                            list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_full_path_mariadb_client_executables_root_dir",
                  list.at(0)))
        {
            YerithERPConfig::pathToMARIA_DB_BASE_DIR = list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_full_path_file_temporary_folder",
                  list.at(0)))
        {
            YerithERPConfig::temporaryFilesDir = list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_file_system_device_thermal_printer",
                  list.at(0)))
        {
            YerithERPConfig::pathToThermalPrinterDeviceFile =
                            list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_type_receipt", list.at(0)))
        {
            YerithERPConfig::receiptType = list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_IP_address_network_thermal_printer",
                  list.at(0)))
        {
            YerithERPConfig::IP_ADDRESS_NETWORK_printer =
                            list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_thermal_printer", list.at(0)))
        {
            YerithERPConfig::printer = list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_SHOW_DATABASE_RAW_table_column", list.at(0)))
        {
            aValue = list.at(1).trimmed();

            if (aValue.toInt() == YerithERPConfig::HIDE_DATABASE_RAW_table_column)
            {
                YerithERPConfig::SHOW_DATABASE_RAW = false;
            }
            else
            {
                YerithERPConfig::SHOW_DATABASE_RAW = true;
            }
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_state_open_cash_register", list.at(0)))
        {
            aValue = list.at(1).trimmed();

            if (aValue.toInt() == YerithERPConfig::CLOSE_CASH_DRAWER)
            {
                YerithERPConfig::ouvrirRegistreDeCaisse = false;
            }
            else
            {
                YerithERPConfig::ouvrirRegistreDeCaisse = true;
            }
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_format_receipt", list.at(0)))
        {
            YerithERPConfig::receiptFormat = list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_starting_year_business_dashboard",
                  list.at(0)))
        {
            YerithERPConfig::annee_depart_rapports_chiffre_affaire_value =
                            list.at(1).trimmed();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_length_line_feed_thermal_print_character_count",
                  list.at(0)))
        {
            YerithERPConfig::
            LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING =
                            list.at(1).trimmed().toInt();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_length_maximal_display_string", list.at(0)))
        {
            YerithERPConfig::max_string_display_length =
                            list.at(1).trimmed().toInt();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_length_standard_pagination_number",
                  list.at(0)))
        {
            YerithERPConfig::pagination_number_of_table_rows =
                            list.at(1).trimmed().toInt();
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 ("local_parameter_length_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT",
                  list.at(0)))
        {
            YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
                            list.at(1).trimmed().toInt();

#ifdef YERITH_SERVER

            YERITH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
        			YerithERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT;

#endif /* YERITH_SERVER */

        }
    }
    while (!line.isNull());
}


void read_yerith_configuration(YerithLogger &logger,
                               YerithERPWindows &allWindows)
{
    YerithSqlTableModel &configurationsTableModel =
                    allWindows.getSqlTableModel_configurations();

    //***
    QSqlRecord configurationRecord =
                    configurationsTableModel.
                    record(YerithERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);
    QString
    alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA
                                 (configurationRecord,
                                  "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.
                    record(YerithERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);
    QString
    alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA
                                   (configurationRecord,
                                    "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record(YerithERPConfig::CONFIG_TVA_VALUE);
    QString
    tvaValue(GET_SQL_RECORD_DATA
             (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record(YerithERPConfig::CONFIG_SALES_STRATEGY);
    QString
    salesStrategyValue(GET_SQL_RECORD_DATA
                       (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record(YerithERPConfig::CONFIG_CURRENCY);
    QString
    currencyValue(GET_SQL_RECORD_DATA
                  (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.
                    record(YerithERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR);
    QString
    pageBeginYearValue(GET_SQL_RECORD_DATA
                       (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record
                    (YerithERPConfig::CONFIG_BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL);
    QString
    BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL_VALUE(GET_SQL_RECORD_DATA
                                                     (configurationRecord,
                                                      "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record
                    (YerithERPConfig::
                     CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3);

    QString DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3_VALUE
    (GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));



    /*
     * Now setting the configuration values
     */
    YerithERPConfig::currency = currencyValue;
    YerithERPConfig::alert_period_time_interval =
                    alertPeriodTimeIntervalValue.toUInt();
    YerithERPConfig::alert_quantity_time_interval =
                    alertQuantityTimeIntervalValue.toUInt();
    YerithERPConfig::tva_value = (tvaValue.toDouble() / 100.0);
    YerithERPConfig::salesStrategy = salesStrategyValue;

    YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY =
                    DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YERITH_ERP_3_VALUE;


    if (BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL_VALUE.isEmpty())
    {
        YerithERPConfig::backup_restore_yerith_erp_3_time_interval = 169200;
    }
    else
    {
        YerithERPConfig::backup_restore_yerith_erp_3_time_interval =
                        BACKUP_DATABASE_YERITH_ERP_3_TIME_INTERVAL_VALUE.toUInt();
    }

    if (!pageBeginYearValue.isEmpty())
    {
        YerithERPConfig::annee_depart_pour_la_pagination = pageBeginYearValue;
    }
    else
    {
        YerithERPConfig::annee_depart_pour_la_pagination =
                        GET_CURRENT_DATE_YEAR;
    }


    logger.log("[main] read_yerith_configuration",
               QString("Folder for backup: %1").
               arg(YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY));

    YerithUtils::YERITH_CREATE_FOLDER
    (YerithERPConfig::fullpathToBACKUP_YERITH_ERP_3_DIRECTORY, &logger);


    logger.log("[main] read_yerith_configuration",
               QString("Folder for temporary files: %1").arg
               (YerithERPConfig::temporaryFilesDir));

    YerithUtils::YERITH_CREATE_FOLDER(YerithERPConfig::temporaryFilesDir,
                                      &logger);
}


int main(int argc, char *argv[])
{

#ifdef YERITH_CHECK_MEMORY
    mtrace();
#endif


#ifdef YERITH_ERP_3_0_TOUCH_SCREEN

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#endif

    YerithERPConfig::YERITH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER =
                    QString(std::getenv("YERITH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER"))
						.trimmed();

    YerithERPConfig::YERITH_ERP_3_0_yri_db_runtime_verif_HOME_FOLDER =
                    QString(std::getenv("YRI_DB_RUNTIME_VERIF_HOME_FOLDER"))
						.trimmed();

    YerithERPConfig::YERITH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER =
                    QString(std::getenv("YERITH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER"))
						.trimmed();

    YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER =
                    QString(std::getenv("YERITH_ERP_3_0_HOME_FOLDER")).trimmed();

    YerithERPConfig::YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER =
                    QString("%1/.yerith_erp_3_0").
                    arg(YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER);


//    QDEBUG_STRING_OUTPUT_2("YerithERPConfig::YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER",
//                      YerithERPConfig::YERITH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);


    YerithERPConfig::
    FILE_ABSOLUTEPATH_YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE
        =
                        QString(std::
                                getenv
                                ("YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE")).
                        trimmed();

//    QDEBUG_STRING_OUTPUT_2("YerithERPConfig::FILE_ABSOLUTEPATH_YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTIES",
//              YerithERPConfig::FILE_ABSOLUTEPATH_YERITH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE);

    YerithERPConfig::FILE_ABSOLUTEPATH_YERITH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER
		.prepend('/')
		.prepend(YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER);


    QApplication *app = new QApplication(argc, argv);


    YerithERPWindows::_YERITH_ERP_PGI_APPLICATION = app;


    YerithERPWindows::_FRENCH_TRANSLATOR = new QTranslator;

    YerithERPWindows::_ENGLISH_TRANSLATOR = new QTranslator;


    YerithERPWindows::_FRENCH_TRANSLATOR->load("/usr/share/qt5/translations/qtbase_fr.qm");

    QString path(QString("%1/bin/%2")
    				.arg(YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER,
    					 TRANSLATION_FILE_ENGLISH));

    YerithERPWindows::_ENGLISH_TRANSLATOR->load(path);


    app->installTranslator(YerithERPWindows::_FRENCH_TRANSLATOR);

    app->installTranslator(YerithERPWindows::_ENGLISH_TRANSLATOR);


#ifdef YERITH_FRANCAIS_LANGUAGE

    YerithMainWindow::LANGUE_ANGLAISE = false;

#else //YERITH_ENGLISH_LANGUAGE

    YerithMainWindow::LANGUE_ANGLAISE = true;

#endif

    QString
    logFileName(QString("%1/%2").arg
                (YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER,
                 FILE_YERITH_ERP_3_0_PUBLIC_LOG_INFO));

    YerithUtils::setMainWorkingDirectory(YerithERPConfig::
                                         YERITH_ERP_3_0_HOME_FOLDER);

    YerithUtils::setLogFileName(logFileName);

    YerithLogger logger("yerith-erp-3.0");

    logger.log("main",
               QString
               ("yerith home folder (located in the environment variable YERITH_ERP_3_0_HOME_FOLDER): %1").
               arg(YerithERPConfig::YERITH_ERP_3_0_HOME_FOLDER));

    logger.log("main", QString("log file name: %1").arg(logFileName));

    YerithMain stockFacileMain(&logger);

    QString
    initCfg(QString("%1/%2").arg
            (YerithERPConfig::YERITH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER,
             FILE_YERITH_ERP_3_0_PUBLIC_USER_PROPERTIES));

    logger.log("main",
               QString("%1 path: %2").arg
               (FILE_YERITH_ERP_3_0_PUBLIC_USER_PROPERTIES, initCfg));

    YerithERPConfig::initYerithConfig(initCfg);

    read_system_local_yerith_configuration();

    YerithDatabase database;

    YerithERPWindows allWindows(YerithERPWindows::_YERITH_ERP_PGI_APPLICATION->desktop());

    QApplication::setStyle(QStyleFactory::
                           create(YerithUtils::YERITH_QT_STYLE_CDE));

    YerithUtils::setAllWindows(&allWindows);


#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

    Test_YerithERPTestUtils::setAllWindows(&allWindows);

#endif

    bool yerithCmdLineParser_isUsed = false;

    YerithCommandLineParser yerithCmdLineParser;

    enum YerithCommandLineArgumentType yerithCmdLineParseResult =
                    yerithCmdLineParser.parseProgramArguments(argc, argv);

    switch (yerithCmdLineParseResult)
    {
    case VERSION:
        yerithCmdLineParser_isUsed = true;
        YerithUtils::YERITH_PRINT_TO_STDOUT(YerithUtils::
                                            YERITH_ERP_3_0_LAST_BUILD_ID);
        break;

    default:
        break;
    }

    if (yerithCmdLineParser_isUsed)
    {
        if (VERSION == yerithCmdLineParseResult)
        {
            return 0;
        }
    }

    qDebug() << QString("db_name: %1, "
                        "db_ip_address: %2, "
                        "db_connection_options: %3").arg(database.db_name(),
                                                         database.
                                                         db_ip_address(),
                                                         database.
                                                         db_connection_options
                                                         ());

    allWindows.setDatabase(&database);

    bool isDatabaseOpened = database.open();

    logger.log("main", "Database connection could be opened successfully");

    QStringList sql_table_list;

#ifdef YERITH_FEATURES_COMPTABILITE_yes
    sql_table_list
            << FILE_YERITH_ERP_3_0_SQL_TABLE_OPERATIONS_COMPTABLES;
#endif

    sql_table_list
			<< FILE_YERITH_ERP_3_0_SQL_TABLE_periodes_dappartenance_groupes_de_paie_hr
			<< FILE_YERITH_ERP_3_0_SQL_TABLE_groupes_demployes_hr
			<< FILE_YERITH_ERP_3_0_SQL_TABLE_groupes_de_paie_hr
            << FILE_YERITH_ERP_3_0_SQL_TABLE_TYPE_DOPERATIONS_FINANCIERES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_COMPTES_BANCAIRES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_ENTREPRISE_INFO
            << FILE_YERITH_ERP_3_0_SQL_TABLE_TYPE_DE_VENTE
            << FILE_YERITH_ERP_3_0_SQL_TABLE_TYPE_DE_PAIEMENT
            << FILE_YERITH_ERP_3_0_SQL_TABLE_PAIEMENTS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_CREDIT_CARD_ISSUING_COMPANY
            << FILE_YERITH_ERP_3_0_SQL_TABLE_INIT_CONFIGURATION
            << FILE_YERITH_ERP_3_0_SQL_TABLE_CONFIGURATIONS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_ROLES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_USERS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_TITRES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_LOCALISATIONS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_CHARGES_FINANCIERES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_YRI_DB_RUNTIME_VERIF
            << FILE_YERITH_ERP_3_0_SQL_TABLE_ACHATS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_STOCKS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_SERVICES_COMPLETES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_MARCHANDISES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_STOCKS_VENDU
            << FILE_YERITH_ERP_3_0_SQL_TABLE_STOCKS_SORTIES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_ALERTES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_REMISES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_CONDITIONS_ALERTES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_COURRIERS_ALERTES
            << FILE_YERITH_ERP_3_0_SQL_TABLE_PROGRAMMES_DE_FIDELITES_CLIENTS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_GROUPES_DE_CLIENTS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_CLIENTS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_FOURNISSEURS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_IMPRIMANTERESEAU_RECUS_PETITS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_DBMS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_DEPARTEMENTS_PRODUITS
            << FILE_YERITH_ERP_3_0_SQL_TABLE_CATEGORIES
			<< FILE_YERITH_ERP_3_0_SQL_TABLE_LIGNES_BUDGETAIRES;

    for (int k = 0; k < sql_table_list.size(); ++k)
    {
        YerithUtils::executer_fichier_sql(sql_table_list.at(k), &logger);
    }

    read_yerith_configuration(logger, allWindows);

    readTexTemplateFiles(logger);

    allWindows.createAllYerithPosUserWindows();


    QApplication::setFont(QFont
                          (STRING_APPLICATION_WIDE_FONT_TYPE_YERITH_ERP_3_0,
                           INT_APPLICATION_WIDE_FONT_SIZE_YERITH_ERP_3_0));

    QApplication::setWindowIcon(QIcon
                                (":yerith-erp-3-0-images/yerith-erp-3-0-icon.png"));


//#define YERITH_DEVEL_TARGET

#ifdef YERITH_DEVEL_TARGET

    YerithERPConfig::THIS_SITE_LOCALISATION_NAME =
                    "YERITH_ERP_3_0_TESTING_RD_BUILDING";

#endif

//      QDEBUG_STRING_OUTPUT_2("YerithERPConfig::THIS_SITE_LOCALISATION_NAME",
//                                                 YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

#ifndef YERITH_ERP_3_0_TESTING_UNIT_TEST

    allWindows._mainWindow->rendreVisibleLocalTOCLASS(isDatabaseOpened);

    app->exec();

    database.close();

    YERITH_DELETE_FREE_POINTER_NOW(YerithERPWindows::_ENGLISH_TRANSLATOR);

    YERITH_DELETE_FREE_POINTER_NOW(YerithERPWindows::_FRENCH_TRANSLATOR);

    return 0;

#else

    Test_YerithUtils aYERITH_TEST_SRC_UTILS_TestInstance;

    Test_YerithERPTableView aYERITHERPTableView_TestInstance;


    int test_SRC_UTILS_ret_value =
                    QTest::qExec(&aYERITH_TEST_SRC_UTILS_TestInstance, argc, argv);

    int test_YERITHERPTableView__ret_value =
                    QTest::qExec(&aYERITHERPTableView_TestInstance, argc, argv);


    QDEBUG_STRING_OUTPUT_2_N("Test_YerithUtils", test_SRC_UTILS_ret_value);

    QDEBUG_STRING_OUTPUT_2_N("Test_YerithERPTableView",
                             test_YERITHERPTableView__ret_value);


    return 0;

#endif
}
