/*
 * yerith-erp-utils.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_UTILS_HPP_
#define SRC_YERITH_UTILS_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yerith-erp-spell-numbers.hpp"

#include "src/utils/yerith-erp-client-account-sale-info.hpp"

#include "src/utils/yerith-erp-database.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"

#include "src/widgets/table-view/yerith-erp-table-view.hpp"

#include "src/utils/yerith-erp-service-stock-marchandise-data.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/widgets/yerith-erp-combo-box.hpp"

#include <QtCore/QRegExp>
#include <QtCore/QCryptographicHash>
#include <QtCore/QLocale>
#include <QtCore/QDate>
#include <QtCore/QDir>

#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QStandardItemModel>

#include <QtGui/QRegExpValidator>

#include <QtGui/QIntValidator>

#include <QtGui/QDoubleValidator>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QStyle>

#include <QtSql/QSqlTableModel>


class YerithLogger;

class YerithTableWidget;
class YerithTableViewWITHpagination;
class YerithDatabaseTableColumn;
class YerithDatabase;
class YerithComboBox;
class YerithQMessageBox;
class QSqlTableModel;
class QStandardItemModel;
class QTableWidget;
class QString;
class QWidget;
class QMainWindow;
class QLabel;
class QComboBox;
class QRegExp;
class QLocale;
class QProcess;

class YerithPOSAdminWindowsCommons;
class YerithWindowsCommons;
class YerithERPWindows;
class YerithLineEdit;
class YerithSqlTableModel;

class QPluginLoader;
class QtPlugin;


enum service_stock_already_exist_type
{
    SERVICE_REFERENCE_EXISTS = 0,
    SERVICE_STOCK_CATEGORY_EXIST = 1,
    SERVICE_STOCK_DESIGNATION_EXIST = 2,
    SERVICE_STOCK_EXIST_IN_MERCHANDISE_DB_TABLE = 3,
    SERVICE_STOCK_UNDEFINED = 4
};


class YerithUtils : public QObject
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline YerithUtils()
	{
        RELOAD_VIEW_STRINGS();    }

	static void RELOAD_VIEW_STRINGS();

    inline ~YerithUtils()
    {
    }

    static void YERITH_resizeEvent(QWidget &a_current_widget_to_move,
                                   const QSize &a_current_windows_SIZE,
                                   const QSize &a_current_windows_PREVIOUS_SIZE);


    template<class classType, typename parameterType>
    static void run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION(classType *aClassInstance,
                                                         parameterType *aParamValue,
                                                         void (classType::*func)(parameterType *));


    template<class classType, typename parameterType, typename returnType>
    static void run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION(classType *aClassInstance,
                                                         parameterType *aParamValue,
                                                         returnType(classType::*func)(parameterType *),
                                                         returnType *aRetValue = 0);


    template<class classType>
    static void run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK(classType *aClassInstance,
                                                                       bool (classType::*func)(void));


    template<class aTableViewClassType>
    static bool instanceOf__YerithTableViewWITHpagination(aTableViewClassType
                                                          * aTableView);


    static bool creerNouveauClient(QString &proposedCustomerName_IN_OUT,
                                   YerithWindowsCommons *_callingWindow = 0);


    static bool creerNouveauFournisseur(QString &
                                        proposedFourisseurName_IN_OUT,
                                        YerithWindowsCommons *_callingWindow =
                                                        0);

    static bool creerNouveauDepartementDeProduits(QString &
                                                  proposedProductDepartmentName_IN_OUT,
                                                  YerithWindowsCommons *
                                                  _callingWindow = 0);

    static bool check_IF_departement_produit_exists(QString &
                                                    proposedProductDepartmentName);

    static bool creerNouvelleCategorie(QString &proposedCategorieName_IN_OUT,
                                       QString &proposedProductDepartmentName,
                                       YerithWindowsCommons *_callingWindow =
                                                       0);

    static bool GREP_YERITH_FILE_CONTENT(const QString &file_full_path,
    									 const QString &EXECUTABLE_full_path,
                                         const QString &keyword_IN);

    static void YERITH_READ_FILE_CONTENT(QFile &file,
                                         QString &fileContentVar);

    static bool YERITH_CREATE_FOLDER(const QString &aFullPathDir,
                                     YerithLogger *aLogger = 0);

    static bool GZIP_YERITH_FILE(const QString &
                                 program_working_directory_full_path,
                                 const QString &file_full_path);

    static inline bool CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING(const QString &
                                                               aCur_db_STRING_STAR_SEPARATED_VALUE,
                                                               const QString &
                                                               a_proposed_duplicate_to_test_string)
    {
        return aCur_db_STRING_STAR_SEPARATED_VALUE.split(YerithUtils::STAR_CHAR,
                                                         Qt::SkipEmptyParts).
               count(a_proposed_duplicate_to_test_string) > 0;
    }

    static void REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(QString &
                                                                  SEPARATED_RESULTING_QSTRING_IN_OUT,
                                                                  const QString
                                                                  &
                                                                  aCur_db_STRING);

    static inline void SPLIT_STAR_SEPARATED_DB_STRING(QStringList &
                                                      SEPARATED_RESULTING_QSTRINGLIST_IN_OUT,
                                                      const QString &
                                                      aCur_db_STRING_STAR_SEPARATED_VALUE)
    {
        SEPARATED_RESULTING_QSTRINGLIST_IN_OUT.append
        (aCur_db_STRING_STAR_SEPARATED_VALUE.split
         (YerithUtils::STAR_CHAR, Qt::SkipEmptyParts));
    }

    static bool APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(const QString &
                                                               aNewValue,
                                                               QString &
                                                               aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT);

    static
    enum service_stock_already_exist_type IS_STOCK_DESIGNATION_OR_REFERENCE_UNIQUE(const QString &
                                                                                   aStockServiceReference,
                                                                                   const QString &
                                                                                   aStockServiceCategory,
                                                                                   const QString &
                                                                                   aStockServiceDesignation,
                                                                                   QString &
                                                                                   curExistingReferenceCategoryDesignation_PRODUCT_in_out);

    static QString GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(int day_of_week);

    static bool UPDATE_PREVIOUS_SELLING_PRICE_IN_ProductList(const
                                                             YerithERPStockMarchandiseData
                                                             &
                                                             aServiceStockData,
                                                             YerithWindowsCommons
                                                             * _callingWindow =
                                                                             0);

    static bool UPDATE_PREVIOUS_BUYING_PRICE_IN_ProductList(const YerithERPStockMarchandiseData &aServiceStockData,
    														YerithWindowsCommons *_callingWindow = 0);


    static bool insertStockItemInProductList(YerithERPStockMarchandiseData &
                                             aServiceStockData_IN_OUT,
                                             YerithWindowsCommons *
                                             _callingWindow = 0);

    static bool insert_SERVICE_ItemInProductList(YerithERPServiceData &
                                                 aServiceStockData_IN_OUT,
                                                 YerithWindowsCommons *
                                                 _callingWindow = 0);

    static QString YERITH_TRUNCATE_STRING_ACCORDING_TO_SETTING(const QString &
                                                               aString_IN);

    static QString GET_STRING_WITH_NUMBERS_WITHIN_KEPT_FOR_LATEX(const QString
                                                                 &
                                                                 aStringWITHBLANKSPACEWITHIN);

    static inline QString GET_STRING_WITH_BLANK_SPACE_WITHIN_KEPT_FOR_LATEX(const QString &
                                                                            aStringWITHBLANKSPACEWITHIN)
    {
        return QString(aStringWITHBLANKSPACEWITHIN).
               replace(YerithUtils::EMPTY_SPACE_REGEXP, "\\ ");
    }

    static double get_prix_dachat_wheter_exists(const QString &stocksID);

    static inline double getMargeBeneficiaire(double prix_vente,
                                              double prix_dachat,
                                              double montant_tva)
    {
        return (prix_vente - QString::number(montant_tva, 'f', 2).toDouble() -
                prix_dachat);
    }

    static inline bool isProfitable(double prix_vente,
                                    double prix_dachat, double montantTva)
    {
        return YerithUtils::getMargeBeneficiaire(prix_vente, prix_dachat,
                                                 montantTva) >= 0;
    }

    static bool REMOVE_QSTRING_LIST(QStringList &
                                    a_qstring_list_to_remove_elements,
                                    QStringList &a_qstring_list);

    static inline bool REMOVE_QSTRING_LIST(QStringList &
                                           a_qstring_list_to_remove_elements,
                                           const QStringList &a_qstring_list)
    {
        return
                        YerithUtils::REMOVE_QSTRING_LIST(a_qstring_list_to_remove_elements,
                                                         const_cast <
                                                         QStringList & >(a_qstring_list));
    }

    static bool start_db_transaction();

    static bool rollback_db_transaction();

    static bool commit_db_transaction();

    static void setAllWindows(YerithERPWindows *allWindows);

    static inline YerithERPWindows *getAllWindows()
    {
        return _allWindows;
    }

    static YerithDatabase *getDatabase();

    static inline QString GET_YRI_DB_RUNTIME_VERIF_EXECUTABLE_FULL_PATH()
    {
        return QString("%1/bin/%2").
               arg(YerithERPConfig::YERITH_ERP_3_0_yri_db_runtime_verif_HOME_FOLDER,
                   YerithERPConfig::YRI_DB_RUNTIME_VERIF_SYSTEM_DAEMON_ID);
    }

    static inline QString getYerithAlertDaemonExecutableFullpath()
    {
        return QString("%1/bin/%2").
               arg(YerithERPConfig::YERITH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER,
                   YerithERPConfig::YERITH_ERP_3_0_SYSTEM_DAEMON_ID);
    }

    static void writeStringToQFilewithUTF8Encoding(QFile &
                                                   latexContentOutputFile,
                                                   QString
                                                   aLatexFileContentString);

    static inline QString getUniquePrefixFileInTemporaryFilesDir(QString
                                                                 aPrefixFileName)
    {
        return QString("%1/%2.").arg(YerithERPConfig::temporaryFilesDir,
                                     YerithUtils::
                                     getFileNameWithUserIDAndCurrentTime
                                     (aPrefixFileName));
    }

    static const QString getCurrentAdminWindowReceiptsFormatAccordingToLanguage(const QString &
                                                                                currentFacturesTypeValue);

    static void saveCurrentAdminWindowFacturesTypeAccordingToLanguage(const QString &currentFacturesTypeValue);

    static bool executer_fichier_sql(const QString &fileName,
                                     YerithLogger  *logger = 0);

    static inline QString prepareSqlStr(const QString &aStr)
    {
        return QString(aStr).replace("'", "''");
    }


    static inline uint YERITH_GET_ROTATIONAL_ARRAY_NEXT_VALUE(uint position)
    {
        return (position <= 12) ? position : ((position - 1) % 12);
    }

    static double YERITH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(const QString &aDoubleQString);

    static int execQueryRowCount(const QString &strQuery,
                                 YerithLogger  *logger = 0);

    static void qDebugStrings(const QString &firstString,
                              const QStringList &aStringList);

    static void qDebugStrings(const QString &firstString,
                              const QString &secondString =
                                              YerithUtils::EMPTY_STRING);

    static bool execQuery(const QString &strQuery, YerithLogger *logger = 0);

    static int execQuery(QSqlQuery &query, const QString &strQuery,
                         YerithLogger *logger = 0);

    static int execQuery(QSqlQuery &query, YerithLogger *logger = 0);

    //static bool imprimer_pdf_document(QWidget *widget);

    static inline QString get_latex_bold_text(const QString &text)
    {
        return QString("\\textbf{%1}").arg(text);
    }

    static QString get_text_sql_escaped(const QVariant &qv);

    static QString get_text(const QVariant &qv);

    static void loadPixmapFromDB(QLabel &label_image,
                                 QVariant imageVariant, const char *format);

    static void savePixmapToByteArray(QByteArray &buffer,
                                      const QPixmap &pixmap,
                                      const char *format);

    static void savePixmapToFile(const QString &aFileFullPath,
                                 const QPixmap &pixmap, const char *format);

    static void supprimer_image(QLabel &label_image);

    static void selectionner_image(QWidget *parent, QLabel &label_image);

    static void CREATE_AN_EMPTY_FILE(const QString &aFileFullPath);

    static void getCenterPosition(unsigned desktopWidth,
                                  unsigned desktopHeight,
                                  const QWidget &aWidget, QPoint &result);

    static void addFiltre(YerithLineEdit *aLineEdit,
                          QString filterName, QString &yerithFiltre_in_out);

    static inline int getComboBoxDatabaseQueryValue(const QString &
                                                    comboBoxStringValue,
                                                    QMap < int,
                                                    QString > *toViewStringMAP)
    {
        return (0 != toViewStringMAP) ?
               YerithUtils::getComboBoxDatabaseQueryValue(comboBoxStringValue,
                                                          *toViewStringMAP) : -1;
    }


    static int getComboBoxDatabaseQueryValue(const QString &
                                             comboBoxStringValue, QMap < int,
                                             QString > &toViewStringMAP);


    static inline QByteArray md5Hash(QString data)
    {
        return QCryptographicHash::hash(data.toLatin1(),
                                        QCryptographicHash::Md5);
    }


    static inline int get_index_of_table_raw_column(const QStringList &
                                                    tableModelRawHeaders,
                                                    const QString &
                                                    databaseTableColumnRAW_NAME)
    {
        return tableModelRawHeaders.indexOf(databaseTableColumnRAW_NAME);
    }

    static void createTableModelHeaders_DEPRECATED(YerithSqlTableModel 	&tableModel,
                                        		   QStandardItemModel 	&stdItemModel,
												   QStringList 			&tableModelHeaders,
												   QStringList 			&tableModelRawHeaders_IN_OUT);


    static void createTableModelHeaders_QStringList(YerithSqlTableModel 	&tableModel,
                                                    QStandardItemModel 		&stdItemModel,
                                                    QStringList 			&tableModelHeaders,
                                                    QStringList 			&tableModelRawHeaders_IN_OUT,
                                                    YerithWindowsCommons 	*A_CALLING_WINDOW,
                                                    bool 					CHARGES_FINANCIERES_WINDOW,
                                                    QList<QString>          &CHARGES_FINANCIERES_WINDOW_Db_ColumnS,
                                                    QList<QString>          &CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STRS);


    static void createTableModelHeaders(YerithSqlTableModel 	&tableModel,
                                        QStandardItemModel 		&stdItemModel,
                                        QStringList 			&tableModelHeaders,
                                        QStringList 			&tableModelRawHeaders_IN_OUT,
										YerithWindowsCommons 	*A_CALLING_WINDOW,
										bool 					CHARGES_FINANCIERES_WINDOW = false,
										QString                 CHARGES_FINANCIERES_WINDOW_Db_Column = YerithUtils::EMPTY_STRING,
										QString                 CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STR = YerithUtils::EMPTY_STRING);


    static inline const char * GET_CHARACTER_C_STRING(QString a_qstring)
    {
        return a_qstring.toStdString().c_str();
    }


    static inline QString toUtf8String(QString &str)
    {
        return QString::fromUtf8(str.toStdString().c_str());
    }


    static QString generateSqlLike__AS_IS(QString sqlTableColumn,
                                          QString searchStr);

    static QString generateSqlLike(QString sqlTableColumn, QString searchStr);

    static inline QString generateSqlLike(const char *sqlTableColumn,
                                          QString searchStr)
    {
        return YerithUtils::generateSqlLike(QString(sqlTableColumn), searchStr);
    }

    static inline QString generateSqlIs(QString sqlTableColumn,
                                        QString searchStr)
    {
        return QString("%1 = \'%2\'").arg(sqlTableColumn,
                                          searchStr.replace("'", "''"));
    }

    static inline QString generateSqlIs(const char *sqlTableColumn,
                                        QString searchStr)
    {
        return YerithUtils::generateSqlIs(QString(sqlTableColumn), searchStr);
    }

    static inline QString generateSqlIsNotEmpty(QString sqlTableColumn)
    {
        return QString("%1 != \'\'").arg(sqlTableColumn);
    }

    static inline QString generateSqlIsNotEmpty(const char *sqlTableColumn)
    {
        return YerithUtils::generateSqlIsNotEmpty(QString(sqlTableColumn));
    }

    static QString generateSqlIsEmpty(QString sqlTableColumn);

    static inline QString generateSqlIsEmpty(const char *sqlTableColumn)
    {
        return YerithUtils::generateSqlIsEmpty(QString(sqlTableColumn));
    }

    static inline void RESET_YERITH_VIEW_RECORD_WIDTH_LAST_SELECTED_ID(YerithSqlTableModel &
                                                                       aYerithSqlTableModel_in_out)
    {
        aYerithSqlTableModel_in_out.resetFilter("src/utils/yerith-erp-utils.hpp", 480);
    }

    static void GET_YERITH_VIEW_RECORD_WIDTH_LAST_SELECTED_ID(YerithSqlTableModel &
                                                              aYerithSqlTableModel_in_out,
                                                              QSqlRecord &
                                                              resultSQL_RECORD_in_out);

    static double GET_QUANTITE_TOTALE_EN_STOCK(const QString &
                                               nom_departement_produit,
                                               const QString &nom_categorie,
                                               const QString &
                                               designation_produit);

    static inline QString GET_REFERENCE_RESTE_FACTURE_CLIENT(QString
                                                             fixedNumber)
    {
        return YerithUtils::GET_REFERENCE_RECU_SUFFIX(YerithUtils::
                                                      PREFIX_RESTE_FACTURE_CLIENT,
                                                      fixedNumber);
    }

    static inline QString GET_REFERENCE_RECU_SORTIE(QString fixedNumber)
    {
        return YerithUtils::GET_REFERENCE_RECU_SUFFIX(YerithUtils::
                                                      PREFIX_RECU_SORTIE,
                                                      fixedNumber);
    }

    static inline QString GET_REFERENCE_RECU_VENDU(QString fixedNumber)
    {
        return YerithUtils::GET_REFERENCE_RECU_SUFFIX(YerithUtils::
                                                      PREFIX_RECU_VENDU,
                                                      fixedNumber);
    }

    static inline QString GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString
                                                             fixedNumber)
    {
        return YerithUtils::GET_REFERENCE_RECU_SUFFIX(YerithUtils::
                                                      PREFIX_RECU_PAIEMENT_CLIENT,
                                                      fixedNumber);
    }

    static inline QString GET_REFERENCE_RECU_SUFFIX(const QString &prefix,
                                                    const QString &
                                                    fixedNumber)
    {
        return QString("%1%2").arg(prefix,
                                   QString("%1-%2-%3").arg(QDate::currentDate
                                                           ().toString
                                                           ("yyyyMMdd"),
                                                           QTime::currentTime
                                                           ().toString
                                                           ("hhmmss"),
                                                           fixedNumber));
    }

    static int STOCK_PURCHASE_RECORDS_QUANTITY(int stockId);

    static inline int STOCK_PURCHASE_RECORDS_QUANTITY(const QString &stockId)
    {
        return YerithUtils::STOCK_PURCHASE_RECORDS_QUANTITY(stockId.toInt());
    }

    static void getColumnListString(QStringList &columnStringList,
                                    const QString &tableName,
                                    const QString &fieldName,
                                    QString conditionStr =
                                                    YerithUtils::EMPTY_STRING);

    static QString colorLatexTextInput(const QString &colorSpec,
                                       const QString &latexString_in);

    static QString LATEX_IN_OUT_handleForeignAccents(const QString &
                                                     texText_in_out);

    static void cleanUpTexTableLastString(QString &texTable_in_out);

    static void handleFactureTexTableItemText(int texTableColumnCount,
                                              QString &texTable_IN_OUT,
                                              int itemTextColumnPosition,
                                              const QString &itemText);

    static void handleTexTableItemText(int 				texTableColumnCount,
                                       QString 			&texTable_IN_OUT,
                                       int 				itemTextColumnPosition,
                                       const QString 	&itemText);

    static QString IMPRIMER_FACTURE_FOR_CLIENT_ACCOUNT_SALE_INVOICE_FACTURE
    (YerithWindowsCommons &current_printing_window,
     YERITH_CLIENT_ACCOUNT_SALE_info &yerith_client_account_sale_info);

    static void getFactureTexTableString(QString 		&texTable_IN_OUT,
                                         QTableWidget 	&tableStandardItemModel,
                                         int 			quantiteVendue,
                                         double 		totalTVA,
										 double 		sommeTotal);

    static void getFactureSmallTexTableString(QString 		&texTable_IN_OUT,
                                              QTableWidget 	&tableStandardItemModel,
                                              int 			quantiteVendue,
                                              double 		totalTVA,
                                              double 		sommeTotal,
                                              double 		montantRecuDuCient = 0.0,
                                              double 		montantARembourserAuClient = 0.0);

    /**
     * This method is in common used by classes YerithPointDeVenteWindow and
     * YerithSortirWindow.
     */
    static void refreshSalesStrategy(YerithSqlTableModel &curStocksTableModel,
                                     YerithLineEdit 	 *lineEdit_recherche_article,
                                     YerithLineEdit 	 *lineEdit_recherche_article_codebar);


    static bool SAVE_AS_csv_file(YerithWindowsCommons   &aCallingWindow,
                                 YerithTableWidget      &aTableView,
                                 const QString 	        &csvFileName,
                                 const QString 	        &strMessage,
                                 int                    row_MAX_TO_GO_export = -1);


    static bool export_csv_file(YerithWindowsCommons &aCallingWindow,
                                YerithTableView 	 &aTableView,
                                QList<int> 			 databaseTableColumnsToIgnore,
                                const QString 		 &csvFileName,
                                const QString 		 &strMessage);

    static void getLatexFinancialAccountingReportData(QString &texDocumentString_in_out);

    static bool import_csv_file_content(QString aCsvFileFullPath,
                                        QStringList &wordList_IN_OUT);

    static void getLatexCLIENT_LOYALTY_CARD_template(QString &texDocumentString_in_out);

    static void getLatex_FINANCIALexpense_Data(QString &texDocumentString_in_out);

    static void getLatexSupplierData(QString &texDocumentString_in_out);

    static void getLatexCustomerData(QString &texDocumentString_in_out);

    static void getSortieDesStocksTexDocumentString(QString &texDocumentString_in_out,
                                                    QString &printString);

    static void getSortieDesStocksSmallTexDocumentString(QString &texDocumentString_in_out,
                                                         QString &printString);

    static void get_FACTURE_INVOICE_TexDocumentString(QString &texDocumentString_in_out);

    static void get_TYPEDOPERATION_UNELIGNEBUDGETAIRE_TexDocumentString
                        (QString &texDocumentString_in_out,
                         QString &TYPEDOPERATION_UNELIGNEBUDGETAIRE__tex_table);

    static void get_GROUPES_DUN_client_TexDocumentString(QString &texDocumentString_in_out,
    													 QString &GROUPES_DUN_CLIENT__tex_table);

    static void get_GROUPES_DUN_EMPLOYE_TexDocumentString(QString &texDocumentString_in_out,
    													  QString &GROUPES_DUN_EMPLOYE__tex_table);

    static void get_RECU_TexDocumentString(QString &texDocumentString_in_out,
                                           QString &printString);

    static void get_RECU_Small_TexDocumentString(QString &texDocumentString_in_out,
                                                 QString &printString);

    static inline QString getFileNameWithCurrentTime(const QString &fileName)
    {
        return QString("%1%2%3")
        		.arg(fileName,
                     QDate::currentDate().
                     toString("-yyyyMMdd"),
                     QTime::currentTime().
                     toString("_HHmmss"));
    }

    static QString getFileNameWithUserIDAndCurrentTime(const QString &fileName);

    static QString getWindowTitleWithStrategy(QMainWindow *aMainWindow,
                                              QString localVisibleStrategy = "");

    static void fillDBTableColumnNameToDBTableColumnType_TEST(const QString &aDBTableName,
                                                              QMap<QString, QString> &aDBbTableColumnToType_IN_OUT,
                                                              QMap<QString, bool> &aDBTableColumnToIsNotNULL_IN_OUT);

    static QString appendPDFReceiptFormat(QString aStr);


    static QString Get_INTITULE_DE_LA_LIGNE_BUDGETAIRE_FOR_GROUPE_DE_PAIE_Hr
                        (const QString &groupe_de_paie_hr);


    static QString Get_INTITULE_COMPTE_BANCAIRE_FOR_intitule_de_la_ligne_budgetaire
                        (const QString &intitule_de_la_ligne_budgetaire);


    static QString GET_LOCALISATION_FOR_EMPLOYEE_PAY_GROUP(const QString &an_employee_PAY_GROUP);


    static QString GET_LOCALISATION_FOR_CLIENT_LOYALTY_PROGRAM(const QString &client_loyalty_program);


    static QString getStrategySqlQueryStr(QString aSelectStmt,
                                          QString aStrategy);


    static inline void YERITH_PRINT_TO_STDOUT(const QString &aQStringOutput)
    {
        QTextStream(stdout) << aQStringOutput << endl;
    }

    static inline QString YERITH_REPLACE_ALL_EMPTYSPACES_WITH_UNDERSCORE_CHARACTER(const QString &aStr_in)
    {
        return QString(aStr_in).replace(YerithUtils::EMPTY_SPACE_REGEXP,
                                        YerithUtils::DASH_CHAR);
    }

    static inline QString getTvaString()
    {
        return QString::number((YerithERPConfig::tva_value * 100), 'f', 2);
    }

    static inline const QString getTvaStringWithPercent()
    {
        return QString("%1 %").arg(YerithUtils::getTvaString());
    }

    static inline const QString getTVALabelStringWithPercent()
    {
        return QObject::tr("TVA (%1)")
        			.arg(YerithUtils::getTvaStringWithPercent());
    }

    static void infosEntreprise(YerithPOSAdminWindowsCommons &aYerithPOSAdminQMainWindow,
                                const QString 				 &infos);

    static inline void infosEntreprise(YerithWindowsCommons &aYerithPOSQMainWindow,
                                       const QString 		&infos)
    {
        YerithQMessageBox::information(&aYerithPOSQMainWindow,
                                       QObject::tr("Informations sur l'entreprise"),
									   infos);
    }


    static double localized_string__get_double_FROM_LOCALIZED_STRING(QString DOUBLE_NUMBER_STRING);

    static QString localized_string__get_number_string(int INT_NUMBER);

    static QString localized_string__get_double_string(double DOUBLE_NUMBER);

    static QString localized_string__get_double_string_P(double DOUBLE_NUMBER,
    													 int 	NUMBER_OF_DECIMALS_after);

    static QString localized_string__get_CURRENCY_double_string(double DOUBLE_NUMBER);

    static QString localized_string__get_CURRENCY_double_string_P(double DOUBLE_NUMBER,
    															  int 	 NUMBER_OF_DECIMALS_after);

    static QString localized_string__get_MONTH_NAME_LOCALIZED(int month_name);


    static void getCurrentSimplifiedDate(QString 		&date_IN_OUT,
                                         const QDate 	&aDate);

    static inline void getCurrentSimplifiedDate(QString &date_IN_OUT)
    {
        YerithUtils::getCurrentSimplifiedDate(date_IN_OUT,
                                              QDate::currentDate());
    }

    static void getCurrentLocaleDate(QString 		&date_IN_OUT,
                                     const QDate 	&aDate);

    static inline void getCurrentLocaleDate(QString &date_IN_OUT)
    {
        YerithUtils::getCurrentLocaleDate(date_IN_OUT, QDate::currentDate());
    }

    static inline bool isEqualCaseSensitive(const QString &str1,
                                            const QString &str2)
    {
        return (0 == str1.compare(str2, Qt::CaseSensitive));
    }

    static inline bool isEqualCaseSensitive(const QString &str1,
    										const char 	*str2)
    {
        return (0 == str1.compare(QString(str2), Qt::CaseSensitive));
    }

    static inline bool isEqualCaseInsensitive(const QString &str1,
                                              const QString &str2)
    {
        return (0 == str1.compare(str2, Qt::CaseInsensitive));
    }

    static inline bool isEqualCaseInsensitive(const QString &str1,
                                              const char 	*str2)
    {
        return (0 == str1.compare(QString(str2), Qt::CaseInsensitive));
    }

    static inline void setMainWorkingDirectory(const QString &
                                               workingDirectory)
    {
        _mainWorkingDirectory = workingDirectory;
    }

    static inline QString getMainWorkingDirectory()
    {
        return _mainWorkingDirectory;
    }

    static inline void setLogFileName(const QString &logFileName)
    {
        _logFileName = logFileName;
    }

    static inline QString getLogFileName()
    {
        return _logFileName;
    }

    static QString GET_YERITH_QT_OBJECT_NAME(QObject *an_object);


    static bool checkIf_KEYWORD_ALREADY_EXISTS(YerithWindowsCommons &aCallingWindow,
                                               YerithSqlTableModel 	&aSqlTableModel,
											   const QString 	    &a_DB_TO_SEARCH_COLUMN_NAME,
                                               const QString 		&for_user_view_db_column_description,
                                               const QString 		&aDB_TABLE_COLUMN_NAME,
                                               int aCurrentClientDetailDBID = YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED);


    static inline bool checkIf_KEYWORD_ALREADY_EXISTS(YerithWindowsCommons  &aCallingWindow,
                                                      YerithSqlTableModel 	&aSqlTableModel,
                                                      QLineEdit 			&aYerithLineEdit_DB_TO_SEARCH_COLUMN_NAME,
                                                      const QString 		&for_user_view_db_column_description,
                                                      const QString 		&aDB_TABLE_COLUMN_NAME,
                                                      int aCurrentClientDetailDBID = YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED)
    {
        return
            checkIf_KEYWORD_ALREADY_EXISTS(aCallingWindow,
                                           aSqlTableModel,
                                           aYerithLineEdit_DB_TO_SEARCH_COLUMN_NAME.text(),
                                           for_user_view_db_column_description,
                                           aDB_TABLE_COLUMN_NAME,
                                           aCurrentClientDetailDBID);
    }


    static bool checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(YerithWindowsCommons &
                                                                 aCallingWindow,
                                                                 YerithSqlTableModel &
                                                                 aClientTableModel,
                                                                 YerithLineEdit &
                                                                 aYerithLineEdit_nom_entreprise,
                                                                 int
                                                                 aCurrentClientDetailDBID
                                                                 =
                                                                                 YerithUtils::
                                                                                 CURRENT_CLIENT_DB_ID_UNDEFINED);

    static bool
    checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE
    (YerithWindowsCommons &aCallingWindow,
     YerithSqlTableModel &aClientTableModel,
     YerithLineEdit &aYerithLineEdit_reference_registre_du_commerce,
     int aCurrentClientDetailDBID =
                     YerithUtils::CURRENT_CLIENT_DB_ID_UNDEFINED);

    static bool checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(YerithWindowsCommons &
                                                                   aCallingWindow,
                                                                   YerithSqlTableModel &
                                                                   aClientTableModel,
                                                                   YerithLineEdit &
                                                                   aYerithLineEdit_reference_client,
                                                                   int
                                                                   aCurrentClientDetailDBID
                                                                   =
                                                                                   YerithUtils::
                                                                                   CURRENT_CLIENT_DB_ID_UNDEFINED);

    static bool IS_SQL_TABLE_COLUMN_MAXIMUM_SIZE_REACHED(int
                                                         a_sql_table_column_CURRENT_history_row_data_size,
                                                         int
                                                         a_sql_table_column_NEXT_WRITE_row_data_size,
                                                         int
                                                         a_sql_table_column_MAXIMUM_SIZE);

    static inline bool is_montant_payer_AU_CLIENT__PAR_FIDELITE__valide(int
                                                                        typeDePaiement)
    {
        return (YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT ==
                typeDePaiement) ? true : false;
    }

    static inline bool is_montant_payer_AU_CLIENT_valide(int typeDePaiement)
    {
        return (YerithUtils::DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT ==
                typeDePaiement
                || YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT ==
                typeDePaiement) ? true : false;
    }

    static inline bool is_montant_payer_par_le_client_valide(int
                                                             typeDePaiement)
    {
        return (YerithUtils::ENCAISSEMENT_INDEFINI != typeDePaiement &&
                YerithUtils::DECAISSEMENT_COMPTANT != typeDePaiement &&
                YerithUtils::DECAISSEMENT_CHEQUE != typeDePaiement &&
                YerithUtils::DECAISSEMENT_TELEPHONE != typeDePaiement &&
                YerithUtils::DECAISSEMENT_BANCAIRE != typeDePaiement &&
                YerithUtils::DECAISSEMENT_VIREMENT_BANCAIRE != typeDePaiement &&
                YerithUtils::DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT !=
                typeDePaiement
                && YerithUtils::DECAISSEMENT_INDEFINI != typeDePaiement
                && YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT !=
                typeDePaiement) ? true : false;
    }

    static inline bool is_montant_payer_PAR_LE_FOURNISSEUR_valide(int
                                                                  typeDePaiement)
    {
        return (YerithUtils::ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE ==
                typeDePaiement) ? true : false;
    }

    static inline bool is_montant_payer_au_fournisseur_valide(int
                                                              typeDePaiement)
    {
        return (YerithUtils::DECAISSEMENT_INDEFINI != typeDePaiement                            &&
                YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT != typeDePaiement   &&
                YerithUtils::ENCAISSEMENT_AVANCE_DE_CASH != typeDePaiement                      &&
                YerithUtils::ENCAISSEMENT_COMPTANT != typeDePaiement                            &&
                YerithUtils::ENCAISSEMENT_CHEQUE != typeDePaiement                              &&
                YerithUtils::ENCAISSEMENT_TELEPHONE != typeDePaiement                           &&
                YerithUtils::ENCAISSEMENT_BANCAIRE != typeDePaiement                            &&
                YerithUtils::ENCAISSEMENT_VIREMENT_BANCAIRE != typeDePaiement                   &&
                YerithUtils::ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE != typeDePaiement            &&
                YerithUtils::ENCAISSEMENT_INDEFINI != typeDePaiement)
                ? true :
                  false;
    }

    static inline bool IS_QSQL_RECORD_AN_ASSET(QSqlRecord &a_qsql_record);

    static inline double montant_paye_AU_CLIENT(const QSqlRecord &
                                                aPaymentRecord);

    static inline double montant_paye_PAR_LE_FOURNISSEUR(const QSqlRecord &
                                                         aPaymentRecord);

    static inline double montant_paye_par_le_client(const QSqlRecord &
                                                    aPaymentRecord);

    static inline double montant_paye_au_fournisseur_OU_Employe(const QSqlRecord &
                                                     aPaymentRecord);

    static inline void yerithSetWidgetColor(QWidget *aWidget)
    {
        return;

        /*if (aWidget->isEnabled())
           {
           aWidget->setPalette(YerithUtils::YERITH_WHITE_PALETTE);
           }
           else
           {
           aWidget->setPalette(YerithUtils::YERITH_BLACK_PALETTE);
           } */
    }


    static QString YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY;


    static const QString UTILISATEUR_NON_EXISTANT;


    static const QString DATABASE_MYSQL_VARCHAR_TYPE_STRING;

    static const QString DATABASE_MYSQL_CHAR_TYPE_STRING;

    static const QString DATABASE_MYSQL_DATE_TYPE_STRING;

    static const QString DATABASE_MYSQL_DOUBLE_TYPE_STRING;

    static const QString DATABASE_MYSQL_INT_TYPE_STRING;

    static const QString DATABASE_MYSQL_TIME_TYPE_STRING;


    static const QString ETHERNET_LOCALHOST_STRING_NAME;

    static const QString ETHERNET_LOCALHOST_LOOPBACK_IP_ADDRESS;

    static const QString YERITH_QT_STYLE_CDE;

    static const unsigned int CURRENT_CLIENT_DB_ID_UNDEFINED;

    static const QString IMPRIMANTE_PDF;

    static const QString IMPRIMANTE_EPSON_TM_T20ii;

    static const QString IMPRIMANTE_EPSON_TM_T20ii_RESEAU;

    static const unsigned int PRINT_TIMEOUT;

    static const QString STRING_STYLESHEETS_YERITH_ERP_3_0;

    static const QString STRING_STYLESHEET_COLOR_YERITHGREEN;


    static const QColor YERITH_WHITE_COLOR_FOR_EMPTY_MERCHANDISE_FILTERING;

    static const QColor YERITH_WHITE_COLOR;

    static const QColor YERITH_BLACK_COLOR;

    static const QColor YERITH_BLUE_COLOR;

    static const QColor YERITH_DARK_GREEN_COLOR;

    static const QColor YERITH_GRAY_COLOR;

    static const QColor YERITH_GREEN_COLOR;

    static const QColor YERITH_YERITHGREEN_COLOR;

    static const QColor YERITH_INDIGO_COLOR;

    static const QColor YERITH_YELLOW_COLOR;

    static const QColor YERITH_ORANGE_COLOR;

    static const QColor YERITH_RED_COLOR;

    static const QBrush YERITH_QBRUSH_GREEN;

    static const QBrush YERITH_QBRUSH_YERITHGREEN;

    static const QBrush YERITH_QBRUSH_BLUE;

    static const QBrush YERITH_QBRUSH_ORANGE;

    static const QBrush YERITH_QBRUSH_YERITHGRAY;

    static const QPalette YERITH_BLACK_PALETTE;

    static const QPalette YERITH_BLUE_PALETTE;

    static const QPalette YERITH_DARK_GREEN_PALETTE;

    static const QPalette YERITH_GRAY_PALETTE;

    static const QPalette YERITH_LIGHT_GREEN_PALETTE;

    static const QPalette YERITH_GREEN_PALETTE;

    static const QPalette YERITH_ORANGE_PALETTE;

    static const QPalette YERITH_INDIGO_PALETTE;

    static const QPalette YERITH_RED_PALETTE;

    static const QPalette YERITH_WHITE_PALETTE;

    static const QString TVA_0_STRING;

    static const QString DATE_FORMAT;

    static const QString TIME_FORMAT_WITH_MILLISECONDS;

    static const QString TIME_FORMAT;

    static const QString DB_DATE_FORMAT;

    static const QString MAGELAN_CLIENT;

    static const QString MAGELAN_CLIENT_OBJECT;

    static const QString MAGELAN_SERVER;

    static const QString MAGELAN_SERVER_OBJECT;

    static const QString YERITH_ERP_3_0_LAST_BUILD_ID;

    static const QString APROPOS_MSG;

    static const QString MYSQL;

    static const QString MYSQL_DUMP;

    static const QString APPLICATION_NAME;

    static const QString STRING_OUI;

    static const QString STRING_NON;

    static const unsigned int FACTURE_GRAND_STRING_MAX_CHARS;

    static const unsigned int FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS;

    static const QString EGAL;

    static const QString INFERIEUR_OU_EGAL;

    static const QString SUPERIEUR;

    static const QString MYSQL_TRUE_LITERAL;

    static const QString MYSQL_FALSE_LITERAL;

    static const QString CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR;

    static const QString STRING_FRENCH_DIVERS;

    static const QString EMPTY_STRING;

    static const QChar DOT_CHAR;

    static const QChar STAR_CHAR;

    static const QChar DASH_CHAR;

    static const QChar PERCENT_CHAR;

    static const QChar SLASH_CHAR;

    static const int YERITH_PAGINATION_INT_VALIDATOR_MAXIMAL_VALUE;

    static const QIntValidator
    nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE_Validator;

    static const QIntValidator Int_BACKUP_YERITH_ERP_3_Validator;

    static const QRegExp STRING_FOR_YERITH_ERP_3_0_VALIDATOR_DB_COLUMN_NAME;

    static const QRegExpValidator
    STRING_FOR_YERITH_ERP_3_0_IP_ADDRESS_VALIDATOR;

    static const QRegExpValidator STRING_FOR_YERITH_ERP_3_0_VALIDATOR;

    static const QIntValidator UINT_VALIDATOR_GREATER_THAN_ZERO;

    static const QIntValidator UintValidator;

    static const QDoubleValidator DoubleValidator;

    static const QDoubleValidator POSITIVE_DoubleValidator;

    static const QRegExp PasswordRegExp;

    static const QRegExpValidator PasswordValidator;

    static const QRegExp LINE_ENDING_STRING_REGEXP;

    static const QRegExp EMPTY_SPACE_REGEXP;

    static const QLocale frenchLocale;

    static const QLocale englishLocale;

    static QString _1a_tex;

    static QString FR_template_bilan_comptable_tex;

    static QString EN_template_bilan_comptable_tex;

    static QString FR_template_CLIENT_LOYALTY_GROUP_LISTING_tex;

    static QString EN_template_CLIENT_LOYALTY_GROUP_LISTING_tex;

    static QString FR_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex;

    static QString EN_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex;

    static QString FR_template_carte_de_fidelite_client_TEMPLATE_tex;

    static QString EN_template_carte_de_fidelite_client_TEMPLATE_tex;

    static QString FR_template_Comptabilite_comptes_Doperations_TEX;

    static QString EN_template_Comptabilite_comptes_Doperations_TEX;

    static QString FR_template_bon_de_commande_TEX;

    static QString EN_template_bon_de_commande_TEX;

    static QString FR_template_lister_admin_objects_TEX;

    static QString EN_template_lister_admin_objects_TEX;

    static QString FR_template_EMPLOYEE_GROUP_LISTING_tex;

    static QString EN_template_EMPLOYEE_GROUP_LISTING_tex;

    static QString FR_template_employee_PAY_GROUP_LISTING_tex;

    static QString EN_template_employee_PAY_GROUP_LISTING_tex;

    static QString FR_template_fiche_dun_fournisseur_tex;

    static QString EN_template_fiche_dun_fournisseur_tex;

    static QString FR_template_fiche_client_tex;

    static QString EN_template_fiche_client_tex;

    static QString FR_template_historique_dun_stock_tex;

    static QString EN_template_historique_dun_stock_tex;

    static QString FR_template_journal_des_paiements_tex;

    static QString EN_template_journal_des_paiements_tex;

    static QString FR_template_journal_des_ventes_tex;

    static QString EN_template_journal_des_ventes_tex;

    static QString FR_template_liste_des_mouvements_de_stocks_tex;

    static QString EN_template_liste_des_mouvements_de_stocks_tex;

    static QString FR_template_sortie_des_stocks_grand_tex;

    static QString EN_template_sortie_des_stocks_grand_tex;

    static QString FR_template_lister_stock_tex;

    static QString EN_template_lister_stock_tex;

    static QString FR_template_sortie_des_stocks_petit_tex;

    static QString EN_template_sortie_des_stocks_petit_tex;

    static QString FR_template_transactions_dun_fournisseur_tex;

    static QString EN_template_transactions_dun_fournisseur_tex;

    static QString FR_template_transactions_dun_client_tex;

    static QString EN_template_transactions_dun_client_tex;

    static QString FR_template_fournisseurs_tex;

    static QString EN_template_fournisseurs_tex;

    static QString FR_template_comptes_clients_tex;

    static QString EN_template_comptes_clients_tex;

    static QString FR_template_marchandises_tex;

    static QString EN_template_marchandises_tex;

    static QString FR_template_lister_alertes_tex;

    static QString EN_template_lister_alertes_tex;

    static QString FR_template_CHARGES_FINANCIERES_tex;

    static QString EN_template_CHARGES_FINANCIERES_tex;

    static QString FR_template_lister_achats_tex;

    static QString EN_template_lister_achats_tex;

    static QString FR_template_comptabilite_tex;

    static QString EN_template_comptabilite_tex;

    static QString FR_template_yerith_erp_3D_BARDIAG_TEMPLATE_tex;

    static QString EN_template_yerith_erp_3D_BARDIAG_TEMPLATE_tex;

    static QString FR_template_THREE_D_bardiag_tex;

    static QString EN_template_THREE_D_bardiag_tex;

    static QString FR_template_GROUPES_DUN_CLIENT_tex;

    static QString EN_template_GROUPES_DUN_CLIENT_tex;

    static QString FR_template_GROUPES_DUN_EMPLOYE_tex;

    static QString EN_template_GROUPES_DUN_EMPLOYE_tex;

    static QString FR_template_FACTURE_GRAND_tex;

    static QString EN_template_FACTURE_GRAND_tex;

    static QString FR_template_RECU_GRAND_tex;

    static QString EN_template_RECU_GRAND_tex;

    static QString FR_template_facture_petit_tex;

    static QString EN_template_facture_petit_tex;

    static QString EN_ZERO_ventes_tex;

    static QString FR_ZERO_ventes_tex;

    static QString FR_pie_chart_tex;

    static QString FR_bar_chart_tex;

    static QString FR_bar_diag_tex;

    static QString EN_pie_chart_tex;

    static QString EN_bar_chart_tex;

    static QString EN_bar_diag_tex;

    enum PRINT_SHAPE
    {
        _VERTICAL = 0,
        _HORIZONTAL
    };


    enum ALERTE_RESOLUE_OUI_NON_INDEX
    {
        ALERTE_RESOLUE_NON_INDEX = 0,
        ALERTE_RESOLUE_OUI_INDEX = 1
    };


    enum STRATEGIE_COMBOBOX_INDEX
    {
        STRATEGIE_ALL_COMBOBOX_INDEX = 0,
        STRATEGIE_FEFO_COMBOBOX_INDEX = 1,
        STRATEGIE_FIFO_COMBOBOX_INDEX = 2,
        STRATEGIE_LIFO_COMBOBOX_INDEX = 3
    };


    enum TITRE
    {
        TITRE_DR = 100,
        TITRE_ME = 120,
        TITRE_MLLE = 140,
        TITRE_MME = 160,
        TITRE_MR = 180,
        TITRE_PROF = 200,
        TITRE_INDEFINI = 220
    };


    enum USER_ROLE
    {
        ROLE_ADMINISTRATEUR = 25,
        ROLE_MANAGER = 35,
        ROLE_GESTIONNAIREDESTOCK = 45,
        ROLE_MAGASINIER = 55,
        ROLE_CAISSIER = 65,
        ROLE_VENDEUR = 75,
        ROLE_INDEFINI = 85
    };


    enum TYPE_DE_CREDIT_DEBIT
    {
        CREDIT_ENTREE_DARGENT = 750,
        DEBIT_SORTIE_DARGENT = 770,
        CREDIT_DEBIT_INDEFINI = 790
    };


    enum TYPE_DOPERATIONS_FINANCIERES
    {
        OPERATION_FINANCIERE_VENTE = 1002,
        OPERATION_FINANCIERE_CHARGE_FINANCIERE = 1014,
        OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES = 1026,
        OPERATION_FINANCIERE_ACHAT_DE_MATERIEL_IMMOBILISATIONS = 1038,
        OPERATION_FINANCIERE_PAYEMENT_RESSOURCES_HUMAINES = 1042,
        OPERATION_FINANCIERE_INDEFINI = 1050
    };


    enum TYPEDEVENTE
    {
        VENTE_COMPTANT = 350,
        VENTE_COMPTECLIENT_AVANCE_DE_CASH = 360,
        VENTE_COMPTE_CLIENT = 370,
        VENTE_INDEFINI = 380
    };


    enum STATUT_DE_LACHAT_DE_SERVICE
    {
        ACHAT_DEJA_COMMANDE = 850,
        ACHAT_DEJA_RECEPTIONE = 851,
        ACHAT_EN_COURS_DE_PROGRAMMATION = 852
    };


    enum TYPEDENCAISSEMENT
    {
        ENCAISSEMENT_AVANCE_DE_CASH = 10,
        ENCAISSEMENT_COMPTANT = 20,
        ENCAISSEMENT_CHEQUE = 30,
        ENCAISSEMENT_TELEPHONE = 40,
        ENCAISSEMENT_BANCAIRE = 50,
        ENCAISSEMENT_VIREMENT_BANCAIRE = 60,
        ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE = 70,
        ENCAISSEMENT_INDEFINI = 80
    };


    enum TYPEDEDECAISSEMENT
    {
        DECAISSEMENT_COMPTANT = 603,
        DECAISSEMENT_CHEQUE = 604,
        DECAISSEMENT_TELEPHONE = 605,
        DECAISSEMENT_BANCAIRE = 606,
        DECAISSEMENT_VIREMENT_BANCAIRE = 607,
        DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT = 608,
        DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT = 609,
        DECAISSEMENT_INDEFINI = 610,
    };


    static QMap<int, QString> _alerte_OUI_NON_ToUserViewString;

    static QMap<int, QString> _strategieindexToUserViewString;

    static QMap<int, QString> _titreToUserViewString;

    static QMap<int, QString> _roleToUserViewString;

    static QMap<int, QString> _typedecreditdebitToUserViewString;

    static QMap<int, QString> _typedoperationfinancieresToUserViewString;

    static QMap<int, QString> _typedeventeToUserViewString;

    static QMap<int, QString> _typedepaiementToUserViewString;


    static const QString PREFIX_RESTE_FACTURE_CLIENT;

    static const QString PREFIX_RECU_PAIEMENT_CLIENT;

    static const QString PREFIX_RECU_VENDU;

    static const QString PREFIX_RECU_SORTIE;


    static const QKeySequence INCREASE_QTABLE_VIEW_WIDGET_SIZE;

    static const QKeySequence DECREASE_QTABLE_VIEW_WIDGET_SIZE;


    static const QKeySequence MESSAGE_DAIDE_QKEYSEQUENCE;

    static const QKeySequence EXPORTER_AU_FORMAT_CSV_QKEYSEQUENCE;

    static const QKeySequence PARAMETRER_IMPRESSION_QKEYSEQUENCE;

    static const QKeySequence IMPRIMER_QKEYSEQUENCE;

    static const QKeySequence
    AFFICHER_LES_MARCHANDISES_NON_TERMINES_QKEYSEQUENCE;

    static const QKeySequence AFFICHER_LES_MARCHANDISES_TERMINES_QKEYSEQUENCE;

    static const QKeySequence RECHERCHER_QKEYSEQUENCE;

    static const QKeySequence REINITIALISER_RECHERCHE_QKEYSEQUENCE;

    static const QKeySequence REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE;

    static const QKeySequence LISTER_STOCKS_QKEYSEQUENCE;

public slots:

    static bool slot_connecter_localisation(QWidget &aWidget,
                                            YerithERPWindows *allWindows,
                                            const QString &localisation);

    static bool slot_deconnecter_localisation(YerithERPWindows *allWindows);

private:

    static YerithERPWindows *_allWindows;
    static QString _mainWorkingDirectory;
    static QString _logFileName;
};


#define YR_SAVE_USER_SETTINGS_QTABLEWIDGET(A_QTABLE_WIDGET)							\
{							  					 									\
	if (0 != allWindows) 		  					 								\
	{							  					 								\
		YerithPOSUser *aUser = allWindows->getUser();								\
    											 	 	 	 						\
		if (0 != aUser)								 								\
		{																			\
			aUser->save_user_personal_PRINTING_PARAMETER_settings(A_QTABLE_WIDGET); \
		}																			\
	}																				\
}


#define YR_SAVE_USER_SETTINGS 					 					\
{							  					 					\
	if (0 != allWindows) 		  					 				\
	{							  					 				\
		YerithPOSUser *aUser = allWindows->getUser();				\
    											 	 	 	 		\
		if (0 != aUser)								 				\
		{															\
			aUser->save_user_personal_PRINTING_PARAMETER_settings();\
		}															\
	}																\
}


#define YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER \
{																		 \
YerithRechercheConditionnelleWindow	*recherche_conditionnelle_window =	 \
		_allWindows->_recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW;	 \
																		 \
if (0 != recherche_conditionnelle_window && 							 \
	recherche_conditionnelle_window->isVisible()) 						 \
{ 																		 \
	recherche_conditionnelle_window->reset_filter(); 					 \
}																		 \
}


#define YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER \
{																		\
YerithRechercheConditionnelleWindow	*recherche_conditionnelle_window = 	\
		_allWindows->_recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW;	\
																		\
if (0 != recherche_conditionnelle_window &&								\
	recherche_conditionnelle_window->isVisible())						\
{																		\
	bool add_AND = false;												\
	if (!recherche_conditionnelle_window->lineEdit_nom_element_string_db_1->isEmpty()) \
	{																	\
		if (!_searchFilter.isEmpty())									\
		{																\
			_searchFilter.append(" AND ");								\
		}																\
		_searchFilter.append(QString(" (%1 LIKE '%%2%') ")				\
.arg(YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(recherche_conditionnelle_window->comboBox_element_string_db_1->currentText()),\
						 recherche_conditionnelle_window->lineEdit_nom_element_string_db_1->text()));								\
		setCurrentlyFiltered(true);										\
		add_AND = true;													\
	}																	\
	if (!recherche_conditionnelle_window->lineEdit_nom_element_string_db_2->isEmpty()) \
	{																	\
		if (add_AND)													\
		{																\
			_searchFilter.append(" AND ");								\
		}																\
		_searchFilter.append(QString(" (%1 LIKE '%%2%') ")				\
.arg(YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(recherche_conditionnelle_window->comboBox_element_string_db_2->currentText()), \
						recherche_conditionnelle_window->lineEdit_nom_element_string_db_2->text()));								\
		setCurrentlyFiltered(true);										\
		add_AND = true;													\
	}																	\
	if (!recherche_conditionnelle_window->lineEdit_nom_element_string_db_3->isEmpty())\
	{																	\
		if (add_AND)													\
		{																\
			_searchFilter.append(" AND ");								\
		}																\
		_searchFilter.append(QString(" (%1 LIKE '%%2%') ")				\
.arg(YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(recherche_conditionnelle_window->comboBox_element_string_db_3->currentText()),\
						recherche_conditionnelle_window->lineEdit_nom_element_string_db_3->text()));								\
		setCurrentlyFiltered(true);										\
		add_AND = true;													\
	}																	\
	if (!recherche_conditionnelle_window->lineEdit_nom_element_string_db_4->isEmpty())\
	{																	\
		if (add_AND)													\
		{																\
			_searchFilter.append(" AND ");								\
		}																\
		_searchFilter.append(QString(" (%1 LIKE '%%2%') ")				\
.arg(YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(recherche_conditionnelle_window->comboBox_element_string_db_4->currentText()), \
				recherche_conditionnelle_window->lineEdit_nom_element_string_db_4->text()));										\
		setCurrentlyFiltered(true);										\
		add_AND = false;												\
	}																	\
}																		\
}


/**
 * Macros to facilitate the use of YerithUtils static methods
 */

#define YERITH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(X, Y) 						\
	static bool first_execution = true;				  							\
	if (!first_execution)							  							\
	{												  							\
		if (0 != X)						  										\
		{											  							\
			if (0 != Y)															\
			{																	\
				Y->setLast_YERITH_TABLE_VIEW_SelectedRow__db_ID(currentIndex());\
			}											  						\
		}																		\
	}																			\
	else 																		\
	{ 																			\
		if (0 != X) 															\
		{ 																		\
			if (0 != Y)															\
			{																	\
				Y->setLast_YERITH_TABLE_VIEW_SelectedRow__db_ID(); 				\
			}																	\
		} 																		\
		first_execution = false; 												\
	}


#define YERITH_set_windowName_TRANSLATED(windowName, sql_table_model) 	\
{ 											   							\
    if (0 != sql_table_model)				   							\
    {																	\
    	_windowName_WITH_NO_MAINTENANCE = windowName;					\
    																	\
    	if (0 != sql_table_model 		   && 							\
			YerithERPConfig::SHOW_DATABASE_RAW)							\
    	{																\
    		QString titre_ADDENDUM = QString(" _ %1").arg(sql_table_model->sqlTableName()); \
    																	\
			if (!windowName.contains(titre_ADDENDUM))					\
			{															\
				_windowName = windowName.append(titre_ADDENDUM); 		\
			}															\
    	}																\
    	else 															\
    	{																\
    		_windowName = _windowName_WITH_NO_MAINTENANCE;				\
		}																\
																		\
		setWindowTitle(_windowName); 									\
    }																	\
}



#define YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS \
{ 										\
	static bool FIRST_EXECUTION = true; \
										\
	if (FIRST_EXECUTION)				\
	{									\
		initialiser_FILTRES();			\
										\
		FIRST_EXECUTION = false;		\
	}									\
}



#define YERITH_DELETE_FREE_POINTER_NOW(P) \
{ \
	if (0 != P) \
	{ \
		delete P; \
	} \
}


#define YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(THIS, NOMBRE, DIALOG_BOX_TITLE) \
{ QString msg(QObject::tr("Il y'a eu")); \
msg.append(QObject::tr(" %1 résultat(s) à votre filtre !").arg(NOMBRE)); \
YerithQMessageBox::information(this, QObject::tr(DIALOG_BOX_TITLE), msg); }


#define YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(THIS, DIALOG_BOX_TITLE) \
		{ QString msg(QObject::tr("Il ny'a eu aucun résultat à votre filtre !")); \
		YerithQMessageBox::information(THIS, QObject::tr(DIALOG_BOX_TITLE), msg); }



#define YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(X) \
		{ \
    		_yerith_WINDOW_QComboBox_SearchDBFieldColumnString = comboBox_element_string_db; \
    		_yerith_WINDOW_QLineEdit_SearchDBFieldColumnString = lineEdit_nom_element_string_db; \
			_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH = X; \
			YerithWindowsCommons::_yerith_references_dbColumnString = _yerith_WINDOW_references_dbColumnString; \
			YerithWindowsCommons::_yerith_QComboBox_SearchDBFieldColumnString = _yerith_WINDOW_QComboBox_SearchDBFieldColumnString; \
			YerithWindowsCommons::_yerith_QLineEdit_SearchDBFieldColumnString = _yerith_WINDOW_QLineEdit_SearchDBFieldColumnString; \
		}


#define YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(X) YerithDatabaseTableColumn::_tableColumnToUserViewString.key(X)

#define YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(X) YerithDatabaseTableColumn::_tableColumnToUserViewString.value(X)

#define YERITH_DATABASE_TABLE_COLUMN_INDEX(W, C) W.getDBTableFieldColumnIndex(C)


#define YERITH_SAVE_ID_TO_ROW_NUMBER_FOR_YERITH_TABLE_VIEW(TS, DB_ID, ROW) \
	TS.clear();									 	 			 		   \
	TS.append(QString::number(DB_ID));								 	   \
	_map_dbID_TO_yerith_table_view_ROW_NUMBER.insert(TS, ROW);


#define YERITH_QSTRING_CONTAINS(S, V) S.contains(V, Qt::CaseInsensitive)

#define QDEBUG_STRINGS_OUTPUT_Q_CONTAINER(X, Y) qDebug() << QString("++ %1: ").arg(X) << Y

#define Q_MANUAL_DEBUGGING_STRINGS_OUTPUT__RESET_TOKEN(X) YerithUtils::qDebugOutputTokenStrings_DURING_MANUAL_DEBUGGING(X, true)

#define Q_MANUAL_DEBUGGING_STRINGS_OUTPUT_TOKEN(X) YerithUtils::qDebugOutputTokenStrings_DURING_MANUAL_DEBUGGING(X)

#define QDEBUG_STRING_OUTPUT_1(X) YerithUtils::qDebugStrings(X, YerithUtils::EMPTY_STRING)

#define QDEBUG_STRING_OUTPUT_2_N(X, Y) YerithUtils::qDebugStrings(X, QString::number(Y))

#define QDEBUG_STRING_OUTPUT_2(X, Y) YerithUtils::qDebugStrings(X, Y)

#define QDEBUG_QSTRINGLIST_OUTPUT(X, Y) YerithUtils::qDebugStrings(X, Y)

#define EXPR_IS_POSITIV(X)	(0 <= X)

#define YERITH_QT_OBJECT_NAME(X) YerithUtils::GET_YERITH_QT_OBJECT_NAME(X)

#define BOOLEAN_STRING_TRUE "True"

#define BOOLEAN_STRING_FALSE "False"

#define INT_TO_STRING(B) ((0 != B) ? "OUI" : "NON")

#define BOOL_TO_STRING(B) (B ? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE)

//Get the value of an QSqlRecord when given as value R
#define GET_SQL_RECORD_DATA(R,V) YerithUtils::get_text(R.value(V))

//Get the MD5 hash of value X
#define MD5_HASH(X) YerithUtils::md5Hash(X)

#define GENERATE_SQL_LIKE_STMT_AS_IS(COLUMN, SEARCH_STR) \
	YerithUtils::generateSqlLike__AS_IS(COLUMN, SEARCH_STR)

#define GENERATE_SQL_LIKE_STMT(COLUMN, SEARCH_STR) \
	YerithUtils::generateSqlLike(COLUMN, SEARCH_STR)

#define GENERATE_SQL_IS_STMT(COLUMN, SEARCH_STR) \
	YerithUtils::generateSqlIs(COLUMN, SEARCH_STR)

#define GENERATE_SQL_IS_NOT_EMPTY(COLUMN) \
	YerithUtils::generateSqlIsNotEmpty(COLUMN)

#define GENERATE_SQL_IS_EMPTY(COLUMN) \
	YerithUtils::generateSqlIsEmpty(COLUMN)


#define YERITH_ERP_3_0_START_DATABASE_TRANSACTION YerithUtils::start_db_transaction()

#define YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION YerithUtils::rollback_db_transaction()

#define YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION YerithUtils::commit_db_transaction()

#define POURCENTAGE_YERITH_GET_VALUE(X, TOTAL) ( (0 == TOTAL) ? 0 : ((X / TOTAL) * 100.0) )

#define GET_CURRENT_DATE QDate::currentDate()

#define GET_CURRENT_DATE_YEAR QString::number((QDate::currentDate().year() != 0) ? QDate::currentDate().year() : 2020)

#define GET_CURRENT_STRING_DATE QDate::currentDate().toString(YerithUtils::DATE_FORMAT)

#define CURRENT_TIME_WITH_MILLISECONDS QTime::currentTime().toString(YerithUtils::TIME_FORMAT_WITH_MILLISECONDS)

#define CURRENT_TIME QTime::currentTime().toString(YerithUtils::TIME_FORMAT)

#define FILE_NAME_USERID_CURRENT_TIME(F) YerithUtils::getFileNameWithUserIDAndCurrentTime(F)

#define DATE_TO_STRING(D) D.toString(YerithUtils::DATE_FORMAT)

#define GET_DATE_FROM_STRING(D) QDate::fromString(D, YerithUtils::DATE_FORMAT)

#define TIME_TO_STRING(D) D.toString(YerithUtils::TIME_FORMAT)

#define DATE_TO_DB_FORMAT_STRING(D) D.toString(YerithUtils::DB_DATE_FORMAT)

#define SET_DATE_TO_STRING(D) D.toString(YerithUtils::DATE_FORMAT)

#define GET_SPELLED_NUMBER_FOR_THREED_BAR_DIAGRAM(N) YerithUtils_SPELL_NUMBERS::YERITH__spell_numbers___ENGLISH_ONLY(N)

#define GET_SPELLED_NUMBER(N) YerithUtils_SPELL_NUMBERS::YERITH__spell_numbers(N)


/*
 * LOCALIZATION DEFINE METHODS
 */
#define DOUBLE_FROM_LOCALIZED_STRING(NUM) YerithUtils::localized_string__get_double_FROM_LOCALIZED_STRING(NUM)

#define GET_NUM_STRING(NUM) YerithUtils::localized_string__get_number_string(NUM)

#define GET_DOUBLE_STRING(NUM) YerithUtils::localized_string__get_double_string(NUM)

#define GET_DOUBLE_STRING_P(NUM, P) YerithUtils::localized_string__get_double_string_P(NUM, P)

#define GET_CURRENCY_STRING_NUM(NUM) YerithUtils::localized_string__get_CURRENCY_double_string(NUM)

#define GET_CURRENCY_STRING_NUM_P(NUM, P) YerithUtils::localized_string__get_CURRENCY_double_string_P(NUM, P)

#define GET_MONTH_NAME_LOCALIZED(M) YerithUtils::localized_string__get_MONTH_NAME_LOCALIZED(M)


#define GET_CURRENCY_STRING_NUM_FOR_LATEX(NUM) YerithUtils::LATEX_IN_OUT_handleForeignAccents(GET_CURRENCY_STRING_NUM(NUM))



inline bool YerithUtils::IS_QSQL_RECORD_AN_ASSET(QSqlRecord &a_qsql_record)
{
    return (0 == GET_SQL_RECORD_DATA(a_qsql_record,
                                     YerithDatabaseTableColumn::PRIX_VENTE).
            toDouble());
}


inline double YerithUtils::montant_paye_AU_CLIENT(const QSqlRecord &aPaymentRecord)
{
    return (YerithUtils::is_montant_payer_AU_CLIENT_valide(GET_SQL_RECORD_DATA
                                                           (aPaymentRecord,
                                                            YerithDatabaseTableColumn::
                                                            TYPE_DE_PAIEMENT).
                                                           toInt()))?
           GET_SQL_RECORD_DATA(aPaymentRecord,
                               YerithDatabaseTableColumn::MONTANT_PAYE).
           toDouble() : 0.0;
}


inline double YerithUtils::montant_paye_par_le_client(const QSqlRecord &aPaymentRecord)
{
    return (YerithUtils::is_montant_payer_par_le_client_valide
            (GET_SQL_RECORD_DATA
             (aPaymentRecord,
              YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt()))?
           GET_SQL_RECORD_DATA(aPaymentRecord,
                               YerithDatabaseTableColumn::MONTANT_PAYE).
           toDouble() : 0.0;
}


inline double YerithUtils::montant_paye_PAR_LE_FOURNISSEUR(const QSqlRecord &aPaymentRecord)
{
    return (YerithUtils::is_montant_payer_PAR_LE_FOURNISSEUR_valide
            (GET_SQL_RECORD_DATA
             (aPaymentRecord,
              YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt()))?
           GET_SQL_RECORD_DATA(aPaymentRecord,
                               YerithDatabaseTableColumn::MONTANT_PAYE).
           toDouble() : 0.0;
}


inline double YerithUtils::montant_paye_au_fournisseur_OU_Employe(const QSqlRecord &aPaymentRecord)
{
    return (YerithUtils::is_montant_payer_au_fournisseur_valide
            (GET_SQL_RECORD_DATA
             (aPaymentRecord,
              YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt()))?
           GET_SQL_RECORD_DATA(aPaymentRecord,
                               YerithDatabaseTableColumn::MONTANT_PAYE).
           toDouble() : 0.0;
}


template<class classType, typename parameterType>
void YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION(classType *aClassInstance,
                                                           parameterType *aParamValue,
                                                           void (classType::*func)(parameterType *))
{
    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    if (0 == func || 0 == aClassInstance)
    {
        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

        return;
    }

    (aClassInstance->*func)(aParamValue);

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


template<class classType, typename parameterType, typename returnType>
void YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION(classType *aClassInstance,
                                                           parameterType *aParamValue,
                                                           returnType(classType::*func)(parameterType *),
                                                           returnType *aRetValue /* = 0 */)
{
    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    if (0 == func || 0 == aClassInstance)
    {
        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

        return;
    }

    returnType YERITH_retValue = (aClassInstance->*func)(aParamValue);

    if (0 != aRetValue)
    {
        *aRetValue = YERITH_retValue;
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


template<class classType>
void YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK(classType *aClassInstance,
                                                                         bool (classType::*func)(void))
{
    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;

    if (0 == func || 0 == aClassInstance)
    {
        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

        return;
    }

    bool YERITH_retValue = (aClassInstance->*func)();

    if (!YERITH_retValue)
    {
        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;
    }

    YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


template<class aTableViewClassType>
bool YerithUtils::instanceOf__YerithTableViewWITHpagination(aTableViewClassType *aTableView)
{
    bool result = false;

    YerithTableViewWITHpagination *aType = dynamic_cast<YerithTableViewWITHpagination *>(aTableView);

    if (0 != aType)
    {
        result = true;
    }

    return result;
}


#endif /* SRC_YERITH_UTILS_HPP_ */
