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

#include "environment.hpp"

#include <cstdlib>

#include <unistd.h>

namespace RC {
	std::string OS::Environment::get(const std::string &key, const std::string &dft_val) {
		char *val = getenv(key.c_str());
		if(val)
			return(val);
		return(dft_val);
	}
	
	bool OS::Environment::exists(const std::string &key) {
		if(getenv(key.c_str()))
			return(true);
		return(false);
	}
	
	bool OS::Environment::set(const std::string &key, const std::string &val, bool rewrite) {
		return(setenv(key.c_str(), val.c_str(), rewrite ? 1 : 0) == 0);
	}
	
	bool OS::Environment::unset(const std::string &key) {
		return(unsetenv(key.c_str()) == 0);
	}
	
	bool OS::Environment::setCwd(const std::string &cwd) {
		return(chdir(cwd.c_str()) == 0);
	}
}
