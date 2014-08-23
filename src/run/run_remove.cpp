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

#include "run_remove.hpp"
#include "../os/users.hpp"
#include "../os/file.hpp"
#include "../rpncron.hpp"

#include <cstdio>
#include <fstream>

namespace RC {
	namespace RunRemove {
		void runRemove(ArgsRct &args) {
			std::string path;
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
			
			if(!OS::File::remove(path)) {
				printf("The rpncrontab for user '%s' could not be removed!\n", user.c_str());
				return;
			}
		}
	}
}
