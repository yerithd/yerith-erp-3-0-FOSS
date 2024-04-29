/*
 * yerith-erp-creer-UN-GROUPE-DE-PAIE-hr-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_CREER_UN_GROUPE_DE_PAIE_WINDOW_HPP_
#define YERITH_CREER_UN_GROUPE_DE_PAIE_WINDOW_HPP_

#include "../../../ui_yerith-erp-creer-GROUPE-DE-PAIE-hr-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yerith-erp-window-commons.hpp"

class YerithERPWindows;
class YerithSqlTableModel;

class YerithCreerUnGroupeDePaieWindow : public YerithWindowsCommons,
    									private Ui_YerithCreerUnGroupeDePaieWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithCreerUnGroupeDePaieWindow();

    inline ~YerithCreerUnGroupeDePaieWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerithCreerGroupeDePaieWindow;
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
                                       QObject::tr("Remplisser le formulaire, et ensuite cliquer "
                                    		   	   "sur enregistrer !"));
    }

    void deconnecter_utilisateur();


protected:

    virtual void setupShortcuts();


private slots:

    inline void HANDLE_lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL_visualisation
            (const QString &text_changed)
    {
    	lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL_visualisation
			->setText(GET_DOUBLE_STRING(text_changed.toDouble()));
    }


    void annuler_la_creation_dun_GROUPE_DE_PAIE_hr();


    bool creerEnregistrerUnGroupeDePaieHr();


private:

    bool payGroupAlreadyExists();

    bool check_fields();

    void clear_all_fields();

    void updatePopulateComboBoxes();

    void populateComboBoxes();

    virtual void setupLineEdits();

    int _last_selected_table_widget_row;

    YerithLogger *_logger;
};


#endif /* YERITH_CREER_UN_GROUPE_DE_PAIE_WINDOW_HPP_ */
