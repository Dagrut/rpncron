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

#include "run_remove.hpp"
#include "../os/users.hpp"
#include "../os/file.hpp"
#include "../os/directory.hpp"
#include "../rpncron.hpp"

#include <cstdio>
#include <fstream>

namespace RC {
	namespace RunRemove {
		void runRemove(ArgsRct &args) {
			std::string path;
			std::string base;
			std::string user = args.getUser();
			
			try {
				OS::Users::userNameToUID(user);
			}
			catch(OS::SystemError &e) {
				printf("User '%s' unknown\n", user.c_str());
				return;
			}
			
			base = std::string(RPNCRON_PROGRAM_USERS_PATH) + '/' + user;
			path = base + '/' + args.getRctFile();
			OS::File file(path);
			if(!file.isFile()) {
				printf("The rpncrontab file %s was not found for user '%s'\n", args.getRctFile().c_str(), user.c_str());
				return;
			}
			
			if(!OS::File::remove(path)) {
				printf("The rpncrontab file %s for user '%s' could not be removed!\n", args.getRctFile().c_str(), user.c_str());
				return;
			}
			
			for(
				path = path.substr(0, path.rfind('/')) ;
				path != base ;
				path = path.substr(0, path.rfind('/'))
			) {
				try {
					if(OS::Directory::isEmpty(path)) {
						if(!OS::Directory::remove(path)) {
							printf("Warning: could not clean empty directory %s for rpncrontab of user %s\n", path.c_str(), user.c_str());
							return;
						}
					}
				}
				catch(OS::SystemError &e) {
					printf("Error when cleaning empty directory %s for rpncrontab of user %s : %s\n", path.c_str(), user.c_str(), e.what());
					return;
				}
			}
		}
	}
}
