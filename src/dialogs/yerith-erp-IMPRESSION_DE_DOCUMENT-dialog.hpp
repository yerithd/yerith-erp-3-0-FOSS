/*
 * yerith-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_IMPRESSION_DE_DOCUMENT_DIALOG_HPP_
#define YERITH_ERP_IMPRESSION_DE_DOCUMENT_DIALOG_HPP_

#include "../../ui_yerith-erp-imprimer-dialog.h"

#include "yerith-erp-dialog-commons.hpp"


#include "src/utils/yerith-erp-utils.hpp"


class QPoint;

class YerithERPWindows;

class YerithWindowsCommons;

class YerithTableViewPRINT_UTILITIES_TEX_TABLE;


class YerithIMPRESSION_DE_DOCUMENT_Dialog : public YerithPOSDialogCommons,
    										public Ui_YerithIMPRESSION_DE_DOCUMENT_Dialog
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithIMPRESSION_DE_DOCUMENT_Dialog();

    virtual inline ~YerithIMPRESSION_DE_DOCUMENT_Dialog()
    {
        delete _currentPosition;
    }

    virtual QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

public:

    void setupLineEdits();

public slots:

    virtual void show();

    virtual inline void rendreInvisible()
    {
        setVisible(false);
    }

private slots:

    void SET_NOMBRE_DE_LIGNES_PAR_DEFAUT_after_RADIOBUTTON_TOGGLE();

    void SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();

    void enable_MAX_TABLE_ROW_COUNT();

    void SET_CURRENT_WINDOW_TO_PRINT_PRINTING_PARAMETERS_AND_POSITION(int pageFROM,
                                                                      int pageTO);

    void valider();

    inline void annuler()
    {
        rendreInvisible();
    }

public:

    static YerithWindowsCommons *_current_window_to_print;

    static YerithTableViewPRINT_UTILITIES_TEX_TABLE *_yerith_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE;

protected:

    virtual inline void closeEvent(QCloseEvent *closeEvent)
    {
        YerithPOSDialogCommons::closeEvent(closeEvent);
    }

private:

    static const QString DIALOG_WINDOW_TITLE;

    QPoint *_currentPosition;
};


#endif /* YERITH_ERP_IMPRESSION_DE_DOCUMENT_DIALOG_HPP_ */
