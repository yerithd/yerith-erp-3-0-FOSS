/*
 * yerith-erp-paiements-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-paiements-window.hpp"


#include "src/process/yerith-erp-process.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/windows/stocks/yerith-erp-stock-detail-window.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <QtSql/QSqlField>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

#include <QtGui/QContextMenuEvent>

#include <QtCore/QThread>

#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcess>

#include <QtCore/QProcessEnvironment>


#include <unistd.h>


const QString YerithPaiementsWindow::CLIENT_TEXT_STRING("client");

const QString YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_ENGLISH("supplier");

const QString YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_FRENCH("fournisseur");


YerithPaiementsWindow::YerithPaiementsWindow()
:YerithWindowsCommons("yerith-erp-journal-paiements"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::PAIEMENTS),
 _logger(new YerithLogger("YerithPaiementsWindow")),
 _currentTabView(0),
 _pushButton_paiements_filtrer_font(0),
 _curSupplierText(YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_FRENCH),
 _paiementsDateFilter(YerithUtils::EMPTY_STRING),
 _curPaiementsTableModel(&_allWindows->getSqlTableModel_paiements())
{
    setYerithSqlTableModel(_curPaiementsTableModel);

    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("paiements clients et fournisseurs"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    enableResizing();

    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_paiements);


    _ACTIONS_TO_DISABLE_WHEN_NOT_PRINT_ACTION
            << action_parametrer_les_impressions
            << actionExporter_au_format_csv
			<< actionAfficherPDF;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
		<< actionAfficherPaiementAuDetail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_paiements);


    QMESSAGE_BOX_STYLE_SHEET =
        QString("QMessageBox {background-color: rgb(%1);}"
                "QMessageBox QLabel {color: rgb(%2);}")
            .arg(COLOUR_RGB_STRING_YERITH_WHITE_255_255_255,
                 COLOUR_RGB_STRING_YERITH_INDIGO_83_0_125);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::NOTES;


    setup_select_configure_dbcolumn(YerithDatabase::PAIEMENTS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_paiements_terme_recherche,
                                           YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString
        .insert(YerithDatabaseTableColumn::REFERENCE);

    _comboBoxesToANDContentForSearch
        .insert(&comboBox_paiements_intitule_du_compte_bancaire,
                YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);

    _comboBoxesToANDContentForSearch
        .insert(&comboBox_paiements_type_de_paiement,
                YerithDatabaseTableColumn::TYPE_DE_PAIEMENT);


    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerithDatabase::PAIEMENTS);


    reinitialiser_colones_db_visibles();

    textEdit_description->setYerithEnabled(false);


    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();


    tableView_paiements->setSqlTableName(&YerithDatabase::PAIEMENTS);


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    _pushButton_paiements_filtrer_font =
                    new QFont(pushButton_paiements_filtrer->font());


    pushButton_add_condition_SEARCH->disable(this);
    pushButton_paiements_filtrer->disable(this);
    pushButton_paiements_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_retour_paiements->disable(this);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));


    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionAfficherPaiementAuDetail, SIGNAL(triggered()), this,
            SLOT(afficher_paiements_detail()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_paiements, SIGNAL(currentChanged(int)), this,
            SLOT(handleCurrentChanged(int)));

    connect(tableView_paiements, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(afficher_paiements_detail()));

    connect(tableView_paiements, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(afficher_paiements_detail()));


    connect(action_BASCULER_client_fournisseur,
            SIGNAL(triggered()),
            this, SLOT(handle_ACTION_CHOICE_TYPE_DENTREPRISE()));


    connect(comboBox_paiements_type_dentreprise,
            SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(handle_combobox_type_dentreprise(const QString &)));


    connect(comboBox_paiements_type_de_paiement,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handleComboBoxClients_Typedepaiement_TextChanged
                 (const QString &)));

    setupShortcuts();
}


YerithPaiementsWindow::~YerithPaiementsWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithPaiementsWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_paiements_element_de_paiements->setVisible(IS_VISIBLE);
    comboBox_paiements_element_de_paiements_condition->setVisible(IS_VISIBLE);
    lineEdit_paiements_element_de_paiements_resultat->setVisible(IS_VISIBLE);
    pushButton_paiements_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_paiements_filtrer->setVisible(IS_VISIBLE);
}


void YerithPaiementsWindow::handleComboBoxClients_Typedepaiement_TextChanged(const
                                                                             QString
                                                                             &
                                                                             currentText)
{
    if (YerithUtils::isEqualCaseInsensitive(currentText,
                                            YerithUtils::
                                            _typedepaiementToUserViewString.
                                            value(YerithUtils::
                                                  ENCAISSEMENT_BANCAIRE))
            || YerithUtils::isEqualCaseInsensitive(currentText,
                                                   YerithUtils::
                                                   _typedepaiementToUserViewString.
                                                   value(YerithUtils::
                                                         ENCAISSEMENT_TELEPHONE))
            || YerithUtils::isEqualCaseInsensitive(currentText,
                                                   YerithUtils::
                                                   _typedepaiementToUserViewString.
                                                   value(YerithUtils::
                                                         ENCAISSEMENT_VIREMENT_BANCAIRE)))
    {
        comboBox_paiements_intitule_du_compte_bancaire->setYerithEnabled(true);
    }
    else
    {
        comboBox_paiements_intitule_du_compte_bancaire->setYerithEnabled(false);
    }
}


void YerithPaiementsWindow::handle_ACTION_CHOICE_TYPE_DENTREPRISE()
{
    QString cur_type_dentreprise(comboBox_paiements_type_dentreprise->currentText());

    QString type_dentreprise;

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        if (YerithUtils::isEqualCaseInsensitive(cur_type_dentreprise,
                                                YerithPaiementsWindow::CLIENT_TEXT_STRING))
        {
            type_dentreprise =
                YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_ENGLISH;
        }
        else if (YerithUtils::isEqualCaseInsensitive(cur_type_dentreprise,
                 YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_ENGLISH))
        {
            type_dentreprise = YerithPaiementsWindow::CLIENT_TEXT_STRING;
        }
    }
    else
    {
        if (YerithUtils::isEqualCaseInsensitive(cur_type_dentreprise,
                                                YerithPaiementsWindow::CLIENT_TEXT_STRING))
        {
            type_dentreprise =
                YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_FRENCH;
        }
        else if (YerithUtils::isEqualCaseInsensitive(cur_type_dentreprise,
                                                     YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_FRENCH))
        {
            type_dentreprise = YerithPaiementsWindow::CLIENT_TEXT_STRING;
        }
    }


    comboBox_paiements_type_dentreprise->find_AND_SET_CURRENT_INDEX(type_dentreprise);
}


void YerithPaiementsWindow::handle_combobox_type_dentreprise(const QString &text)
{
    if (YerithUtils::isEqualCaseInsensitive(_curSupplierText,
                                            comboBox_paiements_type_dentreprise->currentText()))
    {
        action_BASCULER_client_fournisseur->setText(QObject::tr("AFFICHER CLIENTS"));

        update_suppliers_specific_payments_type();
    }
    else
    {
        action_BASCULER_client_fournisseur->setText(QObject::tr("AFFICHER FOURNISSEURS"));

        update_clients_specific_payments_type();
    }

    get_current_table_column_for_company_type_to_HIDE();

    textChangedSearchLineEditsQCompleters();
}


bool YerithPaiementsWindow::filtrer_paiements()
{
    QString paiementsTableColumnValue =
                lineEdit_paiements_element_de_paiements_resultat->text();

    if (paiementsTableColumnValue.isEmpty())
    {
        QString msg =
            QObject::tr("Veuillez saisir une valeur numérique pour la recherche !");

        YerithQMessageBox::information(this,
                                       QObject::tr("filtrer"),
                                       msg);

        return false;
    }


    QString
    paiementsTableColumnProperty
    (comboBox_paiements_element_de_paiements->currentText());

    QString
    mathOperator
    (comboBox_paiements_element_de_paiements_condition->currentText());

    QString
    REAL_DB_ID_NAME_paiementsTableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.key
     (paiementsTableColumnProperty));

    QString
    filterString(QString("%1 AND (%2 %3 %4)").arg
                 (_paiementsDateFilter,
                  REAL_DB_ID_NAME_paiementsTableColumnProperty, mathOperator,
                  paiementsTableColumnValue));

    //qDebug() << "++ " << _searchFilter;

    if (!_searchFilter.isEmpty())
    {
        _searchFilter.append(QString(" AND %1").arg(filterString));
    }
    else
    {
        _searchFilter.append(filterString);
    }

    //qDebug() << QString("++ %1").arg(_searchFilter);

    setCurrentlyFiltered(true);

    _curPaiementsTableModel->yerithSetFilter_WITH_where_clause(_searchFilter);

    int resultRows =
    		_curPaiementsTableModel->easySelect("src/windows/yerith-erp-paiements-window.cpp", 426);

    lister_les_elements_du_tableau(*_curPaiementsTableModel);

    if (resultRows > 0)
    {
        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "paiements - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "paiements - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerithPaiementsWindow::initialiser_FILTRES()
{
    comboBox_paiements_element_de_paiements->setVisible(false);
    comboBox_paiements_element_de_paiements_condition->setVisible(false);
    lineEdit_paiements_element_de_paiements_resultat->setVisible(false);
    pushButton_paiements_reinitialiser_filtre->setVisible(false);
    pushButton_paiements_filtrer->setVisible(false);
}


void YerithPaiementsWindow::prepare__IN__for_export_and_printing()
{
    if (YerithUtils::isEqualCaseInsensitive
            (YerithPaiementsWindow::CLIENT_TEXT_STRING,
             comboBox_paiements_type_dentreprise->currentText()))
    {
        _latex_template_print_pdf_content.
        replace(QObject::tr("Journal des paiements"),
                QObject::tr("Journal des paiements (clients)"));


        _client_fournisseur_visible_string_EXPORT_AND_PRINT_PDF__TO__visible_index.
        insert(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR,
               _visibleDBColumnNameStrList.
               indexOf(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR));


        _visibleDBColumnNameStrList.
        removeAll(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);
    }
    else
    {
        _latex_template_print_pdf_content.
        replace(QObject::tr("Journal des paiements"),
                QObject::tr("Journal des paiements (fournisseurs)"));


        _client_fournisseur_visible_string_EXPORT_AND_PRINT_PDF__TO__visible_index.
        insert(YerithDatabaseTableColumn::COMPTE_CLIENT,
               _visibleDBColumnNameStrList.
               indexOf(YerithDatabaseTableColumn::COMPTE_CLIENT));

        _client_fournisseur_visible_string_EXPORT_AND_PRINT_PDF__TO__visible_index.
        insert(YerithDatabaseTableColumn::
               COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
               _visibleDBColumnNameStrList.
               indexOf(YerithDatabaseTableColumn::
                       COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS));



        _visibleDBColumnNameStrList.
        removeAll(YerithDatabaseTableColumn::COMPTE_CLIENT);

        _visibleDBColumnNameStrList.removeAll
        (YerithDatabaseTableColumn::
         COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS);
    }
}


void YerithPaiementsWindow::prepare__OUT__for_export_and_printing()
{
    QMapIterator < QString,
                 uint >
                 it
                 (_client_fournisseur_visible_string_EXPORT_AND_PRINT_PDF__TO__visible_index);

    while (it.hasNext())
    {
        it.next();

        _visibleDBColumnNameStrList.insert(it.value(), it.key());
    }
}


void YerithPaiementsWindow::update_clients_specific_payments_type()
{
    label_details_de_paiement_compte_client->setText(QObject::tr("compte client"));


    QStringList clients_payment_types;

    clients_payment_types
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_COMPTANT)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_CHEQUE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_TELEPHONE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_BANCAIRE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_VIREMENT_BANCAIRE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT);


    comboBox_paiements_type_de_paiement->clear();

    comboBox_paiements_type_de_paiement->addItem(YerithUtils::EMPTY_STRING);

    comboBox_paiements_type_de_paiement->addItems(clients_payment_types);


    comboBox_paiements_type_de_paiement->yerith_remove_item(YerithUtils::
                                                            _typedepaiementToUserViewString.
                                                            value
                                                            (YerithUtils::
                                                             ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE));

    comboBox_paiements_type_de_paiement->yerith_remove_item(YerithUtils::
                                                            _typedepaiementToUserViewString.
                                                            value
                                                            (YerithUtils::
                                                             DECAISSEMENT_COMPTANT));

    comboBox_paiements_type_de_paiement->yerith_remove_item(YerithUtils::
                                                            _typedepaiementToUserViewString.
                                                            value
                                                            (YerithUtils::
                                                             DECAISSEMENT_CHEQUE));

    comboBox_paiements_type_de_paiement->yerith_remove_item(YerithUtils::
                                                            _typedepaiementToUserViewString.
                                                            value
                                                            (YerithUtils::
                                                             DECAISSEMENT_TELEPHONE));

    comboBox_paiements_type_de_paiement->yerith_remove_item(YerithUtils::
                                                            _typedepaiementToUserViewString.
                                                            value
                                                            (YerithUtils::
                                                             DECAISSEMENT_BANCAIRE));

    comboBox_paiements_type_de_paiement->yerith_remove_item(YerithUtils::
                                                            _typedepaiementToUserViewString.
                                                            value
                                                            (YerithUtils::
                                                             DECAISSEMENT_VIREMENT_BANCAIRE));
}


void YerithPaiementsWindow::update_suppliers_specific_payments_type()
{
    label_details_de_paiement_compte_client->setText(QObject::tr("compte fournisseur"));


    QStringList suppliers_payment_types;

    suppliers_payment_types
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_COMPTANT)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_CHEQUE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_TELEPHONE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_BANCAIRE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_VIREMENT_BANCAIRE)
            << YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE);


    comboBox_paiements_type_de_paiement->clear();

    comboBox_paiements_type_de_paiement->addItem(YerithUtils::EMPTY_STRING);

    comboBox_paiements_type_de_paiement->addItems(suppliers_payment_types);


    comboBox_paiements_type_de_paiement
        ->yerith_remove_item(
            YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_COMPTANT));

    comboBox_paiements_type_de_paiement
        ->yerith_remove_item(
            YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_CHEQUE));

    comboBox_paiements_type_de_paiement
        ->yerith_remove_item(
            YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_TELEPHONE));

    comboBox_paiements_type_de_paiement
        ->yerith_remove_item(
            YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_BANCAIRE));

    comboBox_paiements_type_de_paiement
        ->yerith_remove_item(
            YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::ENCAISSEMENT_VIREMENT_BANCAIRE));


    comboBox_paiements_type_de_paiement
        ->yerith_remove_item(
            YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT));

    comboBox_paiements_type_de_paiement
        ->yerith_remove_item(
            YerithUtils::_typedepaiementToUserViewString.value(YerithUtils::DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT));
}


const QString &YerithPaiementsWindow::get_current_table_column_for_company_type_to_HIDE()
{
    if (YerithUtils::isEqualCaseInsensitive(_curSupplierText,
                                            comboBox_paiements_type_dentreprise->currentText()))
    {
        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.append
        (YerithDatabaseTableColumn::
         COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS);

        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.append
        (YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH);

        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.append
        (YerithDatabaseTableColumn::COMPTE_CLIENT);


        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.removeAll
        (YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);

        return YerithDatabaseTableColumn::COMPTE_CLIENT;

    }
    else
    {
        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.append
        (YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);


        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.removeAll
        (YerithDatabaseTableColumn::
         COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS);

        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.removeAll
        (YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH);

        _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.removeAll
        (YerithDatabaseTableColumn::COMPTE_CLIENT);

        return YerithDatabaseTableColumn::COMPTE_FOURNISSEUR;
    }
}


void YerithPaiementsWindow::populateComboBoxes()
{

    YerithUtils::RELOAD_VIEW_STRINGS();


    int columnIndexTypeDePaiement =
        _dbtablecolumnNameToDBColumnIndex.value(YerithDatabaseTableColumn::TYPE_DE_PAIEMENT);


    _DBFieldNamesToPrintLeftAligned.insert(columnIndexTypeDePaiement);


    comboBox_paiements_type_de_paiement->clear();

    comboBox_paiements_type_de_paiement->addItem(YerithUtils::EMPTY_STRING);


    QStringList aQStringList;

    aQStringList.append(YerithPaiementsWindow::CLIENT_TEXT_STRING);


    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _curSupplierText = YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_ENGLISH;

        aQStringList.append(YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_ENGLISH);
    }
    else
    {
        _curSupplierText = YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_FRENCH;

        aQStringList.append(YerithPaiementsWindow::FOURNISSEUR_TEXT_STRING_FRENCH);
    }


    comboBox_paiements_type_dentreprise->clear();

    comboBox_paiements_type_dentreprise->addItems(aQStringList);



    comboBox_paiements_intitule_du_compte_bancaire->clear();

    comboBox_paiements_intitule_du_compte_bancaire->setYerithEnabled(false);

    comboBox_paiements_intitule_du_compte_bancaire
        ->populateComboBoxRawString(YerithDatabase::COMPTES_BANCAIRES,
                                    YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);


    aQStringList.clear();

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::NOTES);
    aQStringList.removeAll(YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::_tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::MONTANT_PAYE));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::COMPTE_CLIENT));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::COMPTE_FOURNISSEUR));


    comboBox_paiements_element_de_paiements->clear();

    comboBox_paiements_element_de_paiements->addItems(aQStringList);



    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");


    comboBox_paiements_element_de_paiements_condition->clear();

    comboBox_paiements_element_de_paiements_condition->addItems(aQStringList);
}


void YerithPaiementsWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_paiements_nombre_de_lignes_par_page;

    lineEdit_paiements_terme_recherche->enableForSearch(QObject::tr
                                                        ("terme à rechercher (notes)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_paiements_montant_decaisse_debite->setYerithEnabled(false);

    lineEdit_paiements_montant_encaisse_credite->setYerithEnabled(false);

    lineEdit_paiements_nombre_de_clients_fournisseurs->setYerithEnabled(false);

    lineEdit_paiements_nombre_paiements->setYerithEnabled(false);


    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_paiements_nombre_de_lignes_par_page, tableView_paiements);

    lineEdit_details_de_paiement_reference_recu_paiement_client->
    setYerithEnabled(false);
    lineEdit_details_de_paiement_nom_de_lentreprise->setYerithEnabled(false);
    lineEdit_details_de_paiement_reference->setYerithEnabled(false);
    lineEdit_details_de_paiement_typedepaiement->setYerithEnabled(false);
    lineEdit_details_de_paiement_nom_de_lencaisseur->setYerithEnabled(false);
    lineEdit_details_de_paiement_compte_client->setYerithEnabled(false);
    lineEdit_details_de_paiement_montant_paye->setYerithEnabled(false);
    lineEdit_details_de_paiement_heure_de_paiement->setYerithEnabled(false);

    lineEdit_paiements_terme_recherche->setFocus();
}


void YerithPaiementsWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithPaiementsWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_paiements_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_paiements_filtrer_font->setUnderline(false);
    }

    pushButton_paiements_filtrer->setFont(*_pushButton_paiements_filtrer_font);
}


void YerithPaiementsWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    lister_les_elements_du_tableau();
}


void YerithPaiementsWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_paiements_element_de_paiements_resultat->clear();


    setCurrentlyFiltered(false);


    clearSearchFilter();


    QString searchTerm(lineEdit_paiements_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter
                .append(QString("(%1)")
                        .arg(GENERATE_SQL_LIKE_STMT(YerithDatabaseTableColumn::NOTES,
                                                    partSearchTerm)));


            if (k != lastIdx)
            {
                _searchFilter.append(" AND ");
            }
        }
    }


    {
        bool GENERATE_Like_SQL_STMT = false;

        if (!checkBox_EXACT_STRING_MATCH->isChecked())
        {
            GENERATE_Like_SQL_STMT = true;
        }

        YerithWindowsCommons::setYerithLineEditQCompleterSearchFilter(_searchFilter,
                                                                      GENERATE_Like_SQL_STMT);
    }


    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    {
        YerithLineEdit *aYerithLineEdit = 0;

        QMapIterator<YerithLineEdit **, QString > it(_lineEditsToANDContentForSearch);

        while (it.hasNext())
        {
            it.next();

            aYerithLineEdit = *it.key();

            correspondingDBFieldKeyValue = it.value();

            if (0 != aYerithLineEdit)
            {
                aTableColumnFieldContentForANDSearch = aYerithLineEdit->text();

                if (!correspondingDBFieldKeyValue.isEmpty()      &&
                    !aTableColumnFieldContentForANDSearch.isEmpty())
                {
                    if (!_searchFilter.isEmpty())
                    {
                        _searchFilter.append(" AND ");
                    }

                    _searchFilter
                        .append(GENERATE_SQL_IS_STMT(correspondingDBFieldKeyValue,
                                                     aTableColumnFieldContentForANDSearch));
                }
            }
        }
    }

    {
        QString curSearchDBStr;

        YerithComboBox *aYerithComboBox = 0;

        QMapIterator<YerithComboBox **, QString> it(_comboBoxesToANDContentForSearch);

        while (it.hasNext())
        {
            it.next();

            aYerithComboBox = *it.key();

            correspondingDBFieldKeyValue = it.value();

            if (0 != aYerithComboBox)
            {
                aTableColumnFieldContentForANDSearch =
                    aYerithComboBox->currentText();

                if (!correspondingDBFieldKeyValue.isEmpty()      &&
                    !aTableColumnFieldContentForANDSearch.isEmpty())
                {
                    if (!_searchFilter.isEmpty())
                    {
                        _searchFilter.append(" AND ");
                    }

                    if (aYerithComboBox == comboBox_paiements_intitule_du_compte_bancaire)
                    {
                        curSearchDBStr = aTableColumnFieldContentForANDSearch;
                    }
                    else if (aYerithComboBox == comboBox_paiements_type_de_paiement)
                    {
                        int typedepaiement =
                                YerithUtils::getComboBoxDatabaseQueryValue
                                                (aTableColumnFieldContentForANDSearch,
                                                 YerithUtils::_typedepaiementToUserViewString);

                        curSearchDBStr = QString::number(typedepaiement);
                    }

                    _searchFilter.append(GENERATE_SQL_IS_STMT
                                         (correspondingDBFieldKeyValue,
                                          curSearchDBStr));
                }
            }
        }
    }

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER;


    QString finalSearchFilter(_paiementsDateFilter);


    QString companyType_TOHIDE_FILTER =
                QString("(%1 IS NULL)")
                    .arg(get_current_table_column_for_company_type_to_HIDE());


    if (!_searchFilter.isEmpty())
    {
        _searchFilter.append(QString(" AND %1")
                              .arg(companyType_TOHIDE_FILTER));
    }
    else
    {
        _searchFilter = companyType_TOHIDE_FILTER;
    }


    if (!_searchFilter.isEmpty())
    {
        QString searchFilterWithDate =
                    QString("(%1) AND (%2)")
                        .arg(_paiementsDateFilter,
                             _searchFilter);

        finalSearchFilter = searchFilterWithDate;
    }

    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(finalSearchFilter);


    if (_yerithSqlTableModel->select())
    {
        lister_les_elements_du_tableau();
    }
    else
    {
        qDebug() << QString("++ YerithPaiementsWindow::textChangedSearchLineEditsQCompleters(): %1")
                     .arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithPaiementsWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    if (YerithUtils::isEqualCaseInsensitive(_curSupplierText,
                                            comboBox_paiements_type_dentreprise->
                                            currentText()))
    {
        _visibleDBColumnNameStrList
			<< YerithDatabaseTableColumn::DATE_PAIEMENT
			<< YerithDatabaseTableColumn::REFERENCE
			<< YerithDatabaseTableColumn::NOM_ENTREPRISE
			<< YerithDatabaseTableColumn::DESIGNATION
			<< YerithDatabaseTableColumn::MONTANT_PAYE
			<< YerithDatabaseTableColumn::COMPTE_FOURNISSEUR
			<< YerithDatabaseTableColumn::TYPE_DE_PAIEMENT
			<< YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS
			<< YerithDatabaseTableColumn::COMPTE_CLIENT;
    }
    else
    {
    	_visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DATE_PAIEMENT
            << YerithDatabaseTableColumn::NOM_ENTREPRISE
            << YerithDatabaseTableColumn::MONTANT_PAYE
            << YerithDatabaseTableColumn::TYPE_DE_PAIEMENT
            << YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS
            << YerithDatabaseTableColumn::COMPTE_FOURNISSEUR
            << YerithDatabaseTableColumn::COMPTE_CLIENT
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::REFERENCE;
    }
}


void YerithPaiementsWindow::resizeEvent(QResizeEvent *event)
{
    lineEdit_paiements_terme_recherche->setFixedWidth(event->size().width() -
                                                      4);

    tabWidget_paiements->setFixedWidth(event->size().width() - 4);

    tableView_paiements->setFixedWidth(tabWidget_paiements->width() - 4);
}


void YerithPaiementsWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tableView_paiements->rowCount() > 0)
    {
        QMenu menu(this);
        menu.addAction(actionAfficherPaiementAuDetail);
        menu.setPalette(YerithUtils::YERITH_WHITE_PALETTE);
        menu.exec(event->globalPos());
    }
}


void YerithPaiementsWindow::clear_all_fields()
{
    comboBox_paiements_type_de_paiement->clear();

    comboBox_paiements_type_de_paiement->addItem(YerithUtils::EMPTY_STRING);

    textEdit_description->clear();

    lineEdit_details_de_paiement_reference_recu_paiement_client->clearField();
    lineEdit_details_de_paiement_nom_de_lentreprise->clearField();
    lineEdit_details_de_paiement_reference->clearField();
    lineEdit_details_de_paiement_typedepaiement->clearField();
    lineEdit_details_de_paiement_nom_de_lencaisseur->clearField();
    lineEdit_details_de_paiement_compte_client->clearField();
    lineEdit_details_de_paiement_montant_paye->clearField();
    lineEdit_details_de_paiement_heure_de_paiement->clearField();
}

void YerithPaiementsWindow::setupDateTimeEdits()
{
    dateEdit_details_de_paiement_date_paiement->setYerithEnabled(false);

    dateEdit_paiements_debut->
    setStartDate(YerithERPConfig::GET_YERITH_PAGING_DEFAULT_START_DATE());

    dateEdit_paiements_fin->setStartDate(GET_CURRENT_DATE);

    _paiementsDateFilter.clear();

    _paiementsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                                (YerithDatabaseTableColumn::DATE_PAIEMENT,
                                 DATE_TO_DB_FORMAT_STRING
                                 (dateEdit_paiements_debut->date()),
                                 YerithDatabaseTableColumn::DATE_PAIEMENT,
                                 DATE_TO_DB_FORMAT_STRING
                                 (dateEdit_paiements_fin->date())));

    connect(dateEdit_paiements_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));

    connect(dateEdit_paiements_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerithLineEdits()));
}


void YerithPaiementsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerithWindowsCommons::deconnecter_utilisateur();
}


void YerithPaiementsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curPaiementsTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_retour_paiements->enable(this, SLOT(retourPaiements()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_paiements_filtrer->enable(this, SLOT(filtrer_paiements()));
    pushButton_paiements_reinitialiser_filtre->enable(this,
                                                      SLOT
                                                      (reinitialiser_elements_filtrage
                                                       ()));
}


void YerithPaiementsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curPaiementsTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_retour_paiements->enable(this, SLOT(retourPaiements()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_paiements_filtrer->enable(this, SLOT(filtrer_paiements()));
    pushButton_paiements_reinitialiser_filtre->enable(this,
                                                      SLOT
                                                      (reinitialiser_elements_filtrage
                                                       ()));
}


void YerithPaiementsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    (tableView_paiements);

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_retour_paiements->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_paiements_filtrer->disable(this);
    pushButton_paiements_reinitialiser_filtre->disable(this);
}


bool YerithPaiementsWindow::export_csv_file()
{
    bool success = false;

    prepare__IN__for_export_and_printing();

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_paiements,
                                               tableColumnsToIgnore,
                                               "yerith-erp-payments-journal-csv-format",
                                               "payments listing file");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_paiements,
                                               tableColumnsToIgnore,
                                               "yerith-erp-journal-paiements-format-csv",
                                               "fiche des paiements");
    }

    prepare__OUT__for_export_and_printing();

    return success;
}


QString YerithPaiementsWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerithUtils::EN_template_journal_des_paiements_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_journal_des_paiements_tex;
    }

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESDEBUT",
				DATE_TO_STRING(dateEdit_paiements_debut->date()));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YERITHVENTESFIN",
				DATE_TO_STRING(dateEdit_paiements_fin->date()));


    return _latex_template_print_pdf_content;
}


bool YerithPaiementsWindow::imprimer_pdf_document()
{
    get_latex_template_print_pdf_content();

    prepare__IN__for_export_and_printing();

    bool retValue = YerithWindowsCommons::imprimer_pdf_document();

    prepare__OUT__for_export_and_printing();

    return retValue;
}


void YerithPaiementsWindow::resetFilter(YerithSqlTableModel *
                                        historiquePaiementsTableModel)
{
    _curPaiementsTableModel = historiquePaiementsTableModel;

    if (_curPaiementsTableModel)
    {
        _curPaiementsTableModel->resetFilter();
    }

    dateEdit_paiements_debut->reset();
    dateEdit_paiements_fin->reset();
}


void YerithPaiementsWindow::retourPaiements()
{
    _currentTabView = TableauDesPaiements;
    tabWidget_paiements->setCurrentIndex(TableauDesPaiements);
}


void YerithPaiementsWindow::handleCurrentChanged(int index)
{
    _currentTabView = index;

    switch (index)
    {
    case TableauDesPaiements:
        lister_les_elements_du_tableau();
        enable_on_print_actions_SET_VISIBILITY();
        break;

    case AfficherPaiementAuDetail:
        afficher_paiements_detail();
        disable_on_print_actions_SET_VISIBILITY();
        break;

    default:
        break;
    }
}


void YerithPaiementsWindow::lister_les_elements_du_tableau(YerithSqlTableModel &
                                                           paiementsTableModel)
{
    int curPaiementsTableModelRowCount =
    		_curPaiementsTableModel->easySelect("src/windows/yerith-erp-paiements-window.cpp", 1394);

    if (curPaiementsTableModelRowCount < 0)
    {
        curPaiementsTableModelRowCount = 0;
    }


    QMap < QString, double >client_company_name_TO_financial_account_payment;

    QMap < QString, double >supplier_company_name_TO_financial_account_payment;


    double montant_total_paye_AUX_CLIENTS = 0.0;

    double montant_total_paye_PAR_LES_FOURNISSEURS = 0.0;


    double montant_total_paye_par_les_clients = 0.0;

    double montant_total_paye_aux_fournisseurs = 0.0;


    double compte_client_apres_paiement = 0.0;


    double montant_paye_AU_CLIENT = 0.0;

    double montant_paye_PAR_LE_FOURNISSEUR = 0.0;


    double montant_paye_par_le_client = 0.0;

    double montant_paye_au_fournisseur = 0.0;


    int type_de_paiement = YerithUtils::ENCAISSEMENT_INDEFINI;


    QSqlField QSQLFIELD_compte_fournisseur;

    QSqlRecord aRecord;

    for (int k = 0; k < curPaiementsTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = _curPaiementsTableModel->record(k);

        type_de_paiement =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).
                        toInt();

        QSQLFIELD_compte_fournisseur =
                        aRecord.field(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR);

        if (QSQLFIELD_compte_fournisseur.isNull())
        {
            montant_paye_par_le_client =
                            YerithUtils::montant_paye_par_le_client(aRecord);

            if (montant_paye_par_le_client > 0)
            {
                if (type_de_paiement !=
                        YerithUtils::ENCAISSEMENT_AVANCE_DE_CASH)
                {
                    montant_total_paye_par_les_clients +=
                                    montant_paye_par_le_client;
                }

                compte_client_apres_paiement =
                                GET_SQL_RECORD_DATA(aRecord,
                                                    YerithDatabaseTableColumn::
                                                    COMPTE_CLIENT).toDouble();

                client_company_name_TO_financial_account_payment.insert
                (GET_SQL_RECORD_DATA
                 (aRecord, YerithDatabaseTableColumn::NOM_ENTREPRISE),
                 compte_client_apres_paiement);
            }
            else
            {
                montant_paye_AU_CLIENT =
                                YerithUtils::montant_paye_AU_CLIENT(aRecord);

                if (montant_paye_AU_CLIENT < 0)
                {
                    montant_total_paye_AUX_CLIENTS += montant_paye_AU_CLIENT;

                    compte_client_apres_paiement =
                                    GET_SQL_RECORD_DATA(aRecord,
                                                        YerithDatabaseTableColumn::
                                                        COMPTE_CLIENT).toDouble();

                    client_company_name_TO_financial_account_payment.insert
                    (GET_SQL_RECORD_DATA
                     (aRecord, YerithDatabaseTableColumn::NOM_ENTREPRISE),
                     compte_client_apres_paiement);
                }
            }
        }
        else
        {
            montant_paye_au_fournisseur =
                            YerithUtils::montant_paye_au_fournisseur_OU_Employe(aRecord);

            if (montant_paye_au_fournisseur < 0)
            {
                montant_total_paye_aux_fournisseurs +=
                                montant_paye_au_fournisseur;

                supplier_company_name_TO_financial_account_payment.insert
                (GET_SQL_RECORD_DATA
                 (aRecord, YerithDatabaseTableColumn::NOM_ENTREPRISE),
                 montant_paye_au_fournisseur);
            }
            else
            {
                montant_paye_PAR_LE_FOURNISSEUR =
                                YerithUtils::montant_paye_PAR_LE_FOURNISSEUR(aRecord);

                if (montant_paye_PAR_LE_FOURNISSEUR > 0)
                {
                    montant_total_paye_PAR_LES_FOURNISSEURS +=
                                    montant_paye_PAR_LE_FOURNISSEUR;

                    supplier_company_name_TO_financial_account_payment.insert
                    (GET_SQL_RECORD_DATA
                     (aRecord, YerithDatabaseTableColumn::NOM_ENTREPRISE),
                     montant_paye_PAR_LE_FOURNISSEUR);
                }
            }
        }
    }

    lineEdit_paiements_nombre_paiements->setText(GET_NUM_STRING
                                                 (curPaiementsTableModelRowCount));


    double TOTAL_ENCAISSE_CREDITE =
                    montant_total_paye_par_les_clients +
                    montant_total_paye_PAR_LES_FOURNISSEURS;

    double TOTAL_DECAISSE_DEDITE =
                    montant_total_paye_AUX_CLIENTS + montant_total_paye_aux_fournisseurs;


    lineEdit_paiements_montant_encaisse_credite->setText
    (GET_CURRENCY_STRING_NUM(TOTAL_ENCAISSE_CREDITE));

    lineEdit_paiements_montant_decaisse_debite->setText(GET_CURRENCY_STRING_NUM
                                                        (TOTAL_DECAISSE_DEDITE));


    if (YerithUtils::isEqualCaseInsensitive
            (YerithPaiementsWindow::CLIENT_TEXT_STRING,
             comboBox_paiements_type_dentreprise->currentText()))
    {
        lineEdit_paiements_nombre_de_clients_fournisseurs->setText
        (GET_NUM_STRING
         (client_company_name_TO_financial_account_payment.keys().size()));

        label_paiements_nombre_de_clients_fournisseurs->setText(QObject::tr
                                                                ("# clients"));
    }
    else
    {
        label_paiements_nombre_de_clients_fournisseurs->setText(QObject::tr
                                                                ("# fournisseurs"));

        lineEdit_paiements_nombre_de_clients_fournisseurs->setText
        (GET_NUM_STRING
         (supplier_company_name_TO_financial_account_payment.keys().
          size()));
    }


    tableView_paiements->queryYerithTableViewCurrentPageContentRow
    (paiementsTableModel);

    tableView_show_or_hide_columns(*tableView_paiements);

    if (tableView_paiements->rowCount() > 0)
    {
        tabWidget_paiements->setTabEnabled(AfficherPaiementAuDetail, true);
    }
    else
    {
        tabWidget_paiements->setTabEnabled(AfficherPaiementAuDetail, false);
    }
}


void YerithPaiementsWindow::rendreInvisible()
{
    clear_all_fields();
    YerithWindowsCommons::rendreInvisible();
}



void YerithPaiementsWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
    retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;


    _curStocksTableModel = stocksTableModel;

    _curPaiementsTableModel = &_allWindows->getSqlTableModel_paiements();


    setYerithSqlTableModel(_curPaiementsTableModel);


    populateComboBoxes();


    setupLineEditsQCompleters((QObject *) this);


    tabWidget_paiements->setCurrentIndex(TableauDesPaiements);


    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            comboBox_paiements_type_dentreprise->setEnabled(true);
        }
        else
        {
            comboBox_paiements_type_dentreprise->setCurrentIndex
            (comboBox_paiements_type_dentreprise->findText
             (YerithPaiementsWindow::CLIENT_TEXT_STRING));

            comboBox_paiements_type_dentreprise->setEnabled(false);
        }
    }


    handle_combobox_type_dentreprise(comboBox_paiements_type_dentreprise->currentText());


    setVisible(true);

    afficher_paiements();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("paiements clients et fournisseurs"),
    								 _curPaiementsTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    lineEdit_paiements_terme_recherche->setFocus();
}


void YerithPaiementsWindow::afficher_paiements_detail()
{
    if (tableView_paiements->rowCount() <= 0)
    {
        return;
    }

    QSqlRecord record;

    _allWindows->
    _paiementsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    textEdit_description->setText(GET_SQL_RECORD_DATA
                                  (record, YerithDatabaseTableColumn::NOTES));

    lineEdit_details_de_paiement_reference_recu_paiement_client->setText
    (GET_SQL_RECORD_DATA
     (record, YerithDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT));
    lineEdit_details_de_paiement_nom_de_lentreprise->setText
    (GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NOM_ENTREPRISE));
    lineEdit_details_de_paiement_nom_de_lencaisseur->setText
    (GET_SQL_RECORD_DATA(record, YerithDatabaseTableColumn::NOM_ENCAISSEUR));

    lineEdit_details_de_paiement_heure_de_paiement->setText(GET_SQL_RECORD_DATA
                                                            (record,
                                                             YerithDatabaseTableColumn::HEURE_PAIEMENT));

    lineEdit_details_de_paiement_reference->setText(GET_SQL_RECORD_DATA
                                                    (record,
                                                     YerithDatabaseTableColumn::REFERENCE));

    int typeDePaiementIntValue = GET_SQL_RECORD_DATA(record,
                                                     YerithDatabaseTableColumn::TYPE_DE_PAIEMENT).
                                 toInt();

    lineEdit_details_de_paiement_typedepaiement->
    setText(YerithUtils::_typedepaiementToUserViewString.
            value(typeDePaiementIntValue));

    double aDoubleValue = GET_SQL_RECORD_DATA(record,
                                              YerithDatabaseTableColumn::COMPTE_CLIENT).
                          toDouble();

    lineEdit_details_de_paiement_compte_client->setText(GET_CURRENCY_STRING_NUM
                                                        (aDoubleValue));

    aDoubleValue =
                    GET_SQL_RECORD_DATA(record,
                                        YerithDatabaseTableColumn::MONTANT_PAYE).toDouble();

    lineEdit_details_de_paiement_montant_paye->setText(GET_CURRENCY_STRING_NUM
                                                       (aDoubleValue));

    dateEdit_details_de_paiement_date_paiement->setDate(GET_DATE_FROM_STRING
                                                        (GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerithDatabaseTableColumn::DATE_PAIEMENT)));

    tabWidget_paiements->setCurrentIndex(AfficherPaiementAuDetail);
}


void YerithPaiementsWindow::reinitialiser_elements_filtrage()
{
    lineEdit_paiements_element_de_paiements_resultat->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithPaiementsWindow::reinitialiser_recherche()
{
    lineEdit_paiements_element_de_paiements_resultat->clear();

    lineEdit_nom_element_string_db->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_paiements());

    resetLineEditsQCompleters((QObject *) this);

    comboBox_paiements_type_dentreprise->find_AND_SET_CURRENT_INDEX
    (CLIENT_TEXT_STRING);

    afficher_paiements();

    lineEdit_paiements_terme_recherche->setFocus();
}


void YerithPaiementsWindow::refineYerithLineEdits()
{
    _paiementsDateFilter.clear();

    _paiementsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                                (YerithDatabaseTableColumn::DATE_PAIEMENT,
                                 DATE_TO_DB_FORMAT_STRING
                                 (dateEdit_paiements_debut->date()),
                                 YerithDatabaseTableColumn::DATE_PAIEMENT,
                                 DATE_TO_DB_FORMAT_STRING
                                 (dateEdit_paiements_fin->date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_paiements();
}
