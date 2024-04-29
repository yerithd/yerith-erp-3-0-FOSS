/*
 * yerith-erp-select-db-qcheckbox.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_SELECT_DB_QCHECKBOX_HPP_
#define SRC_WIDGETS_YERITH_SELECT_DB_QCHECKBOX_HPP_


#include "src/widgets/yerith-erp-qcheckbox.hpp"


class YerithSelectDBQCheckBox : public YerithERPQCheckBox
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithSelectDBQCheckBox(QWidget *parent,
							QStringList *visibleDBFieldColumnStrList);


    virtual inline ~YerithSelectDBQCheckBox()
    {
    }

    virtual inline void set_visibleDBFieldColumnStrList(QStringList *A_visibleDBFieldColumnStrList)
    {
    	_visibleDBFieldColumnStrList = A_visibleDBFieldColumnStrList;
    }

public slots:

    void handle_visible_db_field_checkBox(bool clicked);


private:

    QStringList *_visibleDBFieldColumnStrList;

};

#endif /* SRC_WIDGETS_YERITH_SELECT_DB_QCHECKBOX_HPP_ */
