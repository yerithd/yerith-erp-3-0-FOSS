/*
 * yerith-erp-qmessage-box.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "src/widgets/yerith-erp-qmessage-box.hpp"

#include "src/admin/yerith-erp-admin-windows-commons.hpp"

#include <QtCore/QDebug>

#include <QtWidgets/QToolBar>

#include <QtWidgets/QMenuBar>

#include "../windows/yerith-erp-window-commons.hpp"


void YerithQMessageBox::about(YerithPOSAdminWindowsCommons *aCallingWindow,
                              QString transLatedPurposeStr, QString translatedMsg)
{
    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    QMessageBox::about(aCallingWindowToolbar,
                       QString("%1 - %2").arg(aCallingWindow->
                                              getWindowName(),
                                              transLatedPurposeStr),
                       translatedMsg);
}


QMessageBox::StandardButton YerithQMessageBox::
information(YerithPOSAdminWindowsCommons *aCallingWindow,
            QString transLatedPurposeStr, QString translatedMsg,
            StandardButtons buttons
            /* = Ok */,
            StandardButton defaultButton
            /*= NoButton*/)
{
    QToolBar *
    aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    return QMessageBox::information(aCallingWindowToolbar,
                                    QString("%1 - %2").arg(aCallingWindow->
                                                           getWindowName(),
                                                           transLatedPurposeStr),
                                    translatedMsg, buttons, defaultButton);
}


QMessageBox::StandardButton YerithQMessageBox::
question(YerithPOSAdminWindowsCommons *aCallingWindow,
         QString transLatedPurposeStr, QString translatedMsg,
         StandardButtons buttons
         /* = Ok */,
         StandardButton defaultButton
         /*= NoButton*/)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
       .arg(COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0));
       qDebug() << "++ stylesheet: " << stylesheet; */

    QToolBar *
    aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    return QMessageBox::question(aCallingWindowToolbar,
                                 QString("%1 - %2").arg(aCallingWindow->
                                                        getWindowName(),
                                                        transLatedPurposeStr),
                                 translatedMsg, buttons, defaultButton);
}


QMessageBox::StandardButton YerithQMessageBox::
warning(YerithPOSAdminWindowsCommons *aCallingWindow,
        QString transLatedPurposeStr, QString translatedMsg,
        StandardButton defaultButton
        /* = Ok */)
{
    QToolBar *
    aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    return QMessageBox::warning(aCallingWindowToolbar,
                                QString("%1 - %2").arg(aCallingWindow->
                                                       getWindowName(),
                                                       transLatedPurposeStr),
                                translatedMsg);
}


void YerithQMessageBox::about(YerithWindowsCommons *aCallingWindow,
                              QString transLatedPurposeStr, QString translatedMsg)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
       .arg(COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0));
       qDebug() << "++ stylesheet: " << stylesheet; */

    QToolBar *aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    QMessageBox::about(aCallingWindowToolbar,
                       QString("%1 - %2").arg(aCallingWindow->
                                              getWindowName(),
                                              transLatedPurposeStr),
                       translatedMsg);
}


QMessageBox::StandardButton YerithQMessageBox::
information(YerithWindowsCommons *aCallingWindow,
            QString transLatedPurposeStr, QString translatedMsg,
            StandardButtons buttons
            /* = Ok */,
            StandardButton defaultButton
            /*= NoButton*/)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
       .arg(COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0));
       qDebug() << "++ stylesheet: " << stylesheet; */

    QToolBar *
    aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    return QMessageBox::information(aCallingWindowToolbar,
                                    QString("%1 - %2").arg(aCallingWindow->
                                                           getWindowName(),
                                                           transLatedPurposeStr),
                                    translatedMsg, buttons, defaultButton);
}


QMessageBox::StandardButton YerithQMessageBox::
question(YerithWindowsCommons *aCallingWindow, QString transLatedPurposeStr,
         QString translatedMsg, StandardButtons buttons
         /* = Ok */,
         StandardButton defaultButton
         /*= NoButton*/)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
       .arg(COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0));
       qDebug() << "++ stylesheet: " << stylesheet; */

    QToolBar *
    aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    return QMessageBox::question(aCallingWindowToolbar,
                                 QString("%1 - %2").arg(aCallingWindow->
                                                        getWindowName(),
                                                        transLatedPurposeStr),
                                 translatedMsg, buttons, defaultButton);
}


QMessageBox::StandardButton YerithQMessageBox::warning(YerithWindowsCommons *
                                                       aCallingWindow,
                                                       QString
                                                       transLatedPurposeStr,
                                                       QString translatedMsg)
{
    /*QString stylesheet(QString("QMessageBox {background-color: rgb(%1);}")
       .arg(COLOUR_RGB_STRING_YERITH_ORANGE_243_162_0));
       qDebug() << "++ stylesheet: " << stylesheet; */

    QToolBar *
    aCallingWindowToolbar = aCallingWindow->getQMainWindowToolBar();

    aCallingWindowToolbar->setStyleSheet(aCallingWindow->
                                         qMessageBoxStyleSheet());

    return QMessageBox::warning(aCallingWindowToolbar,
                                QString("%1 - %2").arg(aCallingWindow->
                                                       getWindowName(),
                                                       transLatedPurposeStr),
                                translatedMsg);
}
