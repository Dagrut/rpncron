/*
 * This file is part of rpncron.
 * (C) 2014 Maxime Ferrino
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
	void ArgsDocRc::showAllDocumentation(ArgsRc &current) {
		printf("%s",
"Rpncron, version " RPNCRON_PROGRAM_VERSION "\n"
"Copyright (C) 2014 Maxime Ferrino.\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
"\n"
"This is free software; you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"\n"
);
		
		#define CALL(name) \
			name ## Doc(current);
		
		CALL(onDaemon)
		CALL(onRun)
		CALL(onRunFile)
		CALL(onRunDir)
		CALL(onDebugFile)
		CALL(onVerbose)
		CALL(onQuiet)
		CALL(onUser)
		CALL(onHelp)
		
		#undef CALL
	}
	
	void ArgsDocRc::onDaemonDoc(ArgsRc &current) {
		printf(
"-d --daemon\n"
"Default=%s, Current=%s\n"
"Toggles the daemonize flag to go to background when starting the process with \n"
"the --run option.\n\n",
			ArgsRc::getDefaultDaemonize() ? "yes" : "no",
			current.getDaemonize() ? "yes" : "no"
		);
	}
	
	void ArgsDocRc::onRunDoc(ArgsRc &current) {
		printf(
"-r -run --run\n"
"Default=%s, Current=%s\n"
"Starts the rpncron program.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_RUN ? "yes" : "no",
			current.getAction() == ArgsRc::ACTION_RUN ? "yes" : "no"
		);
	}
	
	void ArgsDocRc::onRunFileDoc(ArgsRc &current) {
		printf(
"-rf --run-file <File>\n"
"Default=(%s, '%s'), Current=(%s, '%s')\n"
"Run rpncron only on the given file.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_RUN_FILE ? "yes" : "no",
			ArgsRc::getDefaultRunPath().c_str(),
			current.getAction() == ArgsRc::ACTION_RUN_FILE ? "yes" : "no",
			current.getRunPath().c_str()
		);
	}
	
	void ArgsDocRc::onRunDirDoc(ArgsRc &current) {
		printf(
"-rd --run-dir <Directory>\n"
"Default=(%s, '%s'), Current=(%s, '%s')\n"
"Run rpncron only on the files in the given directory.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_RUN_DIR ? "yes" : "no",
			ArgsRc::getDefaultRunPath().c_str(),
			current.getAction() == ArgsRc::ACTION_RUN_DIR ? "yes" : "no",
			current.getRunPath().c_str()
		);
	}
	
	void ArgsDocRc::onDebugFileDoc(ArgsRc &current) {
		printf(
"-df2 --debug-file-2 <Filename> <Timestamp>\n"
"Default=(%s, '%s', %lu), Current=(%s, '%s', %lu)\n"
"Run the given script in debug mode (display exactly what is being done and \n"
"what will be executed). If the timestamp is equal to 'now', the current \n"
"timestamp is taken.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_DEBUG_FILE ? "yes" : "no",
			ArgsRc::getDefaultDebugFile().c_str(),
			(unsigned long) ArgsRc::getDefaultDebugStart(),
			current.getAction() == ArgsRc::ACTION_DEBUG_FILE ? "yes" : "no",
			current.getDebugFile().c_str(),
			(unsigned long) current.getDebugStart()
		);
	}
	
	void ArgsDocRc::onVerboseDoc(ArgsRc &current) {
		printf(
"-v --verbose\n"
"Default=%d, Current=%d\n"
"Increase the verbosity by 1. The maximum value is 3.\n\n",
			ArgsRc::getDefaultVerbosity(),
			current.getVerbosity()
		);
	}
	
	void ArgsDocRc::onQuietDoc(ArgsRc &current) {
		printf(
"-q --quiet\n"
"Default=%d, Current=%d\n"
"Decrease the verbosity by 1. The minimum value is 0, and it means no output at \n"
"all.\n\n",
			ArgsRc::getDefaultVerbosity(),
			current.getVerbosity()
		);
	}
	
	void ArgsDocRc::onUserDoc(ArgsRc &current) {
		printf(
"-u --user <Username>\n"
"Default='%s', Current='%s'\n"
"Set the user name that will be used for the -e and -l options.\n\n",
			ArgsRc::getDefaultUser().c_str(),
			current.getUser().c_str()
		);
	}
	
	void ArgsDocRc::onHelpDoc(ArgsRc &current) {
		printf(
"-h -help --help\n"
"Default=%s, Current=%s\n"
"Display this help text.\n\n",
			ArgsRc::getDefaultAction() == ArgsRc::ACTION_HELP ? "yes" : "no",
			current.getAction() == ArgsRc::ACTION_HELP ? "yes" : "no"
		);
	}
}
