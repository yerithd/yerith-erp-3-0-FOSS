#include "yerith-erp-key-emitter.hpp"

#include <QtCore/QCoreApplication>

#include <QtGui/QGuiApplication>
#include <QtGui/QKeyEvent>

#include <QtQuick/QQuickItem>


void YerithPOSKeyEmitter::emitKey(Qt::Key key)
{
    QQuickItem *receiver =
                    qobject_cast < QQuickItem * >(QGuiApplication::focusObject());

    if (!receiver)
    {
        qDebug() << "++ YerithPOSKeyEmitter::emitKey(Qt::Key) | no receiver.";
        return;
    }

    QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier,
                                     QKeySequence(key).toString());

    QKeyEvent releaseEvent =
                    QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);

    QCoreApplication::sendEvent(receiver, &pressEvent);

    QCoreApplication::sendEvent(receiver, &releaseEvent);
}
