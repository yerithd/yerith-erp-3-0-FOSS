/*
 * yerith-erp-groupes-dun-client-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_

#include "../../../ui_yerith-erp-groupes-dun-client-window.h"


#include "src/windows/yerith-erp-qtable-widget-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class YerithSqlTableModel;


class QContextMenuEvent;


class YerithGroupesDunClientWindow : public YerithQTableWidgetWindowsCommons,
									 private Ui_YerithGroupesDunClientWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithGroupesDunClientWindow();

    virtual inline ~YerithGroupesDunClientWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_menuGroupesDunClientWindow;
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

public slots:

    virtual bool export_csv_file();

    virtual bool imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne();

    virtual inline bool imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne_WITH_A_YERITH_PROGRESS_BAR()
    {
        YerithProgressBar(this)(this,
                                &YerithGroupesDunClientWindow::imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne);
    }


    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Sélectionner un compte client dans le tableau "
                                        "des groupes d'1 client, ensuite "
                                        "cliquer sur l'opération que vous souhaitez réaliser !"));
    }


protected slots:

    virtual bool imprimer_UNE_LISTE(bool *full_list = 0);


protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();


private:


    inline int getLast_SelectedRow_Row_ID()
    {
        return _Last_SelectedRow_Row_INDEX;
    }


private slots:

    inline void setLast_SelectedRow_Row_ID(const QModelIndex &a_model_CELL_index)
    {
        _Last_SelectedRow_Row_INDEX = a_model_CELL_index.row();
    }


    void ajouter_appartenance(const QString &un_groupe_de_clients);


    void executer_ajouter_appartenance(const QString *un_groupe_de_clients);


    void retirer_ce_client_du_groupe_selectionne();


    bool EXECUTER_retirer_ce_client_du_groupe_selectionne();


    void afficher_au_detail(const QModelIndex &modelIndex);

    inline void afficher_au_detail()
    {
        afficher_au_detail(tableWidget_groupes_dun_client->currentIndex());
    }

    void afficher_tous_les_groupes_du_client();


private:

    void get_PRINT_OUT_TexTableString(QString       &texTable_IN_OUT,
                                      int           row_MAX_TO_GO_export = -1);

    inline void setupLineEdits()
    {
    	lineEdit_groupes_dun_client_nombre_de_groupes->setYerithEnabled(false);
    }

    void populateComboBoxes();


    int _Last_SelectedRow_Row_INDEX;

    YerithLogger *_logger;

    YerithSqlTableModel *_curClientsTableModel;

    QString _curClient_NOM_ENTREPRISE;

    QString _curClientDBID;
};

#endif /* SRC_WINDOWS_YERITH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_ */
