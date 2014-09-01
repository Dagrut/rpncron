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

#include "args/args_rc.hpp"
#include "args/args_doc_rc.hpp"
#include "run/run_debug.hpp"
#include "run/run_list.hpp"
#include "run/run_remove.hpp"
#include "run/run_edit.hpp"
#include "run/run_rpncron.hpp"

#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
	RC::ArgsRc args;
	
	args.parse(argc, argv);
	
	switch(args.getAction()) {
		case RC::ArgsRc::ACTION_HELP:
			RC::ArgsDocRc::showAllDocumentation(args);
			break;
		case RC::ArgsRc::ACTION_RUN:
			RC::RunRpncron::run(args);
			break;
		case RC::ArgsRc::ACTION_RUN_FILE:
			RC::RunRpncron::runFile(args);
			break;
		case RC::ArgsRc::ACTION_RUN_DIR:
			RC::RunRpncron::runDir(args);
			break;
		case RC::ArgsRc::ACTION_DEBUG_FILE:
			RC::RunDebug::runDebug(args);
			break;
	}
	
	return(EXIT_SUCCESS);
}
