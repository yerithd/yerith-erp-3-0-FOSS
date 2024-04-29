/*
 * yerith-erp-spell-numbers.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_SPELL_NUMBERS_HPP_
#define SRC_YERITH_SPELL_NUMBERS_HPP_


#include "src/utils/yerith-erp-utils.hpp"


#include <QtCore/QObject>


class YerithUtils_SPELL_NUMBERS:public QObject
{
Q_OBJECT public:

    YERITH_CLASS_OPERATORS inline YerithUtils_SPELL_NUMBERS()
    {
    }

    inline ~YerithUtils_SPELL_NUMBERS()
    {
    }

    static QString YERITH__spell_numbers(unsigned long n);

    static QString YERITH__spell_numbers___ENGLISH_ONLY(unsigned long n);

private:

    static QString yerith_spell_hundreds(unsigned n);

    static QString yerith_spell_hundreds___ENGLISH_ONLY(unsigned n);


    static const char *yerith_small_numbers[];

    static const char *yerith_decades[];

    static const char *yerith_thousand_powers[];


    static const char *yerith_small_numbers_THREE_D_BARDIAGRAM[];

    static const char *yerith_decades_THREE_D_BARDIAGRAM[];

    static const char *yerith_thousand_powers_THREE_D_BARDIAGRAM[];
};


#endif /* SRC_YERITH_SPELL_NUMBERS_HPP_ */
