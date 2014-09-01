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

#ifndef _HPP_RPNCRON_UTILS_VIXIE_CRON_
#define _HPP_RPNCRON_UTILS_VIXIE_CRON_

#include <vector>
#include <string>

namespace RC {
	namespace Utils {
		namespace VixieCron {
			struct Entry {
				public:
					Entry();
					
					bool parseString(const std::string &in);
					bool parseKeyword(const std::string &kw);
					bool parseFields(const std::string &fields);
					bool parseField(
						const std::string &field,
						std::vector<bool> &out,
						int min = 0
					);
					bool parseSubfield(
						const std::string &field,
						std::vector<bool> &out,
						int min = 0
					);
					
					std::vector<bool> minutes;
					std::vector<bool> hours;
					std::vector<bool> doms;
					std::vector<bool> months;
					std::vector<bool> dows;
					bool reboot;
			};
		};
	}
}

#endif
