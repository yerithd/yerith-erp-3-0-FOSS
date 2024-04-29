/*
 * yerith-erp-calcul-salaire-EMPLOYE-window.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-calcul-salaire-EMPLOYE-window.hpp"


#include "src/yerith-erp-windows.hpp"


#include "src/utils/yerith-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>



YerithERPCalculSalaireEMPLOYE::
    YerithERPCalculSalaireEMPLOYE(YerithWindowsCommons *calling_window)
:_sommeTotal(0.0),
 _calling_window(calling_window)
{
    __QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YERITH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YERITH_WHITE_255_255_255);
}


bool YerithERPCalculSalaireEMPLOYE::
        INSERT_employee_Paiement_AS_PAYMENT_ENTRY_DB_TABLE
            (YerithERPPaymentProcessingInformation &Payment_processing_info_In)
{
    bool successPaiementsInsert = false;


    bool is_supplier_payment = true;

    successPaiementsInsert =
        Payment_processing_info_In
            .save_Human_RESOURCE_payment_info_record(is_supplier_payment,
                                                     Payment_processing_info_In._nom_entreprise);


    return successPaiementsInsert;
}


double YerithERPCalculSalaireEMPLOYE::
            calculate_PAY_GROUP_MONEY_BENEFITS(QString  &a_nom_entreprise_EMPLOYEE,
                                               QString  &PAY_GROUP,
                                               YerithERPPaymentProcessingInformation *payment_processing_info_IN_Out /* = 0 */)
{
    double result = 0.0;


    YerithERPWindows *ALL_WINDOWS = YerithUtils::getAllWindows();


    YerithSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_SqlTableModel =
        ALL_WINDOWS->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    QString condition_exist_already =
        QString("(%1) AND (%2)")
            .arg(GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                                      a_nom_entreprise_EMPLOYEE),
                 GENERATE_SQL_IS_STMT(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                      PAY_GROUP));


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .yerithSetFilter_WITH_where_clause(condition_exist_already);


    int query_size = periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
                        .easySelect("src/windows/supplier/yerith-erp-fournisseur-details-window.cpp", 56);


    if (query_size <= 0)
    {
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
            .resetFilter("src/windows/supplier/yerith-erp-fournisseur-details-window.cpp", 62);

        return 0.0;
    }


    QDate begin_DATE, end_DATE;


    QSqlRecord periodes_dappartenance_RECORD =
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.record(0);


    begin_DATE = periodes_dappartenance_RECORD
                    .value(YerithDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE)
                            .toDate();

    end_DATE = periodes_dappartenance_RECORD
                    .value(YerithDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE)
                            .toDate();


    QDate current_date = GET_CURRENT_DATE;


    bool AUTHORIZED_PAYMENT_DATE = (begin_DATE <= current_date &&
                                    current_date <= end_DATE);


    if (!AUTHORIZED_PAYMENT_DATE)
    {
        if (0 != _calling_window)
        {
            YerithQMessageBox::information(_calling_window,
                                           QObject::tr("calcul de salaire d'1 employé"),
                                           QObject::tr("LE GROUPE DE PAIEMENT [\"%1\"] "
                                                       "N'A PLUS DE dates de validités courantes "
                                                       "pour l'employé [%2] !")
                                            .arg(PAY_GROUP,
                                                 a_nom_entreprise_EMPLOYEE));
        }

        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
            .resetFilter("src/windows/supplier/yerith-erp-fournisseur-details-window.cpp", 105);

        return 0.0;
    }


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .resetFilter("src/windows/supplier/yerith-erp-fournisseur-details-window.cpp", 112);



    QString SELECT_GROUPE_DE_PAIE =
                QString("select * from %1 where %2='%3'")
                    .arg(YerithDatabase::GROUPES_DE_PAIE_hr,
                         YerithDatabaseTableColumn::DESIGNATION,
                         PAY_GROUP);


    QSqlQuery a_qsql_query;

    int query_size_3 = YerithUtils::execQuery(a_qsql_query,
                                              SELECT_GROUPE_DE_PAIE);

    if (query_size_3 <= 0)
    {
        return 0.0;
    }

    a_qsql_query.next();

    QSqlRecord aQSqlRecord = a_qsql_query.record();

    double AMOUNT_TO_BE_PAID_to_employee__NO__MONEY_BENEFITS =
                GET_SQL_RECORD_DATA(aQSqlRecord,
                                    YerithDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL)
                                        .toDouble();

    result = AMOUNT_TO_BE_PAID_to_employee__NO__MONEY_BENEFITS;


    //QDEBUG_STRING_OUTPUT_2_N("original amount to be paid TO EMPLOYEE", _sommeTotal);


    double pourcentage_TAXES_IMPOSABLES =
                GET_SQL_RECORD_DATA(aQSqlRecord,
                                    YerithDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES).toDouble();


    double TVA_money =
                    (pourcentage_TAXES_IMPOSABLES *
                     AMOUNT_TO_BE_PAID_to_employee__NO__MONEY_BENEFITS) / 100.0;

    result = result - TVA_money;


    if (0 != payment_processing_info_IN_Out)
    {

    }


    return result;
}


double YerithERPCalculSalaireEMPLOYE::
            CALCULATE_EMPLOYEE_SALARY(const QString &a_nom_entreprise_EMPLOYEE)
{
    YerithERPPaymentProcessingInformation DUMMY_payment_processing_info_IN_Out;

    return
        CALCULATE_EMPLOYEE_SALARY(DUMMY_payment_processing_info_IN_Out,
                                  a_nom_entreprise_EMPLOYEE);
}


double YerithERPCalculSalaireEMPLOYE::
        CALCULATE_EMPLOYEE_SALARY(YerithERPPaymentProcessingInformation &payment_processing_info_IN_Out,
                                  const QString                         &a_nom_entreprise_EMPLOYEE)
{
    //QDEBUG_STRING_OUTPUT_2("a_nom_entreprise_EMPLOYEE",
    //                        a_nom_entreprise_EMPLOYEE);

    if (a_nom_entreprise_EMPLOYEE.isEmpty())
    {
        return 0.0;
    }


    /*
     * I search all client group of customer 'a_nom_entreprise_client'.
     */
    QString SELECT_EMPLOYEE_GROUP
                (QString("select %1 from %2 where %3='%4' AND %5='1'")
                    .arg(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                         YerithDatabase::FOURNISSEURS,
                         YerithDatabaseTableColumn::NOM_ENTREPRISE,
                         a_nom_entreprise_EMPLOYEE,
                         YerithDatabaseTableColumn::EMPLOYE));

    //QDEBUG_STRING_OUTPUT_2("SELECT_EMPLOYEE_GROUP",
    //                        SELECT_EMPLOYEE_GROUP);

    QSqlQuery a_qsql_query;

    int query_size = YerithUtils::execQuery(a_qsql_query,
                                            SELECT_EMPLOYEE_GROUP);

    if (query_size <= 0)
    {
        return 0.0;
    }


    QString employee_group_id;

    a_qsql_query.next();

    employee_group_id = a_qsql_query
                        .value(YerithDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID).toString();

    //QDEBUG_STRING_OUTPUT_2("employee_group_id", employee_group_id);

    if (employee_group_id.isEmpty())
    {
        return 0.0;
    }


    QStringList EMPLOYEE_GroupId_list;

    YerithUtils::SPLIT_STAR_SEPARATED_DB_STRING(EMPLOYEE_GroupId_list,
                                                employee_group_id);


//      QDEBUG_QSTRINGLIST_OUTPUT("EMPLOYEE_GroupId_list", EMPLOYEE_GroupId_list);


//      QDEBUG_STRING_OUTPUT_2("YerithERPConfig::THIS_SITE_LOCALISATION_NAME",
//                                                 YerithERPConfig::THIS_SITE_LOCALISATION_NAME);


    QString payment_processing_info_PAY_GROUPS;


    QString employeeGroup_db_ID;

    QString EMPLOYEE_group_PAY_GR0UP_LigneBUDGETAIRE;

    QString EMPLOYEE_group_PAY_GR0UP_LOCALISATION;

    QString EMPLOYEE_group_PAY_GR0UP;

    QString CUR_SELECT_employee_PAY_GROUP;

    double cur_EMPLOYEE_group_PAY_GR0UP_money_BENEFITS = 0.0;


    for (uint k = 0; k < EMPLOYEE_GroupId_list.size(); ++k)
    {
        employeeGroup_db_ID = EMPLOYEE_GroupId_list.at(k);

        if (employeeGroup_db_ID.isEmpty())
        {
            continue;
        }

        CUR_SELECT_employee_PAY_GROUP =
                        QString("select %1 from %2 where %3='%4'")
                            .arg(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                 YerithDatabase::GROUPES_DEMPLOYES_hr,
                                 YerithDatabaseTableColumn::ID,
                                 employeeGroup_db_ID);

        //QDEBUG_STRING_OUTPUT_2("CUR_SELECT_employee_PAY_GROUP",
        //                        CUR_SELECT_employee_PAY_GROUP);

        a_qsql_query.clear();

        query_size = YerithUtils::execQuery(a_qsql_query,
                                            CUR_SELECT_employee_PAY_GROUP);


        for (int j = 0;
             j < query_size && a_qsql_query.next();
             ++j)
        {
            EMPLOYEE_group_PAY_GR0UP =
                a_qsql_query.value(YerithDatabaseTableColumn::GROUPE_DE_PAIE_HR).toString();

            if (EMPLOYEE_group_PAY_GR0UP.isEmpty())
            {
                continue;
            }


            EMPLOYEE_group_PAY_GR0UP_LOCALISATION =
                            YerithUtils::GET_LOCALISATION_FOR_EMPLOYEE_PAY_GROUP
                                (EMPLOYEE_group_PAY_GR0UP);

            //QDEBUG_STRING_OUTPUT_2("EMPLOYEE_group_PAY_GR0UP_LOCALISATION",
            //                        EMPLOYEE_group_PAY_GR0UP_LOCALISATION);

            if (!EMPLOYEE_group_PAY_GR0UP_LOCALISATION.isEmpty() &&
                !YerithUtils::isEqualCaseInsensitive(YerithERPConfig::THIS_SITE_LOCALISATION_NAME,
                                                     EMPLOYEE_group_PAY_GR0UP_LOCALISATION))
            {
                continue;
            }

            //QDEBUG_STRING_OUTPUT_1(QString("employeeGroup_db_ID (%1), EMPLOYEE_group_PAY_GR0UP => %2")
            //                         .arg(employeeGroup_db_ID,
            //                              EMPLOYEE_group_PAY_GR0UP));



            EMPLOYEE_group_PAY_GR0UP_LigneBUDGETAIRE =
                YerithUtils::Get_INTITULE_DE_LA_LIGNE_BUDGETAIRE_FOR_GROUPE_DE_PAIE_Hr
                    (EMPLOYEE_group_PAY_GR0UP);


            payment_processing_info_PAY_GROUPS
                .append(QString("%1;")
                         .arg(EMPLOYEE_group_PAY_GR0UP));


            payment_processing_info_IN_Out._paiement_INTITULE_DE_LA_LIGNE_BUDGETAIRE =
                EMPLOYEE_group_PAY_GR0UP_LigneBUDGETAIRE;


            payment_processing_info_IN_Out._paiement_intitule_compte_bancaire =
                YerithUtils::Get_INTITULE_COMPTE_BANCAIRE_FOR_intitule_de_la_ligne_budgetaire
                                (EMPLOYEE_group_PAY_GR0UP_LigneBUDGETAIRE);


            cur_EMPLOYEE_group_PAY_GR0UP_money_BENEFITS =
                calculate_PAY_GROUP_MONEY_BENEFITS(const_cast<QString &>(a_nom_entreprise_EMPLOYEE),
                                                   EMPLOYEE_group_PAY_GR0UP);


            _sommeTotal += cur_EMPLOYEE_group_PAY_GR0UP_money_BENEFITS;
        }
    }


    payment_processing_info_IN_Out._nom_entreprise = a_nom_entreprise_EMPLOYEE;


    payment_processing_info_IN_Out._reference =
        QString("%1-SALAIRE-%2")
            .arg(DATE_TO_STRING(GET_CURRENT_DATE),
                 payment_processing_info_IN_Out._nom_entreprise);


    payment_processing_info_IN_Out._designation =
        payment_processing_info_IN_Out._reference;


    payment_processing_info_IN_Out._type_de_paiement = YerithUtils::DECAISSEMENT_COMPTANT;


    payment_processing_info_IN_Out._NOTES =
        QObject::tr("PAYEMENT DU SALAIRE pour l'employé(e) '%1' \n\n"
                    "Groupe salariales: %2.")
            .arg(a_nom_entreprise_EMPLOYEE,
                 payment_processing_info_PAY_GROUPS);


    payment_processing_info_IN_Out._montant_paye = _sommeTotal;


    return _sommeTotal;
    //lineEdit_EMPLOYE_SALAIRE_MENSUEL
    //->setText(GET_CURRENCY_STRING_NUM(_sommeTotal));
}


