/*
 * yerith-erp-user-manager.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_USER_MANAGER_HPP_
#define SRC_YERITH_ERP_USER_MANAGER_HPP_

#include "yerith-erp-users.hpp"

class YerithPOSUserManager:public YerithPOSUser
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithPOSUserManager(YerithERPWindows *
                                allWindows):YerithPOSUser(allWindows)
    {
        _role = YerithUtils::ROLE_MANAGER;
    }

    virtual inline ~YerithPOSUserManager()
    {
    }
};


#endif /* SRC_YERITH_ERP_USER_MANAGER_HPP_ */
