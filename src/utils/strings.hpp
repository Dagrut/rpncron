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

#ifndef _HPP_RPNCRON_UTILS_TEMPLATE_
#define _HPP_RPNCRON_UTILS_TEMPLATE_

#include <string>
#include <map>
#include <vector>

namespace RC {
	namespace Utils {
		namespace Strings {
			std::string strReplace(
				const std::string &from,
				const std::string &to,
				std::string data
			);
			std::string mapStrReplace(
				const std::map<std::string, std::string> &replacements,
				std::string data
			);
			
			template <typename container>
			void split(const std::string &str, const std::string &sep, container &vec) {
				size_t prev = 0;
				size_t pos = 0;
				
				while((pos = str.find(sep, prev)) != std::string::npos) {
					vec.push_back(str.substr(prev, pos - prev));
					prev = pos + sep.size();
				}
				
				vec.push_back(str.substr(prev));
			}
			
			template <typename container>
			void join(const container &vec, const std::string &sep, std::string &str) {
				typename container::const_iterator it;
				typename container::const_iterator end;
				
				it = vec.begin();
				end = vec.end();
				
				for(; it != end ; it++) {
					str += *it;
					str += sep;
				}
				
				if(vec.size() > 0)
					str.replace(str.size() - sep.size(), sep.size(), "");
			}
		};
	}
}

#endif
