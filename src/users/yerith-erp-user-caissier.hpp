/*
 * yerith-erp-user-caissier.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_USER_CAISSIER_HPP_
#define SRC_YERITH_ERP_USER_CAISSIER_HPP_

#include "yerith-erp-users.hpp"

class YerithPOSUserCaissier:public YerithPOSUser
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithPOSUserCaissier(YerithERPWindows *
                                 allWindows):YerithPOSUser(allWindows)
    {
        _role = YerithUtils::ROLE_CAISSIER;
    }

    virtual inline ~YerithPOSUserCaissier()
    {
    }
};

#endif /* SRC_YERITH_ERP_USER_CAISSIER_HPP_ */
