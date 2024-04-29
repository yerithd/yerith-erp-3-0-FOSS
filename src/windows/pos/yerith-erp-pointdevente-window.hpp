/*
 * yerith-erp-pointdevente-window.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_VENTE_WINDOW_HPP_
#define YERITH_VENTE_WINDOW_HPP_


#include "../../../ui_yerith-erp-pointdevente-window.h"


#include "src/windows/yerith-erp-qtable-widget-window-commons.hpp"

#include "src/utils/yerith-erp-LIST-map-COMPLEX-ITEM.hpp"

#include "src/utils/credit-card/yerith-erp-carte-credit-info.hpp"


#include <QtCore/QMap>

#include <QtCore/QDebug>


class YerithSqlTableModel;
class YerithTableView;
class YerithArticleVenteInfo;
class YerithPOSCreditCardInfo;


class YerithPointDeVenteWindow : public YerithQTableWidgetWindowsCommons,
    							 public Ui_YerithPointDeVenteWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithPointDeVenteWindow();

    virtual ~YerithPointDeVenteWindow();

    inline bool paiement_par_carte_credit_carte_debit()
    {
        return _paiement_carte_credit_carte_debit;
    }

    inline bool paiement_comptant()
    {
        return _paiment_comptant;
    }

    inline bool paiment_compteclient_AVANCE_DE_CASH()
    {
        return _paiement_compteclient_AVANCE_DE_CASH;
    }

    inline void set_paiement_par_carte_credit_carte_debit(bool aValueCarteCreditCarteDebit)
    {
        _paiement_carte_credit_carte_debit = aValueCarteCreditCarteDebit;
    }

    inline void set_paiement_comptant(bool aValueComptant)
    {
        _paiment_comptant = aValueComptant;
    }

    inline void set_paiment_compteclient_AVANCE_DE_CASH(bool aValueCompteClient_AVANCE_DE_CASH)
    {
        _paiement_compteclient_AVANCE_DE_CASH = aValueCompteClient_AVANCE_DE_CASH;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_venteWindow;
    }

    void setup_AJOUT_COMMERCIAL(bool aValue);

    void setupRemises(bool aValue);

    void deleteArticleVenteInfos();

    void cleanUpAfterVente();

    virtual void definirCaissier();

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

    void handleTabViews();

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);


signals:

    void SELLING();


public slots:

    virtual void CHANGER_DE_LANGUE();


    virtual inline void ___retranslateUi()
    {
        Ui_YerithPointDeVenteWindow::retranslateUi(this);
    }

    QString afficher_recu_vendu_pdf(QString referenceRecu = YerithUtils::EMPTY_STRING);

    QString imprimer_recu_vendu(QString referenceRecu = YerithUtils::EMPTY_STRING);

    QString imprimer_recu_vendu_grand(QString referenceRecuGRAND = YerithUtils::EMPTY_STRING);

    QString imprimer_recu_vendu_petit(QString referenceRecuPETIT = YerithUtils::EMPTY_STRING);

    void handleAddedArticle();

    void handleCurrentTABChanged(int index);

    void CLEAR_LOYALTY_PROGRAM_DATA_FOR_CURRENT_SESSION_SALE();

    void annuler();

    void setRechercheLineEditFocus();

    void setRechercheDesignationArticleFocus();

    void setRechercheCodebarArticleFocus();

    void activateLineEdit_AJOUT_COMMERCIAL(bool toggled);

    void activateLineEditRemisePrix(bool toggled);

    void activateLineEditRemisePourcentage(bool toggled);

    void calculate_details_window_AJOUT_COMMERCIAL();

    void calculate_details_window_remise_prix();

    void calculate_details_window_remise_pourcentage();

    void actualiser_montant_remise();

    void resetCheckboxTVA();

    void handleTVACheckBox(bool clicked);

    void HANDLE_VISIBILITY_OF_LOYALTY_LABEL_AND_LINEEDIT(const QString &text = YerithUtils::EMPTY_STRING);

    inline void handle_VIEW_FONT_SIZE_change_buttons_FROM__SELLING_signal()
    {
    	handle_VIEW_FONT_SIZE_change_buttons(true);
    }

    void handle_VIEW_FONT_SIZE_change_buttons(bool CLEAR_AFTER_SELLING = false);

    inline void handleQtePressed(QTableWidgetItem *itemPressed)
    {
        _previousPressedQteValue = itemPressed->text();
    }

    void handleQteChange(QTableWidgetItem *itemChanged);

    void update_text_memo_SALE();

    void updateQuantiteAVendre();

    bool article_exist(const QString codeBar,
                       const QString designation);

    void ajouter_article(const QString &aStockID);

    void ajouter_article_stockname(const QString &aStockName);

    void ajouter_article_codebar(const QString &aStockReference);

    void actualiser_articles(int row,
                             unsigned newItemQte);

    void actualiser_articles_codebar(int row,
                                     unsigned newItemQte);

    void actualiser_tableau_vente();

    void actualiser_toutes_valeurs();

    void actualiser_toutes_valeurs(const QString &a_client_nom_entreprise);

    void update_lineedits_and_labels(double total);

    inline void setMontantComptantRecu(double unMontantRecu)
    {
        _montantRecu = unMontantRecu;
    }

    void handleMontantRecu();

    void retirer_un_article();

    void afficher_tableau_ventes(const int tableWidgetRow);

    void afficher_vente_detail(const int tableWidgetRow);

    inline void afficher_vente_detail(const QModelIndex &modelIndex)
    {
        afficher_vente_detail(modelIndex.row());
    }

    inline void afficher_vente_detail(QTableWidgetItem *item)
    {
        afficher_vente_detail(item->row());
    }

    inline void afficher_ventes()
    {
        tabWidget_vente->setCurrentIndex(TableauDesVentes);
    }

    inline void rendreInvisibleAvecConservation()
    {
        setVisible(false);
    }

    void lister();

    inline YerithPOSCreditCardInfo *getCurrentCreditCardInfo()
    {
        return _currentCreditCardInfo;
    }

    void choisir_methode_paiment();

    unsigned int effectuer_check_out_carte_credit_carte_debit();

    void executer_la_vente_comptant();

    unsigned int effectuer_check_out_comptant();

    void updateCompteClient(double nouveau_compte_client);

    double GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS();

    double GET_BEST_CURRENT_LOYALTY_PROGRAM_MONEY_BENEFITS(QString &client_best_loyalty_program_IN_OUT);

    double calculate_LOYALTY_PROGRAM_MONEY_BENEFITS(const QString &a_loyalty_program);

    void updateCompteClient_PROGRAMME_DE_FIDELITE_LOYALTY(const QString &a_nom_entreprise_client,
                                                          bool          CALL_update_lineedits_and_labels = false);

    void handle_CLIENT_LOYALTY_PROGRAM_PAYMENTS_ENTRIES(const QString &a_product_reference);

    void executer_la_vente_compteclient_FIDELITE();

    void executer_la_vente_compteclient_AVANCE_DE_CASH(double TOTAL_PRIX_DE_VENTE);

    unsigned int effectuer_check_out_compte_client();

    unsigned int effectuer_checkout_compteclient_AVANCE_DE_CASH();

    inline void setCurrentClientName(QString curClientName)
    {
        _curClientName = curClientName;
    }

    inline QString getCurrentClientName()
    {
        return _curClientName;
    }

    virtual inline void apropos()
    {
        YerithWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("Choisissez les articles à vendre en utilisant "
                                                   "leur référence à la première barre de recherche, ou bien "
                                                   "leur désignation à la deuxième barre de recherche !"));
    }


private slots:

    void enable_DATA_EXTRA_VENTE(bool enable);

    void retourVentes();

    void setBarcodeAsStandardInput();

    void setStockItemNameAsStandardInput();

    void updateLineEditQCompleterInput();

    void handle_DATA_EXTRA_VENTE(int state);

    void handle_effectuer_vente_en_gros(int state);

    void handleBasculerLecteurDeCodebarres();

    void handleRefreshSaleStrategy();

    bool PROCESS_CREDIT_CARD_PAYMENT();

    bool PRE__PROCESS_CREDIT_CARD_PAYMENT();


protected slots:

    virtual inline bool imprimer_UNE_LISTE(bool *full_list = 0)
    {
        return false;
    }

    virtual inline void disableImprimer()
    {
        actionAfficherPDF->setVisible(false);
    }

    virtual inline void enableImprimer()
    {
        actionAfficherPDF->setVisible(true);
    }


protected:

    virtual void hideEvent(QHideEvent *event);

    virtual void setupShortcuts();


private:

    enum TabIndex
    {
        TableauDesVentes = 0,
        AfficherVenteAuDetail
    };

    virtual double GET_CURRENT_CLIENT_AMOUNT_TO_BE_PAID();

    void disconnect_all_objects_for_stock_article_item_selection();

    void connect_manual_selection_of_article_item();

    void connect_barcode_reader_selection_of_article_item();

    void setupLineEdits();

    void setupLineEditsQCompleters(bool ONLY_CLIENT_NAMES_LINE_EDIT = false);

    void setupDateTimeEdits();


    YerithLogger *_logger;

    bool _paiement_carte_credit_carte_debit;

    bool _paiment_comptant;

    bool _paiement_compteclient_AVANCE_DE_CASH;

    YerithLineEdit *_currentFocusSearchBar;

    YerithPOSCreditCardInfo *_currentCreditCardInfo;

    static bool _qteChangeCodeBar;

    bool _barcodeReaderActivated;

    bool _updateItemConversionError;

    int _typeDeVente;

    QString _previousPressedQteValue;

    bool _tvaCheckBoxPreviousState;

    QString _currentStocksID;

    double _vente_LOYALTY_PROGRAM_NOUVEAU_COMPTE_CLIENT;

    double _TOTAL_PRIX_VENTE__POUR__COMPTECLIENT_AVANCE_DE_CASH;

    double _sommeTotal_HORS_TAXES;

    double _sommeTotal;

    double _remise_somme_total_prix;

    double _remise_somme_total_pourcentage;

    double _SERVICE;

    double _tva;

    double _montantRecu;

    double _montantARembourser;

    int _quantiteAVendre;

    int _currentTabView;

    QAction *actionRechercheArticle;

    QAction *actionRechercheArticleCodebar;

    QString _searchFilter;

    QString _curClientName;

    Yerith_LIST_map_COMPLEX_ITEM _client_group_program_TO_money_benefit;

    QMap < int, YerithArticleVenteInfo * >articleItemToVenteInfo;
};


#endif /* YERITH_VENTE_WINDOW_HPP_ */
