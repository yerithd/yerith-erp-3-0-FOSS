/*
 * yerith-erp-alertes-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-alertes-window.hpp"


#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/utils/yerith-erp-style.hpp"

#include "src/utils/yerith-erp-sqltable-model.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/QThread>

#include <QtCore/QProcess>

#include <QtCore/qmath.h>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <unistd.h>


/**
 * yerith-erp-windows.hpp cannot be included in
 * the header file because it will caused a circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */


YerithAlertesWindow::YerithAlertesWindow()
:YerithWindowsCommons("yerith-erp-alertes"),
 YerithAbstractClassYerithSearchWindow(YerithDatabase::COURRIERS_ALERTES),
 _logger(new YerithLogger("YerithAlertesWindow")),
 _curAlertesSqlTableModel(&_allWindows->getSqlTableModel_courriers_alertes())
{
    setYerithSqlTableModel(_curAlertesSqlTableModel);

    _windowName = QString("%1 - %2")
                	.arg(GET_YERITH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("fiche des alertes"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    checkBox_EXACT_STRING_MATCH->setChecked(true);


    setYerithTableView_FROM_WINDOWS_COMMONS(tableView_alertes);


    _list_yerith_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
        << pushButton_afficher_detail
        << pushButton_marquer_resolue
		<< pushButton_supprimer;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionMarquerResolue
		<< actionAfficher_alerte_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_alertes)


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_ALERT_BLUE_52_101_164,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);


    setup_select_configure_dbcolumn(YerithDatabase::COURRIERS_ALERTES);


    _lineEditsToANDContentForSearch.insert(&lineEdit_alertes_terme_recherche,
                                           YerithUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_alertes_destinataire_2,
                                           YerithDatabaseTableColumn::
                                           DESTINATAIRE);

    _comboBoxesToANDContentForSearch.insert(&comboBox_alertes_resolue_oui_OU_non,
    										YerithDatabaseTableColumn::ALERTE_RESOLUE);

    YERITH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerithDatabase::COURRIERS_ALERTES);

    reinitialiser_colones_db_visibles();

    _curAlertesSqlTableModel =
                    &_allWindows->getSqlTableModel_courriers_alertes();

    comboBox_alertes_condition->setYerithEnabled(false);

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    tableView_alertes->setSqlTableName(&YerithDatabase::COURRIERS_ALERTES);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS

    textEdit_alerte_message->setYerithEnabled(false);

    dateEdit_alertes_date_debut->setYerithEnabled(false);
    dateEdit_alertes_date_fin->setYerithEnabled(false);

    radioButton_alertes_periode_temps->setEnabled(false);
    radioButton_alertes_quantite->setEnabled(false);

    tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, false);

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_reinitialiser->disable(this);


    connect(action_TRADUIRE_EN_ANGLAIS,
            SIGNAL(triggered()),
            this,
            SLOT(CHANGER_DE_LANGUE()));


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    connect(tabWidget_alertes, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentTabChanged(int)));

    connect(tableView_alertes, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(afficher_au_detail()));

    /** Menu actions */
    connect(actionReinitialiserChampsDBVisible,
    		SIGNAL(triggered()),
			this,
			SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible,
    		SIGNAL(triggered()),
			this,
			SLOT(selectionner_champs_db_visibles()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAlertes, SIGNAL(triggered()), this,
            SLOT(afficher_alertes()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionAfficher_au_detail, SIGNAL(triggered()), this,
            SLOT(afficher_au_detail()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));


#ifdef YERITH_SERVER
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif


    setCenterAligned_FOR_YERITH_PDF_LATEX_PRINTING
    (YerithDatabaseTableColumn::ALERTE_RESOLUE);

    setRightAligned_FOR_YERITH_PDF_LATEX_PRINTING
    (YerithDatabaseTableColumn::CONDITION_ALERTE);

    setupShortcuts();
}

YerithAlertesWindow::~YerithAlertesWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerithAlertesWindow::afficher_alertes()
{
    textChangedSearchLineEditsQCompleters();

    if (0 >= tableView_alertes->rowCount())
    {
        return;
    }
    else
    {
        tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, true);
        tabWidget_alertes->setCurrentIndex(TableauDesAlertes);
    }
}


void YerithAlertesWindow::marquer_resolue()
{
    YerithSqlTableModel &courrierAlertesSqlTableModel =
                    _allWindows->getSqlTableModel_courriers_alertes();

    bool courrierAlertesSqlTableModelFILTERED = false;

    if (_allWindows->getUser()->isMagasinier()
            || _allWindows->getUser()->isCaissier())
    {
        QString
        userFilter(GENERATE_SQL_IS_STMT
                   (YerithDatabaseTableColumn::DESTINATAIRE,
                    _allWindows->getUser()->nom_utilisateur()));

        courrierAlertesSqlTableModel.yerithSetFilter_WITH_where_clause
        (userFilter);

        courrierAlertesSqlTableModelFILTERED = true;
    }

    QString
    userFilter(GENERATE_SQL_IS_STMT
               (YerithDatabaseTableColumn::DESTINATAIRE,
                _allWindows->getUser()->nom_utilisateur()));

    QSqlRecord courriersAlertesRecord;

    _allWindows->
    _listerAlertesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (courriersAlertesRecord);

    QString id_alerte = GET_SQL_RECORD_DATA(courriersAlertesRecord,
                                            YerithDatabaseTableColumn::ID_ALERTE);

    YerithSqlTableModel &alertesSqlTableModel =
                    _allWindows->getSqlTableModel_alertes();

    QString
    alerteFilter(GENERATE_SQL_IS_STMT
                 (YerithDatabaseTableColumn::ID, id_alerte));

    alertesSqlTableModel.yerithSetFilter_WITH_where_clause(alerteFilter);

    int alertesSqlTableModelRowCount =
    		alertesSqlTableModel.easySelect("src/windows/yerith-erp-alertes-windows.cpp", 284);

    if (alertesSqlTableModelRowCount <= 0)
    {
        if (courrierAlertesSqlTableModelFILTERED)
        {
            courrierAlertesSqlTableModel.resetFilter("src/windows/yerith-erp-alertes-window.cpp");
        }

        alertesSqlTableModel.resetFilter("src/windows/yerith-erp-alertes-window.cpp");

        return;
    }

    //Set in table 'alertes' that this alert has been resolved
    QSqlRecord alertesRecord = alertesSqlTableModel.record(0);

    if (1 ==
            GET_SQL_RECORD_DATA(alertesRecord,
                                YerithDatabaseTableColumn::ALERTE_RESOLUE).
            toInt())
    {
        if (courrierAlertesSqlTableModelFILTERED)
        {
            courrierAlertesSqlTableModel.resetFilter("src/windows/yerith-erp-alertes-window.cpp");
        }

        alertesSqlTableModel.resetFilter("src/windows/yerith-erp-alertes-window.cpp");

        YerithQMessageBox::information(this,
                                       QObject::tr("marquer résolue"),
                                       QObject::tr
                                       ("L'alerte '%1' est déjà marquée résolue !").
                                       arg(GET_SQL_RECORD_DATA
                                           (courriersAlertesRecord,
                                            YerithDatabaseTableColumn::
                                            DESIGNATION_ALERTE)));
        return;
    }

    alertesRecord.setValue(YerithDatabaseTableColumn::ALERTE_RESOLUE, 1);

    bool successAlertes = alertesSqlTableModel.updateRecord(0,
    														alertesRecord,
															"src/windows/yerith-erp-alertes-window.cpp",
															327);

    //Set in table 'courriers_alertes' that this alert has been resolved
    courriersAlertesRecord.setValue(YerithDatabaseTableColumn::ALERTE_RESOLUE,
                                    1);

    bool successCourriersAlertes =
                    _allWindows->
                    _listerAlertesWindow->SQL_UPDATE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
                    (courriersAlertesRecord);

//    QDEBUG_STRING_OUTPUT_2("successCourriersAlertes", BOOL_TO_STRING(successCourriersAlertes));

//    QDEBUG_STRING_OUTPUT_2("successAlertes", BOOL_TO_STRING(successAlertes));

    if (successAlertes && successCourriersAlertes)
    {
        afficher_alertes();

        YerithQMessageBox::information(this,
                                       QObject::tr
                                       ("marquer résolue - succès"),
                                       QObject::tr
                                       ("L'alerte '%1' a été marquée résolue !").
                                       arg(GET_SQL_RECORD_DATA
                                           (courriersAlertesRecord,
                                            YerithDatabaseTableColumn::
                                            DESIGNATION_ALERTE)));
    }
    else
    {
        YerithQMessageBox::warning(this,
                                   QObject::tr
                                   ("marquer résolue - échec"),
                                   QObject::tr
                                   ("L'alerte '%1' n'a pas pu être marquée résolue !\n "
                                    "Contacter l'administrateur de '%2' !").arg
                                   (GET_SQL_RECORD_DATA
                                    (courriersAlertesRecord,
                                     YerithDatabaseTableColumn::
                                     DESIGNATION_ALERTE),
                                    YerithUtils::APPLICATION_NAME));
    }

    if (courrierAlertesSqlTableModelFILTERED)
    {
        courrierAlertesSqlTableModel.resetFilter("src/windows/yerith-erp-alertes-window.cpp");
    }

    alertesSqlTableModel.resetFilter("src/windows/yerith-erp-alertes-window.cpp");
}


void YerithAlertesWindow::supprimer()
{
    //qDebug() << "YerithAlertesWindow::supprimer()";
    if (!_allWindows->getUser()->isManager())
    {
        return;
    }

    QSqlRecord record;

    _allWindows->
    _listerAlertesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    QString
    designation_alerte(GET_SQL_RECORD_DATA
                       (record,
                        YerithDatabaseTableColumn::DESIGNATION_ALERTE));

    if (!designation_alerte.isEmpty() && !designation_alerte.isNull())
    {
        QString msgSupprimer(QObject::tr
                             ("Poursuivre avec la suppression de l'alerte '%1' ?").
                             arg(designation_alerte));

        if (QMessageBox::Ok ==
                YerithQMessageBox::question(this,
                                            QObject::tr("suppression"),
                                            msgSupprimer,
                                            QMessageBox::Cancel, QMessageBox::Ok))
        {
            bool resRemoved =
                            _allWindows->
                            _listerAlertesWindow->SQL_DELETE_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
                            ();
            //qDebug() << "YerithStocksWindow::supprimer_du_stock() " << resRemoved;

            QString
            supprimerAlerteStr(QString("DELETE FROM %1 WHERE %2 = '%3'").arg
                               (YerithDatabase::ALERTES,
                                YerithDatabaseTableColumn::
                                DESIGNATION_ALERTE, designation_alerte));

            YerithUtils::execQuery(supprimerAlerteStr);

            afficher_alertes();

            if (resRemoved)
            {
                msgSupprimer =
                                QObject::tr("L'alerte '%1' a été supprimée !").arg
                                (designation_alerte);

                YerithQMessageBox::information(this,
                                               QObject::tr("supprimer"),
                                               msgSupprimer);
            }
            else
            {
                msgSupprimer =
                                QObject::tr
                                ("L'alerte '%1' ne pouvait pas être supprimée !").arg
                                (designation_alerte);

                YerithQMessageBox::information(this,
                                               QObject::tr("supprimer"),
                                               msgSupprimer);
            }
        }
        else
        {
        }
    }
}


void YerithAlertesWindow::setCurrentUser_NOM_UTILISATEUR(const QString &
                                                         aUserNomUtilisateur)
{
    lineEdit_alertes_destinataire_2->setYerithEnabled(false);
    lineEdit_alertes_destinataire_2->setText(aUserNomUtilisateur);
}


void YerithAlertesWindow::disable_NOM_UTILISATEUR()
{
    lineEdit_alertes_destinataire_2->setYerithEnabled(false);
    lineEdit_alertes_destinataire_2->myClear();
}


void YerithAlertesWindow::enable_NOM_UTILISATEUR_ONLY_MANAGER()
{
    lineEdit_alertes_destinataire_2->setYerithEnabled(true);
    lineEdit_alertes_destinataire_2->myClear();
}


void YerithAlertesWindow::clear_all_fields()
{
    lineEdit_alertes_nom->clear();
    lineEdit_alertes_designation->clear();
    comboBox_alertes_condition->clear();
    lineEdit_alertes_quantite->clear();
    lineEdit_alertes_destinataire->clear();
    lineEdit_alertes_nom_destinataire->clear();
    textEdit_alerte_message->clear();
}


void YerithAlertesWindow::populateComboBoxes()
{
    comboBox_alertes_resolue_oui_OU_non->clear();

    comboBox_alertes_resolue_oui_OU_non->addItem(YerithUtils::EMPTY_STRING);

    comboBox_alertes_resolue_oui_OU_non->addItem(YerithUtils::STRING_OUI);

    comboBox_alertes_resolue_oui_OU_non->addItem(YerithUtils::STRING_NON);


    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerithDatabaseTableColumn::ID);
    aQStringList.removeAll(YerithDatabaseTableColumn::ID_ALERTE);
    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_DEBUT);
    aQStringList.removeAll(YerithDatabaseTableColumn::DATE_FIN);
    aQStringList.removeAll(YerithDatabaseTableColumn::MESSAGE_ALERTE);
    aQStringList.removeAll(YerithDatabaseTableColumn::ALERTE_RESOLUE);
    aQStringList.removeAll(YerithDatabaseTableColumn::DESTINATAIRE);
    aQStringList.removeAll(YerithDatabaseTableColumn::CONDITION_ALERTE);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerithDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);
}


void YerithAlertesWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_alertes_nombre_de_lignes_par_page;

    lineEdit_alertes_terme_recherche->enableForSearch(QObject::tr("terme à rechercher (message d'alerte)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr("valeur à rechercher"));

    lineEdit_alertes_destinataire_2->enableForSearch
										(QObject::tr("%1 (nom d'utilisateur)")
                                                    .arg(YerithDatabaseTableColumn::DESTINATAIRE));

    lineEdit_alertes_nombre_dalertes->setYerithEnabled(false);


    lineEdit_alertes_nom->setYerithEnabled(false);
    lineEdit_alertes_quantite->setYerithEnabled(false);
    lineEdit_alertes_resolue->setYerithEnabled(false);
    lineEdit_alertes_designation->setYerithEnabled(false);
    lineEdit_alertes_destinataire->setYerithEnabled(false);
    lineEdit_alertes_nom_destinataire->setYerithEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_alertes_nombre_de_lignes_par_page,
    									tableView_alertes);

    lineEdit_alertes_terme_recherche->setFocus();
}


void YerithAlertesWindow::setupShortcuts()
{
    setupShortcut__INCREASE_FONT_SIZE(*actionAUGMENTER_LA_POLICE_DU_TABLEAU);

    setupShortcut__DECREASE_FONT_SIZE(*actiondiminuer_la_police_du_tableau);

    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionReinitialiserRecherche->setShortcut(YerithUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerithAlertesWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();

    if (0 != _curAlertesSqlTableModel)
    {
        lister_les_elements_du_tableau(*_curAlertesSqlTableModel);
    }
}


void YerithAlertesWindow::textChangedSearchLineEditsQCompleters()
{
    tabWidget_alertes->setCurrentIndex(TableauDesAlertes);

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_alertes_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
//              qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("%1").arg(GENERATE_SQL_LIKE_STMT
                                                   (YerithDatabaseTableColumn::
                                                    MESSAGE_ALERTE,
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

    {
        QString curSearchDBStr;

        YerithComboBox *aYerithComboBox = 0;

        QMapIterator < YerithComboBox **,
                     QString > it(_comboBoxesToANDContentForSearch);

        while (it.hasNext())
        {
            it.next();

            aYerithComboBox = *it.key();

            correspondingDBFieldKeyValue = it.value();

            if (0 != aYerithComboBox)
            {
                aTableColumnFieldContentForANDSearch =
                                aYerithComboBox->currentText();

                if (!correspondingDBFieldKeyValue.isEmpty() &&
                        !aTableColumnFieldContentForANDSearch.isEmpty())
                {
                    if (!_searchFilter.isEmpty())
                    {
                        _searchFilter.append(" AND ");
                    }

                    if (aYerithComboBox == comboBox_alertes_resolue_oui_OU_non)
                    {
                        curSearchDBStr =
                                        QString::number
                                        (YerithUtils::_alerte_OUI_NON_ToUserViewString.key
                                         (comboBox_alertes_resolue_oui_OU_non->currentText()));
                    }

                    _searchFilter.append(GENERATE_SQL_IS_STMT
                                         (correspondingDBFieldKeyValue,
                                          curSearchDBStr));
                }
            }
        }
    }

    YERITH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER
    QString finalSearchFilter(_searchFilter);

//    QDEBUG_STRING_OUTPUT_2("finalSearchFilter", finalSearchFilter);

    _yerithSqlTableModel->yerithSetFilter_WITH_where_clause(finalSearchFilter);

    if (_yerithSqlTableModel->select())
    {
        lister_les_elements_du_tableau(*_yerithSqlTableModel);
    }
    else
    {
        disableImprimer();

        qDebug() <<
                 QString
                 ("++ YerithAlertesWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerithSqlTableModel->lastError().text());
    }
}


void YerithAlertesWindow::handleCurrentTabChanged(int index)
{
    switch (index)
    {
    case TableauDesAlertes:
        handle_visibilite_autres_elements(true);
        afficher_alertes();
        clear_all_fields();
        enableImprimer();
        break;

    case AfficherAlerteAuDetail:
        handle_visibilite_autres_elements(false);
        if (afficher_au_detail())
        {
            disableImprimer();
        }
        break;

    default:
        break;
    }
}


void YerithAlertesWindow::
        handleComboBoxClients_courriers_alertes_resolue_oui_OU_non(const QString &currentText)
{

}


void YerithAlertesWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerithDatabaseTableColumn::DESIGNATION_ALERTE
            << YerithDatabaseTableColumn::DESIGNATION
            << YerithDatabaseTableColumn::CONDITION_ALERTE
            << YerithDatabaseTableColumn::QUANTITE
            << YerithDatabaseTableColumn::DATE_DEBUT
            << YerithDatabaseTableColumn::DATE_FIN
            << YerithDatabaseTableColumn::ALERTE_RESOLUE;
}


void YerithAlertesWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tableView_alertes->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_alertesWindow->palette());
        menu.addAction(actionAfficher_alerte_au_detail);
        menu.exec(event->globalPos());
    }
}


void YerithAlertesWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	CHANGER_DE_LANGUE__text__TRANSLATION(this,
                                         action_TRADUIRE_EN_ANGLAIS);

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    populateComboBoxes();

    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enable_NOM_UTILISATEUR_ONLY_MANAGER();
        }
        else
        {
            setCurrentUser_NOM_UTILISATEUR(aUser->nom_utilisateur());
        }
    }
    else
    {
        disable_NOM_UTILISATEUR();
    }

    setYerithSqlTableModel(_curAlertesSqlTableModel);

    _curStocksTableModel = stocksTableModel;

    setVisible(true);

    afficher_alertes();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("fiche des alertes"),
    								 _curAlertesSqlTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    lineEdit_alertes_terme_recherche->setFocus();
}


void YerithAlertesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->disable(this);
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerithAlertesWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curAlertesSqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));
}


void YerithAlertesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);


#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerithAlertesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YERITH_SERVER
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerithAlertesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarquerResolue, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_afficher_detail->enable(this, SLOT(afficher_au_detail()));
    pushButton_marquer_resolue->enable(this, SLOT(marquer_resolue()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(afficher_alertes()));
    pushButton_supprimer->disable(this);
}


void YerithAlertesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_afficher_detail->disable(this);
    pushButton_marquer_resolue->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
}


bool YerithAlertesWindow::afficher_au_detail()
{
    YerithSqlTableModel &courrierAlertesSqlTableModel =
                    _allWindows->getSqlTableModel_courriers_alertes();

    if (_allWindows->getUser()->isMagasinier()
            || _allWindows->getUser()->isCaissier())
    {
        QString
        userFilter(GENERATE_SQL_IS_STMT
                   (YerithDatabaseTableColumn::DESTINATAIRE,
                    _allWindows->getUser()->nom_utilisateur()));
        //qDebug() << "++ userFilter: " << userFilter;
        courrierAlertesSqlTableModel.yerithSetFilter_WITH_where_clause
        (userFilter);
    }

    if (!courrierAlertesSqlTableModel.select())
    {
        return false;
    }

    QSqlRecord record;

    _allWindows->
    _listerAlertesWindow->SQL_QUERY_YERITH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    lineEdit_alertes_nom->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerithDatabaseTableColumn::DESIGNATION_ALERTE));

    lineEdit_alertes_designation->setText(GET_SQL_RECORD_DATA
                                          (record,
                                           YerithDatabaseTableColumn::DESIGNATION));

    comboBox_alertes_condition->addItem(GET_SQL_RECORD_DATA
                                        (record,
                                         YerithDatabaseTableColumn::CONDITION_ALERTE));

    double quantite = GET_SQL_RECORD_DATA(record,
                                          YerithDatabaseTableColumn::
                                          QUANTITE).toDouble();

    if (quantite > -1)
    {
        lineEdit_alertes_quantite->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::QUANTITE));
        radioButton_alertes_quantite->setChecked(true);
        radioButton_alertes_periode_temps->setChecked(false);

        groupBox_alertes_quantite->setVisible(true);

        groupBox_periode_temps->setVisible(false);
    }
    else
    {
        QString
        date_debut(GET_SQL_RECORD_DATA
                   (record, YerithDatabaseTableColumn::DATE_DEBUT));

        QString
        date_fin(GET_SQL_RECORD_DATA
                 (record, YerithDatabaseTableColumn::DATE_FIN));

        dateEdit_alertes_date_debut->setDate(GET_DATE_FROM_STRING
                                             (date_debut));

        dateEdit_alertes_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));

        radioButton_alertes_quantite->setChecked(false);

        radioButton_alertes_periode_temps->setChecked(true);

        groupBox_alertes_quantite->setVisible(false);

        groupBox_periode_temps->setVisible(true);
    }

    lineEdit_alertes_destinataire->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerithDatabaseTableColumn::DESTINATAIRE));
    lineEdit_alertes_nom_destinataire->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerithDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));
    textEdit_alerte_message->setText(GET_SQL_RECORD_DATA
                                     (record,
                                      YerithDatabaseTableColumn::MESSAGE_ALERTE));

    int alerte_resolue = GET_SQL_RECORD_DATA(record,
                                             YerithDatabaseTableColumn::ALERTE_RESOLUE).
                         toInt();

    lineEdit_alertes_resolue->setText(INT_TO_STRING(alerte_resolue));

    tabWidget_alertes->setCurrentIndex(AfficherAlerteAuDetail);

    return true;
}


void YerithAlertesWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_nom_element_string_db->clear();

    YERITH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_courriers_alertes());

    resetLineEditsQCompleters((QObject *) this);

    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enable_NOM_UTILISATEUR_ONLY_MANAGER();
        }
        else
        {
            setCurrentUser_NOM_UTILISATEUR(aUser->nom_utilisateur());
        }
    }
    else
    {
        disable_NOM_UTILISATEUR();
    }

    afficher_alertes();

    lineEdit_alertes_terme_recherche->setFocus();
}


void YerithAlertesWindow::CHANGER_DE_LANGUE()
{
    YerithWindowsCommons::CHANGER_DE_LANGUE(this,
                                            action_TRADUIRE_EN_ANGLAIS);

    rendreVisible(_curStocksTableModel);
}


bool YerithAlertesWindow::export_csv_file()
{
    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_alertes,
                                               tableColumnsToIgnore,
                                               "yerith-erp-stock-alert-listing-csv-format",
                                               "stock alert listing");
    }
    else
    {
        success = YerithUtils::export_csv_file(*this,
                                               *tableView_alertes,
                                               tableColumnsToIgnore,
                                               "yerith-erp-fichier-alertes-de-stocks-format-csv",
                                               "fichier des alertes");
    }

    return success;
}


QString YerithAlertesWindow::get_latex_template_print_pdf_content()
{
    if (YerithMainWindow::LANGUE_ANGLAISE)
    {
        _latex_template_print_pdf_content = YerithUtils::EN_template_lister_alertes_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerithUtils::FR_template_lister_alertes_tex;
    }

    return _latex_template_print_pdf_content;
}


void YerithAlertesWindow::resetFilter(YerithSqlTableModel *alertesSqlTableModel)
{
    _curAlertesSqlTableModel = alertesSqlTableModel;

    if (0 != _curAlertesSqlTableModel)
    {
        _curAlertesSqlTableModel->resetFilter("src/windows/yerith-erp-alertes-window.cpp");
    }

    lineEdit_alertes_terme_recherche->myClear();

    YerithPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enable_NOM_UTILISATEUR_ONLY_MANAGER();
        }
        else
        {
            setCurrentUser_NOM_UTILISATEUR(aUser->nom_utilisateur());
        }
    }
    else
    {
        disable_NOM_UTILISATEUR();
    }
}


void YerithAlertesWindow::lister_les_elements_du_tableau(YerithSqlTableModel &alertesSqlTableModel)
{
    tableView_alertes->queryYerithTableViewCurrentPageContentRow(alertesSqlTableModel);

    tableView_show_or_hide_columns(*tableView_alertes);

    int rowCount = tableView_alertes->rowCount();

    lineEdit_alertes_nombre_dalertes->setText(GET_NUM_STRING(rowCount));

    if (rowCount <= 0)
    {
        tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, false);
    }
    else
    {
        tabWidget_alertes->setTabEnabled(AfficherAlerteAuDetail, true);
    }
}


void YerithAlertesWindow::afficher_alerte_selectioner(const QString &
                                                      alerteDesignation)
{
    _logger->log("afficher_alerte_selectioner(const QString &)");

    //qDebug() << QString("afficher_alerte_selectioner(%1)").arg(alerteDesignation);

    QString
    filter(GENERATE_SQL_IS_STMT
           (YerithDatabaseTableColumn::DESIGNATION_ALERTE,
            alerteDesignation));

    //qDebug() << QString("filter: %1").arg(filter);

    _curAlertesSqlTableModel->yerithSetFilter_WITH_where_clause(filter);

    if (_curAlertesSqlTableModel->easySelect("src/windows/yerith-erp-alertes-windows.cpp", 1310) > 0)
    {
        lister_les_elements_du_tableau(*_curAlertesSqlTableModel);
    }
}


void YerithAlertesWindow::handle_visibilite_autres_elements(bool aBoolValue)
{
    if (aBoolValue)
    {
        tabWidget_alertes->setFixedHeight(331);
    }
    else
    {
        tabWidget_alertes->setFixedHeight(392);
    }


    label_alertes_resolue_oui_OU_non->setVisible(aBoolValue);

    label_nombre_de_lignes_par_page->setVisible(aBoolValue);

    label_alertes_numero_page_courante->setVisible(aBoolValue);
    label_alertes_numero_page_derniere->setVisible(aBoolValue);
    label_alertes_separation_page_precedente_ou_suivante->setVisible
    (aBoolValue);

    label_nombre_dalertes->setVisible(aBoolValue);

    comboBox_element_string_db->setVisible(aBoolValue);
    comboBox_alertes_resolue_oui_OU_non->setVisible(aBoolValue);

    pushButton_page_premiere->setVisible(aBoolValue);
    pushButton_page_derniere->setVisible(aBoolValue);
    pushButton_page_precedente->setVisible(aBoolValue);
    pushButton_page_suivante->setVisible(aBoolValue);

    pushButton_reinitialiser->setVisible(aBoolValue);

    lineEdit_alertes_destinataire_2->setVisible(aBoolValue);

    lineEdit_nom_element_string_db->setVisible(aBoolValue);

    lineEdit_alertes_nombre_de_lignes_par_page->setVisible(aBoolValue);
    lineEdit_alertes_nombre_dalertes->setVisible(aBoolValue);
}
