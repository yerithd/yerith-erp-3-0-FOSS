/*
 * yerith-erp-test-yerith-erp-utils.hpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_TEST_YERITH_ERP_UTILS_WINDOWS_HPP_
#define YERITH_TEST_YERITH_ERP_UTILS_WINDOWS_HPP_


#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST


#include "src/include/yerith-erp-3-0-definition-oo-class-operators.hpp"

#include "src/tests/utils/yerith-erp-test-utils.hpp"


#include <QtCore/QObject>


class YerithUtils;


class Test_YerithUtils:public QObject
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS inline Test_YerithUtils()
    {
    }

    inline ~Test_YerithUtils()
    {
    }

private slots:
    inline void initTestCase()
    {
    }


    void test_CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING();

    void test_REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING();

private:

    YerithUtils *_yerith_erp_utils;
};


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YERITH_TEST_YERITH_ERP_UTILS_WINDOWS_HPP_ */
