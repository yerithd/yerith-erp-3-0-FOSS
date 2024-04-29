/*
 * yerith-erp-qmessage-box.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_QMESSAGE_BOX_HPP_
#define YERITH_ERP_QMESSAGE_BOX_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QMessageBox>

class QToolBar;
class YerithPOSAdminWindowsCommons;
class YerithWindowsCommons;

class YerithQMessageBox:public QMessageBox
{

public:

    YERITH_CLASS_OPERATORS inline YerithQMessageBox(QWidget *parent = 0):QMessageBox
        (parent)
    {
    }

    inline ~YerithQMessageBox()
    {
    }

    static void about(YerithPOSAdminWindowsCommons *aCallingWindow,
                      QString transLatedPurposeStr, QString translatedMsg);

    static QMessageBox::StandardButton information(YerithPOSAdminWindowsCommons
                                                   * aCallingWindow,
                                                   QString
                                                   transLatedPurposeStr,
                                                   QString translatedMsg,
                                                   StandardButtons buttons =
                                                                   Ok,
                                                   StandardButton defaultButton
                                                   = NoButton);

    static QMessageBox::StandardButton question(YerithPOSAdminWindowsCommons *
                                                aCallingWindow,
                                                QString transLatedPurposeStr,
                                                QString translatedMsg,
                                                StandardButtons buttons =
                                                                Ok,
                                                StandardButton defaultButton =
                                                                NoButton);

    static QMessageBox::StandardButton warning(YerithPOSAdminWindowsCommons *
                                               aCallingWindow,
                                               QString transLatedPurposeStr,
                                               QString translatedMsg,
                                               StandardButton defaultButton =
                                                               Ok);


    static void about(YerithWindowsCommons *aCallingWindow,
                      QString transLatedPurposeStr, QString translatedMsg);

    static QMessageBox::StandardButton information(YerithWindowsCommons *
                                                   aCallingWindow,
                                                   QString
                                                   transLatedPurposeStr,
                                                   QString translatedMsg,
                                                   StandardButtons buttons =
                                                                   Ok,
                                                   StandardButton defaultButton
                                                   = NoButton);

    static QMessageBox::StandardButton question(YerithWindowsCommons *
                                                aCallingWindow,
                                                QString transLatedPurposeStr,
                                                QString translatedMsg,
                                                StandardButtons buttons =
                                                                Ok,
                                                StandardButton defaultButton =
                                                                NoButton);

    static QMessageBox::StandardButton warning(YerithWindowsCommons *
                                               aCallingWindow,
                                               QString transLatedPurposeStr,
                                               QString translatedMsg);
};


#endif /* YERITH_ERP_QMESSAGE_BOX_HPP_ */
