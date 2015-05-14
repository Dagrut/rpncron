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

#ifndef _HPP_RPNCRON_ARGS_ARGS_RC_
#define _HPP_RPNCRON_ARGS_ARGS_RC_

#include "args_parser.hpp"

#include <string>

namespace RC {
	class ArgsRc {
		public:
			enum ConfAction {
				ACTION_HELP,
				ACTION_RUN,
				ACTION_RUN_FILE,
				ACTION_RUN_DIR,
				ACTION_DEBUG_FILE,
			};
			
			ArgsRc();
			
			void parse(int argc, char **argv);
			
			#define ITEM(exec_callback, argcnt, ...) \
				static void exec_callback( \
					ArgsParser *parser, \
					const std::string &key, \
					const std::vector<std::string> &values \
				);
			
			#include "args_rc.itm"
			
			static ConfAction getDefaultAction();
			static bool getDefaultDaemonize();
			static int getDefaultVerbosity();
			static std::string getDefaultUser();
			static std::string getDefaultDebugFile();
			static std::string getDefaultRunPath();
			static std::string getDefaultPidPath();
			static time_t getDefaultDebugStart();
			
			ConfAction getAction() const { return(this->action); }
			bool getDaemonize() const { return(this->daemonize); }
			int getVerbosity() const { return(this->verbosity); }
			std::string getUser() const { return(this->user); }
			std::string getDebugFile() const { return(this->debug_file); }
			std::string getRunPath() const { return(this->run_path); }
			std::string getPidPath() const { return(this->pid_path); }
			time_t getDebugStart() const { return(this->debug_start); }
			
		private:
			ConfAction action;
			bool daemonize;
			
			int verbosity;
			
			std::string user;
			
			std::string run_path;
			std::string pid_path;
			
			std::string debug_file;
			time_t debug_start;
	};
}

#endif
