#ifndef YERITH_ERP_KEY_EMITTER_HPP
#define YERITH_ERP_KEY_EMITTER_HPP

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QObject>

class YerithPOSKeyEmitter:public QObject
{
Q_OBJECT public:

    inline YerithPOSKeyEmitter()
    {
    }

    inline ~YerithPOSKeyEmitter()
    {
    }

public slots:
    void emitKey(Qt::Key key);
};

#endif //YERITH_ERP_KEY_EMITTER_HPP
