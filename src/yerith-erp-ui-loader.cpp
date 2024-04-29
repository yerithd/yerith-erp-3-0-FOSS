/*
 * yerith-erp-ui-loader.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-ui-loader.hpp"

#include "src/widgets/yerith-erp-push-button.hpp"

QWidget *YerithUiLoader::createWidget(const QString &className,
                                      QWidget *parent /* = 0 */,
                                      const QString &name /* = QString() */)
{
    if (className == "YerithPushButton")
    {
        YerithPushButton *w = new YerithPushButton(parent);
        w->setObjectName(name);
        return w;
    }

    return QUiLoader::createWidget(className, parent, name);
}
