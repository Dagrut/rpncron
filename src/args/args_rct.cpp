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

#include "args_rct.hpp"
#include "args_parser.hpp"
#include "args_doc_rct.hpp"
#include "../os/users.hpp"
#include "../rpncron.hpp"
#include "../utils/strings.hpp"

#include <deque>
#include <string>

#include <cstdlib>
#include <strings.h>

namespace RC {
	ArgsRct::ArgsRct() :
		action(ArgsRct::getDefaultAction()),
		daemonize(ArgsRct::getDefaultDaemonize()),
		verbosity(ArgsRct::getDefaultVerbosity()),
		user(ArgsRct::getDefaultUser())
	{}
	
	ArgsRct::ConfAction ArgsRct::getDefaultAction() {  return(ArgsRct::ACTION_HELP); }
	bool ArgsRct::getDefaultDaemonize() {           return(false); }
	int ArgsRct::getDefaultVerbosity() {            return(1); }
	std::string ArgsRct::getDefaultUser() {         return(OS::Users::getCurrentUserName()); }
	std::string ArgsRct::getDefaultRunPath() {      return(""); }
	
	void ArgsRct::parse(int argc, char **argv) {
		ArgsParser parser;
		std::vector<int> unknown_args;
		
		parser.setData(this);
		
		#define ITEM(exec_callback, argcnt, ...) \
			parser.addOption( \
				exec_callback, \
				argcnt, \
				__VA_ARGS__, \
				NULL \
			);
		
		#include "args_rct.itm"
		
		parser.parse(argc, argv, unknown_args);
	}
	
	bool ArgsRct::check() {
		switch(this->action) {
			case ArgsRct::ACTION_EDIT:
			case ArgsRct::ACTION_REMOVE:
			case ArgsRct::ACTION_PRINT: {
				std::deque<std::string> split;
				std::deque<std::string>::iterator it;
				
				Utils::Strings::split(this->rct_file, "/", split);
				
				it = split.begin();
				while(it != split.end()) {
					if(it->size() == 0 || *it == ".") {
						it = split.erase(it);
					}
					else if(*it == "..") {
						if(it != split.begin()) {
							it--;
							it = split.erase(it);
						}
						it = split.erase(it);
					}
					else {
						it++;
					}
				}
				
				this->rct_file.erase();
				Utils::Strings::join(split, "/", this->rct_file);
			}
				break;
			default:
				break;
		}
		
		return(true);
	}
	
	void ArgsRct::onVerbosity(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->verbosity = atoi(values[0].c_str());
		if(args->verbosity < 0)
			args->verbosity = 0;
		else if(args->verbosity > 3)
			args->verbosity = 3;
	}
	
	void ArgsRct::onEdit(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->action = ACTION_EDIT;
		args->rct_file = values[0];
	}
	
	void ArgsRct::onList(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->action = ACTION_LIST;
	}
	
	void ArgsRct::onPrint(
		ArgsParser *parser,
		const std::string &key,
		const std::vector<std::string> &values
	) {
		ArgsRct *args = parser->getData<ArgsRct>();
		args->action = ACTION_PRINT;
		args->rct_file = values[0];
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
		args->rct_file = values[0];
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
