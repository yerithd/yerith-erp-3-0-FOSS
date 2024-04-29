/*
 * yerith-erp-GROUPES-dun-employe-hr-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_ERP_GROUPES_DE_PAIE_DUN_employe_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_ERP_GROUPES_DE_PAIE_DUN_employe_WINDOW_HPP_

#include "../../../ui_yerith-erp-GROUPES-dun-employe-hr-window.h"


#include "src/windows/yerith-erp-qtable-widget-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class YerithSqlTableModel;


class QContextMenuEvent;


class YerithGROUPES_DUN_EMPLOYE_Window : public YerithQTableWidgetWindowsCommons,
    								 	 private Ui_YerithGROUPES_DUN_EMPLOYE_Window
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithGROUPES_DUN_EMPLOYE_Window();

    virtual inline ~YerithGROUPES_DUN_EMPLOYE_Window()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_menuGROUPES_DE_PAIE_DUN_EMPLOYE_Window;
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

    virtual void rendreVisible(YerithSqlTableModel *clientTableModel,
                               YerithSqlTableModel *stocksTableModel);


signals:

    void YRI_ROW_count_changed();


public slots:

    virtual bool export_csv_file();

    virtual bool imprimer_pdf_document();

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Sélectionner un groupe d'employés dans le tableau "
                                        "des groupes d'1 employé, ensuite "
                                        "cliquer sur l'opération que vous souhaitez réaliser !"));
    }


    void handle_DATE_DEBUT_et_DATE_FIN_dappartenance(int row);


protected slots:

    virtual bool imprimer_UNE_LISTE(bool *full_list = 0);


protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();


private slots:

    bool CREATE_A_NEW_PERIODE_DE_TEMPS_APPARTENANCE();


    void handle_activer_DATES_DAPPARTENANCE(int state);


    inline void handle_MANUAL_pay_for_selected_PAY_GROUP_button()
    {
        YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK
                        (this,
                         &YerithGROUPES_DUN_EMPLOYE_Window::EXECUTER_handle_MANUAL_pay_for_selected_PAY_GROUP_button);
    }


    bool EXECUTER_handle_MANUAL_pay_for_selected_PAY_GROUP_button();


    void handle_VALIDER_button();


    inline void handle_payer_manuellement_Button()
    {
        (tableWidget_Groupes_Dun_Employe->rowCount() <= 0) ?
            pushButton_MANUAL_pay_for_selected_PAY_GROUP->setVisible(false) :
            pushButton_MANUAL_pay_for_selected_PAY_GROUP->setVisible(true);
    }


    inline void handle_DATE_DEBUT_et_DATE_FIN_dappartenance(const QModelIndex &aQModelIndex)
    {
        handle_DATE_DEBUT_et_DATE_FIN_dappartenance(aQModelIndex.row());
    }


    void ajouter_appartenance(const QString &un_groupe_DEMPLOYES_hr);


    void executer_ajouter_appartenance(const QString *un_groupe_DEMPLOYES_hr);


    inline void retirer_CET_EMPLOYE_du_groupe_selectionne()
    {
        YerithUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK
                        (this,
                         &YerithGROUPES_DUN_EMPLOYE_Window::EXECUTER_retirer_cet_employe_du_groupe_selectionne);
    }


    bool EXECUTER_retirer_cet_employe_du_groupe_selectionne();


    void afficher_au_detail(const QModelIndex &modelIndex);


    inline void afficher_au_detail()
    {
        afficher_au_detail(tableWidget_Groupes_Dun_Employe->currentIndex());
    }


    void afficher_tous_les_groupes_DUN_EMPLOYE_hr();


    void enable_yerith_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();


private:

    void get_PRINT_OUT_TexTableString(QString   &texTable_IN_OUT,
                                      int        row_MAX_TO_GO_export = -1);

    void disable_yerith_widgets();

    void setupLineEdits();

    void populateComboBoxes();


    YerithLogger *_logger;

    YerithSqlTableModel *_curEMPLOYEEgroups_TableModel;

    double  _somme_SALAIRE_CUMULE_A_DATE;

    QString _curEMPLOYEE_groupe_de_paie_hr;

    QString _curEMPLOYEE_NOM_ENTREPRISE;

    QString _curEmployeeDBID;

    QMap<QString, double> _map_GROUPE_SELECTIONNE__to__salaire;
};

#endif /* SRC_WINDOWS_YERITH_ERP_GROUPES_DE_PAIE_DUN_employe_WINDOW_HPP_ */
