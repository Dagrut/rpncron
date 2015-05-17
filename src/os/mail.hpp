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

#ifndef _HPP_RPNCRON_OS_MAIL_
#define _HPP_RPNCRON_OS_MAIL_

#include <sstream>
#include <map>

namespace RC {
	namespace OS {
		class Mail {
			public:
				void appendToBody(const std::string &str);
				void addHeader(const std::string &key, const std::string &val);
				void delHeader(const std::string &key, const std::string &val);
				void delHeaders(const std::string &key);
				std::string build();
				void send(const std::string &to);
				
			private:
				std::multimap<std::string, std::string> headers;
				std::ostringstream body;
		};
	}
}

#endif
