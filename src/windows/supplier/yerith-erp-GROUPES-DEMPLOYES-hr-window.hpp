/*
 * yerith-erp-GROUPES-DEMPLOYES-hr-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_groupes_demployes_hr_WINDOW_HPP_
#define YERITH_ERP_groupes_demployes_hr_WINDOW_HPP_

#include "../../../ui_yerith-erp-GROUPES-DEMPLOYES-hr-window.h"

#include <QtCore/QDebug>

#include <QtCore/QVector>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "src/windows/yerith-erp-window-commons.hpp"


class YerithPOSDialogCommons;
class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerithSqlTableModel;

class YerithERPGroupesDemployesHRWindow : public YerithWindowsCommons,
    									  private Ui_YerithERPGroupesDemployesHRWindow,
										  public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithERPGroupesDemployesHRWindow();

    virtual ~YerithERPGroupesDemployesHRWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_GROUPESDEMPLOYESWindow;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_numero_page_derniere,
    												  label_numero_page_courante)

    virtual void rendreInvisible();

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_GROUPESDEMPLOYES)

	virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("1) Sélectionner un 'groupe de clients' dans le"
                                    		   	   " tableau de cette page, ensuite cliquer sur"
                                    		   	   " l'opération que vous souhaitez"
                                    		   	   " réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    void afficher_au_detail();

    inline void afficher_au_detail(const QModelIndex &modelIndex)
    {
        afficher_au_detail();
    }

    void refineYerithLineEdits();

    bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

private:

    void initialiser_FILTRES();

private slots:

    void handle_filtres_checkBox(int state);

    void supprimer_PLUSIEURS_GROUPESDEMPLOYES(YerithSqlTableModel &
                                                aClientGroupTableModel);

    void supprimer_GROUPE_DEMPLOYES();

    void afficher_GROUPESDEMPLOYES(YerithSqlTableModel &
                                     aClientGroupTableModel);

    inline void afficher_GROUPESDEMPLOYES()
    {
        afficher_GROUPESDEMPLOYES(*_curEMPLOYEEGroupTableModel);
    }

    bool filtrer_GROUPESDEMPLOYES();


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual inline void hideEvent(QHideEvent *hideEvent)
    {
        YerithWindowsCommons::CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
    }

    virtual void setupShortcuts();

    void resetFilter();


protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


private:

    void clear_all_fields();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    QString _localStrategy;

    YerithLogger *_logger;

    QStringList _historiqueStock;

    QString _EmployeeGroupDateCreationFilter;

    QFont *_pushButton_filtrer_font;

    YerithSqlTableModel *_curEMPLOYEEGroupTableModel;
};

#endif /* YERITH_ERP_groupes_demployes_hr_WINDOW_HPP_ */
