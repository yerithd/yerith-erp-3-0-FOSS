/*
 * yerith-erp-abstract-class-yerithsearch-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-abstract-class-yerithsearch-window.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"


YerithAbstractClassYerithSearchWindow::~YerithAbstractClassYerithSearchWindow()
{
    _yerith_WINDOW_QComboBox_SearchDBFieldColumnString = 0;

    _yerith_WINDOW_QLineEdit_SearchDBFieldColumnString = 0;
}


void YerithAbstractClassYerithSearchWindow::setupLineEditsQCompleters(QObject *aThis,
                                                                      QString aConditionStr,
																	  bool A_CHARGE_FINANCIERE_window /* = false */)
{
    QString correspondingDBFieldKeyValue;

    {
        QMapIterator<YerithLineEdit **, QString> it(_lineEditsToANDContentForSearch);

        YerithLineEdit *aYerithLineEdit = 0;

        while (it.hasNext())
        {
            it.next();

            aYerithLineEdit = *it.key();

            correspondingDBFieldKeyValue = it.value();

            if (0 != aYerithLineEdit)
            {
                if (!correspondingDBFieldKeyValue.isEmpty())
                {
                    if (!YerithUtils::isEqualCaseInsensitive(correspondingDBFieldKeyValue,
                             	 	 	 	 	 	 	 	 YerithDatabaseTableColumn::REFERENCE))
                    {
                        aYerithLineEdit->setupMyStaticQCompleter(_dbYerithSqlTableName,
                        									     correspondingDBFieldKeyValue,
																 true,
																 aConditionStr);
                    }
                    else if (YerithUtils::isEqualCaseInsensitive(correspondingDBFieldKeyValue,
                    											  YerithDatabaseTableColumn::CATEGORIE))
                     {
                    	if (A_CHARGE_FINANCIERE_window)
                    	{
                    		correspondingDBFieldKeyValue = QObject::tr("LIGNE BUDGÉTAIRE");
                    	}

                        aYerithLineEdit->setupMyStaticQCompleter(_dbYerithSqlTableName,
                        										 correspondingDBFieldKeyValue,
																 false);
                     }
                    else
                    {
                        aYerithLineEdit->setupMyStaticQCompleter(_dbYerithSqlTableName,
                        										 correspondingDBFieldKeyValue,
																 false);
                    }
                }

                QObject::connect(aYerithLineEdit,
                                 SIGNAL(textChanged(const QString &)),
                                 aThis,
                                 SLOT(textChangedSearchLineEditsQCompleters()));
            }
        }
    }

    {
        YerithComboBox *aYerithComboBox = 0;

        QMapIterator<YerithComboBox **, QString> it(_comboBoxesToANDContentForSearch);

        while (it.hasNext())
        {
            it.next();

            aYerithComboBox = *it.key();

            if (0 != aYerithComboBox)
            {
                QObject::connect(aYerithComboBox,
                                 SIGNAL(currentTextChanged(const QString &)),
                                 aThis,
                                 SLOT(textChangedSearchLineEditsQCompleters()));
            }
        }
    }

    {
        if (0 != _yerith_WINDOW_QComboBox_SearchDBFieldColumnString &&
            0 != _yerith_WINDOW_QLineEdit_SearchDBFieldColumnString)
        {
            ((YerithWindowsCommons *) aThis)
                ->updateYerithLineEditQCompleter
                    (_yerith_WINDOW_QComboBox_SearchDBFieldColumnString->currentText());

            QObject::connect(_yerith_WINDOW_QComboBox_SearchDBFieldColumnString,
                             SIGNAL(currentTextChanged(const QString &)),
                             aThis,
                             SLOT(updateYerithLineEditQCompleter(const QString &)));

            QObject::connect(_yerith_WINDOW_QLineEdit_SearchDBFieldColumnString,
                             SIGNAL(textChanged(const QString &)),
                             aThis,
                             SLOT(textChangedSearchLineEditsQCompleters()));
        }
    }
}


void YerithAbstractClassYerithSearchWindow::setCurrentlyFiltered(bool
                                                                 currentlyFiltered)
{
    _currentlyFiltered = currentlyFiltered;

    set_filtrer_font();
}


void YerithAbstractClassYerithSearchWindow::clearLineEditsQCompleters()
{
    {
        QMapIterator < YerithLineEdit **,
                     QString > it(_lineEditsToANDContentForSearch);

        YerithLineEdit *aYerithLineEdit = 0;

        while (it.hasNext())
        {
            it.next();

            aYerithLineEdit = *it.key();

            if (0 != aYerithLineEdit)
            {
                aYerithLineEdit->clear();
            }
        }
    }

    {
        QMapIterator < YerithComboBox **,
                     QString > it(_comboBoxesToANDContentForSearch);

        YerithComboBox *aYerithComboBox = 0;

        while (it.hasNext())
        {
            it.next();

            aYerithComboBox = *it.key();

            if (0 != aYerithComboBox)
            {
                aYerithComboBox->resetYerithComboBox();
            }
        }
    }
}


void YerithAbstractClassYerithSearchWindow::resetLineEditsQCompleters(QObject *
                                                                      aThis)
{
    _searchFilter.clear();

    if (0 != _yerithSqlTableModel)
    {
        _yerithSqlTableModel->resetFilter();
    }

    clearLineEditsQCompleters();

    setupLineEditsQCompleters(aThis);
}
