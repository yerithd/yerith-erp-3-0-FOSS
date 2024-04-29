/*
 * yerith-erp-payment-processing-information.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-payment-processing-information.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/users/yerith-erp-users.hpp"



bool YerithERPPaymentProcessingInformation::update_supplier_account(int                 type_de_paiment,
                                                                    YerithERPWindows   &allWindows)
{
    YerithERPWindows *_allWindows = YerithUtils::getAllWindows();

    if (0 == _allWindows)
    {
        return false;
    }


    bool success_update_compte_fournisseur = false;


    YerithSqlTableModel &fournisseursTableModel =
                    allWindows.getSqlTableModel_fournisseurs();


    QString fournisseursTableFilter =
        QString("%1 = '%2'")
            .arg(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                 _nom_entreprise);


    fournisseursTableModel.yerithSetFilter_WITH_where_clause(fournisseursTableFilter);


    int rows = fournisseursTableModel.easySelect("src/utils/yerith-erp-payment-processing-information.cpp", 42);


    if (rows > 0)
    {
        QSqlRecord fournisseursRecord = fournisseursTableModel.record(0);

        double compte_fournisseur =
            GET_SQL_RECORD_DATA(fournisseursRecord,
                                YerithDatabaseTableColumn::COMPTE_FOURNISSEUR).toDouble();


        _nouveau_compte_fournisseur =
                        compte_fournisseur + montant_paye(type_de_paiment);


        fournisseursRecord
            .setValue(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR,
                      _nouveau_compte_fournisseur);


        success_update_compte_fournisseur =
                        fournisseursTableModel.updateRecord(0,
                        									fournisseursRecord,
															"src/utils/yerith-erp-payment-processing-information.cpp",
															63);

        fournisseursTableModel.resetFilter();
    }


    return success_update_compte_fournisseur;
}


double YerithERPPaymentProcessingInformation::montant_paye(int type_de_paiement)
{
    if (YerithUtils::ENCAISSEMENT_COMPTANT == _type_de_paiement ||
            YerithUtils::ENCAISSEMENT_CHEQUE == _type_de_paiement ||
            YerithUtils::ENCAISSEMENT_TELEPHONE == _type_de_paiement ||
            YerithUtils::ENCAISSEMENT_BANCAIRE == _type_de_paiement ||
            YerithUtils::ENCAISSEMENT_VIREMENT_BANCAIRE == _type_de_paiement ||
            YerithUtils::ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE == _type_de_paiement)
    {
        return (_montant_paye < 0) ? qFabs(_montant_paye) : _montant_paye;
    }

    return (_montant_paye < 0) ? _montant_paye : (-1 * _montant_paye);
}


bool YerithERPPaymentProcessingInformation::
        save_Human_RESOURCE_payment_info_record(bool is_supplier_payment /* = false */,
                                                QString Receiver_OF_money_WHEN_processed /* = YerithUtils::EMPTY_STRING*/)
{
    YerithERPWindows *_allWindows = YerithUtils::getAllWindows();

    if (0 == _allWindows)
    {
        return false;
    }

    bool successPaiementsInsert = true;



    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    YerithSqlTableModel &paiementsSqlTableModel =
            _allWindows->getSqlTableModel_paiements();


    QSqlRecord paiementsRecord = paiementsSqlTableModel.record();


    paiementsRecord.setValue(YerithDatabaseTableColumn::IS_PAYEMENT_HR_HR,
                             true);


    paiementsRecord.setValue(YerithDatabaseTableColumn::DATE_PAIEMENT,
                             GET_CURRENT_DATE);


    paiementsRecord.setValue(YerithDatabaseTableColumn::HEURE_PAIEMENT,
                             CURRENT_TIME);


    paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                             _nom_entreprise);



    QString ENCAISSEUR_DU_payement_NOM_complet;


    if (Receiver_OF_money_WHEN_processed.isEmpty())
    {
        YerithPOSUser *aUser = _allWindows->getUser();

        if (0 != aUser)
        {
            ENCAISSEUR_DU_payement_NOM_complet = aUser->nom_complet();
        }
        else
        {
            ENCAISSEUR_DU_payement_NOM_complet = QObject::tr("inconnu(e)");
        }
    }
    else
    {
        ENCAISSEUR_DU_payement_NOM_complet =
            Receiver_OF_money_WHEN_processed;
    }


    paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENCAISSEUR,
                             ENCAISSEUR_DU_payement_NOM_complet);


    paiementsRecord.setValue(YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
                             _type_de_paiement);

    paiementsRecord.setValue(YerithDatabaseTableColumn::MONTANT_PAYE,
                             montant_paye(_type_de_paiement));


    paiementsRecord
        .setValue(YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
                 _paiement_intitule_compte_bancaire);


    paiementsRecord.setValue(YerithDatabaseTableColumn::NOTES, _NOTES);


    int IDforReceipt = YerithERPWindows::getNextIdSqlTableModel_paiements();


    QString referenceRecuPaiementClient =
                YerithUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString::number(IDforReceipt));


    paiementsRecord
        .setValue(YerithDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT,
                 referenceRecuPaiementClient);


    paiementsRecord.setValue(YerithDatabaseTableColumn::DESIGNATION,
                             _designation);


    paiementsRecord.setValue(YerithDatabaseTableColumn::LOCALISATION,
                             YerithERPConfig::THIS_SITE_LOCALISATION_NAME);


    paiementsRecord.setValue(YerithDatabaseTableColumn::REFERENCE, _reference);


    if (is_supplier_payment)
    {
        successPaiementsInsert = successPaiementsInsert &&
                                 update_supplier_account(_type_de_paiement, *_allWindows);


        paiementsRecord.setValue(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR,
                                 _nouveau_compte_fournisseur);


        if (!_paiement_INTITULE_DE_LA_LIGNE_BUDGETAIRE.isEmpty())
        {
            paiementsRecord.setValue(YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
                                     _paiement_INTITULE_DE_LA_LIGNE_BUDGETAIRE);
        }
    }
    else
    {
        paiementsRecord
            .setValue(YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                      _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS);

        paiementsRecord.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT,
                                 _nouveau_compte_client);
    }


    paiementsRecord.setValue(YerithDatabaseTableColumn::ID, IDforReceipt);


    successPaiementsInsert = successPaiementsInsert &&
                             paiementsSqlTableModel.insertNewRecord(paiementsRecord,
                            		 	 	 	 	 	 	 	 	0,
																	"src/utils/yerith-erp-payment-processing-information.cpp",
																	210,
																	_reference);


    if (successPaiementsInsert)
    {
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
    else
    {
        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;
    }

    return successPaiementsInsert;
}


bool YerithERPPaymentProcessingInformation::
        save_payment_info_record(bool is_supplier_payment /* = false */,
                                 QString Receiver_OF_money_WHEN_processed /* = YerithUtils::EMPTY_STRING*/)
{
    YerithERPWindows *_allWindows = YerithUtils::getAllWindows();

    if (0 == _allWindows)
    {
        return false;
    }

    bool successPaiementsInsert = true;



    YERITH_ERP_3_0_START_DATABASE_TRANSACTION;


    YerithSqlTableModel &paiementsSqlTableModel =
            _allWindows->getSqlTableModel_paiements();


    QSqlRecord paiementsRecord = paiementsSqlTableModel.record();


    paiementsRecord.setValue(YerithDatabaseTableColumn::IS_PAYEMENT_HR_HR,
                             false);


    paiementsRecord.setValue(YerithDatabaseTableColumn::DATE_PAIEMENT,
                             GET_CURRENT_DATE);


    paiementsRecord.setValue(YerithDatabaseTableColumn::HEURE_PAIEMENT,
                             CURRENT_TIME);


    paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENTREPRISE,
                             _nom_entreprise);



    QString ENCAISSEUR_DU_payement_NOM_complet;


    if (Receiver_OF_money_WHEN_processed.isEmpty())
    {
        YerithPOSUser *aUser = _allWindows->getUser();

        if (0 != aUser)
        {
            ENCAISSEUR_DU_payement_NOM_complet = aUser->nom_complet();
        }
        else
        {
            ENCAISSEUR_DU_payement_NOM_complet = QObject::tr("inconnu(e)");
        }
    }
    else
    {
        ENCAISSEUR_DU_payement_NOM_complet =
            Receiver_OF_money_WHEN_processed;
    }


    paiementsRecord.setValue(YerithDatabaseTableColumn::NOM_ENCAISSEUR,
                             ENCAISSEUR_DU_payement_NOM_complet);


    paiementsRecord.setValue(YerithDatabaseTableColumn::TYPE_DE_PAIEMENT,
                             _type_de_paiement);

    paiementsRecord.setValue(YerithDatabaseTableColumn::MONTANT_PAYE,
                             montant_paye(_type_de_paiement));


    paiementsRecord
        .setValue(YerithDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
                 _paiement_intitule_compte_bancaire);


    paiementsRecord.setValue(YerithDatabaseTableColumn::NOTES, _NOTES);


    int IDforReceipt = YerithERPWindows::getNextIdSqlTableModel_paiements();


    QString referenceRecuPaiementClient =
                YerithUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString::number(IDforReceipt));


    paiementsRecord
        .setValue(YerithDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT,
                 referenceRecuPaiementClient);


    paiementsRecord.setValue(YerithDatabaseTableColumn::DESIGNATION,
                             _designation);


    paiementsRecord.setValue(YerithDatabaseTableColumn::LOCALISATION,
                             YerithERPConfig::THIS_SITE_LOCALISATION_NAME);


    paiementsRecord.setValue(YerithDatabaseTableColumn::REFERENCE, _reference);


    if (is_supplier_payment)
    {
        successPaiementsInsert = successPaiementsInsert &&
                                 update_supplier_account(_type_de_paiement, *_allWindows);


        paiementsRecord.setValue(YerithDatabaseTableColumn::COMPTE_FOURNISSEUR,
                                 _nouveau_compte_fournisseur);


        if (!_paiement_INTITULE_DE_LA_LIGNE_BUDGETAIRE.isEmpty())
        {
            paiementsRecord.setValue(YerithDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
                                     _paiement_INTITULE_DE_LA_LIGNE_BUDGETAIRE);
        }
    }
    else
    {
        paiementsRecord
            .setValue(YerithDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
                      _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS);

        paiementsRecord.setValue(YerithDatabaseTableColumn::COMPTE_CLIENT,
                                 _nouveau_compte_client);
    }


    paiementsRecord.setValue(YerithDatabaseTableColumn::ID, IDforReceipt);


    successPaiementsInsert = successPaiementsInsert &&
                             paiementsSqlTableModel.insertNewRecord(paiementsRecord,
                            		 	 	 	 	 	 	 	 	0,
																	"src/utils/yerith-erp-payment-processing-information.cpp",
																	210,
																	_reference);


    if (successPaiementsInsert)
    {
        YERITH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
    else
    {
        YERITH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;
    }

    return successPaiementsInsert;
}
