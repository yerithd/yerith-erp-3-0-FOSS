/*
 * yerith-erp-push-button-PASSWORD.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_WIDGETS_YERITH_PUSH_BUTTON_PASSWORD_HPP_
#define SRC_WIDGETS_YERITH_PUSH_BUTTON_PASSWORD_HPP_


#include "src/widgets/yerith-erp-push-button.hpp"


class YerithPushButtonPASSWORD:public QObject
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS YerithPushButtonPASSWORD(QObject *parent,
                                                    YerithLineEdit *lineEdit_mot_passe = 0,
                                                    YerithPushButton *cacher_mot_de_passe = 0,
                                                    YerithPushButton *voir_mot_de_passe = 0);

    virtual inline ~YerithPushButtonPASSWORD()
    {
    }

    inline YerithLineEdit *get_LINEEDIT_mot_de_passe()
    {
        return _lineEdit_mot_passe;
    }

    inline YerithPushButton *get_PUSH_BUTTON_voir_le_mot_de_passe()
    {
        return _voir_mot_de_passe;
    }

    inline YerithPushButton *get_PUSH_BUTTON_cacher_mot_de_passe()
    {
        return _cacher_mot_de_passe;
    }


    inline void set_LINEEDIT_mot_de_passe(YerithLineEdit *lineEdit_mot_passe)
    {
        _lineEdit_mot_passe = lineEdit_mot_passe;
    }

    inline void set_PUSH_BUTTON_voir_le_mot_de_passe(YerithPushButton *
                                                     voir_mot_de_passe)
    {
        voir_mot_de_passe = _voir_mot_de_passe;
    }

    inline void set_PUSH_BUTTON_cacher_mot_de_passe(YerithPushButton *
                                                    cacher_mot_de_passe)
    {
        _cacher_mot_de_passe = cacher_mot_de_passe;
    }

public slots:
    void voir_le_mot_de_passe();

    void cacher_le_mot_de_passe();

private:

    YerithLineEdit *_lineEdit_mot_passe;

    YerithPushButton *_voir_mot_de_passe;

    YerithPushButton *_cacher_mot_de_passe;
};


#endif /* SRC_WIDGETS_YERITH_PUSH_BUTTON_PASSWORD_HPP_ */
