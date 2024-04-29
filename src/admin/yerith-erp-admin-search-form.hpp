/*
 * yerith-erp-admin-search-form.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ADMIN_SEARCH_FORM_HPP_
#define SRC_YERITH_ADMIN_SEARCH_FORM_HPP_

#include "../../ui_yerith-erp-admin-search-form.h"

#include <QtWidgets/QDialog>

#include "src/admin/admin-actions-subjects.hpp"

class YerithERPWindows;

class YerithSqlTableModel;
class YerithLogger;

class YerithAdminSearchForm : public QDialog,
							  private Ui_YerithAdminSearchForm
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithAdminSearchForm(YerithERPWindows  *allWindows,
						  QWidget 			*parent);

    inline ~YerithAdminSearchForm()
    {
        delete _logger;
    }


public slots:

	inline void setCurSqlTableModel(YerithSqlTableModel *aCurSqlTableModel)
    {
        _curSqlTableModel = aCurSqlTableModel;
    }

    inline YerithSqlTableModel *getCurSqlTableModel()
    {
        return _curSqlTableModel;
    }

    void reinitialiser();

    void rechercher(const QString &itemName);

    void rendreVisible(int tabWidjetListerIdx);

    void rendreInvisible();


protected:

    virtual QString generate_qstring_FILTER(QString corresponding_DB_column_KeyValue,
                                            QString aTableColumnFieldContentForSearch);

    inline void hideEvent(QHideEvent *hideEvent)
    {
        rendreInvisible();
    }


private slots:

    virtual void setupLineEditsQCompleters();


private:

    inline void clear_all_fields()
    {
        lineEdit_terme_recherche->clear();
    }

    void setupLineEditsQCompleters(int tabWidjetListerIdx);

    enum AdminSujetAction _curSujetAction;

    YerithSqlTableModel *_curSqlTableModel;

    YerithLogger *_logger;

    YerithERPWindows *_allWindows;
};


#endif /* SRC_YERITH_ADMIN_SEARCH_FORM_HPP_ */
