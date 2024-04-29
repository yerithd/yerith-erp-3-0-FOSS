/*
 * yerith-erp-line-edit.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_LINE_EDIT_HPP_
#define SRC_WIDGETS_YERITH_LINE_EDIT_HPP_

#include "src/utils/yerith-erp-utils.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/widgets/table-view/yerith-erp-table-view.hpp"


#include <QtCore/QStringList>

#include <QtWidgets/QLineEdit>


class QCompleter;
class QInputDialog;

class YerithUtils;
class YerithSqlTableModel;
class YerithTableView;


class YerithLineEdit : public QLineEdit
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithLineEdit(QWidget *parent = 0);

    virtual ~YerithLineEdit();

    inline bool isEmpty()
    {
        return text().isEmpty();
    }

    void enableForSearch(QString placeHoderText);

    void disableForSearch();

    bool checkField(const QPalette *aPalette = 0);

    bool checkFieldAndMinimalValue(double min, QPalette *aPalette = 0);

    inline void setRed()
    {
        setPalette(YerithUtils::YERITH_RED_PALETTE);
    }

    void setYerithERPQLineEditDisplayFormat();

    void clearQCompleter();

    void clearField();

    void setYerithEnabled(bool enabled);

    inline QString text_LATEX(int truncate_pos = -1) const
    {
        return YerithUtils::LATEX_IN_OUT_handleForeignAccents(text(truncate_pos));
    }

    inline void setText(const QString &text)
    {
        QLineEdit::setText(QString(text.trimmed())
        					 .remove(YerithUtils::STAR_CHAR));
    }

    QString text(int truncate_pos = -1) const;

    inline QString getLastQCompleterText()
    {
        return _lastQCompleterText;
    }

    void myClear();

    inline bool set_show_ASSET()
    {
        return _show_ASSET;
    }

    inline void set_show_ASSET(bool a_show_ASSET)
    {
        _show_ASSET = a_show_ASSET;
    }

    void setupMyQCompleter(QString 				sqlTableName,
                           int 					listColumnIndex,
                           enum Qt::SortOrder 	sortOrder,
                           bool 				truncateString = true);

    void setupMyStaticQCompleter(QString 	    sqlTableName,
                                 QString 	    fieldName,
                                 bool 		    returnPressedSignalActivated = true,
                                 QString 	    aQSqlConditionStr = YerithUtils::EMPTY_STRING,
                                 Qt::MatchFlags a_filter_mode = Qt::MatchStartsWith);

    inline void setupMyStaticQCompleter(QString         sqlTableName,
                                        QString         fieldName,
                                        QString         aQSqlConditionStr,
                                        Qt::MatchFlags  a_filter_mode = Qt::MatchStartsWith)
    {
        setupMyStaticQCompleter(sqlTableName,
                                fieldName,
								true,
								aQSqlConditionStr,
								a_filter_mode);
    }

    void setupMyQCompleterALL(QString sqlTableName);

    void setupMyQCompleterFEFO(QString sqlTableName);

    void setupMyQCompleterFIFO(QString sqlTableName);

    void setupMyQCompleterLIFO(QString sqlTableName);

    void setupMyQCompleterCodebarALL(QString sqlTableName);

    void setupMyQCompleterCodebarFEFO(QString sqlTableName);

    void setupMyQCompleterCodebarFIFO(QString sqlTableName);

    void setupMyQCompleterCodebarLIFO(QString sqlTableName);


    inline QMap<QString, QString> &getStockNameToStockID()
    {
        return _stockNameToStockID;
    }

    inline QMap<QString, QString> &getStockreferenceCodebarToStockID()
    {
        return _stockReferenceToStockID;
    }


    void refreshCodebarCompleterList(QString strategy);

    void refreshCompleterList(QString strategy);

    inline QCompleter *getMyQCompleter()
    {
        return _searchQCompleter;
    }

    inline YerithSqlTableModel *getMySqlTableModel()
    {
        return _sqlTableModel;
    }

    inline YerithTableView *getMyTableView()
    {
        return _sqlTableModelView;
    }

    inline QStringList getCurrentStaticStringList()
    {
        return _currentStaticStringList;
    }

protected:

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

public slots:

    void clearQCompleterText();

    void isInputValid();

private:

    bool _show_ASSET;

    bool _firstTimeStyleSheetCheck;

    bool _forSearch;

    bool _wasMissingRequiredText;

    QInputDialog *_inputDialog;

    QString _aDefaultStyleSheet;

    QString _originalPlaceHolderText;

    QPalette _originalPaletteBeforeMissingInformation;

    YerithSqlTableModel *_sqlTableModel;

    YerithTableView *_sqlTableModelView;

    QCompleter *_searchQCompleter;

    QMap<QString, QString> _stockNameToStockID;

    QMap<QString, QString> _stockReferenceToStockID;

    QStringList _currentStaticStringList;

    QString _lastQCompleterText;
};

#endif /* SRC_WIDGETS_YERITH_LINE_EDIT_HPP_ */
