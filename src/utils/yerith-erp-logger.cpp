/*
 * yerith-erp-logger.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-logger.hpp"

#include "src/yerith-erp-windows.hpp"

#include "src/utils/yerith-erp-utils.hpp"

#include "src/users/yerith-erp-users.hpp"


#include <QtCore/QDebug>
#include <QtCore/QFile>


QDebug *YerithLogger::_qDebug(0);

QFile *YerithLogger::_logFile(0);

YerithERPWindows *YerithLogger::_allWindows(0);


YerithLogger::YerithLogger(QString cppClassName, YERITH_LOG_LEVEL logLevel):_logLevel(logLevel),
    _cppClassName(cppClassName),
    _curYERITHERPUser(0)
{
    static bool
    alreadyInstantiated = false;

    if (!alreadyInstantiated)
    {
        _logFile = new QFile(YerithUtils::getLogFileName());

        _qDebug = new QDebug(_logFile);

        _allWindows = YerithUtils::getAllWindows();

        _logFile->open(QIODevice::WriteOnly | QIODevice::Append);

        alreadyInstantiated = true;
    }
}

YerithLogger::~YerithLogger()
{
    static bool alreadyDestroyed = false;

    if (!alreadyDestroyed)
    {
        _logFile->close();

        alreadyDestroyed = true;
    }
}


void YerithLogger::renewCurrentYERITHERPUser()
{
    _curUserUtilisateur.clear();

    _curUserNomComplet.clear();

    _curUserUtilisateur.append(YerithUtils::UTILISATEUR_NON_EXISTANT);

    _curUserNomComplet.append(YerithUtils::UTILISATEUR_NON_EXISTANT);

    if (0 == _allWindows)
    {
        _allWindows = YerithUtils::getAllWindows();
    }

    if (0 != _allWindows && !_allWindows->CURRENTLY_CHANGING_USER)
    {
        _curYERITHERPUser = _allWindows->getUser();
    }

    if (0 != _curYERITHERPUser && !_allWindows->CURRENTLY_CHANGING_USER)
    {
        _curUserUtilisateur = _curYERITHERPUser->nom_utilisateur();

        _curUserNomComplet = _curYERITHERPUser->nom_complet();
    }
}


void YerithLogger::debug(const char *cppMethodName, const char *msg)
{
    renewCurrentYERITHERPUser();

    *_qDebug <<
             QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                                 _curUserUtilisateur,
                                                                 GET_CURRENT_STRING_DATE,
                                                                 CURRENT_TIME_WITH_MILLISECONDS,
                                                                 _cppClassName,
                                                                 cppMethodName).toStdString
             ().c_str() << msg << "\n";

#ifdef YERITH_DEBUG_LOG
    qDebug() <<
             QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                                 _curUserUtilisateur,
                                                                 GET_CURRENT_STRING_DATE,
                                                                 CURRENT_TIME_WITH_MILLISECONDS,
                                                                 _cppClassName,
                                                                 cppMethodName).toStdString
             ().c_str() << msg;
#endif
}


void YerithLogger::debug(const char *cppMethodName, const QString &msg)
{
    renewCurrentYERITHERPUser();

    *_qDebug <<
             QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                                 _curUserUtilisateur,
                                                                 GET_CURRENT_STRING_DATE,
                                                                 CURRENT_TIME_WITH_MILLISECONDS,
                                                                 _cppClassName,
                                                                 cppMethodName).toStdString
             ().c_str() << msg << "\n";

#ifdef YERITH_DEBUG_LOG
    qDebug() <<
             QString("DEBUG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                                 _curUserUtilisateur,
                                                                 GET_CURRENT_STRING_DATE,
                                                                 CURRENT_TIME_WITH_MILLISECONDS,
                                                                 _cppClassName,
                                                                 cppMethodName).toStdString
             ().c_str() << msg;
#endif
}


void YerithLogger::log(const char *cppMethodName, const char *msg)
{
    renewCurrentYERITHERPUser();

    assert(_qDebug != 0);

    *_qDebug <<
             QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                               _curUserUtilisateur,
                                                               GET_CURRENT_STRING_DATE,
                                                               CURRENT_TIME_WITH_MILLISECONDS,
                                                               _cppClassName,
                                                               cppMethodName).toStdString
             ().c_str() << msg << "\n";

#ifdef YERITH_DEBUG_LOG
    qDebug() <<
             QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                               _curUserUtilisateur,
                                                               GET_CURRENT_STRING_DATE,
                                                               CURRENT_TIME_WITH_MILLISECONDS,
                                                               _cppClassName,
                                                               cppMethodName).toStdString
             ().c_str() << msg;
#endif
}


void YerithLogger::log(const char *cppMethodName, const QString &msg)
{
    renewCurrentYERITHERPUser();

    assert(_qDebug != 0);

    *_qDebug <<
             QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                               _curUserUtilisateur,
                                                               GET_CURRENT_STRING_DATE,
                                                               CURRENT_TIME_WITH_MILLISECONDS,
                                                               _cppClassName,
                                                               cppMethodName).toStdString
             ().c_str() << msg << "\n";

#ifdef YERITH_DEBUG_LOG
    qDebug() <<
             QString("LOG | %1 \% %2 | %3 %4 | %5 | %6 |").arg(_curUserNomComplet,
                                                               _curUserUtilisateur,
                                                               GET_CURRENT_STRING_DATE,
                                                               CURRENT_TIME_WITH_MILLISECONDS,
                                                               _cppClassName,
                                                               cppMethodName).toStdString
             ().c_str() << msg;
#endif
}


YerithLogger &YerithLogger::operator<< (const char *s)
{
    *_qDebug << s;
    return *this;
}


YerithLogger &YerithLogger::operator<< (const QString &s)
{
    assert(_qDebug != 0);
    *_qDebug << s;
    return *this;
}


YerithLogger &YerithLogger::operator<< (bool s)
{
    *_qDebug << s;
    return *this;
}


YerithLogger &YerithLogger::operator<< (int s)
{
    *_qDebug << s;
    return *this;
}


YerithLogger &YerithLogger::operator<< (char s)
{
    *_qDebug << s;
    return *this;
}


YerithLogger &YerithLogger::operator<< (const QByteArray &s)
{
    *_qDebug << s;
    return *this;
}
