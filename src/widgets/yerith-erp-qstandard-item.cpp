/*
 * yerith-erp-qstandard-item.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-qstandard-item.hpp"


#include "src/utils/yerith-erp-utils.hpp"


YerithQStandardItem::YerithQStandardItem(Qt::Alignment alignPosition	/* = Qt::AlignHCenter |
									   Qt::AlignCenter    */)
    :QStandardItem(), _display__rawUNMODIFIED_FOR_USER_VIEWING_text(true)
{
    setTextAlignment(alignPosition);
}


YerithQStandardItem::YerithQStandardItem(int rows, int columns /* = 1 */,
                                         Qt::Alignment alignPosition	/* = Qt::AlignHCenter |
									   Qt::AlignCenter    */)
    :
    QStandardItem(rows, columns),
    _display__rawUNMODIFIED_FOR_USER_VIEWING_text(true)
{
    setTextAlignment(alignPosition);
}


YerithQStandardItem::YerithQStandardItem(const QString &text,
                                         bool
                                         display__rawUNMODIFIED_FOR_USER_VIEWING_text
                                         /* = true */,
                                         Qt::Alignment alignPosition	/* = Qt::AlignHCenter |
									   Qt::AlignCenter   */)
    :
    QStandardItem(display__rawUNMODIFIED_FOR_USER_VIEWING_text ? text :
                  YerithUtils::YERITH_TRUNCATE_STRING_ACCORDING_TO_SETTING
                  (text)),
    _display__rawUNMODIFIED_FOR_USER_VIEWING_text
    (display__rawUNMODIFIED_FOR_USER_VIEWING_text),
    _rawUNMODIFIED_FOR_USER_VIEWING_text(text)
{
    setTextAlignment(alignPosition);
}


YerithQStandardItem::YerithQStandardItem(const QIcon &icon,
                                         const QString &text,
                                         bool
                                         display__rawUNMODIFIED_FOR_USER_VIEWING_text
                                         /* = true */,
                                         Qt::Alignment alignPosition	/* = Qt::AlignHCenter |
									   Qt::AlignCenter    */)
    :
    QStandardItem(icon,
                  display__rawUNMODIFIED_FOR_USER_VIEWING_text ? text :
                  YerithUtils::YERITH_TRUNCATE_STRING_ACCORDING_TO_SETTING
                  (text)),
    _display__rawUNMODIFIED_FOR_USER_VIEWING_text
    (display__rawUNMODIFIED_FOR_USER_VIEWING_text),
    _rawUNMODIFIED_FOR_USER_VIEWING_text(text)
{
    setTextAlignment(alignPosition);
}
