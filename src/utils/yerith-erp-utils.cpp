/*
 * yerith-erp-utils.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-utils.hpp"

#include "src/widgets/yerith-erp-qstandard-item.hpp"

#include "yerith-erp-logger.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"

#include "src/widgets/widgets-table/yerith-erp-point-de-vente-table-widget.hpp"

#include "src/widgets/yerith-erp-line-edit.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/utils/yerith-erp-payment-processing-information.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/admin/yerith-erp-admin-windows-commons.hpp"

#include "src/yerith-erp-windows.hpp"

#include <string>

#include <cassert>

#include <QtCore/qmath.h>

#include <QtCore/QRegExp>

#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QBuffer>

#include <QtCore/QProcess>

#include <QtGui/QImageReader>

#include <QtSql/QSqlDriver>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>

#include <QtSql/QSqlQuery>

#include <QtWidgets/QStyle>

#include <QtWidgets/QStyleFactory>

#include <QtWidgets/QStylePlugin>

#include <QtWidgets/QLabel>

#include <QtWidgets/QDialog>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QMessageBox>

#include <QtWidgets/QApplication>

#include <QtWidgets/QMainWindow>

#include <QtPrintSupport/QPrintDialog>

#include <QtCore/QPluginLoader>

#include <QtCore/QLibraryInfo>

#include <QtCore/QtPlugin>

YerithERPWindows *YerithUtils::_allWindows(0);

QString YerithUtils::_mainWorkingDirectory("");

QString YerithUtils::_logFileName("");

QString YerithUtils::_1a_tex("");

QString YerithUtils::FR_template_bilan_comptable_tex("");

QString YerithUtils::EN_template_bilan_comptable_tex("");

QString YerithUtils::FR_template_CLIENT_LOYALTY_GROUP_LISTING_tex("");

QString YerithUtils::EN_template_CLIENT_LOYALTY_GROUP_LISTING_tex("");

QString YerithUtils::FR_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex("");

QString YerithUtils::EN_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex("");

QString YerithUtils::FR_template_carte_de_fidelite_client_TEMPLATE_tex("");

QString YerithUtils::EN_template_carte_de_fidelite_client_TEMPLATE_tex("");

QString YerithUtils::FR_template_Comptabilite_comptes_Doperations_TEX("");

QString YerithUtils::EN_template_Comptabilite_comptes_Doperations_TEX("");

QString YerithUtils::FR_template_bon_de_commande_TEX("");

QString YerithUtils::EN_template_bon_de_commande_TEX("");

QString YerithUtils::FR_template_lister_admin_objects_TEX("");

QString YerithUtils::EN_template_lister_admin_objects_TEX("");

QString YerithUtils::FR_template_EMPLOYEE_GROUP_LISTING_tex("");

QString YerithUtils::EN_template_EMPLOYEE_GROUP_LISTING_tex("");

QString YerithUtils::FR_template_employee_PAY_GROUP_LISTING_tex("");

QString YerithUtils::EN_template_employee_PAY_GROUP_LISTING_tex("");

QString YerithUtils::FR_template_fiche_dun_fournisseur_tex("");

QString YerithUtils::EN_template_fiche_dun_fournisseur_tex("");

QString YerithUtils::FR_template_fiche_client_tex("");

QString YerithUtils::EN_template_fiche_client_tex("");

QString YerithUtils::FR_template_historique_dun_stock_tex("");

QString YerithUtils::EN_template_historique_dun_stock_tex("");

QString YerithUtils::FR_template_journal_des_paiements_tex("");

QString YerithUtils::EN_template_journal_des_paiements_tex("");

QString YerithUtils::FR_template_journal_des_ventes_tex("");

QString YerithUtils::EN_template_journal_des_ventes_tex("");

QString YerithUtils::FR_template_liste_des_mouvements_de_stocks_tex("");

QString YerithUtils::EN_template_liste_des_mouvements_de_stocks_tex("");

QString YerithUtils::FR_template_sortie_des_stocks_grand_tex("");

QString YerithUtils::EN_template_sortie_des_stocks_grand_tex("");

QString YerithUtils::FR_template_lister_stock_tex("");

QString YerithUtils::EN_template_lister_stock_tex("");

QString YerithUtils::FR_template_sortie_des_stocks_petit_tex("");

QString YerithUtils::EN_template_sortie_des_stocks_petit_tex("");

QString YerithUtils::FR_template_transactions_dun_fournisseur_tex("");

QString YerithUtils::EN_template_transactions_dun_fournisseur_tex("");

QString YerithUtils::FR_template_transactions_dun_client_tex("");

QString YerithUtils::EN_template_transactions_dun_client_tex("");

QString YerithUtils::FR_template_fournisseurs_tex("");

QString YerithUtils::EN_template_fournisseurs_tex("");

QString YerithUtils::FR_template_comptes_clients_tex("");

QString YerithUtils::EN_template_comptes_clients_tex("");

QString YerithUtils::FR_template_marchandises_tex("");

QString YerithUtils::EN_template_marchandises_tex("");

QString YerithUtils::FR_template_lister_alertes_tex("");

QString YerithUtils::EN_template_lister_alertes_tex("");

QString YerithUtils::FR_template_CHARGES_FINANCIERES_tex("");

QString YerithUtils::EN_template_CHARGES_FINANCIERES_tex("");

QString YerithUtils::FR_template_lister_achats_tex("");

QString YerithUtils::EN_template_lister_achats_tex("");

QString YerithUtils::FR_template_comptabilite_tex("");

QString YerithUtils::EN_template_comptabilite_tex("");

QString YerithUtils::FR_template_yerith_erp_3D_BARDIAG_TEMPLATE_tex("");

QString YerithUtils::EN_template_yerith_erp_3D_BARDIAG_TEMPLATE_tex("");

QString YerithUtils::FR_template_THREE_D_bardiag_tex("");

QString YerithUtils::EN_template_THREE_D_bardiag_tex("");

QString YerithUtils::FR_template_GROUPES_DUN_CLIENT_tex("");

QString YerithUtils::EN_template_GROUPES_DUN_CLIENT_tex("");

QString YerithUtils::FR_template_GROUPES_DUN_EMPLOYE_tex("");

QString YerithUtils::EN_template_GROUPES_DUN_EMPLOYE_tex("");

QString YerithUtils::FR_template_FACTURE_GRAND_tex("");

QString YerithUtils::EN_template_FACTURE_GRAND_tex("");

QString YerithUtils::FR_template_RECU_GRAND_tex("");

QString YerithUtils::EN_template_RECU_GRAND_tex("");

QString YerithUtils::FR_template_facture_petit_tex("");

QString YerithUtils::EN_template_facture_petit_tex("");

QString YerithUtils::EN_ZERO_ventes_tex("");

QString YerithUtils::FR_ZERO_ventes_tex("");

QString YerithUtils::FR_pie_chart_tex("");

QString YerithUtils::FR_bar_chart_tex("");

QString YerithUtils::FR_bar_diag_tex("");

QString YerithUtils::EN_pie_chart_tex("");

QString YerithUtils::EN_bar_chart_tex("");

QString YerithUtils::EN_bar_diag_tex("");

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_alerte_OUI_NON_ToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_strategieindexToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_titreToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_roleToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_typedecreditdebitToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_typedoperationfinancieresToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_typedeventeToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerithUtils::_typedepaiementToUserViewString;

const QString YerithUtils::PREFIX_RESTE_FACTURE_CLIENT("FAC-");

const QString YerithUtils::PREFIX_RECU_PAIEMENT_CLIENT("PAI-");

const QString YerithUtils::PREFIX_RECU_VENDU("VEN-");

const QString YerithUtils::PREFIX_RECU_SORTIE("SOR-");



const QKeySequence YerithUtils::INCREASE_QTABLE_VIEW_WIDGET_SIZE(
		QObject::tr(SHORTCUT_INCREASE_QTABLEVIEW_WIDGET_FONT_SIZE));


const QKeySequence YerithUtils::DECREASE_QTABLE_VIEW_WIDGET_SIZE(
		QObject::tr(SHORTCUT_DECREASE_QTABLEVIEW_WIDGET_FONT_SIZE));


const QKeySequence YerithUtils::MESSAGE_DAIDE_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_USER_TIP));

const QKeySequence YerithUtils::EXPORTER_AU_FORMAT_CSV_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_EXPORT_AS_CSV_FILE));

const QKeySequence YerithUtils::PARAMETRER_IMPRESSION_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_PRINTING_SETTING));

const QKeySequence YerithUtils::IMPRIMER_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_PRINT));

const QKeySequence YerithUtils::AFFICHER_LES_MARCHANDISES_NON_TERMINES_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_AFFICHER_LES_STOCKS_NON_TERMINES));

const QKeySequence YerithUtils::AFFICHER_LES_MARCHANDISES_TERMINES_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_AFFICHER_LES_STOCKS_TERMINES));

const QKeySequence YerithUtils::RECHERCHER_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_SEARCH));

const QKeySequence YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_REINITIALIAZE));

const QKeySequence YerithUtils::REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_WHO_AM_I));

const QKeySequence YerithUtils::LISTER_STOCKS_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_LIST_STOCKS));

QString YerithUtils::YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY("");

const QString YerithUtils::UTILISATEUR_NON_EXISTANT("unknown user");

const QString YerithUtils::DATABASE_MYSQL_VARCHAR_TYPE_STRING("varchar");

const QString YerithUtils::DATABASE_MYSQL_CHAR_TYPE_STRING("char");

const QString YerithUtils::DATABASE_MYSQL_DATE_TYPE_STRING("date");

const QString YerithUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING("double");

const QString YerithUtils::DATABASE_MYSQL_INT_TYPE_STRING("int");

const QString YerithUtils::DATABASE_MYSQL_TIME_TYPE_STRING("time");

const QString YerithUtils::ETHERNET_LOCALHOST_STRING_NAME("localhost");

const QString YerithUtils::ETHERNET_LOCALHOST_LOOPBACK_IP_ADDRESS("127.0.0.1");

const QString YerithUtils::YERITH_QT_STYLE_CDE("cde");

const unsigned int YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED(-2);

const QString YerithUtils::IMPRIMANTE_PDF("pdf");

const QString YerithUtils::IMPRIMANTE_EPSON_TM_T20ii("epson TM-T20ii");

#ifdef YERITH_FRANCAIS_LANGUAGE

const QString YerithUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU("epson TM-T20ii (réseau)");

#else

const QString YerithUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU(
		"epson TM-T20ii (on NETWORK)");

#endif

//This is 3 seconds (3000 milliseconds)
const unsigned int YerithUtils::PRINT_TIMEOUT(TIMEOUT_PRINT);

const QString YerithUtils::STRING_STYLESHEETS_YERITH_ERP_3_0 =
		QString("QLabel { font-size: 17px; color: rgb(%1); }")
            .arg(COLOUR_RGB_STRING_YERITH_BLUE_53_132_228);

const QString YerithUtils::STRING_STYLESHEET_COLOR_YERITHGREEN =
		QString("QLabel { color : rgb(%1); }")
            .arg(COLOUR_RGB_STRING_YERITH_GREEN_0_255_100);


const QColor YerithUtils::YERITH_WHITE_COLOR_FOR_EMPTY_MERCHANDISE_FILTERING(
		QColor(COLOUR_RGB_YERITH_RED_178_34_34));

const QColor YerithUtils::YERITH_WHITE_COLOR(
		QColor(COLOUR_RGB_YERITH_WHITE_255_255_255));

const QColor YerithUtils::YERITH_BLACK_COLOR(
		QColor(COLOUR_RGB_YERITH_BLACK_0_0_0));

const QColor YerithUtils::YERITH_BLUE_COLOR(
		QColor(COLOUR_RGB_YERITH_BLUE_77_93_254));

const QColor YerithUtils::YERITH_DARK_GREEN_COLOR(
		QColor(COLOUR_RGB_YERITH_DARK_GREEN_47_67_67));

const QColor YerithUtils::YERITH_GRAY_COLOR(
		QColor(COLOUR_RGB_YERITH_GRAY_78_78_78));

const QColor YerithUtils::YERITH_GREEN_COLOR(
		QColor(COLOUR_RGB_YERITH_GREEN_0_255_100));

const QColor YerithUtils::YERITH_YERITHGREEN_COLOR(
		QColor(COLOUR_RGB_YERITH_GREEN_2_160_70));

const QColor YerithUtils::YERITH_YELLOW_COLOR(
		QColor(COLOUR_RGB_YERITH_YELLOW_254_254_0));

const QColor YerithUtils::YERITH_ORANGE_COLOR(
		QColor(COLOUR_RGB_YERITH_ORANGE_243_162_0));

const QColor YerithUtils::YERITH_RED_COLOR(
		QColor(COLOUR_RGB_YERITH_RED_178_34_34));

const QColor YerithUtils::YERITH_INDIGO_COLOR(
		QColor(COLOUR_RGB_YERITH_INDIGO_83_0_125));

const QBrush YerithUtils::YERITH_QBRUSH_GREEN(YERITH_GREEN_COLOR);

const QBrush YerithUtils::YERITH_QBRUSH_YERITHGREEN(YERITH_YERITHGREEN_COLOR);

const QBrush YerithUtils::YERITH_QBRUSH_BLUE(YERITH_BLUE_COLOR);

const QBrush YerithUtils::YERITH_QBRUSH_ORANGE(YERITH_ORANGE_COLOR);

const QBrush YerithUtils::YERITH_QBRUSH_YERITHGRAY(YERITH_GRAY_COLOR);

const QPalette YerithUtils::YERITH_BLACK_PALETTE(
		QColor(COLOUR_RGB_YERITH_BLACK_0_0_0));

const QPalette YerithUtils::YERITH_BLUE_PALETTE(QColor(0, 0, 255));

const QPalette YerithUtils::YERITH_DARK_GREEN_PALETTE(YERITH_DARK_GREEN_COLOR);

const QPalette YerithUtils::YERITH_GRAY_PALETTE(QColor(237, 237, 237));

const QPalette YerithUtils::YERITH_LIGHT_GREEN_PALETTE(YERITH_GREEN_COLOR);

const QPalette YerithUtils::YERITH_GREEN_PALETTE(YERITH_YERITHGREEN_COLOR);

const QPalette YerithUtils::YERITH_ORANGE_PALETTE(YERITH_ORANGE_COLOR);

const QPalette YerithUtils::YERITH_INDIGO_PALETTE(YERITH_INDIGO_COLOR);

const QPalette YerithUtils::YERITH_RED_PALETTE(YERITH_RED_COLOR);

const QPalette YerithUtils::YERITH_WHITE_PALETTE(
		QColor(COLOUR_RGB_YERITH_WHITE_255_255_255));

const QLocale YerithUtils::frenchLocale(
		QLocale(QLocale::French, QLocale::France));

const QLocale YerithUtils::englishLocale(
		QLocale(QLocale::English, QLocale::Zambia));

const QString YerithUtils::TVA_0_STRING(STRING_TVA_0_PERCENT);

const QString YerithUtils::DATE_FORMAT(FORMAT_DATE);

const QString YerithUtils::TIME_FORMAT_WITH_MILLISECONDS(FORMAT_TIME_WITH_MS);

const QString YerithUtils::TIME_FORMAT(FORMAT_TIME);

const QString YerithUtils::DB_DATE_FORMAT(FORMAT_DATE_DATABASE);

#ifdef YERITH_FRANCAIS_LANGUAGE

const QString YerithUtils::STRING_OUI(STRING_TEXT_YES_FR);
const QString YerithUtils::STRING_NON(STRING_TEXT_NO_FR);

const QString YerithUtils::INFERIEUR_OU_EGAL(QObject::tr(STRING_MATH_INFERIOR_OR_EQUAL_FR));
const QString YerithUtils::SUPERIEUR(QObject::tr(STRING_MATH_SUPERIOR_FR));

#else //YERITH_ENGLISH_LANGUAGE

const QString YerithUtils::STRING_OUI(STRING_TEXT_YES_EN);
const QString YerithUtils::STRING_NON(STRING_TEXT_NO_EN);

const QString YerithUtils::INFERIEUR_OU_EGAL(
		QObject::tr(STRING_MATH_INFERIOR_OR_EQUAL_EN));
const QString YerithUtils::SUPERIEUR(QObject::tr(STRING_MATH_SUPERIOR_EN));

#endif

const QString YerithUtils::MAGELAN_CLIENT("yri.db-runtime.verif");

const QString YerithUtils::MAGELAN_CLIENT_OBJECT(
		"/YRI_DB_RUNTIME_VERIF_DBusClient");

const QString YerithUtils::MAGELAN_SERVER("com.yerith-erp.server");

const QString YerithUtils::MAGELAN_SERVER_OBJECT("/");

const QString YerithUtils::YERITH_ERP_3_0_LAST_BUILD_ID(
		"b13376b887f40fe3a752a4337b881210dbcc88ba");

const QString YerithUtils::MYSQL("mysql");

const QString YerithUtils::MYSQL_DUMP("mysqldump");

#ifdef YERITH_SERVER

	#ifdef YERITH_FRANCAIS_LANGUAGE

const QString YerithUtils::APROPOS_MSG("YERITH-PGI-3.0-SERVER"
								   "\n(VERSION du YERITH_BUILD_DATE à YERITH_BUILD_DATE_TIMESTAMP)"
								   "\n\n par \n\n"
								   "(c) (2015 - ...) Prof Dr Xavier Noundou.\n\n"
								   "LAST BUILD ID: ''.\n\n"
								   "ON BUILD_COMPUTER: .\n\n"
								   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
								   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerithUtils::APPLICATION_NAME("YERITH-PGI-3.0-SERVER");

	#else //YERITH_ENGLISH_LANGUAGE

const QString YerithUtils::APROPOS_MSG("YERITH-ERP-3.0-SERVER"
								   "\n(VERSION of YERITH_BUILD_DATE at YERITH_BUILD_DATE_TIMESTAMP)"
								   "\n\n by \n\n"
								   "(c) (2015 - ...) Prof Dr Xavier Noundou.\n\n"
								   "LAST BUILD ID: ''.\n\n"
								   "ON BUILD_COMPUTER: .\n\n"
								   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
								   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerithUtils::APPLICATION_NAME("YERITH-ERP-3.0-SERVER");

	#endif //YERITH_FRANCAIS_LANGUAGE
#endif

#ifdef YERITH_CLIENT
const QString YerithUtils::APROPOS_MSG(QObject::
                                       trUtf8
                                       ("YERITH-PGI-3.0-CLIENT"
                                        "\n(VERSION du YERITH_BUILD_DATE à YERITH_BUILD_DATE_TIMESTAMP)"
                                        "\n\n par \n\n"
                                        "(c) (2015 - ...) Prof Dr Xavier Noundou.\n\n"
                                        "LAST BUILD ID: ''.\n\n"
                                        "ON BUILD_COMPUTER: .\n\n"
                                        "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
                                        "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)"));
const
QString
YerithUtils::
APPLICATION_NAME
(QObject::
 trUtf8
 ("YERITH-ERP-3.0-CLIENT"));
#endif

#ifdef YERITH_STANDALONE

	#ifdef YERITH_FRANCAIS_LANGUAGE

const QString YerithUtils::APROPOS_MSG("YERITH-PGI-3.0-STANDALONE"
									   "\n(VERSION du YERITH_BUILD_DATE à YERITH_BUILD_DATE_TIMESTAMP)"
									   "\n\n par \n\n"
									   "(c) (2015 - ...) Prof Dr Xavier Noundou.\n\n"
									   "LAST BUILD ID: ''.\n\n"
									   "ON BUILD_COMPUTER: .\n\n"
									   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
									   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerithUtils::APPLICATION_NAME("YERITH-PGI-3.0-STANDALONE");

	#else //YERITH_ENGLISH_LANGUAGE

const QString YerithUtils::APROPOS_MSG("YERITH-ERP-3.0-STANDALONE"
									   "\n(VERSION of YERITH_BUILD_DATE at YERITH_BUILD_DATE_TIMESTAMP)"
									   "\n\n by \n\n"
									   "(c) (2015 - ...) Prof Dr Xavier Noundou.\n\n"
									   "LAST BUILD ID: ''.\n\n"
									   "ON BUILD_COMPUTER: .\n\n"
									   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
									   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerithUtils::APPLICATION_NAME("YERITH-ERP-3.0-STANDALONE");

	#endif //YERITH_FRANCAIS_LANGUAGE

#endif

#ifdef YERITH_ACADEMIC_EVALUATION_VERSION
const
QString
YerithUtils::
APROPOS_MSG
(QObject::
 trUtf8
 ("YERITH-PGI-3.0-ACADEMIC_EVALUATION"
  "\n(VERSION du YERITH_BUILD_DATE à YERITH_BUILD_DATE_TIMESTAMP)"
  "\n\n par \n\n"
  "(c) (2015 - ...) Prof Dr Xavier Noundou.\n\n"
  "LAST BUILD ID: ''.\n\n"
  "ON BUILD_COMPUTER: .\n\n"
  "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
  "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");
 const
 QString
 YerithUtils::
 APPLICATION_NAME
 (QObject::
  trUtf8
  ("YERITH-ERP-3.0-ACADEMIC_EVALUATION"));
#endif

const unsigned int YerithUtils::FACTURE_GRAND_STRING_MAX_CHARS(12);

const unsigned int YerithUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS(12);

const QString YerithUtils::MYSQL_TRUE_LITERAL("1");

const QString YerithUtils::MYSQL_FALSE_LITERAL("0");

const QString YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR(";");

const QString YerithUtils::STRING_FRENCH_DIVERS(QObject::tr("DIVERS"));

const QString YerithUtils::EMPTY_STRING("");

const QChar YerithUtils::DOT_CHAR('.');

const QChar YerithUtils::STAR_CHAR('*');

const QChar YerithUtils::DASH_CHAR('_');

const QChar YerithUtils::PERCENT_CHAR('%');

const QChar YerithUtils::SLASH_CHAR('/');

const QRegExp YerithUtils::PasswordRegExp("");

const QRegExp YerithUtils::LINE_ENDING_STRING_REGEXP("[\r\n]");

const QRegExp YerithUtils::EMPTY_SPACE_REGEXP("\\s");

const int YerithUtils::YERITH_PAGINATION_INT_VALIDATOR_MAXIMAL_VALUE = 999;

const QIntValidator YerithUtils::nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE_Validator(
		1, 12);

const QIntValidator YerithUtils::Int_BACKUP_YERITH_ERP_3_Validator(1, 172800);

const QRegExp YerithUtils::STRING_FOR_YERITH_ERP_3_0_VALIDATOR_DB_COLUMN_NAME(
		"a-zA-Z0-9_");

const QRegExpValidator YerithUtils::STRING_FOR_YERITH_ERP_3_0_IP_ADDRESS_VALIDATOR(
		QRegExp(
				QString("%1\\.%2\\.%3\\.%4").arg("[1-9]+", "[0-9]+", "[0-9]+",
						"[0-9]+")));

const QRegExpValidator YerithUtils::STRING_FOR_YERITH_ERP_3_0_VALIDATOR(
		QRegExp(
				QString("[%1%2%3]*").arg(
						"a-zA-Z0-9º:;,~´`@§$£#%\\s\\.\\^\\\\(\\)\\&/\<>\\?\\!\\{\\}\\[\\]\\+\\-\{\}=_'|",
						"àéèêôîûÀÉÈÊÔÛÎ", "äëöïüÄËÖÏÜ")));


const QIntValidator YerithUtils::UINT_VALIDATOR_GREATER_THAN_ZERO(1, 444444444);


const QIntValidator YerithUtils::UintValidator(0, 444444444);


const QDoubleValidator YerithUtils::DoubleValidator(-444444444, 444444444, 2);


const QDoubleValidator YerithUtils::POSITIVE_DoubleValidator(0, 444444444, 2);


const QRegExpValidator YerithUtils::PasswordValidator(PasswordRegExp);



void YerithUtils::RELOAD_VIEW_STRINGS()
{
    YerithUtils::_alerte_OUI_NON_ToUserViewString.clear();

    YerithUtils::_strategieindexToUserViewString.clear();

    YerithUtils::_titreToUserViewString.clear();

    YerithUtils::_roleToUserViewString.clear();

    YerithUtils::_typedecreditdebitToUserViewString.clear();

    YerithUtils::_typedoperationfinancieresToUserViewString.clear();

    YerithUtils::_typedeventeToUserViewString.clear();

    YerithUtils::_typedepaiementToUserViewString.clear();



	YerithUtils::_alerte_OUI_NON_ToUserViewString.insert(
			YerithUtils::ALERTE_RESOLUE_NON_INDEX, YerithUtils::STRING_NON);

	YerithUtils::_alerte_OUI_NON_ToUserViewString.insert(
			YerithUtils::ALERTE_RESOLUE_OUI_INDEX, YerithUtils::STRING_OUI);


	YerithUtils::_strategieindexToUserViewString.insert(
			YerithUtils::STRATEGIE_ALL_COMBOBOX_INDEX,
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

	YerithUtils::_strategieindexToUserViewString.insert(
			YerithUtils::STRATEGIE_FEFO_COMBOBOX_INDEX,
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

	YerithUtils::_strategieindexToUserViewString.insert(
			YerithUtils::STRATEGIE_FIFO_COMBOBOX_INDEX,
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

	YerithUtils::_strategieindexToUserViewString.insert(
			YerithUtils::STRATEGIE_LIFO_COMBOBOX_INDEX,
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);


	YerithUtils::_titreToUserViewString.insert(YerithUtils::TITRE_DR,
			QObject::tr("Dr"));

	YerithUtils::_titreToUserViewString.insert(YerithUtils::TITRE_ME,
			QObject::tr("Me"));

	YerithUtils::_titreToUserViewString.insert(YerithUtils::TITRE_MLLE,
			QObject::tr("Mlle"));

	YerithUtils::_titreToUserViewString.insert(YerithUtils::TITRE_MME,
			QObject::tr("Mme"));

	YerithUtils::_titreToUserViewString.insert(YerithUtils::TITRE_MR,
			QObject::tr("M"));

	YerithUtils::_titreToUserViewString.insert(YerithUtils::TITRE_PROF,
			QObject::tr("Prof"));

	YerithUtils::_titreToUserViewString.insert(YerithUtils::TITRE_INDEFINI,
			QObject::tr("titre indéfini"));


	YerithUtils::_roleToUserViewString.insert(YerithUtils::ROLE_ADMINISTRATEUR,
			QObject::tr("administrateur"));

	YerithUtils::_roleToUserViewString.insert(YerithUtils::ROLE_MANAGER,
			QObject::tr("manager"));

	YerithUtils::_roleToUserViewString.insert(
			YerithUtils::ROLE_GESTIONNAIREDESTOCK,
			QObject::tr("gestionnaire de (d'immobilisation) stock"));

	YerithUtils::_roleToUserViewString.insert(YerithUtils::ROLE_MAGASINIER,
			QObject::tr("magasinier"));

	YerithUtils::_roleToUserViewString.insert(YerithUtils::ROLE_CAISSIER,
			QObject::tr("caissier"));

	YerithUtils::_roleToUserViewString.insert(YerithUtils::ROLE_VENDEUR,
			QObject::tr("vendeur"));

	YerithUtils::_roleToUserViewString.insert(YerithUtils::ROLE_INDEFINI,
			QObject::tr("role indéfini"));


    YerithUtils::_typedecreditdebitToUserViewString.insert(
			YerithUtils::CREDIT_ENTREE_DARGENT, QObject::tr("crédit (entrée d'argent)"));

	YerithUtils::_typedecreditdebitToUserViewString.insert(
			YerithUtils::DEBIT_SORTIE_DARGENT,
			QObject::tr("débit (sortie d'argent)"));

	YerithUtils::_typedecreditdebitToUserViewString.insert(
			YerithUtils::CREDIT_DEBIT_INDEFINI,
			QObject::tr("indéfini"));


	YerithUtils::_typedoperationfinancieresToUserViewString.insert(
			YerithUtils::OPERATION_FINANCIERE_VENTE, QObject::tr("vente"));

	YerithUtils::_typedoperationfinancieresToUserViewString.insert(
			YerithUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE,
			QObject::tr("charge financière"));

	YerithUtils::_typedoperationfinancieresToUserViewString.insert(
			YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MATERIEL_IMMOBILISATIONS,
			QObject::tr("achat de matériels (immo.)"));

	YerithUtils::_typedoperationfinancieresToUserViewString.insert(
			YerithUtils::OPERATION_FINANCIERE_PAYEMENT_RESSOURCES_HUMAINES,
			QObject::tr("Payement des ressources humaines"));

	YerithUtils::_typedoperationfinancieresToUserViewString.insert(
			YerithUtils::OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES,
			QObject::tr("achat de marchandises"));

	YerithUtils::_typedoperationfinancieresToUserViewString.insert(
			YerithUtils::OPERATION_FINANCIERE_INDEFINI,
			QObject::tr("indéfini"));


	YerithUtils::_typedeventeToUserViewString.insert(
			YerithUtils::VENTE_COMPTANT, QObject::tr("vente (comptant)"));

	YerithUtils::_typedeventeToUserViewString.insert(
			YerithUtils::VENTE_COMPTECLIENT_AVANCE_DE_CASH,
			QObject::tr("vente (AVANCE DE CASH)"));

	YerithUtils::_typedeventeToUserViewString.insert(
			YerithUtils::VENTE_COMPTE_CLIENT,
			QObject::tr("vente (compte client)"));

	YerithUtils::_typedeventeToUserViewString.insert(
			YerithUtils::VENTE_INDEFINI, QObject::tr("vente indéfini"));



	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_AVANCE_DE_CASH,
			QObject::tr("versement (AVANCE DE CASH)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_COMPTANT,
			QObject::tr("versement (comptant)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_CHEQUE,
			QObject::tr("versement (cheque)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_TELEPHONE,
			QObject::tr("versement (téléphone)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_BANCAIRE,
			QObject::tr("versement (bancaire)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_VIREMENT_BANCAIRE,
			QObject::tr("versement (virement bancaire)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE,
			QObject::tr("versement (charge financière (annulée))"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::ENCAISSEMENT_INDEFINI,
			QObject::tr("versement indéfini"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_COMPTANT,
			QObject::tr("décaissement (comptant)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_CHEQUE,
			QObject::tr("décaissement (cheque)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_TELEPHONE,
			QObject::tr("décaissement (téléphone)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_BANCAIRE,
			QObject::tr("décaissement (bancaire)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_VIREMENT_BANCAIRE,
			QObject::tr("décaissement (virement bancaire)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT,
			QObject::tr("décaissement (retour achat)"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_INDEFINI,
			QObject::tr("décaissement indéfini"));

	YerithUtils::_typedepaiementToUserViewString.insert(
			YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT,
			QObject::tr("décaissement (fidélité clients)"));
}


void YerithUtils::YERITH_resizeEvent(QWidget &a_current_widget_to_move,
                                     const QSize &a_current_windows_SIZE,
                                     const QSize &a_current_windows_PREVIOUS_SIZE)
{
	if (a_current_windows_PREVIOUS_SIZE.width() <= 0
			|| a_current_windows_PREVIOUS_SIZE.height() <= 0)
	{
		return;
	}

	double NEW_X = (a_current_widget_to_move.x()
			/ (double) a_current_windows_PREVIOUS_SIZE.width())
			* a_current_windows_SIZE.width();
	double NEW_Y = (a_current_widget_to_move.y()
			/ (double) a_current_windows_PREVIOUS_SIZE.height())
			* a_current_windows_SIZE.height();
	a_current_widget_to_move.move(qCeil(NEW_X), qCeil(NEW_Y));
}

bool YerithUtils::creerNouveauClient(QString &proposedCustomerName_IN_OUT,
									 YerithWindowsCommons *_callingWindow /* = 0 */)
{
	if (proposedCustomerName_IN_OUT.isEmpty())
	{
		return false;
	}

	YerithSqlTableModel &customerSqlTableModel = _allWindows->getSqlTableModel_clients();

	QString customerTableFilter = QString("%1 = '%2'")
									.arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
										 proposedCustomerName_IN_OUT);

	customerSqlTableModel.yerithSetFilter_WITH_where_clause(
			customerTableFilter);

	int rows = customerSqlTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 870);

	if (rows > 0)
	{
		proposedCustomerName_IN_OUT.clear();

		proposedCustomerName_IN_OUT.append(
				GET_SQL_RECORD_DATA(customerSqlTableModel.record(0),
						YerithDatabaseTableColumn::NOM_ENTREPRISE));

		customerSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 880);

		return true;
	}
	else
	{
		customerSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 886);

		QSqlRecord record = customerSqlTableModel.record();

		record.setValue(YerithDatabaseTableColumn::ID,
				YerithERPWindows::getNextIdSqlTableModel_clients());

		record.setValue(YerithDatabaseTableColumn::LOCALISATION,
				YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

		record.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
				proposedCustomerName_IN_OUT);

		record.setValue(YerithDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
				0.0);

		record.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT, 0.0);

		QString retMsg(
				QObject::tr("L'entreprise cliente '%1'").arg(
						proposedCustomerName_IN_OUT));

		bool success = customerSqlTableModel.insertNewRecord(record,
															 0,
															 "src/utils/yerith-erp-utils.cpp",
															 908,
															 proposedCustomerName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));

			if (0 != _callingWindow)
			{
				YerithQMessageBox::warning(_callingWindow,
						QObject::tr("échec"), retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));
			if (0 != _callingWindow)
			{
				YerithQMessageBox::information(_callingWindow,
						QObject::tr("succès"), retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return true;
		}
	}

	return false;
}

bool YerithUtils::creerNouveauFournisseur(
		QString &proposedFournisseurName_IN_OUT,
		YerithWindowsCommons *_callingWindow /* = 0 */)
{
	if (proposedFournisseurName_IN_OUT.isEmpty())
	{
		return false;
	}

	YerithSqlTableModel &fournisseurSqlTableModel =
			_allWindows->getSqlTableModel_fournisseurs();

	QString fournisseurFilter = QString("%1 = '%2'").arg(
			YerithDatabaseTableColumn::NOM_ENTREPRISE,
			proposedFournisseurName_IN_OUT);

	fournisseurSqlTableModel.yerithSetFilter_WITH_where_clause(
			fournisseurFilter);

	int rows = fournisseurSqlTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 920);

	if (rows > 0)
	{
		proposedFournisseurName_IN_OUT.clear();

		proposedFournisseurName_IN_OUT.append(
				GET_SQL_RECORD_DATA(fournisseurSqlTableModel.record(0),
						YerithDatabaseTableColumn::NOM_ENTREPRISE));

		fournisseurSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 933);

		return true;
	}
	else
	{
		fournisseurSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 939);

		QSqlRecord record = fournisseurSqlTableModel.record();

		record.setValue(YerithDatabaseTableColumn::LOCALISATION,
				YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

		record.setValue(YerithDatabaseTableColumn::ID,
				YerithERPWindows::getNextIdSqlTableModel_fournisseurs());

		record.setValue(YerithDatabaseTableColumn::EMPLOYE, false);

		record.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
				proposedFournisseurName_IN_OUT);

		record.setValue(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR, 0.0);

		QString retMsg(
				QObject::tr("L'entreprise fournisseur '%1'").arg(
						proposedFournisseurName_IN_OUT));

		bool success = fournisseurSqlTableModel.insertNewRecord(record,
																0,
																"src/utils/yerith-erp-utils.cpp",
																1012,
																proposedFournisseurName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));

			if (0 != _callingWindow)
			{
				YerithQMessageBox::warning(_callingWindow,
						QObject::tr("échec"), retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			fournisseurSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 982);

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));

			if (0 != _callingWindow)
			{
				YerithQMessageBox::information(_callingWindow,
						QObject::tr("succès"), retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			fournisseurSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1003);

			return true;
		}
	}

	return false;
}

bool YerithUtils::creerNouveauDepartementDeProduits(
		QString &proposedProductDepartmentName_IN_OUT,
		YerithWindowsCommons *_callingWindow
		/* = 0 */
		)
{
	if (proposedProductDepartmentName_IN_OUT.isEmpty())
	{
		return false;
	}

	YerithSqlTableModel &departements_produits_SqlTableModel =
			_allWindows->getSqlTableModel_departements_produits();

	QString productDepartmentFilter = QString("%1 = '%2'").arg(
			YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			proposedProductDepartmentName_IN_OUT);

	departements_produits_SqlTableModel.yerithSetFilter_WITH_where_clause(
			productDepartmentFilter);

	int rows = departements_produits_SqlTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 1033);

	if (rows > 0)
	{
		proposedProductDepartmentName_IN_OUT.clear();

		proposedProductDepartmentName_IN_OUT.append(
				GET_SQL_RECORD_DATA(
						departements_produits_SqlTableModel.record(0),
						YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

		departements_produits_SqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1044);

		return true;
	}
	else
	{
		departements_produits_SqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1050);

		QSqlRecord record = departements_produits_SqlTableModel.record();

		record.setValue(YerithDatabaseTableColumn::ID,
				YerithERPWindows::getNextIdSqlTableModel_departements_produits());
		record.setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
				proposedProductDepartmentName_IN_OUT);
		record.setValue(
				YerithDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT,
				YerithUtils::EMPTY_STRING);
		QString retMsg(
				QObject::tr("Le département de produits '%1'").arg(
						proposedProductDepartmentName_IN_OUT));

		bool success = departements_produits_SqlTableModel.insertNewRecord(record,
																		   0,
																		   "src/utils/yerith-erp-utils.cpp",
																		   1118,
																		   proposedProductDepartmentName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));
			if (0 != _callingWindow)
			{
				YerithQMessageBox::warning(_callingWindow,
						QObject::tr("échec"), retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));
			if (0 != _callingWindow)
			{
				YerithQMessageBox::information(_callingWindow,
						QObject::tr("succès"), retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return true;
		}
	}

	return false;
}

bool YerithUtils::check_IF_departement_produit_exists(
		QString &proposedProductDepartmentName)
{
	return (YerithUtils::execQueryRowCount(
			QString("select %1 from %2 where %3='%4'").arg(
					YerithDatabaseTableColumn::ID,
					YerithDatabase::DEPARTEMENTS_PRODUITS,
					YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
					proposedProductDepartmentName)) > 0);
}

bool YerithUtils::creerNouvelleCategorie(QString &proposedCategorieName_IN_OUT,
		QString &proposedProductDepartmentName,
		YerithWindowsCommons *_callingWindow
		/* = 0 */
		)
{
	if (proposedCategorieName_IN_OUT.isEmpty()
			|| proposedProductDepartmentName.isEmpty())
	{
		return false;
	}

	if (!YerithUtils::check_IF_departement_produit_exists(
			proposedProductDepartmentName))
	{
		QString retMsg(
				QObject::tr("1 département de produits '%1' N'EXISTE PAS !").arg(
						proposedProductDepartmentName));
		if (0 != _callingWindow)
		{
			YerithQMessageBox::warning(_callingWindow, QObject::tr("échec"),
					retMsg);
		}
		else
		{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
            qDebug()
                    << retMsg;
#endif
		}

		return false;
	}

	YerithSqlTableModel &categorieSqlTableModel = _allWindows->getSqlTableModel_categories();

	QString categorieFilter = QString("%1 = '%2'")
								.arg(YerithDatabaseTableColumn::NOM_CATEGORIE,
									 proposedCategorieName_IN_OUT);

	categorieSqlTableModel.yerithSetFilter_WITH_where_clause(categorieFilter);

	int rows = categorieSqlTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 1218);

	if (rows > 0)
	{
		proposedCategorieName_IN_OUT.clear();

		proposedCategorieName_IN_OUT
			.append(GET_SQL_RECORD_DATA(categorieSqlTableModel.record(0),
										YerithDatabaseTableColumn::NOM_CATEGORIE));

		categorieSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1228);

		return true;
	}
	else
	{
		categorieSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1234);

		QSqlRecord record = categorieSqlTableModel.record();

		record.setValue(YerithDatabaseTableColumn::ID,
						YerithERPWindows::getNextIdSqlTableModel_categories());

		record.setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
						proposedProductDepartmentName);

		record.setValue(YerithDatabaseTableColumn::NOM_CATEGORIE,
						proposedCategorieName_IN_OUT);

		record.setValue(YerithDatabaseTableColumn::DESCRIPTION_CATEGORIE,
						YerithUtils::EMPTY_STRING);

		QString retMsg = QObject::tr("La catégorie '%1'")
							.arg(proposedCategorieName_IN_OUT);

		bool success = categorieSqlTableModel.insertNewRecord(record,
															  0,
															  "src/utils/yerith-erp-utils.cpp",
															  1253,
															  proposedCategorieName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));

			if (0 != _callingWindow)
			{
				YerithQMessageBox::warning(_callingWindow,
										   QObject::tr("échec"),
										   retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug() << retMsg;
#endif
			}

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));

			if (0 != _callingWindow)
			{
				YerithQMessageBox::information(_callingWindow,
											   QObject::tr("succès"),
											   retMsg);
			}
			else
			{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST
                qDebug() << retMsg;
#endif
			}

			return true;
		}
	}

	return false;
}

bool YerithUtils::GREP_YERITH_FILE_CONTENT(const QString &file_full_path,
										   const QString &EXECUTABLE_full_path,
										   const QString &keyword_IN)
{
	QStringList progArguments;

	progArguments << keyword_IN << file_full_path;

	QProcess GREP_PROCESS;

	bool checkProcessFinished = YerithERPProcess::startAndWaitForFinished(
			GREP_PROCESS, "/bin/grep", progArguments, 1000);
	if (checkProcessFinished)
	{
		QString checkAlertDeamonProcessOutput(
				GREP_PROCESS.readAllStandardOutput().trimmed());

		return checkAlertDeamonProcessOutput.contains(EXECUTABLE_full_path);
	}

	return false;
}

void YerithUtils::YERITH_READ_FILE_CONTENT(QFile &file, QString &fileContentVar)
{
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
		QString line;
		do
		{
			line = stream.readLine().trimmed();
			fileContentVar.append(line).append("\n");
		} while (!line.isNull());
		file.close();
	}
}

bool YerithUtils::YERITH_CREATE_FOLDER(const QString &aFullPathDir,
		YerithLogger *aLogger
		/* = 0 */
		)
{
	QFileInfo aFullPathDirInfo(aFullPathDir);
	if (!aFullPathDirInfo.exists())
	{
		if (0 != aLogger)
		{
			aLogger->log(
					"YerithUtils::YERITH_CREATE_FOLDER | read_yerith_configuration",
					QString("Folder '%1' for temporary files does not exist!").arg(
							aFullPathDir));
		}

		QDir tempFileDir;
		if (tempFileDir.mkpath(aFullPathDir))
		{
			if (0 != aLogger)
			{
				aLogger->log(
						"YerithUtils::YERITH_CREATE_FOLDER | read_yerith_configuration",
						QString("Created folder '%1' for temporary files").arg(
								aFullPathDir));
			}

			return true;
		}
		else
		{
			if (0 != aLogger)
			{
				aLogger->log(
						"YerithUtils::YERITH_CREATE_FOLDER | read_yerith_configuration",
						QString(
								"Could not Create folder '%1' for temporary files!").arg(
								aFullPathDir));
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}

bool YerithUtils::GZIP_YERITH_FILE(
		const QString &program_working_directory_full_path,
		const QString &file_full_path)
{
	QStringList gzipPROG_ARGS;
	gzipPROG_ARGS.append(file_full_path);
	int file_size =
			YerithERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE(
					"/usr/bin/gzip", program_working_directory_full_path,
					QString("%1.gz").arg(file_full_path), gzipPROG_ARGS);
	return (file_size >= 0);
}

void YerithUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(
		QString &SEPARATED_RESULTING_QSTRING_IN_OUT,
		const QString &aCur_db_STRING)
{
	QStringList separated_resulting_qstringlist;
	YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING(separated_resulting_qstringlist,
			SEPARATED_RESULTING_QSTRING_IN_OUT);
	SEPARATED_RESULTING_QSTRING_IN_OUT.clear();
	separated_resulting_qstringlist.removeAll(aCur_db_STRING);
	uint SIZE = separated_resulting_qstringlist.size();
	for (uint i = 0; i < SIZE; ++i)
	{
		if (i < SIZE - 1)
		{
			SEPARATED_RESULTING_QSTRING_IN_OUT.append(
					QString("%1%2").arg(separated_resulting_qstringlist.at(i),
							YerithUtils::STAR_CHAR));
		}
		else
		{
			SEPARATED_RESULTING_QSTRING_IN_OUT.append(
					separated_resulting_qstringlist.at(i));
		}
	}
}

/**
 * returns true if '*' separated string has REALLY GOT UPDATED WITH A NEW STRING VALUE 'aNewValue' !
 */
bool YerithUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(
		const QString &aNewValue,
		QString &aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT)
{
	if (!aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.isEmpty())
	{
		if (YerithUtils::CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING(
				aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT, aNewValue))
		{
			return false;
		}

		if (aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.endsWith("*"))
		{
			aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.append(aNewValue);
		}
		else
		{
			aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.append(
					QString("%1%2").arg("*", aNewValue));
		}
	}
	else
	{
		aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.append(aNewValue);
	}

	return true;
}

enum service_stock_already_exist_type YerithUtils::IS_STOCK_DESIGNATION_OR_REFERENCE_UNIQUE(
		const QString &aStockServiceReference,
		const QString &aStockServiceCategory,
		const QString &aStockServiceDesignation,
		QString &curExistingReferenceCategoryDesignation_PRODUCT_in_out)
{
	curExistingReferenceCategoryDesignation_PRODUCT_in_out.clear();
	QSqlRecord record;
	int marchandisesTableModelRowCount = 0;
	YerithSqlTableModel &marchandisesTableModel =
			_allWindows->getSqlTableModel_marchandises();
	/** Verifies stock reference **/
	{
		marchandisesTableModel.yerithSetFilter_WITH_where_clause(
				QString("%1 = '%2'").arg(YerithDatabaseTableColumn::REFERENCE,
						aStockServiceReference));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 1389);

		record.clear();
		QString stockDesignation;
		QString stockReference;
		for (int k = 0; k < marchandisesTableModelRowCount; ++k)
		{
			record = marchandisesTableModel.record(k);
			stockDesignation = GET_SQL_RECORD_DATA(record,
					YerithDatabaseTableColumn::DESIGNATION);
			stockReference = GET_SQL_RECORD_DATA(record,
					YerithDatabaseTableColumn::REFERENCE);
			if (YerithUtils::isEqualCaseInsensitive(stockReference,
					aStockServiceReference))
			{
				if (!YerithUtils::isEqualCaseInsensitive(aStockServiceCategory,
						GET_SQL_RECORD_DATA(record,
								YerithDatabaseTableColumn::CATEGORIE))
						|| !YerithUtils::isEqualCaseInsensitive(
								aStockServiceDesignation,
								GET_SQL_RECORD_DATA(record,
										YerithDatabaseTableColumn::DESIGNATION)))
				{
					curExistingReferenceCategoryDesignation_PRODUCT_in_out =
							stockDesignation;

					marchandisesTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1438);

					return SERVICE_REFERENCE_EXISTS;
				}
			}
		}
	}

	/** Verifies stock designation **/
	{
		marchandisesTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1448);

		record.clear();
		QString stockDesignation;
		marchandisesTableModel.yerithSetFilter_WITH_where_clause(
				QString("%1 = '%2'").arg(YerithDatabaseTableColumn::DESIGNATION,
						aStockServiceDesignation));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 1431);

		for (int k = 0; k < marchandisesTableModelRowCount; ++k)
		{
			record = marchandisesTableModel.record(k);
			stockDesignation = GET_SQL_RECORD_DATA(record,
					YerithDatabaseTableColumn::DESIGNATION);
			if (YerithUtils::isEqualCaseInsensitive(stockDesignation,
					aStockServiceDesignation))
			{
				if (!YerithUtils::isEqualCaseInsensitive(aStockServiceCategory,
						GET_SQL_RECORD_DATA(record,
								YerithDatabaseTableColumn::CATEGORIE))
						|| !YerithUtils::isEqualCaseInsensitive(
								aStockServiceReference,
								GET_SQL_RECORD_DATA(record,
										YerithDatabaseTableColumn::REFERENCE)))
				{
					curExistingReferenceCategoryDesignation_PRODUCT_in_out =
							stockDesignation;

					marchandisesTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1478);

					return SERVICE_STOCK_DESIGNATION_EXIST;
				}
			}
		}
	}

	/** Verifies stock category **/
	{
		marchandisesTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1488);

		record.clear();
		QString serviceStockCategory;
		/**
		 * IT IS SUFFICIENT TO ONLY CHECK AGAINST STOCK DESIGNATION
		 * BECAUSE IT IS ONLY IT THAT MUST BE NON EMPTY.
		 *
		 * STOCK REFERENCE COULD BE EMPTY AT TIME!
		 */
		marchandisesTableModel.yerithSetFilter_WITH_where_clause(
				QString("%1 = '%2' AND %3 != '%4'").arg(
						YerithDatabaseTableColumn::DESIGNATION,
						aStockServiceDesignation,
						YerithDatabaseTableColumn::CATEGORIE,
						aStockServiceCategory));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 1477);

		for (int k = 0; k < marchandisesTableModelRowCount; ++k)
		{
			record = marchandisesTableModel.record(k);
			serviceStockCategory = GET_SQL_RECORD_DATA(record,
					YerithDatabaseTableColumn::CATEGORIE);
			if (!YerithUtils::isEqualCaseInsensitive(serviceStockCategory,
					aStockServiceCategory))
			{
				curExistingReferenceCategoryDesignation_PRODUCT_in_out =
						serviceStockCategory;

				marchandisesTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1519);

				return SERVICE_STOCK_CATEGORY_EXIST;
			}
		}
	}

	/** Verifies whether merchandise already exists **/
	{
		marchandisesTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1528);

		marchandisesTableModel.yerithSetFilter_WITH_where_clause(
				QString("%1 = '%2' AND %3 = '%4' AND %5='%6'").arg(
						YerithDatabaseTableColumn::REFERENCE,
						aStockServiceReference,
						YerithDatabaseTableColumn::DESIGNATION,
						aStockServiceDesignation,
						YerithDatabaseTableColumn::CATEGORIE,
						aStockServiceCategory));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 1508);

		if (marchandisesTableModelRowCount > 0)
		{
			return SERVICE_STOCK_EXIST_IN_MERCHANDISE_DB_TABLE;
		}
	}

	marchandisesTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1548);
	return SERVICE_STOCK_UNDEFINED;
}

QString YerithUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(int day_of_week)
{
	switch (day_of_week)
	{
	case 1:
		return YerithMainWindow::STRING_MONDAY;

	case 2:
		return YerithMainWindow::STRING_TUESDAY;

	case 3:
		return YerithMainWindow::STRING_WEDNESDAY;

	case 4:
		return YerithMainWindow::STRING_THURSDAY;

	case 5:
		return YerithMainWindow::STRING_FRIDAY;

	case 6:
		return YerithMainWindow::STRING_SATURDAY;

	case 7:
		return YerithMainWindow::STRING_SUNDAY;

	default:
		return YerithUtils::EMPTY_STRING;
	}
}

bool YerithUtils::UPDATE_PREVIOUS_SELLING_PRICE_IN_ProductList(
		const YerithERPStockMarchandiseData &aServiceStockData,
		YerithWindowsCommons *_callingWindow /* = 0 */)
{
	bool success = false;
	QString stockItemUpdateSellingPriceQuery(
			QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").arg(
					YerithDatabase::MARCHANDISES,
					YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
					aServiceStockData._prix_vente_precedent,
					YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT,
					aServiceStockData._prix_vente_en_gros_precedent,
					YerithDatabaseTableColumn::DESIGNATION,
					aServiceStockData._designation));

	success = YerithUtils::execQuery(stockItemUpdateSellingPriceQuery);

	return success;
}

bool YerithUtils::UPDATE_PREVIOUS_BUYING_PRICE_IN_ProductList(
		const YerithERPStockMarchandiseData &aServiceStockData,
		YerithWindowsCommons *_callingWindow /* = 0 */)
{
	bool ret_value = YerithUtils::execQuery(
			QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg(
					YerithDatabase::MARCHANDISES,
					YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
					aServiceStockData._prix_dachat_precedent,
					YerithDatabaseTableColumn::DESIGNATION,
					aServiceStockData._designation), 0);

	return ret_value;
}

bool YerithUtils::insertStockItemInProductList(
		YerithERPStockMarchandiseData &aServiceStockData_IN_OUT,
		YerithWindowsCommons *_callingWindow /* = 0 */)
{
	bool success = false;

	if (!YerithUtils::creerNouvelleCategorie(
			aServiceStockData_IN_OUT._categorie,
			aServiceStockData_IN_OUT._nom_departement_produit, _callingWindow))
	{
		QString retMsg(
				QObject::tr(
						"La désignation de la catégorie ne doit pas être vide !"));

		if (0 != _callingWindow)
		{
			YerithQMessageBox::warning(_callingWindow,
					QObject::tr("création d'une catégorie"), retMsg);
		}
		else
		{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}

		return false;
	}

	YerithSqlTableModel &productListSqlTableModel =
			_allWindows->getSqlTableModel_marchandises();

	QSqlRecord record = productListSqlTableModel.record();

	record.setValue(YerithDatabaseTableColumn::ID,
			YerithERPWindows::getNextIdSqlTableModel_marchandises());

	record.setValue(YerithDatabaseTableColumn::IS_SERVICE,
			aServiceStockData_IN_OUT._isService);

	if (aServiceStockData_IN_OUT._isService)
	{
		record.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
				aServiceStockData_IN_OUT._nom_entreprise_client);
	}

	record.setValue(YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
			YerithUtils::YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(
					aServiceStockData_IN_OUT._prix_dachat_precedent));

	record.setValue(YerithDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT,
			YerithUtils::YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(
					aServiceStockData_IN_OUT._prix_vente_en_gros_precedent));

	record.setValue(YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
			YerithUtils::YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(
					aServiceStockData_IN_OUT._prix_vente_precedent));

	record.setValue(YerithDatabaseTableColumn::REFERENCE,
			aServiceStockData_IN_OUT._reference);

	record.setValue(YerithDatabaseTableColumn::DESIGNATION,
			aServiceStockData_IN_OUT._designation);

	record.setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			aServiceStockData_IN_OUT._nom_departement_produit);

	record.setValue(YerithDatabaseTableColumn::CATEGORIE,
			aServiceStockData_IN_OUT._categorie);

	record.setValue(YerithDatabaseTableColumn::DESCRIPTION_PRODUIT,
			aServiceStockData_IN_OUT._description);

	if (!aServiceStockData_IN_OUT._designation.isEmpty())
	{
		if (productListSqlTableModel.Is_SearchQSqlTable(
				YerithDatabaseTableColumn::REFERENCE,
				aServiceStockData_IN_OUT._reference) > 0
				|| productListSqlTableModel.Is_SearchQSqlTable(
						YerithDatabaseTableColumn::DESIGNATION,
						aServiceStockData_IN_OUT._designation) > 0)
		{
			productListSqlTableModel.yerithSetFilter_WITH_where_clause(
					QString("%1='%2' AND %3='%4' AND %5='%6'")
						.arg(YerithDatabaseTableColumn::REFERENCE,
							 aServiceStockData_IN_OUT._reference,
							 YerithDatabaseTableColumn::DESIGNATION,
							 aServiceStockData_IN_OUT._designation,
							 YerithDatabaseTableColumn::CATEGORIE,
							 aServiceStockData_IN_OUT._categorie));

			int rows = productListSqlTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 1770);

			if (rows > 0)
			{
				record.clear();

				record = productListSqlTableModel.record(0);

				QSqlRecord aMarchandiseUpdateRecord(record);

				if (!aServiceStockData_IN_OUT._prix_dachat_precedent.isEmpty())
				{
					aMarchandiseUpdateRecord.setValue(
							YerithDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
							YerithUtils::YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED
										(aServiceStockData_IN_OUT._prix_dachat_precedent));
				}

				if (!aServiceStockData_IN_OUT._prix_vente_precedent.isEmpty())
				{
					aMarchandiseUpdateRecord
						.setValue(YerithDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
								  YerithUtils::YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED
								  	  (aServiceStockData_IN_OUT._prix_vente_precedent));
				}

				productListSqlTableModel.updateRecord(0,
													  aMarchandiseUpdateRecord,
													  "src/utils/yerith-erp-utils.cpp",
													  1796,
													  aServiceStockData_IN_OUT._designation);


				productListSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 1803);

				return true;
			}
		}
	}

	success = productListSqlTableModel.insertNewRecord(record,
													   0,
													   "src/utils/yerith-erp-utils.cpp",
													   1810,
													   aServiceStockData_IN_OUT._designation);

	QString retMsg(
			QObject::tr("Le stock '%1'").arg(
					aServiceStockData_IN_OUT._designation));

	if (success)
	{
		retMsg.append(
				QObject::tr(
						" a été enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerithQMessageBox::information(_callingWindow,
					QObject::tr("enregistrement de l'article type "
							"dans la liste des marchandises"), retMsg);
		}
		else
		{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}
	else
	{
		retMsg.append(
				QObject::tr(
						" n'a pas pu être enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerithQMessageBox::warning(_callingWindow,
					QObject::tr("échec de l'enregistrement de "
							"l'article type dans la liste des marchandises"),
					retMsg);
		}
		else
		{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}

	return success;
}

bool YerithUtils::insert_SERVICE_ItemInProductList(
		YerithERPServiceData &aServiceStockData_IN_OUT,
		YerithWindowsCommons *_callingWindow /* = 0 */)
{
	bool success = false;

	if (!YerithUtils::creerNouvelleCategorie(
			aServiceStockData_IN_OUT._categorie,
			aServiceStockData_IN_OUT._nom_departement_produit, _callingWindow))
	{
		QString retMsg(
				QObject::tr(
						"La désignation de la catégorie ne doit pas être vide !"));

		if (0 != _callingWindow)
		{
			YerithQMessageBox::warning(_callingWindow,
					QObject::tr("création d'une catégorie"), retMsg);
		}
		else
		{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}

		return false;
	}

	YerithSqlTableModel &productListSqlTableModel =
			_allWindows->getSqlTableModel_marchandises();

	QSqlRecord record = productListSqlTableModel.record();

	record.setValue(YerithDatabaseTableColumn::ID,
			YerithERPWindows::getNextIdSqlTableModel_marchandises());

	record.setValue(YerithDatabaseTableColumn::IS_SERVICE,
			YerithUtils::MYSQL_TRUE_LITERAL);

	record.setValue(YerithDatabaseTableColumn::REFERENCE,
			aServiceStockData_IN_OUT._reference);

	record.setValue(YerithDatabaseTableColumn::DESIGNATION,
			aServiceStockData_IN_OUT._designation);

	record.setValue(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			aServiceStockData_IN_OUT._nom_departement_produit);

	record.setValue(YerithDatabaseTableColumn::CATEGORIE,
			aServiceStockData_IN_OUT._categorie);

	record.setValue(YerithDatabaseTableColumn::DESCRIPTION_PRODUIT,
			aServiceStockData_IN_OUT._description);

	success = productListSqlTableModel.insertNewRecord(record,
													   0,
													   "src/utils/yerith-erp-utils.cpp",
													   1923,
													   aServiceStockData_IN_OUT._nom_departement_produit);


	QString retMsg = QObject::tr("Le service '%1 (%2)'")
						.arg(aServiceStockData_IN_OUT._designation,
							 aServiceStockData_IN_OUT._reference);


	if (success)
	{
		retMsg.append(
				QObject::tr(
						" a été enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerithQMessageBox::information(_callingWindow,
					QObject::tr("enregistrement de l'article type "
								"dans la liste des marchandises"),
								retMsg);
		}
		else
		{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}
	else
	{
		retMsg.append(
				QObject::tr(
						" n'a pas pu être enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerithQMessageBox::warning(_callingWindow,
					QObject::tr("échec de l'enregistrement de "
							"l'article type dans la liste des marchandises"),
					retMsg);
		}
		else
		{
#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}

	return success;
}

QString YerithUtils::YERITH_TRUNCATE_STRING_ACCORDING_TO_SETTING(
		const QString &aString_IN)
{
	QString aString_OUT(aString_IN);

	if (aString_OUT.length() > YerithERPConfig::max_string_display_length)
	{
		aString_OUT.truncate(YerithERPConfig::max_string_display_length);

		aString_OUT.append(".");
	}

	return aString_OUT;
}

QString YerithUtils::GET_STRING_WITH_NUMBERS_WITHIN_KEPT_FOR_LATEX(
		const QString &aStringWITHBLANKSPACEWITHIN)
{
	QString resultString(aStringWITHBLANKSPACEWITHIN);

	unsigned int nextPosIn_RESULT_STRING = 0;

	for (unsigned int k = 0; k < aStringWITHBLANKSPACEWITHIN.length(); ++k)
	{
		if (aStringWITHBLANKSPACEWITHIN.at(k).isDigit())
		{
			resultString.replace(nextPosIn_RESULT_STRING, 1,
					QString("$%1$").arg(aStringWITHBLANKSPACEWITHIN.at(k)));

			nextPosIn_RESULT_STRING += 3;
		}
		else
		{
			nextPosIn_RESULT_STRING += 1;
		}
	}

	return resultString;
}

double YerithUtils::get_prix_dachat_wheter_exists(const QString &stocksID)
{
	QString prixDachatQueryString(
			QString("SELECT %1 FROM %2 WHERE %3='%4'").arg(
					YerithDatabaseTableColumn::PRIX_DACHAT,
					YerithDatabase::ACHATS,
					YerithDatabaseTableColumn::STOCKS_ID, stocksID));

	//    QDEBUG_STRING_OUTPUT_2("prixDachatQueryString", prixDachatQueryString);

	QSqlQuery prixDachatQuery;

	int achatQuerySize = YerithUtils::execQuery(prixDachatQuery,
			prixDachatQueryString);

	double prix_dachat = 0.0;

	if (achatQuerySize > 0 && prixDachatQuery.next())
	{
		prix_dachat = prixDachatQuery.value(0).toDouble();
	}

	return prix_dachat;
}

bool YerithUtils::REMOVE_QSTRING_LIST(
		QStringList &a_qstring_list_to_remove_elements,
		QStringList &a_qstring_list)
{
	bool ret_value = false;

	for (uint i = 0; i < a_qstring_list.size(); ++i)
	{
		ret_value = ret_value
				|| a_qstring_list_to_remove_elements.removeOne(
						a_qstring_list.at(i));
	}

	return ret_value;
}

bool YerithUtils::start_db_transaction()
{
	QSqlDriver *driver = _allWindows->getDatabase().driver();

	if (0 != driver && driver->hasFeature(QSqlDriver::Transactions))
	{
		bool couldStartDBTransaction = _allWindows->getDatabase().transaction();

		if (!couldStartDBTransaction)
		{
			qDebug() << QString("Couldn't start a database transaction !");

			qDebug() << _allWindows->getDatabase().lastError();
		}

		return couldStartDBTransaction;
	}

	return false;
}

bool YerithUtils::rollback_db_transaction()
{
	bool could_rollback_db_transaction = _allWindows->getDatabase().roolback();

	if (!could_rollback_db_transaction)
	{
		qDebug() << QString("Couldn't roolback a database transaction !");

		qDebug() << _allWindows->getDatabase().lastError();
	}

	return could_rollback_db_transaction;
}

bool YerithUtils::commit_db_transaction()
{
	bool couldCommitDBTransaction = _allWindows->getDatabase().commit();

	if (!couldCommitDBTransaction)
	{
		qDebug() << QString("Couldn't commit a database transaction !");

		qDebug() << _allWindows->getDatabase().lastError();
	}

	return couldCommitDBTransaction;
}

void YerithUtils::setAllWindows(YerithERPWindows *allWindows)
{
	_allWindows = allWindows;

	YerithTableView::YERITH_TABLE_VIEW_ALL_WINDOWS_POINTER = allWindows;

	YerithPOSDialogCommons::setAllWindows(allWindows);

	YerithWindowsCommons::setAllWindows(allWindows);

	YerithPOSAdminWindowsCommons::setAllWindows(allWindows);
}

YerithDatabase* YerithUtils::getDatabase()
{
	if (0 != _allWindows)
	{
		return &_allWindows->getDatabase();
	}

	return 0;
}

void YerithUtils::writeStringToQFilewithUTF8Encoding(
		QFile &latexContentOutputFile, QString aLatexFileContentString)
{
	if (latexContentOutputFile.open(QFile::WriteOnly))
	{
		latexContentOutputFile.write(aLatexFileContentString.toUtf8());
	}

	latexContentOutputFile.close();
}

const QString YerithUtils::getCurrentAdminWindowReceiptsFormatAccordingToLanguage(
		const QString &currentReceiptFormatValue)
{
	QString resultReceiptFormat(currentReceiptFormatValue);

#ifdef YERITH_ENGLISH_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
                                            QObject::tr(RECEIPT_FORMAT_SMALL_FR)))
    {
        resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_SMALL_EN);
    }
    else if (YerithUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
                                                 QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_FR)))
    {
        resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_EN);
    }

#else

	if (YerithUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
			QObject::tr(RECEIPT_FORMAT_SMALL_EN)))
	{
		resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_SMALL_FR);
	}
	else if (YerithUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
			QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_EN)))
	{
		resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_FR);
	}

#endif

	return resultReceiptFormat;
}

void YerithUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage(
		const QString &currentFacturesTypeValue)
{
	//qDebug() << QString("YerithUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage");

	assert(0 != _allWindows);

	QString currentFacturesTypeValue_FR(YerithERPConfig::receiptFormat);

#ifdef YERITH_ENGLISH_LANGUAGE

    if (YerithUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
                                            QObject::tr(RECEIPT_FORMAT_SMALL_EN)))
    {
        currentFacturesTypeValue_FR = QObject::tr(RECEIPT_FORMAT_SMALL_FR);
    }
    else if (YerithUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
                                                 QObject::tr(RECEIPT_FORMAT_LARGE_A5PAPER_EN)))
    {
        currentFacturesTypeValue_FR = QObject::tr(RECEIPT_FORMAT_LARGE_A5PAPER_FR);
    }
    else if (YerithUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
                                                 QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_EN)))
    {
        currentFacturesTypeValue_FR = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_FR);
    }

#else

	currentFacturesTypeValue_FR = currentFacturesTypeValue;

#endif

	YerithERPConfig::receiptFormat = currentFacturesTypeValue;
//    qDebug() << QString("++ saveCu... currentFacturesTypeValue_FR: %1")
//                              .arg(currentFacturesTypeValue_FR);
}


bool YerithUtils::executer_fichier_sql(const QString &fileName,
                                       YerithLogger  *logger)
{
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//QDEBUG_STRING_OUTPUT_2("YerithUtils::executer_fichier_sql",
        //                        fileName);

		QMessageBox::critical(0, QObject::tr("Cannot open file"),
				QObject::tr(
						"Unable to open file establish a database connection.\n"
								"This example needs SQLite support. Please read "
								"the Qt SQL driver documentation for information how "
								"to build it.\n\n"
								"Click Cancel to exit."), QMessageBox::Cancel);

		return false;
	}

	static QSqlQuery query;

	QTextStream in(&file);

	query.clear();

	QString line;

	bool success = false;

	while (!in.atEnd())
	{
		line = in.readLine();

		success = query.exec(line);

		if (logger)
		{
			logger->log("YerithUtils::executer_fichier_sql",
					QString("[%1] %2").arg(
							success ?
									BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE,
							line));
		}

		if (!success)
		{
			if (logger)
			{
				logger->log("YerithUtils::executer_fichier_sql",
						QString("[reason for failing] %1").arg(
								query.lastError().text()));
			}
		}
	}

	return true;
}


double YerithUtils::YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(const QString &aDoubleQString)
{
	double aDoubleNum = 0.0;

	bool ok = true;

	aDoubleNum = aDoubleQString.toDouble(&ok);

	if (false == ok)
	{
		if (YerithMainWindow::LANGUE_ANGLAISE)
		{
			aDoubleNum = YerithUtils::englishLocale.toDouble(aDoubleQString, &ok);
		}
		else
		{
			aDoubleNum = YerithUtils::frenchLocale.toDouble(aDoubleQString, &ok);
		}
	}

	return aDoubleNum;
}


int YerithUtils::execQueryRowCount(const QString &strQuery,
								   YerithLogger  *logger)
{
	//qDebug() << "[YerithUtils][execQuery]";

	QSqlQuery query;

	query.prepare(strQuery);

	bool success = query.exec();
	//qDebug() << "\t[" << success << "]" << query.executedQuery();

	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << "\t\t[reason for failing] " << query.lastError().text()
					<< "\n";
		}
		else
		{
			qDebug() << "\t\t[reason for failing] " << query.lastError();
		}
	}

	return query.size();
}

void YerithUtils::qDebugStrings(const QString &firstString,
		const QStringList &aStringList)
{
	qDebug() << QString("++ %1: ").arg(firstString) << aStringList;
}

void YerithUtils::qDebugStrings(const QString &firstString,
		const QString &secondString /* = YerithUtils::EMPTY_STRING */)
{
	qDebug() << QString("++ %1: %2").arg(firstString, secondString);
}

bool YerithUtils::execQuery(const QString &strQuery, YerithLogger *logger)
{
	//qDebug() << "[YerithUtils][execQuery]";

	QSqlQuery query;

	query.prepare(strQuery);

	bool success = query.exec();

	//qDebug() << "\t[" << success << "]" << query.executedQuery();
	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << "\t\t[reason for failing] " << query.lastError().text()
					<< "\n";
		}
		else
		{
			qDebug() << "\t\t" << strQuery;

			qDebug() << "\t\t[reason for failing] " << query.lastError().text();
		}
	}

	return success;
}

int YerithUtils::execQuery(QSqlQuery &query, const QString &strQuery,
		YerithLogger *logger)
{
	//qDebug() << "[YerithUtils][execQuery]";
	query.prepare(strQuery);

	bool success = query.exec();

	//qDebug() << "\t[" << success << "]" << query.executedQuery();

	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << strQuery << "\t\t[reason for failing] "
					<< query.lastError().text() << "\n";
		}
		else
		{
			qDebug() << "\t\t" << strQuery;

			qDebug() << "\t\t[reason for failing] " << query.lastError().text();
		}

		return -1;
	}

	return query.size();
}

int YerithUtils::execQuery(QSqlQuery &query, YerithLogger *logger)
{
	//qDebug() << "[YerithUtils][execQuery]";

	bool success = query.exec();
	//qDebug() << "\t[" << success << "]" << query.executedQuery();

	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << "\t\t[reason for failing] " << query.lastError().text()
					<< "\n";
		}
		else
		{
			qDebug() << "\t\t[reason for failing] " << query.lastError().text();
		}

		return -1;
	}

	return query.size();
}

void YerithUtils::loadPixmapFromDB(QLabel &label_image, QVariant imageVariant,
		const char *format)
{
	QPixmap pixmap;

	pixmap.loadFromData(imageVariant.toByteArray(), format);

	label_image.setAlignment(Qt::AlignCenter);

	label_image.setFrameShape(QFrame::NoFrame);

	label_image.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	label_image.setBackgroundRole(QPalette::Background);

	label_image.setAutoFillBackground(true);

	label_image.setScaledContents(true);

	label_image.setPixmap(pixmap);
}

void YerithUtils::savePixmapToByteArray(QByteArray &bytes,
		const QPixmap &pixmap, const char *format)
{
	QBuffer buffer(&bytes);

	buffer.open(QIODevice::WriteOnly);

	pixmap.save(&buffer, format);
}

void YerithUtils::savePixmapToFile(const QString &aFileFullPath,
		const QPixmap &pixmap, const char *format)
{
	QFile aFile(aFileFullPath);

	aFile.open(QIODevice::WriteOnly);

	pixmap.save(&aFile, format);
}

void YerithUtils::supprimer_image(QLabel &aLabel_image_produit)
{
	aLabel_image_produit.clear();

	aLabel_image_produit.setAutoFillBackground(false);
}

void YerithUtils::selectionner_image(QWidget *parent, QLabel &label_image)
{
	QString path;
	path =
			QFileDialog::getOpenFileName(parent, "Choisir une photo du produit",
					QString::null,
					QObject::tr(
							"Images (*.png *.xpm *.jpg *.jpeg *.gif *.PNG *.XPM *.JPG *.JPEG *.GIF)"));

	QImageReader image_reader(path);

	QImage image;

	image_reader.setAutoTransform(true);

	if (image_reader.read(&image))
	{
		QPixmap pixMap(QPixmap::fromImage(image));

		label_image.setAlignment(Qt::AlignCenter);

		label_image.setFrameShape(QFrame::NoFrame);

		label_image.setSizePolicy(QSizePolicy::Expanding,
				QSizePolicy::Expanding);

		label_image.setBackgroundRole(QPalette::Background);

		label_image.setAutoFillBackground(true);

		label_image.setScaledContents(true);

		label_image.setPixmap(pixMap);

		label_image.setEnabled(true);
	}
}

void YerithUtils::CREATE_AN_EMPTY_FILE(const QString &aFileFullPath)
{
	QImage image(":YERITH.JPG");

	QPixmap pixMap(QPixmap::fromImage(image));

	QFile empty_jpg_yerith_file(aFileFullPath);

	pixMap.save(&empty_jpg_yerith_file, "JPG");

	empty_jpg_yerith_file.close();
}

QString YerithUtils::get_text_sql_escaped(const QVariant &qv)
{
	QString retStr;

	switch (qv.type())
	{
	case QVariant::UInt:
		return QString::number(qv.toUInt());

	case QVariant::Int:
		return QString::number(qv.toInt());

	case QVariant::Double:
		return QString::number(qv.toDouble(), 'f', 2);

	case QVariant::ULongLong:
		return QString::number(qv.toULongLong());

	case QVariant::LongLong:
		return QString::number(qv.toLongLong());

	case QVariant::Char:
		retStr = QString(qv.toChar());
		break;

	case QVariant::String:
		retStr = qv.toString();
		break;

	case QVariant::Bool:
		retStr.append(qv.toBool());
		break;

	case QVariant::Date:
		retStr = DATE_TO_STRING(qv.toDate());
		break;

	case QVariant::Time:
		retStr = TIME_TO_STRING(qv.toTime());
		break;

	default:
		retStr = QString(qv.type());
		break;
	}

	return YerithUtils::prepareSqlStr(retStr);
}

QString YerithUtils::get_text(const QVariant &qv)
{
	QString retStr;

	switch (qv.type())
	{
	case QVariant::UInt:
		return QString::number(qv.toUInt());

	case QVariant::Int:
		return QString::number(qv.toInt());

	case QVariant::Double:
		return QString::number(qv.toDouble(), 'f', 2);

	case QVariant::ULongLong:
		return QString::number(qv.toULongLong());

	case QVariant::LongLong:
		return QString::number(qv.toLongLong());

	case QVariant::Char:
		if (!qv.isNull())
		{
			return QString(qv.toChar());
		}
		break;

	case QVariant::String:
		if (!qv.isNull())
		{
			return QString(qv.toString());
		}
		break;

	case QVariant::Bool:
		retStr.append(qv.toBool());
		break;

	case QVariant::Date:
		return DATE_TO_STRING(qv.toDate());

	case QVariant::Time:
		return TIME_TO_STRING(qv.toTime());

	default:
		break;
	}

	return retStr;
}

void YerithUtils::getCenterPosition(unsigned desktopWidth,
		unsigned desktopHeight, const QWidget &aWidget, QPoint &result)
{
	int x = desktopWidth / 2 - aWidget.width() / 2;

	int y = desktopHeight / 2 - aWidget.height() / 2 - 25;

	result.setX(x);

	result.setY(y);
}

void YerithUtils::createTableModelHeaders_DEPRECATED(YerithSqlTableModel 	&tableModel,
												     QStandardItemModel 	&stdItemModel,
													 QStringList 			&tableModelHeaders,
													 QStringList 			&tableModelRawHeaders_IN_OUT)
{
	tableModelRawHeaders_IN_OUT.clear();

	tableModelHeaders.clear();

	QString viewStrHdr;

	QString strHdr;

	//Nous mettons les noms des colones
	for (unsigned k = 0; k < tableModel.columnCount(); ++k)
	{
		strHdr = tableModel.record(0).fieldName(k);

		if (!YerithERPConfig::SHOW_DATABASE_RAW)
		{
			viewStrHdr = YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(strHdr);
		}
		else
		{
			viewStrHdr = strHdr;
		}

		tableModelRawHeaders_IN_OUT.append(strHdr);

		tableModelHeaders.append(viewStrHdr);

		stdItemModel.setHeaderData(k, Qt::Horizontal, viewStrHdr);
	}
}


void YerithUtils::createTableModelHeaders_QStringList(YerithSqlTableModel 	&tableModel,
                                                      QStandardItemModel 	&stdItemModel,
                                                      QStringList 			&tableModelHeaders,
                                                      QStringList 			&tableModelRawHeaders_IN_OUT,
                                                      YerithWindowsCommons 	*A_CALLING_WINDOW,
                                                      bool 					CHARGES_FINANCIERES_WINDOW,
                                                      QList<QString>        &CHARGES_FINANCIERES_WINDOW_Db_ColumnS,
                                                      QList<QString>        &CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STRS)
{
	tableModelRawHeaders_IN_OUT.clear();

	tableModelHeaders.clear();

	QString viewStrHdr;

	QString strHdr;


	if (0 != A_CALLING_WINDOW)
	{
		if (!CHARGES_FINANCIERES_WINDOW)
		{
			A_CALLING_WINDOW->selectionner_champs_db_visibles(false,
                                                              false,
                                                              YerithUtils::EMPTY_STRING,
                                                              YerithUtils::EMPTY_STRING);
		}
		else
		{
			A_CALLING_WINDOW->selectionner_champs_db_visibles(false,
                                                              true,
                                                              CHARGES_FINANCIERES_WINDOW_Db_ColumnS,
                                                              CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STRS);
		}
	}


	//Nous mettons les noms des colones
	for (unsigned k = 0; k < tableModel.columnCount(); ++k)
	{
		strHdr = tableModel.record(0).fieldName(k);

		if (!YerithERPConfig::SHOW_DATABASE_RAW)
		{
			viewStrHdr = YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(strHdr);
		}
		else
		{
			viewStrHdr = strHdr;
		}

		tableModelRawHeaders_IN_OUT.append(strHdr);

		tableModelHeaders.append(viewStrHdr);

		stdItemModel.setHeaderData(k, Qt::Horizontal, viewStrHdr);
	}
}


void YerithUtils::createTableModelHeaders(YerithSqlTableModel 	&tableModel,
										  QStandardItemModel 	&stdItemModel,
										  QStringList 			&tableModelHeaders,
										  QStringList 			&tableModelRawHeaders_IN_OUT,
										  YerithWindowsCommons 	*A_CALLING_WINDOW,
										  bool 					CHARGES_FINANCIERES_WINDOW /* = false*/,
										  QString        CHARGES_FINANCIERES_WINDOW_Db_Column /* = YerithUtils::EMPTY_STRING*/,
										  QString        CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STR /* = YerithUtils::EMPTY_STRING */)
{
	tableModelRawHeaders_IN_OUT.clear();

	tableModelHeaders.clear();

	QString viewStrHdr;

	QString strHdr;


	if (0 != A_CALLING_WINDOW)
	{
		if (!CHARGES_FINANCIERES_WINDOW)
		{
			A_CALLING_WINDOW->selectionner_champs_db_visibles(false, false);
		}
		else
		{
			A_CALLING_WINDOW->selectionner_champs_db_visibles(false,
                                                              true,
                                                              CHARGES_FINANCIERES_WINDOW_Db_Column,
                                                              CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STR);
		}
	}


	//Nous mettons les noms des colones
	for (unsigned k = 0; k < tableModel.columnCount(); ++k)
	{
		strHdr = tableModel.record(0).fieldName(k);

		if (!YerithERPConfig::SHOW_DATABASE_RAW)
		{
			viewStrHdr = YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(strHdr);
		}
		else
		{
			viewStrHdr = strHdr;
		}

		tableModelRawHeaders_IN_OUT.append(strHdr);

		tableModelHeaders.append(viewStrHdr);

		stdItemModel.setHeaderData(k, Qt::Horizontal, viewStrHdr);
	}
}


QString YerithUtils::generateSqlLike__AS_IS(QString sqlTableColumn,
		QString searchStr)
{
	QString result;

	searchStr.replace("'", "''");

	result.append(sqlTableColumn)
          .append(QString(" LIKE '%1'")
                    .arg(searchStr));

	return result;
}

QString YerithUtils::generateSqlLike(QString sqlTableColumn, QString searchStr)
{
	QString result;

	searchStr.replace("'", "''");

	result.append(sqlTableColumn)
          .append(" LIKE '%")
          .append(searchStr)
          .append("%' ");

	return result;
}

void YerithUtils::addFiltre(YerithLineEdit *aLineEdit, QString filterName,
		QString &yerithFiltre_in_out)
{
	if (aLineEdit && !aLineEdit->text().isEmpty())
	{
		yerithFiltre_in_out.append(
				QString("%1: %2").arg(filterName, aLineEdit->text())).append(
				", ");
	}
}

int YerithUtils::getComboBoxDatabaseQueryValue(
		const QString &comboBoxStringValue, QMap<int, QString> &toViewStringMAP)
{
	int databaseQueryValue = -1;

	QMapIterator<int, QString> it(toViewStringMAP);

	while (it.hasNext())
	{
		it.next();

		databaseQueryValue = it.key();

		if (YerithUtils::isEqualCaseInsensitive(comboBoxStringValue,
				it.value()))
		{
			return databaseQueryValue;
		}
	}

	return databaseQueryValue;
}

void YerithUtils::GET_YERITH_VIEW_RECORD_WIDTH_LAST_SELECTED_ID(
		YerithSqlTableModel &aYerithSqlTableModel_in_out,
		QSqlRecord &resultSQL_RECORD_in_out)
{
	aYerithSqlTableModel_in_out.yerithSetFilter_WITH_where_clause(
			QString("%1 = '%2'").arg(YerithDatabaseTableColumn::ID,
					YerithERPWindows::get_last_lister_selected_row_db_ID()));

	resultSQL_RECORD_in_out = aYerithSqlTableModel_in_out.record(0);
}

double YerithUtils::GET_QUANTITE_TOTALE_EN_STOCK(
		const QString &nom_departement_produit, const QString &nom_categorie,
		const QString &designation_produit)
{
	double quantite_totale_en_stock = 0.0;

	QString QUERY_QUANTITE_TOTALE_STOCK = QString(
			"select * from %1 where %2='%3' AND %4='%5' AND %6='%7'").arg(
			YerithDatabase::STOCKS,
			YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			nom_departement_produit, YerithDatabaseTableColumn::CATEGORIE,
			nom_categorie, YerithDatabaseTableColumn::DESIGNATION,
			designation_produit);

	QSqlQuery a_qsql_query;

	int query_size = YerithUtils::execQuery(a_qsql_query,
			QUERY_QUANTITE_TOTALE_STOCK);

	for (uint i = 0; i < query_size && a_qsql_query.next(); ++i)
	{
		quantite_totale_en_stock += a_qsql_query.value(
				YerithDatabaseTableColumn::QUANTITE_TOTALE).toDouble();
	}

	return quantite_totale_en_stock;
}

int YerithUtils::STOCK_PURCHASE_RECORDS_QUANTITY(int stockId)
{
	QString strAchatsQuery(
			QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(
					YerithDatabaseTableColumn::ID, YerithDatabase::ACHATS,
					YerithDatabaseTableColumn::STOCKS_ID,
					QString::number(stockId)));

	QSqlQuery query;

	int achatQuerySize = YerithUtils::execQuery(query, strAchatsQuery);

	return achatQuerySize;
}

void YerithUtils::getColumnListString(QStringList &columnStringList,
		const QString &tableName, const QString &fieldName,
		QString conditionStr /* = YerithUtils::EMPTY_STRING */)
{
	columnStringList.clear();

	QString strQuery(
			QString("SELECT %1 FROM %2 ORDER BY `%3` ASC").arg(fieldName,
					tableName, fieldName));

	if (!conditionStr.isEmpty())
	{
		strQuery = QString("SELECT %1 FROM %2 WHERE %3 ORDER BY `%4` ASC").arg(
				fieldName, tableName, conditionStr, fieldName);
	}

	//qDebug() << "++ query: " << strQuery;
	QSqlQuery query;

	query.prepare(strQuery);

	bool success = query.exec();
	//qDebug() << "[" << success << "]" << query.executedQuery();

	if (success)
	{
		QSqlRecord rec = query.record();

		while (query.next())
		{
			QString currentItem(query.value(0).toString());

			if (!currentItem.isEmpty()
					&& !columnStringList.contains(currentItem))
			{
				columnStringList.append(currentItem);
			}
		}
	}
}


QString YerithUtils::GET_YERITH_QT_OBJECT_NAME(QObject *an_object)
{
	QString YR_OBJECT_NAME;

	if (0 != an_object)
	{
		YerithAdminListerWindow *admin_lister_window
			= dynamic_cast<YerithAdminListerWindow *>(an_object);

		if (0 != admin_lister_window)
		{
			QTabWidget *YR_tabWidget_lister =
					admin_lister_window->get_tabwidget_lister();


			assert(0 != YR_tabWidget_lister);


			QString a_current_tab_index_STRING = QString::number(YR_tabWidget_lister->currentIndex());


			YR_OBJECT_NAME = QString("%1_%2")
							  .arg(admin_lister_window->objectName().trimmed(),
								   a_current_tab_index_STRING);


			YR_OBJECT_NAME = YR_OBJECT_NAME.toUpper();
		}
		else
		{
			YR_OBJECT_NAME = an_object->objectName().toUpper();
		}
	}

	return YR_OBJECT_NAME;
}


bool YerithUtils::checkIf_KEYWORD_ALREADY_EXISTS(YerithWindowsCommons &aCallingWindow,
												 YerithSqlTableModel  &aSqlTableModel,
												 const QString 	  	  &a_DB_TO_SEARCH_COLUMN_NAME,
												 const QString 		  &for_user_view_db_column_description,
												 const QString 		  &aDB_TABLE_COLUMN_NAME,
												 int 				  aCurrentClientDetailDBID /* = YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */)
{
	if (a_DB_TO_SEARCH_COLUMN_NAME.isEmpty())
	{
		return false;
	}

	// ** check if customer account with same name exist
	QString nom_entreprise_filter(
			QString("LOWER(%1) = LOWER('%2')")
                .arg(aDB_TABLE_COLUMN_NAME,
					 a_DB_TO_SEARCH_COLUMN_NAME));

	//              qDebug() << QString("++ nom_entreprise_filter: %1")
	//                                              .arg(nom_entreprise_filter);

	aSqlTableModel.yerithSetFilter_WITH_where_clause(nom_entreprise_filter);

	int clientsTableModelRowCount = aSqlTableModel.rowCount();

	if (clientsTableModelRowCount > 0)
	{
		if (YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED != aCurrentClientDetailDBID)
		{
			for (int k = 0; k < clientsTableModelRowCount; ++k)
			{
				QSqlRecord clientRecord = aSqlTableModel.record(k);

				int clientRecordDBID(GET_SQL_RECORD_DATA(clientRecord,
                                                         YerithDatabaseTableColumn::ID).toInt());

				if (aCurrentClientDetailDBID != clientRecordDBID)
				{
					aSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 3042);

					QString retMsg(
							QObject::tr("Une %1 nommé(e) '%2' existe déjà "
									"dans la base de données !").arg(
									for_user_view_db_column_description,
									a_DB_TO_SEARCH_COLUMN_NAME));

					YerithQMessageBox::warning(&aCallingWindow,
							QObject::tr("%1 déjà existant(e)")
                                        .arg(for_user_view_db_column_description),
							retMsg);

					return true;
				}
			}
		}
		else
		{
			QString retMsg(
					QObject::tr("Une %1 nommé(e) '%2' existe déjà "
                                "dans la base de données !")
                            .arg(for_user_view_db_column_description,
                                 a_DB_TO_SEARCH_COLUMN_NAME));

			YerithQMessageBox::warning(&aCallingWindow,
					QObject::tr("%1 déjà existant(e)")
                            .arg(for_user_view_db_column_description),
                                 retMsg);

			return true;
		}
	}

	aSqlTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 3076);

	return false;
}

bool YerithUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(
		YerithWindowsCommons &aCallingWindow,
		YerithSqlTableModel &aClientTableModel,
		YerithLineEdit &aYerithLineEdit_nom_entreprise,
		int aCurrentClientDetailDBID /* = YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */)
{
	// ** check if customer account with same name exist
	QString nom_entreprise_filter(
			QString("LOWER(%1) = LOWER('%2')").arg(
					YerithDatabaseTableColumn::NOM_ENTREPRISE,
					aYerithLineEdit_nom_entreprise.text()));

	//              qDebug() << QString("++ nom_entreprise_filter: %1")
	//                                              .arg(nom_entreprise_filter);

	aClientTableModel.yerithSetFilter_WITH_where_clause(nom_entreprise_filter);

	int clientsTableModelRowCount = aClientTableModel.rowCount();

	if (clientsTableModelRowCount > 0)
	{
		if (YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED
				!= aCurrentClientDetailDBID)
		{
			for (int k = 0; k < clientsTableModelRowCount; ++k)
			{
				QSqlRecord clientRecord = aClientTableModel.record(k);

				int clientRecordDBID(GET_SQL_RECORD_DATA(clientRecord,
						YerithDatabaseTableColumn::ID).toInt());

				if (aCurrentClientDetailDBID != clientRecordDBID)
				{
					aClientTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 2944);

					QString retMsg(
							QObject::tr(
									"Une entreprise nommée '%1' existe déjà "
											"dans la base de données !").arg(
									aYerithLineEdit_nom_entreprise.text()));

					YerithQMessageBox::warning(&aCallingWindow,
							QObject::tr("compte client déjà existant"),
							retMsg);

					return true;
				}
			}
		}
		else
		{
			QString retMsg(
					QObject::tr("Une entreprise nommée '%1' existe déjà "
							"dans la base de données !").arg(
							aYerithLineEdit_nom_entreprise.text()));

			YerithQMessageBox::warning(&aCallingWindow,
					QObject::tr("compte client déjà existant"), retMsg);

			return true;
		}
	}

	aClientTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 2974);

	return false;
}

bool YerithUtils::checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(
		YerithWindowsCommons &aCallingWindow,
		YerithSqlTableModel &aClientTableModel,
		YerithLineEdit &aYerithLineEdit_reference_registre_du_commerce,
		int aCurrentClientDetailDBID /* = YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */)
{
	// ** check if customer account with same trade registry number exist
	if (!aYerithLineEdit_reference_registre_du_commerce.isEmpty())
	{
		QString reference_du_registre_du_commerce_filter(
				QString("LOWER(%1) = LOWER('%2')").arg(
						YerithDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
						aYerithLineEdit_reference_registre_du_commerce.text()));
		aClientTableModel.yerithSetFilter_WITH_where_clause(
				reference_du_registre_du_commerce_filter);
		int clientsTableModelRowCount = aClientTableModel.rowCount();
		if (clientsTableModelRowCount > 0)
		{
			if (YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED
					!= aCurrentClientDetailDBID)
			{
				for (int k = 0; k < clientsTableModelRowCount; ++k)
				{
					QSqlRecord clientRecord = aClientTableModel.record(k);
					int clientRecordDBID(GET_SQL_RECORD_DATA
					(clientRecord,
							YerithDatabaseTableColumn::
							ID).toInt());
					if (aCurrentClientDetailDBID != clientRecordDBID)
					{
						aClientTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 3009);
						QString retMsg(
								QObject::tr(
										"Une entreprise avec la référence "
												"régistre du commerce '%1' existe déjà "
												"dans la base de données !").arg(
										aYerithLineEdit_reference_registre_du_commerce.text()));
						YerithQMessageBox::warning(&aCallingWindow,
								QObject::tr("compte client déjà existant"),
								retMsg);
						return true;
					}
				}
			}
			else
			{
				QString retMsg(
						QObject::tr("Une entreprise avec la référence "
								"régistre du commerce '%1' existe déjà "
								"dans la base de données !").arg(
								aYerithLineEdit_reference_registre_du_commerce.text()));
				YerithQMessageBox::warning(&aCallingWindow,
						QObject::tr("compte client déjà existant"), retMsg);
				return true;
			}
		}
		aClientTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 3035);
	}

	return false;
}

bool YerithUtils::checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(
		YerithWindowsCommons &aCallingWindow,
		YerithSqlTableModel &aClientTableModel,
		YerithLineEdit &aYerithLineEdit_reference_client,
		int aCurrentClientDetailDBID
		/* = YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */
		)
{
	// ** check if customer account with same reference exist
	if (!aYerithLineEdit_reference_client.isEmpty())
	{
		QString reference_client_filter(
				QString("LOWER(%1) = LOWER('%2')").arg(
						YerithDatabaseTableColumn::REFERENCE_CLIENT,
						aYerithLineEdit_reference_client.text()));
		aClientTableModel.yerithSetFilter_WITH_where_clause(
				reference_client_filter);
		int clientsTableModelRowCount = aClientTableModel.rowCount();
		if (clientsTableModelRowCount > 0)
		{
			if (YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED
					!= aCurrentClientDetailDBID)
			{
				for (int k = 0; k < clientsTableModelRowCount; ++k)
				{
					QSqlRecord clientRecord = aClientTableModel.record(k);
					int clientRecordDBID(GET_SQL_RECORD_DATA
					(clientRecord,
							YerithDatabaseTableColumn::
							ID).toInt());
					if (aCurrentClientDetailDBID != clientRecordDBID)
					{
						aClientTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 3073);
						QString retMsg(
								QObject::tr(
										"Une entreprise avec la référence '%1' existe déjà "
												"dans la base de données !").arg(
										aYerithLineEdit_reference_client.text()));
						YerithQMessageBox::warning(&aCallingWindow,
								QObject::tr("compte client déjà existant"),
								retMsg);
						return true;
					}
				}
			}
			else
			{
				QString retMsg(
						QObject::tr(
								"Une entreprise avec la référence '%1' existe déjà "
										"dans la base de données !").arg(
								aYerithLineEdit_reference_client.text()));
				YerithQMessageBox::warning(&aCallingWindow,
						QObject::tr("compte client déjà existant"), retMsg);
				return true;
			}
		}
		aClientTableModel.resetFilter("src/utils/yerith-erp-utils.cpp", 3098);
	}

	return false;
}

bool YerithUtils::IS_SQL_TABLE_COLUMN_MAXIMUM_SIZE_REACHED(
		int a_sql_table_column_CURRENT_history_row_data_size,
		int a_sql_table_column_NEXT_WRITE_row_data_size,
		int a_sql_table_column_MAXIMUM_SIZE)
{
	//QDEBUG_STRING_OUTPUT_2_N("a_sql_table_column_MAXIMUM_SIZE", a_sql_table_column_MAXIMUM_SIZE);

	//QDEBUG_STRING_OUTPUT_2_N("a_sql_table_column_CURRENT_history_row_data_size",
	//a_sql_table_column_CURRENT_history_row_data_size);

	if (a_sql_table_column_MAXIMUM_SIZE
			<= a_sql_table_column_CURRENT_history_row_data_size)
	{
		return true;
	}

	//QDEBUG_STRING_OUTPUT_2_N("a_sql_table_column_NEXT_WRITE_row_data_size",
	//a_sql_table_column_NEXT_WRITE_row_data_size);

	if (a_sql_table_column_NEXT_WRITE_row_data_size
			>= a_sql_table_column_MAXIMUM_SIZE)
	{
		return true;
	}

	return false;
}

QString YerithUtils::getFileNameWithUserIDAndCurrentTime(
		const QString &fileName)
{
	QString resultFileName(fileName);
	YerithPOSUser *aUser = 0;
	if (0 != YerithUtils::_allWindows)
	{
		aUser = YerithUtils::_allWindows->getUser();
		if (0 != aUser)
		{
			resultFileName.append(QString("-%1").arg(aUser->nom_utilisateur()));
		}
	}

	resultFileName = YerithUtils::getFileNameWithCurrentTime(resultFileName);
	return resultFileName;
}

QString YerithUtils::getWindowTitleWithStrategy(QMainWindow *aMainWindow,
		QString localVisibleStrategy
		/* = "" */
		)
{
	QString aNewTitle(aMainWindow->windowTitle());
	QString currentStockListingStrategy(localVisibleStrategy);
	if (localVisibleStrategy.isEmpty())
	{
		currentStockListingStrategy = YerithERPConfig::salesStrategy;
	}

	if (aNewTitle.contains(YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
	{
		aNewTitle.replace(YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL,
				currentStockListingStrategy);
	}
	else if (aNewTitle.contains(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO))
	{
		aNewTitle.replace(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
				currentStockListingStrategy);
	}
	else if (aNewTitle.contains(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
	{
		aNewTitle.replace(YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
				currentStockListingStrategy);
	}
	else if (aNewTitle.contains(YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
	{
		aNewTitle.replace(YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
				currentStockListingStrategy);
	}
	else
	{
		if (!aNewTitle.contains(YerithERPConfig::salesStrategy))
		{
			aNewTitle.append(" - ").append(currentStockListingStrategy);
		}
	}

	return aNewTitle;
}

void YerithUtils::fillDBTableColumnNameToDBTableColumnType_TEST(
		const QString &aDBTableName,
		QMap<QString, QString> &aDBbTableColumnToType_IN_OUT,
		QMap<QString, bool> &aDBTableColumnToIsNotNULL_IN_OUT)
{
	aDBbTableColumnToType_IN_OUT.clear();
	aDBTableColumnToIsNotNULL_IN_OUT.clear();
	QString strShowColumnQuery(
			QString("SHOW COLUMNS FROM %1").arg(aDBTableName));
	QSqlQuery query;
	bool dbFieldNullAble = false;
	QString dbFieldName;
	QString dbFieldType;
	int querySize = YerithUtils::execQuery(query, strShowColumnQuery);
	for (int j = 0; j < querySize && query.next(); ++j)
	{
		dbFieldName = query.value(0).toString();
		dbFieldType = query.value(1).toString();
		dbFieldNullAble = (query.value(2).toString() == "NO") ? false : true;
		if (false == dbFieldNullAble)
		{
			aDBTableColumnToIsNotNULL_IN_OUT.insert(dbFieldName,
					dbFieldNullAble);
		}

		aDBbTableColumnToType_IN_OUT.insert(dbFieldName, dbFieldType);
	}
}

QString YerithUtils::appendPDFReceiptFormat(QString aStr)
{
	QString resultStr(aStr);
	if (resultStr.contains(YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER))
	{
		resultStr.replace(YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
				YerithERPConfig::receiptFormat);
	}
	else if (resultStr.contains(YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER))
	{
		resultStr.replace(YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER,
				YerithERPConfig::receiptFormat);
	}
	else if (resultStr.contains(YerithERPConfig::RECEIPT_FORMAT_PETIT))
	{
		resultStr.replace(YerithERPConfig::RECEIPT_FORMAT_PETIT,
				YerithERPConfig::receiptFormat);
	}
	else
	{
		if (!resultStr.contains(YerithERPConfig::receiptFormat))
		{
			resultStr.append(
					QObject::tr("\t[ factures pdf au format: '%1' ]").arg(
							YerithERPConfig::receiptFormat));
		}
	}

	return resultStr;
}


QString YerithUtils::
            Get_INTITULE_DE_LA_LIGNE_BUDGETAIRE_FOR_GROUPE_DE_PAIE_Hr
                (const QString &groupe_de_paie_hr)
{
    QString result_INTITULE_DE_LA_LIGNE_BUDGETAIRE;


    YerithSqlTableModel &GROUPES_DE_PAIE_HR =
        _allWindows->getSqlTableModel_groupes_de_paie_hr();


    QString QUERY_intitule_de_la_ligne_budgetaire_GROUPES_DE_PAIE_HR =
                YerithUtils::generateSqlIs(YerithDatabaseTableColumn::DESIGNATION,
                                           groupe_de_paie_hr);


    GROUPES_DE_PAIE_HR
        .yerithSetFilter_WITH_where_clause
            (QUERY_intitule_de_la_ligne_budgetaire_GROUPES_DE_PAIE_HR);


    int rowCount =
            GROUPES_DE_PAIE_HR.easySelect("src/utils/yerith-erp-utils.cpp", 3441);


    if (1 == rowCount)
    {
        QSqlRecord a_qsql_record = GROUPES_DE_PAIE_HR.record(0);

        result_INTITULE_DE_LA_LIGNE_BUDGETAIRE =
            GET_SQL_RECORD_DATA(a_qsql_record,
                                YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire);

    }

    return result_INTITULE_DE_LA_LIGNE_BUDGETAIRE;
}


QString YerithUtils::
            Get_INTITULE_COMPTE_BANCAIRE_FOR_intitule_de_la_ligne_budgetaire
                (const QString &intitule_de_la_ligne_budgetaire)
{
    QString result_INTITULE_DU_COMPTE_BANCAIRE;


    YerithSqlTableModel &LIGNES_BUDGETAIRES =
        _allWindows->getSqlTableModel_lignes_budgetaires();


    QString QUERY_intitule_du_compte_bancaire_LIGNE_BUDGETAIRE =
                YerithUtils::generateSqlIs(YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
                                           intitule_de_la_ligne_budgetaire);


    LIGNES_BUDGETAIRES
        .yerithSetFilter_WITH_where_clause
            (QUERY_intitule_du_compte_bancaire_LIGNE_BUDGETAIRE);


    int rowCount =
            LIGNES_BUDGETAIRES.easySelect("src/utils/yerith-erp-utils.cpp", 3482);


    if (1 == rowCount)
    {
        QSqlRecord a_qsql_record = LIGNES_BUDGETAIRES.record(0);

        result_INTITULE_DU_COMPTE_BANCAIRE =
            GET_SQL_RECORD_DATA(a_qsql_record,
                                YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);

    }


    return result_INTITULE_DU_COMPTE_BANCAIRE;
}


QString YerithUtils::GET_LOCALISATION_FOR_EMPLOYEE_PAY_GROUP
                        (const QString &an_employee_PAY_GROUP)
{
	QString CUR_GET_employee_pay_group_LOCALISATION_ONLY =
        QString("select %1 from %2 where %3='%4'")
            .arg(YerithDatabaseTableColumn::LOCALISATION,
                 YerithDatabase::GROUPES_DE_PAIE_hr,
                 YerithDatabaseTableColumn::DESIGNATION,
                 an_employee_PAY_GROUP);

	QSqlQuery a_qsql_query;

	int query_size =
        YerithUtils::execQuery(a_qsql_query,
                               CUR_GET_employee_pay_group_LOCALISATION_ONLY);

	if (query_size > 0   &&
        a_qsql_query.next())
	{
		return a_qsql_query.value(0).toString();
	}

	return YerithUtils::EMPTY_STRING;
}


QString YerithUtils::GET_LOCALISATION_FOR_CLIENT_LOYALTY_PROGRAM
                        (const QString &client_loyalty_program)
{
	QString CUR_GET_CLIENT_LOYALTY_PROGRAM_LOCALISATION_ONLY =
        QString("select %1 from %2 where %3='%4'")
            .arg(YerithDatabaseTableColumn::LOCALISATION,
                 YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                 YerithDatabaseTableColumn::DESIGNATION,
                 client_loyalty_program);

	QSqlQuery a_qsql_query;

	int query_size =
        YerithUtils::execQuery(a_qsql_query,
                               CUR_GET_CLIENT_LOYALTY_PROGRAM_LOCALISATION_ONLY);

	if (query_size > 0   &&
        a_qsql_query.next())
	{
		return a_qsql_query.value(0).toString();
	}

	return YerithUtils::EMPTY_STRING;
}


QString YerithUtils::getStrategySqlQueryStr(QString aSelectStmt,
		QString aStrategy)
{
	QString resultStrategyQueryStr;
	if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO, aStrategy))
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC, %3 ASC").arg(
					aSelectStmt, YerithDatabaseTableColumn::DESIGNATION,
					YerithDatabaseTableColumn::DATE_PEREMPTION);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC, %3 ASC").arg(
					aSelectStmt, YerithDatabaseTableColumn::DESIGNATION,
					YerithDatabaseTableColumn::DATE_PEREMPTION);
		}
	}
	else if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, aStrategy))
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC, %3 ASC").arg(
					aSelectStmt, YerithDatabaseTableColumn::DESIGNATION,
					YerithDatabaseTableColumn::DATE_ENTREE);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC, %3 ASC").arg(
					aSelectStmt, YerithDatabaseTableColumn::DESIGNATION,
					YerithDatabaseTableColumn::DATE_ENTREE);
		}
	}
	else if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, aStrategy))
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC, %3 DESC").arg(
					aSelectStmt, YerithDatabaseTableColumn::DESIGNATION,
					YerithDatabaseTableColumn::DATE_ENTREE);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC, %3 DESC").arg(
					aSelectStmt, YerithDatabaseTableColumn::DESIGNATION,
					YerithDatabaseTableColumn::DATE_ENTREE);
		}
	}
	else	//YerithConfig::STRATEGIE_VENTE_SORTIE_ALL
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC").arg(
					aSelectStmt, YerithDatabaseTableColumn::ID);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC").arg(aSelectStmt,
					YerithDatabaseTableColumn::ID);
		}
	}

	return resultStrategyQueryStr;
}


void YerithUtils::infosEntreprise(YerithPOSAdminWindowsCommons 	&aYerithPOSAdminQMainWindow,
								  const QString 				&infos)
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        YerithQMessageBox::information(&aYerithPOSAdminQMainWindow,
        							   QString("Information about the company"),
									   infos);
    }
    else
    {
        YerithQMessageBox::information(&aYerithPOSAdminQMainWindow,
        							   QString("Informations sur l'entreprise"),
									   infos);
    }
}


//!!! #define DOUBLE_FROM_LOCALIZED_STRING(NUM) YerithUtils::frenchLocale.toDouble(NUM) !!!

double YerithUtils::localized_string__get_double_FROM_LOCALIZED_STRING(QString DOUBLE_NUMBER_STRING)
{
	double result;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		result = YerithUtils::englishLocale.toDouble(DOUBLE_NUMBER_STRING);
	}
	else
	{
		result = YerithUtils::frenchLocale.toDouble(DOUBLE_NUMBER_STRING);
	}

	return result;
}


QString YerithUtils::localized_string__get_number_string(int INT_NUMBER)
{
	QString result;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		result = YerithUtils::englishLocale.toString(INT_NUMBER);
	}
	else
	{
		result = YerithUtils::frenchLocale.toString(INT_NUMBER);
	}

	return result;
}


QString YerithUtils::localized_string__get_double_string(double DOUBLE_NUMBER)
{
	QString result;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		result = YerithUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', 2);
	}
	else
	{
		result = YerithUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', 2);
	}

	return result;
}


QString YerithUtils::localized_string__get_double_string_P(double DOUBLE_NUMBER,
													 	   int NUMBER_OF_DECIMALS_after)
{
	QString result;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		result = YerithUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after);
	}
	else
	{
		result = YerithUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after);
	}

	return result;
}


QString YerithUtils::localized_string__get_CURRENCY_double_string(double DOUBLE_NUMBER)
{
	QString result;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		result = YerithUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', 2)
										   .append(" ")
										   .append(YerithERPConfig::currency);
	}
	else
	{
		result = YerithUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', 2)
										  .append(" ")
										  .append(YerithERPConfig::currency);
	}

	return result;
}


QString YerithUtils::localized_string__get_CURRENCY_double_string_P(double DOUBLE_NUMBER,
															  	    int NUMBER_OF_DECIMALS_after)
{
	QString result;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		result = YerithUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after)
										   .append(" ")
										   .append(YerithERPConfig::currency);
	}
	else
	{
		result = YerithUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after)
										  .append(" ")
										  .append(YerithERPConfig::currency);
	}

	return result;
}


QString YerithUtils::localized_string__get_MONTH_NAME_LOCALIZED(int month_name)
{
	QString result;

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		result = YerithUtils::englishLocale.monthName(month_name);
	}
	else
	{
		result = YerithUtils::frenchLocale.monthName(month_name);
	}

	return result;
}


void YerithUtils::getCurrentSimplifiedDate(QString 		&date_IN_OUT,
										   const QDate 	&aDate)
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	date_IN_OUT.append(QString(", The %1 at %2")
    						.arg(aDate.toString("dd-MM-yyyy"),
    							 CURRENT_TIME));

    	date_IN_OUT = YerithUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);
    }
    else
    {
    	date_IN_OUT.append(QString(", Le %1 à %2")
							.arg(aDate.toString("dd-MM-yyyy"),
								 CURRENT_TIME));

    	date_IN_OUT = YerithUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);
    }
}


void YerithUtils::getCurrentLocaleDate(QString 		&date_IN_OUT,
									   const QDate 	&aDate)
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        QString d(YerithUtils::englishLocale.toString(aDate));

        int firstCommaIndex = d.indexOf(",", 0);

        d.replace(firstCommaIndex, 1, " ");

        date_IN_OUT.append(", ").append(d);

        date_IN_OUT = YerithUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);
    }
    else
    {
        date_IN_OUT.append(", le ")
        		   .append(YerithUtils::frenchLocale.toString(aDate));

        //qDebug() << "++ before date: " << date_IN_OUT << " (" << date_IN_OUT.indexOf(QString::fromUtf8("û")) << ")";
        date_IN_OUT = YerithUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);

        //qDebug() << "++ after date: " << date_IN_OUT << " (" << date_IN_OUT.indexOf(QString::fromUtf8("û")) << ")";
    }
}


bool YerithUtils::slot_connecter_localisation(QWidget &aWidget,
											  YerithERPWindows *allWindows,
											  const QString &localisation)
{
	YerithSqlTableModel &localisationsSqlTableModel =
			allWindows->getSqlTableModel_localisations();

	localisationsSqlTableModel.yerithSetFilter_WITH_where_clause(
			GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::NOM_LOCALISATION,
					localisation));

	int rowCount = localisationsSqlTableModel.easySelect("src/utils/yerith-erp-utils.cpp", 3418);

	if (rowCount <= 0)
	{
		//qDebug() << "++ pas de localisation au nom de: " << localisation;
		return false;
	}

	QSqlRecord localisationsRecord = localisationsSqlTableModel.record(0);

	QString addresseIP(
			GET_SQL_RECORD_DATA(localisationsRecord,
					YerithDatabaseTableColumn::ADRESSE_IP));

	QString widgetTitle(aWidget.windowTitle());

	if (addresseIP.isEmpty())
	{
		QString retMsg;

		retMsg.append(QObject::tr("La localisation %1 n'a pas d'adresse IP enregistrée !")
							.arg(localisation));

		QString msg(QObject::tr("%1 - pas d'adresse IP !").arg(widgetTitle));

		YerithQMessageBox::information(_allWindows->_mainWindow,
									   msg,
									   retMsg);

		return false;
	}
	else
	{
		if (YerithUtils::isEqualCaseInsensitive(
				ETHERNET_LOCALHOST_LOOPBACK_IP_ADDRESS, addresseIP))
		{
			QString retMsg;

			retMsg.append(QObject::tr("La localisation '%1' est "
										  "déjà connectée par l'adresse "
										  "IP loopback ('%2') !")
								.arg(localisation,
									 YerithUtils::ETHERNET_LOCALHOST_LOOPBACK_IP_ADDRESS));

			YerithQMessageBox::information(_allWindows->_mainWindow,
										   QObject::tr("%1 - Connection par adresse IP !").arg(widgetTitle),
										   retMsg);

			return false;
		}
	}

	YerithDatabase &database = allWindows->getDatabase();

	database.close();

	database.set_db_ip_address(addresseIP);

	if (database.open())
	{
		YerithERPConfig::_distantSiteConnected = true;
		YerithERPConfig::_connectedSiteIPAddress = addresseIP;
		YerithERPConfig::_connectedSite = localisation;

		allWindows->reinitialiseSqlTableModels();

		QString retMsg(
				QObject::tr("Connecter à la localisation %1 !").arg(
						localisation));

		QString msg(
				QObject::tr("%1 - Connection à la localisation !").arg(
						widgetTitle));

		YerithQMessageBox::information(_allWindows->_mainWindow,
									   msg,
									   retMsg);

		return true;
	}
	else
	{
		QString retMsg(
				QObject::tr(
						"Impossible de se connecter à la localisation %1 !").arg(
								localisation));
		QString msg(
				QObject::tr("%1 - Pas de connection à la localisation !").arg(
						widgetTitle));

		YerithQMessageBox::information(_allWindows->_mainWindow,
									   msg,
									   retMsg);

		qDebug() << database.lastError();

		slot_deconnecter_localisation(allWindows);
	}

	return false;
}


bool YerithUtils::slot_deconnecter_localisation(YerithERPWindows *allWindows)
{
	if (!YerithERPConfig::_distantSiteConnected)
	{
		return true;
	}

	YerithDatabase &database = allWindows->getDatabase();
	database.close();
	YerithERPConfig::_distantSiteConnected = false;
	YerithERPConfig::_connectedSite.clear();
	database.set_db_ip_address(YerithERPConfig::_db_ip_address);
	if (!database.open())
	{
		QString errMsg(
				QObject::tr("Impossible de se connecter au serveur '%1' "
						"de base de données !\n"
						"Contacter l'administrateur de yerith\n\n"
						"Cliquer sur 'Cancel' pour terminer %2").arg(
						database.db_type(), YerithUtils::APPLICATION_NAME));
		QMessageBox::critical(0, YerithUtils::APPLICATION_NAME, errMsg,
				QMessageBox::Cancel);
		qDebug() << database.lastError();
		return false;
		//logger.log("main",
		//QString("Impossible de se connecter au serveur MYSQL: %1 !").arg(db.lastError().text()));
	}
	//else
	//{
	allWindows->reinitialiseSqlTableModels();
	return true;
	//}
}

QString YerithUtils::colorLatexTextInput(const QString &colorSpec,
		const QString &latexString_in)
{
	return QString(
			QString("\\textcolor{%1}{%2}").arg(colorSpec, latexString_in));
}

QString YerithUtils::LATEX_IN_OUT_handleForeignAccents(
		const QString &texText_in)
{
	if (texText_in.isEmpty())
	{
		return YerithUtils::EMPTY_STRING;
	}

	QString tempText(texText_in);
	tempText =
			tempText.isEmpty() ?
					"\"\"" : tempText.replace(EMPTY_SPACE_REGEXP, "\\ ");
	tempText.replace("º", "\\textdegree");
	tempText.replace("#", "\\#");
	tempText.replace("_", "\\_");
	tempText.replace("%", "\\%");
	tempText.replace("&", "\\&");
	tempText.replace("$", "\\$");
	tempText.replace(QString::fromUtf8("ç"), "\\c{c}");
	tempText.replace(QString::fromUtf8("è"), "\\`e");
	tempText.replace(QString::fromUtf8("é"), "\\'e");
	tempText.replace(QString::fromUtf8("ê"), "\\^e");
	tempText.replace(QString::fromUtf8("à"), "\\`a");
	tempText.replace(QString::fromUtf8("â"), "\\^a");
	tempText.replace(QString::fromUtf8("ô"), "\\^o");
	tempText.replace(QString::fromUtf8("î"), "\\^i");
	tempText.replace(QString::fromUtf8("û"), "\\^u");
	tempText.replace(QString::fromUtf8("È"), "\\`E");
	tempText.replace(QString::fromUtf8("É"), "\\'E");
	tempText.replace(QString::fromUtf8("Ê"), "\\^E");
	tempText.replace(QString::fromUtf8("À"), "\\`A");
	tempText.replace(QString::fromUtf8("Â"), "\\^A");
	tempText.replace(QString::fromUtf8("Ô"), "\\^O");
	tempText.replace(QString::fromUtf8("Î"), "\\^I");
	tempText.replace(QString::fromUtf8("Û"), "\\^U");
	return tempText;
}

void YerithUtils::cleanUpTexTableLastString(QString &texTable_in_out)
{
	int lastPos = texTable_in_out.size() - 1;
	if (YerithUtils::isEqualCaseInsensitive("&", texTable_in_out.at(lastPos)))
	{
		texTable_in_out.remove(lastPos, 1);
	}
}

void YerithUtils::handleFactureTexTableItemText(int texTableColumnCount,
		QString &texTable_IN_OUT, int itemTextColumnPosition,
		const QString &itemText)
{
	QString resultItemText(itemText);
	if (!itemText.isEmpty())
	{
		resultItemText = LATEX_IN_OUT_handleForeignAccents(resultItemText);
		texTable_IN_OUT.append(resultItemText);
	}

	if (itemTextColumnPosition < texTableColumnCount - 1)
	{
		texTable_IN_OUT.append(" &");
	}
	else
	{
		texTable_IN_OUT.append(" \\\\").append("\n");
	}
}


void YerithUtils::handleTexTableItemText(int 			texTableColumnCount,
										 QString 		&texTable_IN_OUT,
										 int 			itemTextColumnPosition,
										 const QString  &itemText)
{
	QString resultItemText(LATEX_IN_OUT_handleForeignAccents(itemText));

	if (!resultItemText.isEmpty())
	{
		texTable_IN_OUT.append(resultItemText);
	}

	if (itemTextColumnPosition < texTableColumnCount - 1)
	{
		texTable_IN_OUT.append(" &");
	}
	else
	{
		texTable_IN_OUT.append(" \\\\").append("\n");
	}
}


QString YerithUtils::IMPRIMER_FACTURE_FOR_CLIENT_ACCOUNT_SALE_INVOICE_FACTURE(
		YerithWindowsCommons &current_printing_window,
		YERITH_CLIENT_ACCOUNT_SALE_info &yerith_client_account_sale_info)
{
	YerithPOSUser *yerithUser = _allWindows->getUser();
	YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();
	QString factureTexDocument;
	QString factureDate(infoEntreprise.getVille_LATEX());
	YerithUtils::getCurrentLocaleDate(factureDate);

    YerithUtils::get_FACTURE_INVOICE_TexDocumentString(factureTexDocument);

	if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
			YerithERPConfig::receiptFormat))
	{
		factureTexDocument.replace("YERITHPAPERSPEC", "a4paper");
	}
	else if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER,
			YerithERPConfig::receiptFormat))
	{
		factureTexDocument.replace("YERITHPAPERSPEC", "a5paper");
	}

	factureTexDocument.replace("YERITHTYPEDEVENTE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getSaleType()));
	factureTexDocument.replace("YERITHLOCALISATIONEMISSIONINVOICE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getSiteOfInvoiceEmission()));
	factureTexDocument.replace("YERITHFACTUREINVOICEPRODUCTREFERENCE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getProductReference()));
	factureTexDocument.replace("YERITHFACTUREINVOICEPRODUCTNAME",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getProductName()));
	factureTexDocument.replace("YERITHFACTUREINVOICECATEGORY",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getCategory()));
	factureTexDocument.replace("YERITHFACTUREINVOICESALEQUANTITY",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getQuantity()));
	factureTexDocument.replace("YERITHFACTUREINVOICEUNITPRICE",
			GET_DOUBLE_STRING(
					yerith_client_account_sale_info.getUnitPrice().toDouble()));
	factureTexDocument.replace("YERITHFACTUREINVOICETAX",
			GET_DOUBLE_STRING(
					yerith_client_account_sale_info.getTaxes().toDouble()));
	QString referenceFACTURECompteClient(
			YerithUtils::GET_REFERENCE_RESTE_FACTURE_CLIENT(
					yerith_client_account_sale_info.get_db_ID()));
	factureTexDocument.replace("YERITHNUMEROFACTUREINVOICE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					referenceFACTURECompteClient));
	/*
	 * LA RÉFÉRENCE DU DERNIER PAIEMENT.
	 * ELLE EST UTILISÉ DANS LE CHAMPS TEXTUEL: 'notre référence'
	 * DE LA FACTURE PRÉSENTÉE AU CLIENT.
	 *
	 * 'notre référence' PERMET AU VENDEUR DE NE PAS GÉNÉRER PLUS
	 * D'1 FACTURE AU RISQUE DE PERTURBER LE CLIENT.
	 */
	YERITH_ERP_3_0_START_DATABASE_TRANSACTION;
	QString query_last_payment_FOR_THIS_INVOICE_GENERATION(
			QString(
					"SELECT %1 from %2 WHERE %3='%4' ORDER BY CONCAT(%5,' ',%6) desc limit 1").arg(
					YerithDatabaseTableColumn::ID, YerithDatabase::PAIEMENTS,
					YerithDatabaseTableColumn::REFERENCE,
					yerith_client_account_sale_info.getProductReference(),
					YerithDatabaseTableColumn::DATE_PAIEMENT,
					YerithDatabaseTableColumn::HEURE_PAIEMENT));
//      QDEBUG_STRING_OUTPUT_2("query_last_payment_FOR_THIS_INVOICE_GENERATION",
//                                                 query_last_payment_FOR_THIS_INVOICE_GENERATION);
	QSqlQuery a_qsql_query;
	int a_query_size_FOR_INVOICE = YerithUtils::execQuery(a_qsql_query,
			query_last_payment_FOR_THIS_INVOICE_GENERATION);
	bool found_last_payment_CORRESPONDING_TO_THIS_INVOICE_GENERATION =
			(a_query_size_FOR_INVOICE > 0 && a_qsql_query.next());
	if (found_last_payment_CORRESPONDING_TO_THIS_INVOICE_GENERATION)
	{
		QString db_ID_of_last_payment = a_qsql_query.value(
				YerithDatabaseTableColumn::ID).toString();
		factureTexDocument.replace(
				"YERITHVENTECOMPTECLIENTREFERENCEINVOICEFACTURE",
				YerithUtils::LATEX_IN_OUT_handleForeignAccents(
						QString("%1_%2").arg(referenceFACTURECompteClient,
								db_ID_of_last_payment)));
	}
	else
	{
		factureTexDocument.replace(
				"YERITHVENTECOMPTECLIENTREFERENCEINVOICEFACTURE",
				YerithUtils::LATEX_IN_OUT_handleForeignAccents(
						referenceFACTURECompteClient));
	}

	YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
	factureTexDocument.replace("YERITHFACTUREINVOICEREMAININGAMOUNT",
			GET_CURRENCY_STRING_NUM(
					yerith_client_account_sale_info.getTotalRemainingAmount().toDouble()));
	QString YERITHMONTANTLITERAL(
			QString("%1 %2").arg(
					GET_SPELLED_NUMBER(
							yerith_client_account_sale_info.getTotalRemainingAmount().toDouble()),
					YerithERPConfig::currency));
	factureTexDocument.replace("YERITHMONTANTLITERAL",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					YERITHMONTANTLITERAL.toUpper()));
	factureTexDocument.replace("YERITHFACTUREINVOICETOTALATC",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getTaxes()));
	factureTexDocument.replace("YERITHFACTUREINVOICETOTALWITHCURRENCYATC",
			QString("%1").arg(
					GET_DOUBLE_STRING(
							yerith_client_account_sale_info.getTotalAmountAtc().toDouble())));
	factureTexDocument.replace("YERITHENTREPRISE",
			infoEntreprise.getNomCommercial_LATEX());
	factureTexDocument.replace("YERITHACTIVITESENTREPRISE",
			infoEntreprise.getSecteursActivitesTex());
	factureTexDocument.replace("YERITHBOITEPOSTALE",
			infoEntreprise.getBoitePostal());
	factureTexDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
	factureTexDocument.replace("YERITHLOCALISATION_SORTIE",
			infoEntreprise.getLocalisation_LATEX());
	factureTexDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
	factureTexDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
	factureTexDocument.replace("YERITHTELEPHONE",
			infoEntreprise.getTelephone());
	factureTexDocument.replace("YERITHDATE", factureDate);
	factureTexDocument.replace("YERITHHEUREVENTE",
	CURRENT_TIME);
	factureTexDocument.replace("YERITHVENDEUR", yerithUser->nom_completTex());
	factureTexDocument.replace("YERITHCOMPTEBANCAIRENR",
			infoEntreprise.getNumeroCompteBancaire());
	factureTexDocument.replace("YERITHCONTRIBUABLENR",
			infoEntreprise.getNumeroDeContribuable());
	factureTexDocument.replace("YERITHAGENCECOMPTEBANCAIRE",
			infoEntreprise.getAgenceCompteBancaireTex());
//    QString YERITHMONTANTLITERAL(QString("'%1' %2")
//                                                              .arg(YerithUtils_SPELL_NUMBERS::YERITH__spell_numbers(_sommeTotal),
//                                                                       YerithERPConfig::currency));
	factureTexDocument.replace("YERITHCLIENT",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getClientName()));
	factureTexDocument.replace("CLIENTYERITHREPRESENTANT",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getClientRepresentantName()));
	factureTexDocument.replace("CLIENTYERITHCITY",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getClientCity()));
	factureTexDocument.replace("CLIENTYERITHPOBOX",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getClientPoBox()));
	factureTexDocument.replace("CLIENTYERITHMAIL",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getClientEmail()));
	factureTexDocument.replace("CLIENTYERITHPHONE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					yerith_client_account_sale_info.getClientPhone()));
	QString prefixFileName;
#ifdef YERITH_FRANCAIS_LANGUAGE
    prefixFileName.
    append
    (YerithUtils::
     getUniquePrefixFileInTemporaryFilesDir
     ("yerith-erp-vente-facture-grand"));
#endif
#ifdef YERITH_ENGLISH_LANGUAGE
    prefixFileName.
    append
    (YerithUtils::
     getUniquePrefixFileInTemporaryFilesDir
     ("yerith-erp-sale-receipt-big"));
#endif //qDebug() << "++\n" << factureTexDocument;
	QFile tmpLatexFile(prefixFileName + "tex");
	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(factureTexDocument.toUtf8());
	}

	tmpLatexFile.close();
	return YerithERPProcess::compileLatex(prefixFileName);
}


void YerithUtils::getFactureTexTableString(QString 			&texTable_IN_OUT,
										   QTableWidget 	&tableStandardItemModel,
										   int 				quantiteVendue,
										   double 			totalTVA,
										   double 			sommeTotal)
{
	texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
			"\\resizebox{\\textwidth}{!}{\n"
			"\\centering\n"
			"\\begin{tabular}"
			"{|");
	/**
	 * We reduce the header items of the table by 1 element
	 * (at position 4: stock quantity field) because the
	 * stock quantity field shall not be part of the
	 * information printed to the customer.
	 */
	int texTableColumnCount = tableStandardItemModel.columnCount();
	//Tex table header
	for (int k = 0; k < texTableColumnCount; ++k)
	{
		if (k == YerithPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		if (YerithPointDeVenteTableWidget::TOTAL_COLUMN == k
				|| YerithPointDeVenteTableWidget::TOTAL_TVA_COLUMN == k)
		{
			texTable_IN_OUT.append("r|");
		}
		else if (YerithPointDeVenteTableWidget::DESIGNATION_COLUMN == k
				|| YerithPointDeVenteTableWidget::CATEGORIE_COLUMN == k)
		{
			texTable_IN_OUT.append("l|");
		}
		else
		{
			texTable_IN_OUT.append("r|");
		}
	}

	texTable_IN_OUT.append("} \\hline\n");	//Table header end
	QTableWidgetItem *item;
	for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
	{
		if (k == YerithPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		item = tableStandardItemModel.horizontalHeaderItem(k);
		if (item)
		{
			QString emptyString;
			handleTexTableItemText(tableStandardItemModel.columnCount(),
					texTable_IN_OUT, k, emptyString);
		}
	}
	for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
	{
		if (k == YerithPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		item = tableStandardItemModel.horizontalHeaderItem(k);
		if (item)
		{
			QString itemText(item->text().prepend("\\textbf{").append("}"));
			handleTexTableItemText(tableStandardItemModel.columnCount(),
					texTable_IN_OUT, k, itemText);
		}
	}
	for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
	{
		if (k == YerithPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		item = tableStandardItemModel.horizontalHeaderItem(k);
		if (item)
		{
			QString emptyString;
			handleTexTableItemText(tableStandardItemModel.columnCount(),
					texTable_IN_OUT, k, emptyString);
		}
	}

	cleanUpTexTableLastString(texTable_IN_OUT);
	texTable_IN_OUT.append("\\hline\n");
	//Tex table body
	for (int j = 0; j < tableStandardItemModel.rowCount(); ++j)
	{
		for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
		{
			if (k == YerithPointDeVenteTableWidget::QTE_TOTAL_STOCK)
			{
				continue;
			}

			item = tableStandardItemModel.item(j, k);
			if (item)
			{
				QString itemText(item->text());
				if (itemText.length()
						> YerithUtils::FACTURE_GRAND_STRING_MAX_CHARS)
				{
					itemText.truncate(
							YerithUtils::FACTURE_GRAND_STRING_MAX_CHARS - 1);
					itemText.append(".");
					//qDebug() << QString("article name after truncate: %1").arg(articleName);
				}

				handleFactureTexTableItemText(
						tableStandardItemModel.columnCount(), texTable_IN_OUT,
						k, itemText);
			}
			else
			{
				if (k < tableStandardItemModel.columnCount() - 1)
				{
					texTable_IN_OUT.append("\"\" &");
				}
				else
				{
					texTable_IN_OUT.append("\"\"  \\hline\n");
				}
			}
		}

		cleanUpTexTableLastString(texTable_IN_OUT);
		texTable_IN_OUT.append("\\hline\n");
	}

	QString quantiteVendueStr(
			LATEX_IN_OUT_handleForeignAccents(GET_NUM_STRING(quantiteVendue)));
	quantiteVendueStr.prepend("\\textbf{").append("}");
	QString totalTVAStr(
			LATEX_IN_OUT_handleForeignAccents(GET_DOUBLE_STRING(totalTVA)));
	totalTVAStr.prepend("\\textbf{").append("}");
	QString sommeTotalStr(
			LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(sommeTotal)));
	sommeTotalStr.prepend("\\textbf{").append("}");
	QString totalStr;
#ifdef YERITH_FRANCAIS_LANGUAGE
    totalStr.
    append
    ("TOTAUX");
#endif
#ifdef YERITH_ENGLISH_LANGUAGE
    totalStr.
    append
    ("TOTAL");
#endif
	texTable_IN_OUT.append(
			QString(
					"\\multicolumn{%1}{|c}{\\textbf{%2}} & & %3 & & %4 & %5 \\\\ \\hline \n").arg(
					GET_NUM_STRING(tableStandardItemModel.columnCount() - 6),
					totalStr, quantiteVendueStr, totalTVAStr, sommeTotalStr));
	//Removes the empty character "" from Latex output
	texTable_IN_OUT.replace("\"\"", "");
	texTable_IN_OUT.append("\\end{tabular}}\n"
			"\\end{table*}\n");
}


void YerithUtils::getFactureSmallTexTableString(QString &texTable_IN_OUT,
		QTableWidget &tableStandardItemModel, int quantiteVendue,
		double totalTVA, double sommeTotal, double montantRecuDuCient, /* = 0.0 */
		double montantARembourserAuClient
		/* = 0.0 */
		)
{
	texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
			"\\resizebox{\\textwidth}{!}{"
			"\\centering\n"
			"\\begin{tabular}{lrr}\n");
	int tableColumnCount = tableStandardItemModel.columnCount();
	QTableWidgetItem *item = 0;
	for (int k = 0; k < tableColumnCount; ++k)
	{
		if (k == YerithPointDeVenteTableWidget::DESIGNATION_COLUMN
				|| k == YerithPointDeVenteTableWidget::QTE_COLUMN
				|| k == YerithPointDeVenteTableWidget::TOTAL_COLUMN)
		{
			item = tableStandardItemModel.horizontalHeaderItem(k);
			if (item)
			{
				QString itemText(item->text());
				handleTexTableItemText(tableStandardItemModel.columnCount(),
						texTable_IN_OUT, k, itemText);
			}
		}
	}
	cleanUpTexTableLastString(texTable_IN_OUT);
	texTable_IN_OUT.append("\\hline\n");
	QString articleName;
	int articleNameLength;
	QTableWidgetItem *articleNameItem;
	QTableWidgetItem *articleQteItem;
	QTableWidgetItem *totalTtcItem;
	int tableRowCount = tableStandardItemModel.rowCount();
	for (int k = 0; k < tableRowCount; ++k)
	{
		articleNameItem = tableStandardItemModel.item(k,
				YerithPointDeVenteTableWidget::DESIGNATION_COLUMN);
		articleQteItem = tableStandardItemModel.item(k,
				YerithPointDeVenteTableWidget::QTE_COLUMN);
		totalTtcItem = tableStandardItemModel.item(k,
				YerithPointDeVenteTableWidget::TOTAL_COLUMN);
		if (articleNameItem && articleQteItem && totalTtcItem)
		{
			articleName = articleNameItem->text();
			articleNameLength = articleName.length();
			//qDebug() << QString("article name: %1, articleNameLength: %2")
			//                          .arg(articleName, articleNameLength);
			if (articleNameLength
					> YerithUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS)
			{
				articleName.truncate(
						YerithUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS - 1);
				//qDebug() << QString("article name after truncate: %1").arg(articleName);
			}

			texTable_IN_OUT.append(
					QString("%1. %2 & $x%3$ & %4 \\\\ \n").arg(
							QString::number(k + 1),
							YerithUtils::LATEX_IN_OUT_handleForeignAccents(
									articleName), articleQteItem->text(),
							YerithUtils::LATEX_IN_OUT_handleForeignAccents(
									totalTtcItem->text())));
		}
	}

	//We now add the sum
	QString sommeTotalStr(
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(sommeTotal)));
	texTable_IN_OUT.append(
			QString("\\hline\n & $%1$ & %2 \\\\ \n").arg(
					QString::number(quantiteVendue), sommeTotalStr));
	texTable_IN_OUT.append(QString(" & & \\\\ \n"));
	QString totalTVAStr(
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(totalTVA)));
	QString montantRecuDuClientStr(
			YerithUtils::LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(montantRecuDuCient)));
	QString montantARembourserAuClientStr;
	if (montantARembourserAuClient >= 0)
	{
		montantARembourserAuClientStr =
				YerithUtils::LATEX_IN_OUT_handleForeignAccents(
						GET_CURRENCY_STRING_NUM(montantARembourserAuClient));
	}
	else
	{
		montantARembourserAuClientStr =
				YerithUtils::LATEX_IN_OUT_handleForeignAccents(
						GET_CURRENCY_STRING_NUM(0.0));
	}

	texTable_IN_OUT.append(
			QObject::tr("dont total TVA: & & %1\\\\ \n").arg(totalTVAStr));
	texTable_IN_OUT.append(
			QObject::tr("Montant re\\c{c}u: & & %1\\\\ \n").arg(
					montantRecuDuClientStr));
	texTable_IN_OUT.append(
			QObject::tr("Montant remi: & & %1\\ \n").arg(
					montantARembourserAuClientStr));
	texTable_IN_OUT.append("\\end{tabular}}\n"
			"\\end{table*}\n");
}


void YerithUtils::refreshSalesStrategy(YerithSqlTableModel &curStocksTableModel,
		YerithLineEdit *lineEdit_recherche_article,
		YerithLineEdit *lineEdit_recherche_article_codebar)
{
	QString curStocksTableModelName = curStocksTableModel.sqlTableName();
	if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
			YerithERPConfig::salesStrategy))
	{
		lineEdit_recherche_article->setupMyQCompleterFIFO(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarFIFO(
				curStocksTableModelName);
	}
	else if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
			YerithERPConfig::salesStrategy))
	{
		lineEdit_recherche_article->setupMyQCompleterLIFO(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarLIFO(
				curStocksTableModelName);
	}
	else if (YerithUtils::isEqualCaseInsensitive(
			YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
			YerithERPConfig::salesStrategy))
	{
		lineEdit_recherche_article->setupMyQCompleterFEFO(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarFEFO(
				curStocksTableModelName);
	}
	else	//YerithConfig::STRATEGIE_VENTE_SORTIE_ALL
	{
		lineEdit_recherche_article->setupMyQCompleterALL(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarALL(
				curStocksTableModelName);
	}
}


void YerithUtils::get_FACTURE_INVOICE_TexDocumentString(QString &texDocumentString_in_out)
{
	QString printString("\n \\vspace*{0.9cm}\n");

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		printString
			.append("\\textbf{INVOICE \\textcolor{yerithColorRed}{TOTAL remaining AMOUNT ALL TAXES} "
					"COMPRISED FOR yourself IS OF "
					"\\textcolor{yerithColorRed}{'YERITHFACTUREINVOICEREMAININGAMOUNT'} (YERITHMONTANTLITERAL)}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(EN_template_FACTURE_GRAND_tex);
	}
	else
	{
		printString
			.append("\\textbf{Arr\\^et\\'e LE \\textcolor{yerithColorRed}{RESTE à PAYER TTC} pour cet achat au compte client "
				    "est de \\textcolor{yerithColorRed}{'YERITHFACTUREINVOICEREMAININGAMOUNT'} (YERITHMONTANTLITERAL)}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(FR_template_FACTURE_GRAND_tex);
	}

	texDocumentString_in_out.append(printString).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerithUtils::
        get_TYPEDOPERATION_UNELIGNEBUDGETAIRE_TexDocumentString(QString &texDocumentString_in_out,
                                                                QString &TYPEDOPERATION_UNELIGNEBUDGETAIRE__tex_table)
{
	texDocumentString_in_out.clear();

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(EN_template_Comptabilite_comptes_Doperations_TEX);
	}
	else
	{
		texDocumentString_in_out.append(FR_template_Comptabilite_comptes_Doperations_TEX);
	}

	texDocumentString_in_out.append(TYPEDOPERATION_UNELIGNEBUDGETAIRE__tex_table).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerithUtils::get_GROUPES_DUN_client_TexDocumentString(QString &texDocumentString_in_out,
														   QString &GROUPES_DUN_CLIENT__tex_table)
{
	texDocumentString_in_out.clear();

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(EN_template_GROUPES_DUN_CLIENT_tex);
	}
	else
	{
		texDocumentString_in_out.append(FR_template_GROUPES_DUN_CLIENT_tex);
	}

	texDocumentString_in_out.append(GROUPES_DUN_CLIENT__tex_table).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerithUtils::get_GROUPES_DUN_EMPLOYE_TexDocumentString(QString &texDocumentString_in_out,
														    QString &GROUPES_DUN_EMPLOYE__tex_table)
{
	texDocumentString_in_out.clear();

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(EN_template_GROUPES_DUN_EMPLOYE_tex);
	}
	else
	{
		texDocumentString_in_out.append(FR_template_GROUPES_DUN_EMPLOYE_tex);
	}

	texDocumentString_in_out.append(GROUPES_DUN_EMPLOYE__tex_table).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerithUtils::get_RECU_TexDocumentString(QString &texDocumentString_in_out,
											 QString &printString)
{
	printString.append("\n \\vspace*{0.9cm}\n");

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		printString.append("The total amount of this receipt all taxes comprised is of \\textbf{YERITHMONTANTLITERAL}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(EN_template_RECU_GRAND_tex);
	}
	else
	{
		printString.append("Arr\\^et\\'e le pr\\'esent re\\c{c}u au montant total TTC de \\textbf{YERITHMONTANTLITERAL}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(FR_template_RECU_GRAND_tex);
	}

	texDocumentString_in_out.append(printString).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerithUtils::get_RECU_Small_TexDocumentString(QString &texDocumentString_in_out,
												   QString &printString)
{
	texDocumentString_in_out.clear();

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(EN_template_facture_petit_tex);
	}
	else
	{
		texDocumentString_in_out.append(FR_template_facture_petit_tex);
	}

	texDocumentString_in_out.append(printString).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


bool YerithUtils::SAVE_AS_csv_file(YerithWindowsCommons    &aCallingWindow,
                                   YerithTableWidget       &aTableView,
                                   const QString           &csvFileName,
                                   const QString           &strMessage,
                                   int                     row_MAX_TO_GO_export /* = -1 */)
{
	QAbstractItemModel *tableModel = aTableView.model();

	if (0 == tableModel)
	{
		return false;
	}


	int tableModelRowCount = tableModel->rowCount();

	int tableModelColumnCount = tableModel->columnCount();


	if (tableModelRowCount <= 0  ||
	    tableModelColumnCount <= 0)
	{
        YerithQMessageBox::information(&aCallingWindow,
                                       QObject::tr("No CSV data"),
                                       QObject::tr("No CSV data to save out !"));
		return false;
	}


	QString csvFileContent;

	QHeaderView *a_calling_window_qheaderview = aTableView.horizontalHeader();

	if (0 == a_calling_window_qheaderview)
	{
        YerithQMessageBox::information(&aCallingWindow,
                                       QObject::tr("Bad deployment"),
                                       QObject::tr("Table bad deployment !"));

		return false;
	}


	QVariant anItem;

	QString anItemText;

	for (int k = 0; k < a_calling_window_qheaderview->count(); ++k)
	{
		anItem = tableModel->headerData(k, Qt::Horizontal);

		if (anItem.isValid())
		{
			anItemText = anItem.toString();

			csvFileContent.append(QString("\"%1\"%2 ")
                                    .arg(anItemText,
                                         YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
		}
	}


	csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");



	int MAX_TABLE_MODDEL_ROW_COUNT__to_export = tableModelRowCount;


	if (row_MAX_TO_GO_export > -1)
	{
        MAX_TABLE_MODDEL_ROW_COUNT__to_export = row_MAX_TO_GO_export;
	}


	QMap<int, QVariant> item_data;

	for (unsigned int j = 0; j < MAX_TABLE_MODDEL_ROW_COUNT__to_export; ++j)
	{
		for (unsigned int k = 0; k < tableModelColumnCount; ++k)
		{
			item_data = tableModel->itemData(tableModel->index(j, k));

            anItemText = item_data.value(Qt::DisplayRole).toString();

            csvFileContent
                .append(QString("\"%1\"%2 ")
                         .arg(anItemText,
                              YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
		}

		csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");
	}


	QString yerithStocksListingCSVFileName
				(QString("%1/%2")
					.arg(YerithERPConfig::temporaryFilesDir,
						 csvFileName));


	yerithStocksListingCSVFileName =
				FILE_NAME_USERID_CURRENT_TIME(yerithStocksListingCSVFileName);


        yerithStocksListingCSVFileName
            = QFileDialog::getSaveFileName(&aCallingWindow,
                             	 	 	   "Type in a '.csv' file name ",
										   yerithStocksListingCSVFileName,
										   QString("%1 \"*.csv\" (*.csv)")
                                             .arg(strMessage));


    yerithStocksListingCSVFileName.append(".csv");

    QFile tmpFile(yerithStocksListingCSVFileName);

	if (tmpFile.open(QFile::WriteOnly))
	{
		tmpFile.write(csvFileContent.toUtf8());
	}

	tmpFile.close();

	return true;
}


bool YerithUtils::export_csv_file(YerithWindowsCommons &aCallingWindow,
                                  YerithTableView &aTableView,
                                  QList<int> databaseTableColumnsToIgnore,
                                  const QString &csvFileName,
                                  const QString &strMessage)
{
	YerithPOSQStandardItemModel *tableModel = aTableView.getStandardItemModel();
	if (0 == tableModel)
	{
		return false;
	}

	int tableModelRowCount = tableModel->rowCount();
	int tableModelColumnCount = tableModel->columnCount();
	if (tableModelRowCount <= 0 || tableModelColumnCount <= 0
			|| tableModelColumnCount <= databaseTableColumnsToIgnore.count())
	{
		YerithQMessageBox::information(&aCallingWindow,
				QObject::tr("pas de données à exporter au format csv"),
				QObject::tr(
						"Il n'y a pas de données à exporter au format csv !"));
		return false;
	}

	QString csvFileContent;
	QHeaderView *a_calling_window_qheaderview = aTableView.horizontalHeader();
	if (0 == a_calling_window_qheaderview)
	{
		YerithQMessageBox::information(&aCallingWindow,
				QObject::tr("MAUVAIS DÉPLOIEMENT"),
				QObject::tr("MAUVAIS DÉPLOIEMENT du tableau!"));
		return false;
	}

	YerithQStandardItem *anItem = 0;
	QString anItemText;
	int a_logical_index_k = 0;
	for (int k = 0; k < a_calling_window_qheaderview->count(); ++k)
	{
		a_logical_index_k = a_calling_window_qheaderview->logicalIndex(k);
		if (databaseTableColumnsToIgnore.contains(a_logical_index_k))
		{
			continue;
		}

		anItem = (YerithQStandardItem*) tableModel->horizontalHeaderItem(
				a_logical_index_k);
		if (0 != anItem)
		{
			anItemText = anItem->text();
			csvFileContent.append(
					QString("\"%1\"%2 ").arg(anItemText,
							YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
		}
	}

	csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");

	for (unsigned int j = 0; j < tableModelRowCount; ++j)
	{
		for (unsigned int k = 0; k < tableModelColumnCount; ++k)
		{
			if (databaseTableColumnsToIgnore.contains(k))
			{
				continue;
			}

			anItem = (YerithQStandardItem*) tableModel->item(j, k);

			if (0 != anItem)
			{
				anItemText = anItem->get_rawUNMODIFIED_FOR_USER_VIEWING_text();
//                QDEBUG_STRING_OUTPUT_2("anItemText", anItemText);

				csvFileContent
					.append(QString("\"%1\"%2 ")
								.arg(anItemText,
								     YerithUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
			}
		}

		csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");
	}


	QString yerithStocksListingCSVFileName
				(QString("%1/%2")
					.arg(YerithERPConfig::temporaryFilesDir,
						 csvFileName));


	yerithStocksListingCSVFileName =
				FILE_NAME_USERID_CURRENT_TIME(yerithStocksListingCSVFileName);


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        yerithStocksListingCSVFileName
            = QFileDialog::getSaveFileName(&aCallingWindow,
                             	 	 	   "Type in '.csv' file name ",
										   yerithStocksListingCSVFileName,
										   QString("%1 \"*.csv\" (*.csv)")
										   	   .arg(strMessage));
    }
    else
    {
        yerithStocksListingCSVFileName
            = QFileDialog::getSaveFileName(&aCallingWindow,
                             	 	 	   "Saisir le nom du fichier '.csv'",
										   yerithStocksListingCSVFileName,
										   QString("%1 \"*.csv\" (*.csv)")
										   	   .arg(strMessage));
    }


    yerithStocksListingCSVFileName.append(".csv");

    QFile tmpFile(yerithStocksListingCSVFileName);

	if (tmpFile.open(QFile::WriteOnly))
	{
		tmpFile.write(csvFileContent.toUtf8());
	}

	tmpFile.close();

	return true;
}


void YerithUtils::getLatexFinancialAccountingReportData(QString &texDocumentString_in_out)
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(YerithUtils::EN_template_bilan_comptable_tex);
    }
    else
    {
    	texDocumentString_in_out.append(YerithUtils::FR_template_bilan_comptable_tex);
    }
}


bool YerithUtils::import_csv_file_content(QString aCsvFileFullPath,
										  QStringList &wordList_IN_OUT)
{
	wordList_IN_OUT.clear();

	QFile aCsvFile(aCsvFileFullPath);

	if (!aCsvFile.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream aCsvFileTextStream(&aCsvFile);

	wordList_IN_OUT.append(aCsvFileTextStream.readAll().split(YerithUtils::LINE_ENDING_STRING_REGEXP));

	return true;
}


void YerithUtils::getLatexCLIENT_LOYALTY_CARD_template(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out
    		.append(YerithUtils::EN_template_carte_de_fidelite_client_TEMPLATE_tex);
    }
    else
    {
    	texDocumentString_in_out
    		.append(YerithUtils::FR_template_carte_de_fidelite_client_TEMPLATE_tex);
    }
}


void YerithUtils::getLatex_FINANCIALexpense_Data(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerithUtils::EN_template_bon_de_commande_TEX));
    }
    else
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerithUtils::FR_template_bon_de_commande_TEX));
    }
}


void YerithUtils::getLatexSupplierData(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerithUtils::EN_template_fiche_dun_fournisseur_tex));
    }
    else
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerithUtils::FR_template_fiche_dun_fournisseur_tex));
    }
}


void YerithUtils::getLatexCustomerData(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
					.arg(YerithUtils::EN_template_fiche_client_tex));
    }
    else
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
					.arg(YerithUtils::FR_template_fiche_client_tex));
    }
}


void YerithUtils::getSortieDesStocksTexDocumentString(QString &texDocumentString_in_out,
													  QString &printString)
{
	texDocumentString_in_out.clear();

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerithUtils::EN_template_sortie_des_stocks_grand_tex,
						 printString));
	}
	else
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerithUtils::FR_template_sortie_des_stocks_grand_tex,
						 printString));
	}
}


void YerithUtils::getSortieDesStocksSmallTexDocumentString(QString &texDocumentString_in_out,
														   QString &printString)
{
	texDocumentString_in_out.clear();

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerithUtils::EN_template_sortie_des_stocks_petit_tex,
						 printString));
	}
	else
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerithUtils::FR_template_sortie_des_stocks_petit_tex,
						 printString));
	}
}

