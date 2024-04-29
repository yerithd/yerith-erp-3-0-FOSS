/*
 * yerith-erp-qtable-widget-admin-window-commons.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_QTABLE_WIDGET_ADMIN_WINDOWS_COMMONS_HPP_
#define SRC_YERITH_ERP_QTABLE_WIDGET_ADMIN_WINDOWS_COMMONS_HPP_


#include "src/admin/yerith-erp-admin-windows-commons.hpp"


#include "src/widgets/widgets-table/yerith-erp-table-widget.hpp"


class YerithQTableWidgetAdminWindowsCommons : public YerithPOSAdminWindowsCommons
{
	Q_OBJECT

public:

    inline YerithQTableWidgetAdminWindowsCommons(const QString &anOutput_print_pdf_latexFileNamePrefix = "")
    :YerithPOSAdminWindowsCommons(anOutput_print_pdf_latexFileNamePrefix),
     _Last_SelectedRow_Row_INDEX(0),
     _a_CURRENT_table_widget(0)
    {
    }


    virtual inline ~YerithQTableWidgetAdminWindowsCommons()
    {
    }


public slots:

    virtual void imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR();


    virtual void handle_some_actions_tools_enabled();


	virtual void incrementFontSize__OF_TABLE__WIDGET();

	virtual void decrementFontSize__OF_TABLE__WIDGET();


protected slots:

    virtual bool imprimer_UNE_LISTE(bool *full_list = 0) = 0;


    virtual bool imprimer_UNE_LISTE(YerithTableWidget &a_yerith_qtable_widget,
                                    QString           &a_yerith_qtable_widget__TexDocument_In_Out,
                                    QString            file_name_Subject_Title_FRENCH,
                                    QString            file_name_Subject_Title_ENGLISH,
                                    QString            prefix_file_name_FRENCH,
                                    QString            prefix_file_name_ENGLISH);


    virtual void *imprimer_UNE_LISTE_JUSQUAU_selectionne();


    virtual void imprimer_UNE_LISTE_WITH_A_YERITH_PROGRESS_BAR();


    virtual inline void imprimer_UNE_LISTE_JUSQUAU_selectionne_WITH_A_YERITH_PROGRESS_BAR()
    {
        YerithProgressBar(this)(this,
                                &YerithQTableWidgetAdminWindowsCommons::imprimer_UNE_LISTE_JUSQUAU_selectionne);
    }


    virtual inline void setLast_SelectedRow_Row_ID(const QModelIndex &a_model_CELL_index)
    {
        _Last_SelectedRow_Row_INDEX = a_model_CELL_index.row();
    }


protected:

    void connect_Qtable_Widgets(YerithTableWidget &a_yerith_qtable_widget);


    virtual void get_PRINT_OUT_TexTableString(YerithTableWidget &a_yerith_qtable_widget,
                                              QString           &texTable_IN_OUT,
                                              QString            Header_TEMPLATE_texTable_In,
                                              QString            FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In = "",
                                              QString            ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In = "",
                                              int                row_MAX_TO_GO_export = -1);


    virtual inline int getLast_SelectedRow_Row_ID()
    {
        return _Last_SelectedRow_Row_INDEX;
    }


    int                  _Last_SelectedRow_Row_INDEX;

    YerithTableWidget   *_a_CURRENT_table_widget;
};


#endif /* SRC_YERITH_ERP_QTABLE_WIDGET_ADMIN_WINDOWS_COMMONS_HPP_ */
