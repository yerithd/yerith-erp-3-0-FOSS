/*
 * yerith-erp-modifier-fournisseur.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_MODIFIER_FOURNISSEUR_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_MODIFIER_FOURNISSEUR_WINDOW_HPP_

#include "../../../ui_yerith-erp-modifier-fournisseur-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithSqlTableModel;


class YerithModifierFournisseurWindow : public YerithWindowsCommons,
    									private Ui_YerithModifierFournisseurWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithModifierFournisseurWindow();

    inline ~YerithModifierFournisseurWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_modifierFournisseurWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void rendreVisible(YerithSqlTableModel *fournisseurTableModel,
                       YerithSqlTableModel *stocksTableModel,
                       bool 			   show_TabDESCRIPTION_SUPPLIER = false,
					   bool 			   show_EMPLOYE = false);

public slots:

    virtual void deconnecter_utilisateur();

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Modifier les informations sur le compte fournisseur, "
                                        "et ensuite cliquer sur le bouton 'actualiser'!"));
    }

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private slots:

    inline void selectionner_image_produit()
    {
        YerithWindowsCommons::selectionner_image(*label_image_produit);
    }

    void supprimer_image_fournisseur();

    bool customerAccountAlreadyExist();

    void annuler();

    void actualiserFournisseur();

    void supprimerFournisseur();

private:

    bool modifier_fournisseur_check_fields();

    void clear_all_fields();

    void showFournisseurDetail(bool show_EMPLOYE = false);


    inline void setupLineEdits()
    {
        lineEdit_modifier_fournisseur_reference_fournisseur->setYerithEnabled
        (false);
    }


    YerithLogger 		*_logger;

    YerithSqlTableModel *_curFournisseurTableModel;

    bool 				_SHOW_EMPLOYEE;

    int 				_curFournisseurDetailDBID;
};


#endif /* SRC_WINDOWS_YERITH_MODIFIER_FOURNISSEUR_WINDOW_HPP_ */
