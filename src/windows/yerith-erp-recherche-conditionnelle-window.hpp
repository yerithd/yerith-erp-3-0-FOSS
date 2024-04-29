/*
 * yerith-erp-recherche-conditionnelle-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_RECHERCHE_CONDITIONNELLE_WINDOW_HPP_
#define SRC_YERITH_RECHERCHE_CONDITIONNELLE_WINDOW_HPP_

#include "../../ui_yerith-erp-recherche-conditionnelle-window.h"

#include "src/yerith-erp-windows.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"



class YerithRechercheConditionnelleWindow : public YerithWindowsCommons,
    										public Ui_YerithRechercheConditionnelleWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithRechercheConditionnelleWindow();

    inline ~YerithRechercheConditionnelleWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_recherche_conditionnelle;
    }


    void rendreVisible(QStringList 	&db_column_to_SEARCH_FILTER_APPEND_LIST,
                       QPalette		&aPalette);

    inline void rendreInvisible()
    {
        setVisible(false);
    }


public slots:

    virtual void updateYerithLineEditQCompleter_1(const QString &
                                                  currentDBColumnString);

    virtual void updateYerithLineEditQCompleter_2(const QString &
                                                  currentDBColumnString);

    virtual void updateYerithLineEditQCompleter_3(const QString &
                                                  currentDBColumnString);

    virtual void updateYerithLineEditQCompleter_4(const QString &
                                                  currentDBColumnString);

    void reset_filter();

private:

    void populateComboBoxes();

    void setupLineEdits();

public:

    static YerithWindowsCommons
    *_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING;

protected:

    QStringList _db_column_to_SEARCH_FILTER_APPEND_LIST;


    YerithLogger *_logger;
};

#endif /* SRC_YERITH_RECHERCHE_CONDITIONNELLE_WINDOW_HPP_ */
