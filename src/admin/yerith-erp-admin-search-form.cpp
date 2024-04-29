/**
 * yerith-erp-admin-search-form.cpp
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-admin-search-form.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-logger.hpp"

#include <QtCore/QDebug>

#include <QtCore/QRegExp>

#include <QtCore/QStringList>

#include <QtSql/QSqlError>

#include <QtWidgets/QLabel>

#include <QtWidgets/QFormLayout>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QCompleter>



YerithAdminSearchForm::YerithAdminSearchForm(YerithERPWindows  	*allWindows,
											 QWidget 			*parent)
:QDialog(parent),
 _curSujetAction(SUJET_ACTION_ALERTE),
 _curSqlTableModel(0),
 _logger(0),
 _allWindows(allWindows)
{
    setupUi(this);

    _logger = new YerithLogger("YerithAdminSearchForm");

    setFixedSize(width(),
    			 height());


    connect(checkBox_EXACT_STRING_MATCH,
            SIGNAL(stateChanged(int)),
			this,
            SLOT(setupLineEditsQCompleters()));


    pushButton_annuler->enable(this, SLOT(reinitialiser()));
}


void YerithAdminSearchForm::rendreVisible(int tabWidjetListerIdx)
{
    _logger->log("rendreVisible");

    setupLineEditsQCompleters(tabWidjetListerIdx);

    lineEdit_terme_recherche->setFocus();

    show();
}


void YerithAdminSearchForm::rendreInvisible()
{
    _logger->log("rendreInvisible");

    clear_all_fields();

    close();
}


QString YerithAdminSearchForm::generate_qstring_FILTER(QString corresponding_DB_column_KeyValue,
                                                       QString aTableColumnFieldContentForSearch)
{
    QString qstring_filter;

    if (!checkBox_EXACT_STRING_MATCH->isChecked())
    {
        qstring_filter =
            YerithUtils::generateSqlLike(corresponding_DB_column_KeyValue,
                                         aTableColumnFieldContentForSearch);
    }
    else
    {
        qstring_filter =
            YerithUtils::generateSqlIs(corresponding_DB_column_KeyValue,
                                       aTableColumnFieldContentForSearch);
    }

//    QDEBUG_STRING_OUTPUT_2("qstring_filter",
//                            qstring_filter);

    return qstring_filter;
}


void YerithAdminSearchForm::setupLineEditsQCompleters()
{
    setupLineEditsQCompleters(_allWindows
                                ->_adminListerWindow
                                    ->current_tabWidget_index());
}


void YerithAdminSearchForm::setupLineEditsQCompleters(int tabWidjetListerIdx)
{
    Qt::MatchFlags a_filter_mode = Qt::MatchContains;

    if (!checkBox_EXACT_STRING_MATCH->isChecked())
    {
        a_filter_mode = Qt::MatchContains;
    }
    else
    {
        a_filter_mode = Qt::MatchStartsWith;
    }

    switch (tabWidjetListerIdx)
    {

    case SUJET_ACTION_ALERTE:

        lineEdit_terme_recherche->enableForSearch(QObject::tr("nom de l'alerte"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerithDatabase::ALERTES,
                                	  YerithDatabaseTableColumn::DESIGNATION_ALERTE,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);


        _curSujetAction = SUJET_ACTION_ALERTE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_alertes();
        break;


    case SUJET_ACTION_CHARGE_FINANCIERE:

        lineEdit_terme_recherche
 			->enableForSearch(QObject::tr("désignation (BON DE COMMANDE)"));


         lineEdit_terme_recherche
         	->setupMyStaticQCompleter(YerithDatabase::CHARGES_FINANCIERES,
                                 	  YerithDatabaseTableColumn::DESIGNATION,
 									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

         _curSujetAction = SUJET_ACTION_CHARGE_FINANCIERE;

         _curSqlTableModel = &_allWindows->getSqlTableModel_charges_financieres();
        break;


    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom du département de produits"));

        lineEdit_terme_recherche
        	->setupMyStaticQCompleter(YerithDatabase::DEPARTEMENTS_PRODUITS,
                                  	  YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS;

        _curSqlTableModel = &_allWindows->getSqlTableModel_departements_produits();
        break;


    case SUJET_ACTION_CATEGORIE:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom de la catégorie"));

        lineEdit_terme_recherche
        	->setupMyStaticQCompleter(YerithDatabase::CATEGORIES,
                                	  YerithDatabaseTableColumn::NOM_CATEGORIE,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_CATEGORIE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_categories();
        break;


    case SUJET_ACTION_ligne_budgetaire:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("INTITULÉ DE LA LIGNE BUDGÉTAIRE"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerithDatabase::LIGNES_BUDGETAIRES,
                                	  YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_ligne_budgetaire;

        _curSqlTableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
        break;


    case SUJET_ACTION_COMPTE_BANCAIRE:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("référence du compte bancaire"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerithDatabase::COMPTES_BANCAIRES,
                                	  YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_COMPTE_BANCAIRE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();
        break;


    case SUJET_ACTION_COMPTE_UTILISATEUR:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom de l'utilisateur"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerithDatabase::USERS,
                                	  YerithDatabaseTableColumn::NOM_COMPLET,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_COMPTE_UTILISATEUR;

        _curSqlTableModel = &_allWindows->getSqlTableModel_users();
        break;


    case SUJET_ACTION_LOCALISATION:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom de la localisation"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerithDatabase::LOCALISATIONS,
                                	  YerithDatabaseTableColumn::NOM_LOCALISATION,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_LOCALISATION;

        _curSqlTableModel = &_allWindows->getSqlTableModel_localisations();
        break;


    case SUJET_ACTION_REMISE:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("désignation de la remise"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerithDatabase::REMISES,
                                	  YerithDatabaseTableColumn::DESIGNATION_REMISE,
									  false,
									  YerithUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_REMISE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_remises();
        break;

    default:
        break;
    }


    connect(lineEdit_terme_recherche->getMyQCompleter(),
            SIGNAL(activated(const QString &)),
			this,
            SLOT(rechercher(const QString &)));
}


void YerithAdminSearchForm::reinitialiser()
{
    _logger->log("reinitialiser");

    lineEdit_terme_recherche->clear();

    if (_curSqlTableModel)
    {
        _curSqlTableModel->resetFilter();
    }

    setCurSqlTableModel(0);

    rendreInvisible();
}


void YerithAdminSearchForm::rechercher(const QString &itemName)
{
    //qDebug() << "++ YerithAdminSearchForm::rechercher | itemName: " << itemName;

    QString searchString(itemName);

    QString filter;

    switch (_curSujetAction)
    {
    case SUJET_ACTION_ALERTE:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::DESIGNATION_ALERTE,
                                         searchString);
        break;


    case SUJET_ACTION_CHARGE_FINANCIERE:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::DESIGNATION,
                                         searchString);
        break;


    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                                         searchString);
        break;


    case SUJET_ACTION_CATEGORIE:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::NOM_CATEGORIE,
                                         searchString);
        break;


    case SUJET_ACTION_ligne_budgetaire:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
                                         searchString);
        break;


    case SUJET_ACTION_COMPTE_BANCAIRE:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
                                         searchString);
        break;


    case SUJET_ACTION_COMPTE_UTILISATEUR:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::NOM_COMPLET,
                                         searchString);
        break;


    case SUJET_ACTION_LOCALISATION:
        filter = generate_qstring_FILTER(YerithDatabaseTableColumn::NOM_LOCALISATION,
                                         searchString);
        break;


    default:
        break;
    }

    //qDebug() << "++ YerithAdminSearchForm::rechercher, searchString: " << searchString;
    if (0 != _curSqlTableModel)
    {
        _curSqlTableModel->yerithSetFilter_WITH_where_clause(filter);

//        qDebug() << "++ filter: "
//                 << filter << ", _curSqlTableModel: "
//                 << _curSqlTableModel->sqlTableName();

        int curSqlTableModelRows =
        		_curSqlTableModel->easySelect("src/admin/yerith-erp-admin-search-form.cpp",
        									  390);

        if (curSqlTableModelRows > 0)
        {
            switch (_curSujetAction)
            {
            case SUJET_ACTION_ALERTE:
                _allWindows->_adminListerWindow->setAlertCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_alerte(_curSqlTableModel);
                break;


            case SUJET_ACTION_CHARGE_FINANCIERE:
                _allWindows->_adminListerWindow->setCharge_financieres_CurrentlyFiltered(true);
                _allWindows->_adminListerWindow->LISTER_CHARGES_FINANCIERES(_curSqlTableModel);
                break;


            case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
                _allWindows->_adminListerWindow->setProductDepartmentCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_departements_de_produits(_curSqlTableModel);
                break;


            case SUJET_ACTION_CATEGORIE:
                _allWindows->_adminListerWindow->setCategoryCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_categorie(_curSqlTableModel);
                break;


            case SUJET_ACTION_ligne_budgetaire:
                _allWindows->_adminListerWindow->setLIGNE_BUDGETAIRE_CurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_LIGNE_BUDGETAIRE(_curSqlTableModel);
                break;


            case SUJET_ACTION_COMPTE_BANCAIRE:
                _allWindows->_adminListerWindow->setBankAccountCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_compte_bancaire(_curSqlTableModel);
                break;


            case SUJET_ACTION_COMPTE_UTILISATEUR:
                _allWindows->_adminListerWindow-> setUserCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_utilisateur(_curSqlTableModel);
                break;


            case SUJET_ACTION_LOCALISATION:
                _allWindows->_adminListerWindow->setSiteCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_localisation(_curSqlTableModel);
                break;


            default:
                break;
            }
        }
    }


    rendreInvisible();
}

