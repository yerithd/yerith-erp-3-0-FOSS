/*
 * yerith-erp-user-gestionaire-des-stocks.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_USER_GESTIONAIRE_DES_STOCKS_HPP_
#define SRC_YERITH_ERP_USER_GESTIONAIRE_DES_STOCKS_HPP_

#include "yerith-erp-users.hpp"

class YerithPOSUserGestionaireDesStocks:public YerithPOSUser
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithPOSUserGestionaireDesStocks(YerithERPWindows *
                                             allWindows):YerithPOSUser
        (allWindows)
    {
        _role = YerithUtils::ROLE_GESTIONNAIREDESTOCK;
    }

    virtual inline ~YerithPOSUserGestionaireDesStocks()
    {
    }
};


#endif /* SRC_YERITH_ERP_USER_MANAGER_HPP_ */
