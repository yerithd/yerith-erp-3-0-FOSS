/*
 * yerith-erp-map-COMPLEX-ITEM.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_MAP_COMPLEX_ITEM_HPP_
#define SRC_YERITH_MAP_COMPLEX_ITEM_HPP_


#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"


#include <QtCore/QString>


class Yerith_MAP_COMPLEX_Item
{
public:
    QString _itemName;
    double _itemValue;
    double _itemSecondValue;

    inline Yerith_MAP_COMPLEX_Item(QString itemName,
                                   double itemValue,
                                   double
                                   itemSecondValue):_itemName(itemName),
        _itemValue(itemValue), _itemSecondValue(itemSecondValue)
    {
    }

    inline Yerith_MAP_COMPLEX_Item(QString itemName,
                                   double itemValue):_itemName(itemName),
        _itemValue(itemValue), _itemSecondValue(0.0)
    {
    }

    inline QString toString()
    {
        return QString("%1 [%3, %4]").arg(_itemName,
                                          QString::number(_itemValue),
                                          QString::number(_itemSecondValue));
    }

    inline static bool lessThan(const Yerith_MAP_COMPLEX_Item *anItem1,
                                const Yerith_MAP_COMPLEX_Item *anItem2)
    {
        return anItem1->_itemValue < anItem2->_itemValue;
    }

    inline static bool lessThan_second_value(const Yerith_MAP_COMPLEX_Item *
                                             anItem1,
                                             const Yerith_MAP_COMPLEX_Item *
                                             anItem2)
    {
        return anItem1->_itemSecondValue < anItem2->_itemSecondValue;
    }
};

#endif /* SRC_YERITH_MAP_COMPLEX_ITEM_HPP_ */
