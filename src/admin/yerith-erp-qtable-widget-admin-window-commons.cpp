/*
 * yerith-erp-qtable-widget-admin-window-commons.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-qtable-widget-admin-window-commons.hpp"


#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/windows/yerith-erp-main-window.hpp"



void YerithQTableWidgetAdminWindowsCommons::handle_some_actions_tools_enabled()
{
    if (0 == _a_CURRENT_table_widget)
    {
        return;
    }

    bool enable_action = false;

    // enabling QAction on positive YERITHQTABLEWIDGET rowcount
    {
        QAction *anAction = 0;

        QMapIterator<QAction *, bool>
        	j(_MAP_actions_to_enable_on_positive_tableview_ROW_COUNT__TO__AUTHORIZED_FOR_CURRENT_USER);

        while (j.hasNext())
        {
            j.next();

            anAction = j.key();

            if (0 != anAction)
            {
                if (_list_actions_to_enable_on_positive_tableview_ROW_COUNT.contains(anAction))
                {
                    enable_action = (_a_CURRENT_table_widget->rowCount() > 0);
                }
                else
                {
                    enable_action = true;
                }

                anAction->setVisible((j.value() && enable_action));
            }
        }
    }

    // enabling YerithPushButton on positive YERITHQTABLEWIDGET rowcount
    {
        enable_action = (_a_CURRENT_table_widget->rowCount() > 0);

        int list_size = _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT.size();

        YerithPushButton *aYerithPushButton = 0;

        for (unsigned int j = 0; j < list_size; ++j)
        {
            aYerithPushButton = _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT.at(j);

            if (0 != aYerithPushButton)
            {
                aYerithPushButton
					->setVisible((aYerithPushButton->isAuthorizedForCurrentUser() && enable_action));
            }
        }
    }

    _a_CURRENT_table_widget->resizeRowsToContents();

    _a_CURRENT_table_widget->resizeColumnsToContents();
}


void YerithQTableWidgetAdminWindowsCommons::incrementFontSize__OF_TABLE__WIDGET()
{
	if (0 != _a_CURRENT_table_widget &&
        0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		++_TABLE_FONT_SIZE;

		_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

		_a_CURRENT_table_widget->hide();

		_a_CURRENT_table_widget->resizeColumnsToContents();

		_a_CURRENT_table_widget->show();

        YerithERPWindows *allWindows = YerithUtils::getAllWindows();

        YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
	}
}


void YerithQTableWidgetAdminWindowsCommons::decrementFontSize__OF_TABLE__WIDGET()
{
	if (0 != _a_CURRENT_table_widget &&
		0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		if (_TABLE_FONT_SIZE > 5)
		{
			--_TABLE_FONT_SIZE;

			_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

            _a_CURRENT_table_widget->hide();

            _a_CURRENT_table_widget->resizeColumnsToContents();

            _a_CURRENT_table_widget->show();

            YerithERPWindows *allWindows = YerithUtils::getAllWindows();

            YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
		}
	}
}


void YerithQTableWidgetAdminWindowsCommons::imprimer_pdf_document_WITH_A_YERITH_PROGRESS_BAR()
{
    if (0 != _yerith_PRINT_UTILITIES_TEX_TABLE)
    {
        _yerith_PRINT_UTILITIES_TEX_TABLE->RESET_NOMBRE_DE_LIGNES_TABLEAU_STANDARD();
    }


    if (0 != _a_CURRENT_table_widget)
    {
        imprimer_UNE_LISTE_WITH_A_YERITH_PROGRESS_BAR();
    }
    else
    {
        YerithProgressBar((YerithWindowsCommons *)this)((YerithWindowsCommons *)this,
                                                        &YerithWindowsCommons::imprimer_pdf_document);
    }
}


bool YerithQTableWidgetAdminWindowsCommons::imprimer_UNE_LISTE(YerithTableWidget &a_yerith_qtable_widget,
                                                          QString           &a_yerith_qtable_widget__TexDocument_In_Out,
                                                          QString            file_name_Subject_Title_FRENCH,
                                                          QString            file_name_Subject_Title_ENGLISH,
                                                          QString            prefix_file_name_FRENCH,
                                                          QString            prefix_file_name_ENGLISH)
{
	if (a_yerith_qtable_widget.rowCount() <= 0)
	{
		YerithQMessageBox::information(this,
				QObject::tr("impression"),
				QObject::tr("Il n'y a pas de données à imprimer !"));

		return false;
	}


	YerithPOSUser *yerithUser = _allWindows->getUser();

	YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();


	QString factureDate(infoEntreprise.getVille_LATEX());

	YerithUtils::getCurrentSimplifiedDate(factureDate);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(50);


	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
        a_yerith_qtable_widget__TexDocument_In_Out
            .replace("YERITHSUBJECT",
                     file_name_Subject_Title_ENGLISH);
	}
	else
	{
        a_yerith_qtable_widget__TexDocument_In_Out
            .replace("YERITHSUBJECT",
                     file_name_Subject_Title_FRENCH);
	}


	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHPAPERSPEC",
											get_PRINTING_PARAMETER_printing_position());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHBOITEPOSTALE", infoEntreprise.getBoitePostal());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHPAYS", infoEntreprise.getPaysTex());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHDATE", factureDate);

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHHEUREVENTE", CURRENT_TIME);

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());


	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHNOMUTILISATEUR",
			QString("%1 %2")
			.arg(YerithUtils::getAllWindows()->getUser()->titre(),
					YerithUtils::getAllWindows()->getUser()->nom_completTex()));

	a_yerith_qtable_widget__TexDocument_In_Out
	.replace("YERITHSUCCURSALE",
			YerithUtils::LATEX_IN_OUT_handleForeignAccents
			(YerithERPConfig::THIS_SITE_LOCALISATION_NAME));

	a_yerith_qtable_widget__TexDocument_In_Out.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(78);


	QString prefixFileName;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        prefixFileName
            .append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                        (prefix_file_name_ENGLISH));
    }
    else
    {
        prefixFileName
            .append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir
                        (prefix_file_name_FRENCH));
    }

	//qDebug() << "++\n" << a_yerith_qtable_widget__TexDocument_In_Out;

	QFile tmpLatexFile(prefixFileName + "tex");

	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(a_yerith_qtable_widget__TexDocument_In_Out.toUtf8());
	}

	tmpLatexFile.close();

	QString pdfReceiptFileName = YerithERPProcess::compileLatex(prefixFileName);

	if (!pdfReceiptFileName.isEmpty())
	{
		YerithERPProcess::startPdfViewerProcess(pdfReceiptFileName);
	}

	emit SIGNAL_INCREMENT_PROGRESS_BAR(97);

	return true;
}


void *YerithQTableWidgetAdminWindowsCommons::imprimer_UNE_LISTE_JUSQUAU_selectionne()
{
    bool full_list = false;

    imprimer_UNE_LISTE(&full_list);
}


void YerithQTableWidgetAdminWindowsCommons::imprimer_UNE_LISTE_WITH_A_YERITH_PROGRESS_BAR()
{
    bool full_list = true;

    YerithProgressBar(this)(this,
                            &full_list,
                            &YerithQTableWidgetAdminWindowsCommons::imprimer_UNE_LISTE);
}


void YerithQTableWidgetAdminWindowsCommons::
        connect_Qtable_Widgets(YerithTableWidget &a_yerith_qtable_widget)
{
    connect(&a_yerith_qtable_widget,
            SIGNAL(clicked(const QModelIndex &)),
            this,
            SLOT(setLast_SelectedRow_Row_ID(const QModelIndex &)));

    connect(&a_yerith_qtable_widget,
            SIGNAL(pressed(const QModelIndex &)),
            this,
            SLOT(setLast_SelectedRow_Row_ID(const QModelIndex &)));
}


void YerithQTableWidgetAdminWindowsCommons::
        get_PRINT_OUT_TexTableString(YerithTableWidget &a_yerith_qtable_widget,
                                     QString           &texTable_IN_OUT,
                                     QString            Header_TEMPLATE_texTable_In,
                                     QString            FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In /* = "" */,
                                     QString            ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In /* = "" */,
                                     int                row_MAX_TO_GO_export /* = -1 */)
{
	texTable_IN_OUT.append(Header_TEMPLATE_texTable_In);

	if (YerithMainWindow::LANGUE_ANGLAISE)
	{
		texTable_IN_OUT.append(ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In);
	}
	else //FRANCAIS
	{
		texTable_IN_OUT.append(FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In);
	}


	int rowCount = a_yerith_qtable_widget.rowCount();

	int columnCount = a_yerith_qtable_widget.columnCount();


    int MAX_TABLE_MODDEL_ROW_COUNT__to_export = rowCount;

	if (row_MAX_TO_GO_export > -1)
	{
        MAX_TABLE_MODDEL_ROW_COUNT__to_export = row_MAX_TO_GO_export;
	}

	bool color_this_row_grey = true;


	int LINE_COUNT_PER_PDF_PAGE = 41;


	QString cell_text;


    int TABLE_COUNT = qFloor(rowCount / LINE_COUNT_PER_PDF_PAGE);

    int current_table_count = 0;

	int current_pdf_page_line_count = 0;

	uint Visual_ID_counter = 0;

	//Tex table body
	for (int i = 0;
         i < MAX_TABLE_MODDEL_ROW_COUNT__to_export && current_pdf_page_line_count <= LINE_COUNT_PER_PDF_PAGE;
         ++i)
	{
        color_this_row_grey = (0 == i%2);

        if (color_this_row_grey)
        {
            texTable_IN_OUT.append(QString("\\rowcolor{yerithColorGray}"));
        }
        else
        {
            texTable_IN_OUT.append(QString("\\rowcolor{white}"));
        }


        //We add a cell for row numbering wioth an ID number.
        {
            ++Visual_ID_counter;

            QTableWidgetItem *an_item_ID = new QTableWidgetItem(QString::number(Visual_ID_counter));

            if (0 != an_item_ID)
            {
                YerithUtils::handleTexTableItemText(columnCount,
                                                    texTable_IN_OUT,
                                                    0,
                                                    an_item_ID->text());
            }
        }


        for (int j = 0; j < columnCount; ++j)
        {
            QTableWidgetItem *an_item = 0;

            an_item = a_yerith_qtable_widget.item(i, j);

            if (0 != an_item)
            {
                if (j != 4)
                {
                    cell_text = an_item->text();
                }
                else
                {
                    cell_text = GET_NUM_STRING(an_item->text().toDouble());

                }
            }

            YerithUtils::handleTexTableItemText(columnCount,
                                                texTable_IN_OUT,
                                                j,
                                                cell_text);
        }

        if (i < rowCount - 1)
        {
            texTable_IN_OUT.append("\\hline\n");
        }


        ++current_pdf_page_line_count;


        if (LINE_COUNT_PER_PDF_PAGE - 1 == current_pdf_page_line_count)
        {
            current_pdf_page_line_count = 0;

            if (current_table_count < TABLE_COUNT)
            {
                texTable_IN_OUT.append("\\hline\n"
                                       "\\end{tabular}}\n"
                                       "\\end{table*}\n");

                texTable_IN_OUT.append("\\newpage\n");

                texTable_IN_OUT.append(Header_TEMPLATE_texTable_In);

                if (YerithMainWindow::LANGUE_ANGLAISE)
                {
                    texTable_IN_OUT.append(ENGLISH_QTableWidgetHeader_TEMPLATE_texTable_In);
                }
                else //FRANCAIS
                {
                    texTable_IN_OUT.append(FRENCH_QTableWidgetHeader_TEMPLATE_texTable_In);
                }
            }

            ++current_table_count;

            if (current_table_count > 0)
            {
                LINE_COUNT_PER_PDF_PAGE = 57;
            }
        }

    } //for-i


    texTable_IN_OUT.append("\\hline\n"
                           "\\end{tabular}}\n"
                           "\\end{table*}\n");

}
