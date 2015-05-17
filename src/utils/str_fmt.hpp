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

#ifndef _HPP_RPNCRON_UTILS_STRFMT_
#define _HPP_RPNCRON_UTILS_STRFMT_

#include <string>

#include <cstdio>

namespace RC {
	namespace Utils {
		template <typename T>
		std::string format(const char *fmt, T val) {
			char buffer[1024];
			snprintf(buffer, sizeof(buffer), fmt, val);
			return(buffer);
		}
		
		template <typename T>
		T parse(const char *fmt, const std::string &input) {
			T val;
			sscanf(input.c_str(), fmt, &val);
			return(val);
		}
	}
}

#endif
