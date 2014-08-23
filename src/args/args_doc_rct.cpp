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

#include "args_doc_rct.hpp"
#include "args_rc.hpp"
#include "../rpncron.hpp"

#include <cstdio>

namespace RC {
	void ArgsDocRct::showAllDocumentation(ArgsRct &current) {
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
		
		CALL(onVerbose)
		CALL(onQuiet)
		CALL(onEdit)
		CALL(onList)
		CALL(onUser)
		CALL(onRemove)
		CALL(onHelp)
		
		#undef CALL
	}
	
	void ArgsDocRct::onVerboseDoc(ArgsRct &current) {
		printf(
"-v --verbose\n"
"Default=%d, Current=%d\n"
"Increase the verbosity by 1. The maximum value is 3.\n\n",
			ArgsRct::getDefaultVerbosity(),
			current.getVerbosity()
		);
	}
	
	void ArgsDocRct::onQuietDoc(ArgsRct &current) {
		printf(
"-q --quiet\n"
"Default=%d, Current=%d\n"
"Decrease the verbosity by 1. The minimum value is 0, and it means no output at \n"
"all.\n\n",
			ArgsRct::getDefaultVerbosity(),
			current.getVerbosity()
		);
	}
	
	void ArgsDocRct::onEditDoc(ArgsRct &current) {
		printf(
"-e --edit\n"
"Default=%s, Current=%s\n"
"Edit the rpncron file of the user given by -u.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_EDIT ? "yes" : "no",
			current.getAction() == ArgsRct::ACTION_EDIT ? "yes" : "no"
		);
	}
	
	void ArgsDocRct::onListDoc(ArgsRct &current) {
		printf(
"-l --list\n"
"Default=%s, Current=%s\n"
"Shows the content of the rpncron file for the user given by -u.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_LIST ? "yes" : "no",
			current.getAction() == ArgsRct::ACTION_LIST ? "yes" : "no"
		);
	}
	
	void ArgsDocRct::onUserDoc(ArgsRct &current) {
		printf(
"-u --user <Username>\n"
"Default='%s', Current='%s'\n"
"Set the user name that will be used for the -e and -l options.\n\n",
			ArgsRct::getDefaultUser().c_str(),
			current.getUser().c_str()
		);
	}
	
	void ArgsDocRct::onRemoveDoc(ArgsRct &current) {
		printf(
"-r --remove\n"
"Default=%s, Current=%s\n"
"Remove the rpncron file of the user given by -u.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_REMOVE ? "yes" : "no",
			current.getAction() == ArgsRct::ACTION_REMOVE ? "yes" : "no"
		);
	}
	
	void ArgsDocRct::onHelpDoc(ArgsRct &current) {
		printf(
"-h -help --help\n"
"Default=%s, Current=%s\n"
"Display this help text.\n\n",
			ArgsRct::getDefaultAction() == ArgsRct::ACTION_HELP ? "yes" : "no",
			current.getAction() == ArgsRct::ACTION_HELP ? "yes" : "no"
		);
	}
}
