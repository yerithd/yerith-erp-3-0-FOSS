/*
 * yerith-erp-command-line-parser.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef SRC_YERITH_COMMAND_LINE_PARSER_HPP_
#define SRC_YERITH_COMMAND_LINE_PARSER_HPP_

#include "src/include/yerith-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

enum YerithCommandLineArgumentType
{
    HELP = 0,
    GUI = 1,
    VERSION = 2,
    UNDEFINED = 3
};

class YerithCommandLineParser
{
public:

    YERITH_CLASS_OPERATORS inline YerithCommandLineParser()
    {
    }

    inline ~YerithCommandLineParser()
    {
    }

    enum YerithCommandLineArgumentType parseProgramArguments(int argc,
                                                             char *argv[]);
};

#endif /* SRC_YERITH_COMMAND_LINE_PARSER_HPP_ */
