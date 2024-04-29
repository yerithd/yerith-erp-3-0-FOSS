/*
 * yerith-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.cpp
 *
 * Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include "src/yerith-erp-windows.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtWidgets/QHeaderView>


void YerithERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION::
		handle_yerith_header_view_position_changed(int logicalIndex,
												   int oldVisualIndex,
												   int newVisualIndex)
{
    //      QDEBUG_QSTRINGLIST_OUTPUT("_tableModelRawHeaders_IN_OUT",
    //                                                         _tableModelRawHeaders_IN_OUT);

    //      QDEBUG_STRING_OUTPUT_1(QString("handle_yerith_header_view_position_changed, "
    //                                                                 "logicalIndex: %1,"
    //                                                                 "oldVisualIndex: %2, "
    //                                                                 "newVisualIndex: %3")
    //                                                              .arg(QString::number(logicalIndex),
    //                                                                       QString::number(oldVisualIndex),
    //                                                                       QString::number(newVisualIndex)));

    YerithTableView *a_yerith_table_view = 0;

    if (0 != parent() &&
            (0 != parent()->parent()) &&
            (0 != parent()->parent()->parent()) &&
            (0 != parent()->parent()->parent()->parent()))
    {

        QTabWidget *a_q_tab_widget = dynamic_cast<QTabWidget *>(parent()->parent()->parent());

        if (0 == a_q_tab_widget)
        {
//                      QDEBUG_STRING_OUTPUT_1("YERITH-TAB_WIDGET-TYPE-NOT_OK");
            return;
        }


        YerithWindowsCommons *a_yerith_window =
        		dynamic_cast<YerithWindowsCommons *>(a_q_tab_widget->parent()->parent());

        if (0 == a_yerith_window)
        {
//                      QDEBUG_STRING_OUTPUT_1("YERITH-WINDOW-TYPE-NOT_OK");
            return;
        }


        if (0 != _q_header_view 														&&
            !a_yerith_window->GET_CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER())
        {
            QStringList yerithTableView_model_raw_visible_headers(_tableModelRawHeaders_IN_OUT);

            const QStringList *a_visible_DB_columnname_string_List =
            		a_yerith_window->get_visible_DB_column_name_str_list();

            QString pageTableColumnOrder_STRING;

            if (0 != a_visible_DB_columnname_string_List)
            {
                for (uint i = 0;
                	 i < yerithTableView_model_raw_visible_headers.size();
                	 ++i)
                {
                    const QString &headerColumnData =
                    		yerithTableView_model_raw_visible_headers.at(i);

                    if (!headerColumnData.isEmpty())
                    {
                        if (a_visible_DB_columnname_string_List->contains(headerColumnData))
                        {
                            pageTableColumnOrder_STRING
								.append(QString("%1:%2;")
										 .arg(headerColumnData,
											  QString::number(_q_header_view->visualIndex(i))));
                        }
                    }
                }
            }

            a_yerith_window->set_PARAMETER_TABLE_COLUMN_ORDER(pageTableColumnOrder_STRING);

            YerithERPWindows *allWindows = YerithUtils::getAllWindows();

            if (0 != allWindows)
            {
                YerithPOSUser *aUser = allWindows->getUser();

                if (0 != aUser)
                {
                    aUser->save_user_personal_PRINTING_PARAMETER_settings();
                }
            }
        }
    }
}
