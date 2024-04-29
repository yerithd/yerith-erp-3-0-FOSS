/*
 * yerith-erp-progress-bar.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-progress-bar.hpp"


#include "src/admin/yerith-erp-admin-windows-commons.hpp"


#include "src/windows/yerith-erp-window-commons.hpp"


YerithProgressBar::YerithProgressBar(YerithPOSAdminWindowsCommons *aYerithParentAdminWindow,
                                     QString PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec /* = "rgb(0, 255, 70)" */)
:QProgressBar((QWidget *) aYerithParentAdminWindow),
 _setup(false)
{
    setVisible(false);

    QString RGB_COLOR_TO_DISPLAY_AT_YERITH_PROGRESS_BAR_BACKGROUND
				= COLOUR_RGB_STRING_YERITH_FIREBRICK_RED_255_48_48;

    if (0 != aYerithParentAdminWindow)
    {
        QToolBar *aYerithCommonWindowToolBar =
        		aYerithParentAdminWindow->getQMainWindowToolBar();

        if (0 != aYerithCommonWindowToolBar)
        {
            const QColor &aQColor = aYerithCommonWindowToolBar->palette().color(QPalette::Button);

            RGB_COLOR_TO_DISPLAY_AT_YERITH_PROGRESS_BAR_BACKGROUND =
                            QString("%1, %2, %3").arg(QString::number(aQColor.red()),
                                                      QString::number(aQColor.green()),
                                                      QString::number(aQColor.blue()));
        }
    }

    setStyleSheet(QString("QProgressBar "
                          "{text-align: center; "
                          "border: 2px solid white; "
                          "border-radius: 1px; color: %1;}"
                          "QProgressBar::chunk {"
                          "background-color: rgb(%2); "
                          "color: rgb(%3); "
                          "width: 3px;}")
    				.arg(PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec,
                         RGB_COLOR_TO_DISPLAY_AT_YERITH_PROGRESS_BAR_BACKGROUND,
    					 COLOUR_RGB_STRING_YERITH_DARK_GREEN_47_67_67));

    int width = qCeil(((QMainWindow *) parent())->width() / 2.0);

    int X = qCeil(width / 2.0);

    int Y = qCeil(((QMainWindow *) parent())->height() / 2.0);

    setFixedWidth(width);

    move(X, Y);
}


YerithProgressBar::YerithProgressBar(YerithWindowsCommons *aYerithParentWindow,
                                     QString PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec /* '= "rgb(0, 0, 0)" */)
:QProgressBar((QWidget *) aYerithParentWindow),
 _setup(false)
{
    setVisible(false);

    QString RGB_COLOR_TO_DISPLAY_AT_YERITH_PROGRESS_BAR_BACKGROUND
				= COLOUR_RGB_STRING_YERITH_FIREBRICK_RED_255_48_48;

    if (0 != aYerithParentWindow)
    {
        QToolBar *aYerithCommonWindowToolBar =
        		aYerithParentWindow->getQMainWindowToolBar();

        if (0 != aYerithCommonWindowToolBar)
        {
            const QColor &aQColor =
            		aYerithCommonWindowToolBar->palette().color(QPalette::Button);

            RGB_COLOR_TO_DISPLAY_AT_YERITH_PROGRESS_BAR_BACKGROUND =
                            QString("%1, %2, %3").arg(QString::number(aQColor.red()),
                                                      QString::number(aQColor.green()),
                                                      QString::number(aQColor.blue()));
        }
    }

    setStyleSheet(QString("QProgressBar "
                          "{text-align: center; "
                          "border: 2px solid white; "
                          "border-radius: 1px; color: %1;}"
                          "QProgressBar::chunk {"
                          "background-color: rgb(%2); "
                          "color: rgb(%3); "
                          "width: 3px;}")
    				.arg(PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec,
                         RGB_COLOR_TO_DISPLAY_AT_YERITH_PROGRESS_BAR_BACKGROUND,
    					 COLOUR_RGB_STRING_YERITH_DARK_GREEN_47_67_67));

    int width = qCeil(((QMainWindow *) parent())->width() / 2.0);

    int X = qCeil(width / 2.0);

    int Y = qCeil(((QMainWindow *) parent())->height() / 2.0);

    setFixedWidth(width);

    move(X, Y);
}



