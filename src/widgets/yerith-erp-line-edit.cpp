/*
 * yerith-erp-line-edit.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-line-edit.hpp"


#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"


#include <QtWidgets/QInputDialog>

#include <QtWidgets/QCompleter>

#include <QtCore/QDebug>


YerithLineEdit::YerithLineEdit(QWidget *parent /* = 0 */)
    :QLineEdit(parent),
    _show_ASSET(true),
    _firstTimeStyleSheetCheck(true),
    _forSearch(false),
    _inputDialog(0),
    _wasMissingRequiredText(false),
    _sqlTableModel(0),
    _sqlTableModelView(0),
    _searchQCompleter(0)
{
    setMaxLength(255);

    setValidator(&YerithUtils::STRING_FOR_YERITH_ERP_3_0_VALIDATOR);

    _aDefaultStyleSheet =
                    QString("QInputDialog {background-color: rgb(%1); color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_DARK_GREEN_47_67_67,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);

    _inputDialog = new QInputDialog;

    _inputDialog->setWindowTitle(YerithUtils::EMPTY_STRING);

    _inputDialog->setLabelText(QObject::tr("contenu de ce champs de texte"));

    _inputDialog->setTextEchoMode(QLineEdit::Normal);

    _inputDialog->setOption(QInputDialog::NoButtons);

    _originalPlaceHolderText = placeholderText();

    _originalPaletteBeforeMissingInformation = palette();


    setYerithERPQLineEditDisplayFormat();
}


YerithLineEdit::~YerithLineEdit()
{
    if (0 != _inputDialog)
    {
        delete _inputDialog;
    }
}


void YerithLineEdit::enableForSearch(QString placeHoderText)
{
    _forSearch = true;

    setPlaceholderText(placeHoderText);

    _originalPlaceHolderText = placeHoderText;
}

void YerithLineEdit::disableForSearch()
{
    _forSearch = false;

    setPlaceholderText(YerithUtils::EMPTY_STRING);
}

bool YerithLineEdit::checkField(const QPalette *aPalette /* = 0 */)
{
    if (text().isEmpty())
    {
        _wasMissingRequiredText = true;

        setText(YerithUtils::EMPTY_STRING);

        setPlaceholderText(QObject::tr("[* champ de texte obligatoire]"));

        if (0 != aPalette)
        {
            setPalette(*aPalette);
        }
        else
        {
            setPalette(YerithUtils::YERITH_RED_COLOR);
        }

        return false;
    }

    setPlaceholderText(_originalPlaceHolderText);

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}

bool YerithLineEdit::checkFieldAndMinimalValue(double min,
                                               QPalette *aPalette /* = 0 */)
{
    if (text().isEmpty() || text().toDouble() < min)
    {
        _wasMissingRequiredText = true;

        if (0 != aPalette)
        {
            setPalette(*aPalette);
        }
        else
        {
            setPalette(YerithUtils::YERITH_RED_COLOR);
        }

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}


void YerithLineEdit::setYerithERPQLineEditDisplayFormat()
{
    if (isReadOnly())
    {
        bool isLeftAligned = ((Qt::AlignLeft | alignment()) == 1);

        /*
         * Read-only text must be horizontally centered.
         * We don't modify already centered or
         * right-aligned text.
         */
        if (isLeftAligned)
        {
            setAlignment(Qt::AlignHCenter);
        }
    }
    else
    {
        setAlignment(Qt::AlignLeft);
    }

    if (!_forSearch)
    {
        if (isReadOnly())
        {
            setPalette(YerithUtils::YERITH_BLACK_PALETTE);
        }
        else
        {
            setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        }
    }

    YerithUtils::yerithSetWidgetColor(this);
}


void YerithLineEdit::clearQCompleter()
{
    clearQCompleterText();

    if (0 != _searchQCompleter)
    {
        delete _searchQCompleter;

        _searchQCompleter = 0;
    }
}


void YerithLineEdit::clearField()
{
    QLineEdit::clear();

    setPlaceholderText(_originalPlaceHolderText);

    setPalette(_originalPaletteBeforeMissingInformation);

    if (_wasMissingRequiredText)
    {
        _wasMissingRequiredText = false;
    }

    setYerithERPQLineEditDisplayFormat();
}


void YerithLineEdit::setYerithEnabled(bool enabled)
{
    QLineEdit::setReadOnly(!enabled);

    setYerithERPQLineEditDisplayFormat();
}

void YerithLineEdit::clearQCompleterText()
{
    _lastQCompleterText = text();
    myClear();
}

void YerithLineEdit::isInputValid()
{
    if (text().isEmpty())
    {
        return;
    }
}


QString YerithLineEdit::text(int truncate_pos /* = -1 */) const
{
    QString a_truncated_string(QLineEdit::text().trimmed());

    if (truncate_pos < 0 || a_truncated_string.length() <= truncate_pos)
    {
        return a_truncated_string;
    }

    a_truncated_string.truncate(truncate_pos - 1);

    return a_truncated_string.append(YerithUtils::DOT_CHAR);
}


void YerithLineEdit::myClear()
{
    QLineEdit::clear();
    QLineEdit::setText(YerithUtils::EMPTY_STRING);
}


void YerithLineEdit::setupMyQCompleter(QString sqlTableName,
                                       int listColumnIndex,
                                       enum Qt::SortOrder sortOrder,
                                       bool truncateString /* = true */)
{
    _sqlTableModel = new YerithSqlTableModel(sqlTableName);
    _sqlTableModel->yerithSetSort(listColumnIndex, sortOrder);

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);
    _sqlTableModelView->lister(*_sqlTableModel, truncateString);

    _searchQCompleter = new QCompleter(this);

    _searchQCompleter->setModel(_sqlTableModelView->getStandardItemModel());
    _searchQCompleter->setCompletionColumn(listColumnIndex);
    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(returnPressed()), this,
            SLOT(clearQCompleterText()));
}


void YerithLineEdit::setupMyStaticQCompleter(QString        sqlTableName,
                                             const QString  fieldName,
                                             bool           returnPressedSignalActivated /* = true */,
                                             QString        aQSqlConditionStr /* = YerithUtils::EMPTY_STRING */,
                                             Qt::MatchFlags a_filter_mode /* = Qt::MatchStartsWith */)
{
    YerithUtils::getColumnListString(_currentStaticStringList,
                                     sqlTableName,
                                     fieldName,
                                     aQSqlConditionStr);

    if (0 != _searchQCompleter)
    {
        delete _searchQCompleter;
    }

    _searchQCompleter = new QCompleter(_currentStaticStringList, this);


    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);

    _searchQCompleter->setFilterMode(a_filter_mode);

    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);


    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(editingFinished()), this, SLOT(isInputValid()));

    if (returnPressedSignalActivated)
    {
        connect(this,
                SIGNAL(returnPressed()),
                this,
                SLOT(clearQCompleterText()));
    }
    else
    {
        disconnect(this,
                   SIGNAL(returnPressed()),
                   this,
                   SLOT(clearQCompleterText()));
    }
}


void YerithLineEdit::setupMyQCompleterALL(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    if (!_show_ASSET)
    {
        _sqlTableModel->setFilter(QString(" (%1 > 0) ").arg
                                  (YerithDatabaseTableColumn::PRIX_VENTE));

    }
    else
    {
        _sqlTableModel->resetFilter();
    }

    _stockNameToStockID.clear();

    _sqlTableModelView->lister_ALL(*_sqlTableModel, _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator < QString, QString > it(_stockNameToStockID);

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(returnPressed()), this,
            SLOT(clearQCompleterText()));
}


void YerithLineEdit::setupMyQCompleterFEFO(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    if (!_show_ASSET)
    {
        _sqlTableModel->setFilter(QString(" (%1 > 0) ").arg
                                  (YerithDatabaseTableColumn::PRIX_VENTE));

    }
    else
    {
        _sqlTableModel->resetFilter();
    }

    _sqlTableModelView->lister_FEFO(*_sqlTableModel, _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator < QString, QString > it(_stockNameToStockID);

    //qDebug() << "++ YerithLineEdit::setupMyQCompleterDPF_DPO, size articleDesignations: "
    //<< articleDesignations.size();

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    //qDebug() << "++ qlineedit. articleDesignations: " << articleDesignations;

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(returnPressed()), this,
            SLOT(clearQCompleterText()));
}


void YerithLineEdit::setupMyQCompleterFIFO(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    if (!_show_ASSET)
    {
        _sqlTableModel->setFilter(QString(" (%1 > 0) ").arg
                                  (YerithDatabaseTableColumn::PRIX_VENTE));

    }
    else
    {
        _sqlTableModel->resetFilter();
    }

    _sqlTableModelView->lister_FIFO(*_sqlTableModel, _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator < QString, QString > it(_stockNameToStockID);

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(returnPressed()), this,
            SLOT(clearQCompleterText()));
}


void YerithLineEdit::setupMyQCompleterLIFO(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    if (!_show_ASSET)
    {
        _sqlTableModel->setFilter(QString(" (%1 > 0) ").arg
                                  (YerithDatabaseTableColumn::PRIX_VENTE));

    }
    else
    {
        _sqlTableModel->resetFilter();
    }

    _sqlTableModelView->lister_LIFO(*_sqlTableModel, _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator < QString, QString > it(_stockNameToStockID);

    //qDebug() << "++ YerithLineEdit::setupMyQCompleterLIFO, size articleDesignations: "
    //   << articleDesignations.size();

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(returnPressed()), this,
            SLOT(clearQCompleterText()));
}


void YerithLineEdit::setupMyQCompleterCodebarALL(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_ALL(*_sqlTableModel,
                                           _stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator < QString, QString > it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerithLineEdit::setupMyQCompleterCodebarFEFO(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_FEFO(*_sqlTableModel,
                                            _stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator < QString, QString > it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerithLineEdit::setupMyQCompleterCodebarFIFO(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_FIFO(*_sqlTableModel,
                                            _stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator < QString, QString > it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerithLineEdit::setupMyQCompleterCodebarLIFO(QString sqlTableName)
{
    if (0 != _sqlTableModel)
    {
        delete _sqlTableModel;
    }

    if (0 != _sqlTableModelView)
    {
        delete _sqlTableModelView;
    }

    _sqlTableModelView = new YerithTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerithSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_LIFO(*_sqlTableModel,
                                            _stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator < QString, QString > it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerithLineEdit::refreshCodebarCompleterList(QString strategy)
{
    //qDebug() << "++ test refresh: " << _sqlTableModelView;
    _stockNameToStockID.clear();

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL, strategy))
    {
        _sqlTableModelView->lister_codebar_ALL(*_sqlTableModel,
                                               _stockNameToStockID);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, strategy))
    {
        _sqlTableModelView->lister_codebar_FIFO(*_sqlTableModel,
                                                _stockNameToStockID);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, strategy))
    {
        _sqlTableModelView->lister_codebar_LIFO(*_sqlTableModel,
                                                _stockNameToStockID);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO, strategy))
    {
        _sqlTableModelView->lister_codebar_FEFO(*_sqlTableModel,
                                                _stockNameToStockID);
    }
}


void YerithLineEdit::refreshCompleterList(QString strategy)
{
    //qDebug() << "++ test refresh: " << _sqlTableModelView;
    _stockNameToStockID.clear();

    if (YerithUtils::isEqualCaseInsensitive
            (YerithERPConfig::STRATEGIE_VENTE_SORTIE_ALL, strategy))
    {
        _sqlTableModelView->lister_ALL(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, strategy))
    {
        _sqlTableModelView->lister_FIFO(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, strategy))
    {
        _sqlTableModelView->lister_LIFO(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (YerithERPConfig::STRATEGIE_VENTE_SORTIE_FEFO, strategy))
    {
        _sqlTableModelView->lister_FEFO(*_sqlTableModel, _stockNameToStockID);
    }
}


void YerithLineEdit::hideEvent(QHideEvent *hideEvent)
{
    if (0 != _inputDialog)
    {
        _inputDialog->close();
    }
}


void YerithLineEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (text().isEmpty() || !isReadOnly())
    {
        return;
    }

    if (_firstTimeStyleSheetCheck)
    {
        QString aStyleSheet(styleSheet());

        if (!aStyleSheet.isEmpty())
        {
            _inputDialog->setStyleSheet(aStyleSheet);
        }
        else
        {
            _inputDialog->setStyleSheet(_aDefaultStyleSheet);
        }

        _firstTimeStyleSheetCheck = false;
    }

    _inputDialog->setTextValue(text());

    _inputDialog->show();
}
