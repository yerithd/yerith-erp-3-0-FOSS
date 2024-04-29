/*
 * yerith-erp-test-yerith-erp-windows.hpp
 *
 *      @author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_TEST_YERITH_ERP_WINDOWS_HPP_
#define YERITH_TEST_YERITH_ERP_WINDOWS_HPP_


#ifdef YERITH_ERP_3_0_TESTING_UNIT_TEST


#include <QtTest/QtTest>


class YerithERPWindows;


class Test_YerithERPWindows:public QObject
{
Q_OBJECT public:

    inline Test_YerithERPWindows(YerithERPWindows *
                                 allWindows):_allWindows(0)
    {
        _allWindows = allWindows;
    }

private slots:
    inline void initTestCase()
    {
        QVERIFY(0 != _allWindows);
    }

private:

    YerithERPWindows *_allWindows;
};


#endif /* YERITH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YERITH_TEST_YERITH_ERP_WINDOWS_HPP_ */
