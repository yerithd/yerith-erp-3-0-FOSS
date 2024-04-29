/*
 * yerith-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp
 *
 * Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION_HPP_
#define SRC_YERITH_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION_HPP_


#include "src/widgets/table-view/yerith-erp-table-view-with-pagination.hpp"


class YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION : public YerithTableViewWITHpagination
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

    inline YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
    :YerithTableViewWITHpagination()
    {
    }

    inline YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION(QWidget *parent)
    :YerithTableViewWITHpagination(parent)
    {
    }

    virtual inline ~YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
    {
    }

protected slots:

    virtual void handle_yerith_header_view_position_changed(int logicalIndex,
                                                            int oldVisualIndex,
                                                            int newVisualIndex);
};


#endif /* SRC_YERITH_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION_HPP_ */
