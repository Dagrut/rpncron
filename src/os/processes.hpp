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

#ifndef _HPP_RPNCRON_OS_PROCESSES_
#define _HPP_RPNCRON_OS_PROCESSES_

#include "exceptions.hpp"

#include "users.hpp"

#include <string>
#include <vector>

#define RPNCRON_OS_PROCESS_POPENPIPE_EXECUTION_ERROR 0x100

namespace RC {
	namespace OS {
		namespace Processes {
			typedef pid_t ID;
			int system(const std::string &cmd);
			ID fork();
			void daemonize(bool reset_pwd = true, bool close_io = true);
			void exec(
				const std::string &cmd,
				const std::vector<std::string> &args,
				const std::vector<std::string> *env = NULL
			);
			int popenPipe(
				const std::vector<char> &input,
				std::vector<char> &output,
				int max_output,
				const std::string &cmd,
				const std::vector<std::string> &args,
				const std::vector<std::string> *env = NULL
			);
			bool setuid(OS::Users::UserID id);
			bool setgid(OS::Users::GroupID id);
			ID getPPidOfPid(ID id);
			ID getPid();
			ID getPPid();
			void acceptChildDeath();
		}
	}
}

#endif
