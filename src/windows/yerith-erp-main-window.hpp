/*
 * yerith-erp-main-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_MAIN_WINDOW_HPP_
#define SRC_YERITH_MAIN_WINDOW_HPP_

#include "../../ui_yerith-erp-main-window.h"

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "yerith-erp-window-commons.hpp"


#include <tuple>

#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>


class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;
class YerithUtils;
class YerithWindowsCommons;
class YerithPOSUser;

class YerithMainWindow : public YerithWindowsCommons,
						 public Ui_YerithMainWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithMainWindow();

    inline ~YerithMainWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_mainWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirAdministrateur();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void setup_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    void write_AFTER_SETUP_YERITH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    inline static QString GET_YERITH_ERP_WINDOW_TITLE()
    {
    	return YerithMainWindow::LANGUE_ANGLAISE ?
    			QString("YERITH-ERP-3.0") :
				QString("Yerith-pgi-3.0");
    }

    inline static QString get_TRANSLATED_WindowName(const char *WINDOW_TITLE)
    {
        return QString("%1 - %2")
        			.arg(YerithMainWindow::GET_YERITH_ERP_WINDOW_TITLE(),
        				 QObject::tr(WINDOW_TITLE));
    }

    inline static QString get_TRANSLATED_WindowName(QString WINDOW_TITLE)
    {
        return get_TRANSLATED_WindowName(WINDOW_TITLE.toStdString().c_str());
    }

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

    void rendreVisibleLocalTOCLASS(bool isDatabaseOpened = false);


    static bool LANGUE_ANGLAISE;


private slots:

    virtual void ON_action_START_YR_QVGE_dynamic_analysis_tool();


public slots:

    virtual void CHANGER_DE_LANGUE();

    virtual inline void ___retranslateUi()
    {
        retranslateUi(this);
    }

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual void help();

    virtual void deconnecter_utilisateur();

    void cleanupAllOtherWindows();


protected:

    virtual void setupShortcuts();


private:

    void calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW();


    YerithLogger *_logger;

    QList<YerithPushButton *> _main_window_buttons;

    QMap<uint, std::tuple <int, int>> _main_window_button_TO_page_position;
};


#define GET_YERITH_ERP_WINDOW_TITLE_MACRO YerithMainWindow::GET_YERITH_ERP_WINDOW_TITLE()



#endif /* SRC_YERITH_MAIN_WINDOW_HPP_ */
