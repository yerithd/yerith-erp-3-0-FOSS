/*
 * yerith-erp-tableaux-de-bord-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */
#include "yerith-erp-tableaux-de-bord-window.hpp"


#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-map-COMPLEX-ITEM.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <unistd.h>

#include <QtWidgets/QFileDialog>

#include <QtCore/QDateTime>

#include <QtCore/QMapIterator>

#include <QtCore/QtMath>

#include <QtCore/QPair>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>



QString YerithTableauxDeBordWindow::ANALYSE_COMPAREE_VENTES_BENEFICES("ANALYSE COMPARÉ VENTES BÉNÉFICES");

QString YerithTableauxDeBordWindow::ANALYSE_COMPAREE_ACHATS_VENTES("ANALYSE COMPARÉ ACHATS VENTES");


QString YerithTableauxDeBordWindow::OPERATION_GENERER("générer les");

QString YerithTableauxDeBordWindow::OPERATION_GENERER_BILAN_COMPTABLE("générer le bilan comptable");

QString YerithTableauxDeBordWindow::OPERATION_GENERER_CHIFFRE_DAFFAIRE("générer le chiffre d'affaire");


QString YerithTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE("avec les quantités les plus vendues");
QString YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE("avec les quantités les moins vendues");
QString YerithTableauxDeBordWindow::QUALITE_MEILLEURS("avec les chiffres d'affaires les plus élevés");
QString YerithTableauxDeBordWindow::QUALITE_BENEFICES_PLUS_ELEVES("avec les bénéfices les plus élevés");
QString YerithTableauxDeBordWindow::QUALITE_BENEFICES_MOINS_ELEVES("avec les bénéfices les moins élevés");
QString YerithTableauxDeBordWindow::QUALITE_ZERO("avec les chiffres d'affaires zéro");
QString YerithTableauxDeBordWindow::QUALITE_DERNIERS("avec les chiffres d'affaires les moins élevés");


QString YerithTableauxDeBordWindow::OBJET_ARTICLES("products");
QString YerithTableauxDeBordWindow::OBJET_CATEGORIES("categories");
QString YerithTableauxDeBordWindow::OBJET_CAISSIERS("cashiers");
QString YerithTableauxDeBordWindow::OBJET_CLIENTS("customers");
QString YerithTableauxDeBordWindow::OBJET_FOURNISSEURS("suppliers");
QString YerithTableauxDeBordWindow::OBJET_VILLES("cities");
QString YerithTableauxDeBordWindow::OBJET_QUARTIER("client location");
QString YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS("regions, states");
QString YerithTableauxDeBordWindow::OBJET_SERVICES("services");
QString YerithTableauxDeBordWindow::GRAPHE_BAR_CHART("bar chart");
QString YerithTableauxDeBordWindow::GRAPHE_PIE_CHART("pie chart");


const QString YerithTableauxDeBordWindow::YERITH_DTL_SET_BAR_PIE_COLOR_ARRAY[12] =
{
    "forestgreen",
    "firebrickred",
    "mediumblue",
    "yellow",
    "lbcolor",
    "purplish",
    "medgrey",
    "orange",
    "listingray",
    "blue",
    "green",
    "brown"
};


const unsigned int YerithTableauxDeBordWindow::QUANTITE_9 = 9;


QString YerithTableauxDeBordWindow::MOIS_1("");
QString YerithTableauxDeBordWindow::MOIS_2("");
QString YerithTableauxDeBordWindow::MOIS_3("");
QString YerithTableauxDeBordWindow::MOIS_4("");
QString YerithTableauxDeBordWindow::MOIS_5("");
QString YerithTableauxDeBordWindow::MOIS_6("");
QString YerithTableauxDeBordWindow::MOIS_7("");
QString YerithTableauxDeBordWindow::MOIS_8("");
QString YerithTableauxDeBordWindow::MOIS_9("");
QString YerithTableauxDeBordWindow::MOIS_10("");
QString YerithTableauxDeBordWindow::MOIS_11("");
QString YerithTableauxDeBordWindow::MOIS_12("");


const unsigned int YerithTableauxDeBordWindow::MAX_YEARS_REPORTS(10);

const double YerithTableauxDeBordWindow::STATS_MIN_VALUE(0.0009);


YerithTableauxDeBordWindow::YerithTableauxDeBordWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithRapportsWindow")),
 _objetServiceLastIndex(-1),
 _objetClientLastIndex(-1),
 _csvFileItemSize(0),
 _startYear(0),
 _GENERATE_THREE_D_BARD_DIAGRAM(false),
 _curStocksVenduTableModel(&_allWindows->getSqlTableModel_stocks_vendu())
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("tableaux de bords"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_INDIGO_83_0_125,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    comboBox_bilan_comptable_SUCCURSALE->setYerithEditable(true);

    comboBox_operations_SUCCURSALE->setYerithEditable(true);

    comboBox_operations_chiffre_SUCCURSALE->setYerithEditable(true);

    comboBox_evolution_objets_value->setYerithEditable(true);

    populateComboBoxes();


    setupDateTimeEdits_EVOLUTION_DES_CHIFFRES_DAFFAIRES();


    setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES();


    setupDateTimeEdits_BILAN_COMPTABLE();


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    _moisToNombre[MOIS_1] = 1;
    _moisToNombre[MOIS_2] = 2;
    _moisToNombre[MOIS_3] = 3;
    _moisToNombre[MOIS_4] = 4;
    _moisToNombre[MOIS_5] = 5;
    _moisToNombre[MOIS_6] = 6;
    _moisToNombre[MOIS_7] = 7;
    _moisToNombre[MOIS_8] = 8;
    _moisToNombre[MOIS_9] = 9;
    _moisToNombre[MOIS_10] = 10;
    _moisToNombre[MOIS_11] = 11;
    _moisToNombre[MOIS_12] = 12;


    connect(action_TRADUIRE_EN_ANGLAIS,
            SIGNAL(triggered()),
            this,
            SLOT(CHANGER_DE_LANGUE()));


    // Menu actions
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));


    connect(checkBox_analyse_comparee,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_checkBox_analyse_comparee(int)));


    connect(radioButton_mensuel,
            SIGNAL(toggled(bool)),
            this,
            SLOT(handle_tab_business_turnover_progress_radio_button(bool)));

    connect(radioButton_jour_semaine,
            SIGNAL(toggled(bool)),
            this,
            SLOT(handle_tab_business_turnover_progress_radio_button(bool)));


    connect(comboBox_bilan_comptable_SUCCURSALE,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_comboBox_bilan_comptable_SUCCURSALE_current_text
                 (const QString &)));

    connect(comboBox_operations_SUCCURSALE,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_comboBox_operations_SUCCURSALE_current_text
                 (const QString &)));

    connect(comboBox_operations_chiffre_SUCCURSALE,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_comboBox_operations_chiffre_SUCCURSALE_current_text
                 (const QString &)));


    connect(comboBox_qualite,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER
                 (const QString &)));


    changeLineEditEvolutionObjetsTextSetup(YerithTableauxDeBordWindow::OBJET_ARTICLES);


    connect(comboBox_evolution_objets,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(changeLineEditEvolutionObjetsTextSetup(const QString &)));


    setupTab_BILAN_COMPTABLE();

    setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE();


#ifdef YERITH_CLIENT
    actionAdministration->setEnabled(false);
#else
    //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
#endif


    connect(tabWidget_rapports, SIGNAL(currentChanged(int)), this,
            SLOT(handleTabChanged(int)));


    checkBox_analyse_comparee->setChecked(true);


    handleTabChanged(SUJET_ACTION_FINANCIAL_ACCOUNTING_REPORT_GENERATION);


    setupShortcuts();
}


bool YerithTableauxDeBordWindow::check_fields_FINANCIAL_ACCOUNTING_REPORT()
{
    bool result = true;

    result = result && comboBox_bilan_comptable_SUCCURSALE->checkField(true);

    return result;
}


bool YerithTableauxDeBordWindow::check_fields_BUSINESS_TURNOVER_COMPARISON()
{
    bool result = true;

    result = result && lineEdit_quantite->checkField();

    result = result
             && comboBox_operations_chiffre_SUCCURSALE->checkField(true);

    return result;
}


bool YerithTableauxDeBordWindow::check_fields_BUSINESS_TURNOVER_PROGRESS()
{
    bool result = true;

    result = result && comboBox_operations_SUCCURSALE->checkField(true);

    //QDEBUG_STRING_OUTPUT_2("YerithTableauxDeBordWindow::OBJET_ARTICLES - 5", YerithTableauxDeBordWindow::OBJET_ARTICLES);
    //QDEBUG_STRING_OUTPUT_2("comboBox_evolution_objets->currentText() - 5", comboBox_evolution_objets->currentText());

    if (!YerithUtils::isEqualCaseInsensitive
            (YerithTableauxDeBordWindow::OBJET_ARTICLES,
             comboBox_evolution_objets->currentText())
            && !YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::
                                                    OBJET_QUARTIER,
                                                    comboBox_evolution_objets->
                                                    currentText())
            && !YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::
                                                    OBJET_VILLES,
                                                    comboBox_evolution_objets->
                                                    currentText())
            && !YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::
                                                    OBJET_REGIONS_ETATS,
                                                    comboBox_evolution_objets->
                                                    currentText()))
    {
        result = result && comboBox_evolution_objets_value->checkField(true);
    }
    else
    {
        //QDEBUG_STRING_OUTPUT_2("comboBox_evolution_objets_value->currentText() - 5", comboBox_evolution_objets_value->text());
        result = result && comboBox_evolution_objets_value->checkField();
        //QDEBUG_STRING_OUTPUT_2("comboBox_evolution_objets_value->currentText() - 6", comboBox_evolution_objets_value->text());
    }

    return result;
}


void YerithTableauxDeBordWindow::handle_checkBox_analyse_comparee(int new_tate)
{
    if (checkBox_analyse_comparee->isChecked())
    {
        comboBox_ANALYSE_COMPAREE->setYerithEnabled(true);
    }
    else
    {
        comboBox_ANALYSE_COMPAREE->setYerithEnabled(false);
    }
}


void
YerithTableauxDeBordWindow::
handle_tab_business_turnover_progress_radio_button(bool toggled)
{
    if (radioButton_mensuel->isChecked())
    {
        handle_enabled_chiffre_daffaire_mois(true);

        handle_enabled_chiffre_daffaire_jour_semaine(false);
    }
    else if (radioButton_jour_semaine->isChecked())
    {
        handle_enabled_chiffre_daffaire_jour_semaine(true);

        handle_enabled_chiffre_daffaire_mois(false);
    }
}


void YerithTableauxDeBordWindow::populateComboBoxes()
{
    comboBox_bilan_comptable_SUCCURSALE->populateComboBoxRawString
    (YerithDatabase::LOCALISATIONS,
     YerithDatabaseTableColumn::NOM_LOCALISATION);

    comboBox_operations_SUCCURSALE->
    populateComboBoxRawString(YerithDatabase::LOCALISATIONS,
                              YerithDatabaseTableColumn::NOM_LOCALISATION);

    comboBox_operations_chiffre_SUCCURSALE->populateComboBoxRawString
    (YerithDatabase::LOCALISATIONS,
     YerithDatabaseTableColumn::NOM_LOCALISATION);


    comboBox_bilan_comptable_SUCCURSALE->addItem_AFTER_POPULATE
    (YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    comboBox_operations_SUCCURSALE->
    addItem_AFTER_POPULATE(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    comboBox_operations_chiffre_SUCCURSALE->addItem_AFTER_POPULATE
    (YerithERPConfig::THIS_SITE_LOCALISATION_NAME);


    comboBox_bilan_comptable_SUCCURSALE->find_AND_SET_CURRENT_INDEX
    (YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    comboBox_operations_SUCCURSALE->find_AND_SET_CURRENT_INDEX
    (YerithERPConfig::THIS_SITE_LOCALISATION_NAME);

    comboBox_operations_chiffre_SUCCURSALE->find_AND_SET_CURRENT_INDEX
    (YerithERPConfig::THIS_SITE_LOCALISATION_NAME);


    _current_succursale_STRING = YerithERPConfig::THIS_SITE_LOCALISATION_NAME;
}


void YerithTableauxDeBordWindow::handleTabChanged(int index)
{
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);

    _GENERATE_THREE_D_BARD_DIAGRAM = false;

    actionAfficherPDF->disconnect();
    actionGenererPDF->disconnect();
    actionReinitialiserRecherche->disconnect();

    if (index == SUJET_ACTION_FINANCIAL_ACCOUNTING_REPORT_GENERATION)
    {
        setWindowTitle(QObject::tr("%1 - bilan comptable").
                       arg(_windowName));

        connect(actionAfficherPDF,
                SIGNAL(triggered()),
                this,
                SLOT(PRINT_with_Progress_BAR_bilanComptable()));

        connect(actionGenererPDF,
                SIGNAL(triggered()),
                this,
                SLOT(PRINT_with_Progress_BAR_bilanComptable()));

        connect(actionReinitialiserRecherche,
                SIGNAL(triggered()),
                this,
                SLOT(reinitialiser_bilan_comptable()));

        check_fields_FINANCIAL_ACCOUNTING_REPORT();
    }
    else if (index == SUJET_ACTION_BUSINESS_TURNOVER_COMPARISON)
    {
        remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER(comboBox_qualite->currentText());

        setWindowTitle(QObject::tr
                       ("%1 - PALMARÈS des chiffres d'affaires").arg
                       (_windowName));

        connect(actionAfficherPDF,
                SIGNAL(triggered()),
                this,
                SLOT(PRINT_with_Progress_BAR_generer()));

        connect(actionGenererPDF,
                SIGNAL(triggered()),
                this,
                SLOT(PRINT_with_Progress_BAR_generer()));

        connect(actionReinitialiserRecherche,
                SIGNAL(triggered()),
                this,
                SLOT(reinitialiser()));

        check_fields_BUSINESS_TURNOVER_COMPARISON();
    }
    else				// SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS
    {
        setWindowTitle(QObject::tr
                       ("%1 - évolution du chiffre d'affaire").arg
                       (_windowName));


        connect(actionAfficherPDF,
                SIGNAL(triggered()),
                this,
                SLOT(PRINT_with_Progress_BAR_choisirEvolutionDuChiffreDaffaire()));


        connect(actionGenererPDF,
                SIGNAL(triggered()),
                this,
                SLOT(PRINT_with_Progress_BAR_choisirEvolutionDuChiffreDaffaire()));


        connect(actionReinitialiserRecherche,
                SIGNAL(triggered()),
                this,
                SLOT(reinitialiser_chiffre_affaire()));


        check_fields_BUSINESS_TURNOVER_PROGRESS();
    }
}


QString
YerithTableauxDeBordWindow::GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING
(uint position)
{
    if (position >= 0)
    {
        uint
        real_position = 1 + position;

        if (position <= 11)
        {
            return QString("\\DTLsetpiesegmentcolor{%1}{%2}").
                   arg(QString::number(real_position),
                       YERITH_DTL_SET_BAR_PIE_COLOR_ARRAY[position]);
        }
        else
        {
            return QString("\\DTLsetpiesegmentcolor{%1}{%2}").
                   arg(QString::number(real_position),
                       YERITH_DTL_SET_BAR_PIE_COLOR_ARRAY[(position - 1) % 11]);
        }
    }

    return YerithUtils::EMPTY_STRING;
}


QString
YerithTableauxDeBordWindow::
GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(uint size)
{
    QString
    resultSTR;

    for (uint k = 0; k < size; ++k)
    {
        resultSTR.append(QString("%1\n").arg
                         (YerithTableauxDeBordWindow::
                          GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING(k)));
    }

    //QDEBUG_STRING_OUTPUT_2("GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX", resultSTR);

    return resultSTR;
}


QString YerithTableauxDeBordWindow::GET_YERITH_DTL_SET_BAR_COLOR_STRING(uint
                                                                        position)
{
    if (position >= 0)
    {
        uint real_position = 1 + position;

        if (position <= 11)
        {
            return QString("\\DTLsetbarcolor{%1}{%2}").arg(QString::number
                                                           (real_position),
                                                           YERITH_DTL_SET_BAR_PIE_COLOR_ARRAY
                                                           [position]);
        }
        else
        {
            return QString("\\DTLsetbarcolor{%1}{%2}").arg(QString::number
                                                           (real_position),
                                                           YERITH_DTL_SET_BAR_PIE_COLOR_ARRAY
                                                           [(position -
                                                             1) % 11]);
        }
    }

    return YerithUtils::EMPTY_STRING;
}


QString
YerithTableauxDeBordWindow::GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX
(uint size)
{
    QString
    resultSTR;

    for (uint k = 0; k < size; ++k)
    {
        resultSTR.append(QString("%1\n").arg
                         (YerithTableauxDeBordWindow::
                          GET_YERITH_DTL_SET_BAR_COLOR_STRING(k)));
    }

    //QDEBUG_STRING_OUTPUT_2("GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX", resultSTR);

    return resultSTR;
}


void YerithTableauxDeBordWindow::setupDateTimeEdits_EVOLUTION_DES_CHIFFRES_DAFFAIRES()
{
    dateEdit_chiffre_daffaire_jour_semaine_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_chiffre_daffaire_jour_semaine_fin->setStartDate(GET_CURRENT_DATE);
}


void YerithTableauxDeBordWindow::setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES()
{
    dateEdit_rapports_debut
        ->setStartDate(YerithERPConfig::GET_YERITH_BUSINESS_DASHBOARD_DEFAULT_START_DATE());

    dateEdit_rapports_fin->setStartDate(GET_CURRENT_DATE);
}


void YerithTableauxDeBordWindow::setupDateTimeEdits_BILAN_COMPTABLE()
{
    dateEdit_bilan_comptable_debut
        ->setStartDate(YerithERPConfig::GET_YERITH_BUSINESS_DASHBOARD_DEFAULT_START_DATE());

    dateEdit_bilan_comptable_fin->setStartDate(GET_CURRENT_DATE);
}


void YerithTableauxDeBordWindow::setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE()
{
    int comboBox_evolution_objets_value_IDX =       comboBox_evolution_objets_value->currentIndex();
    int comboBox_ANALYSE_COMPAREE_IDX =             comboBox_ANALYSE_COMPAREE->currentIndex();
    int comboBox_evolution_objets_IDX =             comboBox_evolution_objets->currentIndex();
    int comboBox_mois_debut_chiffre_affaire_IDX =   comboBox_mois_debut_chiffre_affaire->currentIndex();
    int comboBox_mois_fin_chiffre_affaire_IDX =     comboBox_mois_fin_chiffre_affaire->currentIndex();


    comboBox_evolution_objets_value->clear();
    comboBox_ANALYSE_COMPAREE->clear();
    comboBox_operations_chiffre->clear();
    comboBox_evolution_objets->clear();
    comboBox_mois_debut_chiffre_affaire->clear();
    comboBox_mois_fin_chiffre_affaire->clear();


    radioButton_jour_semaine->setChecked(true);

    handle_enabled_chiffre_daffaire_mois(false);

    handle_enabled_chiffre_daffaire_jour_semaine(true);


    comboBox_ANALYSE_COMPAREE->addItem(ANALYSE_COMPAREE_ACHATS_VENTES);
    comboBox_ANALYSE_COMPAREE->addItem(ANALYSE_COMPAREE_VENTES_BENEFICES);


    comboBox_operations_chiffre->addItem(YerithTableauxDeBordWindow::OPERATION_GENERER_CHIFFRE_DAFFAIRE);


    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_ARTICLES);
    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_CAISSIERS);
    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_CATEGORIES);
    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_CLIENTS);
    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);
    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_QUARTIER);
    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);
    comboBox_evolution_objets->addItem(YerithTableauxDeBordWindow::OBJET_VILLES);

    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_1);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_2);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_3);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_4);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_5);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_6);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_7);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_8);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_9);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_10);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_11);
    comboBox_mois_debut_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_12);

    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_1);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_2);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_3);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_4);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_5);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_6);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_7);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_8);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_9);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_10);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_11);
    comboBox_mois_fin_chiffre_affaire->addItem(YerithTableauxDeBordWindow::MOIS_12);


    if (-1 != comboBox_ANALYSE_COMPAREE_IDX)
    {
        comboBox_ANALYSE_COMPAREE->setCurrentIndex(comboBox_ANALYSE_COMPAREE_IDX);
    }

    if (-1 != comboBox_evolution_objets_IDX)
    {
        comboBox_evolution_objets->setCurrentIndex(comboBox_evolution_objets_IDX);
    }

    if (-1 != comboBox_mois_debut_chiffre_affaire_IDX)
    {
        comboBox_mois_debut_chiffre_affaire->setCurrentIndex(comboBox_mois_debut_chiffre_affaire_IDX);
    }

    if (-1 != comboBox_mois_fin_chiffre_affaire_IDX)
    {
        comboBox_mois_fin_chiffre_affaire->setCurrentIndex(comboBox_mois_fin_chiffre_affaire_IDX);
    }


    check_fields_BUSINESS_TURNOVER_PROGRESS();


    if (-1 != comboBox_evolution_objets_value_IDX)
    {
        comboBox_evolution_objets_value->setCurrentIndex(comboBox_evolution_objets_value_IDX);
    }
}


void YerithTableauxDeBordWindow::setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES()
{
    int comboBox_operations_IDX =   comboBox_operations->currentIndex();
    int comboBox_qualite_IDX =      comboBox_qualite->currentIndex();
    int comboBox_objets_IDX =       comboBox_objets->currentIndex();
    int comboBox_type_graphes_IDX = comboBox_type_graphes->currentIndex();

    comboBox_operations->clear();
    comboBox_qualite->clear();
    comboBox_objets->clear();
    comboBox_type_graphes->clear();


    comboBox_operations->addItem(YerithTableauxDeBordWindow::OPERATION_GENERER);

    comboBox_qualite->addItem(YerithTableauxDeBordWindow::QUALITE_BENEFICES_PLUS_ELEVES);
    comboBox_qualite->addItem(YerithTableauxDeBordWindow::QUALITE_BENEFICES_MOINS_ELEVES);
    comboBox_qualite->addItem(YerithTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE);
    comboBox_qualite->addItem(YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE);
    comboBox_qualite->addItem(YerithTableauxDeBordWindow::QUALITE_MEILLEURS);
    comboBox_qualite->addItem(YerithTableauxDeBordWindow::QUALITE_DERNIERS);
    comboBox_qualite->addItem(YerithTableauxDeBordWindow::QUALITE_ZERO);

    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_ARTICLES);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_CAISSIERS);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_CATEGORIES);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_CLIENTS);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_QUARTIER);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_SERVICES);
    comboBox_objets->addItem(YerithTableauxDeBordWindow::OBJET_VILLES);

    comboBox_type_graphes->addItem(YerithTableauxDeBordWindow::GRAPHE_BAR_CHART);
    comboBox_type_graphes->addItem(YerithTableauxDeBordWindow::GRAPHE_PIE_CHART);


    if (-1 != comboBox_operations_IDX)
    {
        comboBox_operations->setCurrentIndex(comboBox_operations_IDX);
    }

    if (-1 != comboBox_qualite_IDX)
    {
        comboBox_qualite->setCurrentIndex(comboBox_qualite_IDX);
    }

    if (-1 != comboBox_objets_IDX)
    {
        comboBox_objets->setCurrentIndex(comboBox_objets_IDX);
    }

    if (-1 != comboBox_type_graphes_IDX)
    {
        comboBox_type_graphes->setCurrentIndex(comboBox_type_graphes_IDX);
    }


    check_fields_BUSINESS_TURNOVER_COMPARISON();
}


void YerithTableauxDeBordWindow::setupTab_BILAN_COMPTABLE()
{
    comboBox_bilan_comptable_operation->clear();

    comboBox_bilan_comptable_operation->addItem(YerithTableauxDeBordWindow::OPERATION_GENERER_BILAN_COMPTABLE);

    check_fields_FINANCIAL_ACCOUNTING_REPORT();
}


void YerithTableauxDeBordWindow::rendreInvisible()
{
    _GENERATE_THREE_D_BARD_DIAGRAM = false;
    YerithWindowsCommons::rendreInvisible();
}


void YerithTableauxDeBordWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
    OPERATION_GENERER =                     QObject::tr("générer les");

    OPERATION_GENERER_BILAN_COMPTABLE =     QObject::tr("générer le bilan comptable");

    OPERATION_GENERER_CHIFFRE_DAFFAIRE =    QObject::tr("générer le chiffre d'affaire");

    ANALYSE_COMPAREE_VENTES_BENEFICES =     QObject::tr("ANALYSE COMPARÉ VENTES BÉNÉFICES");

    ANALYSE_COMPAREE_ACHATS_VENTES =        QObject::tr("ANALYSE COMPARÉ ACHATS VENTES");

    QUALITE_PLUS_VENDU_PAR_QUANTITE =   QObject::tr("avec les quantités les plus vendues");
    QUALITE_MOINS_VENDU_PAR_QUANTITE =  QObject::tr("avec les quantités les moins vendues");
    QUALITE_MEILLEURS =                 QObject::tr("avec les chiffres d'affaires les plus élevés");
    QUALITE_BENEFICES_PLUS_ELEVES =     QObject::tr("avec les bénéfices les plus élevés");
    QUALITE_BENEFICES_MOINS_ELEVES =    QObject::tr("avec les bénéfices les moins élevés");
    QUALITE_ZERO =                      QObject::tr("avec les chiffres d'affaires zéro");
    QUALITE_DERNIERS =                  QObject::tr("avec les chiffres d'affaires les moins élevés");

    OBJET_ARTICLES =        QObject::tr("articles");
    OBJET_CATEGORIES =      QObject::tr("catégories");
    OBJET_CAISSIERS =       QObject::tr("caissiers");
    OBJET_CLIENTS =         QObject::tr("clients");
    OBJET_FOURNISSEURS =    QObject::tr("fournisseurs");
    OBJET_VILLES =          QObject::tr("villes");
    OBJET_QUARTIER =        QObject::tr("quartier");
    OBJET_REGIONS_ETATS =   QObject::tr("régions, états");
    OBJET_SERVICES =        QObject::tr("services");
    GRAPHE_BAR_CHART =      QObject::tr("diagramme à bandes");
    GRAPHE_PIE_CHART =      QObject::tr("diagramme circulaire");


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        MOIS_1 = MONTH_YERITH_ERP_3_0_JANUARY_EN;
        MOIS_2 = MONTH_YERITH_ERP_3_0_FEBRUARY_EN;
        MOIS_3 = MONTH_YERITH_ERP_3_0_MARCH_EN;
        MOIS_4 = MONTH_YERITH_ERP_3_0_APRIL_EN;
        MOIS_5 = MONTH_YERITH_ERP_3_0_MAY_EN;
        MOIS_6 = MONTH_YERITH_ERP_3_0_JUNE_EN;
        MOIS_7 = MONTH_YERITH_ERP_3_0_JULY_EN;
        MOIS_8 = MONTH_YERITH_ERP_3_0_AUGUST_EN;
        MOIS_9 = MONTH_YERITH_ERP_3_0_SEPTEMBER_EN;
        MOIS_10 = MONTH_YERITH_ERP_3_0_OCTOBER_EN;
        MOIS_11 = MONTH_YERITH_ERP_3_0_NOVEMBER_EN;
        MOIS_12 = MONTH_YERITH_ERP_3_0_DECEMBER_EN;

    }
    else
    {
        MOIS_1 = MONTH_YERITH_ERP_3_0_JANUARY_FR;
        MOIS_2 = MONTH_YERITH_ERP_3_0_FEBRUARY_FR;
        MOIS_3 = MONTH_YERITH_ERP_3_0_MARCH_FR;
        MOIS_4 = MONTH_YERITH_ERP_3_0_APRIL_FR;
        MOIS_5 = MONTH_YERITH_ERP_3_0_MAY_FR;
        MOIS_6 = MONTH_YERITH_ERP_3_0_JUNE_FR;
        MOIS_7 = MONTH_YERITH_ERP_3_0_JULY_FR;
        MOIS_8 = MONTH_YERITH_ERP_3_0_AUGUST_FR;
        MOIS_9 = MONTH_YERITH_ERP_3_0_SEPTEMBER_FR;
        MOIS_10 = MONTH_YERITH_ERP_3_0_OCTOBER_FR;
        MOIS_11 = MONTH_YERITH_ERP_3_0_NOVEMBER_FR;
        MOIS_12 = MONTH_YERITH_ERP_3_0_DECEMBER_FR;
    }



    retranslateUi(this);


    setupTab_BILAN_COMPTABLE();

    setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE();

    setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES();


    _curStocksTableModel = stocksTableModel;

    _curStocksVenduTableModel = &_allWindows->getSqlTableModel_stocks_vendu();

    QString previousYearStr = comboBox_annee_chiffre_affaire->currentText();

    if (_startYear != YerithERPConfig::annee_depart_rapports_chiffre_affaire_value.toUInt())
    {
        _startYear = YerithERPConfig::annee_depart_rapports_chiffre_affaire_value.toUInt();

        comboBox_annee_chiffre_affaire->clear();

        for (unsigned int k = 0; k < MAX_YEARS_REPORTS; ++k)
        {
            comboBox_annee_chiffre_affaire->addItem(QString::number(_startYear + k));
        }
    }

    unsigned int previousYearIdx = comboBox_annee_chiffre_affaire->findText(previousYearStr);

    if (-1 != comboBox_annee_chiffre_affaire->findText(previousYearStr))
    {
        comboBox_annee_chiffre_affaire->setCurrentIndex(previousYearIdx);
    }


    check_fields_FINANCIAL_ACCOUNTING_REPORT();

    check_fields_BUSINESS_TURNOVER_COMPARISON();

    check_fields_BUSINESS_TURNOVER_PROGRESS();


    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("tableaux de bords"),
    								 _curStocksTableModel);

    setVisible(true);
}


void YerithTableauxDeBordWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Two *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}

void YerithTableauxDeBordWindow::definirManager()
{
    _logger->log("definirManager");

    actionDeconnecter_utilisateur->setEnabled(true);
    actionAlertes->setEnabled(true);
    actionMenu->setEnabled(true);
    actionAdministration->setEnabled(true);

    actionQui_suis_je->setEnabled(true);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer
        ->enable(this,
                 SLOT(PRINT_with_Progress_BAR_choisirEvolutionDuChiffreDaffaire()));

    pushButton_chiffre_affaire_reinitialiser
        ->enable(this,
                 SLOT(reinitialiser_chiffre_affaire()));

    /** Tab Two *************************************/

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser()));

    pushButton_generer->enable(this, SLOT(PRINT_with_Progress_BAR_generer()));

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser
        ->enable(this,
            SLOT(reinitialiser_bilan_comptable()));

    pushButton_bilan_comptable_generer
        ->enable(this,
                 SLOT(PRINT_with_Progress_BAR_bilanComptable()));
}


void YerithTableauxDeBordWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Two *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}


void YerithTableauxDeBordWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    actionDeconnecter_utilisateur->setEnabled(true);
    actionAlertes->setEnabled(true);
    actionMenu->setEnabled(true);
    actionAdministration->setEnabled(false);

    actionQui_suis_je->setEnabled(true);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Two *************************************/

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser()));
    pushButton_generer->enable(this, SLOT(PRINT_with_Progress_BAR_generer()));

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}


void YerithTableauxDeBordWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Two *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}


void YerithTableauxDeBordWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    actionDeconnecter_utilisateur->setEnabled(false);
    actionAlertes->setEnabled(false);
    actionMenu->setEnabled(false);
    actionAdministration->setEnabled(false);
    actionQui_suis_je->setEnabled(false);

    /** Tab One *************************************/

    pushButton_reinitialiser->disable(this);
    pushButton_generer->disable(this);

    /** Tab Two *************************************/

    pushButton_chiffre_affaire_generer->disable(this);
    pushButton_chiffre_affaire_reinitialiser->disable(this);

    /** Tab Three *************************************/

    pushButton_bilan_comptable_reinitialiser->disable(this);
    pushButton_bilan_comptable_generer->disable(this);
}


void YerithTableauxDeBordWindow::CHANGER_DE_LANGUE()
{
    YerithWindowsCommons::CHANGER_DE_LANGUE(this,
                                            action_TRADUIRE_EN_ANGLAIS);

    rendreVisible(_curStocksTableModel);
}


void YerithTableauxDeBordWindow::reinitialiser()
{
    _logger->log("reinitialiser");

    lineEdit_quantite->clear();
    lineEdit_quantite->checkField();

    comboBox_objets->resetYerithComboBox();
    comboBox_qualite->resetYerithComboBox();
    comboBox_type_graphes->resetYerithComboBox();

    dateEdit_rapports_debut->reset();
    dateEdit_rapports_fin->reset();
}

void YerithTableauxDeBordWindow::reinitialiser_chiffre_affaire()
{
    _logger->log("reinitialiser_chiffre_affaire");

    checkBox_analyse_comparee->setChecked(true);

    comboBox_ANALYSE_COMPAREE->resetYerithComboBox();

    comboBox_operations_chiffre->resetYerithComboBox();

    dateEdit_chiffre_daffaire_jour_semaine_debut->reset();
    dateEdit_chiffre_daffaire_jour_semaine_fin->reset();

    comboBox_mois_debut_chiffre_affaire->resetYerithComboBox();
    comboBox_mois_fin_chiffre_affaire->resetYerithComboBox();
    comboBox_annee_chiffre_affaire->resetYerithComboBox();


    //QDEBUG_STRING_OUTPUT_2("comboBox_evolution_objets->currentText() - 3", comboBox_evolution_objets->currentText());


    if (!YerithUtils::isEqualCaseInsensitive
            (YerithTableauxDeBordWindow::OBJET_ARTICLES,
             comboBox_evolution_objets->currentText()))
    {
        comboBox_evolution_objets_value->clearField();
        comboBox_evolution_objets_value->checkField(true);
    }
    else
    {
        comboBox_evolution_objets_value->clearField();
    }
}


void YerithTableauxDeBordWindow::reinitialiser_bilan_comptable()
{
    _logger->log("reinitialiser_bilan_comptable");

    dateEdit_bilan_comptable_debut->reset();
    dateEdit_bilan_comptable_fin->reset();
}


void *YerithTableauxDeBordWindow::generer()
{
    if (YerithTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText()  ||
        YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText())
    {
        emit SIGNAL_INCREMENT_PROGRESS_BAR(50);

        compterLesArticlesVendusParQuantite();
    }
    else if (YerithTableauxDeBordWindow::QUALITE_BENEFICES_PLUS_ELEVES == comboBox_qualite->currentText() ||
             YerithTableauxDeBordWindow::QUALITE_BENEFICES_MOINS_ELEVES == comboBox_qualite->currentText())
    {

        emit SIGNAL_INCREMENT_PROGRESS_BAR(50);

        compterLesArticlesVendusParBENEFICES();
    }
    else
    {
        emit SIGNAL_INCREMENT_PROGRESS_BAR(50);

        rechercher();
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(92);
}


#include "include/YERITH_TABLEAUX_DE_BORDS_quantite_max_moindre.cpp"


#include "include/YERITH_TABLEAUX_DE_BORDS_meilleurs_stats.cpp"


#include "include/YERITH_TABLEAUX_DE_BORDS_zero_stats.cpp"


#include "include/YERITH_TABLEAUX_DE_BORDS_zero_stats_stocks.cpp"


void YerithTableauxDeBordWindow::derniersStats(QString fileName,
                                               QString fieldId,
                                               int size,
                                               bool service /* = false */)
{
    _logger->log("derniersStats");

    QString strQuery;

    if (!service)
    {
        strQuery.append(QString
                        ("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE is_service = '0' AND %6 >= '%7' AND %8 <= '%9'").
                        arg(fieldId,
                            YerithDatabaseTableColumn::QUANTITE_VENDUE,
                            YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                            YerithDatabaseTableColumn::MONTANT_TVA,
                            YerithDatabase::STOCKS_VENDU,
                            YerithDatabaseTableColumn::DATE_VENTE,
                            DATE_TO_DB_FORMAT_STRING
                            (dateEdit_rapports_debut->date()),
                            YerithDatabaseTableColumn::DATE_VENTE,
                            DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->
                                                     date())));
    }
    else
    {
        strQuery.append(QString
                        ("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5"
                         " WHERE date_vente >= '%6' AND date_vente <= '%7' AND"
                         " %8=0 AND %9=1").arg(fieldId,
                                               YerithDatabaseTableColumn::
                                               QUANTITE_VENDUE,
                                               YerithDatabaseTableColumn::
                                               MONTANT_TOTAL_VENTE,
                                               YerithDatabaseTableColumn::
                                               MONTANT_TVA,
                                               YerithDatabase::SERVICES_COMPLETES,
                                               DATE_TO_DB_FORMAT_STRING
                                               (dateEdit_rapports_debut->date
                                                ()),
                                               DATE_TO_DB_FORMAT_STRING
                                               (dateEdit_rapports_fin->date
                                                ()),
                                               YerithDatabaseTableColumn::
                                               MONTANT_A_REMBOURSER,
                                               YerithDatabaseTableColumn::
                                               IS_SERVICE));
    }


    strQuery.append(QString(" AND %1 = '%2'").arg
                    (YerithDatabaseTableColumn::LOCALISATION,
                     _current_succursale_STRING));


    QSqlQuery query;

    unsigned int SIZE_FOR_CSV_FILE_CONTENT =
                    (size > QUANTITE_9) ? QUANTITE_9 : size;

    int querySize = YerithUtils::execQuery(query, strQuery, _logger);

//    qDebug() << "++ query: " << query.executedQuery() << ", querySize: " << querySize;

    QString fieldIdValue;

    double montant_total_vente = 0.0;
    double total = 0.0;

    double quantite_vendue = 0.0;

    QList < Yerith_MAP_COMPLEX_Item * >caissierToVentes;

    if (querySize > 0)
    {
        while (query.next())
        {
            fieldIdValue = query.value(0).toString();

            quantite_vendue = query.value(1).toDouble();

            montant_total_vente = query.value(2).toDouble();

            if (fieldIdValue.isEmpty())
            {
                continue;
            }

            int idx = -1;
            for (int i = 0; i < caissierToVentes.size(); ++i)
            {
                if (YerithUtils::isEqualCaseInsensitive
                        (caissierToVentes.value(i)->_itemName, fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }

            if (-1 == idx)
            {
                Yerith_MAP_COMPLEX_Item *aNewItem =
                                new Yerith_MAP_COMPLEX_Item(fieldIdValue,
                                                            montant_total_vente,
                                                            quantite_vendue);
                caissierToVentes.push_back(aNewItem);
            }
            else
            {
                Yerith_MAP_COMPLEX_Item *anItem = caissierToVentes.value(idx);

                anItem->_itemValue += montant_total_vente;

                anItem->_itemSecondValue += quantite_vendue;
            }

            total += montant_total_vente;
        }
    }

    qSort(caissierToVentes.begin(),
          caissierToVentes.end(), Yerith_MAP_COMPLEX_Item::lessThan);

    //Remove all items with a zero value
    for (int j = 0; j < caissierToVentes.size(); ++j)
    {
        if (0 == caissierToVentes.at(j)->_itemValue)
        {
            caissierToVentes.removeAt(j);
        }
    }

    QString csvFileContent;

    csvFileContent.append("Nom, Total\n");

    QString label;
    QString pourcentageStr;
    QString curValueStr;
    QString curSecondValueStr;

    double pourcentage = 0;
    double curValue = 0;

    _csvFileItemSize = 0;

    _reportTexFileEndString =
                    YerithUtils::LATEX_IN_OUT_handleForeignAccents(QObject::tr
                                                                   ("D\\'etails en %1:").arg
                                                                   (YerithERPConfig::
                                                                    currency));

    _reportTexFileEndString.prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    for (int j = 0, k = 0; j < caissierToVentes.size() && k < size; ++j)
    {
        curValue = caissierToVentes.value(j)->_itemValue;

        pourcentage = (curValue / total) * 100.0;

//        qDebug() << QString("++ j:%1. value: %2, total: %3, pourcentage: %4")
//                                      .arg(QString::number(j, 'f', 2),
//                                               QString::number(curValue, 'f', 2),
//                                               QString::number(total, 'f', 2),
//                                               QString::number(pourcentage, 'f', 2));
//
//        qDebug() << "++ pourcentage: " << QString::number(pourcentage, 'f', 9);

        if (pourcentage <= STATS_MIN_VALUE)
        {
            continue;
        }

        pourcentageStr = QString::number(pourcentage, 'f', 3);

        curValueStr = GET_CURRENCY_STRING_NUM(curValue);

        curSecondValueStr =
                        GET_DOUBLE_STRING(caissierToVentes.value(j)->_itemSecondValue);

        label =
                        QString("\"%1\"").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                                              (caissierToVentes.at(j)->_itemName));

        if (k < SIZE_FOR_CSV_FILE_CONTENT)
        {
            csvFileContent.append(QString("%1, %2\n").arg(label,
                                                          (pourcentage <=
                                                           0.001 ? "0.001" :
                                                           pourcentageStr)));
            ++_csvFileItemSize;
        }

        _reportTexFileEndString.append(QObject::tr
                                       ("\\item %1: \\textbf{%2} (quantité vendue: %3)\n").
                                       arg(label,
                                           YerithUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (curValueStr),
                                           YerithUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (curSecondValueStr)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;
        ++k;
    }

    _reportTexFileEndString.append("\\end{enumerate}\n");

    //clean up
    for (int k = 0; k < caissierToVentes.size(); ++k)
    {
        delete caissierToVentes.value(k);
    }

    QFile csvFile(fileName);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        _logger->log("derniersStats", csvFile.errorString());
    }
    else
    {
        QTextStream outCsvFile(&csvFile);

        outCsvFile << csvFileContent;

        csvFile.close();
    }
}


void YerithTableauxDeBordWindow::rechercher()
{
    _logger->log("rechercher");

    QString objet(comboBox_objets->currentText());

    int size = lineEdit_quantite->text().toInt();

    QStringList progArguments;
    QString tmpFilePrefix;

    QDir tempDir(YerithERPConfig::temporaryFilesDir);

    QString csvFile;
    QString pdfFileTitle;

    if (YerithTableauxDeBordWindow::QUALITE_MEILLEURS == comboBox_qualite->currentText() ||
        YerithTableauxDeBordWindow::QUALITE_DERNIERS == comboBox_qualite->currentText())
    {
        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(QString("The %1 ")
                                 .arg(lineEdit_quantite->text()));

        }
        else
        {
            pdfFileTitle.append(QString("Les %1 ")
                                 .arg(lineEdit_quantite->text()));
        }
    }
    else
    {
        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append("The ");

        }
        else
        {
            pdfFileTitle.append("Les ");
        }

    }

    if (YerithTableauxDeBordWindow::QUALITE_MEILLEURS == comboBox_qualite->currentText())
    {
        if (YerithTableauxDeBordWindow::OBJET_SERVICES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_SERVICES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-services");

            }
            else
            {
                tmpFilePrefix =
                    FILE_NAME_USERID_CURRENT_TIME("meilleurs-services");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursServices(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_VILLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_VILLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-cities");

            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleures-VILLES");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleurs_VILLES(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_QUARTIER == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_QUARTIER);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-client-location");

            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-quartier");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleurs_QUARTIER(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-regions-cities");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-regions-etats");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleurs_REGIONS_ETATS(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_ARTICLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-articles");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-articles");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursArticles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_MEILLEURS,
                                     "best");

                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-categories");
            }
            else
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_MEILLEURS,
                                     "meilleures");

                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleures-categories");
            }

            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CATEGORIES);

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursCategories(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CAISSIERS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-cashiers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-caissiers");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursCaissiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CLIENTS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-customers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-clients");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursClients(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-supplier-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("meilleurs-fournisseurs-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsMeilleursFournisseursVentes(csvFile, size);
        }

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(" giving the best financial income");
        }
        else
        {
            pdfFileTitle.append(" avec les chiffres d'affaires les plus \\'elev\\'es");
        }
    }

    else if (YerithTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
    {
        if (YerithTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_ARTICLES);

            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-articles");

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_Articles(csvFile);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CATEGORIES);

            tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-categories");

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_Categories(csvFile);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CAISSIERS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-cashiers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-caissiers");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_Caissiers(csvFile);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CLIENTS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-turnover-customers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-chiffre-daffaire-clients");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_Clients(csvFile);
        }
        else if (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-suppliers-sales");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-fournisseurs-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_FournisseursVentes(csvFile);
        }
        else if (YerithTableauxDeBordWindow::OBJET_QUARTIER == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_QUARTIER);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-client-location");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-quartier");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_QUARTIER(csvFile);
        }
        else if (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-regions-states");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-regions-etats");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_REGIONS_ETATS(csvFile);
        }
        else if (YerithTableauxDeBordWindow::OBJET_VILLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_VILLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-cities");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("zero-villes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsZERO_VILLES(csvFile);
        }

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append
                (QString(" (quantity: %1) with NO (zero) financial income")
                    .arg(GET_NUM_STRING(_csvFileItemSize)));
        }
        else
        {
            pdfFileTitle.append
                (QString(" (quantité: %1) avec les chiffres d'affaires nuls (0)")
                    .arg(GET_NUM_STRING(_csvFileItemSize)));
        }
    }

    else if (YerithTableauxDeBordWindow::QUALITE_DERNIERS == comboBox_qualite->currentText())
    {
        if (YerithTableauxDeBordWindow::OBJET_SERVICES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_SERVICES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-services");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-services");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersServices(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_ARTICLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-articles");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-articles");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersArticles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_DERNIERS,
                                     "last");
            }
            else
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_DERNIERS,
                                     "derni\\`eres");
            }

            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CATEGORIES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-categories");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("dernieres-categories");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersCategories(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CAISSIERS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-cashiers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-caissiers");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersCaissiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CLIENTS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-customers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-clients");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersClients(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-suppliers-sales");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-fournisseurs-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersFournisseursVentes(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_QUARTIER == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_QUARTIER);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-client-location");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-quartier");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniers_QUARTIERS(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-regions-states");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-regions-etats");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniersREGIONS_ETATS(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_VILLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_VILLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("last-cities");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("derniers-villes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsDerniers_VILLES(csvFile, size);
        }

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(" giving the least financial income");
        }
        else
        {
            pdfFileTitle.append(" avec les chiffres d'affaires les moins \\'elev\\'es");
        }
    }

    if (_csvFileItemSize <= 0)
    {
        QString retMsg(QObject::tr("Il n'y a pas de données correspondante à la requête !\n"
                                   "Vérifier que les dates de début et de fin sont correctes !"));

        YerithQMessageBox::information(this,
                                       QObject::tr("rankings - pas de données !"),
                                       retMsg);
        _csvFileItemSize = 0;

        return;
    }

    QString latexChartTemplate;
    QString latexChartFileNamePrefix;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        if (YerithTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_ZERO_ventes_tex);

            latexChartFileNamePrefix.append(QString("%1/%2_ZERO")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_bar_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETBARCOLOR",
                                       GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-bar-chart")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART ==
                 comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_pie_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETPIESEGMENTCOLOR",
                                       GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-pie-chart")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
    }
    else
    {
        if (YerithTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_ZERO_ventes_tex);

            latexChartFileNamePrefix.append(QString("%1/%2_ZERO")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_BAR_CHART ==
                 comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_bar_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETBARCOLOR",
                                       GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-bar-chart")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART ==
                 comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_pie_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETPIESEGMENTCOLOR",
                                       GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-pie-chart")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
    }


    QString pdfFile(latexChartFileNamePrefix + ".pdf");

    tempDir.remove(pdfFile);

    //qDebug() << "++ csvFile: " << csvFile;

    if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART == comboBox_type_graphes->currentText())
    {
        QString YERITHCUTAWAY;

        for (int k = 1; k <= _csvFileItemSize; ++k)
        {
            YERITHCUTAWAY.append(QString::number(k)).append(",");
            //qDebug() << "++ YERITHCUTAWAY: " << YERITHCUTAWAY;
        }

        YERITHCUTAWAY.remove(YERITHCUTAWAY.length() - 1, 1);

        latexChartTemplate.replace("YERITHCUTAWAY", YERITHCUTAWAY);
    }

    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString statsDate = YerithUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX());

    YerithUtils::getCurrentLocaleDate(statsDate);

    //qDebug() << "++ type fact. rapports: " << YerithConfig::typeOfFacturation;

    latexChartTemplate.replace("YERITHPAPERSPEC", "a4paper");

    latexChartTemplate.replace("YERITHCSVFILE", tmpFilePrefix + ".csv");
    latexChartTemplate.replace("YERITHCHARTFIN", _reportTexFileEndString);

    latexChartTemplate.replace("YERITHUTILISATEUR",
                               QString("%1 %2")
                                .arg(YerithUtils::getAllWindows()->getUser()->titreTex(),
                                   YerithUtils::getAllWindows()->getUser()->nom_completTex()));

    latexChartTemplate.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());

    latexChartTemplate.replace("YERITHBARCHARTTITLE",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents(pdfFileTitle));

    latexChartTemplate.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());

    latexChartTemplate.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());

    latexChartTemplate.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());

    latexChartTemplate.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());

    latexChartTemplate.replace("YERITHPAYS", infoEntreprise.getPaysTex());

    latexChartTemplate.replace("YERITHEMAIL",
                               infoEntreprise.getEmail_LATEX());

    latexChartTemplate.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());

    latexChartTemplate.replace("YERITHDATE", statsDate);

    latexChartTemplate.replace("YERITHVENTESDEBUT",
                               DATE_TO_STRING(dateEdit_rapports_debut->date()));

    latexChartTemplate.replace("YERITHVENTESFIN",
                               DATE_TO_STRING(dateEdit_rapports_fin->date()));

    latexChartTemplate.replace("YERITHSUCCURSALE",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerithERPConfig::THIS_SITE_LOCALISATION_NAME));

    latexChartTemplate.replace("YERITHHEUREGENERATION", CURRENT_TIME);

    latexChartTemplate.replace("YERITHCOMPTEBANCAIRENR",
                               infoEntreprise.getNumeroCompteBancaire());

    latexChartTemplate.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());

    latexChartTemplate.replace("YERITHAGENCECOMPTEBANCAIRE",
                               infoEntreprise.getAgenceCompteBancaireTex());


    QFile latexChartFile(latexChartFileNamePrefix + ".tex");

    if (latexChartFile.open(QFile::WriteOnly))
    {
        latexChartFile.write(latexChartTemplate.toUtf8());
    }

    latexChartFile.close();


    progArguments << "-interaction";
    progArguments << "nonstopmode";
    progArguments << latexChartFile.fileName();

    //qDebug() << "++ -> " << latexChartFile.fileName();

    if (!progArguments.empty())
    {
        QProcess aProcess;

        aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

        aProcess.start(YerithERPConfig::pathToPdfLatex(),
                       progArguments);

        aProcess.waitForFinished(-1);

        progArguments.clear();

        progArguments << pdfFile;

        aProcess.startDetached(YerithERPConfig::pathToPdfReader,
                               progArguments);

        aProcess.waitForFinished();
    }

    _csvFileItemSize = 0;
}


bool YerithTableauxDeBordWindow::export_csv_file()
{
    QString comboBoxQualiteCurrentText(comboBox_qualite->currentText());

    if (!YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::QUALITE_ZERO,
                                             comboBoxQualiteCurrentText))
    {
        return false;
    }

    if (!YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::OBJET_ARTICLES,
                                             comboBox_objets->currentText()))
    {
        return false;
    }


    QString csvFileName("articles-zero-chiffre-daffaire");

    QString yerithStocksListingCSVFileName
                (QString("%1/%2")
                    .arg(YerithERPConfig::temporaryFilesDir,
                         csvFileName));


    yerithStocksListingCSVFileName =
        FILE_NAME_USERID_CURRENT_TIME(yerithStocksListingCSVFileName);


    yerithStocksListingCSVFileName =
        QFileDialog::getSaveFileName(this,
                                     QObject::tr("Saisir le nom du fichier '.csv'"),
                                     yerithStocksListingCSVFileName,
                                     QObject::tr("articles avec chiffre d'affaire nul \"*.csv\" (*.csv)"));


    statsZERO_Articles(yerithStocksListingCSVFileName.append(".csv"));


    _reportTexFileEndString.clear();


    return true;
}


void YerithTableauxDeBordWindow::
        remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER(const QString &comboBoxQualiteCurrentText)
{
    //qDebug() << QString("remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER: %1")
    //                              .arg(comboBoxQualiteCurrentText);
    if (YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::QUALITE_ZERO,
                                            comboBoxQualiteCurrentText))
    {
        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);

        _objetServiceLastIndex =
            comboBox_objets->findText(YerithTableauxDeBordWindow::OBJET_SERVICES);

        if (-1 != _objetServiceLastIndex)
        {
            comboBox_objets->removeItem(_objetServiceLastIndex);
        }


        _objetClientLastIndex =
            comboBox_objets->findText(YerithTableauxDeBordWindow::OBJET_CLIENTS);

        if (-1 != _objetClientLastIndex)
        {
            comboBox_objets->removeItem(_objetClientLastIndex);
        }


        label_CHART->setVisible(false);

        label_comparaison_chiffres_daffaires_quantite->setVisible(false);

        lineEdit_quantite->setVisible(false);

        comboBox_type_graphes->setVisible(false);
    }
    else
    {
        YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

        if (-1 != _objetServiceLastIndex)
        {
            comboBox_objets->insertItem(_objetServiceLastIndex,
                                        YerithTableauxDeBordWindow::OBJET_SERVICES);

            _objetServiceLastIndex = -1;
        }

        if (-1 != _objetClientLastIndex)
        {
            comboBox_objets->insertItem(_objetClientLastIndex,
                                        YerithTableauxDeBordWindow::OBJET_CLIENTS);

            _objetClientLastIndex = -1;
        }

        label_CHART->setVisible(true);

        label_comparaison_chiffres_daffaires_quantite->setVisible(true);

        lineEdit_quantite->setVisible(true);

        comboBox_type_graphes->setVisible(true);
    }
}


/*
 *  !!! MODIFY DB TABLES COLUMNS INDEXES.
 */
void YerithTableauxDeBordWindow::changeLineEditEvolutionObjetsTextSetup(const
                                                                        QString &
                                                                        comboBoxEvolutionObjetsCurrentText)
{
    //qDebug() << QString("changeLineEditEvolutionObjetsTextSetup: %1")
    //                              .arg(comboBoxEvolutionObjetsCurrentText);
    if (YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerithTableauxDeBordWindow::OBJET_ARTICLES))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::MARCHANDISES, YerithDatabaseTableColumn::DESIGNATION,
         QString("%1 = 0").arg(YerithDatabaseTableColumn::IS_SERVICE));
        checkBox_analyse_comparee->setYerithEnabled(true);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_CAISSIERS))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::USERS, YerithDatabaseTableColumn::NOM_COMPLET,
         QString("%1 != %2").arg(YerithDatabaseTableColumn::ROLE,
                                 QString::number
                                 (YerithUtils::ROLE_ADMINISTRATEUR)));
        checkBox_analyse_comparee->setYerithEnabled(true);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_CATEGORIES))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::CATEGORIES,
         YerithDatabaseTableColumn::NOM_CATEGORIE);
        checkBox_analyse_comparee->setYerithEnabled(true);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_CLIENTS))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::CLIENTS, YerithDatabaseTableColumn::NOM_ENTREPRISE);
        checkBox_analyse_comparee->setYerithEnabled(true);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_FOURNISSEURS))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::FOURNISSEURS,
         YerithDatabaseTableColumn::NOM_ENTREPRISE);
        checkBox_analyse_comparee->setYerithEnabled(true);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_QUARTIER))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::CLIENTS, YerithDatabaseTableColumn::QUARTIER);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::CLIENTS, YerithDatabaseTableColumn::PROVINCE_ETAT);
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_VILLES))
    {
        comboBox_evolution_objets_value->populateComboBoxRawString
        (YerithDatabase::CLIENTS, YerithDatabaseTableColumn::VILLE);
    }

    check_fields_BUSINESS_TURNOVER_PROGRESS();
}


void *YerithTableauxDeBordWindow::bilanComptable()
{
    _logger->log("bilanComptable");

    if (dateEdit_bilan_comptable_debut->date() >
            dateEdit_bilan_comptable_fin->date())
    {
        YerithQMessageBox::warning(this, QObject::tr("bilan comptable"),
                                   QObject::tr
                                   ("La date de 'début' doit être"
                                    " antérieure à la date de 'fin' !"));

        return 0;
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);


    QSqlQuery query;

    QString
    strDetteClientelleQuery(QString("SELECT %1 FROM %2 WHERE %3 < 0").arg
                            (YerithDatabaseTableColumn::COMPTE_CLIENT,
                             YerithDatabase::CLIENTS,
                             YerithDatabaseTableColumn::COMPTE_CLIENT));

    strDetteClientelleQuery.append(QString(" AND %1 = '%2'").arg
                                   (YerithDatabaseTableColumn::LOCALISATION,
                                    _current_succursale_STRING));

//    qDebug() << QString("++ strAchatsQuery: %1")
//                              .arg(strAchatsQuery);

    int detteClientelleSize =
                    YerithUtils::execQuery(query, strDetteClientelleQuery, _logger);

    double dette_clientelle = 0.0;

    double montant_total_dette_clientelle = 0.0;

    for (int k = 0; k < detteClientelleSize && query.next(); ++k)
    {
        dette_clientelle =
                        query.value(YerithDatabaseTableColumn::COMPTE_CLIENT).toDouble();

        montant_total_dette_clientelle =
                        montant_total_dette_clientelle + dette_clientelle;
    }

//    qDebug() << QString("++ detteClientelleSize: %1, montant_total_dette_clientelle: %2")
//                              .arg(QString::number(detteClientelleSize),
//                                       QString::number(montant_total_dette_clientelle, 'f', 2));

    query.clear();

    QString
    strAchatsQuery(QString
                   ("SELECT %1, %2 FROM %3 WHERE %4 >= '%5' AND %6 <= '%7'").
                   arg(YerithDatabaseTableColumn::PRIX_DACHAT,
                       YerithDatabaseTableColumn::QUANTITE_TOTALE,
                       YerithDatabase::ACHATS,
                       YerithDatabaseTableColumn::DATE_ENTREE,
                       DATE_TO_DB_FORMAT_STRING
                       (dateEdit_bilan_comptable_debut->date()),
                       YerithDatabaseTableColumn::DATE_ENTREE,
                       DATE_TO_DB_FORMAT_STRING
                       (dateEdit_bilan_comptable_fin->date())));


    strAchatsQuery.append(QString(" AND %1 = '%2'").arg
                          (YerithDatabaseTableColumn::LOCALISATION,
                           _current_succursale_STRING));


//    qDebug() << QString("++ strAchatsQuery: %1")
//                              .arg(strAchatsQuery);

    int achatQuerySize =
                    YerithUtils::execQuery(query, strAchatsQuery, _logger);

    double montant_total_achat = 0.0;

    double total_achat = 0.0;

    double prix_dachat = 0.0;

    double qte_total = 0.0;

    for (int k = 0; k < achatQuerySize && query.next(); ++k)
    {
        prix_dachat = query.value(0).toDouble();

        qte_total = query.value(1).toDouble();

        total_achat = qte_total * prix_dachat;

        montant_total_achat = montant_total_achat + total_achat;
    }

    // We negate stock item buying amount because it is
    // an expense of the company !
    montant_total_achat = -1 * montant_total_achat;

//    qDebug() << QString("++ achatQuerySize: %1, montant_total_achat: %2")
//                              .arg(QString::number(achatQuerySize),
//                                       QString::number(montant_total_achat, 'f', 2));


    //CALCUL TVA ENGRANGE POUR SERVICES NON STOCKES.


    emit SIGNAL_INCREMENT_PROGRESS_BAR(20);


    query.clear();

    QString strVentes__SERVICES_NON_STOCKES_charge_TVA__Query =
                    QString("SELECT %1 FROM %2 WHERE %3 >= '%4' AND %5 <= '%6'").arg
                    (YerithDatabaseTableColumn::MONTANT_TVA,
                     YerithDatabase::SERVICES_COMPLETES,
                     YerithDatabaseTableColumn::DATE_VENTE,
                     DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_debut->date()),
                     YerithDatabaseTableColumn::DATE_VENTE,
                     DATE_TO_DB_FORMAT_STRING(dateEdit_bilan_comptable_fin->date()));


    strVentes__SERVICES_NON_STOCKES_charge_TVA__Query.append(QString
                                                             (" AND %1 = '%2'").
                                                             arg
                                                             (YerithDatabaseTableColumn::LOCALISATION,
                                                              _current_succursale_STRING));


    //    qDebug() << QString("++ strVentesQuery: %1")
    //                                  .arg(strVentesQuery);

    int ventes__SERVICES_NON_STOCKES_TVA_QuerySize =
                    YerithUtils::execQuery(query,
                                           strVentes__SERVICES_NON_STOCKES_charge_TVA__Query,
                                           _logger);


    double montant_tva = 0.0;

    double montant_TOTAL_TVA_COLLECTE = 0.0;

    for (int k = 0;
            k < ventes__SERVICES_NON_STOCKES_TVA_QuerySize && query.next(); ++k)
    {
        montant_tva =
                        query.value(YerithDatabaseTableColumn::MONTANT_TVA).toDouble();

        montant_TOTAL_TVA_COLLECTE = montant_TOTAL_TVA_COLLECTE + montant_tva;
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(40);


    //VENTES D'ARTICLES PHYSIQUES
    query.clear();

    QString
    strVentesQuery(QString
                   ("SELECT stocks_id, %1, %2, %3, (%4 - %5) FROM stocks_vendu WHERE %6 >= '%7' AND %8 <= '%9'").
                   arg(YerithDatabaseTableColumn::MONTANT_TVA,
                       YerithDatabaseTableColumn::QUANTITE_VENDUE,
                       YerithDatabaseTableColumn::REMISE_PRIX,
                       YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                       YerithDatabaseTableColumn::MONTANT_TVA,
                       YerithDatabaseTableColumn::DATE_VENTE,
                       DATE_TO_DB_FORMAT_STRING
                       (dateEdit_bilan_comptable_debut->date()),
                       YerithDatabaseTableColumn::DATE_VENTE,
                       DATE_TO_DB_FORMAT_STRING
                       (dateEdit_bilan_comptable_fin->date())));


    strVentesQuery.append(QString(" AND %1 = '%2'").arg
                          (YerithDatabaseTableColumn::LOCALISATION,
                           _current_succursale_STRING));


//    qDebug() << QString("++ strVentesQuery: %1")
//                              .arg(strVentesQuery);

    int ventesQuerySize =
                    YerithUtils::execQuery(query, strVentesQuery, _logger);

    int stocks_id = -1;

    QMap < int, StockQteVendueEtRemiseTotalSurVente * >stocksidToqtevendue;

    double qte_vendue = 0.0;

    double remise_prix_vente = 0.0;

    double total_remise = 0.0;

    double total_vente = 0.0;

    double montant_total_vente = 0.0;

    for (int k = 0; k < ventesQuerySize && query.next(); ++k)
    {
        stocks_id = query.value(YerithDatabaseTableColumn::STOCKS_ID).toInt();

        qte_vendue =
                        query.value(YerithDatabaseTableColumn::QUANTITE_VENDUE).toDouble();

        remise_prix_vente =
                        query.value(YerithDatabaseTableColumn::REMISE_PRIX).toDouble();

        total_remise = qte_vendue * remise_prix_vente;

        total_vente = query.value(4).toDouble();

        montant_tva =
                        query.value(YerithDatabaseTableColumn::MONTANT_TVA).toDouble();

        montant_TOTAL_TVA_COLLECTE = montant_TOTAL_TVA_COLLECTE + montant_tva;

        if (stocksidToqtevendue.contains(stocks_id))
        {
            StockQteVendueEtRemiseTotalSurVente *curValue =
                            stocksidToqtevendue.value(stocks_id);

            curValue->_qteVendue = curValue->_qteVendue + qte_vendue;
            curValue->_remiseTotalSurVentes =
                            curValue->_remiseTotalSurVentes + total_remise;

            stocksidToqtevendue.insert(stocks_id, curValue);
        }
        else
        {
            StockQteVendueEtRemiseTotalSurVente *curValue =
                            new StockQteVendueEtRemiseTotalSurVente;

            curValue->_qteVendue = qte_vendue;
            curValue->_remiseTotalSurVentes =
                            curValue->_remiseTotalSurVentes + total_remise;

            stocksidToqtevendue.insert(stocks_id, curValue);
        }

        montant_total_vente = montant_total_vente + total_vente;
    }

    // We negate COLLECTED V.A.T because it doesn't belong to company !
    montant_TOTAL_TVA_COLLECTE = -1 * montant_TOTAL_TVA_COLLECTE;


    emit SIGNAL_INCREMENT_PROGRESS_BAR(60);


    //    qDebug() << QString("++ ventesQuerySize: %1, montant_total_vente: %2")
    //                                  .arg(QString::number(ventesQuerySize),
    //                                           QString::number(montant_total_vente, 'f', 2));

    query.clear();

    QString
    strVersementsQuery(QString
                       ("SELECT %1, %2 FROM %3 WHERE (%4 IS NOT NULL) AND (%5 >= '%6') AND (%7 <= '%8')").
                       arg(YerithDatabaseTableColumn::MONTANT_PAYE,
                           YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
                           YerithDatabase::PAIEMENTS,
                           YerithDatabaseTableColumn::COMPTE_CLIENT,
                           YerithDatabaseTableColumn::DATE_PAIEMENT,
                           DATE_TO_DB_FORMAT_STRING
                           (dateEdit_bilan_comptable_debut->date()),
                           YerithDatabaseTableColumn::DATE_PAIEMENT,
                           DATE_TO_DB_FORMAT_STRING
                           (dateEdit_bilan_comptable_fin->date())));


    //strVersementsQuery.append(QString(" AND %1 = '%2'")
    //                                                      .arg(YerithDatabaseTableColumn::LOCALISATION,
    //                                                               _current_succursale_STRING));

//    qDebug() << QString("++ strVersementsQuery: %1")
//                              .arg(strVersementsQuery);

    int versementsQuerySize =
                    YerithUtils::execQuery(query, strVersementsQuery, _logger);

    int type_de_paiement = YerithUtils::ENCAISSEMENT_INDEFINI;

    double montant_paye = 0.0;

    double montant_total_versements = 0.0;


    double montant_paye__PAR_FIDELITE_CLIENTS = 0.0;

    double montant_total_versements__PAR_FIDELITE_CLIENTS = 0.0;


    for (int k = 0; k < versementsQuerySize && query.next(); ++k)
    {
        type_de_paiement =
                        query.value(YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt();

        if (YerithUtils::is_montant_payer_AU_CLIENT__PAR_FIDELITE__valide
                (type_de_paiement))
        {
            montant_paye__PAR_FIDELITE_CLIENTS =
                            query.value(YerithDatabaseTableColumn::MONTANT_PAYE).toDouble();

            montant_total_versements__PAR_FIDELITE_CLIENTS =
                            montant_total_versements__PAR_FIDELITE_CLIENTS +
                            montant_paye__PAR_FIDELITE_CLIENTS;
        }

        if (YerithUtils::is_montant_payer_par_le_client_valide
                (type_de_paiement))
        {
            montant_paye =
                            query.value(YerithDatabaseTableColumn::MONTANT_PAYE).toDouble();

            montant_total_versements = montant_total_versements + montant_paye;
        }
    }

//    qDebug() << QString("++ versementsQuerySize: %1, montant_total_versements: %2")
//                              .arg(QString::number(versementsQuerySize),
//                                       QString::number(montant_total_versements, 'f', 2));


    emit SIGNAL_INCREMENT_PROGRESS_BAR(72);


    query.clear();

    QString
    strPaiementsAuComptesFournisseursQuery(QString
                                           ("SELECT %1, %2 FROM %3 WHERE (%4 IS NOT NULL) AND (%5 >= '%6') AND (%7 <= '%8')").
                                           arg(YerithDatabaseTableColumn::
                                               MONTANT_PAYE,
                                               YerithDatabaseTableColumn::
                                               TYPE_DE_PAIEMENT,
                                               YerithDatabase::PAIEMENTS,
                                               YerithDatabaseTableColumn::
                                               COMPTE_FOURNISSEUR,
                                               YerithDatabaseTableColumn::
                                               DATE_PAIEMENT,
                                               DATE_TO_DB_FORMAT_STRING
                                               (dateEdit_bilan_comptable_debut->
                                                date()),
                                               YerithDatabaseTableColumn::
                                               DATE_PAIEMENT,
                                               DATE_TO_DB_FORMAT_STRING
                                               (dateEdit_bilan_comptable_fin->
                                                date())));


    strPaiementsAuComptesFournisseursQuery.append(QString(" AND %1 = '%2'").
                                                  arg
                                                  (YerithDatabaseTableColumn::
                                                   LOCALISATION,
                                                   _current_succursale_STRING));

//    qDebug() << QString("++ strVersementsQuery: %1")
//                              .arg(strVersementsQuery);

    int paiementsAuComptesFournisseursQuerySize = YerithUtils::execQuery(query,
                                                                         strPaiementsAuComptesFournisseursQuery,
                                                                         _logger);

    type_de_paiement = YerithUtils::DECAISSEMENT_INDEFINI;


    double montant_paye_au_fournisseur = 0.0;

    double montant_total_paiements_aux_fournisseurs = 0.0;

    for (int k = 0;
            k < paiementsAuComptesFournisseursQuerySize && query.next(); ++k)
    {
        type_de_paiement =
                        query.value(YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).toInt();

        if (YerithUtils::is_montant_payer_au_fournisseur_valide
                (type_de_paiement))
        {
            montant_paye_au_fournisseur =
                            query.value(YerithDatabaseTableColumn::MONTANT_PAYE).toDouble();

            montant_total_paiements_aux_fournisseurs =
                            montant_total_paiements_aux_fournisseurs +
                            montant_paye_au_fournisseur;
        }
    }

//    qDebug() << QString("++ paiementsAuComptesFournisseursQuerySize: %1, montant_total_paiements_aux_fournisseurs: %2")
//                              .arg(QString::number(paiementsAuComptesFournisseursQuerySize),
//                                       QString::number(montant_total_paiements_aux_fournisseurs, 'f', 2));

    query.clear();


    double total_entrees = montant_total_vente + montant_total_versements;


    double total_sorties = montant_total_versements__PAR_FIDELITE_CLIENTS +
                           montant_TOTAL_TVA_COLLECTE +
                           montant_total_achat +
                           montant_total_paiements_aux_fournisseurs + montant_total_dette_clientelle;


    double achats_depenses_financieres_effectues =
                    montant_total_paiements_aux_fournisseurs;


    double balance = 0.0;
    double benefice_sur_vente_effectuees = 0.0;
    double chiffre_daffaire = 0.0;

    QMapIterator < int,
                 StockQteVendueEtRemiseTotalSurVente * >it(stocksidToqtevendue);

    int querySize = -1;
    QString strQuery;

    while (it.hasNext())
    {
        it.next();

        int stocks_id = it.key();
        StockQteVendueEtRemiseTotalSurVente *curValue = it.value();

        strQuery =
                        QString("SELECT %1 FROM %2 WHERE %3 = '%4' AND %5 = '%6'").arg
                        (YerithDatabaseTableColumn::MARGE_BENEFICIAIRE,
                         YerithDatabase::ACHATS, YerithDatabaseTableColumn::STOCKS_ID,
                         QString::number(stocks_id), YerithDatabaseTableColumn::LOCALISATION,
                         _current_succursale_STRING);


        int querySize = YerithUtils::execQuery(query, strQuery, _logger);

        if (querySize > 0 && query.next())
        {
            double marge_beneficiaire = query.value(0).toDouble();

            benefice_sur_vente_effectuees =
                            benefice_sur_vente_effectuees +
                            (curValue->_qteVendue * marge_beneficiaire);

            benefice_sur_vente_effectuees =
                            benefice_sur_vente_effectuees - curValue->_remiseTotalSurVentes;
        }

        delete curValue;
    }

    benefice_sur_vente_effectuees = benefice_sur_vente_effectuees -
                                    montant_total_dette_clientelle;

    benefice_sur_vente_effectuees = benefice_sur_vente_effectuees -
                                    qFabs(montant_total_versements__PAR_FIDELITE_CLIENTS);

    if (benefice_sur_vente_effectuees <= 0)
    {
        benefice_sur_vente_effectuees = 0;
    }

    chiffre_daffaire = montant_total_vente;

    balance = total_entrees - qFabs(total_sorties);

//    qDebug() << QString("++ benefice: %1, chiffre_daffaire: %2, balance: %3")
//                              .arg(QString::number(benefice, 'f', 2),
//                                       QString::number(chiffre_daffaire, 'f', 2),
//                                       QString::number(balance, 'f', 2));

    QString latexFileNamePrefix("yerith-erp-bilan-comptable");

#ifdef YERITH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yerith-erp-financial-accounting-report");
#endif

    QString texDocument;

    YerithUtils::getLatexFinancialAccountingReportData(texDocument);

    YerithInfoEntreprise &infoEntreprise =
                    YerithUtils::getAllWindows()->getInfoEntreprise();

    QString
    fileDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
             (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(fileDate);

    QString bilanComptableDateDebut;
    QString bilanComptableDateFin;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        bilanComptableDateDebut =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::englishLocale.toString
                             (dateEdit_bilan_comptable_debut->date())));

        bilanComptableDateFin =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::englishLocale.toString
                             (dateEdit_bilan_comptable_fin->date())));
    }
    else
    {
        bilanComptableDateDebut =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::frenchLocale.toString
                             (dateEdit_bilan_comptable_debut->date())));

        bilanComptableDateFin =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::frenchLocale.toString
                             (dateEdit_bilan_comptable_fin->date())));
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(80);


    texDocument.replace("YERITHBILANCOMPTABLEDEBUT", bilanComptableDateDebut);

    texDocument.replace("YERITHBILANCOMPTABLEFIN", bilanComptableDateFin);

    texDocument.replace("YERITHBILANCOMPTABLEVENTESDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_vente));

    texDocument.replace("YERITHBILANCOMPTABLEVERSEMENTSCLIENTSDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_versements));

    texDocument.replace("YERITHBILANCOMPTABLETOTALENTREESDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(total_entrees));

    texDocument.replace("YERITHBILANCOMPTABLEDETTECLIENTELLEDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_dette_clientelle));

    texDocument.replace("YERITHBILANCOMPTABLEACHATSDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_achat));

    texDocument.replace("YERITHBILANCOMPTABLETVAENGRANGE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_TOTAL_TVA_COLLECTE));

    texDocument.replace("YERITHBILANCOMPTABLETOTALSORTIESDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(total_sorties));


    QString balanceDeviseLatexStr(GET_CURRENCY_STRING_NUM_FOR_LATEX(balance));


    if (balance >= 0)
    {
        balanceDeviseLatexStr =
                        YerithUtils::colorLatexTextInput("yerithColorGreen",
                                                         balanceDeviseLatexStr);
    }
    else
    {
        balanceDeviseLatexStr =
                        YerithUtils::colorLatexTextInput("yerithColorRed",
                                                         balanceDeviseLatexStr);
    }

    texDocument.replace("YERITHBILANCOMPTABLEBALANCEDEVISE",
                        balanceDeviseLatexStr);

    texDocument.replace("YERITHBILANCOMPTABLEPROGRAMMEFIDELITECLIENTS",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(montant_total_versements__PAR_FIDELITE_CLIENTS));

    texDocument.replace("YERITHBILANCOMPTABLECHARGESDEPENSESFINANCIERES",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(achats_depenses_financieres_effectues));

    texDocument.replace("YERITHBILANCOMPTABLEBENEFICEDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(benefice_sur_vente_effectuees));

    texDocument.replace("YERITHBILANCOMPTABLECHIFFREDAFFAIREDEVISE",
                        GET_CURRENCY_STRING_NUM_FOR_LATEX(total_entrees));

    texDocument.replace("YERITHPAPERSPEC", "a4paper");
    texDocument.replace("YERITHENTREPRISE",
                        infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YERITHACTIVITESENTREPRISE",
                        infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YERITHBOITEPOSTALE",
                        infoEntreprise.getBoitePostal());
    texDocument.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YERITHDATE", fileDate);

    texDocument.replace("YERITHNOMUTILISATEUR",
                        QString("%1 %2").arg(YerithUtils::getAllWindows()->
                                             getUser()->titreTex(),
                                             YerithUtils::getAllWindows()->
                                             getUser()->nom_completTex()));

    texDocument.replace("YERITHSUCCURSALE",
                        YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (_current_succursale_STRING));

    texDocument.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YERITHCOMPTEBANCAIRENR",
                        infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YERITHCONTRIBUABLENR",
                        infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YERITHAGENCECOMPTEBANCAIRE",
                        infoEntreprise.getAgenceCompteBancaireTex());

    QString yerithPrefixFileName;

    yerithPrefixFileName.append
    (YerithUtils::getUniquePrefixFileInTemporaryFilesDir
     (latexFileNamePrefix));

    //qDebug() << "++\n" << texDocument;


    emit SIGNAL_INCREMENT_PROGRESS_BAR(98);


    QFile tmpLatexFile(QString("%1tex").arg(yerithPrefixFileName));

    YerithUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    QString
    pdfCustomerDataFileName(YerithERPProcess::compileLatex
                            (yerithPrefixFileName));

    if (pdfCustomerDataFileName.isEmpty())
    {
        return 0;
    }

    YerithERPProcess::startPdfViewerProcess(pdfCustomerDataFileName);
}


void YerithTableauxDeBordWindow::analyse_comparee_jour_semaine()
{
    if (checkBox_analyse_comparee->isChecked())
    {
        QString current_compared_analysis(comboBox_ANALYSE_COMPAREE->currentText());

        if (YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::ANALYSE_COMPAREE_ACHATS_VENTES,
                                                current_compared_analysis))
        {
            analyse_comparee_jour_semaine_ACHATS_VENTES();
        }
        else if (YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::ANALYSE_COMPAREE_VENTES_BENEFICES,
                                                     current_compared_analysis))
        {
            analyse_comparee_jour_semaine_VENTES_BENEFICES();
        }
    }
}


void YerithTableauxDeBordWindow::analyse_comparee_mensuelle()
{
    if (checkBox_analyse_comparee->isChecked())
    {
        QString current_compared_analysis(comboBox_ANALYSE_COMPAREE->currentText());

        if (YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::ANALYSE_COMPAREE_ACHATS_VENTES,
                                                current_compared_analysis))
        {
            analyse_comparee_mensuelle_ACHATS_VENTES();
        }
        else if (YerithUtils::isEqualCaseInsensitive(YerithTableauxDeBordWindow::ANALYSE_COMPAREE_VENTES_BENEFICES,
                                                     current_compared_analysis))
        {
            analyse_comparee_mensuelle_VENTES_BENEFICES();
        }
    }
}


void YerithTableauxDeBordWindow::compterLesArticlesVendusParBENEFICES()
{
    QString objet(comboBox_objets->currentText());

    int size = lineEdit_quantite->text().toInt();

    QStringList progArguments;
    QString tmpFilePrefix;

    QDir tempDir(YerithERPConfig::temporaryFilesDir);

    QString csvFile;
    QString pdfFileTitle;

    if (YerithTableauxDeBordWindow::QUALITE_BENEFICES_PLUS_ELEVES == comboBox_qualite->currentText() ||
        YerithTableauxDeBordWindow::QUALITE_BENEFICES_MOINS_ELEVES == comboBox_qualite->currentText())
    {
        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(QString("The %1 ").arg(lineEdit_quantite->text()));
        }
        else
        {
            pdfFileTitle.append(QString("Les %1 ").arg(lineEdit_quantite->text()));
        }
    }
    else
    {
        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append("The ");
        }
        else
        {
            pdfFileTitle.append("Les ");
        }
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(60);


    if (YerithTableauxDeBordWindow::QUALITE_BENEFICES_PLUS_ELEVES == comboBox_qualite->currentText())
    {
        if (YerithTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_ARTICLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-articles");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-articles");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxArticles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_MEILLEURS,
                                     "best");
            }
            else
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_MEILLEURS,
                                     "meilleures");
            }

            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CATEGORIES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-categories");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-categories");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxCategories(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CAISSIERS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-cashiers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-caissiers");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxCaissiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CLIENTS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-customers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-clients");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxClients(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-supplier-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-fournisseurs-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxFournisseursVentes(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_VILLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_VILLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-cities-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-villes-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxVilles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_QUARTIER == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_QUARTIER);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-client-location-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-quartiers-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxQuartiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-FINANCIAL-BENEFITS-regions-states-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-BENEFICES-regions-etats-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMaxRegionsEtats(csvFile, size);
        }

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(" giving the best sold by FINANCIAL BENEFITS");
        }
        else
        {
            pdfFileTitle.append(" avec les b\\'en\\'efices les plus \\'elev\\'ees");
        }
    }
    else if (YerithTableauxDeBordWindow::QUALITE_BENEFICES_MOINS_ELEVES == comboBox_qualite->currentText())
    {
        if (YerithTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_ARTICLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-articles");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-articles");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreArticles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                pdfFileTitle
                    .replace(YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE,
                             "least");
            }
            else
            {
                pdfFileTitle
                    .replace(YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE,
                             "les moins");
            }

            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CATEGORIES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-categories");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-categories");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreCategories(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CAISSIERS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-cashiers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-caissiers");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreCaissiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CLIENTS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-customers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-clients");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreClients(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-supplier-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-fournisseurs-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreFournisseursVentes(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_VILLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_VILLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-cities-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-villes-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreVilles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_QUARTIER == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_QUARTIER);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-client-location-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-quartiers-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreQuartiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-FINANCIAL-BENEFITS-regions-states-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-BENEFICES-regions-etats-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsBENEFICESMoindreRegionsEtats(csvFile, size);
        }

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(" giving the least sold by FINANCIAL BENEFITS");
        }
        else
        {
            pdfFileTitle.append(" avec les B\\'EN\\'EFICES les moins \\'elev\\'es");
        }
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


    if (_csvFileItemSize <= 0)
    {
        QString retMsg(QObject::tr
                       ("Il n'y a pas de données correspondante à la requête !\n"
                        "Vérifier que les dates de début et de fin sont correctes !"));

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("rankings - pas de données !"),
                                       retMsg);
        _csvFileItemSize = 0;

        return;
    }

    QString latexChartTemplate;
    QString latexChartFileNamePrefix;


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        if (YerithTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_ZERO_ventes_tex);

            latexChartFileNamePrefix.append(QString("%1/%2_ZERO")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_bar_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETBARCOLOR",
                                       GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-bar-chart")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART == comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_pie_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETPIESEGMENTCOLOR",
                                       GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-pie-chart")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
    }
    else
    {
        if (YerithTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_ZERO_ventes_tex);

            latexChartFileNamePrefix.append(QString("%1/%2_ZERO")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_bar_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETBARCOLOR",
                                       GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-bar-chart")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART == comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_pie_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETPIESEGMENTCOLOR",
                                       GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-pie-chart")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(80);


    QString pdfFile(latexChartFileNamePrefix + ".pdf");

    tempDir.remove(pdfFile);

    //qDebug() << "++ csvFile: " << csvFile;

    if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART ==
            comboBox_type_graphes->currentText())
    {
        QString YERITHCUTAWAY;

        for (int k = 1; k <= _csvFileItemSize; ++k)
        {
            YERITHCUTAWAY.append(QString::number(k)).append(",");
            //qDebug() << "++ YERITHCUTAWAY: " << YERITHCUTAWAY;
        }

        YERITHCUTAWAY.remove(YERITHCUTAWAY.length() - 1, 1);

        latexChartTemplate.replace("YERITHCUTAWAY", YERITHCUTAWAY);
    }

    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString
    statsDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
              (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(statsDate);

    //qDebug() << "++ type fact. rapports: " << YerithConfig::typeOfFacturation;

    latexChartTemplate.replace("YERITHPAPERSPEC", "a4paper");

    latexChartTemplate.replace("YERITHCSVFILE", tmpFilePrefix + ".csv");
    latexChartTemplate.replace("YERITHCHARTFIN", _reportTexFileEndString);

    latexChartTemplate.replace("YERITHUTILISATEUR",
                               QString("%1 %2").
                               arg(YerithUtils::getAllWindows()->getUser()->
                                   titreTex(),
                                   YerithUtils::getAllWindows()->getUser()->
                                   nom_completTex()));

    latexChartTemplate.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    latexChartTemplate.replace("YERITHBARCHARTTITLE",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (pdfFileTitle));
    latexChartTemplate.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    latexChartTemplate.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    latexChartTemplate.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    latexChartTemplate.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());
    latexChartTemplate.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    latexChartTemplate.replace("YERITHEMAIL",
                               infoEntreprise.getEmail_LATEX());
    latexChartTemplate.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());
    latexChartTemplate.replace("YERITHDATE", statsDate);
    latexChartTemplate.replace("YERITHVENTESDEBUT",
                               DATE_TO_STRING(dateEdit_rapports_debut->date
                                              ()));
    latexChartTemplate.replace("YERITHVENTESFIN",
                               DATE_TO_STRING(dateEdit_rapports_fin->date
                                              ()));

    latexChartTemplate.replace("YERITHSUCCURSALE",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (YerithERPConfig::THIS_SITE_LOCALISATION_NAME));

    latexChartTemplate.replace("YERITHHEUREGENERATION", CURRENT_TIME);
    latexChartTemplate.replace("YERITHCOMPTEBANCAIRENR",
                               infoEntreprise.getNumeroCompteBancaire());
    latexChartTemplate.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    latexChartTemplate.replace("YERITHAGENCECOMPTEBANCAIRE",
                               infoEntreprise.getAgenceCompteBancaireTex());

    QFile latexChartFile(latexChartFileNamePrefix + ".tex");

    if (latexChartFile.open(QFile::WriteOnly))
    {
        latexChartFile.write(latexChartTemplate.toUtf8());
    }

    latexChartFile.close();


    emit SIGNAL_INCREMENT_PROGRESS_BAR(84);


    progArguments << "-interaction";
    progArguments << "nonstopmode";
    progArguments << latexChartFile.fileName();

    //qDebug() << "++ -> " << latexChartFile.fileName();

    if (!progArguments.empty())
    {
        QProcess aProcess;

        aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

        aProcess.start(YerithERPConfig::pathToPdfLatex(), progArguments);

        aProcess.waitForFinished(-1);

        progArguments.clear();
        progArguments << pdfFile;

        aProcess.startDetached(YerithERPConfig::pathToPdfReader,
                               progArguments);
        aProcess.waitForFinished();
    }

    _csvFileItemSize = 0;
}


void YerithTableauxDeBordWindow::compterLesArticlesVendusParQuantite()
{
    QString objet(comboBox_objets->currentText());

    int size = lineEdit_quantite->text().toInt();

    QStringList progArguments;
    QString tmpFilePrefix;

    QDir tempDir(YerithERPConfig::temporaryFilesDir);

    QString csvFile;
    QString pdfFileTitle;

    if (YerithTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE ==
            comboBox_qualite->currentText()
            || YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE ==
            comboBox_qualite->currentText())
    {
        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(QString("The %1 ").
                                arg(lineEdit_quantite->text()));
        }
        else
        {
            pdfFileTitle.append(QString("Les %1 ").
                                arg(lineEdit_quantite->text()));
        }
    }
    else
    {
        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append("The ");
        }
        else
        {
            pdfFileTitle.append("Les ");
        }
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(60);


    if (YerithTableauxDeBordWindow::QUALITE_PLUS_VENDU_PAR_QUANTITE ==
            comboBox_qualite->currentText())
    {
        if (YerithTableauxDeBordWindow::OBJET_SERVICES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_SERVICES);
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-services");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-services");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxServices(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_ARTICLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-articles");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-articles");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxArticles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_MEILLEURS,
                                     "best");

                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-categories");
            }
            else
            {
                pdfFileTitle.replace(YerithTableauxDeBordWindow::QUALITE_MEILLEURS,
                                     "meilleures");

                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-categories");
            }

            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CATEGORIES);

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxCategories(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CAISSIERS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-caissiers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-cashiers");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxCaissiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CLIENTS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-customers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-clients");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxClients(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-supplier-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-fournisseurs-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxFournisseursVentes(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_VILLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_VILLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-villes-ventes");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-cities-sale");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxVilles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_QUARTIER == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_QUARTIER);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-client-location-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-quartier-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxQuartiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("best-sold-by-quantity-regions-states-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-plus-vendus-par-quantite-regions-etats-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMaxRegionsEtats(csvFile, size);
        }

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(" giving the best sold by quantity");
        }
        else
        {
            pdfFileTitle.append(" avec les quantit\\'es vendues les plus \\'elev\\'ees");
        }
    }
    else if (YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE == comboBox_qualite->currentText())
    {
        if (YerithTableauxDeBordWindow::OBJET_SERVICES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_SERVICES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-services");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-services");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreServices(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_ARTICLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_ARTICLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-articles");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-articles");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreArticles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CATEGORIES == objet)
        {
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                pdfFileTitle.replace
                    (YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE,
                     "least");
            }
            else
            {
                pdfFileTitle.replace
                    (YerithTableauxDeBordWindow::QUALITE_MOINS_VENDU_PAR_QUANTITE,
                     "les moins");
            }

            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CATEGORIES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-categories");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-categories");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreCategories(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CAISSIERS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CAISSIERS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-cashiers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-caissiers");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreCaissiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_CLIENTS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_CLIENTS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-customers");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-clients");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreClients(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_VILLES == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_VILLES);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-cities-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-villes-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreVilles(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_QUARTIER == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_QUARTIER);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-client-location-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-quartiers-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreQuartiers(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS);
            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-regions-states-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-regions-etats-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreRegionsEtats(csvFile, size);
        }
        else if (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS == objet)
        {
            pdfFileTitle.append(YerithTableauxDeBordWindow::OBJET_FOURNISSEURS);

            if (YerithMainWindow::LANGUE_ANGLAISE)
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("least-sold-by-quantity-supplier-sale");
            }
            else
            {
                tmpFilePrefix = FILE_NAME_USERID_CURRENT_TIME("les-moins-vendus-par-quantite-fournisseurs-ventes");
            }

            csvFile = tmpFilePrefix + ".csv";

            csvFile.prepend(YerithERPConfig::temporaryFilesDir + "/");

            tempDir.remove(csvFile);

            statsQuantiteMoindreFournisseursVentes(csvFile, size);
        }

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            pdfFileTitle.append(" giving the least sold by quantity");
        }
        else
        {
            pdfFileTitle.append(" avec les quantit\\'es vendues les moins \\'elev\\'ees");
        }
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


    if (_csvFileItemSize <= 0)
    {
        QString retMsg(QObject::tr("Il n'y a pas de données correspondante à la requête !\n"
                                   "Vérifier que les dates de début et de fin sont correctes !"));

        YerithQMessageBox::information(this,
                                       QObject::tr("rankings - pas de données !"),
                                       retMsg);
        _csvFileItemSize = 0;

        return;
    }

    QString latexChartTemplate;
    QString latexChartFileNamePrefix;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        if (YerithTableauxDeBordWindow::QUALITE_ZERO == comboBox_qualite->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_ZERO_ventes_tex);

            latexChartFileNamePrefix.append(QString("%1/%2_ZERO")
                                             .arg(YerithERPConfig::temporaryFilesDir,
                                                  tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_BAR_CHART == comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_bar_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETBARCOLOR",
                                       GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-bar-chart")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART ==
                 comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::EN_pie_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETPIESEGMENTCOLOR",
                                       GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-pie-chart")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
    }
    else
    {
        if (YerithTableauxDeBordWindow::QUALITE_ZERO ==
                comboBox_qualite->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_ZERO_ventes_tex);

            latexChartFileNamePrefix.append(QString("%1/%2_ZERO")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_BAR_CHART ==
                 comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_bar_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETBARCOLOR",
                                       GET_YERITH_DTL_SET_BAR_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-bar-chart")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
        else if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART ==
                 comboBox_type_graphes->currentText())
        {
            latexChartTemplate.append(YerithUtils::FR_pie_chart_tex);

            latexChartTemplate.replace("YERITHDTLSETPIESEGMENTCOLOR",
                                       GET_YERITH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(size));

            latexChartFileNamePrefix.append(QString("%1/%2-pie-chart")
                                            .arg(YerithERPConfig::temporaryFilesDir,
                                                 tmpFilePrefix));
        }
    }


    emit SIGNAL_INCREMENT_PROGRESS_BAR(84);


    QString pdfFile(latexChartFileNamePrefix + ".pdf");

    tempDir.remove(pdfFile);

    //qDebug() << "++ csvFile: " << csvFile;

    if (YerithTableauxDeBordWindow::GRAPHE_PIE_CHART == comboBox_type_graphes->currentText())
    {
        QString YERITHCUTAWAY;

        for (int k = 1; k <= _csvFileItemSize; ++k)
        {
            YERITHCUTAWAY.append(QString::number(k)).append(",");
            //qDebug() << "++ YERITHCUTAWAY: " << YERITHCUTAWAY;
        }

        YERITHCUTAWAY.remove(YERITHCUTAWAY.length() - 1, 1);

        latexChartTemplate.replace("YERITHCUTAWAY", YERITHCUTAWAY);
    }

    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString
    statsDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
              (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(statsDate);

    //qDebug() << "++ type fact. rapports: " << YerithConfig::typeOfFacturation;

    latexChartTemplate.replace("YERITHPAPERSPEC", "a4paper");

    latexChartTemplate.replace("YERITHCSVFILE", tmpFilePrefix + ".csv");
    latexChartTemplate.replace("YERITHCHARTFIN", _reportTexFileEndString);

    latexChartTemplate.replace("YERITHUTILISATEUR",
                               QString("%1 %2").
                               arg(YerithUtils::getAllWindows()->getUser()->
                                   titreTex(),
                                   YerithUtils::getAllWindows()->getUser()->
                                   nom_completTex()));

    latexChartTemplate.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    latexChartTemplate.replace("YERITHBARCHARTTITLE",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (pdfFileTitle));
    latexChartTemplate.replace("YERITHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    latexChartTemplate.replace("YERITHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    latexChartTemplate.replace("YERITHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    latexChartTemplate.replace("YERITHVILLE",
                               infoEntreprise.getVille_LATEX());
    latexChartTemplate.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    latexChartTemplate.replace("YERITHEMAIL",
                               infoEntreprise.getEmail_LATEX());
    latexChartTemplate.replace("YERITHTELEPHONE",
                               infoEntreprise.getTelephone());
    latexChartTemplate.replace("YERITHDATE", statsDate);
    latexChartTemplate.replace("YERITHVENTESDEBUT",
                               DATE_TO_STRING(dateEdit_rapports_debut->date
                                              ()));
    latexChartTemplate.replace("YERITHVENTESFIN",
                               DATE_TO_STRING(dateEdit_rapports_fin->date
                                              ()));

    latexChartTemplate.replace("YERITHSUCCURSALE",
                               YerithUtils::LATEX_IN_OUT_handleForeignAccents
                               (YerithERPConfig::THIS_SITE_LOCALISATION_NAME));

    latexChartTemplate.replace("YERITHHEUREGENERATION", CURRENT_TIME);
    latexChartTemplate.replace("YERITHCOMPTEBANCAIRENR",
                               infoEntreprise.getNumeroCompteBancaire());
    latexChartTemplate.replace("YERITHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    latexChartTemplate.replace("YERITHAGENCECOMPTEBANCAIRE",
                               infoEntreprise.getAgenceCompteBancaireTex());

    QFile latexChartFile(latexChartFileNamePrefix + ".tex");

    if (latexChartFile.open(QFile::WriteOnly))
    {
        latexChartFile.write(latexChartTemplate.toUtf8());
    }

    latexChartFile.close();


    progArguments << "-interaction";
    progArguments << "nonstopmode";
    progArguments << latexChartFile.fileName();

    //qDebug() << "++ -> " << latexChartFile.fileName();

    if (!progArguments.empty())
    {
        QProcess aProcess;

        aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

        aProcess.start(YerithERPConfig::pathToPdfLatex(), progArguments);

        aProcess.waitForFinished(-1);

        progArguments.clear();
        progArguments << pdfFile;

        aProcess.startDetached(YerithERPConfig::pathToPdfReader,
                               progArguments);
        aProcess.waitForFinished();
    }

    _csvFileItemSize = 0;
}


void YerithTableauxDeBordWindow::handle_enabled_chiffre_daffaire_jour_semaine(bool
                                                                              enabled)
{
    label_jour_semaine_debut_chiffre_affaire->setVisible(enabled);
    label_jour_semaine_fin_chiffre_affaire->setVisible(enabled);

    dateEdit_chiffre_daffaire_jour_semaine_debut->setVisible(enabled);
    dateEdit_chiffre_daffaire_jour_semaine_fin->setVisible(enabled);
}


void YerithTableauxDeBordWindow::handle_enabled_chiffre_daffaire_mois(bool
                                                                      enabled)
{
    label_annee_chiffre_affaire->setVisible(enabled);
    label_mois_debut_chiffre_affaire->setVisible(enabled);
    label_mois_fin_chiffre_affaire->setVisible(enabled);

    comboBox_annee_chiffre_affaire->setVisible(enabled);
    comboBox_mois_debut_chiffre_affaire->setVisible(enabled);
    comboBox_mois_fin_chiffre_affaire->setVisible(enabled);
}


void YerithTableauxDeBordWindow::calculer_chiffre_daffaire_jour_semaine()
{
//      QDEBUG_STRING_OUTPUT_1("calculer_chiffre_daffaire_jour_semaine");

    _logger->log("calculer_chiffre_daffaire_jour_semaine");

    if (dateEdit_chiffre_daffaire_jour_semaine_fin->date() <
            dateEdit_chiffre_daffaire_jour_semaine_debut->date())
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("évolution du chiffre d'affaire"),
                                   QObject::tr
                                   ("Le jour de 'début' doit être "
                                    "antérieur au jour de la 'fin' !"));
        return;
    }

    QMap < int, double >dayOfWeek__TO__businessturnover;

    qint64 dates_range =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->date().
                    daysTo(dateEdit_chiffre_daffaire_jour_semaine_fin->date()) + 1;


    QString string_chiffre_daffaire_semaine_query;

    int current_day_of_week;

    QSqlQuery qsql_query;

    QDateTime current_day_date =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

    double a_temp_day_of_week_business_turnover = 0.0;

    double current_day_business_turnover = 0.0;

    int current_query_size;

    qint64 i = 0;

    bool filtreActif = false;

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    QString yerithFiltre;

    QString databaseTableColumn;

    QString
    comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText
                                       ());

    QString
    textFromLineEditEvolutionSujets
    (comboBox_evolution_objets_value->currentText());

    do
    {
        current_day_of_week = current_day_date.date().dayOfWeek();

        string_chiffre_daffaire_semaine_query =
                        QString("SELECT (%1 - %2) FROM %3 WHERE %4='%5'").arg
                        (YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                         YerithDatabaseTableColumn::MONTANT_TVA, YerithDatabase::STOCKS_VENDU,
                         YerithDatabaseTableColumn::DATE_VENTE,
                         DATE_TO_DB_FORMAT_STRING(current_day_date));


        bool filtreActif = false;

        AUCUN_FILTRE = QObject::tr("aucun filtre !");

        yerithFiltre.clear();

        databaseTableColumn.clear();

        comboBoxEvolutionObjetsCurrentText =
                        comboBox_evolution_objets->currentText();

        textFromLineEditEvolutionSujets =
                        comboBox_evolution_objets_value->currentText();


        if (YerithUtils::isEqualCaseInsensitive
                (comboBoxEvolutionObjetsCurrentText,
                 YerithTableauxDeBordWindow::OBJET_CAISSIERS))
        {
            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::OBJET_CAISSIERS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerithDatabaseTableColumn::NOM_CAISSIER;
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerithTableauxDeBordWindow::OBJET_CATEGORIES))
        {
            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::OBJET_CATEGORIES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerithDatabaseTableColumn::CATEGORIE;
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerithTableauxDeBordWindow::OBJET_CLIENTS))
        {
            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::OBJET_CLIENTS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

        }
        else if (YerithUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerithTableauxDeBordWindow::OBJET_ARTICLES))
        {
            if (textFromLineEditEvolutionSujets.isEmpty())
            {
                filtreActif = false;

                yerithFiltre.append(AUCUN_FILTRE);
            }
            else
            {
                filtreActif = true;

                yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                    (YerithTableauxDeBordWindow::
                                     OBJET_ARTICLES,
                                     textFromLineEditEvolutionSujets));

                databaseTableColumn = YerithDatabaseTableColumn::DESIGNATION;
            }
        }
        else if (YerithUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerithTableauxDeBordWindow::OBJET_FOURNISSEURS))
        {

            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::
                                 OBJET_FOURNISSEURS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

        }
        else if (YerithUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerithTableauxDeBordWindow::OBJET_QUARTIER))
        {

            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::OBJET_QUARTIER,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT;

        }
        else if (YerithUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerithTableauxDeBordWindow::OBJET_VILLES))
        {

            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::OBJET_VILLES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerithDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT;

        }
        else if (YerithUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS))
        {

            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::
                                 OBJET_REGIONS_ETATS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerithDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT;

        }

//              qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
//                                              .arg(databaseTableColumn, comboBox_evolution_objets->currentText());

        if (true == filtreActif)
        {
            if (textFromLineEditEvolutionSujets.isEmpty())
            {
                YerithQMessageBox::warning(this,
                                           QObject::tr
                                           ("paramètre manquant"),
                                           QObject::tr
                                           ("Vous devez spécifier un paramètre dans"
                                            " le champs de texte 'nom' !"));
                return;
            }
            else
            {
                if (!databaseTableColumn.isEmpty())
                {
                    string_chiffre_daffaire_semaine_query.append(QString
                                                                 (" AND %1 = '%2'").
                                                                 arg
                                                                 (databaseTableColumn,
                                                                  textFromLineEditEvolutionSujets));
                }
            }
        }

//              qDebug() << "++ string_chiffre_daffaire_semaine_query: " << string_chiffre_daffaire_semaine_query;

        qsql_query.clear();

        current_query_size =
                        YerithUtils::execQuery(qsql_query,
                                               string_chiffre_daffaire_semaine_query);

        if (current_query_size > 0)
        {
            while (qsql_query.next())
            {
                current_day_business_turnover =
                                qsql_query.value(0).toDouble();

                QString STRING_current_day_of_week =
                                YerithUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                                (current_day_of_week);

                if (!YerithUtils::isEqualCaseInsensitive
                        (YerithUtils::EMPTY_STRING, STRING_current_day_of_week))
                {
                    a_temp_day_of_week_business_turnover =
                                    dayOfWeek__TO__businessturnover[current_day_of_week];

                    dayOfWeek__TO__businessturnover.insert(current_day_of_week,
                                                           current_day_business_turnover
                                                           +
                                                           a_temp_day_of_week_business_turnover);
                }
            }
        }

        current_day_date = current_day_date.addDays(1);

        ++i;
    }
    while (i < dates_range);

//      qDebug() << dayOfWeek__TO__businessturnover;

    if (dayOfWeek__TO__businessturnover.isEmpty())
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("pas de données"),
                                       QObject::tr
                                       ("Il n'y a pas de données correspondante à la requête !\n"
                                        "Vérifier que les dates de début et de fin, "
                                        "sont correctes !"));
        return;
    }

    _reportTexFileEndString.clear();


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _reportTexFileEndString
            .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (QString("Details in %1:")
                            .arg(YerithERPConfig::currency)));
    }
    else
    {
        _reportTexFileEndString
            .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (QString("D\\'etails en %1:")
                            .arg(YerithERPConfig::currency)));
    }


    _reportTexFileEndString.prepend("\\textbf{").append("}\n");
    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    //Fill in the PDF file which amount of money for each day of week.

    double somme_totale_ventes_jour_semaine = 0.0;

    QMapIterator < int, double >it(dayOfWeek__TO__businessturnover);

    while (it.hasNext())
    {
        it.next();

        somme_totale_ventes_jour_semaine += it.value();


        _reportTexFileEndString.append("\\item ");


        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            _reportTexFileEndString
                .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerithUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())));
        }
        else
        {
            _reportTexFileEndString
                .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerithUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())));
        }

        _reportTexFileEndString
            .append(QString(": $%1$\n")
                     .arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (GET_CURRENCY_STRING_NUM(it.value()))));
    }

//          QDEBUG_STRING_OUTPUT_2_N("somme_totale_jour_semaine", somme_totale_jour_semaine);

    _reportTexFileEndString.append("\\end{enumerate}\n");


    QString barItems;

    double ratio = 0;

    const int TICKS = 100;
    const double MAX_RATIO = 900.0;

    it.toFront();

    while (it.hasNext())
    {
        it.next();

        ratio = (it.value() * MAX_RATIO) / somme_totale_ventes_jour_semaine;

//              qDebug() << QString("++ jour semaine: %1, max amount: %2, ratio: %3")
//                                              .arg(YerithUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key()),
//                                                       QString::number(somme_totale_jour_semaine, 'f', 2),
//                                                       QString::number(ratio, 'f', 2));

        barItems.append(QString("\\baritem{%1}{%2}{gray}\n").arg
                        (YerithUtils::LATEX_IN_OUT_handleForeignAccents
                         (YerithUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                          (it.key())), QString::number(ratio, 'f', 2)));
    }


//          qDebug() << "++ barItems: " << barItems;

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerithUtils::_1a_tex);


    texDocument.replace("YERITHBARPERGROUP", "");
    texDocument.replace("YERITHBARITEMS", barItems);
    texDocument.replace("YERITHTICKS", QString::number(TICKS));


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        texDocument.replace("YERITHLEGENDANALYSECOMPAREE", "");

        texDocument.replace("YERITHDIAGRAMMETITRE",
                            "Ratio of days of week income.");

        texDocument.replace("YERITHNIVEAUCHIFFREAFFAIRE", "Income Level");
    }
    else
    {
        texDocument.replace("YERITHLEGENDANALYSECOMPAREE", "");

        texDocument.replace("YERITHDIAGRAMMETITRE",
                            "Ratio du chiffre d'affaire jour--semaine.");

        texDocument.replace("YERITHNIVEAUCHIFFREAFFAIRE",
                            "Niveau du chiffre d'affaire");
    }


    QString fileName1(YerithERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }

    tmpFile1.close();


    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();


    QString texDocument2;


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.append(YerithUtils::EN_bar_diag_tex);
    }
    else
    {
        texDocument2.append(YerithUtils::FR_bar_diag_tex);
    }


    QString factureDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;

    QString longDateFin;


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        longDateDebut =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::englishLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::englishLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }
    else
    {
        longDateDebut =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::frenchLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::frenchLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }

    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerithConfig::typeOfFacturation;


    texDocument2.replace("YERITHPAPERSPEC", "a4paper");

    if (YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerithTableauxDeBordWindow::OBJET_CAISSIERS)    ||
        YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerithTableauxDeBordWindow::OBJET_CATEGORIES)   ||
        YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerithTableauxDeBordWindow::OBJET_CLIENTS))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            yerithFiltre.clear();
            yerithFiltre.append(QObject::tr("aucun filtre !"));
        }
    }
    else
    {
        yerithFiltre =
                        YerithUtils::LATEX_IN_OUT_handleForeignAccents(yerithFiltre);
    }

    texDocument2.replace("YERITHMENTION", "");

    texDocument2.replace("YERITHFILTRE",
                         yerithFiltre.prepend("''").append("''"));

    texDocument2.replace("YERITHCHIFFREAFFAIREDATEDEBUT", longDateDebut);
    texDocument2.replace("YERITHCHIFFREAFFAIREDATEFIN", longDateFin);
    texDocument2.replace("YERITHCHARTFIN", _reportTexFileEndString);
    texDocument2.replace("YERITHENTREPRISE",
                         infoEntreprise.getNomCommercial_LATEX());
    texDocument2.replace("YERITHACTIVITESENTREPRISE",
                         infoEntreprise.getSecteursActivitesTex());
    texDocument2.replace("YERITHBOITEPOSTALE",
                         infoEntreprise.getBoitePostal());
    texDocument2.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
    texDocument2.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    texDocument2.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());
    texDocument2.replace("YERITHDATE", factureDate);

    texDocument2.replace("YERITHNOMUTILISATEUR",
                         QString("%1 %2").arg(YerithUtils::getAllWindows()->
                                              getUser()->titreTex(),
                                              YerithUtils::getAllWindows()->
                                              getUser()->
                                              nom_completTex()));

    texDocument2.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument2.replace("YERITHCOMPTEBANCAIRENR",
                         infoEntreprise.getNumeroCompteBancaire());
    texDocument2.replace("YERITHCONTRIBUABLENR",
                         infoEntreprise.getNumeroDeContribuable());
    texDocument2.replace("YERITHAGENCECOMPTEBANCAIRE",
                         infoEntreprise.getAgenceCompteBancaireTex());


    texDocument2.replace("1a.tex", fileName1);


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.replace("YERITHTITREDOCUMENT",
                             QString("Chart illustrating days of week income from %1 to %2.")
                                .arg(longDateDebut, longDateFin));
    }
    else
    {
        texDocument2.replace("YERITHTITREDOCUMENT",
                             QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                     " d'affaire par jour de la semaine du %1 au %2.")
                                .arg(longDateDebut, longDateFin));
    }


    //qDebug() << "++ test: " << texDocument2;

    YerithUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire-jour-semaine"));

    fileName.append(".");

    QString tmpFilePrefix(YerithERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }

    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

//          qDebug() << "++ file name to latex compile bar diag: " << texRm;

    aProcess.start(YerithERPConfig::pathToLatex(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("%1.dvi").arg(texRm);

    aProcess.start(YerithERPConfig::pathToDvips(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("-dALLOWPSTRANSPARENCY");
    progArguments << QString("%1.ps").arg(texRm);
    progArguments << QString("%1.pdf").arg(texRm);

    aProcess.start(YerithERPConfig::pathToPs2Pdf(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

//          qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerithERPConfig::pathToPdfReader, progArguments);

    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


void YerithTableauxDeBordWindow::calculer_chiffre_daffaire_mois()
{
    _logger->log("calculer_chiffre_daffaire_mois");

    if (comboBox_mois_debut_chiffre_affaire->currentIndex() >
            comboBox_mois_fin_chiffre_affaire->currentIndex())
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("évolution du chiffre d'affaire"),
                                   QObject::tr
                                   ("Le mois de 'début' doit être"
                                    " antérieur au mois de 'fin' !"));
    }

    //"yyyy-MM-dd"; D.toString(YerithUtils::DB_DATE_FORMAT)

    QString annee = comboBox_annee_chiffre_affaire->currentText();
    QString mois_debut = comboBox_mois_debut_chiffre_affaire->currentText();
    QString mois_fin = comboBox_mois_fin_chiffre_affaire->currentText();

    int moisDebut = _moisToNombre[mois_debut];
    int moisFin = _moisToNombre[mois_fin];

    QString dateDebut;
    dateDebut.append(annee).append("-").append(QString::number(moisDebut)).
    append("-01");

    QDate qDateDebut(annee.toInt(), moisDebut, 1);
    QDate qDateFin;

    QString dateFin;
    bool moisFinAjuster = false;

    if (_moisToNombre[YerithTableauxDeBordWindow::MOIS_12] == moisFin)
    {
        dateFin.append(annee).append("-").append(QString::number(moisFin)).
        append("-31");

        qDateFin.setDate(annee.toInt(), moisFin, 31);
    }
    else
    {
        moisFin += 1;
        moisFinAjuster = true;

        dateFin.append(annee).append("-").append(QString::number(moisFin)).
        append("-01");

        qDateFin.setDate(annee.toInt(), moisFin, 1);
    }

//    qDebug() << "calculerChiffresDaffaireMois\n\t" <<
//                              QString("annee: %1, mois_debut: %2, mois fin ajuster: %3, mois_fin: %4")
//                                      .arg(annee,
//                                               QString::number(moisDebut),
//                                               BOOL_TO_STRING(moisFinAjuster),
//                                               QString::number(moisFin));

    QString
    strQuery(QString
             ("SELECT %1, (%2 - %3) FROM %4 WHERE %5 >= '%6' AND ").arg
             (YerithDatabaseTableColumn::DATE_VENTE,
              YerithDatabaseTableColumn::MONTANT_TOTAL_VENTE,
              YerithDatabaseTableColumn::MONTANT_TVA,
              YerithDatabase::STOCKS_VENDU,
              YerithDatabaseTableColumn::DATE_VENTE, dateDebut));

    if (moisFinAjuster)
    {
        strQuery.append(QString("%1 < '%2'").arg
                        (YerithDatabaseTableColumn::DATE_VENTE, dateFin));
    }
    else
    {
        strQuery.append(QString("%1 <= '%2'").arg
                        (YerithDatabaseTableColumn::DATE_VENTE, dateFin));
    }

    bool filtreActif = false;

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    QString yerithFiltre;

    QString databaseTableColumn;

    QString
    comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText
                                       ());

    QString
    textFromLineEditEvolutionSujets
    (comboBox_evolution_objets_value->currentText());

    if (YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerithTableauxDeBordWindow::OBJET_CAISSIERS))
    {
        filtreActif = true;

        yerithFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerithTableauxDeBordWindow::OBJET_CAISSIERS,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerithDatabaseTableColumn::NOM_CAISSIER;
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_CATEGORIES))
    {
        filtreActif = true;

        yerithFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerithTableauxDeBordWindow::OBJET_CATEGORIES,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerithDatabaseTableColumn::CATEGORIE;
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_CLIENTS))
    {
        filtreActif = true;

        yerithFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerithTableauxDeBordWindow::OBJET_CLIENTS,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerithDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_ARTICLES))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            filtreActif = false;

            yerithFiltre.append(AUCUN_FILTRE);
        }
        else
        {
            filtreActif = true;

            yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerithTableauxDeBordWindow::OBJET_ARTICLES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerithDatabaseTableColumn::DESIGNATION;
        }
    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_FOURNISSEURS))
    {

        filtreActif = true;

        yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerithTableauxDeBordWindow::OBJET_FOURNISSEURS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerithDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_QUARTIER))
    {

        filtreActif = true;

        yerithFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerithTableauxDeBordWindow::OBJET_QUARTIER,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerithDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT;

    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_VILLES))
    {

        filtreActif = true;

        yerithFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerithTableauxDeBordWindow::OBJET_VILLES,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerithDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT;

    }
    else if (YerithUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS))
    {

        filtreActif = true;

        yerithFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerithTableauxDeBordWindow::OBJET_REGIONS_ETATS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerithDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT;

    }

    //qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
    //                          .arg(databaseTableColumn, comboBox_evolution_objets->currentText());



    if (true == filtreActif)
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            YerithQMessageBox::warning(this,
                                       QObject::tr("paramètre manquant"),
                                       QObject::tr
                                       ("Vous devez spécifier un paramètre dans"
                                        " le champs de texte 'nom' !"));
            return;
        }
        else
        {
            if (!databaseTableColumn.isEmpty())
            {
                strQuery.append(QString(" AND %1 = '%2'").arg
                                (databaseTableColumn,
                                 YerithUtils::LATEX_IN_OUT_handleForeignAccents
                                 (textFromLineEditEvolutionSujets)));
            }
        }
    }


//    qDebug() << "++ query: " << strQuery;

    QSqlQuery query;

    double montant_total_vente = 0.0;

    int querySize = YerithUtils::execQuery(query, strQuery, _logger);

    if (0 >= querySize)
    {

        YerithQMessageBox::information(this,
                                       QObject::tr("pas de données"),
                                       QObject::tr
                                       ("Il n'y a pas de données correspondante à la requête !\n"
                                        "Vérifier que les dates de début et de fin, "
                                        "ainsi que l'année sont correctes !"));
        return;
    }

    QMap < int, double >monthToVentesTotalAmount;

    int curMonth = 0;
    QDate curDate;

    double maxAmount = 0.0;

    for (int k = 0; k < querySize && query.next(); ++k)
    {
        curDate = query.value(0).toDate();
        curMonth = curDate.month();

        montant_total_vente = query.value(1).toDouble();

        //qDebug() << "++ curDate: " << curDate
        //               << ", curMonth: " << curMonth
        //               << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
            monthToVentesTotalAmount[curMonth] += montant_total_vente;

            if (maxAmount < monthToVentesTotalAmount[curMonth])
            {
                maxAmount = monthToVentesTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
               << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth]; */
        }
    }

    _reportTexFileEndString.clear();


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _reportTexFileEndString
            .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                (QString("Details in %1:").arg(YerithERPConfig::currency)));
    }
    else
    {
        _reportTexFileEndString
            .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                (QString("D\\'etails en %1:").arg(YerithERPConfig::currency)));
    }


    _reportTexFileEndString.prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");


    //Fill in the PDF file which amount of money for which month.

    if (moisFinAjuster)
    {
        moisFin -= 1;
    }

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        _reportTexFileEndString.append("\\item ");

        if (YerithMainWindow::LANGUE_ANGLAISE)
        {
            _reportTexFileEndString
                .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerithUtils::englishLocale.monthName(k)));
        }
        else
        {
            _reportTexFileEndString
                .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerithUtils::frenchLocale.monthName(k)));
        }


        _reportTexFileEndString
            .append(": $")
            .append(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                        (GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])))
            .append("$\n");
    }

    _reportTexFileEndString.append("\\end{enumerate}\n");

    //qDebug() << "++ END";


    QString barItems;

    double ratio = 0;

    double sommeTotalMois = 0.0;

    const int TICKS = 100;

    const double MAX_RATIO = 900.0;


    for (int k = moisDebut; k <= moisFin; ++k)
    {
        sommeTotalMois += monthToVentesTotalAmount[k];
    }

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        if (monthToVentesTotalAmount.contains(k))
        {
            ratio = (monthToVentesTotalAmount[k] * MAX_RATIO) / sommeTotalMois;

            //qDebug() << QString("++ mois(k): %1, max amount: %2, ratio: %3")
            //                              .arg(QString::number(k),
            //                                              QString::number(monthToVentesTotalAmount[k], 'f', 2),
            //                                              QString::number(ratio, 'f', 2));

            barItems.append(QString("\\baritem{%1}{%2}{gray}\n").arg
                            (YerithUtils::LATEX_IN_OUT_handleForeignAccents
                             (GET_MONTH_NAME_LOCALIZED(k)),
                             QString::number(ratio, 'f', 2)));
        }
    }

    //qDebug() << "++ barItems: " << barItems;

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerithERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerithUtils::_1a_tex);


    texDocument.replace("YERITHBARPERGROUP", "");
    texDocument.replace("YERITHBARITEMS", barItems);
    texDocument.replace("YERITHTICKS", QString::number(TICKS));

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        texDocument.replace("YERITHLEGENDANALYSECOMPAREE", "");

        texDocument.replace("YERITHDIAGRAMMETITRE",
                            "Ratio of the monthly income.");

        texDocument.replace("YERITHNIVEAUCHIFFREAFFAIRE",
                            "Income Level");
    }
    else
    {
        texDocument.replace("YERITHLEGENDANALYSECOMPAREE", "");

        texDocument.replace("YERITHDIAGRAMMETITRE",
                            "Ratio du chiffre d'affaire du mois.");

        texDocument.replace("YERITHNIVEAUCHIFFREAFFAIRE",
                            "Niveau du chiffre d'affaire");
    }


    QString fileName1(YerithERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }

    tmpFile1.close();


    YerithInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.append(YerithUtils::EN_bar_diag_tex);
    }
    else
    {
        texDocument2.append(YerithUtils::FR_bar_diag_tex);
    }


    QString
    factureDate(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                (infoEntreprise.getVille_LATEX()));

    YerithUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;
    QString longDateFin;


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        longDateDebut =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::englishLocale.toString(qDateDebut)));

        longDateFin =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::englishLocale.toString(qDateFin)));
    }
    else
    {
        longDateDebut =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::frenchLocale.toString(qDateDebut)));

        longDateFin =
        QString("'%1'").arg(YerithUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerithUtils::frenchLocale.toString(qDateFin)));
    }


    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerithConfig::typeOfFacturation;


    texDocument2.replace("YERITHPAPERSPEC", "a4paper");

    if (YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerithTableauxDeBordWindow::OBJET_CAISSIERS)
            ||
            YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                                YerithTableauxDeBordWindow::OBJET_CATEGORIES)
            ||
            YerithUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                                YerithTableauxDeBordWindow::OBJET_CLIENTS))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            yerithFiltre.clear();
            yerithFiltre.append(QObject::tr("aucun filtre !"));
        }
    }
    else
    {
        yerithFiltre =
                        YerithUtils::LATEX_IN_OUT_handleForeignAccents(yerithFiltre);
    }

    texDocument2.replace("YERITHMENTION", "");

    texDocument2.replace("YERITHFILTRE",
                         yerithFiltre.prepend("''").append("''"));

    texDocument2.replace("YERITHCHIFFREAFFAIREDATEDEBUT", longDateDebut);
    texDocument2.replace("YERITHCHIFFREAFFAIREDATEFIN", longDateFin);
    texDocument2.replace("YERITHCHARTFIN", _reportTexFileEndString);
    texDocument2.replace("YERITHENTREPRISE",
                         infoEntreprise.getNomCommercial_LATEX());
    texDocument2.replace("YERITHACTIVITESENTREPRISE",
                         infoEntreprise.getSecteursActivitesTex());
    texDocument2.replace("YERITHBOITEPOSTALE",
                         infoEntreprise.getBoitePostal());
    texDocument2.replace("YERITHVILLE", infoEntreprise.getVille_LATEX());
    texDocument2.replace("YERITHPAYS", infoEntreprise.getPaysTex());
    texDocument2.replace("YERITHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YERITHTELEPHONE", infoEntreprise.getTelephone());
    texDocument2.replace("YERITHDATE", factureDate);

    texDocument2.replace("YERITHNOMUTILISATEUR",
                         QString("%1 %2").arg(YerithUtils::getAllWindows()->
                                              getUser()->titreTex(),
                                              YerithUtils::getAllWindows()->
                                              getUser()->
                                              nom_completTex()));

    texDocument2.replace("YERITHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument2.replace("YERITHCOMPTEBANCAIRENR",
                         infoEntreprise.getNumeroCompteBancaire());
    texDocument2.replace("YERITHCONTRIBUABLENR",
                         infoEntreprise.getNumeroDeContribuable());
    texDocument2.replace("YERITHAGENCECOMPTEBANCAIRE",
                         infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("1a.tex", fileName1);


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.replace("YERITHTITREDOCUMENT",
                             QString("Chart illustrating the income from %1 to %2.")
                                .arg(longDateDebut, longDateFin));
    }
    else
    {
        texDocument2.replace("YERITHTITREDOCUMENT",
                             QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                     " d'affaire du %1 au %2.")
                                .arg(longDateDebut, longDateFin));
    }


    //qDebug() << "++ test: " << texDocument2;

    YerithUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString
    fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire"));

    fileName.append(".");


    QString tmpFilePrefix(YerithERPConfig::temporaryFilesDir + "/" + fileName);


    QFile tmpFile(tmpFilePrefix + "tex");


    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }

    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

    //qDebug() << "++ file name to latex compile bar diag: " << texRm;

    aProcess.start(YerithERPConfig::pathToLatex(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("%1.dvi").arg(texRm);

    aProcess.start(YerithERPConfig::pathToDvips(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("-dALLOWPSTRANSPARENCY");
    progArguments << QString("%1.ps").arg(texRm);
    progArguments << QString("%1.pdf").arg(texRm);

    aProcess.start(YerithERPConfig::pathToPs2Pdf(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

    //qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerithERPConfig::pathToPdfReader, progArguments);

    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


void *YerithTableauxDeBordWindow::choisirEvolutionDuChiffreDaffaire()
{
    emit SIGNAL_INCREMENT_PROGRESS_BAR(10);

    if (checkBox_analyse_comparee->isChecked())
    {
        if (radioButton_jour_semaine->isChecked())
        {
            analyse_comparee_jour_semaine();

            emit SIGNAL_INCREMENT_PROGRESS_BAR(70);
        }
        else if (radioButton_mensuel->isChecked())
        {
            analyse_comparee_mensuelle();

            emit SIGNAL_INCREMENT_PROGRESS_BAR(70);
        }
    }
    else
    {
        if (radioButton_jour_semaine->isChecked())
        {
            calculer_chiffre_daffaire_jour_semaine();

            emit SIGNAL_INCREMENT_PROGRESS_BAR(70);
        }
        else if (radioButton_mensuel->isChecked())
        {
            calculer_chiffre_daffaire_mois();

            emit SIGNAL_INCREMENT_PROGRESS_BAR(70);
        }
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(90);
}


void YerithTableauxDeBordWindow::setupShortcuts()
{
    setupShortcutActionAfficherPDF(*actionGenererPDF);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
