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

#include "run_edit.hpp"
#include "../os/users.hpp"
#include "../os/file.hpp"
#include "../os/processes.hpp"
#include "../os/environment.hpp"
#include "../rpncron.hpp"

#include <cstdio>
#include <fstream>

namespace RC {
	namespace RunEdit {
		void runEdit(ArgsRct &args) {
			std::ifstream ifs;
			std::string path;
			std::string tmp_path;
			std::string cmd;
			std::string user = args.getUser();
			int ret;
			
			try {
				OS::Users::userNameToUID(user);
			}
			catch(OS::SystemError &e) {
				printf("User '%s' unknown\n", user.c_str());
				return;
			}
			
			path = std::string(RPNCRON_PROGRAM_USERS_PATH) + '/' + user + '/' + args.getRctFile();
			OS::File file(path);
			if(!file.isFile()) {
				try {
					tmp_path = OS::File::tmpFileName();
				}
				catch(OS::SystemError &e) {
					printf("Error : %s\n", e.what());
					return;
				}
			}
			else {
				try {
					OS::File::dupToTmpFile(path, tmp_path);
				}
				catch(OS::SystemError &e) {
					printf("Could not copy '%s' to a temporary file : %s\n", path.c_str(), e.what());
					return;
				}
			}
			
			cmd = OS::Environment::get("EDITOR", RPNCRON_PROGRAM_EDITOR);
			cmd += " " + tmp_path;
			ret = OS::Processes::system(cmd);
			if(ret != 0) {
				printf(
					"The command %s returned code %d, so the crontab will not "
					"be installed. The file is kept in %s",
					cmd.c_str(), ret, tmp_path.c_str()
				);
				return;
			}
			
			try {
				OS::File::mkdir(path, true, true);
				OS::File::move(tmp_path, path);
			}
			catch(OS::SystemError &e) {
				printf("Could not move file from %s to %s : %s\n",
					tmp_path.c_str(), path.c_str(), e.what()
				);
				return;
			}
			
			OS::File::remove(tmp_path);
		}
	}
}
