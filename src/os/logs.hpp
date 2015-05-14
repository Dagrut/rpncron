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

#ifndef _HPP_RPNCRON_OS_LOGS_
#define _HPP_RPNCRON_OS_LOGS_

#include <string>

#include <stdarg.h>

#define RPNCRON_LOGS_LOGGER_BUFFER_SIZE     1024

namespace RC {
	namespace OS {
		class Logs {
			public:
				enum LogDest {
					LOG_TO_FILE,
					LOG_TO_SYSLOG,
					LOG_TO_REMOTE_SYSLOG,
				};
				
				enum LogLevel {
					LVL_CRIT = 0,
					LVL_WARN = 1,
					LVL_INFO = 2,
				};
				
				static void setVerbosity(int verbosity);
				static int getVerbosity();
				static void log(int lvl, const std::string &log_str);
				static void log(int lvl, const char *fmt, ...);
				
			private:
				Logs();
				~Logs();
				static Logs *singleton();
				
				static Logs *self;
				int verbosity;
				LogDest log_dest;
				std::string dest_file;
				std::string dest_syslog;
		};
	}
}

#endif
