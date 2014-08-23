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

#ifndef _HPP_RPNCRON_UTILS_TEMPLATE_
#define _HPP_RPNCRON_UTILS_TEMPLATE_

#include <string>
#include <map>

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
		};
	}
}

#endif
