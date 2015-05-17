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

#ifndef _HPP_RPNCRON_OS_EXCEPTIONS_
#define _HPP_RPNCRON_OS_EXCEPTIONS_

#include <string>

#include <exception>

namespace RC {
	namespace OS {
		class SystemError : public std::exception {
			public:
				SystemError(const std::string &e) : e(e) {}
				virtual ~SystemError() throw() {}
				
				const char* what() const throw() {
					return(this->e.c_str());
				}
				
			private:
				const std::string e;
		};
		
		class SystemWarning : public std::exception {
			public:
				SystemWarning(const std::string &e) : e(e) {}
				virtual ~SystemWarning() throw() {}
				
				const char* what() const throw() {
					return(this->e.c_str());
				}
				
			private:
				const std::string e;
		};
	}
}

#endif
