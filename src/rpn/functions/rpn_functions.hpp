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

#ifndef _HPP_RPNCRON_RPN_FUNCTIONS_
#define _HPP_RPNCRON_RPN_FUNCTIONS_

#include <exception>

namespace RC {
	namespace RpnFunctions {
		class UnderflowError : public std::exception {
			public:
				UnderflowError(const std::string &e) : e(e) {}
				virtual ~UnderflowError() throw() {}
				
				const char* what() const throw() {
					return(this->e.c_str());
				}
				
			private:
				const std::string e;
		};
		
		class DomainError : public std::exception {
			public:
				DomainError(const std::string &e) : e(e) {}
				~DomainError() throw() {}
				
				const char* what() const throw() {
					return(this->e.c_str());
				}
				
			private:
				const std::string e;
		};
	}
}

#endif
