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

#include "rpn/functions/rpn_functions_arith_elem.hpp"

#include <stdexcept>
#include <cmath>

#define THROW_UNDERFLOW() \
	throw UnderflowError(std::string("Not enough values to process function ") + __PRETTY_FUNCTION__);

#define THROW_DOMAIN() \
	throw DomainError(std::string("Invalid variables type when calling function ") + __PRETTY_FUNCTION__);

namespace RC {
	void RpnFunctionsArithElem::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("+", addition);
		rpn.addFunction("-", substraction);
		rpn.addFunction("*", multiplication);
		rpn.addFunction("/", division);
		rpn.addFunction("%", modulo);
	}
	
	void RpnFunctionsArithElem::addition(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		(*v1)->parse();
		(*v2)->parse();
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_STRING || (*v2)->getType() == RpnSimpleToken::TYPE_STRING)
			THROW_DOMAIN()
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_INT && (*v2)->getType() == RpnSimpleToken::TYPE_INT) {
			long result = (*v1)->parseAsInt() + (*v2)->parseAsInt();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
		else if((*v1)->getType() == RpnSimpleToken::TYPE_FLOAT || (*v2)->getType() == RpnSimpleToken::TYPE_FLOAT) {
			double result = (*v1)->parseAsFloat() + (*v2)->parseAsFloat();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
	}
	
	void RpnFunctionsArithElem::substraction(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		(*v1)->parse();
		(*v2)->parse();
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_STRING || (*v2)->getType() == RpnSimpleToken::TYPE_STRING)
			THROW_DOMAIN()
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_INT && (*v2)->getType() == RpnSimpleToken::TYPE_INT) {
			long result = (*v2)->parseAsInt() - (*v1)->parseAsInt();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
		else if((*v1)->getType() == RpnSimpleToken::TYPE_FLOAT || (*v2)->getType() == RpnSimpleToken::TYPE_FLOAT) {
			double result = (*v2)->parseAsFloat() - (*v1)->parseAsFloat();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
	}
	
	void RpnFunctionsArithElem::multiplication(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		(*v1)->parse();
		(*v2)->parse();
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_STRING || (*v2)->getType() == RpnSimpleToken::TYPE_STRING)
			THROW_DOMAIN()
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_INT && (*v2)->getType() == RpnSimpleToken::TYPE_INT) {
			long result = (*v1)->parseAsInt() * (*v2)->parseAsInt();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
		else if((*v1)->getType() == RpnSimpleToken::TYPE_FLOAT || (*v2)->getType() == RpnSimpleToken::TYPE_FLOAT) {
			double result = (*v1)->parseAsFloat() * (*v2)->parseAsFloat();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
	}
	
	void RpnFunctionsArithElem::division(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		(*v1)->parse();
		(*v2)->parse();
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_STRING || (*v2)->getType() == RpnSimpleToken::TYPE_STRING)
			THROW_DOMAIN()
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_INT && (*v2)->getType() == RpnSimpleToken::TYPE_INT) {
			long result = (*v2)->parseAsInt() / (*v1)->parseAsInt();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
		else if((*v1)->getType() == RpnSimpleToken::TYPE_FLOAT || (*v2)->getType() == RpnSimpleToken::TYPE_FLOAT) {
			double result = (*v2)->parseAsFloat() / (*v1)->parseAsFloat();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
	}
	
	void RpnFunctionsArithElem::modulo(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		Rpn<RpnSimpleToken>::QueueIterator v1;
		Rpn<RpnSimpleToken>::QueueIterator v2;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		v1 = rpn->queue.begin();
		v2 = v1;
		v2++;
		
		(*v1)->parse();
		(*v2)->parse();
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_STRING || (*v2)->getType() == RpnSimpleToken::TYPE_STRING)
			THROW_DOMAIN()
		
		if((*v1)->getType() == RpnSimpleToken::TYPE_INT && (*v2)->getType() == RpnSimpleToken::TYPE_INT) {
			long result = (*v2)->parseAsInt() % (*v1)->parseAsInt();
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
		else if((*v1)->getType() == RpnSimpleToken::TYPE_FLOAT || (*v2)->getType() == RpnSimpleToken::TYPE_FLOAT) {
			double result = fmod((*v2)->parseAsFloat(), (*v1)->parseAsFloat());
			rpn->queue.pop_front();
			(*v2)->reset(result);
			delete *v1;
		}
	}
}
