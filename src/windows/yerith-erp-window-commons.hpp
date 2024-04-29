/*
 * yerith-erp-windows-commons.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_WINDOWS_COMMONS_HPP_
#define SRC_YERITH_ERP_WINDOWS_COMMONS_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/widgets/yerith-erp-progress-bar.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include "src/utils/windows/yerith-erp-abstract-class-yerithsearch-window.hpp"


#include <QtCore/QSemaphore>

#include <QtCore/QSet>

#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>


class YerithPushButton;
class YerithTableViewPRINT_UTILITIES_TEX_TABLE;
class YerithSelectDBQCheckBox;
class YerithERPGenericSelectDBFieldDialog;
class YerithERPWindows;


class YerithWindowsCommons : public QMainWindow
{
	Q_OBJECT

protected:

    enum entrer_VISIBLE_CHOICE
    {
        set_choice_sell_TO_CLIENT = 0,
        set_choice_insert_FINANCIAL_EXPENSE = 1,
        set_choice_insert_ASSET = 2,
        set_choice_insert_STANDARD_INVENTORY_STOCK = 3
    };

public:

    YERITH_CLASS_OPERATORS

    inline YerithWindowsCommons(const QString &anOutput_print_pdf_latexFileNamePrefix = "")
        :_CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER(false),
         _CURRENT_TABLE_VIEW_font(0),
         _WRITE_READ_YERITH_SEMAPHORE_APPLY_USER_SETTING_FILE_PROPERTIES(0),
         _selectExportDBQDialog(0),
         _is_pdf_printing_initialized(false),
         _yerith_PRINT_UTILITIES_TEX_TABLE(0),
         _output_print_pdf_latexFileNamePrefix(anOutput_print_pdf_latexFileNamePrefix),
         _yerith_QComboBox_SearchDBFieldColumnString(0),
         _yerith_QLineEdit_SearchDBFieldColumnString(0),
         _curStocksTableModel(0),
         _pagination_nombre_de_ligne_IntValidator(0),
         _yerithTableView_FROM_WINDOWS_COMMONS(0),
         _TABLE_FONT_SIZE(5),
         _page_from(-1),
         _page_to(-1),
         _USERSQL_table_row_count(59),
         _print_table_row_count(20),
         _QLINEEDIT_nombre_de_lignes_par_page(0),
         _table_COLUMN_ORDER(""),
         _a4paper_printing_position(QString("a4paper,landscape")),
         _first_time_imprimer_pdf_document_call(true),
         QMESSAGE_BOX_STYLE_SHEET(QString("QMessageBox {background-color: rgb(%1);}")
                                  .arg(COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0)),
         _windowName_WITH_NO_MAINTENANCE(YerithUtils::EMPTY_STRING),
         _windowName(YerithUtils::EMPTY_STRING)
    {
        _WRITE_READ_YERITH_SEMAPHORE_APPLY_USER_SETTING_FILE_PROPERTIES =
        new QSemaphore(1);
    }

    virtual ~YerithWindowsCommons();

    void yerith_hide_columns();

    void PRINT_OUTPUT_PRINTING_DEBUG_PARAMETERS();

    int get_INT_last_selected_row_number();


    bool RECONDUCT_current_TableColumnUser__Viewing_Strings();


    bool SAVE_current_TableColumnUser__Viewing_Strings();


    virtual void YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(QAction *anAction,
                                                        bool aBooleanValue);

    void setYerithTableView_FROM_WINDOWS_COMMONS(const QList <YerithTableView *> &aYerithTableView_FROM_WINDOWS_COMMONS_QLIST);

    void setYerithTableView_FROM_WINDOWS_COMMONS(YerithTableView *aYerithTableView_FROM_WINDOWS_COMMONS,
                                                 bool a_setLast_YERITH_TABLE_VIEW_SelectedRow__db_ID = true);

    inline QSet<int>  &getDBFieldNamesToPrintCenterAligned()
    {
        return _DBFieldNamesToPrintCenterAligned;
    }

    inline QSet<int>  &getDBFieldNamesToPrintRightAligned()
    {
        return _DBFieldNamesToPrintRightAligned;
    }

    inline QSet<int>  &getDBFieldNamesToPrintLeftAligned()
    {
        return _DBFieldNamesToPrintLeftAligned;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return 0;
    }

    inline QSet<QString> &get_yerith_references_dbColumnString()
    {
        return _yerith_references_dbColumnString;
    }

    inline QString get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH()
    {
        return _dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH;
    }

    inline QString getWindowName()
    {
        return _windowName;
    }

    virtual inline QString qMessageBoxStyleSheet()
    {
        return QMESSAGE_BOX_STYLE_SHEET;
    }

    inline static QPoint *getCenterPosition()
    {
        return _centerPosition;
    }

    inline static void setCenterPosition(QPoint *desktopCenterPosition)
    {
        _centerPosition = desktopCenterPosition;
    }

    inline static void setAllWindows(YerithERPWindows *allWindows)
    {
        _allWindows = allWindows;
    }

    inline YerithSqlTableModel *getCurStocksTableModel()
    {
        return _curStocksTableModel;
    }

    virtual inline void definirCaissier()
    {
    }

    virtual inline void definirManager()
    {
    }

    virtual inline void definirGestionaireDesStocks()
    {
    }

    virtual inline void definirMagasinier()
    {
    }

    virtual inline void definirPasDeRole()
    {
    }

    inline int getDBTableFieldColumnIndex(const QString &aDBTableColumnName)
    {
        return _dbtablecolumnNameToDBColumnIndex.value(aDBTableColumnName);
    }

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

    virtual void rendreInvisible();

    virtual void fill_table_columns_to_ignore(QList<int> &tableColumnsToIgnore_in_out);

    inline static YerithERPWindows *getAllWindows()
    {
        return _allWindows;
    }


    inline void set_visible_DB_column_name_str_list(QStringList &a_visible_column_str_list)
    {
        _visibleDBColumnNameStrList = a_visible_column_str_list;
    }

    inline const QStringList *get_visible_DB_column_name_str_list() const
    {
        return &_visibleDBColumnNameStrList;
    }


    bool SQL_UPDATE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(QSqlRecord &resultRecord_IN);


    bool SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(QSqlRecord &resultRecord_IN_OUT);


    bool SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW();


    inline static const QString &get_last_lister_selected_row_db_ID()
    {
        return _yerithTableView_FROM_WINDOWS_COMMONS_LAST_SELECTED_ROW__db_ID;
    }

    inline const QString &getLastListerSelectedRow__ID()
    {
        return (0 ==
                _yerithTableView_FROM_WINDOWS_COMMONS) ? YerithUtils::EMPTY_STRING
               : _yerithTableView_FROM_WINDOWS_COMMONS->lastSelectedRow__ID();
    }

    inline int getLastListerSelectedRow__ID_AS_INTEGER()
    {
        return (getLastListerSelectedRow__ID().isEmpty())? -1 :
               getLastListerSelectedRow__ID().toInt();
    }

    inline YerithLineEdit *get_QLINEEDIT_nombre_de_lignes_par_page()
    {
        return _QLINEEDIT_nombre_de_lignes_par_page;
    }

    inline YerithTableView *GET_YERITH_TABLE_VIEW()
    {
        return _yerithTableView_FROM_WINDOWS_COMMONS;
    }

    inline int get_PARAMETER_TABLE_FONT_SIZE()
    {
        return _TABLE_FONT_SIZE;
    }

    inline void set_PARAMETER_TABLE_FONT_SIZE(int TABLE_FONT_SIZE)
    {
    	_TABLE_FONT_SIZE = TABLE_FONT_SIZE;
    }

    inline void set_PARAMETER_TABLE_COLUMN_ORDER(const QString &a_PRINTING_PARAMETER_table_column_order)
    {
        _table_COLUMN_ORDER = a_PRINTING_PARAMETER_table_column_order;
    }

    void set_PARAMETER_TABLE_COLUMN_ORDER();

    inline QString get_PARAMETER_TABLE_COLUMN_ORDER()
    {
        return _table_COLUMN_ORDER;
    }

    inline void set_PRINTING_PARAMETER_printing_position(const QString &an_a4_printing_position)
    {
        _a4paper_printing_position = an_a4_printing_position;
    }

    inline QString get_PRINTING_PARAMETER_printing_position()
    {
        return _a4paper_printing_position;
    }

    inline uint get_PRINTING_PARAMETER_USERSQL_table_row_count()
    {
        return _USERSQL_table_row_count;
    }

    inline void set_PRINTING_PARAMETER_USERSQL_table_row_count(uint a_USERSQLt_table_row_count)
    {
        _USERSQL_table_row_count = a_USERSQLt_table_row_count;
    }

    inline uint get_PRINTING_PARAMETER_print_table_row_count()
    {
        return _print_table_row_count;
    }

    inline void set_PRINTING_PARAMETER_print_table_row_count(uint a_print_table_row_count)
    {
        _print_table_row_count = a_print_table_row_count;
    }

    inline int get_PRINTING_PARAMETER_pageFROM()
    {
        return _page_from;
    }

    inline void set_PRINTING_PARAMETER_pageFROM(int a_page_from)
    {
        _page_from = a_page_from;
    }

    inline int get_PRINTING_PARAMETER_pageTO()
    {
        return _page_to;
    }

    inline void set_PRINTING_PARAMETER_pageTO(int a_page_to)
    {
        _page_to = a_page_to;
    }

    inline bool GET_CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER()
    {
        return _CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER;
    }


signals:

    void SIGNAL_INCREMENT_PROGRESS_BAR(int countSuccess);


public slots:

	virtual void incrementFontSize__OF_TABLE();

	virtual void decrementFontSize__OF_TABLE();

	virtual void CLOSE_SELECT_EXPORT_DB_DIALOG(YerithSqlTableModel 			 *aSqlTableModel = 0,
											   YerithTableViewWITHpagination *a_table_view_to_list_show = 0);

	virtual inline void selectionner_champs_db_visibles_CHARGES_FINANCIERES()
	{
		selectionner_champs_db_visibles(true, true);
	}


	virtual void selectionner_champs_db_visibles(bool           SETUP_DIALOG_BOX_GEOMETRY,
												 bool           CHARGES_FINANCIERES_WINDOW,
												 QList<QString> &CHARGES_FINANCIERES_WINDOW_Db_ColumnS,
												 QList<QString> &CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STRS);


	virtual void selectionner_champs_db_visibles(bool       SETUP_DIALOG_BOX_GEOMETRY = true,
												 bool       CHARGES_FINANCIERES_WINDOW = false,
												 QString    CHARGES_FINANCIERES_WINDOW_Db_Column = YerithUtils::EMPTY_STRING,
												 QString    CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STR = YerithUtils::EMPTY_STRING);


	virtual void handle_some_actions_tools_enabled();

    virtual void getManuelUtilisateurPDF();

    virtual void infosEntreprise();

    virtual void charges_financieres();

    virtual void achats_de_stocks();

    virtual void stocks();

    virtual void afficherMarchandises();

    virtual inline void modifier_les_articles()
    {
        stocks();
    }

    virtual void alertes();

    virtual void SLOT_CREER_A_PARTIR_DE_CECI();

    virtual void SLOT_CREER_A_PARTIR_DE_CECI_charge_financiere();

    virtual void CREER_UN_GROUPE_DE_SALARIAL_hr();

    virtual void CREER_GROUPE_demployes_hr();

    virtual void GROUPES_demployes_hr();

    virtual void GROUPES_DE_PAIE_hr();

    virtual void fournisseurs();

    virtual void groupes_de_clients();

    virtual void programmes_de_fidelite_clients();

    virtual void creer_un_programme_de_fidelite_clients();

    virtual void clients();

    virtual inline void courrier()
    {
        alertes();
    }

    virtual void sortir();

    virtual void tableaux_de_bords();

    virtual void mouvements_de_stocks();

    virtual void creerFournisseur();

    virtual void creerCompteClient();

    virtual void creerUnGroupeDeClients();

    virtual void vendre_au_COMPTE_CLIENT(bool sowItem = false);

    virtual void entrer_une_charge_financiere();

    virtual void entrer();

    virtual void paiements();

    virtual void ventes();

    virtual void vendre();

    virtual void menu();

    virtual void comptabilite();

    virtual void administration();

    virtual void setup_select_configure_dbcolumn(const QString &aSqlTableName);

    virtual void qui_suis_je();

    virtual bool imprimer_pdf_document_WITH_PAGES_SPECIFICATION(int *pageFROM,
                                                                int *pageTO);

    virtual void INITIALIZE_PDF_PRINTING_AT_ONCE();

    virtual bool imprimer_pdf_document();

    virtual void changer_utilisateur();

    virtual void deconnecter_utilisateur();

    virtual inline void apropos(YerithWindowsCommons *aCallingWindow)
    {
        YerithQMessageBox::about(aCallingWindow,
                                 YerithUtils::APPLICATION_NAME,
                                 YerithUtils::APROPOS_MSG);
    }

    inline void fermeture()
    {
        close();
    }

    virtual inline void help()
    {
    }

    inline void resetTableViewHorizontalHeader_DEFAULT_ORDERING()
    {
//      if (0 == _yerithTableView_FROM_WINDOWS_COMMONS)
//      {
//              return ;
//      }
//
//      //TODO: (ACHEVER L'ALGORITHME).
    }

    virtual inline void setYerithTableViewLastPageNumberText(const QString &aLastPageNumberText)
    {
    }

    virtual inline void setYerithTableViewCurrentPageNumberText(const QString &aNextPageNumberText)
    {
    }

    virtual void updateYerithLineEditQCompleter(const QString &currentDBColumnString);

    virtual inline const QStringList &get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME()
    {
        return _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME;
    }

    void getQModelIndex_db_VALUE_from_MODEL_INDEX(const QString 	&dbTableColumnName,
                                                  const QModelIndex &modelIndex,
                                                  QString 			&db_VALUE_in_out);

    void getQModelIndex_dbID_from_MODEL_INDEX(const QModelIndex &modelIndex,
                                              QString 			&db_ID_in_out);

    virtual void setLast_YERITH_TABLE_VIEW_SelectedRow__db_ID();

    virtual void setLast_YERITH_TABLE_VIEW_SelectedRow__db_ID(const QModelIndex &modelIndex);

    virtual inline void imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR(int pageFROM,
                                                                         int pageTO)
    {
        YerithProgressBar(this)(this,
                                &pageFROM,
                                &pageTO,
                                &YerithWindowsCommons::imprimer_pdf_document_WITH_PAGES_SPECIFICATION);
    }

    virtual inline QString get_latex_template_print_pdf_content()
    {
        return _latex_template_print_pdf_content;
    }

    virtual void imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR();


    /**
     * Created to be able to perform a polymorphic
     * class instance call to an Qt-Gui automatically generated
     * user interface class.
     */
    virtual inline void ___retranslateUi()
    {
    }


	virtual void CHANGER_DE_LANGUE(YerithWindowsCommons *a_calling_window,
                                   QAction              *a_qaction_FOR_TRANSLATION_TEXT_tooltip = 0);


protected slots:


    virtual void CHANGER_DE_LANGUE__text__TRANSLATION(YerithWindowsCommons *a_calling_window,
                                                      QAction              *a_qaction_FOR_TRANSLATION_TEXT_tooltip = 0);


    virtual void CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();

    virtual void AJOUTER_CONDITION_DE_RECHERCHE();

    virtual void setup_print();

    virtual void print_PDF_PREVIOUSLY_SETUP();

    virtual void setYerithLineEditQCompleterSearchFilter
                    (QString &aYerithLineEditQCompleterSearchFilter_IN_OUT,
                      bool GENERATE_Like_SQL_STMT = true);

    virtual void tableView_show_or_hide_columns(YerithTableView &tableView_in_out);

    inline void supprimer_image(QLabel &aLabel_image_produit)
    {
        YerithUtils::supprimer_image(aLabel_image_produit);
    }

    inline void selectionner_image(QLabel &aLabel_image_produit)
    {
        YerithUtils::selectionner_image(this, aLabel_image_produit);
    }

    /**
     * Ce 'slot' est suppose reinitialiser les colones
     * du tableau de la base de donnees et, enfin,
     * actualiser la nouvelle vue du tableau.
     *
     * Exemple d'implementation standard dans la classe
     * 'YerithStocksWindow'.
     */
    virtual inline void slot_reinitialiser_colones_db_visibles()
    {
        reinitialiser_colones_db_visibles();
    }

    static int getDialogBox_LONGUEUR(unsigned int n);

    static int getDialogBox_Yn_coordinate(unsigned int n);

    static int getDialogBox_Xn_coordinate(unsigned int n);


protected:

    bool RESIZE_CHILD_OBJECT(QResizeEvent *event, QWidget *a_child_widget);


    virtual void resizeEvent(QResizeEvent *event);


    void YERITH_ERP_WRAPPER_QACTION_SET_ENABLED_AUTOMATIC_CONSTRUCTOR_ONLY();


    void APPLY_USER_LOCAL_SETTINGS_PARAMETERS_TABLE_COLUMN_ORDER_from_settings_parameters(const QString &pageTableColumnOrder_STRING);

    void APPLY_USER_LOCAL_SETTINGS_PARAMETERS_QTABLEWIDGET(YerithWindowsCommons &a_window);

    void APPLY_USER_LOCAL_SETTINGS_PARAMETERS();


    virtual inline void reinitialiser_colones_db_visibles()
    {
    }


    void setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING(const QString &
                                                      dbtableColumnString);

    void setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING(int dbtableColumnIndex);

    void setCenterAligned_FOR_YERITH_PDF_LATEX_PRINTING(const QString &
                                                        dbtableColumnString);

    void setCenterAligned_FOR_YERITH_PDF_LATEX_PRINTING(int
                                                        dbtableColumnIndex);

    void setRightAligned_FOR_YERITH_PDF_LATEX_PRINTING(const QString &
                                                       dbtableColumnString);

    void setRightAligned_FOR_YERITH_PDF_LATEX_PRINTING(int dbtableColumnIndex);

    void on_print_actions_visibility_SET(bool isVisible);

    inline void enable_on_print_actions_SET_VISIBILITY()
    {
        on_print_actions_visibility_SET(true);
    }

    inline void disable_on_print_actions_SET_VISIBILITY()
    {
        on_print_actions_visibility_SET(false);
    }

    void enableResizing();

    void mySetupUi(QMainWindow *aWindow);

    virtual inline void setupShortcuts()
    {
    }

    inline void setupShortcut__INCREASE_FONT_SIZE(QAction &anAction) const
    {
        anAction.setShortcut(YerithUtils::INCREASE_QTABLE_VIEW_WIDGET_SIZE);
    }

    inline void setupShortcut__DECREASE_FONT_SIZE(QAction &anAction) const
    {
        anAction.setShortcut(YerithUtils::DECREASE_QTABLE_VIEW_WIDGET_SIZE);
    }

    inline void setupShortcutActionMessageDaide(QAction &anAction) const
    {
        anAction.setShortcut(YerithUtils::MESSAGE_DAIDE_QKEYSEQUENCE);
    }

    inline void setupShortcutActionExporterAuFormatCsv(QAction &anAction) const
    {
        anAction.setShortcut(YerithUtils::EXPORTER_AU_FORMAT_CSV_QKEYSEQUENCE);
    }

    inline void setupShortcutActionPARAMETRER_IMPRESSION_PDF(QAction &
                                                             anAction) const
    {
        anAction.setShortcut(YerithUtils::PARAMETRER_IMPRESSION_QKEYSEQUENCE);
    }

    inline void setupShortcutActionAfficherPDF(QAction &anAction) const
    {
        anAction.setShortcut(YerithUtils::IMPRIMER_QKEYSEQUENCE);
    }

    inline void setupShortcutActionQuiSuisJe(QAction &anAction) const
    {
        anAction.setShortcut(YerithUtils::REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE);
    }

    bool _CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER;

    QFont *_CURRENT_TABLE_VIEW_font;

    QSemaphore *_WRITE_READ_YERITH_SEMAPHORE_APPLY_USER_SETTING_FILE_PROPERTIES;

    /**
     * Ces membres (les 3 suivants) de la classe sont utilises afin d'executer
     * la 'recherche' automatique.
     *
     * Ces membres (les 3 suivants) sont assignes des valeurs
     * dans le constructeur de la classe descendante de "QMainWindow",
     * et de "YerithAbstractClassYerithSearchWindow".
     */
    QSet<QString> _yerith_references_dbColumnString;

    YerithComboBox *_yerith_QComboBox_SearchDBFieldColumnString;

    YerithLineEdit *_yerith_QLineEdit_SearchDBFieldColumnString;


    QList<YerithPushButton *> _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT;

    QList<QAction *> _list_actions_to_enable_on_positive_tableview_ROW_COUNT;


    /*
     * SHALL BE A COPY of YerithDatabaseTableColumn::_tableColumnToUserViewString
     */

    QMap<QString, QString> __Previous_TO_Reconduct_tableColumnToUserViewString;



    QMap<QAction *, bool> _MAP_actions_to_enable_on_positive_tableview_ROW_COUNT__TO__AUTHORIZED_FOR_CURRENT_USER;

    QMap<QString, QString> _documentSpecificElements_FOR_PDF_LATEX_PRINTING;

    QString _dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH;

    QMap<QString, QString> _db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName;

    QMap<QString, int> _dbtablecolumnNameToDBColumnIndex;

    QSet<QString> _varchar_dbtable_column_name_list;



    static const QStringList _ALL_WINDOWS_FILTER_NUMERIC_CONDITIONS;



    QStringList _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME;

    QStringList _visibleDBColumnNameStrList;

    QSet<QAction *> _ACTIONS_TO_DISABLE_WHEN_NOT_PRINT_ACTION;


    //MODIFICATION BY USING method 'setCenterAligned_FOR_YERITH_PDF_LATEX_PRINTING'
    QSet<int> _DBFieldNamesToPrintCenterAligned;


    //MODIFICATION BY USING method 'setRightAligned_FOR_YERITH_PDF_LATEX_PRINTING'
    QSet<int> _DBFieldNamesToPrintRightAligned;


    //MODIFICATION BY USING method 'setLeftAligned_FOR_YERITH_PDF_LATEX_PRINTING'
    QSet<int> _DBFieldNamesToPrintLeftAligned;


    QVector<YerithSelectDBQCheckBox *> _visibleQCheckboxs;

    YerithERPGenericSelectDBFieldDialog *_selectExportDBQDialog;


    static YerithERPWindows *_allWindows;

    bool _is_pdf_printing_initialized;

    YerithTableViewPRINT_UTILITIES_TEX_TABLE *_yerith_PRINT_UTILITIES_TEX_TABLE;

    QString _latex_template_print_pdf_content;

    QString _output_print_pdf_latexFileNamePrefix;

    YerithSqlTableModel *_curStocksTableModel;

    QIntValidator *_pagination_nombre_de_ligne_IntValidator;

    bool _first_time_imprimer_pdf_document_call;

    QString QMESSAGE_BOX_STYLE_SHEET;

    QString _windowName_WITH_NO_MAINTENANCE;

    QString _windowName;

    YerithTableView *_yerithTableView_FROM_WINDOWS_COMMONS;

    //PRINTING SETUP PARAMETERS FOR EACH WINDOW.
    int _TABLE_FONT_SIZE;
    int _page_from;
    int _page_to;
    uint _USERSQL_table_row_count;
    uint _print_table_row_count;

    QString _table_COLUMN_ORDER;
    QString _a4paper_printing_position;

    YerithLineEdit *_QLINEEDIT_nombre_de_lignes_par_page;


    static const uint CHECKBOX_YERITH_FOR_DB_TABLE_COLUMN_FILTERING_WIDTH_SIZE;


public:

    static QString STRING_MONDAY;

    static QString STRING_TUESDAY;

    static QString STRING_WEDNESDAY;

    static QString STRING_THURSDAY;

    static QString STRING_FRIDAY;

    static QString STRING_SATURDAY;

    static QString STRING_SUNDAY;


private:

    static QString _yerithTableView_FROM_WINDOWS_COMMONS_LAST_SELECTED_ROW__db_ID;

    static QPoint *_centerPosition;
};




#define MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS		\
	    pushButton_page_premiere->disable(this);						\
		pushButton_page_derniere->disable(this);						\
	    pushButton_page_precedente->disable(this);						\
		pushButton_page_suivante->disable(this);


#define MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(X)	\
		virtual inline void viewYerithTableViewFirstPage(YerithSqlTableModel &curYerithSqlTableModel_IN) \
		{																								 \
			X->viewYerithTableViewFirstPage(curYerithSqlTableModel_IN);									 \
		}																								 \
		virtual inline void viewYerithTableViewLastPage(YerithSqlTableModel &curYerithSqlTableModel_IN)  \
		{																								 \
			X->viewYerithTableViewLastPage(curYerithSqlTableModel_IN);									 \
		}																								 \
		virtual inline void viewYerithTableViewPreviousPage(YerithSqlTableModel &curYerithSqlTableModel_IN) \
		{																								 \
			X->viewYerithTableViewPreviousPage(curYerithSqlTableModel_IN);			 					 \
		}																								 \
		virtual inline void viewYerithTableViewNextPage(YerithSqlTableModel &curYerithSqlTableModel_IN)	 \
		{																								 \
			X->viewYerithTableViewNextPage(curYerithSqlTableModel_IN);									 \
		}


#define MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(X, Y)					\
	    pushButton_page_premiere->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerithAbstractClassYerithSearchWindow*)X, ((YerithAbstractClassYerithSearchWindow*)X)->Get__viewYerithTableViewFirstPage__FPointer(X), &Y);	\
		pushButton_page_derniere->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerithAbstractClassYerithSearchWindow*)X, ((YerithAbstractClassYerithSearchWindow*)X)->Get__viewYerithTableViewLastPage__FPointer(X), &Y);	\
	    pushButton_page_precedente->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerithAbstractClassYerithSearchWindow*)X, ((YerithAbstractClassYerithSearchWindow*)X)->Get__viewYerithTableViewPreviousPage__FPointer(X), &Y);\
		pushButton_page_suivante->enable_WITH_SQL_TABLE_MODEL_AS_CALL_ARGUMENT((YerithAbstractClassYerithSearchWindow*)X, ((YerithAbstractClassYerithSearchWindow*)X)->Get__viewYerithTableViewNextPage__FPointer(X), &Y);


#define MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(X, Y) 				\
	virtual inline void setYerithTableViewLastPageNumberText(const QString &aLastPageNumberText) \
	{																							 \
		X->setText(aLastPageNumberText);						 								 \
	}																							 \
																								 \
	virtual inline void setYerithTableViewCurrentPageNumberText(const QString &aNextPageNumberText) \
	{																								\
		Y->setText(aNextPageNumberText);															\
	}


#define MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(X) {		 \
		YERITH_ERP_WRAPPER_QACTION_SET_ENABLED_AUTOMATIC_CONSTRUCTOR_ONLY(); \
		X->_currentViewWindow = this;										 \
}


#define MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR 		\
	{														\
		if (0 != _pagination_nombre_de_ligne_IntValidator) 	\
		{													\
			delete _pagination_nombre_de_ligne_IntValidator;\
		}													\
	}


#define MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(X, Y) 				\
	{ 															\
			if (0 == _pagination_nombre_de_ligne_IntValidator) 	\
			{					 								\
				_pagination_nombre_de_ligne_IntValidator = 		\
					new QIntValidator(1, YerithUtils::YERITH_PAGINATION_INT_VALIDATOR_MAXIMAL_VALUE); 	\
				X->setValidator(_pagination_nombre_de_ligne_IntValidator); 					\
			} 																				\
																				\
			X->setText(QString::number(Y->getYerithTableViewPageRowCount())); 	\
																				\
			connect(X,															\
					SIGNAL(textChanged(const QString &)),	  					\
					Y,						  									\
					SLOT(slot_set_page_view_row_count(const QString &))); 		\
	}


#endif /* SRC_YERITH_ERP_WINDOWS_COMMONS_HPP_ */
