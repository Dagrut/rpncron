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

#include "logs.hpp"

#include <cstdio>

#include <syslog.h>

namespace RC {
	namespace OS {
		Logs *Logs::self = NULL;
		
		Logs::Logs() : 
			verbosity(Logs::LVL_INFO),
			log_dest(Logs::LOG_TO_SYSLOG)
		{
			openlog(NULL, LOG_CONS | LOG_PID, LOG_CRON);
		}
		
		Logs::~Logs() {
			closelog();
		}
		
		Logs *Logs::singleton() {
			if(Logs::self == NULL) {
				Logs::self = new Logs();
			}
			return(Logs::self);
		}
		
		void Logs::setVerbosity(int verbosity) {
			Logs::singleton()->verbosity = verbosity;
		}
		
		int Logs::getVerbosity() {
			return(Logs::singleton()->verbosity);
		}
		
		void Logs::log(int lvl, const std::string &log_str) {
			Logs *self = Logs::singleton();
			if(lvl >= self->verbosity)
				return;
			
			if(self->log_dest == Logs::LOG_TO_FILE) {
				printf("FIXME %s:%d\n", __FILE__, __LINE__);
			}
			else if(self->log_dest == Logs::LOG_TO_SYSLOG) {
				if(lvl == 0)
					lvl = LOG_ERR;
				else if(lvl == 1)
					lvl = LOG_WARNING;
				else
					lvl = LOG_NOTICE;
				syslog(lvl, "%s", log_str.c_str());
			}
			else if(self->log_dest == Logs::LOG_TO_REMOTE_SYSLOG) {
				printf("FIXME %s:%d\n", __FILE__, __LINE__);
			}
		}
		
		void Logs::log(int lvl, const char *fmt, ...) {
			char buffer[RPNCRON_LOGS_LOGGER_BUFFER_SIZE];
			va_list valist;
			
			if(lvl >= Logs::singleton()->verbosity)
				return;
			
			va_start(valist, fmt);
			
			vsnprintf(buffer, sizeof(buffer), fmt, valist);
			
			Logs::log(lvl, std::string(buffer));
		}
	}
}
