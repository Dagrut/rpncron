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

#include "users.hpp"

#include <unistd.h>
#include <sys/types.h>

namespace RC {
	std::string OS::Users::getCurrentUserName() {
		struct passwd *user_infos = getpwuid(getuid());
		if(user_infos == NULL)
			throw SystemError("Unknown current user!");
		return(user_infos->pw_name);
	}
	
	std::string OS::Users::getCurrentUserShell() {
		struct passwd *user_infos = getpwuid(getuid());
		if(user_infos == NULL)
			throw SystemError("Unknown current user!");
		return(user_infos->pw_shell);
	}
	
	OS::Users::UserID OS::Users::getCurrentUserID() {
		return(getuid());
	}
	
	OS::Users::GroupID OS::Users::getCurrentGroupID() {
		struct passwd *user_infos = getpwuid(getuid());
		if(user_infos == NULL)
			throw SystemError("Unknown current user!");
		return(user_infos->pw_gid);
	}
	
	OS::Users::UserID OS::Users::userNameToUID(const std::string &name) {
		struct passwd *user_infos = getpwnam(name.c_str());
		if(user_infos == NULL)
			throw SystemError("Unknown given user!");
		return(user_infos->pw_uid);
	}
	
	OS::Users::UserID OS::Users::groupNameToGID(const std::string &name) {
		struct group *group_infos = getgrnam(name.c_str());
		if(group_infos == NULL)
			throw SystemError("Unknown group user!");
		return(group_infos->gr_gid);
	}
}
