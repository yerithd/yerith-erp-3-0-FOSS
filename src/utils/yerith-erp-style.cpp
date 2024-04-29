/*
 * yerith-erp-style.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */
#include "src/utils/yerith-erp-style.hpp"

#include <QtCore/QString>
#include <QtGui/QColor>

QString YerithPosStyle::getColorStyleSheetString(const QColor &aColour)
{
    return QString("QLabel { color : %1; }").arg(aColour.name());
}
