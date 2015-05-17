/*
 * This file is part of rpncron.
 * (C) 2014,2015 Maxime Ferrino
 * 
 * rpncron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * rpncron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with rpncron.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "args_doc_rct.hpp"
#include "args_rc.hpp"
#include "../rpncron.hpp"

#include <stdarg.h>

#include <cstdio>

namespace RC {
	namespace ArgsDocRct {
		static void printOptions(const char *first, ...);
	}
	
	void ArgsDocRct::showAllDocumentation(ArgsRct &current) {
		printf("%s",
"Rpncron, version " RPNCRON_PROGRAM_VERSION "\n"
"Copyright (C) 2014,2015 Maxime Ferrino.\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
"\n"
"This is free software; you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"\n"
);
		
		#define ITEM(exec_callback, argcnt, ...) \
			printOptions(__VA_ARGS__, NULL); \
			printf("\tThis option takes %d arguments\n", argcnt); \
			exec_callback ## Doc(current);
		
		#include "args_rct.itm"
	}
	
	static void ArgsDocRct::printOptions(const char *first, ...) {
		const char *next;
		va_list args;
		
		printf("%s", first);
		
		va_start(args, first);
		while((next = va_arg(args, const char*)) != NULL) {
			printf(" | %s", next);
		}
		va_end(args);
		
		printf("\n");
	}
	
	void ArgsDocRct::onVerbosityDoc(ArgsRct &current) {
		printf(
"\tDefault=%d, Current=%d\n"
"\tSet the wanted verbosity, between 0 and 3.\n\n",
			ArgsRct::getDefaultVerbosity(),
			current.getVerbosity()
		);
	}
	
	void ArgsDocRct::onEditDoc(ArgsRct &current) {
		printf(
"\tDefault=%s, File=%s, Current=%s\n"
"\tEdit the given rpncron file in the user directory of the one given by -u.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_EDIT ? "yes" : "no",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_EDIT ? current.getRctFile().c_str() : "",
			current.getAction() == ArgsRct::ACTION_EDIT ? "yes" : "no"
		);
	}
	
	void ArgsDocRct::onListDoc(ArgsRct &current) {
		printf(
"\tDefault=%s, Current=%s\n"
"\tShows a list of all rpncron files for the user given by -u.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_LIST ? "yes" : "no",
			current.getAction() == ArgsRct::ACTION_LIST ? "yes" : "no"
		);
	}
	
	void ArgsDocRct::onPrintDoc(ArgsRct &current) {
		printf(
"\tDefault=%s, File=%s, Current=%s\n"
"\tPrint the given rpncron file in the user directory of the one given by -u.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_PRINT ? "yes" : "no",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_PRINT ? current.getRctFile().c_str() : "",
			current.getAction() == ArgsRct::ACTION_PRINT ? "yes" : "no"
		);
	}
	
	void ArgsDocRct::onUserDoc(ArgsRct &current) {
		printf(
"\tDefault='%s', Current='%s'\n"
"\tSet the user name that will be used for the edit and list options.\n\n",
			ArgsRct::getDefaultUser().c_str(),
			current.getUser().c_str()
		);
	}
	
	void ArgsDocRct::onRemoveDoc(ArgsRct &current) {
		printf(
"\tDefault=%s, File=%s, Current=%s\n"
"\tRemove the rpncron file of the user given by -u.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_REMOVE ? "yes" : "no",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_REMOVE ? current.getRctFile().c_str() : "",
			current.getAction() == ArgsRct::ACTION_REMOVE ? "yes" : "no"
		);
	}
	
	void ArgsDocRct::onHelpDoc(ArgsRct &current) {
		printf(
"\tDefault=%s, Current=%s\n"
"\tDisplay this help text.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_HELP ? "yes" : "no",
			current.getAction() == ArgsRct::ACTION_HELP ? "yes" : "no"
		);
	}
}
