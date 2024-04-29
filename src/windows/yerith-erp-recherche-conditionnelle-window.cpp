/*
 * yerith-erp-recherche-conditionnelle-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-recherche-conditionnelle-window.hpp"


YerithWindowsCommons *
YerithRechercheConditionnelleWindow::
_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING;


YerithRechercheConditionnelleWindow::YerithRechercheConditionnelleWindow()
:YerithWindowsCommons("yerith-erp-recherche-conditionnelle-stock"),
 _logger(new YerithLogger("YerithRechercheConditionnelleWindow"))
{
	_windowName = YerithMainWindow::get_TRANSLATED_WindowName("recherche conditionnelle");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    pushButton_CONDITIONAL_SEARCH_RESET_filter->enable(this,
                                                       SLOT(reset_filter()));
}


void YerithRechercheConditionnelleWindow::rendreVisible(QStringList &db_column_to_SEARCH_FILTER_APPEND_LIST,
                                                        QPalette 	&aPalette)
{
    toolBar_recherche_conditionnelle->setPalette(aPalette);

    _db_column_to_SEARCH_FILTER_APPEND_LIST =
                    db_column_to_SEARCH_FILTER_APPEND_LIST;


    populateComboBoxes();

    setupLineEdits();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("recherche conditionnelle"),
      								 _curStocksTableModel);

    setVisible(true);
}


void YerithRechercheConditionnelleWindow::updateYerithLineEditQCompleter_1(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yerith_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_1->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_1->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerithRechercheConditionnelleWindow::updateYerithLineEditQCompleter_2(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yerith_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_2->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_2->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerithRechercheConditionnelleWindow::updateYerithLineEditQCompleter_3(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yerith_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_3->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_3->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerithRechercheConditionnelleWindow::updateYerithLineEditQCompleter_4(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yerith_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_4->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_4->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerithSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerithRechercheConditionnelleWindow::reset_filter()
{
    lineEdit_nom_element_string_db_1->myClear();
    lineEdit_nom_element_string_db_2->myClear();
    lineEdit_nom_element_string_db_3->myClear();
    lineEdit_nom_element_string_db_4->myClear();

    comboBox_element_string_db_1->resetYerithComboBox();
    comboBox_element_string_db_2->resetYerithComboBox();
    comboBox_element_string_db_3->resetYerithComboBox();
    comboBox_element_string_db_4->resetYerithComboBox();
}


void YerithRechercheConditionnelleWindow::populateComboBoxes()
{
    if (0 ==
            YerithRechercheConditionnelleWindow::
            _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING)
    {
        return;
    }

    reset_filter();

    comboBox_element_string_db_1->clear();
    comboBox_element_string_db_2->clear();
    comboBox_element_string_db_3->clear();
    comboBox_element_string_db_4->clear();

//      qDebug() << YerithRechercheConditionnelleWindow::_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING;

    QObject::disconnect(lineEdit_nom_element_string_db_1, 0, 0, 0);
    QObject::disconnect(lineEdit_nom_element_string_db_2, 0, 0, 0);
    QObject::disconnect(lineEdit_nom_element_string_db_3, 0, 0, 0);
    QObject::disconnect(lineEdit_nom_element_string_db_4, 0, 0, 0);

    QObject::connect(lineEdit_nom_element_string_db_1,
                     SIGNAL(textChanged(const QString &)),
                     YerithRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(lineEdit_nom_element_string_db_2,
                     SIGNAL(textChanged(const QString &)),
                     YerithRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(lineEdit_nom_element_string_db_3,
                     SIGNAL(textChanged(const QString &)),
                     YerithRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(lineEdit_nom_element_string_db_4,
                     SIGNAL(textChanged(const QString &)),
                     YerithRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(comboBox_element_string_db_1,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerithLineEditQCompleter_1
                          (const QString &)));

    QObject::connect(comboBox_element_string_db_2,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerithLineEditQCompleter_2
                          (const QString &)));

    QObject::connect(comboBox_element_string_db_3,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerithLineEditQCompleter_3
                          (const QString &)));

    QObject::connect(comboBox_element_string_db_4,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerithLineEditQCompleter_4
                          (const QString &)));


    YerithUtils::REMOVE_QSTRING_LIST(_db_column_to_SEARCH_FILTER_APPEND_LIST,
                                     YerithRechercheConditionnelleWindow::
                                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
                                     ());


    _db_column_to_SEARCH_FILTER_APPEND_LIST.
    removeAll(YerithUtils::EMPTY_STRING);


    comboBox_element_string_db_1->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerithDatabaseTableColumn::_tableColumnToUserViewString);

    _db_column_to_SEARCH_FILTER_APPEND_LIST.removeOne
    (YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN
     (comboBox_element_string_db_1->currentText()));

    comboBox_element_string_db_2->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerithDatabaseTableColumn::_tableColumnToUserViewString);

    _db_column_to_SEARCH_FILTER_APPEND_LIST.removeOne
    (YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN
     (comboBox_element_string_db_2->currentText()));

    comboBox_element_string_db_3->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerithDatabaseTableColumn::_tableColumnToUserViewString);

    _db_column_to_SEARCH_FILTER_APPEND_LIST.removeOne
    (YERITH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN
     (comboBox_element_string_db_3->currentText()));

    comboBox_element_string_db_4->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerithDatabaseTableColumn::_tableColumnToUserViewString);


    comboBox_element_string_db_1->setCurrentIndex(0);
    comboBox_element_string_db_2->setCurrentIndex(0);
    comboBox_element_string_db_3->setCurrentIndex(0);
    comboBox_element_string_db_4->setCurrentIndex(0);
}


void YerithRechercheConditionnelleWindow::setupLineEdits()
{
    lineEdit_nom_element_string_db_1->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
    lineEdit_nom_element_string_db_2->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
    lineEdit_nom_element_string_db_3->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
    lineEdit_nom_element_string_db_4->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
}
