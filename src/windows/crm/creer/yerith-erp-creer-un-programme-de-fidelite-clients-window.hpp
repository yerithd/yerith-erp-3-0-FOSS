/*
 * yerith-erp-creer-un-programme-de-fidelite-clients-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define YERITH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yerith-erp-creer-un-programme-de-fidelite-clients-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yerith-erp-window-commons.hpp"

class YerithERPWindows;
class YerithSqlTableModel;

class YerithCreerUnProgrammeDeFideliteClientsWindow : public YerithWindowsCommons,
    												  private Ui_YerithCreerUnProgrammeDeFideliteClientsWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithCreerUnProgrammeDeFideliteClientsWindow();

    inline ~YerithCreerUnProgrammeDeFideliteClientsWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerithCreerProgrammeDeFideliteClientsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual inline void definirVendeur()
    {
        definirPasDeRole();
    }

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

    void annuler_la_creation_dun_programme_de_fidelite_de_clients();

    bool creerEnregistrerUnProgrammeDeFideliteClients();

private:

    bool royaltyClientProgramAlreadyExists();

    bool check_fields();

    void clear_all_fields();

    void updatePopulateComboBoxes();

    inline void populateComboBoxes()
    {
        comboBox_creer_un_programme_de_fidelite_clients_localisation
			->populateComboBoxRawString(YerithDatabase::LOCALISATIONS,
										YerithDatabaseTableColumn::NOM_LOCALISATION);
    }


    int _last_selected_table_widget_row;

    YerithLogger *_logger;
};


#endif /* YERITH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
