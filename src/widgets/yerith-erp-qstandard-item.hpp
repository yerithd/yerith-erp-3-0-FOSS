/*
 * yerith-erp-qstandard-item.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_QSTANDARD_ITEM_HPP_
#define YERITH_QSTANDARD_ITEM_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"


#include <QtGui/QStandardItem>


class YerithQStandardItem:public QStandardItem
{

public:

    YERITH_CLASS_OPERATORS YerithQStandardItem(Qt::Alignment alignPosition =
                                                               Qt::AlignHCenter | Qt::AlignCenter);

    YerithQStandardItem(int rows,
                        int columns = 1,
                        Qt::Alignment alignPosition =
                                        Qt::AlignHCenter | Qt::AlignCenter);

    YerithQStandardItem(const QString &text,
                        bool display__rawUNMODIFIED_FOR_USER_VIEWING_text =
                                        true, Qt::Alignment alignPosition =
                                        Qt::AlignHCenter | Qt::AlignCenter);

    YerithQStandardItem(const QIcon &icon,
                        const QString &text,
                        bool display__rawUNMODIFIED_FOR_USER_VIEWING_text =
                                        true, Qt::Alignment alignPosition =
                                        Qt::AlignHCenter | Qt::AlignCenter);

    inline ~YerithQStandardItem()
    {
    }

    inline void set_rawUNMODIFIED_FOR_USER_VIEWING_text(const QString &
                                                        aRawUNMODIFIED_FOR_USER_VIEWING_text)
    {
        _rawUNMODIFIED_FOR_USER_VIEWING_text =
                        aRawUNMODIFIED_FOR_USER_VIEWING_text;
    }

    inline QString get_rawUNMODIFIED_FOR_USER_VIEWING_text()
    {
        return _rawUNMODIFIED_FOR_USER_VIEWING_text;
    }

    inline void set__display__rawUNMODIFIED_FOR_USER_VIEWING_text(bool
                                                                  display__rawUNMODIFIED_FOR_USER_VIEWING_text)
    {
        _display__rawUNMODIFIED_FOR_USER_VIEWING_text =
                        display__rawUNMODIFIED_FOR_USER_VIEWING_text;
    }

    inline bool get__display__rawUNMODIFIED_FOR_USER_VIEWING_text()
    {
        return _display__rawUNMODIFIED_FOR_USER_VIEWING_text;
    }

protected:

    bool _display__rawUNMODIFIED_FOR_USER_VIEWING_text;

    QString _rawUNMODIFIED_FOR_USER_VIEWING_text;
};


#endif /* YERITH_QSTANDARD_ITEM_HPP_ */
