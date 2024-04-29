/*
 * yerith-erp-creer-groupe-DEMPLOYES-hr-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CREER_GROUPE_demployes_HR_WINDOW_HPP_
#define YERITH_CREER_GROUPE_demployes_HR_WINDOW_HPP_

#include "../../../ui_yerith-erp-creer-groupe-DEMPLOYES-hr-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yerith-erp-window-commons.hpp"

class YerithERPWindows;
class YerithSqlTableModel;

class YerithCreerGroupeDemployesWindow : public YerithWindowsCommons,
    									 private Ui_YerithCreerGroupeDemployesWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithCreerGroupeDemployesWindow();

    inline ~YerithCreerGroupeDemployesWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_creerGroupeDemployesWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    virtual inline void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }

    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

public slots:

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Remplisser le formulaire, et ensuite cliquer "
                                        "sur enregistrer !"));
    }

    void deconnecter_utilisateur();

protected:

    virtual void setupShortcuts();

private slots:

    inline void handle_select_table_widget_row(QTableWidgetItem *anItem)
    {
        _last_selected_table_widget_row = anItem->row();
    }

    bool IS_client_present_dans_un_groupe_DEMPLOYES(const QString &anEmployeeGroupMemberName);

    void ajouter_un_membre_au_groupe_DEMPLOYES(const QString &anEmployeeGroupMemberName);

    void annuler_la_creation_dun_groupe_demployes();

    inline void supprimerUnMembreDunGroupeDemployes_HR()
    {
        tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe
    		->removeArticle(_last_selected_table_widget_row);
    }

    bool creerEnregistrerUnGroupeDEMPLOYES_hr();

private:

    void update_EMPLOYEE_GROUP_MEMBER_DATA(const QString &new_client_group_designation,
                                           const QString &new_client_group_db_ID,
                                           const QString &client_db_ID);

    inline void setLastSelectedTableWidgetRow(int lastSelectedTableWidgetRow)
    {
        _last_selected_table_widget_row = lastSelectedTableWidgetRow;
    }

    bool employeeGroupAlreadyExists();

    bool check_fields();

    void clear_all_fields();

    virtual inline void setupLineEdits()
    {
        lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres
			->setValidator(&YerithUtils::UintValidator);
    }

    inline void populateComboBoxes()
    {
    	comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR
        	->populateComboBoxRawString(YerithDatabase::GROUPES_DE_PAIE_hr,
                                  	    YerithDatabaseTableColumn::DESIGNATION);
    }

    void setupLineEditsQCompleters();


    int _last_selected_table_widget_row;

    YerithLogger *_logger;
};


#endif /* YERITH_CREER_GROUPE_demployes_HR_WINDOW_HPP_ */
