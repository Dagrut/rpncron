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

#include "rpn_functions_comp.hpp"

#define THROW_UNDERFLOW() \
	throw RpnFunctions::UnderflowError(std::string("Not enough values to process function ") + __PRETTY_FUNCTION__);

#define THROW_DOMAIN() \
	throw RpnFunctions::DomainError(std::string("Invalid variables type when calling function ") + __PRETTY_FUNCTION__);

namespace RC {
	void RpnFunctionsComp::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("<",  fnLt);
		rpn.addFunction("<=", fnLe);
		rpn.addFunction(">",  fnGt);
		rpn.addFunction(">=", fnGe);
		rpn.addFunction("==", fnEq);
		rpn.addFunction("===", fnStrictEq);
		rpn.addFunction("!=", fnNe);
	}
	
#define COMPARE(_sign, _name) \
	void RpnFunctionsComp::_name(Rpn<RpnSimpleToken> *rpn, const std::string &key) { \
		Rpn<RpnSimpleToken>::QueueIterator v1; \
		Rpn<RpnSimpleToken>::QueueIterator v2; \
		 \
		if(rpn->queue.size() < 2) \
			THROW_UNDERFLOW() \
		 \
		v1 = rpn->queue.begin(); \
		v2 = v1; \
		v2++; \
		 \
		(*v1)->parse(); \
		(*v2)->parse(); \
		 \
		if((*v1)->getType() == RpnSimpleToken::TYPE_STRING || (*v2)->getType() == RpnSimpleToken::TYPE_STRING) { \
			long result = (*v2)->parseAsString() _sign (*v1)->parseAsString(); \
			delete *v1; \
			rpn->queue.pop_front(); \
			(*v2)->reset(result); \
		} \
		else if((*v1)->getType() == RpnSimpleToken::TYPE_INT && (*v2)->getType() == RpnSimpleToken::TYPE_INT) { \
			long result = (*v2)->parseAsInt() _sign (*v1)->parseAsInt(); \
			delete *v1; \
			rpn->queue.pop_front(); \
			(*v2)->reset(result); \
		} \
		else if((*v1)->getType() == RpnSimpleToken::TYPE_FLOAT || (*v2)->getType() == RpnSimpleToken::TYPE_FLOAT) { \
			long result = (*v2)->parseAsFloat() _sign (*v1)->parseAsFloat(); \
			delete *v1; \
			rpn->queue.pop_front(); \
			(*v2)->reset(result); \
		} \
	}
	
COMPARE(<,  fnLt)
COMPARE(<=, fnLe)
COMPARE(>,  fnGt)
COMPARE(>=, fnGe)
COMPARE(==, fnEq)
COMPARE(!=, fnNe)

	void RpnFunctionsComp::fnStrictEq(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		(*v1)->parse();
		(*v2)->parse();
		
		if((*v1)->getType() != (*v2)->getType()) {
			delete *v1;
			rpn->queue.pop_front();
			(*v2)->reset((long) 0);
		}
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_STRING && (*v2)->getType() == RpnSimpleToken::TYPE_STRING) {
			long result = (*v2)->parseAsString() == (*v1)->parseAsString();
			delete *v1;
			rpn->queue.pop_front();
			(*v2)->reset(result);
		}
		else if((*v1)->getType() == RpnSimpleToken::TYPE_INT && (*v2)->getType() == RpnSimpleToken::TYPE_INT) {
			long result = (*v2)->parseAsInt() == (*v1)->parseAsInt();
			delete *v1;
			rpn->queue.pop_front();
			(*v2)->reset(result);
		}
		else if((*v1)->getType() == RpnSimpleToken::TYPE_FLOAT && (*v2)->getType() == RpnSimpleToken::TYPE_FLOAT) {
			long result = (*v2)->parseAsFloat() == (*v1)->parseAsFloat();
			delete *v1;
			rpn->queue.pop_front();
			(*v2)->reset(result);
		}
	}
}
