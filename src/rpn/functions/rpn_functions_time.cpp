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

#include "rpn_functions_time.hpp"

#define THROW_UNDERFLOW() \
	throw RpnFunctions::UnderflowError(std::string("Not enough values to process function ") + __PRETTY_FUNCTION__);

namespace RC {
	void RpnFunctionsTime::load(Rpn<RpnSimpleToken> &rpn) {
		rpn.addFunction("min2sec()",    fnMinToSec);
		rpn.addFunction("hour2sec()",   fnHourToSec);
		rpn.addFunction("day2sec()",    fnDayToSec);
		rpn.addFunction("week2sec()",   fnWeekToSec);
		rpn.addFunction("year2sec()",   fnYearToSec);
		
		rpn.addFunction("sec2min()",    fnSecToMin);
		rpn.addFunction("sec2hour()",   fnSecToHour);
		rpn.addFunction("sec2day()",    fnSecToDay);
		rpn.addFunction("sec2week()",   fnSecToWeek);
		rpn.addFunction("sec2year()",   fnSecToYear);
	}
	
#define ITEM(name, op) \
	void RpnFunctionsTime::name(Rpn<RpnSimpleToken> *rpn, const std::string &key) { \
		Rpn<RpnSimpleToken>::QueueIterator v1; \
		 \
		if(rpn->queue.size() < 1) \
			THROW_UNDERFLOW() \
		 \
		v1 = rpn->queue.begin(); \
		 \
		(*v1)->reset((*v1)->parseAsInt() op); \
	}
	
	ITEM(fnMinToSec, * 60)
	ITEM(fnHourToSec, * 3600)
	ITEM(fnDayToSec, * 86400)
	ITEM(fnWeekToSec, * 604800)
	ITEM(fnYearToSec, * 31536000)
	
	ITEM(fnSecToMin, / 60)
	ITEM(fnSecToHour, / 3600)
	ITEM(fnSecToDay, / 86400)
	ITEM(fnSecToWeek, / 604800)
	ITEM(fnSecToYear, / 31536000)
	
#undef ITEM
}
