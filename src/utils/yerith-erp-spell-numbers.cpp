/*
 * yerith-erp-spell-numbers.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#include "src/utils/yerith-erp-spell-numbers.hpp"


#ifdef YERITH_FRANCAIS_LANGUAGE

const char *
YerithUtils_SPELL_NUMBERS::yerith_small_numbers[] =
{
    "zéro", "un", "deux", "trois", "quatre", "cinq",
    "six", "sept", "huit", "neuf", "dix",
    "onze", "douze", "treize", "quatorze", "quinze",
    "seize", "dix sept", "dix huit", "dix neuf"
};

const char *
YerithUtils_SPELL_NUMBERS::yerith_decades[] =
{
    "", "", "vingt", "trente", "quarante",
    "cinquante", "soixante", "soixante dix", "quatre vingt", "quatre vingt dix"
};

const char *
YerithUtils_SPELL_NUMBERS::yerith_thousand_powers[] =
{
    " milliard(s)", " million(s)", " mille(s)", ""
};

#else

const char *
YerithUtils_SPELL_NUMBERS::yerith_small_numbers[] =
{
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};

const char *
YerithUtils_SPELL_NUMBERS::yerith_decades[] =
{
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

const char *
YerithUtils_SPELL_NUMBERS::yerith_thousand_powers[] =
{
    " billion(s)", " million(s)", " thousand(s)", ""
};

#endif


const char *
YerithUtils_SPELL_NUMBERS::yerith_small_numbers_THREE_D_BARDIAGRAM[] =
{
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};

const char *
YerithUtils_SPELL_NUMBERS::yerith_decades_THREE_D_BARDIAGRAM[] =
{
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

const char *
YerithUtils_SPELL_NUMBERS::yerith_thousand_powers_THREE_D_BARDIAGRAM[] =
{
    " billion(s)", " million(s)", " thousand(s)", ""
};



QString YerithUtils_SPELL_NUMBERS::YERITH__spell_numbers(unsigned long n)
{
    if (n < 20)
    {
        return QString(yerith_small_numbers[n]);
    }

    QString res;

    const char **pScaleName = yerith_thousand_powers;

    //1 MILLIARD
    unsigned long scaleFactor = 1000000000;

    while (scaleFactor > 0)
    {
        if (n >= scaleFactor)
        {
            unsigned long h = n / scaleFactor;

#ifdef YERITH_FRANCAIS_LANGUAGE

            unsigned first_characters = (n / 100);

            if (h < 2)
            {
                res.append(*pScaleName);
            }
            else
            {
                res.append(yerith_spell_hundreds(h) + *pScaleName);
            }

#else

            res.append(yerith_spell_hundreds(h) + *pScaleName);

#endif

            n %= scaleFactor;

            if (0 != n)
            {
                res.append(" ");
            }
        }

        scaleFactor /= 1000;

        ++pScaleName;
    }

    return res;
}


QString YerithUtils_SPELL_NUMBERS::YERITH__spell_numbers___ENGLISH_ONLY(unsigned
                                                                        long n)
{
    if (n < 20)
    {
        return QString(yerith_small_numbers_THREE_D_BARDIAGRAM[n]);
    }

    QString res;

    const char **pScaleName = yerith_thousand_powers_THREE_D_BARDIAGRAM;

    //1 MILLIARD
    unsigned long scaleFactor = 1000000000;

    while (scaleFactor > 0)
    {
        if (n >= scaleFactor)
        {
            unsigned long h = n / scaleFactor;

            res.append(yerith_spell_hundreds___ENGLISH_ONLY(h) + *pScaleName);

            n %= scaleFactor;

            if (0 != n)
            {
                res.append(" ");
            }
        }

        scaleFactor /= 1000;

        ++pScaleName;
    }

    return res;
}


QString YerithUtils_SPELL_NUMBERS::yerith_spell_hundreds(unsigned n)
{
    QString res;

    if (n > 99)
    {
        res = yerith_small_numbers[n / 100];

#ifdef YERITH_FRANCAIS_LANGUAGE

        unsigned first_characters = (n / 100);

        if (first_characters < 2)
        {
            res.clear();
        }

        res.append(" cent(s)");
#else
        res.append(" hundred(s)");
#endif

        n %= 100;

        if (n)
        {
#ifdef YERITH_FRANCAIS_LANGUAGE
            res.append(" ");
#else
            res.append(" and ");
#endif
        }
    }

    if (n >= 20)
    {
        res.append(yerith_decades[n / 10]);

        n %= 10;

        if (n)
        {
            res.append("-");
        }
    }

    if (0 < n && n < 20)
    {
        res.append(yerith_small_numbers[n]);
    }

    return res;
}


QString YerithUtils_SPELL_NUMBERS::yerith_spell_hundreds___ENGLISH_ONLY(unsigned n)
{
    QString res;

    if (n > 99)
    {
        res = yerith_small_numbers_THREE_D_BARDIAGRAM[n / 100];

        res.append(" hundred(s)");

        n %= 100;

        if (n)
        {
            res.append(" and ");
        }
    }

    if (n >= 20)
    {
        res.append(yerith_decades_THREE_D_BARDIAGRAM[n / 10]);

        n %= 10;

        if (n)
        {
            res.append("-");
        }
    }

    if (0 < n && n < 20)
    {
        res.append(yerith_small_numbers_THREE_D_BARDIAGRAM[n]);
    }

    return res;
}
