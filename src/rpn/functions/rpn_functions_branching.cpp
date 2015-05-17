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

#include "rpn_functions_branching.hpp"

#include <stdexcept>
#include <cmath>

#define THROW_UNDERFLOW() \
	throw RpnFunctions::UnderflowError(std::string("Not enough values to process function ") + __PRETTY_FUNCTION__);

#define THROW_DOMAIN() \
	throw RpnFunctions::DomainError(std::string("Invalid variables type when calling function ") + __PRETTY_FUNCTION__);

namespace RC {
	void RpnFunctionsBranching::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("if()",     fnIf);
		rpn.addFunction("ifelse()", fnIfElse);
		rpn.addFunction("switch()", fnSwitch);
	}
	
	void RpnFunctionsBranching::fnIf(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		bool ret;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		ret = RpnSimpleToken::getBoolValue(*v1);
		
		delete *v1;
		rpn->queue.pop_front();
		
		if(ret == false) {
			delete *v2;
			rpn->queue.pop_front();
		}
	}
	
	void RpnFunctionsBranching::fnIfElse(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		Rpn<RpnSimpleToken>::QueueIterator v3;
		bool ret;
		
		if(rpn->queue.size() < 3)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		v3 = v2;
		v3++;
		
		ret = RpnSimpleToken::getBoolValue(*v1);
		
		delete *v1;
		rpn->queue.pop_front();
		
		if(ret == true) {
			delete *v3;
			rpn->queue.erase(v3);
		}
		else {
			delete *v2;
			rpn->queue.pop_front();
		}
	}
	
	void RpnFunctionsBranching::fnSwitch(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator it;
		RpnSimpleToken* switch_val;
		long ret;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		switch_val = rpn->queue.front();
		rpn->queue.pop_front();
		
		if(rpn->queue.size() == 0) {
			delete switch_val;
			return;
		}
		
		ret = switch_val->parseAsInt();
		
		ret = ret % rpn->queue.size();
		ret = ret < 0 ? ret + rpn->queue.size() : ret;
		
		it = rpn->queue.begin();
		for(int i = 0 ; it != rpn->queue.end() ; i++) {
			if(i == ret) {
				it++;
			}
			else {
				delete *it;
				it = rpn->queue.erase(it);
			}
		}
		
		delete switch_val;
	}
}
