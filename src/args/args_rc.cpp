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

#include "args_rc.hpp"
#include "args_parser.hpp"
#include "args_doc_rc.hpp"
#include "../os/users.hpp"
#include "../rpncron.hpp"

#include <cstdlib>
#include <ctime>
#include <strings.h>

namespace RC {
	ArgsRc::ArgsRc() :
		action(ArgsRc::getDefaultAction()),
		daemonize(ArgsRc::getDefaultDaemonize()),
		verbosity(ArgsRc::getDefaultVerbosity()),
		user(ArgsRc::getDefaultUser()),
		debug_file(ArgsRc::getDefaultDebugFile()),
		debug_start(ArgsRc::getDefaultDebugStart())
	{}
	
	ArgsRc::ConfAction ArgsRc::getDefaultAction() {  return(ArgsRc::ACTION_HELP); }
	bool ArgsRc::getDefaultDaemonize() {           return(false); }
	int ArgsRc::getDefaultVerbosity() {            return(1); }
	std::string ArgsRc::getDefaultUser() {         return(OS::Users::getCurrentUserName()); }
	std::string ArgsRc::getDefaultDebugFile() {    return(""); }
	std::string ArgsRc::getDefaultRunPath() {      return(""); }
	time_t ArgsRc::getDefaultDebugStart() {        return(time(NULL)); }
	
	void ArgsRc::parse(int argc, char **argv) {
		ArgsParser parser;
		std::vector<int> unknown_args;
		
		parser.setData(this);
		
		#define ITEM(name, argcnt, exec_callback) \
			parser.addOption( \
				name, \
				exec_callback, \
				argcnt \
			);
		
		#include "args_rc.itm"
		
		parser.parse(argc, argv, unknown_args);
	}
	
	void ArgsRc::onDaemon(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		args->daemonize = !args->daemonize;
	}
	
	void ArgsRc::onRun(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		args->action = ACTION_RUN;
	}
	
	void ArgsRc::onRunFile(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		args->action = ACTION_RUN_FILE;
		args->run_path = values[0];
	}
	
	void ArgsRc::onRunDir(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		args->action = ACTION_RUN_DIR;
		args->run_path = values[0];
	}
	
	void ArgsRc::onDebugFile(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		args->debug_file = values[0];
		if(strcasecmp(values[1].c_str(), "now") == 0)
			time(&args->debug_start);
		else
			args->debug_start = atol(values[1].c_str());
		args->action = ACTION_DEBUG_FILE;
	}
	
	void ArgsRc::onVerbose(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		if(args->verbosity < 3)
			args->verbosity++;
	}
	
	void ArgsRc::onQuiet(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		if(args->verbosity > 0)
			args->verbosity--;
	}
	
	void ArgsRc::onUser(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		args->user = values[0];
	}
	
	void ArgsRc::onHelp(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRc *args = parser->getData<ArgsRc>();
		args->action = ACTION_HELP;
		parser->stopParser();
	}
}
