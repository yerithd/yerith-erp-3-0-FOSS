/*
 * yerith-erp-groupes-dun-EMPLOYE-table-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_GROUPE_DUN_EMPLOYE_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YERITH_GROUPE_DUN_EMPLOYE_TABLE_WIDGET_HPP_


#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"


class YerithGROUPES_DUN_EMPLOYE_Window;


class YerithGroupesDunEmployeTableWidget : public YerithTableWidget
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline YerithGroupesDunEmployeTableWidget(QWidget *parent = 0)
    :YerithTableWidget(parent),
     _groupDesignationItem(0),
     _groupReferenceItem(0),
	 _groupNumberItem(0)
    {
    }

    virtual inline ~YerithGroupesDunEmployeTableWidget()
    {
    }

    int insert_group(const QString &group_db_ID,
                     const QString &groupDesignation,
                     const QString &groupReference,
                     const QString &groupNumber,
                     bool          COLOR_RED = false);


protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);



public:

    YerithGROUPES_DUN_EMPLOYE_Window *a_current_window;


private:

    YerithQTableWidgetItem *_groupDesignationItem;

    YerithQTableWidgetItem *_groupReferenceItem;

    YerithQTableWidgetItem *_groupNumberItem;
};


#endif /* SRC_WIDGETS_YERITH_GROUPE_DUN_EMPLOYE_TABLE_WIDGET_HPP_ */
