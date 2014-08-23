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

#ifndef _HPP_RPNCRON_RPN_FUNCTIONS_QUEUE_
#define _HPP_RPNCRON_RPN_FUNCTIONS_QUEUE_

#include "../rpn_simple_token.hpp"
#include "../rpn.hpp"

namespace RC {
	namespace RpnFunctionsQueue {
		void load(Rpn<RpnSimpleToken> &rpn);
		
		void fnSwap(Rpn<RpnSimpleToken> *rpn, const std::string &key);
		void fnPopHead(Rpn<RpnSimpleToken> *rpn, const std::string &key);
		void fnPopTail(Rpn<RpnSimpleToken> *rpn, const std::string &key);
		void fnRotatePop(Rpn<RpnSimpleToken> *rpn, const std::string &key);
		void fnRotatePush(Rpn<RpnSimpleToken> *rpn, const std::string &key);
		void fnReverse(Rpn<RpnSimpleToken> *rpn, const std::string &key);
		void fnClear(Rpn<RpnSimpleToken> *rpn, const std::string &key);
	}
}

#endif
