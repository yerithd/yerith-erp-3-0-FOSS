/*
 * yerith-erp-user-magasinier.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_ERP_USER_MAGASINIER_HPP_
#define SRC_YERITH_ERP_USER_MAGASINIER_HPP_

#include "yerith-erp-users.hpp"

class YerithPOSUserMagasinier:public YerithPOSUser
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS
    inline YerithPOSUserMagasinier(YerithERPWindows *
                                   allWindows):YerithPOSUser(allWindows)
    {
        _role = YerithUtils::ROLE_MAGASINIER;
    }

    virtual inline ~YerithPOSUserMagasinier()
    {
    }
};

#endif /* SRC_YERITH_ERP_USER_MAGASINIER_HPP_ */
