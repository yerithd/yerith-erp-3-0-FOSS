/*
 * yerith-erp-logger.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_LOGGER_HPP_
#define SRC_YERITH_LOGGER_HPP_


#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"


#include <QtCore/QString>


class QDebug;
class QFile;


class YerithPOSUser;
class YerithERPWindows;


class YerithLogger
{
public:

    YERITH_CLASS_OPERATORS typedef enum _YERITH_LOG_LEVEL
    {
        YERITH_DEBUG = 0,
        YERITH_LOG,
        YERITH_WARN
    } YERITH_LOG_LEVEL;

    YerithLogger(QString cppClassName, YERITH_LOG_LEVEL logLevel =
                                 YERITH_LOG);

    ~YerithLogger();

    void renewCurrentYERITHERPUser();

    void debug(const char *cppMethodName, const char *msg = "");

    void debug(const char *cppMethodName, const QString &msg);

    void log(const char *cppMethodName, const char *msg = "");

    void log(const char *cppMethodName, const QString &msg);

    YerithLogger &operator<< (const char *s);
    YerithLogger &operator<< (const QString &s);
    YerithLogger &operator<< (bool s);
    YerithLogger &operator<< (int s);
    YerithLogger &operator<< (char s);
    YerithLogger &operator<< (const QByteArray &s);

private:

    YERITH_LOG_LEVEL _logLevel;

    QString _curUserUtilisateur;

    QString _curUserNomComplet;

    QString _cppClassName;

    YerithPOSUser *_curYERITHERPUser;

    static QDebug *_qDebug;

    static QFile *_logFile;

    static YerithERPWindows *_allWindows;
};


#endif //SRC_YERITH_LOGGER_HPP_
