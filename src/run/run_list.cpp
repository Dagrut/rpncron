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

#include "run_list.hpp"
#include "../os/users.hpp"
#include "../os/file.hpp"
#include "../rpncron.hpp"

#include <cstdio>
#include <fstream>

namespace RC {
	namespace RunList {
		void runList(ArgsRct &args) {
			std::ifstream ifs;
			std::string path;
			char tmp;
			std::string user = args.getUser();
			
			try {
				OS::Users::userNameToUID(user);
			}
			catch(OS::SystemError &e) {
				printf("User '%s' unknown\n", user.c_str());
				return;
			}
			
			path = std::string(RPNCRON_PROGRAM_USERS_PATH) + '/' + user;
			OS::File file(path);
			if(!file.isFile()) {
				printf("No rpncrontab found for user '%s'\n", user.c_str());
				return;
			}
			
			ifs.open(path.c_str());
			if(!ifs.is_open()) {
				printf("Could not open rpncrontab file for user '%s'\n", user.c_str());
				return;
			}
			
			while(ifs.get(tmp)) {
				printf("%c", tmp);
			}
			
			if(!ifs.eof()) {
				printf("\n\nAn error was encountered while reading '%s'\n", user.c_str());
				return;
			}
		}
	}
}
