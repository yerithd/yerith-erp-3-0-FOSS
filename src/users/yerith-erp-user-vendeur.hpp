/*
 * yerith-erp-user-vendeur.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_USER_VENDEUR_HPP_
#define SRC_YERITH_ERP_USER_VENDEUR_HPP_

#include "yerith-erp-users.hpp"

class YerithERPUserVendeur:public YerithPOSUser
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithERPUserVendeur(YerithERPWindows *
                                allWindows):YerithPOSUser(allWindows)
    {
        _role = YerithUtils::ROLE_VENDEUR;
    }

    virtual inline ~YerithERPUserVendeur()
    {
    }
};


#endif /* SRC_YERITH_ERP_USER_MANAGER_HPP_ */
