/*
 * yerith-erp-creer-un-programme-de-fidelite-clients-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-creer-un-programme-de-fidelite-clients-window.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/users/yerith-erp-users.hpp"

#include "src/utils/yerith-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yerith-erp-database-table-column.hpp"

#include "src/utils/yerith-erp-historique-stock.hpp"


YerithCreerUnProgrammeDeFideliteClientsWindow::YerithCreerUnProgrammeDeFideliteClientsWindow()
:YerithWindowsCommons(),
 _logger(new YerithLogger("YerithCreerUnProgrammeDeFideliteClientsWindow"))
{
	_windowName = YerithMainWindow::get_TRANSLATED_WindowName("créer un programme de fidélité clients");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YERITH_BLACK_0_0_0);


//    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
//              << ;


    setup_select_configure_dbcolumn(YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS);

    doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->setMaximum(100.0);

    populateComboBoxes();


    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);


    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_clients->disable(this);
    pushButton_enregistrer->disable(this);


    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this,
            SLOT(annuler_la_creation_dun_programme_de_fidelite_de_clients
                 ()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerUnProgrammeDeFideliteClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));



#ifdef YERITH_CLIENT
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerithWindowsCommons::deconnecter_utilisateur();
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::updatePopulateComboBoxes()
{
    QStringList new_items;

    new_items.append(YerithUtils::EMPTY_STRING);

    new_items.append(YerithERPConfig::THIS_SITE_LOCALISATION_NAME);


    comboBox_creer_un_programme_de_fidelite_clients_localisation->populateComboBoxRawString
    (YerithDatabase::LOCALISATIONS,
     YerithDatabaseTableColumn::NOM_LOCALISATION);


    comboBox_creer_un_programme_de_fidelite_clients_localisation->addItem_AFTER_POPULATE
    (new_items);
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);

    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_clients->disable(this);
    pushButton_enregistrer->disable(this);

}


void YerithCreerUnProgrammeDeFideliteClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);
    YERITH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, true);


    pushButton_annuler->enable(this,
                               SLOT
                               (annuler_la_creation_dun_programme_de_fidelite_de_clients
                                ()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_enregistrer->enable(this,
                                   SLOT
                                   (creerEnregistrerUnProgrammeDeFideliteClients
                                    ()));
}


bool
YerithCreerUnProgrammeDeFideliteClientsWindow::
royaltyClientProgramAlreadyExists()
{
    bool
    result = false;

    YerithSqlTableModel &programmes_de_fidelite_clients_TableModel =
                    _allWindows->getSqlTableModel_programmes_de_fidelite_clients();

    // ** check if customer account with same name exist
    result =
                    YerithUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                                programmes_de_fidelite_clients_TableModel,
                                                                *lineEdit_creer_un_programme_de_fidelite_clients_designation,
                                                                QObject::tr
                                                                ("programme de fidélité clients"),
                                                                YerithDatabaseTableColumn::
                                                                DESIGNATION);

    return result;
}


bool YerithCreerUnProgrammeDeFideliteClientsWindow::check_fields()
{
    bool result = false;

    bool programme_de_fidelite_clients_designation =
                    lineEdit_creer_un_programme_de_fidelite_clients_designation->checkField
                    ();

    result = programme_de_fidelite_clients_designation;

    return result;
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::clear_all_fields()
{
    lineEdit_creer_un_programme_de_fidelite_clients_reference->clear();

    lineEdit_creer_un_programme_de_fidelite_clients_designation->clearField();

    textEdit_creer_un_programme_de_fidelite_clients_description->clear();

    doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->
    setValue(0.0);

    comboBox_creer_un_programme_de_fidelite_clients_localisation->
    resetYerithComboBox();
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::rendreVisible(YerithSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    updatePopulateComboBoxes();

    check_fields();

    lineEdit_creer_un_programme_de_fidelite_clients_designation->setFocus();

    YERITH_set_windowName_TRANSLATED(YerithMainWindow::get_TRANSLATED_WindowName("créer un programme de fidélité clients"),
      								 _curStocksTableModel);

    setVisible(true);
}


void YerithCreerUnProgrammeDeFideliteClientsWindow::annuler_la_creation_dun_programme_de_fidelite_de_clients()
{
    clear_all_fields();

    programmes_de_fidelite_clients();
}


bool YerithCreerUnProgrammeDeFideliteClientsWindow::creerEnregistrerUnProgrammeDeFideliteClients()
{
    if (!check_fields())
    {
        if (QMessageBox::Ok ==
                YerithQMessageBox::warning(this, "",
                                           QObject::tr
                                           ("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }

    if (royaltyClientProgramAlreadyExists())
    {
        return false;
    }

    QString
    retMsg(QObject::tr("Le programme de fidélité de clients '"));

    YerithSqlTableModel &clientRoyaltyProgramTableModel =
                    _allWindows->getSqlTableModel_programmes_de_fidelite_clients();


    QSqlRecord
    record = clientRoyaltyProgramTableModel.record();


    QString
    new_royalty_program_designation =
                    lineEdit_creer_un_programme_de_fidelite_clients_designation->text();

    int
    new_client_LOYALTY_PROGRAM_db_ID =
                    YerithERPWindows::getNextIdSqlTableModel_programme_de_fidelite_clients();


    record.setValue(YerithDatabaseTableColumn::ID,
                    new_client_LOYALTY_PROGRAM_db_ID);

    record.setValue(YerithDatabaseTableColumn::DATE_CREATION,
                    DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

    record.setValue(YerithDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS,
    				lineEdit_creer_un_programme_de_fidelite_clients_reference->text());

    record.setValue(YerithDatabaseTableColumn::DESIGNATION,
                    new_royalty_program_designation);

    if (!comboBox_creer_un_programme_de_fidelite_clients_localisation->isEmpty
            ())
    {
        record.setValue(YerithDatabaseTableColumn::LOCALISATION,
                        comboBox_creer_un_programme_de_fidelite_clients_localisation->currentText());
    }

    record.setValue(YerithDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS,
    				textEdit_creer_un_programme_de_fidelite_clients_description->toPlainText());

    record.setValue(YerithDatabaseTableColumn::POURCENTAGE_DU_RABAIS,
                    doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->value());


    retMsg.append(lineEdit_creer_un_programme_de_fidelite_clients_designation->text());

    bool success = clientRoyaltyProgramTableModel
    					.insertNewRecord(record,
    									 this,
										 "src/windows/crm/creer/yerith-erp-creer-un-programme-de-fidelite-clients-window.cpp",
										 358,
										 new_royalty_program_designation);

    if (!success)
    {
        retMsg.append(QObject::tr("' n'a pas pu être créer !"));

        YerithQMessageBox::warning(this,
                                   QObject::tr("créer 1 programme de fidélité clients"),
                                   retMsg);
        return false;
    }

    clientRoyaltyProgramTableModel.select();

    retMsg.append(QObject::tr("' a été créer avec succès !"));

    YerithQMessageBox::information(this,
                                   QObject::tr
                                   ("créer 1 programme de fidélité clients"),
                                   retMsg);

    clear_all_fields();

    programmes_de_fidelite_clients();

    return true;
}
