/*
 * yerith-erp-payment-processing-information.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_PAYMENT_PROCESSING_HPP_
#define YERITH_ERP_PAYMENT_PROCESSING_HPP_

#include "src/utils/yerith-erp-utils.hpp"


#include <QtCore/QString>


class YerithERPPaymentProcessingInformation
{
public:

    YERITH_CLASS_OPERATORS
    inline YerithERPPaymentProcessingInformation():
    _type_de_paiement(YerithUtils::DECAISSEMENT_INDEFINI),
    _nouveau_compte_fournisseur(0.0),
    _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS(0.0),
    _nouveau_compte_client(0.0),
    _montant_paye(0.0)
    {
    }

    inline ~YerithERPPaymentProcessingInformation()
    {
    }


public:

    double montant_paye(int type_de_paiement);


    bool save_Human_RESOURCE_payment_info_record(bool is_supplier_payment = false,
                                                 QString Receiver_OF_money_WHEN_processed = YerithUtils::EMPTY_STRING);


    bool save_payment_info_record(bool is_supplier_payment = false,
                                  QString Receiver_OF_money_WHEN_processed = YerithUtils::EMPTY_STRING);


protected:

    bool update_supplier_account(int                 type_de_paiment,
                                 YerithERPWindows   &allWindows);


public:

    int _type_de_paiement;

    double _nouveau_compteClient_PROGRAMME_DE_FIDELITE_CLIENTS;

    double _nouveau_compte_client;

    double _montant_paye;

    QString _paiement_INTITULE_DE_LA_LIGNE_BUDGETAIRE;

    QString _paiement_intitule_compte_bancaire;

    QString _nom_entreprise;

    QString _NOTES;

    QString _designation;

    QString _reference;


protected:

    double _nouveau_compte_fournisseur;
};


#endif //YERITH_ERP_PAYMENT_PROCESSING_HPP_
