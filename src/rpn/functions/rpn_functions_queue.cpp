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

#include "rpn_functions_queue.hpp"

namespace RC {
	void RpnFunctionsQueue::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("swap()",        fnSwap);
		rpn.addFunction("pop(head)",     fnPopHead);
		rpn.addFunction("pop(tail)",     fnPopTail);
		rpn.addFunction("rotate(pop)",   fnRotatePop);
		rpn.addFunction("rotate(push)",  fnRotatePush);
		rpn.addFunction("reverse()",     fnReverse);
		rpn.addFunction("clear()",       fnClear);
	}
	
	void RpnFunctionsQueue::fnSwap(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *p1;
		RpnSimpleToken *p2;
		
		if(rpn->queue.size() < 2)
			return;
		
		p1 = rpn->queue.front();
		rpn->queue.pop_front();
		p2 = rpn->queue.front();
		rpn->queue.pop_front();
		
		rpn->queue.push_front(p1);
		rpn->queue.push_front(p2);
	}
	
	void RpnFunctionsQueue::fnPopHead(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *p1;
		
		if(rpn->queue.size() < 1)
			return;
		
		p1 = rpn->queue.front();
		rpn->queue.pop_front();
		
		delete p1;
	}
	
	void RpnFunctionsQueue::fnPopTail(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *p1;
		
		if(rpn->queue.size() < 1)
			return;
		
		p1 = rpn->queue.back();
		rpn->queue.pop_back();
		
		delete p1;
	}
	
	void RpnFunctionsQueue::fnRotatePop(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *p1;
		
		if(rpn->queue.size() < 2)
			return;
		
		p1 = rpn->queue.front();
		rpn->queue.pop_front();
		
		rpn->queue.push_back(p1);
	}
	
	void RpnFunctionsQueue::fnRotatePush(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *p1;
		
		if(rpn->queue.size() < 2)
			return;
		
		p1 = rpn->queue.back();
		rpn->queue.pop_back();
		rpn->queue.push_front(p1);
	}
	
	void RpnFunctionsQueue::fnReverse(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::Queue queue2;
		
		if(rpn->queue.size() < 2)
			return;
		
		rpn->queue.swap(queue2);
		
		rpn->queue.insert(rpn->queue.end(), queue2.rbegin(), queue2.rend());
	}
	
	void RpnFunctionsQueue::fnClear(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		rpn->queue.clear();
	}
}
