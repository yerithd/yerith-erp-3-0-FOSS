/*
 * yerith-erp-ui-loader.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_UI_LOADER_HPP_
#define SRC_YERITH_UI_LOADER_HPP_

#include <QtUiTools/QUiLoader>

class YerithUiLoader:public QUiLoader
{

public:

    inline YerithUiLoader(QObject *parent = 0)
    {
    }

    virtual inline ~YerithUiLoader()
    {
    }

    virtual QWidget *createWidget(const QString &className,
                                  QWidget *parent = 0,
                                  const QString &name = QString());

};

#endif /* SRC_YERITH_UI_LOADER_HPP_ */
