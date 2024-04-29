/*
 * yerith-erp-mouvements_de_stocks-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef MOUVEMENTSDESTOCKS_WINDOW_HPP_
#define MOUVEMENTSDESTOCKS_WINDOW_HPP_


#include "../../../ui_yerith-erp-mouvements_de_stocks-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class YerithUtils;
class YerithSqlTableModel;
class QProcess;

class YerithMouvementsDeStocksWindow : public YerithWindowsCommons,
    								   private Ui_YerithMouvementsDeStocksWindow,
									   public YerithAbstractClassYerithSearchWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithMouvementsDeStocksWindow();

    ~YerithMouvementsDeStocksWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_mouvementsDeStocksWindow;
    }

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_mouvementsDeStocks_numero_page_derniere,
    												  label_mouvementsDeStocks_numero_page_courante)

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

    void lister_les_elements_du_tableau();

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual inline void definirVendeur()
    {
        definirPasDeRole();
    }

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();


public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_mouvements_de_stocks)

	virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr(""));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    void setFilter();

    void resetFilter(YerithSqlTableModel *sortie_stocks_table_model);

    inline void afficher_stocks_sorties_OU_transferes()
    {
        textChangedSearchLineEditsQCompleters();
    }

    void refineYerithLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();


protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

    virtual void set_filtrer_font();

private slots:
    void private_slot_afficher_historique_du_stock();

    void handle_ACTION_CHOICE_SORTIES_TRANSFERTS();

    void handle_combobox_type_de_mouvement(const QString &type_de_mouvement);

    void handle_filtres_checkBox(int state);

    bool filtrer_stocks_sorties_OU_transferes();

private:

    void initialiser_FILTRES();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();


    static const QString SORTIE_DE_STOCKS_TEXT_STRING_ENGLISH;

    static const QString SORTIE_DE_STOCKS_TEXT_STRING_FRENCH;


    static const QString TRANSFERT_DE_STOCKS_TEXT_STRING_FRENCH;

    static const QString TRANSFERT_DE_STOCKS_TEXT_STRING_ENGLISH;


    YerithLogger *_logger;

    QFont *_pushButton_mouvements_de_stocks_filtrer_font;

    QString _cur_type_de_mouvements_de_stocks;

    QString _mouvements_de_stocks_DateFilter;

    YerithSqlTableModel *_curMouvementsDeStocksTableModel;
};


#endif /* MOUVEMENTSDESTOCKS_WINDOW_HPP_ */
