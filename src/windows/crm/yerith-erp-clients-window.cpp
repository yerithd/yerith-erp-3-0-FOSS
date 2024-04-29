/*
 * yerith-erp-clients-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-clients-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/process/yerith-erp-process.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/widgets/yerith-erp-qmessage-box.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerithERPClientsWindow::YerithERPClientsWindow()
:YerithWindowsCommons("yerith-erp-clients"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::CLIENTS),
 _logger(new YerithLogger("YerithERPComptesClientsWindow")),
 _pushButton_filtrer_font(0),
 _curClientsTableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("clients"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_clients);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
            << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionSupprimerCompteClient
        << action_vendre_au_compte_client
        << actionModifierCompteClient
        << actionPayerAuCompteClient
        << action_afficher_groupes_du_client
		<< actionAfficherDetailsClient;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_clients)


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
                        .arg(COLOUR_RGB_STRING_YERITH_GOLD_245_194_17,
                             COLOUR_RGB_STRING_YERITH_BLUE_53_132_228);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerithDatabaseTableColumn::GROUPES_DU_CLIENT
            << YerithDatabaseTableColumn::GROUPES_DU_CLIENT_ID
            << YerithDatabaseTableColumn::IMAGE_COMPTE_CLIENT;


    setup_select_configure_dbcolumn(YerithDatabase::CLIENTS);


    _lineEditsToANDContentForSearch.insert
    (&lineEdit_comptes_clients_terme_recherche, YerithUtils::EMPTY_STRING);

    _yerith_WINDOW_references_dbColumnString.
    insert(YerithDatabaseTableColumn::REFERENCE_CLIENT);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION
    (YerithDatabase::CLIENTS);

    reinitialiser_colones_db_visibles();

    _curClientsTableModel = &_allWindows->getSqlTableModel_clients();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    populateClientsComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_clients->setSqlTableName(&YerithDatabase::CLIENTS);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_vendre_au_compte_client,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_groupes_de_clients->disable(this);
    pushButton_programmes_de_fidelite_clients->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    //Menu actions
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
    connect(actionPayerAuCompteClient, SIGNAL(triggered()), this,
            SLOT(private_payer_au_compteclient()));
    connect(actionCreerCompteClient, SIGNAL(triggered()), this,
            SLOT(creerCompteClient()));
    connect(actionModifierCompteClient, SIGNAL(triggered()), this,
            SLOT(modifierCompteClient()));
    connect(actionSupprimerCompteClient, SIGNAL(triggered()), this,
            SLOT(supprimerCompteClient()));
    connect(action_vendre_au_compte_client, SIGNAL(triggered()), this,
            SLOT(vendre_au_COMPTE_CLIENT()));
    connect(action_afficher_groupes_du_client, SIGNAL(triggered()), this,
            SLOT(afficher_groupes_dun_client()));
    connect(actionAfficherDetailsClient, SIGNAL(triggered()), this,
            SLOT(afficher_au_detail()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this,
            SLOT(reinitialiser_elements_filtrage()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

#ifdef YERITH_CLIENT

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YERITH_SERVER, YERITH_STANDALONE, YERITH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#endif

    connect(tableView_clients,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(private_payer_au_compteclient(const QModelIndex &)));

    setupShortcuts();
}


YerithERPClientsWindow::~YerithERPClientsWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR

    delete _pushButton_filtrer_font;

    delete _logger;
}


void YerithERPClientsWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::LOCALISATION
            << YerithDatabaseTableColumn::REFERENCE_CLIENT
            << YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH
            << YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS
            << YerithDatabaseTableColumn::COMPTE_CLIENT
            << YerithDatabaseTableColumn::NOM_ENTREPRISE
            << YerithDatabaseTableColumn::VILLE
            << YerithDatabaseTableColumn::EMAIL
            << YerithDatabaseTableColumn::NUMERO_TELEPHONE_1;
}


void YerithERPClientsWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_clientsWindow->palette());
    menu.addAction(actionAfficherDetailsClient);
    menu.addAction(action_afficher_groupes_du_client);
    menu.addAction(actionPayerAuCompteClient);
    menu.addAction(actionModifierCompteClient);
    menu.addAction(action_afficher_groupes_du_client);
    menu.addAction(actionSupprimerCompteClient);
    menu.addAction(action_vendre_au_compte_client);
    menu.exec(event->globalPos());
}


void YerithERPClientsWindow::hideEvent(QHideEvent *hideEvent)
{
    YerithWindowsCommons::
    CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();

    _allWindows->_transactionsDunClientWindow->close();
}


void YerithERPClientsWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);

    actionReinitialiserRecherche->
    setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithERPClientsWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
        _pushButton_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_filtrer_font->setUnderline(false);
    }

    pushButton_filtrer->setFont(*_pushButton_filtrer_font);
}


void YerithERPClientsWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    afficherClients();
}


void YerithERPClientsWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_comptes_clients_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("(%1 OR %2 OR %3)").arg
                                 (GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::EMAIL,
                                   partSearchTerm),
                                  GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::NOM_REPRESENTANT,
                                   partSearchTerm),
                                  GENERATE_SQL_LIKE_STMT
                                  (YerithDatabaseTableColumn::
                                   DESCRIPTION_CLIENT, partSearchTerm)));

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


    YerithLineEdit *aYerithLineEdit = 0;

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    QMapIterator < YerithLineEdit **,
                 QString > it(_lineEditsToANDContentForSearch);

    while (it.hasNext())
    {
        it.next();

        aYerithLineEdit = *it.key();

        correspondingDBFieldKeyValue = it.value();

        if (0 != aYerithLineEdit)
        {
            aTableColumnFieldContentForANDSearch = aYerithLineEdit->text();

            if (!correspondingDBFieldKeyValue.isEmpty() &&
                    !aTableColumnFieldContentForANDSearch.isEmpty())
            {
                if (!_searchFilter.isEmpty())
                {
                    _searchFilter.append(" AND ");
                }

                _searchFilter.append(GENERATE_SQL_IS_STMT
                                     (correspondingDBFieldKeyValue,
                                      aTableColumnFieldContentForANDSearch));
            }
        }
    }

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER
    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(_searchFilter);

    if (_yerithSqlTableModel->select())
    {
        afficherClients(*_yerithSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerithERPClientsWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerithERPClientsWindow::afficher_groupes_dun_client()
{
    if (_curClientsTableModel->rowCount() > 0)
    {
        rendreInvisible();

        _allWindows->_groupesDunClientWindow->rendreVisible(_curClientsTableModel,
        													_curStocksTableModel);
    }
    else
    {
        YerithQMessageBox::information(this,
                                       QObject::tr("groupes d'appartenance"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client afin de visualiser ses groupes d'appartenance."));
    }
}


void YerithERPClientsWindow::private_payer_au_compteclient()
{
    if (_curClientsTableModel->rowCount() > 0)
    {
        rendreInvisible();

        _allWindows->_payerAuCompteClientWindow->rendreVisible(_curClientsTableModel,
        													   _curStocksTableModel);
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("verser"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client afin d'effectuer un versement."));
    }
}


void YerithERPClientsWindow::private_payer_au_compteclient(const QModelIndex &aModelIndex)
{
    if (_curClientsTableModel->rowCount() > 0)
    {
        rendreInvisible();

        _allWindows->_payerAuCompteClientWindow->rendreVisible(_curClientsTableModel,
        													   _curStocksTableModel);
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("verser"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client afin d'effectuer un versement."));
    }
}


void YerithERPClientsWindow::modifierCompteClient()
{
    if (_curClientsTableModel->rowCount() > 0)
    {
        rendreInvisible();

        _allWindows->_modifierCompteClientWindow->rendreVisible(_curClientsTableModel,
        														_curStocksTableModel);
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("modifier"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client à modifier."));
    }
}


void YerithERPClientsWindow::supprimer_PLUSIEURS_Clients(YerithSqlTableModel &
                                                         aClientsTableModel)
{
    QString YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING;

    QMapIterator < QString,
                 QString > j(tableView_clients->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();

        YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING.append(QString
                                                                   ("DELETE FROM %1 WHERE %2 = '%3';").
                                                                   arg
                                                                   (YerithDatabase::CLIENTS,
                                                                    YerithDatabaseTableColumn::ID,
                                                                    j.value
                                                                    ()));
    }

    QString
    msgConfirmation(QObject::tr
                    ("Supprimer les clients sélectionés ?"));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool success =
                        YerithUtils::execQuery
                        (YERITH_TABLE_VIEW_DELETE_SELECTED_ROWS_QUERY_STRING);

        QString msg(QObject::tr("Les clients sélectionés"));

        if (success && aClientsTableModel.select())
        {
            setupLineEditsQCompleters((QObject *) this);

            msg.append(QObject::tr
                       (" ont été supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"), msg,
                                           QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'ont pas pu être supprimés de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - échec"), msg,
                                           QMessageBox::Ok);
        }
    }
}


void YerithERPClientsWindow::supprimerCompteClient()
{
    if (getLastListerSelectedRow__ID_AS_INTEGER() <= -1 ||
            _curClientsTableModel->rowCount() <= 0)
    {
        YerithQMessageBox::information(this, QObject::tr("supprimer"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client à supprimer."));

        return;
    }

    YerithSqlTableModel *clientsTableModel = 0;

    if (_curClientsTableModel &&
            YerithUtils::isEqualCaseInsensitive(YerithDatabase::CLIENTS,
                                                _curClientsTableModel->sqlTableName
                                                ()))
    {
        clientsTableModel = _curClientsTableModel;
    }
    else
    {
        return;
    }

    if (tableView_clients->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_Clients(*clientsTableModel);

        tableView_clients->clearSelection();

        afficherClients();

        return;
    }

    QSqlRecord record;

    _allWindows->
    _clientsWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    if (record.isEmpty()
            || record.isNull(YerithDatabaseTableColumn::NOM_ENTREPRISE))
    {
        return;
    }

    QString
    nom_entreprise(GET_SQL_RECORD_DATA
                   (record, YerithDatabaseTableColumn::NOM_ENTREPRISE));

    QString msgConfirmation(QObject::tr("Supprimer le client '%1' ?").arg
                            (nom_entreprise));

    if (QMessageBox::Ok ==
            YerithQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgConfirmation,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool success =
                        _allWindows->
                        _clientsWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW();

        QString msg(QObject::tr("Le client '%1").arg(nom_entreprise));

        if (success && clientsTableModel->select())
        {
            setupLineEditsQCompleters((QObject *) this);

            tableView_clients->clearSelection();

            afficherClients();

            msg.append(QObject::tr
                       ("' a été supprimée de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"), msg,
                                           QMessageBox::Ok);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'a pas été supprimée de la base de données !"));

            YerithQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - échec"), msg,
                                           QMessageBox::Ok);
        }
    }
}


void YerithERPClientsWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (_curClientsTableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_clientsDetailWindow->rendreVisible(_curClientsTableModel,
                                                         _curStocksTableModel);

        rendreInvisible();
    }
    else
    {
        YerithQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client à afficher les détails."));
    }
}


bool YerithERPClientsWindow::filtrer()
{
    QString stockTableColumnValue(lineEdit_resultat_filtre->text());

    if (stockTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche."));

        YerithQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;
    }

    QString stockTableColumnProperty(comboBox_element->currentText());

    QString mathOperator(comboBox_condition->currentText());

    QString filterString;

    QString
    REAL_DB_ID_NAME_stockTableColumnProperty
    (YerithDatabaseTableColumn::_tableColumnToUserViewString.key
     (stockTableColumnProperty));

    filterString.append(QString("%2 %3 '%4'").arg
                        (REAL_DB_ID_NAME_stockTableColumnProperty,
                         mathOperator, stockTableColumnValue));

    //qDebug() << QString("filterString: %1")
    //                              .arg(filterString);

    _curClientsTableModel->yerithSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curClientsTableModel->easySelect("src/windows/crm/yerith-erp-clients-windows.cpp", 696);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        afficherClients(*_curClientsTableModel);

        YERITH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "clients - filtrer");

        return true;
    }
    else
    {
        YERITH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "clients - filtrer");
    }

    return false;
}


void YerithERPClientsWindow::reinitialiser_elements_filtrage()
{
    lineEdit_resultat_filtre->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerithERPClientsWindow::reinitialiser_recherche()
{
    lineEdit_nom_element_string_db->clear();

    lineEdit_resultat_filtre->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *) this);

    afficherClients();
}


void YerithERPClientsWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element->setVisible(IS_VISIBLE);
    comboBox_condition->setVisible(IS_VISIBLE);
    lineEdit_resultat_filtre->setVisible(IS_VISIBLE);
    pushButton_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_filtrer->setVisible(IS_VISIBLE);
}


void YerithERPClientsWindow::initialiser_FILTRES()
{
    comboBox_element->setVisible(false);
    comboBox_condition->setVisible(false);
    lineEdit_resultat_filtre->setVisible(false);
    pushButton_reinitialiser_filtre->setVisible(false);
    pushButton_filtrer->setVisible(false);
}


void YerithERPClientsWindow::populateClientsComboBoxes()
{
    _logger->log("populateClientsComboBoxes");

    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    YerithUtils::REMOVE_QSTRING_LIST(aQStringList,
                                     _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME);

    aQStringList.removeAll(YerithDatabaseTableColumn::DESCRIPTION_CLIENT);
    aQStringList.removeAll(YerithDatabaseTableColumn::NUMERO_TELEPHONE_1);
    aQStringList.removeAll(YerithDatabaseTableColumn::NUMERO_TELEPHONE_2);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::_tableColumnToUserViewString);


    aQStringList.clear();

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::COMPTE_CLIENT));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::
                         COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS));

    aQStringList.append(YERITH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerithDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT));



    comboBox_element->clear();

    comboBox_element->addItems(aQStringList);



    comboBox_condition->clear();

    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");

    comboBox_condition->addItems(aQStringList);
}


void YerithERPClientsWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_clients_nombre_de_lignes_par_page;

    lineEdit_comptes_clients_terme_recherche->enableForSearch(QObject::tr
                                                              ("terme à rechercher (émail, réprésentant, et description de l'entreprise cliente)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_nombre_de_comptes_clients->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_clients_nombre_de_lignes_par_page, tableView_clients);

    lineEdit_resultat_filtre->setValidator(&YerithUtils::DoubleValidator);
}


void YerithERPClientsWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
    retranslateUi(this);

    YERITH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    populateClientsComboBoxes();

    setYerithSqlTableModel(_curClientsTableModel);

    _curStocksTableModel = stocksTableModel;

    setVisible(true);

    afficherClients(*_curClientsTableModel);

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("clients"),
    								 _curClientsTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    lineEdit_comptes_clients_terme_recherche->setFocus();
}


void YerithERPClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_du_client,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_du_client,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_vendre_au_compte_client,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curClientsTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_compte_client->enable(this, SLOT(creerCompteClient()));
    pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_programmes_de_fidelite_clients->enable(this,
                                                      SLOT
                                                      (programmes_de_fidelite_clients
                                                       ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
}


void YerithERPClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_du_client,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_du_client,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_vendre_au_compte_client,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curClientsTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_creer_compte_client->enable(this, SLOT(creerCompteClient()));
    pushButton_groupes_de_clients->disable(this);
    pushButton_programmes_de_fidelite_clients->disable(this);

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));
    pushButton_filtrer->enable(this, SLOT(filtrer()));
    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
}


void YerithERPClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_du_client,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherDetailsClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPayerAuCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCreerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_afficher_groupes_du_client,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_vendre_au_compte_client,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_creer_compte_client->disable(this);
    pushButton_groupes_de_clients->disable(this);
    pushButton_programmes_de_fidelite_clients->disable(this);

    pushButton_reinitialiser->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_reinitialiser_filtre->disable(this);
}


void YerithERPClientsWindow::afficher_nom_entreprise_selectioner(const QString &
                                                                 nomEntreprise)
{
    _logger->log("afficher_nom_entreprise_selectioner(const QString &)");

    QString
    filter(GENERATE_SQL_IS_STMT
           (YerithDatabaseTableColumn::NOM_ENTREPRISE, nomEntreprise));

    //qDebug() << QString("filter: %1").arg(filter);

    _curClientsTableModel->yerithSetFilter_WITH_where_clause(filter);

    if (_curClientsTableModel->easySelect("src/windows/crm/yerith-erp-clients-windows.cpp", 1038) > 0)
    {
        afficherClients(*_curClientsTableModel);
    }
}


void YerithERPClientsWindow::afficherClients(YerithSqlTableModel &clientSqlTableModel)
{
    tableView_clients->queryYerithTableViewCurrentPageContentRow(clientSqlTableModel);

    tableView_show_or_hide_columns(*tableView_clients);

    int rowCount = tableView_clients->rowCount();

    lineEdit_nombre_de_comptes_clients->setText(GET_NUM_STRING(rowCount));
}


bool YerithERPClientsWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_clients,
                                               tableColumnsToIgnore,
                                               "yerith-erp-customer-account-listing-csv-format",
                                               "customer account listing file");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_clients,
                                               tableColumnsToIgnore,
                                               "yerith-erp-fiche-clients-format-csv",
                                               "fiche des comptes clients");
    }

    return success;
}


QString YerithERPClientsWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _latex_template_print_pdf_content = YerithUtils::EN_template_comptes_clients_tex;
    }
    else
    {
        _latex_template_print_pdf_content = YerithUtils::FR_template_comptes_clients_tex;
    }

    return _latex_template_print_pdf_content;
}
