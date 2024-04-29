/*
 * yerith-erp-admin-windows-commons.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "src/admin/yerith-erp-admin-windows-commons.hpp"

#include "src/yerith-erp-windows.hpp"


#include "src/widgets/yerith-erp-select-db-qcheckbox.hpp"

#include "src/dialogs/yerith-erp-generic-select-db-field-dialog.hpp"


#include <QtCore/QProcess>

#include <QtCore/QObject>

#include <QtWidgets/QDesktopWidget>


YerithERPWindows *YerithPOSAdminWindowsCommons::_allWindows(0);

QPoint *YerithPOSAdminWindowsCommons::_centerPosition(new QPoint);



YerithPOSAdminWindowsCommons::~YerithPOSAdminWindowsCommons()
{
	YerithERPGenericSelectDBFieldDialog *cur_dbfield_dialog = 0;

	 QMapIterator<QString, std::tuple<bool, YerithERPGenericSelectDBFieldDialog *>>
	 	 iterator_yr_selectfield_dialog(sqlTableName__TO__selectDBFieldDialog);

	 while (iterator_yr_selectfield_dialog.hasNext())
	 {
		 iterator_yr_selectfield_dialog.next();

		 cur_dbfield_dialog = std::get<1>(iterator_yr_selectfield_dialog.value());

		 if (0 != cur_dbfield_dialog)
		 {
			 delete cur_dbfield_dialog;
		 }
	 }
}


void YerithPOSAdminWindowsCommons::menu()
{
    _allWindows->_adminWindow->rendreVisible();
    rendreInvisible();
}


void YerithPOSAdminWindowsCommons::retour_menu_principal()
{
    if (0 != _allWindows->_adminWindow->_curStocksTableModel)
    {
        _curStocksTableModel = _allWindows->_adminWindow->_curStocksTableModel;
    }

    _allWindows->_mainWindow->rendreVisible(_curStocksTableModel);

    rendreInvisible();
}


void YerithPOSAdminWindowsCommons::
		CLOSE_SELECT_EXPORT_DB_DIALOG(YerithSqlTableModel 			*aSqlTableModel /* = 0 */,
									  YerithTableViewWITHpagination *a_table_view_to_list_show /* = 0 */)
{
	YerithERPGenericSelectDBFieldDialog *cur_dbfield_dialog = 0;

	for (uint i = 0;
		 i < sqlTableName__TO__selectDBFieldDialog.size();
		 ++i)
	{
		if (0 != cur_dbfield_dialog)
		{
			cur_dbfield_dialog->close();
		}
	}

	if (0 != aSqlTableModel 		&&
		0 != a_table_view_to_list_show)
	{
		_allWindows->_adminListerWindow
			->LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*a_table_view_to_list_show,
				 	 	 	 	 	 	 	   	   *aSqlTableModel);
	}
}


void YerithPOSAdminWindowsCommons::
		selectionner_champs_db_visibles(bool        SETUP_DIALOG_BOX_GEOMETRY /* = true */,
										bool        CHARGES_FINANCIERES_WINDOW /* = false */,
										QString     CHARGES_FINANCIERES_WINDOW_Db_Column /* = YerithUtils::EMPTY_STRING */,
										QString     CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STR /* = YerithUtils::EMPTY_STRING */)
{
	if (0 != _selectExportDBQDialog)
	{
		if (0 != _selectExportDBQDialog->_associatedWindow)
		{
			YerithTableView *a_table_view =
					_selectExportDBQDialog->_associatedWindow->GET_YERITH_TABLE_VIEW();

			if (0 != a_table_view)
			{
				QString ASSOCIATED_qsql_database = a_table_view->getSqlTableName();

				_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName.clear();

				setup_select_configure_dbcolumn(ASSOCIATED_qsql_database);
			}
		}
	}


	if (_visibleDBColumnNameStrList.size() >= 0)
    {
        _visibleQCheckboxs.clear();
        _visibleQCheckboxs.resize(_visibleDBColumnNameStrList.size());
    }


    QString aDBColumnName;

    QString aDBColumnName_VIEW_STRING;

    for (unsigned k = 0; k < _dbtablecolumnNameToDBColumnIndex.size(); ++k)
    {
        aDBColumnName = _dbtablecolumnNameToDBColumnIndex.key(k);

        if (_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.contains(aDBColumnName))
        {
            continue;
        }


        if (! YerithERPConfig::SHOW_DATABASE_RAW)
        {
        	if (! CHARGES_FINANCIERES_WINDOW)
        	{
        		YerithDatabaseTableColumn::_tableColumnToUserViewString
					.insert(YerithDatabaseTableColumn::CATEGORIE,
							QObject::tr("Catégorie"));
        	}
        	else
        	{
        		YerithDatabaseTableColumn::_tableColumnToUserViewString
					.insert(YerithDatabaseTableColumn::CATEGORIE,
							QObject::tr("LIGNE BUDGÉTAIRE"));
        	}

            aDBColumnName_VIEW_STRING =
                            YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(aDBColumnName);
        }
        else
        {
            aDBColumnName_VIEW_STRING = aDBColumnName;
        }


        if (!aDBColumnName_VIEW_STRING.isEmpty())
        {
            _db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName
				.insert(aDBColumnName_VIEW_STRING,
						aDBColumnName);
        }
    }


    //A QMAP AUTOMATICALLY SORT ITS KEYS !
    QStringList all_dbfieldColumnName_IN_ALPHABETIC_ORDER
					(_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName.keys());


    if (SETUP_DIALOG_BOX_GEOMETRY)
    {
    	YerithSelectDBQCheckBox *aQCheckBox = 0;

    	for (unsigned k = 0; k < all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size();
    			++k)
    	{
    		aQCheckBox = new YerithSelectDBQCheckBox(_selectExportDBQDialog,
    												 &_visibleDBColumnNameStrList);


    		aDBColumnName_VIEW_STRING = all_dbfieldColumnName_IN_ALPHABETIC_ORDER.at(k);

    		aDBColumnName =
    				_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName
						.value(aDBColumnName_VIEW_STRING);

    		aQCheckBox->setObjectName(aDBColumnName);

    		aQCheckBox->setGeometry(QRect(getDialogBox_Xn_coordinate(k),
    								getDialogBox_Yn_coordinate(k),
									YerithWindowsCommons::CHECKBOX_YERITH_FOR_DB_TABLE_COLUMN_FILTERING_WIDTH_SIZE,
									25));

    		aQCheckBox->setText(aDBColumnName_VIEW_STRING);

    		if (_visibleDBColumnNameStrList.contains(aDBColumnName))
    		{
    			aQCheckBox->setChecked(true);
    		}

    		connect(aQCheckBox,
    				SIGNAL(clicked(bool)),
					aQCheckBox,
					SLOT(handle_visible_db_field_checkBox(bool)));

    		_visibleQCheckboxs.append(aQCheckBox);
    	}

    	_selectExportDBQDialog
			->setWindowTitle(QObject::tr("%1 - sélectionner des colones "
    									 "visibles ('%2' colones)")
								.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
									 QString::number(all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size())));

    	_selectExportDBQDialog
			->setFixedSize(getDialogBox_LONGUEUR(all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size()),
												 382.5);

    	_selectExportDBQDialog->showAsModalDialogWithParent(*this);
    }
}


/**
 * !!! TEMPORARY SOLUTION:
 * 1. FRENCH LANGUAGE SETTING ALWAYS RE-CREATE A
 *    SELECT DATABASE COLUMN FIELD DIALOG.
 * 2. TODO: generaliye with previous and new saved
 *    language setting !
 */
YerithERPGenericSelectDBFieldDialog *YerithPOSAdminWindowsCommons::
			GET__LOCALIZED__generic_selectdbfield__DIALOG__(QString aSqlTableName)
{
	YerithERPGenericSelectDBFieldDialog *result__LOCALIZED__selectdbfield__DIALOG = 0;


	if (sqlTableName__TO__selectDBFieldDialog.contains(aSqlTableName))
	{

		bool previous_setting_ENGLISH_LANGUAGE =
				(YerithMainWindow::LANGUE_ANGLAISE == std::get<0>(sqlTableName__TO__selectDBFieldDialog.value(aSqlTableName)));


		YerithERPGenericSelectDBFieldDialog *CURRENT_SELECTDBFIELD_DIALOG =
				std::get<1>(sqlTableName__TO__selectDBFieldDialog.value(aSqlTableName));


		if (previous_setting_ENGLISH_LANGUAGE)
		{
			result__LOCALIZED__selectdbfield__DIALOG = CURRENT_SELECTDBFIELD_DIALOG;
		}
		else
		{
			YERITH_DELETE_FREE_POINTER_NOW(CURRENT_SELECTDBFIELD_DIALOG);

			CURRENT_SELECTDBFIELD_DIALOG = 0;

			result__LOCALIZED__selectdbfield__DIALOG = 0;
		}

		if (0 == result__LOCALIZED__selectdbfield__DIALOG)
		{
		    _selectExportDBQDialog =
		    		new YerithERPGenericSelectDBFieldDialog(_allWindows,
		    												this);

			if (0 != getQMainWindowToolBar())
			{
				_selectExportDBQDialog->setPalette(getQMainWindowToolBar()->palette());
			}

		    result__LOCALIZED__selectdbfield__DIALOG = _selectExportDBQDialog;

		    sqlTableName__TO__selectDBFieldDialog.remove(aSqlTableName);

		    sqlTableName__TO__selectDBFieldDialog.insert(aSqlTableName,
		    											 std::make_tuple(YerithMainWindow::LANGUE_ANGLAISE,
		    													 	 	 _selectExportDBQDialog));
		}
	}
	else
	{
	    _selectExportDBQDialog =
	    		new YerithERPGenericSelectDBFieldDialog(_allWindows,
	    												this);

		if (0 != getQMainWindowToolBar())
		{
			_selectExportDBQDialog->setPalette(getQMainWindowToolBar()->palette());
		}

	    result__LOCALIZED__selectdbfield__DIALOG = _selectExportDBQDialog;

	    sqlTableName__TO__selectDBFieldDialog.remove(aSqlTableName);

	    sqlTableName__TO__selectDBFieldDialog.insert(aSqlTableName,
	    											 std::make_tuple(YerithMainWindow::LANGUE_ANGLAISE,
	    													 	 	 _selectExportDBQDialog));
	}

	return result__LOCALIZED__selectdbfield__DIALOG;
}


void YerithPOSAdminWindowsCommons::setup_select_configure_dbcolumn(const QString &aSqlTableName)
{
	if (aSqlTableName.isEmpty())
	{
		return ;
	}


	_selectExportDBQDialog =
			GET__LOCALIZED__generic_selectdbfield__DIALOG__(aSqlTableName);


	_selectExportDBQDialog->setStyleSheet(qMessageBoxStyleSheet());


	_varchar_dbtable_column_name_list.clear();

	_DBFieldNamesToPrintLeftAligned.clear();

	_DBFieldNamesToPrintRightAligned.clear();

	_dbtablecolumnNameToDBColumnIndex.clear();


	QString strShowColumnQuery = QString("SHOW COLUMNS FROM %1")
    												.arg(aSqlTableName);

	QSqlQuery query;

	int querySize = YerithUtils::execQuery(query, strShowColumnQuery);

	unsigned int columnIdx = -1;

	for (int k = 0; k < querySize && query.next(); ++k)
	{
		QString fieldName(query.value(0).toString());

		QString type(query.value(1).toString());

		columnIdx = columnIdx + 1;

		if (type.contains(YerithUtils::DATABASE_MYSQL_VARCHAR_TYPE_STRING) ||
			YerithUtils::isEqualCaseInsensitive(fieldName,
												YerithDatabaseTableColumn::ID))
		{
			_varchar_dbtable_column_name_list.insert(fieldName);
		}

		if (type.contains(YerithUtils::DATABASE_MYSQL_VARCHAR_TYPE_STRING)  ||
			type.contains(YerithUtils::DATABASE_MYSQL_CHAR_TYPE_STRING) 	||
			type.contains(YerithUtils::DATABASE_MYSQL_TIME_TYPE_STRING) 	||
			type.contains(YerithUtils::DATABASE_MYSQL_DATE_TYPE_STRING))
		{
			_DBFieldNamesToPrintLeftAligned.insert(columnIdx);
		}
		else if (type.contains(YerithUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING) ||
				 type.contains(YerithUtils::DATABASE_MYSQL_INT_TYPE_STRING))
		{
			_DBFieldNamesToPrintRightAligned.insert(columnIdx);
		}

		_dbtablecolumnNameToDBColumnIndex.insert(fieldName,
												 columnIdx);
	}
}


void YerithPOSAdminWindowsCommons::infosEntreprise()
{
    YerithUtils::infosEntreprise(*this,
                                 _allWindows->getInfoEntreprise().
                                 toString());
}

void YerithPOSAdminWindowsCommons::getManuelUtilisateurPDF()
{
    YerithPOSUser *user = _allWindows->getUser();
    if (user)
    {
        QStringList progArguments;
        QProcess aProcess;

        if (user->isAdmin() || user->isManager())
        {
            progArguments <<
                          YerithERPConfig::
                          FILE_ABSOLUTEPATH_YERITH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;
            aProcess.startDetached(YerithERPConfig::pathToPdfReader,
                                   progArguments);
        }
    }
}

void YerithPOSAdminWindowsCommons::mySetupUi(QMainWindow *aWindow)
{
    aWindow->setFixedSize(aWindow->width(), aWindow->height());
    aWindow->move(*_centerPosition);
}

void YerithPOSAdminWindowsCommons::qui_suis_je()
{
    YerithQMessageBox::information(this, QObject::tr("qui suis je ?"),
                                   _allWindows->getUser()->toString());
}

void YerithPOSAdminWindowsCommons::changer_utilisateur()
{
    _allWindows->_changerUtilisateurDialog->setParent(this,
                                                      Qt::Popup | Qt::Dialog);
    _allWindows->_changerUtilisateurDialog->show();
    _allWindows->_changerUtilisateurDialog->activateWindow();
}

void YerithPOSAdminWindowsCommons::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->rendreVisible(&_allWindows->
                                            getSqlTableModel_stocks());
    setVisible(false);
}


void YerithPOSAdminWindowsCommons::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
    setVisible(true);
}
