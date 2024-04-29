/*
 * yerith-erp-text-edit.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_TEXT_EDIT_HPP_
#define SRC_WIDGETS_YERITH_TEXT_EDIT_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include <QtWidgets/QTextEdit>

class YerithUtils;

class YerithTextEdit:public QTextEdit
{
public:

    YERITH_CLASS_OPERATORS inline YerithTextEdit(QWidget *parent = 0):QTextEdit(parent),
        _originalPaletteBeforeMissingInformation(palette
                                                 ())
    {
        setYerithPOSTextEditDisplayFormat();
    }


    inline YerithTextEdit(const QString &text,
                          QWidget *parent):QTextEdit(text, parent),
        _originalPaletteBeforeMissingInformation(palette())
    {
        setYerithPOSTextEditDisplayFormat();
    }

    inline ~YerithTextEdit()
    {
    }

    bool checkField();

    void setYerithPOSTextEditDisplayFormat();

    void setVisible(bool visible);

    void clearField();

    void setYerithEnabled(bool enabled);

    QString toPlainTextForLatex() const;

    inline QString toPlainText() const
    {
        return QTextEdit::toPlainText().trimmed();
    }

private:

    QPalette _originalPaletteBeforeMissingInformation;
};


#endif /* SRC_WIDGETS_YERITH_TEXT_EDIT_HPP_ */
