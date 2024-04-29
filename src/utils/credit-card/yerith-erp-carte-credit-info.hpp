/*
 * yerith-erp-credit-card-info.hpp
 *
 *   Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_CREDIT_CARD_INFO_HPP_
#define YERITH_ERP_CREDIT_CARD_INFO_HPP_



#include <QtCore/QString>



class YerithPOSCreditCardInfo
{

public:

    inline YerithPOSCreditCardInfo()
    {
    }


    inline ~YerithPOSCreditCardInfo()
    {
    }

    void clearAllFields();

    QString _issuingCompany;

    QString _nameOnCard;

    QString _postalCode;

    QString _cardNumber;

    QString _expiryDate_YY;

    QString _expiryDate_MM;

    QString _cvCode;
};





#endif // YERITH_ERP_CREDIT_CARD_INFO_HPP_
