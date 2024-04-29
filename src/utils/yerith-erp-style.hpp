/*
 * yerith-erp-style.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_3_0_STYLE_HPP_
#define SRC_YERITH_ERP_3_0_STYLE_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <cstdlib>

class QColor;
class QString;

class YerithPosStyle
{
public:

    YERITH_CLASS_OPERATORS
    static QString getColorStyleSheetString(const QColor &aColour);

private:

    YerithPosStyle();

    ~YerithPosStyle();
};

#endif /* SRC_YERITH_ERP_3_0_STYLE_HPP_ */
