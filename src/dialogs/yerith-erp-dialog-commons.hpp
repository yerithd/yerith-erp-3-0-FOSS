/*
 * yerith-erp-dialogs-commons.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_DIALOGS_COMMONS_HPP_
#define YERITH_ERP_DIALOGS_COMMONS_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QDialog>

class YerithERPWindows;

class YerithPOSDialogCommons : public QDialog
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

    inline YerithPOSDialogCommons(QString dialogName)
    :_dialogName(dialogName)
    {
    }

    inline ~YerithPOSDialogCommons()
    {
    }

    void showAsModalDialogWithParent(QWidget &parent);

    virtual inline void show()
    {
        QDialog::show();
    }

    virtual inline void rendreInvisible()
    {
        close();
    }

    inline QString getDialogName()
    {
        return _dialogName;
    }

    /**
     * This is "pure virtual method".
     *
     * It must be implemented by all
     * sub classes.
     */
    virtual QPoint *getCurrentPosition() = 0;

    inline static void setAllWindows(YerithERPWindows *allWindows)
    {
        _allWindows = allWindows;
    }

    inline static YerithERPWindows *getAllWindows()
    {
        return _allWindows;
    }

protected:

    virtual void closeEvent(QCloseEvent *closeEvent);

    static YerithERPWindows *_allWindows;

    const QString _dialogName;
};

#endif /* YERITH_ERP_DIALOGS_COMMONS_HPP_ */
