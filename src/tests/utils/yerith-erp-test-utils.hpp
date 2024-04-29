/*
 * yerith-erp-test-utils.hpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_TEST_UTILS_HPP_
#define YERITH_TEST_UTILS_HPP_


#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST


#include "src/imports/yerith-erp-stock-import.hpp"


class YerithERPWindows;


class Test_YerithERPTestUtils:public QObject
{
Q_OBJECT public:

    static void TEST_UTILS_truncate_database_all_tables();

    inline static YerithERPWindows *getAllWindows()
    {
        return _allWindows;
    }

    static void setAllWindows(YerithERPWindows *allWindows);

private:

    static YerithERPWindows *_allWindows;
};


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YERITH_TEST_UTILS_HPP_ */
