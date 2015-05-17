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

#include "strings.hpp"

namespace RC {
	std::string Utils::Strings::strReplace(
		const std::string &from,
		const std::string &to,
		std::string data
	) {
		size_t pos = 0;
		
		while((pos = data.find(from, pos)) != std::string::npos) {
			data.replace(pos, from.length(), to);
			pos += to.length();
		}
		
		return(data);
	}
	
	std::string Utils::Strings::mapStrReplace(
		const std::map<std::string, std::string> &replacements,
		std::string data
	) {
		std::map<std::string, std::string>::const_iterator it;
		std::map<std::string, std::string>::const_iterator end;
		
		it = replacements.begin();
		end = replacements.end();
		
		for(; it != end ; it++) {
			data = strReplace(it->first, it->second, data);
		}
		
		return(data);
	}
}
