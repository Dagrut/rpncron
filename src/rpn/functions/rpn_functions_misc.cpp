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

#include "rpn_functions_misc.hpp"

#include "../../utils/vixie_cron.hpp"

#define THROW_UNDERFLOW() \
	throw RpnFunctions::UnderflowError(std::string("Not enough values to process function ") + __PRETTY_FUNCTION__);

#define THROW_SYNTAX() \
	throw RpnFunctions::DomainError(std::string("Syntax error when calling ") + __PRETTY_FUNCTION__);

namespace RC {
	void RpnFunctionsMisc::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("eval()",       fnEval);
		rpn.addFunction("set()",        fnSet);
		rpn.addFunction("=",            fnSet);
		rpn.addFunction("eval(vixie)",   fnEvalVixie);
	}
	
	void RpnFunctionsMisc::fnEval(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *expr;
		std::vector<std::string> tokens;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		expr = rpn->queue.front();
		rpn->queue.pop_front();
		
		Rpn<RpnSimpleToken>::parse(
			expr->parseAsString(),
			tokens
		);
		
		delete expr;
		
		for(int i = 0, l = tokens.size() ; i < l ; i++) {
			rpn->parseToken(tokens[i]);
		}
	}
	
	void RpnFunctionsMisc::fnSet(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *name;
		RpnSimpleToken *value;
		
		if(rpn->queue.size() < 2)
			THROW_UNDERFLOW()
		
		name = rpn->queue.front();
		rpn->queue.pop_front();
		value = rpn->queue.front();
		rpn->queue.pop_front();
		
		rpn->addVariable(name->parseAsString(), value);
		delete name;
	}
	
	void RpnFunctionsMisc::fnEvalVixie(Rpn<RpnSimpleToken> *rpn, const std::string &key) {
		RpnSimpleToken *expr;
		Utils::VixieCron::Entry entry;
		long minute;
		long hour;
		long month;
		long month_day;
		long week_day;
		
		if(rpn->queue.size() < 1)
			THROW_UNDERFLOW()
		
		expr = rpn->queue.front();
		
		if(entry.parseString(expr->parseAsString()) == false)
			THROW_SYNTAX()
		
		minute = rpn->variables.find("time_MoH")->second->parseAsInt();
		hour = rpn->variables.find("time_Hod")->second->parseAsInt();
		month = rpn->variables.find("time_moy")->second->parseAsInt();
		month_day = rpn->variables.find("time_dom")->second->parseAsInt();
		week_day = rpn->variables.find("time_dow")->second->parseAsInt();
		
		if(
			entry.minutes[minute] &&
			entry.hours[hour] &&
			entry.months[month] &&
			entry.doms[month_day] &&
			entry.dows[week_day]
		) {
			expr->reset(1L);
		}
		else {
			expr->reset(0L);
		}
	}
}
