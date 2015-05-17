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

#ifndef _HPP_RPNCRON_RPN_FUNCTIONS_
#define _HPP_RPNCRON_RPN_FUNCTIONS_

#include <exception>

#ifdef RPNCRON_RPN_FUNCTIONS_INCLUDE_ALL
#undef RPNCRON_RPN_FUNCTIONS_INCLUDE_ALL
#include "src/rpn/functions/rpn_functions_arith_elem.hpp"
#include "src/rpn/functions/rpn_functions_branching.hpp"
#include "src/rpn/functions/rpn_functions_comp.hpp"
#include "src/rpn/functions/rpn_functions.hpp"
#include "src/rpn/functions/rpn_functions_logic.hpp"
#include "src/rpn/functions/rpn_functions_misc.hpp"
#include "src/rpn/functions/rpn_functions_queue.hpp"
#include "src/rpn/functions/rpn_functions_time.hpp"
#include "src/rpn/functions/rpn_functions_types.hpp"
#endif

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
