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

#include "args_rct.hpp"
#include "args_parser.hpp"
#include "args_doc_rct.hpp"
#include "../os/users.hpp"
#include "../rpncron.hpp"

#include <cstdlib>
#include <ctime>
#include <strings.h>

namespace RC {
	ArgsRct::ArgsRct() :
		action(ArgsRct::getDefaultAction()),
		daemonize(ArgsRct::getDefaultDaemonize()),
		verbosity(ArgsRct::getDefaultVerbosity()),
		user(ArgsRct::getDefaultUser()),
		debug_file(ArgsRct::getDefaultDebugFile()),
		debug_start(ArgsRct::getDefaultDebugStart())
	{}
	
	ArgsRct::ConfAction ArgsRct::getDefaultAction() {  return(ArgsRct::ACTION_HELP); }
	bool ArgsRct::getDefaultDaemonize() {           return(false); }
	int ArgsRct::getDefaultVerbosity() {            return(1); }
	std::string ArgsRct::getDefaultUser() {         return(OS::Users::getCurrentUserName()); }
	std::string ArgsRct::getDefaultDebugFile() {    return(""); }
	std::string ArgsRct::getDefaultRunPath() {      return(""); }
	time_t ArgsRct::getDefaultDebugStart() {        return(time(NULL)); }
	
	void ArgsRct::parse(int argc, char **argv) {
		ArgsParser parser;
		std::vector<int> unknown_args;
		
		parser.setData(this);
		
		#define ITEM(name, argcnt, exec_callback) \
			parser.addOption( \
				name, \
				exec_callback, \
				argcnt \
			);
		
		#include "args_rct.itm"
		
		parser.parse(argc, argv, unknown_args);
	}
	
	void ArgsRct::onVerbose(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		if(args->verbosity < 3)
			args->verbosity++;
	}
	
	void ArgsRct::onQuiet(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		if(args->verbosity > 0)
			args->verbosity--;
	}
	
	void ArgsRct::onEdit(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->action = ACTION_EDIT;
	}
	
	void ArgsRct::onList(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->action = ACTION_LIST;
	}
	
	void ArgsRct::onUser(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->user = values[0];
	}
	
	void ArgsRct::onRemove(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->action = ACTION_REMOVE;
	}
	
	void ArgsRct::onHelp(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->action = ACTION_HELP;
		parser->stopParser();
	}
}
