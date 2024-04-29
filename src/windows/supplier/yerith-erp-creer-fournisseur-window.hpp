/*
 * yerith-erp-creer-fournisseur-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CREER_FOURNISSEUR_WINDOW_HPP_
#define YERITH_CREER_FOURNISSEUR_WINDOW_HPP_

#include "../../../ui_yerith-erp-creer-fournisseur-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class YerithERPWindows;
class YerithSqlTableModel;


class YerithCreerFournisseurWindow : public YerithWindowsCommons,
									 private Ui_YerithCreerFournisseurWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithCreerFournisseurWindow();

    inline ~YerithCreerFournisseurWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_creerFournisseurWindow;
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

	virtual void SWITCH_TO_EMPLOYE_fournisseur(const QString &EMPLOYE_ou_FOURNISSEUR = YerithUtils::EMPTY_STRING);

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


protected slots:

    inline void supprimer_image_fournisseur()
    {
        YerithWindowsCommons::supprimer_image(*label_image_produit);
    }

    inline void selectionner_image_fournisseur()
    {
        YerithWindowsCommons::selectionner_image(*label_image_produit);
    }


protected:

    virtual void setupShortcuts();


private slots:

    bool creerEnregistrerFournisseur();


private:

    static QString STRING_FOURNISSEUR;

    static QString STRING_EMPLOYE;

    bool supplierAlreadyExist();

    bool check_fields();

    void clear_all_fields();


    void populateCreerFournisseurComboBoxes();

    void setupLineEdits();


    YerithLogger *_logger;
};


#endif /* YERITH_CREER_FOURNISSEUR_WINDOW_HPP_ */
