/*
 * yerith-erp-fournisseur-details-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_FOURNISSEUR_DETAILS_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_FOURNISSEUR_DETAILS_WINDOW_HPP_

#include "../../../ui_yerith-erp-fournisseur-details-window.h"


#include "src/utils/yerith-erp-logger.hpp"

#include "src/windows/yerith-erp-window-commons.hpp"


#include "src/utils/yerith-erp-LIST-map-COMPLEX-ITEM.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithERPWindows;
class YerithSqlTableModel;
class YerithLogger;

class YerithFournisseurDetailsWindow : public YerithWindowsCommons,
    								   private Ui_YerithFournisseurDetailsWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithFournisseurDetailsWindow();

    inline ~YerithFournisseurDetailsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_fournisseurDetailsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    void definirManager();

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

    void definirPasDeRole();


    void rendreInvisible();

    void rendreVisible(YerithSqlTableModel *fournisseurTableModel,
                       YerithSqlTableModel *stocksTableModel,
					   bool 			   employe = false);


public slots:

    virtual bool imprimer_pdf_document();

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("Vous pouvez modifier les détails de ce fournisseur"
                                    		   	   " en cliquant sur 'Modifier' !"));
    }


protected:

    virtual void setupShortcuts();


private slots:

    void afficher_groupes_dun_employe();

    void modifierFournisseur();

    void supprimerFournisseur();


private:

    void setupLineEdits();

    void showFournisseurDetail(bool employe = false);

    void checkCourrierAlerts();



    YerithLogger *_logger;

    bool _SHOW_EMPLOYEE;

    double _sommeTotal;

    YerithSqlTableModel *_curFournisseurTableModel;
};


#endif /* SRC_WINDOWS_YERITH_FOURNISSEUR_DETAILS_WINDOW_HPP_ */
