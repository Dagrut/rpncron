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

#ifndef _HPP_RPNCRON_ARGS_ARGS_RCT_
#define _HPP_RPNCRON_ARGS_ARGS_RCT_

#include "args_parser.hpp"

#include <string>

namespace RC {
	class ArgsRct {
		public:
			enum ConfAction {
				ACTION_HELP,
				ACTION_EDIT,
				ACTION_LIST,
				ACTION_REMOVE,
			};
			
			ArgsRct();
			
			void parse(int argc, char **argv);
			
			#define CALLBACK(name) \
				static void name( \
					ArgsParser *parser, \
					const std::string &key, \
					const std::vector<std::string> &values \
				);
			
			CALLBACK(onVerbose)
			CALLBACK(onQuiet)
			CALLBACK(onEdit)
			CALLBACK(onList)
			CALLBACK(onUser)
			CALLBACK(onRemove)
			CALLBACK(onHelp)
			
			#undef CALLBACK
			
			static ConfAction getDefaultAction();
			static bool getDefaultDaemonize();
			static int getDefaultVerbosity();
			static std::string getDefaultUser();
			static std::string getDefaultDebugFile();
			static std::string getDefaultRunPath();
			static time_t getDefaultDebugStart();
			
			ConfAction getAction() const { return(this->action); }
			bool getDaemonize() const { return(this->daemonize); }
			int getVerbosity() const { return(this->verbosity); }
			std::string getUser() const { return(this->user); }
			std::string getDebugFile() const { return(this->debug_file); }
			std::string getRunPath() const { return(this->run_path); }
			time_t getDebugStart() const { return(this->debug_start); }
			
		private:
			ConfAction action;
			bool daemonize;
			
			int verbosity;
			
			std::string user;
			
			std::string run_path;
			
			std::string debug_file;
			time_t debug_start;
	};
}

#endif
