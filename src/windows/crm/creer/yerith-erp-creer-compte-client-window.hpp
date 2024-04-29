/*
 * yerith-erp-creer-compte-client-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CREER_COMPTE_CLIENT_WINDOW_HPP_
#define YERITH_CREER_COMPTE_CLIENT_WINDOW_HPP_

#include "../../../../ui_yerith-erp-creer-compte-client-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yerith-erp-window-commons.hpp"

class YerithERPWindows;
class YerithSqlTableModel;

class YerithCreerCompteClientWindow : public YerithWindowsCommons,
    								  private Ui_YerithCreerCompteClientWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithCreerCompteClientWindow();

    inline ~YerithCreerCompteClientWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_creerCompteClientWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void rendreVisible(YerithSqlTableModel *stocksTableModel);

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

protected slots:

    inline void supprimer_image_compte_client()
    {
        YerithWindowsCommons::supprimer_image(*label_image_produit);
    }

    inline void selectionner_image_compte_client()
    {
        YerithWindowsCommons::selectionner_image(*label_image_produit);
    }

protected:

    virtual void setupShortcuts();

private slots:

    bool creerEnregistrerCompteClient();

private:

    bool customerAccountAlreadyExist();

    bool check_fields();

    void clear_all_fields();

    inline void localSetupLineEditsQCompleters()
    {
        lineEdit_compte_client_refereur
			->setupMyStaticQCompleter(YerithDatabase::CLIENTS,
                                      YerithDatabaseTableColumn::NOM_ENTREPRISE);
    }

    void setupLineEdits();

    YerithLogger *_logger;
};


#endif /* YERITH_CREER_COMPTE_CLIENT_WINDOW_HPP_ */
