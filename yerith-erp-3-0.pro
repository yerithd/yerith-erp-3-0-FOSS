RC_ICONS = yerith-erp-3-0-icon.ico

#!!! ALWAYS KEEP THIS FOLLWING INSTRUCTION AT LINE 5 !!! (FOR UNIT
#    TESTING SCRIPTS TO WORK PROPERLY !
YERITH_ERP_3_0_CONFIG_BUILD = "NO_YERITH_ERP_3_0_TESTING_UNIT_TEST"

YERITH_ERP_3_0_TOUCH_SCREEN = "NO_YERITH_ERP_3_0_TOUCH_SCREEN_SET"

DEFINES += $${YERITH_ERP_3_0_CONFIG_BUILD}
DEFINES += $${YERITH_ERP_3_0_TOUCH_SCREEN}
DEFINES += ${YERITH_VIRTUAL_KEYBOARD_OPTIONS}
DEFINES += ${YERITH_FEATURES_COMPTABILITE_VALUE}
DEFINES += ${YERITH_LANGUAGE}
DEFINES += ${YERITH_DEBUG_LOG}
DEFINES += ${YERITH_VERSION}
#DEFINES += ${YERITH_CHECK_MEMORY}

#QML_IMPORT_PATH =

#include(yerith-erp-3-0-qtvirtualkeyboard-deployment.pri)

CONFIG += precompile_header 
CONFIG += qt 
CONFIG += moc 

contains(YERITH_ERP_3_0_CONFIG_BUILD, "NO_YERITH_ERP_3_0_TESTING_UNIT_TEST") {
	CONFIG -= testcase
  
	QT -= testlib
  
	LIBS -= -lgcov
	
	QMAKE_CXXFLAGS_RELEASE -= --coverage
} else {
	CONFIG += testcase
  
	QT += testlib
  
	LIBS += -lgcov
	
	QMAKE_CXXFLAGS_RELEASE += --coverage
}

contains(YERITH_ERP_3_0_TOUCH_SCREEN, "NO_YERITH_ERP_3_0_TOUCH_SCREEN_SET") {
  CONFIG -= disable-desktop
} else {
  CONFIG += disable-desktop
}

#CONFIG += debug

CONFIG -= import_plugins

QT += widgets
QT += sql
QT += network
QT += dbus
QT += quick

#disable-desktop {
#    DEFINES += MAIN_QML=\\\"basic-b2qt.qml\\\"
#} 

#static {
#  QTPLUGIN += qtvirtualkeyboardplugin
#	QT += svg
#}

#LIBS += -L$$PWD/lib -lyri-db-runtime-verif_FOR_YERITH_ERP

#LIBS += -L/usr/lib/i386-linux-gnu/qt5/plugins/styles -lqmotifstyle -lqplastiquestyle

QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -Wunused-parameter

QMAKE_CFLAGS_RELEASE -= -O2

DESTDIR = bin

MOC_DIR  = moc

OBJECTS_DIR = obj

FORMS = ui/yerith-erp-main-window.ui \
			ui/yerith-erp-recherche-conditionnelle-window.ui \
			ui/yerith-erp-imprimer-dialog.ui \
			ui/yerith-erp-comptabilite-window.ui \
			ui/stocks/yerith-erp-sortir-window.ui \
		  ui/yerith-erp-entrer-window.ui \
		  ui/stocks/yerith-erp-mouvements_de_stocks-window.ui \
		  ui/stocks/yerith-erp-marchandises-window.ui \
		  ui/stocks/yerith-erp-modifier-window.ui \
			ui/stocks/yerith-erp-historique-du-stock-window.ui \
			ui/stocks/yerith-erp-stocks-window.ui \
		  ui/stocks/yerith-erp-sortir-liststocks-window.ui \
		  ui/stocks/yerith-erp-stock-detail-window.ui \
			ui/pos/yerith-erp-pointdevente-liststocks-window.ui \
		  ui/pos/yerith-erp-pointdevente-window.ui \
		  ui/pos/yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.ui \
		  ui/pos/yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.ui \
		  ui/pos/yerith-erp-pointdevente-methode-paiement-dialog.ui \
			ui/purchase/yerith-erp-achats-detail-window.ui \
			ui/purchase/yerith-erp-achats-window.ui \
			ui/yerith-erp-charges-financieres-window.ui \
			ui/yerith-erp-charges-financieres-detail-window.ui \
			ui/supplier/yerith-erp-tableau-des-transactions-du-fournisseur-window.ui \
			ui/supplier/yerith-erp-modifier-fournisseur-window.ui \
			ui/supplier/yerith-erp-creer-fournisseur-window.ui \
			ui/supplier/yerith-erp-fournisseur-details-window.ui \
		  ui/supplier/yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.ui \
		  ui/supplier/yerith-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.ui \
		  ui/supplier/yerith-erp-GROUPES-dun-employe-hr-window.ui \
		  ui/supplier/yerith-erp-creer-GROUPE-DE-PAIE-hr-window.ui \
		  ui/supplier/yerith-erp-creer-groupe-DEMPLOYES-hr-window.ui \
		  ui/supplier/yerith-erp-GROUPES-DEMPLOYES-hr-window.ui \
		  ui/supplier/yerith-erp-GROUPES-DE-PAIE-hr-window.ui \
		  ui/supplier/yerith-erp-fournisseurs-window.ui \
			ui/crm/creer/yerith-erp-creer-un-programme-de-fidelite-clients-window.ui \
			ui/crm/creer/yerith-erp-creer-compte-client-window.ui \
			ui/crm/creer/yerith-erp-creer-groupe-de-clients-window.ui \
			ui/crm/details/yerith-erp-details-dun-groupe-de-clients-window.ui \
			ui/crm/details/yerith-erp-clients-detail-window.ui \
			ui/crm/details/yerith-erp-details-dun-programme-de-fidelite-clients-window.ui \
			ui/crm/yerith-erp-tableau-des-transactions-du-client-window.ui \
			ui/crm/modifier/yerith-erp-modifier-compte-client-window.ui \
			ui/crm/modifier/yerith-erp-modifier-groupe-de-clients-window.ui \
			ui/crm/modifier/yerith-erp-modifier-un-programme-de-fidelite-clients-window.ui \
			ui/crm/yerith-erp-payer-compte-client-window.ui \
			ui/crm/yerith-erp-programmes-de-fidelite-clients-window.ui \
			ui/crm/yerith-erp-groupes-de-clients-window.ui \
			ui/crm/yerith-erp-groupes-dun-client-window.ui \
			ui/crm/yerith-erp-clients-window.ui \
			ui/yerith-erp-paiements-window.ui \
		  ui/yerith-erp-ventes-window.ui \
		  ui/yerith-erp-tableaux-de-bord-window.ui \
			ui/yerith-erp-alertes-window.ui \
		  ui/yerith-erp-changer-utilisateur-dialog.ui \
		  ui/yerith-erp-admin-window.ui \
		  ui/yerith-erp-admin-search-form.ui \
		  ui/yerith-erp-admin-create-window.ui \
		  ui/yerith-erp-admin-lister-window.ui \
		  ui/yerith-erp-admin-detail-window.ui \
			ui/yerith-erp-admin-modifier-window.ui

PRECOMPILED_HEADER  = src/include/yerith-erp-3-0-precompiled-header.hpp

HEADERS = src/include/yerith-erp-3-0-precompiled-header.hpp \
		  src/YRIruntimeverification_adaptor.h \
			src/IYRIruntimeverificationAdaptor_interface.h \
			src/yerith-erp-windows.hpp \
		  src/imports/yerith-erp-database-table-import-info.hpp \
			src/imports/yerith-erp-marchandise-import.hpp \
			src/imports/yerith-erp-stock-import.hpp \
		  src/include/yerith-erp-3-0-definition-special-qt5.7-macro.hpp \
		  src/include/yerith-erp-3-0-definition-colour-rgb.hpp \
		  src/include/yerith-erp-3-0-definition-user-manual.hpp \
		  src/include/yerith-erp-3-0-definition-file-folder.hpp \
		  src/include/yerith-erp-3-0-definition-format-date-time.hpp \
		  src/include/yerith-erp-3-0-definition-initial-focus-searchbar.hpp \
		  src/include/yerith-erp-3-0-definition-month.hpp \
		  src/include/yerith-erp-3-0-definition-shortcut.hpp \
		  src/include/yerith-erp-3-0-software.text-configuration.hpp \
			src/widgets/table-view/yerith-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp \
			src/widgets/yerith-erp-push-button-PASSWORD.hpp \
			src/widgets/yerith-erp-progress-bar.hpp \
	  	src/widgets/yerith-erp-qcolumn-view.hpp \
	  	src/widgets/widgets-table/yerith-erp-tab-widget.hpp \
			src/widgets/widgets-table/yerith-erp-admin-verif-logging-table-widget.hpp \
			src/widgets/widgets-table/yerith-erp-creer-un-groupe-de-clients-table-widget.hpp \
			src/widgets/widgets-table/yerith-erp-groupes-dun-EMPLOYE-table-widget.hpp \
			src/widgets/widgets-table/yerith-erp-groupes-dun-client-table-widget.hpp \
			src/widgets/widgets-table/yerith-erp-COMPTABILITE-table-widget.hpp \
			src/widgets/widgets-table/yerith-erp-point-de-vente-table-widget.hpp \
			src/widgets/widgets-table/yerith-erp-table-widget.hpp \
			src/widgets/widgets-table/yerith-erp-qtable-widget-item.hpp \
			src/widgets/widgets-table/yerith-erp-tab-widget.hpp \
			src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp \
			src/widgets/table-view/yerith-erp-table-view.hpp \
			src/widgets/table-view/yerith-erp-comptes-doperations-comptabilite-table-view.hpp \
			src/widgets/table-view/yerith-erp-paiements-table-view.hpp \
		  src/widgets/table-view/yerith-erp-stocks-table-view.hpp \
		  src/widgets/table-view/yerith-erp-ventes-table-view.hpp \
		  src/widgets/table-view/yerith-erp-mouvements_de_stocks-table-view.hpp \
			src/widgets/table-view/yerith-erp-GROUPES-DE-PAIE-HR-table-view.hpp \
		  src/widgets/table-view/yerith-erp-fournisseurs-table-view.hpp \
			src/widgets/table-view/yerith-erp-programmes-de-fidelite-clients-table-view.hpp \
			src/widgets/table-view/yerith-erp-groupes-de-clients-table-view.hpp \
		  src/widgets/table-view/yerith-erp-clients-table-view.hpp \
		  src/widgets/table-view/yerith-erp-alertes-table-view.hpp \
		  src/widgets/table-view/yerith-erp-CHARGES-FINANCIERES-table-view.hpp \
		  src/widgets/table-view/yerith-erp-achats-table-view.hpp \
		  src/widgets/table-view/yerith-erp-marchandises-table-view.hpp \
		  src/widgets/yerith-erp-line-edit.hpp \
		  src/widgets/yerith-erp-radio-button.hpp \
		  src/widgets/yerith-erp-qmessage-box.hpp \
		  src/widgets/yerith-erp-push-button.hpp \
		  src/widgets/yerith-erp-text-edit.hpp \
		  src/widgets/yerith-erp-combo-box.hpp \
		  src/widgets/yerith-erp-qstandard-item-model.hpp \
		  src/widgets/yerith-erp-qstandard-item.hpp \
		  src/widgets/yerith-erp-spinbox.hpp \
		  src/widgets/yerith-erp-datetime-edit.hpp \
		  src/widgets/yerith-erp-qcheckbox.hpp \
		  src/widgets/yerith-erp-select-db-qcheckbox.hpp \
		  src/process/yerith-erp-process-info.hpp \
		  src/process/yerith-erp-process.hpp \
			src/dialogs/yerith-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp \
		  src/dialogs/yerith-erp-dialog-commons.hpp \
		  src/dialogs/yerith-erp-generic-select-db-field-dialog.hpp \
		  src/dialogs/yerith-erp-changer-utilisateur-dialog.hpp \
		  src/dialogs/yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp \
		  src/dialogs/yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp \
		  src/dialogs/yerith-erp-pointdevente-methode-paiement-dialog.hpp \
			src/windows/crm/creer/yerith-erp-creer-groupe-de-clients-window.hpp \
			src/windows/crm/creer/yerith-erp-creer-un-programme-de-fidelite-clients-window.hpp \
		  src/windows/crm/creer/yerith-erp-creer-compte-client-window.hpp \
			src/windows/crm/modifier/yerith-erp-modifier-compte-client-window.hpp \
			src/windows/crm/details/yerith-erp-details-dun-groupe-de-clients-window.hpp \
			src/windows/crm/details/yerith-erp-clients-detail-window.hpp \
			src/windows/crm/details/yerith-erp-details-dun-programme-de-fidelite-clients-window.hpp \
			src/windows/crm/yerith-erp-programmes-de-fidelite-clients-window.hpp \
		  src/windows/crm/yerith-erp-groupes-de-clients-window.hpp \
		  src/windows/crm/yerith-erp-groupes-dun-client-window.hpp \
		  src/windows/crm/yerith-erp-clients-window.hpp \
			src/windows/crm/yerith-erp-payer-compte-client-window.hpp \
		  src/windows/crm/yerith-erp-tableau-des-transactions-du-client-window.hpp \
		  src/windows/yerith-erp-entrer-window.hpp \
		  src/windows/stocks/yerith-erp-mouvements_de_stocks-window.hpp \
		  src/windows/stocks/yerith-erp-modifier-window.hpp \
		  src/windows/stocks/yerith-erp-marchandises-window.hpp \
		  src/windows/stocks/yerith-erp-sortir-liststocks-window.hpp \
		  src/windows/stocks/yerith-erp-sortir-window.hpp \
		  src/windows/stocks/yerith-erp-historique-du-stock-window.hpp \
		  src/windows/stocks/yerith-erp-stock-detail-window.hpp \
		  src/windows/stocks/yerith-erp-stocks-window.hpp \
			src/windows/yerith-erp-charges-financieres-window.hpp \
			src/windows/yerith-erp-charges-financieres-detail-window.hpp \
		  src/windows/purchase/yerith-erp-achats-detail-window.hpp \
		  src/windows/purchase/yerith-erp-achats-window.hpp \
		  src/windows/pos/yerith-erp-pointdevente-window.hpp \
		  src/windows/pos/yerith-erp-pointdevente-liststocks-window.hpp \
			src/windows/supplier/yerith-erp-calcul-salaire-EMPLOYE-window.hpp \
			src/windows/supplier/yerith-erp-tableau-des-transactions-du-fournisseur-window.hpp \
			src/windows/supplier/yerith-erp-modifier-fournisseur-window.hpp \
		  src/windows/supplier/yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.hpp \
		  src/windows/supplier/yerith-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.hpp \
			src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.hpp \
		  src/windows/supplier/yerith-erp-creer-GROUPE-DE-PAIE-hr-window.hpp \
			src/windows/supplier/yerith-erp-creer-groupe-DEMPLOYES-hr-window.hpp \
			src/windows/supplier/yerith-erp-GROUPES-DEMPLOYES-hr-window.hpp \
			src/windows/supplier/yerith-erp-GROUPES-DE-PAIE-hr-window.hpp \
			src/windows/supplier/yerith-erp-fournisseurs-window.hpp \
			src/windows/supplier/yerith-erp-creer-fournisseur-window.hpp \
			src/windows/supplier/yerith-erp-fournisseur-details-window.hpp \
		  src/windows/yerith-erp-comptabilite-window.hpp \
		  src/windows/yerith-erp-recherche-conditionnelle-window.hpp \
		  src/windows/yerith-erp-qtable-widget-window-commons.hpp \
		  src/windows/yerith-erp-window-commons.hpp \
		  src/windows/yerith-erp-main-window.hpp \
		  src/windows/yerith-erp-alertes-window.hpp \
		  src/windows/yerith-erp-tableaux-de-bord-window.hpp \
		  src/windows/yerith-erp-paiements-window.hpp \
		  src/windows/yerith-erp-ventes-window.hpp \
		  src/tests/utils/yerith-erp-test-yerith-erp-utils.hpp \
		  src/tests/yerith-erp-test-yerith-erp-table-view.hpp \
			src/tests/yerith-erp-test-yerith-erp-windows.hpp \
		  src/tests/utils/yerith-erp-test-import-csv-file-data.hpp \
			src/tests/utils/yerith-erp-test-utils.hpp \
		  src/users/yerith-erp-user-settings.hpp \
			src/users/yerith-erp-users.hpp \
		  src/users/yerith-erp-user-vendeur.hpp \		  
		  src/users/yerith-erp-user-gestionaire-des-stocks.hpp \		  
		  src/users/yerith-erp-user-manager.hpp \
		  src/users/yerith-erp-user-magasinier.hpp \
		  src/users/yerith-erp-user-caissier.hpp \
		  src/users/yerith-erp-user-administrateur.hpp \
		 	src/dbus/YRIruntimeverification_adaptor.h \
			src/dbus/YERITH_RUNTIME_VERIFIER.hpp \
	 		src/utils/credit-card/yerith-erp-carte-credit-info.hpp \
			src/utils/windows/yerith-erp-abstract-class-detail-view-window.hpp \
			src/utils/windows/yerith-erp-abstract-class-yerithsearch-window.hpp \
		  src/utils/virtual-keyboard/yerith-erp-key-emitter.hpp \
		  src/utils/yerith-erp-spell-numbers.hpp \
		  src/utils/yerith-erp-sqltable-model.hpp \
		  src/utils/yerith-erp-LIST-map-COMPLEX-ITEM.hpp \
		  src/utils/yerith-erp-map-COMPLEX-ITEM.hpp \
		  src/utils/yerith-erp-payment-processing-information.hpp \
			src/utils/yerith-erp-config.hpp \
		  src/utils/yerith-erp-service-stock-marchandise-data.hpp \
			src/utils/yerith-erp-utils.hpp \
		  src/utils/yerith-erp-database.hpp \
		  src/utils/yerith-erp-logger.hpp \
		  src/utils/yerith-erp-historique-stock.hpp \
		  src/utils/yerith-erp-info-entreprise.hpp \
		  src/utils/yerith-erp-article-vente-info.hpp \
		  src/utils/yerith-erp-database-table-column.hpp \
		  src/utils/yerith-erp-command-line-parser.hpp \
		  src/utils/yerith-erp-style.hpp \
		  src/utils/print_latex_pdf/yerith-erp-print_yerith_latex_util_pdf.hpp \
		  src/utils/yerith-erp-client-account-sale-info.cpp \
	    src/admin/yerith-erp-qtable-widget-admin-window-commons.hpp \
	    src/admin/yerith-erp-admin-windows-commons.hpp \
			src/admin/yerith-erp-admin-window.hpp \
		  src/admin/yerith-erp-admin-search-form.hpp \
		  src/admin/widgets/table-view/yerith-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp \
			src/admin/widgets/table-view/yerith-erp-financial-expense-table-view.hpp \
			src/admin/widgets/table-view/yerith-erp-product-department-table-view.hpp \
			src/admin/widgets/table-view/yerith-erp-alert-table-view.hpp \
		  src/admin/widgets/table-view/yerith-erp-site-table-view.hpp \
		  src/admin/widgets/table-view/yerith-erp-user-table-view.hpp \
			src/admin/widgets/table-view/yerith-erp-category-table-view.hpp \
			src/admin/widgets/table-view/yerith-erp-bank-account-table-view.hpp \
		  src/admin/creer/yerith-erp-admin-create-window.hpp \
		  src/admin/lister/yerith-erp-admin-lister-window.hpp \
		  src/admin/detail/yerith-erp-admin-detail-window.hpp \		
		  src/admin/modifier/yerith-erp-admin-modifier-window.hpp
 
SOURCES = src/yerith-erp-3-0.cpp \
		  src/YRIruntimeverification_adaptor.cpp \
			src/IYRIruntimeverificationAdaptor_interface.cpp \
		  src/yerith-erp-windows.cpp \
			src/imports/yerith-erp-marchandise-import.cpp \
		  src/imports/yerith-erp-stock-import.cpp \
			src/widgets/table-view/yerith-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.cpp \
			src/widgets/yerith-erp-push-button-PASSWORD.cpp \
		  src/widgets/yerith-erp-progress-bar.cpp \
	  	src/widgets/yerith-erp-qcolumn-view.cpp \
			src/widgets/table-view/yerith-erp-table-view-with-pagination.cpp \
			src/widgets/table-view/yerith-erp-table-view.cpp \
			src/widgets/table-view/yerith-erp-comptes-doperations-comptabilite-table-view.cpp \
		  src/widgets/table-view/yerith-erp-paiements-table-view.cpp \
		  src/widgets/table-view/yerith-erp-stocks-table-view.cpp \
			src/widgets/table-view/yerith-erp-ventes-table-view.cpp \
		  src/widgets/table-view/yerith-erp-mouvements_de_stocks-table-view.cpp \
			src/widgets/table-view/yerith-erp-GROUPES-DE-PAIE-HR-table-view.cpp \
		  src/widgets/table-view/yerith-erp-fournisseurs-table-view.cpp \
			src/widgets/table-view/yerith-erp-groupes-de-clients-table-view.cpp \
		  src/widgets/table-view/yerith-erp-clients-table-view.cpp \
		  src/widgets/table-view/yerith-erp-alertes-table-view.cpp \
		  src/widgets/table-view/yerith-erp-CHARGES-FINANCIERES-table-view.cpp \
		  src/widgets/table-view/yerith-erp-achats-table-view.cpp \
		  src/widgets/table-view/yerith-erp-marchandises-table-view.cpp \
			src/widgets/table-view/yerith-erp-programmes-de-fidelite-clients-table-view.cpp \
		  src/widgets/yerith-erp-line-edit.cpp \
		  src/widgets/yerith-erp-push-button.cpp \
		  src/widgets/yerith-erp-text-edit.cpp \
		  src/widgets/yerith-erp-combo-box.cpp \
		  src/widgets/yerith-erp-radio-button.cpp \
		  src/widgets/yerith-erp-qmessage-box.cpp \
		  src/widgets/yerith-erp-qstandard-item-model.cpp \		  
		  src/widgets/yerith-erp-qstandard-item.cpp \		  
		  src/widgets/yerith-erp-spinbox.cpp \
		  src/widgets/yerith-erp-datetime-edit.cpp \
		  src/widgets/yerith-erp-qcheckbox.cpp \
		  src/widgets/yerith-erp-select-db-qcheckbox.cpp \
			src/widgets/widgets-table/yerith-erp-tab-widget.cpp \
			src/widgets/widgets-table/yerith-erp-admin-verif-logging-table-widget.cpp \
			src/widgets/widgets-table/yerith-erp-creer-un-groupe-de-clients-table-widget.cpp \
			src/widgets/widgets-table/yerith-erp-groupes-dun-EMPLOYE-table-widget.cpp \
			src/widgets/widgets-table/yerith-erp-groupes-dun-client-table-widget.cpp \
			src/widgets/widgets-table/yerith-erp-COMPTABILITE-table-widget.cpp \
			src/widgets/widgets-table/yerith-erp-point-de-vente-table-widget.cpp \
			src/widgets/widgets-table/yerith-erp-table-widget.cpp \
			src/widgets/widgets-table/yerith-erp-qtable-widget-item.cpp \
			src/widgets/widgets-table/yerith-erp-tab-widget.cpp \
		  src/process/yerith-erp-process-info.cpp \
		  src/process/yerith-erp-process.cpp \
			src/dialogs/yerith-erp-IMPRESSION_DE_DOCUMENT-dialog.cpp \
		  src/dialogs/yerith-erp-dialog-commons.cpp \
		  src/dialogs/yerith-erp-generic-select-db-field-dialog.cpp \
		  src/dialogs/yerith-erp-changer-utilisateur-dialog.cpp \
		  src/dialogs/yerith-erp-pointdevente-methode-paiement-carte-credit-dialog.cpp \
		  src/dialogs/yerith-erp-pointdevente-methode-paiement-comptant-entree-dialog.cpp \
		  src/dialogs/yerith-erp-pointdevente-methode-paiement-dialog.cpp \
			src/windows/include/YERITH_TABLEAUX_DE_BORDS_analyse_comparee_jour_semaine.cpp \
			src/windows/include/YERITH_TABLEAUX_DE_BORDS_analyse_comparee_mensuelle.cpp \
			src/windows/include/YERITH_TABLEAUX_DE_BORDS_meilleurs_stats.cpp \
			src/windows/include/YERITH_TABLEAUX_DE_BORDS_zero_stats.cpp \
			src/windows/include/YERITH_TABLEAUX_DE_BORDS_zero_stats_stocks.cpp \
			src/windows/yerith-erp-comptabilite-window.cpp \
		  src/windows/crm/creer/yerith-erp-creer-groupe-de-clients-window.cpp \
			src/windows/crm/creer/yerith-erp-creer-un-programme-de-fidelite-clients-window.cpp \
		  src/windows/crm/creer/yerith-erp-creer-compte-client-window.cpp \
			src/windows/crm/details/yerith-erp-details-dun-groupe-de-clients-window.cpp \
			src/windows/crm/details/yerith-erp-clients-detail-window.cpp \
			src/windows/crm/details/yerith-erp-details-dun-programme-de-fidelite-clients-window.cpp \
			src/windows/crm/modifier/yerith-erp-modifier-compte-client-window.cpp \
			src/windows/crm/yerith-erp-programmes-de-fidelite-clients-window.cpp \
		  src/windows/crm/yerith-erp-groupes-de-clients-window.cpp \
		  src/windows/crm/yerith-erp-groupes-dun-client-window.cpp \
			src/windows/crm/yerith-erp-clients-window.cpp \
			src/windows/crm/yerith-erp-payer-compte-client-window.cpp \
		  src/windows/crm/yerith-erp-tableau-des-transactions-du-client-window.cpp \
		  src/windows/yerith-erp-entrer-window.cpp \
		  src/windows/stocks/yerith-erp-mouvements_de_stocks-window.cpp \
		  src/windows/stocks/yerith-erp-modifier-window.cpp \
		  src/windows/stocks/yerith-erp-marchandises-window.cpp \
			src/windows/stocks/yerith-erp-sortir-liststocks-window.cpp \
		  src/windows/stocks/yerith-erp-sortir-window.cpp \
			src/windows/stocks/yerith-erp-historique-du-stock-window.cpp \
		  src/windows/stocks/yerith-erp-stock-detail-window.cpp \
		  src/windows/stocks/yerith-erp-stocks-window.cpp \
		  src/windows/yerith-erp-charges-financieres-window.cpp \
			src/windows/yerith-erp-charges-financieres-detail-window.cpp \
		  src/windows/purchase/yerith-erp-achats-detail-window.cpp \
		  src/windows/purchase/yerith-erp-achats-window.cpp \
		  src/windows/pos/yerith-erp-pointdevente-window.cpp \
		  src/windows/pos/yerith-erp-pointdevente-liststocks-window.cpp \
			src/windows/supplier/yerith-erp-calcul-salaire-EMPLOYE-window.cpp \
			src/windows/supplier/yerith-erp-tableau-des-transactions-du-fournisseur-window.cpp \
			src/windows/supplier/yerith-erp-modifier-fournisseur-window.cpp \
			src/windows/supplier/yerith-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.cpp \
		  src/windows/supplier/yerith-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.cpp \
			src/windows/supplier/yerith-erp-GROUPES-dun-employe-hr-window.cpp \
		  src/windows/supplier/yerith-erp-creer-GROUPE-DE-PAIE-hr-window.cpp \
			src/windows/supplier/yerith-erp-creer-groupe-DEMPLOYES-hr-window.cpp \
			src/windows/supplier/yerith-erp-GROUPES-DEMPLOYES-hr-window.cpp \
			src/windows/supplier/yerith-erp-GROUPES-DE-PAIE-hr-window.cpp \
			src/windows/supplier/yerith-erp-fournisseurs-window.cpp \
			src/windows/supplier/yerith-erp-creer-fournisseur-window.cpp \
			src/windows/supplier/yerith-erp-fournisseur-details-window.cpp \
		  src/windows/yerith-erp-recherche-conditionnelle-window.cpp \
		  src/windows/yerith-erp-qtable-widget-window-commons.cpp \
		  src/windows/yerith-erp-window-commons.cpp \
		  src/windows/yerith-erp-main-window.cpp \
		  src/windows/yerith-erp-alertes-window.cpp \
		  src/windows/yerith-erp-tableaux-de-bord-window.cpp \
		  src/windows/yerith-erp-paiements-window.cpp \
		  src/windows/yerith-erp-ventes-window.cpp \
		  src/tests/utils/yerith-erp-test-yerith-erp-utils.cpp \
		  src/tests/yerith-erp-test-yerith-erp-table-view.cpp \
			src/tests/yerith-erp-test-yerith-erp-windows.cpp \
		  src/tests/utils/yerith-erp-test-import-csv-file-data.cpp \
			src/tests/utils/yerith-erp-test-utils.cpp \
		  src/users/yerith-erp-user-settings.cpp \
			src/users/yerith-erp-users.cpp \
		  src/users/yerith-erp-user-vendeur.cpp \		  
		  src/users/yerith-erp-user-gestionaire-des-stocks.cpp \
		  src/users/yerith-erp-user-manager.cpp \
		  src/users/yerith-erp-user-magasinier.cpp \
		  src/users/yerith-erp-user-caissier.cpp \
		  src/users/yerith-erp-user-administrateur.cpp \
			src/dbus/YERITH_RUNTIME_VERIFIER.cpp \
			src/utils/credit-card/yerith-erp-carte-credit-info.cpp \
			src/utils/windows/yerith-erp-abstract-class-detail-view-window.cpp \
		  src/utils/windows/yerith-erp-abstract-class-yerithsearch-window.cpp \
		  src/utils/virtual-keyboard/yerith-erp-key-emitter.cpp \
		  src/utils/yerith-erp-spell-numbers.cpp \
		  src/utils/yerith-erp-sqltable-model.cpp \
		  src/utils/yerith-erp-LIST-map-COMPLEX-ITEM.cpp \
		  src/utils/yerith-erp-map-COMPLEX-ITEM.cpp \
		  src/utils/yerith-erp-payment-processing-information.cpp \
		  src/utils/yerith-erp-config.cpp \
		  src/utils/yerith-erp-utils.cpp \
		  src/utils/yerith-erp-database.cpp \		  
		  src/utils/yerith-erp-logger.cpp \
		  src/utils/yerith-erp-historique-stock.cpp \
		  src/utils/yerith-erp-info-entreprise.cpp \
		  src/utils/yerith-erp-article-vente-info.cpp \
		  src/utils/yerith-erp-database-table-column.cpp \
		  src/utils/yerith-erp-command-line-parser.cpp \
			src/utils/yerith-erp-style.cpp \
			src/utils/print_latex_pdf/yerith-erp-print_yerith_latex_util_pdf.cpp \
		  src/utils/yerith-erp-client-account-sale-info.hpp \
	    src/admin/yerith-erp-qtable-widget-admin-window-commons.cpp \
			src/admin/yerith-erp-admin-windows-commons.cpp \
		  src/admin/yerith-erp-admin-window.cpp \
		  src/admin/yerith-erp-admin-search-form.cpp \		  
		  src/admin/widgets/table-view/yerith-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.cpp \
		  src/admin/widgets/table-view/yerith-erp-financial-expense-table-view.cpp \
		  src/admin/widgets/table-view/yerith-erp-product-department-table-view.cpp \
			src/admin/widgets/table-view/yerith-erp-alert-table-view.cpp \
		  src/admin/widgets/table-view/yerith-erp-site-table-view.cpp \
		  src/admin/widgets/table-view/yerith-erp-user-table-view.cpp \
		  src/admin/widgets/table-view/yerith-erp-category-table-view.cpp \
			src/admin/widgets/table-view/yerith-erp-bank-account-table-view.cpp \
		  src/admin/creer/yerith-erp-admin-create-window.cpp \
		  src/admin/lister/yerith-erp-admin-lister-window.cpp \
		  src/admin/detail/yerith-erp-admin-detail-window.cpp \
		  src/admin/modifier/yerith-erp-admin-modifier-window.cpp

RESOURCES    = yerith-erp-3-0.qrc


TRANSLATIONS = yerith-erp-3-0_english.ts

# install
target.path = yerith-erp-3-0
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources
