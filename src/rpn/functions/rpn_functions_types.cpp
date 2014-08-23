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

#include "rpn_functions_types.hpp"

#define THROW_UNDERFLOW() \
	throw RpnFunctions::UnderflowError(std::string("Not enough values to process function ") + __PRETTY_FUNCTION__);

#define THROW_DOMAIN() \
	throw RpnFunctions::DomainError(std::string("Invalid variables type when calling function ") + __PRETTY_FUNCTION__);

namespace RC {
	void RpnFunctionsTypes::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("int()",     fnInt);
		rpn.addFunction("float()",   fnFloat);
		rpn.addFunction("string()",  fnString);
		rpn.addFunction("bool()",    fnBool);
		rpn.addFunction("!!",        fnBool);
		rpn.addFunction("void()",    fnVoid);
	}
	
	void RpnFunctionsTypes::fnInt(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		
		(*v1)->parseAsInt();
	}
	
	void RpnFunctionsTypes::fnFloat(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		
		(*v1)->parseAsFloat();
	}
	
	void RpnFunctionsTypes::fnString(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		
		(*v1)->parseAsString();
	}
	
	void RpnFunctionsTypes::fnBool(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		
		(*v1)->reset((long) RpnSimpleToken::getBoolValue(*v1));
	}
	
	void RpnFunctionsTypes::fnVoid(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		(void) rpn;
		(void) key;
	}
}
