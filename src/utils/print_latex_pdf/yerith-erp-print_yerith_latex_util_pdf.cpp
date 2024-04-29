/*
 * yerith-erp-print_yerith_latex_util_pdf.cpp
 *
 *      Prof Dr Xavier Noundou
 */

#include "yerith-erp-print_yerith_latex_util_pdf.hpp"


#include "src/process/yerith-erp-process.hpp"

#include "src/utils/yerith-erp-info-entreprise.hpp"

#include "src/yerith-erp-windows.hpp"


#include <QtCore/qmath.h>


const double YerithTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT(63);

const double YerithTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE(20);


const QString YerithTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_PORTRAIT_SPECIFICATION("a4paper");

const QString YerithTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_LANDSCAPE_SPECIFICATION ("a4paper,landscape");


YerithTableViewPRINT_UTILITIES_TEX_TABLE::
	YerithTableViewPRINT_UTILITIES_TEX_TABLE(const QString 			&output_pdf_file_name_out,
											 YerithWindowsCommons 	&aYerithWindowTableOutputView,
											 YerithTableView 		&aYerithTableView)
:_LATEX_A4_PAPER_SPEC(_A4PAGE_LANDSCAPE_SPECIFICATION),
 _MAX_TABLE_ROW_COUNT(20),
 _output_pdf_file_name(output_pdf_file_name_out),
 _yerithWindowTableOutputView(&aYerithWindowTableOutputView),
 _yerithTableView(&aYerithTableView)
{
    _dbCENTER_TO_ALIGN_FieldName =
                    _yerithWindowTableOutputView->getDBFieldNamesToPrintCenterAligned();

    _dbRIGHT_TO_ALIGN_FieldName =
                    _yerithWindowTableOutputView->getDBFieldNamesToPrintRightAligned();

    _dbLEFT_TO_ALIGN_FieldName =
                    _yerithWindowTableOutputView->getDBFieldNamesToPrintLeftAligned();
}


void YerithTableViewPRINT_UTILITIES_TEX_TABLE::setYerithTableView(YerithTableView
                                                                  *
                                                                  aYerithTableView)
{
    if (0 == aYerithTableView)
    {
        return;
    }

    _yerithTableView = aYerithTableView;
}


void
YerithTableViewPRINT_UTILITIES_TEX_TABLE::
RESET_NOMBRE_DE_LIGNES_TABLEAU_STANDARD()
{
    if (YerithUtils::isEqualCaseInsensitive(_LATEX_A4_PAPER_SPEC,
                                            _A4PAGE_PORTRAIT_SPECIFICATION))
    {
        _MAX_TABLE_ROW_COUNT =
                        YerithTableViewPRINT_UTILITIES_TEX_TABLE::
                        _STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT;

    }
    else if (YerithUtils::isEqualCaseInsensitive(_LATEX_A4_PAPER_SPEC,
                                                 _A4PAGE_LANDSCAPE_SPECIFICATION))
    {
        _MAX_TABLE_ROW_COUNT =
                        YerithTableViewPRINT_UTILITIES_TEX_TABLE::
                        _STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE;
    }
}


uint YerithTableViewPRINT_UTILITIES_TEX_TABLE::MAX_TABLE_ROW_COUNT_first_page()
{
    if (YerithUtils::isEqualCaseInsensitive(_LATEX_A4_PAPER_SPEC,
                                            _A4PAGE_PORTRAIT_SPECIFICATION))
    {
        return 59;
    }
    else if (YerithUtils::isEqualCaseInsensitive(_LATEX_A4_PAPER_SPEC,
                                                 _A4PAGE_LANDSCAPE_SPECIFICATION))
    {
        return 20;
    }

    return 20;
}


void YerithTableViewPRINT_UTILITIES_TEX_TABLE::SET_VERTICAL_PRINT_POSITION()
{
    _LATEX_A4_PAPER_SPEC = _A4PAGE_PORTRAIT_SPECIFICATION;

    if (0 != _yerithWindowTableOutputView)
    {
        _yerithWindowTableOutputView->set_PRINTING_PARAMETER_printing_position(_LATEX_A4_PAPER_SPEC);
    }
}

void YerithTableViewPRINT_UTILITIES_TEX_TABLE::SET_HORIZONTAL_PRINT_POSITION()
{
    _LATEX_A4_PAPER_SPEC = _A4PAGE_LANDSCAPE_SPECIFICATION;

    if (0 != _yerithWindowTableOutputView)
    {
        _yerithWindowTableOutputView->set_PRINTING_PARAMETER_printing_position(_LATEX_A4_PAPER_SPEC);
    }
}


QString YerithTableViewPRINT_UTILITIES_TEX_TABLE::
			print_YERITH_document_from_TableView(uint 					pageFROM,
												 uint 					pageTO,
												 const QString 			&aLatex_template_document_string,
												 QMap<QString, QString> *documentSpecificElements /* = 0 */)
{
    _tex_template_document_string.clear();

    _tex_template_document_string.append(aLatex_template_document_string);


    QString latexFileNamePrefix(_output_pdf_file_name);


    QList <int>dbTableColumnsToIgnore_in_out;


    _yerithWindowTableOutputView->fill_table_columns_to_ignore(dbTableColumnsToIgnore_in_out);


    QString pdfBuyingsFileName;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	latexFileNamePrefix.append("_english");
    }
    else
    {
    	latexFileNamePrefix.append("_french");
    }


    QStandardItemModel *tableModel = _yerithTableView->getStandardItemModel();


    int tableModelRowCount = tableModel->rowCount();


    if (tableModelRowCount <= 0 										||
        tableModel->columnCount() <= dbTableColumnsToIgnore_in_out.count())
    {
        YerithQMessageBox::information(_yerithWindowTableOutputView,
                                       QObject::tr("impression"),
                                       QObject::tr("Il n'y a pas de données à imprimer !"));

        return YerithUtils::EMPTY_STRING;
    }


    int fromRowIndex = 0;

    int toRowIndex = tableModelRowCount;

    QString latexTable_in_out;

//    qDebug() << QString("number of pages to print: %1").arg(pageNumber);
//    _logger->log("imprimer_pdf_document",
//                      QString("number of pages to print: %1").arg(pageNumber));

    int currentProgressBarCount = 25;


    emit _yerithWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(currentProgressBarCount);


    if (tableModelRowCount > MAX_TABLE_ROW_COUNT_first_page())
    {
        fromRowIndex = 0;

        toRowIndex = fromRowIndex + MAX_TABLE_ROW_COUNT_first_page();

        int pageNumber =
        		qCeil((tableModelRowCount - MAX_TABLE_ROW_COUNT_first_page()) / _MAX_TABLE_ROW_COUNT);

        int k = 1;

        do
        {
            if (pageFROM <= k && k <= pageTO)
            {
                //qDebug() << QString("## fromRowIndex: %1, toRowIndex: %2")
                //          .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
                get_YERITH_TableViewListingTexDocumentString
							(dbTableColumnsToIgnore_in_out,
							 latexTable_in_out,
							 (fromRowIndex >= tableModelRowCount) ? tableModelRowCount : fromRowIndex,
							 (toRowIndex >= tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex, k == pageNumber);
            }

            fromRowIndex = toRowIndex;


            toRowIndex = (fromRowIndex >= tableModelRowCount) ?
            				(fromRowIndex + 1) :
							fromRowIndex + _MAX_TABLE_ROW_COUNT;


            currentProgressBarCount += abs((((k + 1) / pageNumber) * 100) - 4);

            currentProgressBarCount = (currentProgressBarCount > 98) ? 97 : currentProgressBarCount;

            emit _yerithWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(currentProgressBarCount);

            ++k;
        }

        while (k <= pageNumber && fromRowIndex <= toRowIndex);
    }
    else
    {
        fromRowIndex = 0;

        toRowIndex = fromRowIndex + MAX_TABLE_ROW_COUNT_first_page();

        int pageNumber = 1;

        int k = 1;

        get_YERITH_TableViewListingTexDocumentString
        	(dbTableColumnsToIgnore_in_out, latexTable_in_out,
        	 (fromRowIndex >= tableModelRowCount) ? tableModelRowCount : fromRowIndex,
        	 (toRowIndex >= tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex, k == pageNumber);
    }


    YerithInfoEntreprise &infoEntreprise = YerithUtils::getAllWindows()->getInfoEntreprise();

    QString texDocument;

    get_YERITH_ListingTex_TEMPLATE_DocumentString(texDocument,
                                                  latexTable_in_out);

    QString factureDate =
    		YerithUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX());


    YerithUtils::getCurrentLocaleDate(factureDate);


    if (0 != documentSpecificElements)
    {
        QMapIterator<QString, QString > itDocumentSpecificElements(*documentSpecificElements);

        while (itDocumentSpecificElements.hasNext())
        {
            itDocumentSpecificElements.next();

            texDocument.replace(itDocumentSpecificElements.key(),
                                itDocumentSpecificElements.value());
        }
    }


    texDocument.replace("YERITHPAPERSPEC", _LATEX_A4_PAPER_SPEC);

    texDocument.replace("YERITHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YERITHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YERITHBOITEPOSTALE", infoEntreprise.getBoitePostal());
    texDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YERITHDATE", factureDate);

    texDocument.replace("YERITHNOMUTILISATEUR",
                        QString("%1 %2")
							.arg(YerithUtils::getAllWindows()->getUser()->titre(),
                                 YerithUtils::getAllWindows()->getUser()->nom_completTex()));

    texDocument.replace("YERITHSUCCURSALE",
                        YerithUtils::LATEX_IN_OUT_handleForeignAccents
							(YerithERPConfig::THIS_SITE_LOCALISATION_NAME));

    texDocument.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);

    texDocument.replace("YERITHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YERITHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YERITHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());

    QString yerithPrefixFileName;

    yerithPrefixFileName.append
		(YerithUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

    //qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex").arg(yerithPrefixFileName));

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    												texDocument);

    emit _yerithWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(94);

    return YerithERPProcess::compileLatex(yerithPrefixFileName);
}


QString YerithTableViewPRINT_UTILITIES_TEX_TABLE::
			print_YERITH_document_from_TableView(const QString 				&aLatex_template_document_string,
												 QMap<QString, QString> 	*documentSpecificElements /* = 0 */)
{
    _tex_template_document_string.clear();

    _tex_template_document_string.append(aLatex_template_document_string);

    QString latexFileNamePrefix(_output_pdf_file_name);

    QList <int>dbTableColumnsToIgnore_in_out;

    _yerithWindowTableOutputView
		->fill_table_columns_to_ignore(dbTableColumnsToIgnore_in_out);

    QString pdfBuyingsFileName;


#ifdef YERITH_FRANCAIS_LANGUAGE
    latexFileNamePrefix.append("_french");
#endif

#ifdef YERITH_ENGLISH_LANGUAGE
    latexFileNamePrefix.append("_english");
#endif


    QStandardItemModel *tableModel = _yerithTableView->getStandardItemModel();

    int tableModelRowCount = tableModel->rowCount();

    if (tableModelRowCount <= 0 										||
        tableModel->columnCount() <= dbTableColumnsToIgnore_in_out.count())
    {
        YerithQMessageBox::information(_yerithWindowTableOutputView,
                                       QObject::tr("impression"),
                                       QObject::tr("Il n'y a pas de données à imprimer !"));

        return YerithUtils::EMPTY_STRING;
    }

    int fromRowIndex = 0;

    int toRowIndex = tableModelRowCount;

    QString latexTable_in_out;

    int pageNumber = qCeil(tableModelRowCount / _MAX_TABLE_ROW_COUNT);

//    QDEBUG_STRING_OUTPUT_2_N("pageNumber", pageNumber);
//
//    QDEBUG_STRING_OUTPUT_2_N("_MAX_TABLE_ROW_COUNT", _MAX_TABLE_ROW_COUNT);
//
//    QDEBUG_STRING_OUTPUT_2_N("tableModelRowCount", tableModelRowCount);

    get_YERITH_TableViewListingTexDocumentString(dbTableColumnsToIgnore_in_out,
                                                 latexTable_in_out,
                                                 0,
                                                 (MAX_TABLE_ROW_COUNT_first_page() >= tableModelRowCount)
												 	 ? tableModelRowCount : MAX_TABLE_ROW_COUNT_first_page(),
                                                 tableModelRowCount <= MAX_TABLE_ROW_COUNT_first_page());


    int currentProgressBarCount = abs(((2.0 / pageNumber) * 50) - 4);

    emit _yerithWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(currentProgressBarCount);

    if (tableModelRowCount >= MAX_TABLE_ROW_COUNT_first_page())
    {
        fromRowIndex = MAX_TABLE_ROW_COUNT_first_page();

        toRowIndex =
                        (fromRowIndex >=
                         tableModelRowCount) ? fromRowIndex : fromRowIndex +
                        _MAX_TABLE_ROW_COUNT;

        int k = 1;

        do
        {
            //qDebug() << QString("## fromRowIndex: %1, toRowIndex: %2")
            //          .arg(QString::number(fromRowIndex), QString::number(toRowIndex));
            get_YERITH_TableViewListingTexDocumentString
            (dbTableColumnsToIgnore_in_out, latexTable_in_out,
             (fromRowIndex >=
              tableModelRowCount) ? tableModelRowCount : fromRowIndex,
             (toRowIndex >=
              tableModelRowCount) ? (tableModelRowCount + 1) : toRowIndex,
             k == pageNumber);

            fromRowIndex = toRowIndex;

            toRowIndex =
                            (fromRowIndex >=
                             tableModelRowCount) ? (fromRowIndex + 1) : fromRowIndex +
                            _MAX_TABLE_ROW_COUNT;

            currentProgressBarCount += abs((((k + 1) / pageNumber) * 100) - 4);

            currentProgressBarCount =
                            (currentProgressBarCount > 98) ? 97 : currentProgressBarCount;

            emit _yerithWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(currentProgressBarCount);

            ++k;
        }

        while (k <= pageNumber && fromRowIndex <= toRowIndex);
    }

    YerithInfoEntreprise &infoEntreprise =
                    YerithUtils::getAllWindows()->getInfoEntreprise();

    QString texDocument;

    get_YERITH_ListingTex_TEMPLATE_DocumentString(texDocument,
                                                  latexTable_in_out);

    QString
    factureDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(factureDate);


    if (0 != documentSpecificElements)
    {
        QMapIterator < QString,
                     QString > itDocumentSpecificElements(*documentSpecificElements);

        while (itDocumentSpecificElements.hasNext())
        {
            itDocumentSpecificElements.next();
            texDocument.replace(itDocumentSpecificElements.key(),
                                itDocumentSpecificElements.value());
        }
    }


    texDocument.replace("YERITHPAPERSPEC", _LATEX_A4_PAPER_SPEC);

    texDocument.replace("YERITHENTREPRISE",
                        infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YERITHACTIVITESENTREPRISE",
                        infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YERITHBOITEPOSTALE",
                        infoEntreprise.getBoitePostal());
    texDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YERITHDATE", factureDate);

    texDocument.replace("YERITHNOMUTILISATEUR",
                        QString("%1 %2").arg(YerithUtils::getAllWindows()->
                                             getUser()->titre(),
                                             YerithUtils::getAllWindows()->
                                             getUser()->nom_completTex()));

    texDocument.replace("YERITHSUCCURSALE",
                        YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerithERPConfig::THIS_SITE_LOCALISATION_NAME));
    texDocument.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YERITHCOMPTEBANCAIRENR",
                        infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YERITHCONTRIBUABLENR",
                        infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YERITHAGENCECOMPTEBANCAIRE",
                        infoEntreprise.getAgenceCompteBancaireTex());

    QString yerithPrefixFileName;

    yerithPrefixFileName
		.append(YerithUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

    //qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex").arg(yerithPrefixFileName));

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile,
    												texDocument);

    emit _yerithWindowTableOutputView->SIGNAL_INCREMENT_PROGRESS_BAR(94);

    return YerithERPProcess::compileLatex(yerithPrefixFileName);
}


void
YerithTableViewPRINT_UTILITIES_TEX_TABLE::get_YERITH_ListingTex_TEMPLATE_DocumentString
(QString &output_tex_document_string_in_out, const QString &append_string)
{
    output_tex_document_string_in_out.clear();
    output_tex_document_string_in_out.append(_tex_template_document_string);
    output_tex_document_string_in_out.append(append_string).append("\n");
    output_tex_document_string_in_out.append("\\end{document}");
}


void YerithTableViewPRINT_UTILITIES_TEX_TABLE::
    get_YERITH_TableViewListingTexDocumentString(const QList <int> &aDBTableColumnsToIgnore_in_out,
                                                 QString &latexTable_in_out,
                                                 int fromRowIndex,
                                                 int toRowIndex,
                                                 bool lastPage)
{
    if (lastPage && toRowIndex > MAX_TABLE_ROW_COUNT_first_page())
    {
        toRowIndex -= 1;
    }

    if (fromRowIndex == toRowIndex)
    {
        return;
    }

    if (0 == fromRowIndex)
    {
        latexTable_in_out.append("\\begin{table*}[!hbp]").append("\n").
        append("\\centering").append("\n").
        append
        ("\\begin{adjustbox}{width={\\textwidth},{totalheight=\\textheight-(\\textheight/3)},keepaspectratio}").
        append("\n").append("\\begin{tabular}").append("{|");
    }
    else
    {
        latexTable_in_out.append("\\begin{table*}[!b]").append("\n").
        append("\\centering").append("\n").
        append
        ("\\begin{adjustbox}{width={\\textwidth},{totalheight=\\textheight-(\\textheight/3)},keepaspectratio}").
        append("\n").append("\\begin{tabular}").append("{|");
    }


    QStandardItemModel &tableStandardItemModel =
                    *(static_cast < QStandardItemModel * >(_yerithTableView->model()));

    int texTableColumnCount = tableStandardItemModel.columnCount() + 1;

    latexTable_in_out.append("c|");

    QList <int>headerViewModelIndex;

    int realK_pos = 0;

    //Tex table header
    for (int k = 0; k < _yerithTableView->horizontalHeader()->count(); ++k)
    {
        realK_pos = _yerithTableView->horizontalHeader()->logicalIndex(k);

        if (aDBTableColumnsToIgnore_in_out.contains(realK_pos))
        {
            continue;
        }

        headerViewModelIndex.append(realK_pos);

        if (_dbLEFT_TO_ALIGN_FieldName.contains(realK_pos))
        {
            latexTable_in_out.append("l|");
        }
        else if (_dbRIGHT_TO_ALIGN_FieldName.contains(realK_pos))
        {
            latexTable_in_out.append("r|");
        }
        else if (_dbCENTER_TO_ALIGN_FieldName.contains(realK_pos))
        {
            latexTable_in_out.append("c|");
        }
        else
        {
            latexTable_in_out.append("r|");
        }
    }

    latexTable_in_out.append("} \\hline \\xrowht{1.0em} \n");

    /** We add a column named 'id' for numbering the rows
     * in the Tex table. */
    unsigned int id = fromRowIndex + 1;

    QStandardItem *item;


    int
    max_headerViewModelIndex_count_FOR_SEPARATION_CHAR =
                    headerViewModelIndex.count();


    for (int k = 0; k < max_headerViewModelIndex_count_FOR_SEPARATION_CHAR; ++k)
    {
        latexTable_in_out.append(" & ");
    }

    latexTable_in_out.append("\\\\ \n");

    latexTable_in_out.append("\\textbf{n\\textsuperscript{o}} & ");


    for (int k = 0; k < headerViewModelIndex.count(); ++k)
    {
        realK_pos = headerViewModelIndex.at(k);

        item = tableStandardItemModel.horizontalHeaderItem(realK_pos);

        if (0 != item)
        {
            QString
            itemText(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                     (item->text()).toUpper().trimmed());

            itemText.prepend("\\textbf{").append("}");

            itemText.prepend("\\multicolumn{1}{c|}{").append("}");

            latexTable_in_out.append(itemText);

            if (k < max_headerViewModelIndex_count_FOR_SEPARATION_CHAR - 1)
            {
                latexTable_in_out.append(" &");
            }
        }
    }

    latexTable_in_out.append(" \\\\").append("\n");

    YerithUtils::cleanUpTexTableLastString(latexTable_in_out);

    for (int k = 0; k < max_headerViewModelIndex_count_FOR_SEPARATION_CHAR; ++k)
    {
        latexTable_in_out.append(" & ");
    }

    latexTable_in_out.append("\\\\ \\hline \\xrowht{0.72em} \n");

//    QDEBUG_STRING_OUTPUT_2("YERITH HEADER *", latexTable_in_out);

    /** Closing Tex table header */

//    latexTable_in_out.append(" \\\\ \\hline \\xrowht{0.72em} \n");

    //qDebug() << QString("++ fromRowIndex: %1, toRowIndex: %2")
    //                  .arg(QString::number(fromRowIndex), QString::number(toRowIndex));

    bool color_next_row = true;

    for (int j = fromRowIndex; j < toRowIndex; ++j)
    {
        /*
         * WE COLOR TABLE CELL ROW ALTERNATIVELY.
         */
        if (color_next_row)
        {
            latexTable_in_out.append("\\rowcolor{yerithColorGray} \n");

            color_next_row = false;
        }
        else
        {
            latexTable_in_out.append("\\rowcolor{white} \n");

            color_next_row = true;
        }

        latexTable_in_out.append(QString::number(id));
        latexTable_in_out.append(" &");
        ++id;

        realK_pos = 0;

        for (int k = 0; k < headerViewModelIndex.count(); ++k)
        {
            realK_pos = headerViewModelIndex.at(k);

            item = tableStandardItemModel.item(j, realK_pos);

            if (0 != item)
            {
                QString
                itemText(YerithUtils::LATEX_IN_OUT_handleForeignAccents(item->text()));

                if (YerithUtils::YERITH_YERITHGREEN_COLOR ==
                        item->foreground().color())
                {
                    itemText.prepend("\\textcolor{yerithColorGreen}{").
                    append("}");
                }
                else if (YerithUtils::YERITH_BLUE_COLOR ==
                         item->foreground().color())
                {
                    itemText.prepend("\\textcolor{yerithColorBlue}{").
                    append("}");
                }
                else if (YerithUtils::YERITH_RED_COLOR ==
                         item->foreground().color())
                {
                    itemText.prepend("\\textcolor{yerithColorRed}{").
                    append("}");
                }
                else if (YerithUtils::YERITH_YELLOW_COLOR ==
                         item->foreground().color())
                {
                    itemText.prepend("\\textcolor{yerithColorGreen}{").
                    append("}");
                }

                latexTable_in_out.append(itemText);

                if (k < max_headerViewModelIndex_count_FOR_SEPARATION_CHAR)
                {
                    latexTable_in_out.append(" &");
                }
            }
            else
            {
                if (realK_pos < tableStandardItemModel.columnCount() - 1)
                {
                    latexTable_in_out.append("\"\"").append(" &");
                }
                else
                {
                    latexTable_in_out.append("\"\"").append("\n");
                }
            }
        }

        latexTable_in_out = latexTable_in_out.trimmed();

        YerithUtils::cleanUpTexTableLastString(latexTable_in_out);


        if (j < toRowIndex - 1)
        {
            latexTable_in_out.append("\\\\ \\hline \\xrowht{0.72em} \n");
        }
        else
        {
            latexTable_in_out.append("\\\\ \\hline \n");
        }
    }

    //Removes the empty character "" from Latex output
    latexTable_in_out.replace("\"\"", YerithUtils::EMPTY_STRING);


    latexTable_in_out
        .append("\\end{tabular}\n\\end{adjustbox}")
        .append("\n")
        .append("\\end{table*}")
        .append("\n");

    latexTable_in_out.append("\\clearpage \n");

//    qDebug() << "++ latexTable_in_out in get_YERITH_TableViewListingTexDocumentString: " << latexTable_in_out;
}
