/*
 * yerith-erp-historique-stock.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "src/utils/yerith-erp-historique-stock.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-utils.hpp"


#include <QtCore/QMap>


const QString YerithHistoriqueStock::SEPARATION_EXTERNE(YerithUtils::STAR_CHAR);

const QString YerithHistoriqueStock::SEPARATION_INTERNE(YerithUtils::DASH_CHAR);

QMap < QString, enum TypeMouvementStock >
YerithHistoriqueStock::_qstringToTypeMouvement;


YerithHistoriqueStock::YerithHistoriqueStock()
{
    _qstringToTypeMouvement.insert("0", MOUVEMENT_DE_STOCK_INDEFINI);
    _qstringToTypeMouvement.insert("1", MOUVEMENT_DE_STOCK_ENTREE);
    _qstringToTypeMouvement.insert("2", MOUVEMENT_DE_STOCK_VENTE);
    _qstringToTypeMouvement.insert("3", MOUVEMENT_DE_STOCK_SORTIE);
    _qstringToTypeMouvement.insert("4", MOUVEMENT_DE_STOCK_TRANSFERT);
    _qstringToTypeMouvement.insert("5", MOUVEMENT_DE_STOCK_RETOUR_VENTE);
    _qstringToTypeMouvement.insert("6",
                                   MOUVEMENT_DE_STOCK_RE_APPROVISIONNEMENT);
}


void YerithHistoriqueStock::listHistoriqueDuStock(const QString &
                                                  historiqueDuStock,
                                                  const QString &stockReference,
                                                  const QString &
                                                  stockDesignation,
                                                  bool show_ASSET /* = false */)
{
    //qDebug() << QString("YerithHistoriqueStock::listHistoriqueDuStock | %1")
    //                              .arg(QString::number(aStockId));

    QStringList aMouvementStockList;

    aMouvementStockList.append(historiqueDuStock.split
                               (YerithHistoriqueStock::SEPARATION_EXTERNE));

    YerithUtils::getAllWindows()->_historiqueDuStockWindow->
    listHistoriqueDuStock(aMouvementStockList, stockReference,
                          stockDesignation, show_ASSET);
}


QString YerithHistoriqueStock::get_mouvement_stock(QString aMouvementStock_in_out)
{
    QString aMouvementStock;

    QStringList
    aComplexMouvementStock(aMouvementStock_in_out.split
                           (YerithHistoriqueStock::SEPARATION_INTERNE));

    return aMouvementStock;
}


QString YerithHistoriqueStock::creer_mouvement_stock(enum TypeMouvementStock
                                                     aTypeMouvementStock,
                                                     int mouvementId, QDate date,
                                                     double qte_initiale_en_stock,
                                                     double qte_en_mouvement,
                                                     double qte_restante)
{
    unsigned int typeMouvement = (unsigned int) aTypeMouvementStock;

    QString result =
                    QString("%1_%2_%3_%4_%5_%6_%7").arg(DATE_TO_STRING(date),
                                                        CURRENT_TIME,
                                                        QString::number(typeMouvement),
                                                        QString::number(mouvementId),
                                                        GET_DOUBLE_STRING
                                                        (qte_initiale_en_stock),
                                                        GET_DOUBLE_STRING
                                                        (qte_en_mouvement),
                                                        GET_DOUBLE_STRING(qte_restante));

    return result;
}


QString YerithHistoriqueStock::get_type_mouvement_stock_string(const QString &
                                                               aStockHistory)
{
    //enum TypeMouvementStock type_mouvement(YerithHistoriqueStock::get_type_mouvement_stock(aStockHistory));
    //qDebug() << "++ qstring: " << aStockHistory;

    if ("1" == aStockHistory)
    {
        return QObject::tr("ENTRÉE");
    }
    else if ("2" == aStockHistory)
    {
        return QObject::tr("VENTE");
    }
    else if ("3" == aStockHistory)
    {
        return QObject::tr("SORTIE");
    }
    else if ("4" == aStockHistory)
    {
        return QObject::tr("TRANSFERT");
    }
    else if ("5" == aStockHistory)
    {
        return QObject::tr("RETOUR VENTE");
    }
    else if ("6" == aStockHistory)
    {
        return QObject::tr("RÉ-APPROVISIONNEMENT");
    }
    else
    {
        return QObject::tr("INDEFINI");
    }
}


enum TypeMouvementStock YerithHistoriqueStock::get_type_mouvement_stock(const QString &
                                                                        aStockHistoryEntry)
{
    QString type_mouvement =
                    aStockHistoryEntry.split(YerithHistoriqueStock::SEPARATION_INTERNE).
                    at(2);

    QMap < QString, enum TypeMouvementStock >::const_iterator i =
                    _qstringToTypeMouvement.find(type_mouvement);

    if (i != _qstringToTypeMouvement.end())
    {
        //qDebug() << QString("++ test-2: %1").arg(QString::number((int)i.value()));
        return i.value();
    }
    else
    {
        //qDebug() << QString("++ test-3: %1").arg(QString::number(INDEFINI));
        return MOUVEMENT_DE_STOCK_INDEFINI;
    }
}


QDate YerithHistoriqueStock::get_date_mouvement_stock(QString aStockHistory)
{
    QString
    date =
                    aStockHistory.split(YerithHistoriqueStock::SEPARATION_INTERNE).first();

    return GET_DATE_FROM_STRING(date);
}
