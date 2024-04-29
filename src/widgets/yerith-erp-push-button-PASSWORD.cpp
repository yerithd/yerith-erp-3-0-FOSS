/*
 * yerith-erp-push-button-PASSWORD.cpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "yerith-erp-push-button-PASSWORD.hpp"


YerithPushButtonPASSWORD::YerithPushButtonPASSWORD(QObject *parent,
                                                   YerithLineEdit *
                                                   lineEdit_mot_passe
                                                   /* = 0 */,
                                                   YerithPushButton *
                                                   cacher_mot_de_passe
                                                   /* = 0 */,
                                                   YerithPushButton *
                                                   voir_mot_de_passe
                                                   /* = 0 */)
    :

    QObject(parent), _lineEdit_mot_passe(lineEdit_mot_passe),
    _cacher_mot_de_passe(cacher_mot_de_passe),
    _voir_mot_de_passe(voir_mot_de_passe)
{
    if (0 != _cacher_mot_de_passe)
    {
        _cacher_mot_de_passe->enable(this, SLOT(cacher_le_mot_de_passe()));
        _cacher_mot_de_passe->setEnabled(false);
    }

    if (0 != _voir_mot_de_passe)
    {
        _voir_mot_de_passe->enable(this, SLOT(voir_le_mot_de_passe()));
        _voir_mot_de_passe->setEnabled(true);
    }
}


void YerithPushButtonPASSWORD::cacher_le_mot_de_passe()
{
    if (0 != _lineEdit_mot_passe)
    {
        _lineEdit_mot_passe->setEchoMode(QLineEdit::Password);
    }

    if (0 != _cacher_mot_de_passe)
    {
        _cacher_mot_de_passe->setEnabled(false);
    }

    if (0 != _voir_mot_de_passe)
    {
        _voir_mot_de_passe->setEnabled(true);
    }
}


void YerithPushButtonPASSWORD::voir_le_mot_de_passe()
{
    if (0 != _lineEdit_mot_passe)
    {
        _lineEdit_mot_passe->setEchoMode(QLineEdit::Normal);
    }

    if (0 != _cacher_mot_de_passe)
    {
        _cacher_mot_de_passe->setEnabled(true);
    }

    if (0 != _voir_mot_de_passe)
    {
        _voir_mot_de_passe->setEnabled(false);
    }
}
