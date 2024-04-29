/*
 * yerith-erp-admin-windows-commons.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_ADMIN_WINDOWS_COMMONS_HPP_
#define SRC_YERITH_ERP_ADMIN_WINDOWS_COMMONS_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include <tuple>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>

class YerithERPWindows;

class YerithPOSAdminWindowsCommons : public YerithWindowsCommons
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

    inline YerithPOSAdminWindowsCommons(QString windowName)
	 :_windowName_WITH_NO_MAINTENANCE(windowName),
     _windowName(windowName)
    {
    }

    ~YerithPOSAdminWindowsCommons();

    virtual inline QString qMessageBoxStyleSheet()
    {
        return QMESSAGE_BOX_STYLE_SHEET;
    }

    inline QString getWindowName()
    {
        return _windowName;
    }

    virtual QToolBar *getQMainWindowToolBar() = 0;

    virtual inline void YERITH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(QAction 	*anAction,
                                                                     bool		aBooleanValue)
    {
        anAction->setVisible(aBooleanValue);
    }

    inline static void setCenterPosition(QPoint *desktopCenterPosition)
    {
        _centerPosition = desktopCenterPosition;
    }

    inline static void setAllWindows(YerithERPWindows *allWindows)
    {
        _allWindows = allWindows;
    }

    virtual inline void definirCaissier()
    {
    }

    virtual inline void definirManager()
    {
    }

    virtual inline void definirVendeur()
    {
    }

    virtual inline void definirGestionaireDesStocks()
    {
    }

    virtual inline void definirMagasinier()
    {
    }

    virtual inline void definirPasDeRole()
    {
    }

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

    virtual inline void rendreInvisible()
    {
        setVisible(false);
    }

    inline static YerithERPWindows *getAllWindows()
    {
        return _allWindows;
    }

public slots:

    virtual void menu();

    virtual void retour_menu_principal();

    virtual void changer_utilisateur();

    virtual void deconnecter_utilisateur();

    virtual inline void fermeture()
    {
        close();
    }

    virtual inline void apropos()
    {
        YerithQMessageBox::about(this,
        						 YerithUtils::APPLICATION_NAME,
                                 YerithUtils::APROPOS_MSG);
    }

    virtual void CLOSE_SELECT_EXPORT_DB_DIALOG(YerithSqlTableModel 			 *aSqlTableModel = 0,
    										   YerithTableViewWITHpagination *a_table_view_to_list_show = 0);

	virtual void selectionner_champs_db_visibles(bool       SETUP_DIALOG_BOX_GEOMETRY = true,
												 bool       CHARGES_FINANCIERES_WINDOW = false,
												 QString    CHARGES_FINANCIERES_WINDOW_Db_Column = YerithUtils::EMPTY_STRING,
												 QString    CHARGES_FINANCIERES_WINDOW_Column_Custom_Viewing_STR = YerithUtils::EMPTY_STRING);

	virtual YerithERPGenericSelectDBFieldDialog *GET__LOCALIZED__generic_selectdbfield__DIALOG__(QString aSqlTableName);

    virtual void setup_select_configure_dbcolumn(const QString &aSqlTableName);

    virtual void infosEntreprise();

    virtual void qui_suis_je();

    virtual inline void help()
    {
    }

    void getManuelUtilisateurPDF();

    inline YerithERPGenericSelectDBFieldDialog *GET_ERP_GENERIC_SELECTDBFIELD_DIALOG(QString sqlTableName)
    {
    	return GET__LOCALIZED__generic_selectdbfield__DIALOG__(sqlTableName);
    }

protected:

    void mySetupUi(QMainWindow *aWindow);

    static YerithERPWindows *_allWindows;


    QString QMESSAGE_BOX_STYLE_SHEET;

    QString _windowName_WITH_NO_MAINTENANCE;

    QString _windowName;

    QMap<QString, std::tuple<bool, YerithERPGenericSelectDBFieldDialog *>> sqlTableName__TO__selectDBFieldDialog;

    static QPoint *_centerPosition;
};

#endif /* SRC_YERITH_ERP_ADMIN_WINDOWS_COMMONS_HPP_ */
