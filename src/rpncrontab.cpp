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

#include "args/args_rct.hpp"
#include "args/args_doc_rct.hpp"
#include "run/run_debug.hpp"
#include "run/run_list.hpp"
#include "run/run_remove.hpp"
#include "run/run_edit.hpp"
#include "run/run.hpp"

#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
	RC::ArgsRct args;
	
	args.parse(argc, argv);
	
	switch(args.getAction()) {
		case RC::ArgsRct::ACTION_HELP:
			RC::ArgsDocRct::showAllDocumentation(args);
			break;
		case RC::ArgsRct::ACTION_EDIT:
			RC::RunEdit::runEdit(args);
			break;
		case RC::ArgsRct::ACTION_LIST:
			RC::RunList::runList(args);
			break;
		case RC::ArgsRct::ACTION_REMOVE:
			RC::RunRemove::runRemove(args);
			break;
	}
	
	return(EXIT_SUCCESS);
}
