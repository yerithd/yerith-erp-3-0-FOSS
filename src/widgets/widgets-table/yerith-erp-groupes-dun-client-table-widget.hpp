/*
 * yerith-erp-groupes-dun-client-table-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_GROUPE_DUN_CLIENT_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YERITH_GROUPE_DUN_CLIENT_TABLE_WIDGET_HPP_


#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"


class YerithGroupesDunClientTableWidget : public YerithTableWidget
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	inline YerithGroupesDunClientTableWidget(QWidget *parent = 0)
    :YerithTableWidget(parent),
     _groupDesignationItem(0),
     _groupReferenceItem(0),
	 _groupNumberItem(0)
    {
    }

    virtual inline ~YerithGroupesDunClientTableWidget()
    {
    }

    int insert_group(const QString &group_db_ID,
                     const QString &groupDesignation,
                     const QString &groupReference,
                     const QString &groupNumber);

private:

    YerithQTableWidgetItem *_groupDesignationItem;

    YerithQTableWidgetItem *_groupReferenceItem;

    YerithQTableWidgetItem *_groupNumberItem;
};


#endif /* SRC_WIDGETS_YERITH_GROUPE_DUN_CLIENT_TABLE_WIDGET_HPP_ */
