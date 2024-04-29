/*
 * yerith-erp-database.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-database.hpp"

#include "src/utils/yerith-erp-utils.hpp"



const QString YerithDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR("periodes_dappartenance_groupes_de_paie_hr");

const QString YerithDatabase::IMPRIMANTERESEAU_RECUS_PETITS("imprimantereseau_recus_petits");

const
QString YerithDatabase::COMPTES_BANCAIRES("comptes_bancaires");

const QString YerithDatabase::OPERATIONS_COMPTABLES("operations_comptables");

const QString YerithDatabase::YRI_DB_RUNTIME_VERIF("yri_db_runtime_verif");

const QString YerithDatabase::ENTREPRISE_INFO("entreprise_info");

const QString YerithDatabase::PAIEMENTS("paiements");

const QString YerithDatabase::USERS("users");

const QString YerithDatabase::TITRES("titres");

const QString YerithDatabase::LOCALISATIONS("localisations");

const QString YerithDatabase::DEPARTEMENTS_PRODUITS("departements_produits");

const QString YerithDatabase::CATEGORIES("categories");

const QString YerithDatabase::LIGNES_BUDGETAIRES("LIGNES_BUDGETAIRES");

const QString YerithDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS("programmes_de_fidelite_clients");

const QString YerithDatabase::GROUPES_DEMPLOYES_hr("groupes_demployes_hr");

const QString YerithDatabase::GROUPES_DE_PAIE_hr("groupes_de_paie_hr");

const QString YerithDatabase::CLIENTS("clients");

const QString YerithDatabase::GROUPES_DE_CLIENTS("groupes_de_clients");

const QString YerithDatabase::FOURNISSEURS("fournisseurs");

const QString YerithDatabase::ALERTES("alertes");

const QString YerithDatabase::REMISES("remises");

const QString YerithDatabase::CREDIT_CARD_ISSUING_COMPANY("credit_card_issuing_company");

const QString YerithDatabase::CONDITIONS_ALERTES("conditions_alertes");

const QString YerithDatabase::COURRIERS_ALERTES("courriers_alertes");

const QString YerithDatabase::CHARGES_FINANCIERES("charges_financieres");

const QString YerithDatabase::ACHATS("achats");

const QString YerithDatabase::STOCKS("stocks");

const QString YerithDatabase::SERVICES_COMPLETES("services_completes");

const QString YerithDatabase::STOCKS_VENDU("stocks_vendu");

const QString YerithDatabase::STOCKS_SORTIES("stocks_sorties");

const QString YerithDatabase::MARCHANDISES("marchandises");

const QString YerithDatabase::CONFIGURATIONS("configurations");

const QString YerithDatabase::INIT_CONFIGURATIONS("init_configurations");

const QString YerithDatabase::TYPE_DOPERATIONS_FINANCIERES("type_doperations_financieres");

const QString YerithDatabase::TYPE_DE_VENTE("type_de_vente");

const QString YerithDatabase::TYPE_DE_PAIEMENT("type_de_paiement");

const QString YerithDatabase::ROLES("roles");

const QString YerithDatabase::DBMS("dbms");


const QString YerithDatabase::MYSQL("MySQL");

const QString YerithDatabase::QSQLITE("QSQLITE");

QString YerithDatabase::_db_type("");

QString YerithDatabase::_db_name("");

QString YerithDatabase::_db_ip_address("");

QString YerithDatabase::_db_user_name("");

QString YerithDatabase::_db_user_pwd("");

QString YerithDatabase::_db_connection_options("");


YerithDatabase::YerithDatabase()
{
    set_db_name(YerithERPConfig::_db_name);
    set_db_ip_address(YerithERPConfig::_db_ip_address);
    set_db_user_name(YerithERPConfig::_db_user_name);
    set_db_user_pwd(YerithERPConfig::_db_user_pwd);
    set_db_type(YerithERPConfig::_db_type);
    set_db_connection_options(YerithERPConfig::_db_connection_options);

    if (YerithUtils::isEqualCaseInsensitive(YerithDatabase::MYSQL,
                                            YerithERPConfig::_db_type))
    {
        //logger << "++ main(): QMYSQL" << "\n";
        _database = QSqlDatabase::addDatabase("QMYSQL");
    }
    else if (YerithUtils::isEqualCaseInsensitive(YerithDatabase::QSQLITE,
                                                 YerithERPConfig::_db_type))
    {
        //logger << "++ main(): QSQLITE" << "\n";
        _database = QSqlDatabase::addDatabase("QSQLITE");
    }

    _database.setDatabaseName(_db_name);
    _database.setHostName(_db_ip_address);
    _database.setUserName(_db_user_name);
    _database.setPassword(_db_user_pwd);
    _database.setConnectOptions(_db_connection_options);
}


void YerithDatabase::set_db_name(const QString &db_name)
{
    _db_name = db_name;
    _database.setDatabaseName(_db_name);
}


void YerithDatabase::set_db_ip_address(const QString &db_ip_address)
{
    _db_ip_address = db_ip_address;
    _database.setHostName(_db_ip_address);
}


void YerithDatabase::set_db_user_name(const QString &db_user_name)
{
    _db_user_name = db_user_name;
    _database.setUserName(_db_user_name);
}


void YerithDatabase::set_db_user_pwd(const QString &db_user_pwd)
{
    _db_user_pwd = db_user_pwd;
    _database.setPassword(_db_user_pwd);
}


void YerithDatabase::set_db_connection_options(const QString &
                                               db_connection_options)
{
    _db_connection_options = db_connection_options;
    _database.setConnectOptions(_db_connection_options);

}


QString YerithDatabase::toString() const
{
    QString ret;

    if (_database.isValid())
    {
        ret.append(QString("db_type: %1\n"
                           "db_name: %2\n"
                           "db_ip_address: %3\n"
                           "db_connection_options: %4").arg(_db_type,
                                                            _database.
                                                            databaseName(),
                                                            _database.
                                                            hostName(),
                                                            _database.
                                                            connectOptions
                                                            ()));
    }
    else
    {
        ret.append(QString("db_type: %1\n"
                           "db_name: %2\n"
                           "db_ip_address: %3\n"
                           "db_connection_options: %4").arg(_db_type,
                                                            YerithDatabase::
                                                            _db_name,
                                                            YerithDatabase::
                                                            _db_ip_address,
                                                            YerithDatabase::
                                                            _db_connection_options));
    }

    return ret;
}

