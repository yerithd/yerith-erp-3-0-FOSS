/*
 * yerith-erp-qcheckbox.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */


#include "yerith-erp-qcheckbox.hpp"


#include <QtCore/QDebug>



void YerithERPQCheckBox::setReadOnly(bool isReadOnly)
{
    if (isReadOnly)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        setFocusPolicy(Qt::NoFocus);
    }
    else
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        setFocusPolicy(Qt::StrongFocus);
    }
}


void YerithERPQCheckBox::
		setYerithChecked(bool checked, const QString &text /*= YerithUtils::EMPTY_STRING*/)
{
    if (!YerithUtils::isEqualCaseInsensitive(text,
    										 YerithUtils::EMPTY_STRING))
    {
        setText(text);
    }

    setChecked(checked);
}


void YerithERPQCheckBox::setYerithEnabled(bool isEnabled)
{
    if (isEnabled)
    {
        setReadOnly(false);
        setEnabled(true);
    }
    else
    {
        setReadOnly(true);
        setEnabled(false);
    }
}



