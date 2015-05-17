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

#include "args_doc_rc.hpp"
#include "args_rc.hpp"
#include "../rpncron.hpp"

#include <cstdio>

namespace RC {
	namespace ArgsDocRc {
		static void printOptions(const char *first, ...);
	}
	
	void ArgsDocRc::showAllDocumentation(ArgsRc &current) {
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
		
		#include "args_rc.itm"
	}
	
	static void ArgsDocRc::printOptions(const char *first, ...) {
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
	
	void ArgsDocRc::onDaemonDoc(ArgsRc &current) {
		printf(
"\tDefault=%s, Current=%s\n"
"\tToggles the daemonize flag to go to background when starting the process with \n"
"\tthe --run option.\n\n",
			ArgsRc::getDefaultDaemonize() ? "yes" : "no",
			current.getDaemonize() ? "yes" : "no"
		);
	}
	
	void ArgsDocRc::onRunDoc(ArgsRc &current) {
		printf(
"\tDefault=%s, Current=%s\n"
"\tStarts the rpncron program.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_RUN ? "yes" : "no",
			current.getAction() == ArgsRc::ACTION_RUN ? "yes" : "no"
		);
	}
	
	void ArgsDocRc::onRunFileDoc(ArgsRc &current) {
		printf(
"\tDefault=(%s, '%s'), Current=(%s, '%s')\n"
"\tRun rpncron only on the given file. It cannot be used with -rd.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_RUN_FILE ? "yes" : "no",
			ArgsRc::getDefaultRunPath().c_str(),
			current.getAction() == ArgsRc::ACTION_RUN_FILE ? "yes" : "no",
			current.getRunPath().c_str()
		);
	}
	
	void ArgsDocRc::onRunDirDoc(ArgsRc &current) {
		printf(
"\tDefault=(%s, '%s'), Current=(%s, '%s')\n"
"\tRun rpncron only on the files in the given directory. It cannot be used with "
"\t-rf.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_RUN_DIR ? "yes" : "no",
			ArgsRc::getDefaultRunPath().c_str(),
			current.getAction() == ArgsRc::ACTION_RUN_DIR ? "yes" : "no",
			current.getRunPath().c_str()
		);
	}
	
	void ArgsDocRc::onPidFileDoc(ArgsRc &current) {
		printf(
"\tDefault='%s', Current='%s'\n"
"\tWrite the PID of the started process into the given file, if one is given.\n"
"\tThe file will only be written with -d.\n\n",
			ArgsRc::getDefaultPidPath().c_str(),
			current.getPidPath().c_str()
		);
	}
	
	void ArgsDocRc::onDebugFileDoc(ArgsRc &current) {
		printf(
"\tDefault=(%s, '%s', %lu), Current=(%s, '%s', %lu)\n"
"\tRun the given script in debug mode (display exactly what is being done and \n"
"\twhat will be executed). If the timestamp is equal to 'now', the current \n"
"\ttimestamp is taken.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_DEBUG_FILE ? "yes" : "no",
			ArgsRc::getDefaultDebugFile().c_str(),
			(unsigned long) ArgsRc::getDefaultDebugStart(),
			current.getAction() == ArgsRc::ACTION_DEBUG_FILE ? "yes" : "no",
			current.getDebugFile().c_str(),
			(unsigned long) current.getDebugStart()
		);
	}
	
	void ArgsDocRc::onVerbosityDoc(ArgsRc &current) {
		printf(
"\tDefault=%d, Current=%d\n"
"\tSet the wanted verbosity, between 0 and 3.\n\n",
			ArgsRc::getDefaultVerbosity(),
			current.getVerbosity()
		);
	}
	
	void ArgsDocRc::onUserDoc(ArgsRc &current) {
		printf(
"\tDefault='%s', Current='%s'\n"
"\tSet the user name that will be used for the -e and -l options.\n\n",
			ArgsRc::getDefaultUser().c_str(),
			current.getUser().c_str()
		);
	}
	
	void ArgsDocRc::onHelpDoc(ArgsRc &current) {
		printf(
"\tDefault=%s, Current=%s\n"
"\tDisplay this help text.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_HELP ? "yes" : "no",
			current.getAction() == ArgsRc::ACTION_HELP ? "yes" : "no"
		);
	}
}
