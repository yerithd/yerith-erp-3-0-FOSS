/*
 * yerith-erp-creer-un-groupe-de-clients-table-widget.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_CREER_UN_GROUPE_DE_CLIENTS_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YERITH_CREER_UN_GROUPE_DE_CLIENTS_TABLE_WIDGET_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include "yerith-erp-qtable-widget-item.hpp"

#include "src/utils/yerith-erp-article-vente-info.hpp"

#include "yerith-erp-point-de-vente-table-widget.hpp"

#include <QtCore/QMap>

class YerithTableWidget;
class YerithQTableWidgetItem;
class YerithLogger;

class YerithCreerUnGroupeDeClientsTableWidget:public YerithTableWidget
{
	Q_OBJECT

public:

    YERITH_CLASS_OPERATORS

    inline YerithCreerUnGroupeDeClientsTableWidget(QWidget *parent = 0)
    :YerithTableWidget(parent),
	 _clientReferenceItem(0),
	 _clientNameItem(0)
    {
    }

    virtual inline ~YerithCreerUnGroupeDeClientsTableWidget()
    {
    }

    unsigned int addAClientGroupMember(const QString &aClientGroupMember_db_ID,
                                       const QString &aClientGroupMemberReferenceClient,
                                       const QString &aClientGroupMemberNomDuClient);

private:

    YerithQTableWidgetItem *_clientReferenceItem;

    YerithQTableWidgetItem *_clientNameItem;
};


#endif /* SRC_WIDGETS_YERITH_CREER_UN_GROUPE_DE_CLIENTS_TABLE_WIDGET_HPP_ */
