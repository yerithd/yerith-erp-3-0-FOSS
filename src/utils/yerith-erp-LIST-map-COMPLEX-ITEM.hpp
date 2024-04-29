/*
 * yerith-erp-LIST-MAP-COMPLEX-ITEM.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_LIST_MAP_COMPLEX_ITEM_HPP_
#define SRC_YERITH_LIST_MAP_COMPLEX_ITEM_HPP_


#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include "yerith-erp-map-COMPLEX-ITEM.hpp"


#include <QtCore/QtAlgorithms>

#include <QtCore/QString>


class Yerith_LIST_map_COMPLEX_ITEM
{
public:

    inline Yerith_LIST_map_COMPLEX_ITEM()
    {
    }

    virtual inline ~Yerith_LIST_map_COMPLEX_ITEM()
    {
        clear();
    }

    virtual void clear();

    virtual const Yerith_MAP_COMPLEX_Item *find_item(const QString
                                                     an_item_name);

    virtual void insert_item(const QString &an_item_name,
                             double aDOUBLE_value);

    virtual inline void q_sort()
    {
        qSort(_my_list_complex_items.begin(),
              _my_list_complex_items.end(), Yerith_MAP_COMPLEX_Item::lessThan);
    }

    virtual inline int isEmpty()
    {
        return (_my_list_complex_items.size() <= 0);
    }

    virtual inline int size()
    {
        return _my_list_complex_items.size();
    }

    virtual inline const QList < Yerith_MAP_COMPLEX_Item * > &q_list()
    {
        return _my_list_complex_items;
    }

    virtual void print(const QString &a_label = YerithUtils::EMPTY_STRING);

private:

    QList < Yerith_MAP_COMPLEX_Item * >_my_list_complex_items;
};

#endif /* SRC_YERITH_LIST_MAP_COMPLEX_ITEM_HPP_ */
