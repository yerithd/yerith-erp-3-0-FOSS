/*
 * yerith-erp-admin-detail-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ADMIN_DETAIL_WINDOW_HPP_
#define YERITH_ADMIN_DETAIL_WINDOW_HPP_

#include "../../../ui_yerith-erp-admin-detail-window.h"

#include "src/admin/yerith-erp-admin-windows-commons.hpp"

#include "src/admin/admin-actions-subjects.hpp"

#include <QtWidgets/QMessageBox>

class YerithERPWindows;
class YerithLogger;

class YerithAdminDetailWindow : public YerithPOSAdminWindowsCommons,
								private Ui_YerithAdminDetailWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithAdminDetailWindow();

    inline ~YerithAdminDetailWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_adminDetailWindow;
    }


    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();


    void rendreVisibleCompteUtilisateur(int sqlTableRow);

    void rendreVisibleLocalisation(int sqlTableRow);

    void rendreVisibleDepartementsDeProduits(int sqlTableRow);

    void rendreVisibleCategorie(int sqlTableRow);

	void rendreVisibleLigneBudgetaire(int sqlTableRow);

    void rendreVisibleCompteBancaire(int sqlTableRow);

    void rendreVisibleRemise(int sqlTableRow);

    void rendreVisible_CHARGE_FINANCIERE(int sqlTableRow);

    void rendreVisibleAlerte(int sqlTableRow);


    virtual void rendreInvisible();


public slots:

    void creer();

    void lister();

    void modifier();

    void supprimer();


    void retourListerCompteUtilisateur();

    void retourListerLocalisation();

    void retourListerDepartementsDeProduits();

    void retourListerCategorie();

    void retourLister_LIGNE_BUDGETAIRE();

    void retourListerCompteBancaire();

    void retourListerRemise();

    void retourListerChargeFinanciere();

    void retourListerAlerte();


    virtual bool imprimer_pdf_document();


protected slots:

    void handleCurrentChanged(int index);


private:

    void enableOtherTabs(enum AdminSujetAction curAction, bool enabled);


    void setupLineEdits();


    void clear_set_edit_comboBoxes();

    void clear_set_edit_utilisateur_comboBoxes();

    void clear_set_edit_alerte_comboBoxes();


    YerithLogger *_logger;
};

#endif /* YERITH_ADMIN_DETAIL_WINDOW_HPP_ */
