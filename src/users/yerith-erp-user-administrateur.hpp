/*
 * yerith-erp-user-administrateur.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_USER_ADMINISTRATEUR_HPP_
#define SRC_YERITH_ERP_USER_ADMINISTRATEUR_HPP_

#include "yerith-erp-users.hpp"

class YerithPOSUserAdministrateur:public YerithPOSUser
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithPOSUserAdministrateur(YerithERPWindows *
                                       allWindows):YerithPOSUser(allWindows)
    {
        _role = YerithUtils::ROLE_ADMINISTRATEUR;
    }

    virtual inline ~YerithPOSUserAdministrateur()
    {
    }
};

#endif /* SRC_YERITH_ERP_USER_ADMINISTRATEUR_HPP_ */
