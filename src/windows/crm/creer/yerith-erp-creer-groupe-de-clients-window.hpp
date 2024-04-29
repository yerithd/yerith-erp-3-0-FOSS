/*
 * yerith-erp-creer-groupe-de-clients-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_
#define YERITH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yerith-erp-creer-groupe-de-clients-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yerith-erp-window-commons.hpp"

class YerithERPWindows;
class YerithSqlTableModel;

class YerithCreerGroupeDeClientsWindow : public YerithWindowsCommons,
    									 private Ui_YerithCreerGroupeDeClientsWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithCreerGroupeDeClientsWindow();

    inline ~YerithCreerGroupeDeClientsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_creerGroupeDeClientsWindow;
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
                                       QObject::tr("Remplisser le formulaire, et ensuite cliquer "
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

    bool IS_client_present_dans_un_groupe_de_clients(const QString &aClientGroupMemberName);

    void ajouter_un_membre_au_groupe_de_clients(const QString &aClientGroupMemberName);

    void annuler_la_creation_dun_groupe_de_clients();

    void supprimerUnMembreDunGroupeDeClients();

    bool creerEnregistrerUnGroupeDeClients();

private:

    void update_client_group_member_data(const QString &new_client_group_designation,
                                         const QString &new_client_group_db_ID,
                                         const QString &client_db_ID);

    inline void setLastSelectedTableWidgetRow(int lastSelectedTableWidgetRow)
    {
        _last_selected_table_widget_row = lastSelectedTableWidgetRow;
    }

    bool clientGroupAlreadyExists();

    bool check_fields();

    void clear_all_fields();

    inline void populateComboBoxes()
    {
    	comboBox_creer_groupe_clients_programme_de_fidelite_de_clients
        	->populateComboBoxRawString(YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                                  	    YerithDatabaseTableColumn::DESIGNATION);
    }

    virtual inline void setupLineEdits()
    {
        lineEdit_creer_groupe_clients_maximum_de_membres
			->setValidator(&YerithUtils::UintValidator);
    }

    void setupLineEditsQCompleters();


    int _last_selected_table_widget_row;

    YerithLogger *_logger;
};


#endif /* YERITH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_ */
