/*
 * yerith-erp-modifier.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WINDOWS_YERITH_MODIFIER_WINDOW_HPP_
#define SRC_WINDOWS_YERITH_MODIFIER_WINDOW_HPP_

#include "../../../ui_yerith-erp-modifier-window.h"


#include "src/windows/yerith-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerithSqlTableModel;

class YerithModifierWindow : public YerithWindowsCommons,
    						 private Ui_YerithModifierWindow
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

	YerithModifierWindow();

    inline ~YerithModifierWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_modifierWindow;
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

    virtual void definirGestionaireDesStocks();

    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerithSqlTableModel *stocksTableModel);

public slots:

    void display_prix_vente();

    void display_prix_vente_EN_GROS();

    void handleTVACheckBox(bool clicked);

    void supprimer_ce_stock();

    void supprimer_image_stock();

    inline void selectionner_image_produit()
    {
        YerithWindowsCommons::selectionner_image(*label_image_produit);
    }

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
                                       ("Modifier les informations sur le stock (service), "
                                        "et ensuite cliquer sur le bouton 'actualiser'!"));
    }

protected:

    virtual void setupShortcuts();

private slots:

    void handle_HISTORIQUE_CHECKBOX(int aState);

    void handle_achat_checkBox(int aState);

    void handle_re_approvisionnement_checkBox(int state);

    void actualiser_stock();

    void edited_prix_vente(const QString &newPrixVente);

    void edited_prix_vente_EN_GROS(const QString &newPrixVente_en_gros);

    void calculate_and_display_ALL_benefit_buying_price_percentage();

    void calculate_and_display_benefit_buying_price_percentage();

    void calculate_and_display_benefit_buying_price_percentage_EN_GROS();

private:

    void handleVisibilityPrixDachat(bool anEditable);

    void ajouter_nouveau_re_approvisionnement_achat(double
                                                    quantiteEnReApprovisionement,
                                                    int stockId);

    void update_achat_deja_existant(const QSqlRecord &aStockRecord,
                                    double aPrixUnitaireHT,
                                    double aPrixUnitaire_EN_GROS_HT);

    void setupLineEdits();

    void setupLineEditsQCompleters();

    inline void setupDateTimeEdits()
    {
        dateEdit_date_peremption->setStartDate(GET_CURRENT_DATE);
    }

    bool check_fields();

    void clear_all_fields();

    void setStockSpecificWidgetVisible(bool visible);

    void showItem();


    YerithLogger *_logger;

    int _currentStockID;

    double _montantTva;

    double _montantTva_EN_GROS;

    bool _IS_AN_ASSET;

    bool _tvaCheckBoxPreviousState;

    QString _currentPrixDachat;

    QString _lastEditedPrixVente;

    QString _lastEditedPrixVente_EN_GROS;

    QString _tvaPercent;
};


#endif /* SRC_WINDOWS_YERITH_MODIFIER_WINDOW_HPP_ */
