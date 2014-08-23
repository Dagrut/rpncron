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

#include "rpn_functions_logic.hpp"

#define THROW_UNDERFLOW() \
	throw RpnFunctions::UnderflowError(std::string("Not enough values to process function ") + __PRETTY_FUNCTION__);

#define THROW_DOMAIN() \
	throw RpnFunctions::DomainError(std::string("Invalid variables type when calling function ") + __PRETTY_FUNCTION__);

namespace RC {
	void RpnFunctionsLogic::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("&&",     fnAnd);
		rpn.addFunction("and()",  fnAnd);
		rpn.addFunction("||",     fnOr);
		rpn.addFunction("or()",   fnOr);
		rpn.addFunction("!",      fnNot);
		rpn.addFunction("not()",  fnNot);
		rpn.addFunction("&&(*)",  fnAndStar);
		rpn.addFunction("and(*)", fnAndStar);
		rpn.addFunction("||(*)",  fnOrStar);
		rpn.addFunction("or(*)",  fnOrStar);
	}
	
	void RpnFunctionsLogic::fnAnd(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		long result = RpnSimpleToken::getBoolValue(*v1) && RpnSimpleToken::getBoolValue(*v2);
		delete *v1;
		rpn->queue.pop_front();
		(*v2)->reset(result);
	}
	
	void RpnFunctionsLogic::fnOr(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		long result = RpnSimpleToken::getBoolValue(*v1) || RpnSimpleToken::getBoolValue(*v2);
		delete *v1;
		rpn->queue.pop_front();
		(*v2)->reset(result);
	}
	
	void RpnFunctionsLogic::fnNot(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		
		long result = ! RpnSimpleToken::getBoolValue(*v1);
		(*v1)->reset(result);
	}
	
	void RpnFunctionsLogic::fnAndStar(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator it;
		Rpn<RpnSimpleToken>::QueueIterator end;
		RpnSimpleToken *first;
		bool result;
		
		if(rpn->queue.size() == 0)
			THROW_UNDERFLOW()
		
		first = NULL;
		it = rpn->queue.begin();
		end = rpn->queue.end();
		result = true;
		for(; it != end ; it++) {
			result = result && RpnSimpleToken::getBoolValue(*it);
			
			if(first == NULL)
				first = *it;
			else
				delete *it;
		}
		
		first->reset((long) result);
		rpn->queue.clear();
		rpn->queue.push_back(first);
	}
	
	void RpnFunctionsLogic::fnOrStar(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator it;
		Rpn<RpnSimpleToken>::QueueIterator end;
		RpnSimpleToken *first;
		bool result;
		
		if(rpn->queue.size() == 0)
			THROW_UNDERFLOW()
		
		first = NULL;
		it = rpn->queue.begin();
		end = rpn->queue.end();
		result = false;
		for(; it != end ; it++) {
			result = result || RpnSimpleToken::getBoolValue(*it);
			
			if(first == NULL)
				first = *it;
			else
				delete *it;
		}
		
		first->reset((long) result);
		rpn->queue.clear();
		rpn->queue.push_back(first);
	}
}
