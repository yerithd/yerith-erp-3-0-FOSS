/*
 * yerith-erp-3-0-definition-thermal-printers-commands.hpp
 *
 *      Author: Prof Dr Xavier Noundou
 */

#ifndef YERITH_ERP_DEFINITION_THERMAL_PRINTERS_COMMANDS_HPP_
#define YERITH_ERP_DEFINITION_THERMAL_PRINTERS_COMMANDS_HPP_



#define COMMAND_LINE_COMMAND_OPEN_CASH_DRAWER_EPSON_TM_T20II	"echo -e -n \x1b\x70\x00\x19\xfa > %1"


#define COMMAND_LINE_COMMAND_PRINT_EPSON_TM_T20II				"echo | cat %1 >> %2"



#endif /* YERITH_ERP_DEFINITION_THERMAL_PRINTERS_COMMANDS_HPP_ */
