/*
 * yerith-erp-abstract-class-yerithsearch-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_UTILS_WINDOWS_YERITH_ERP_ABSTRACT_CLASS_YERITHSEARCH_WINDOW_HPP_
#define SRC_UTILS_WINDOWS_YERITH_ERP_ABSTRACT_CLASS_YERITHSEARCH_WINDOW_HPP_


#include "src/widgets/yerith-erp-combo-box.hpp"

#include "src/widgets/yerith-erp-line-edit.hpp"


#include <QtCore/QObject>

#include <QtCore/QMap>

#include <QtCore/QString>


class YerithComboBox;
class YerithLineEdit;


class YerithAbstractClassYerithSearchWindow
{

public:

    YERITH_CLASS_OPERATORS

    typedef void (YerithAbstractClassYerithSearchWindow::*fptr)(YerithSqlTableModel &);

    inline YerithAbstractClassYerithSearchWindow(QString aDBYerithSqlTableName)
    :_currentlyFiltered(false),
     _searchFilter(YerithUtils::EMPTY_STRING),
     _dbYerithSqlTableName(aDBYerithSqlTableName),
     _yerithSqlTableModel(0),
     _yerith_WINDOW_QComboBox_SearchDBFieldColumnString(0),
     _yerith_WINDOW_QLineEdit_SearchDBFieldColumnString(0)
    {
    }

    virtual ~YerithAbstractClassYerithSearchWindow();


public slots:

    virtual void viewYerithTableViewFirstPage(YerithSqlTableModel &curYerithSqlTableModel_IN) = 0;

    virtual void viewYerithTableViewLastPage(YerithSqlTableModel &curYerithSqlTableModel_IN) = 0;

    virtual void viewYerithTableViewPreviousPage(YerithSqlTableModel &curYerithSqlTableModel_IN) = 0;

    virtual void viewYerithTableViewNextPage(YerithSqlTableModel &curYerithSqlTableModel_IN) = 0;


    template<class aYerithMainWindowClassType>
    inline fptr Get__viewYerithTableViewFirstPage__FPointer(aYerithMainWindowClassType *aThisRef)
    {
        return (fptr) & aYerithMainWindowClassType::viewYerithTableViewFirstPage;
    }


    template<class aYerithMainWindowClassType>
    inline fptr Get__viewYerithTableViewLastPage__FPointer(aYerithMainWindowClassType *aThisRef)
    {
        return (fptr) & aYerithMainWindowClassType::viewYerithTableViewLastPage;
    }


    template<class aYerithMainWindowClassType>
    inline fptr Get__viewYerithTableViewPreviousPage__FPointer(aYerithMainWindowClassType *aThisRef)
    {
        return (fptr) &
               aYerithMainWindowClassType::viewYerithTableViewPreviousPage;
    }


    template<class aYerithMainWindowClassType>
    inline fptr Get__viewYerithTableViewNextPage__FPointer(aYerithMainWindowClassType *aThisRef)
    {
        return (fptr) & aYerithMainWindowClassType::viewYerithTableViewNextPage;
    }


protected slots:

	virtual inline void setupLineEditsQCompleters(QObject *aThis,
												  bool A_CHARGE_FINANCIERE_window = false)
	{
		setupLineEditsQCompleters(aThis, YerithUtils::EMPTY_STRING, A_CHARGE_FINANCIERE_window);
	}


    virtual void setupLineEditsQCompleters(QObject *aThis,
                                           QString aConditionStr,
										   bool A_CHARGE_FINANCIERE_window = false);


    virtual void textChangedSearchLineEditsQCompleters() = 0;


protected:

    virtual inline void setYerithSqlTableModel(YerithSqlTableModel *aYerithSqlTableModel)
    {
        _yerithSqlTableModel = aYerithSqlTableModel;
    }


    virtual inline void clearSearchFilter()
    {
        _searchFilter.clear();
    }


    virtual inline bool isCurrentlyFiltered()
    {
        return _currentlyFiltered;
    }


    virtual void setCurrentlyFiltered(bool currentlyFiltered);


    virtual void clearLineEditsQCompleters();


    virtual void resetLineEditsQCompleters(QObject *aThis);


    virtual inline void set_filtrer_font()
    {
    }



    bool _currentlyFiltered;

    QString _searchFilter;

    QString _dbYerithSqlTableName;

    YerithSqlTableModel *_yerithSqlTableModel;

    /**
     * for use with _yerith_WINDOW_QComboBox_SearchDBFieldColumnString,
     * and _yerith_WINDOW_QLineEdit_SearchDBFieldColumnString
     */
    QSet<QString> _yerith_WINDOW_references_dbColumnString;

    YerithComboBox *_yerith_WINDOW_QComboBox_SearchDBFieldColumnString;

    YerithLineEdit *_yerith_WINDOW_QLineEdit_SearchDBFieldColumnString;

    QMap<YerithComboBox **, QString> _comboBoxesToANDContentForSearch;

    QMap<YerithLineEdit **, QString> _lineEditsToANDContentForSearch;
};


#endif /* SRC_UTILS_WINDOWS_YERITH_ERP_ABSTRACT_CLASS_YERITHSEARCH_WINDOW_HPP_ */
