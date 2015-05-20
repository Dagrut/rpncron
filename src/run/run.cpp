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

#include "run.hpp"

#include "../rpn/functions/rpn_functions_arith_elem.hpp"
#include "../rpn/functions/rpn_functions_comp.hpp"
#include "../rpn/functions/rpn_functions_logic.hpp"
#include "../rpn/functions/rpn_functions_types.hpp"
#include "../rpn/functions/rpn_functions_queue.hpp"
#include "../rpn/functions/rpn_functions_branching.hpp"
#include "../rpn/functions/rpn_functions_misc.hpp"
#include "../rpn/functions/rpn_functions_time.hpp"

#include "../os/processes.hpp"
#include "../os/file.hpp"
#include "../os/directory.hpp"
#include "../os/environment.hpp"
#include "../os/time.hpp"

#include "../os/logs.hpp"
#include "../os/mail.hpp"

#include "../rpncron.hpp"

#include <ctime>
#include <cstdio>
#include <cstdlib>


namespace RC {
	void Run::prepare(Rpn<RpnSimpleToken> &rpn, OS::Time::Tstamp now, int proc_count, long exec_count) {
		rpn.clearVariables();
		rpn.clearFunctions();
		rpn.clearQueue();
		
		Run::prepareProcVars(rpn, proc_count, exec_count);
		Run::prepareTimeVars(rpn, now);
		Run::prepareFunctions(rpn);
	}
	
	void Run::prepareTimeVars(Rpn<RpnSimpleToken> &rpn, OS::Time::Tstamp now) {
		rpn.addVariable("time_sec",  new RpnSimpleToken(now));
		rpn.addVariable("time_min",  new RpnSimpleToken(now/60));
		rpn.addVariable("time_hour", new RpnSimpleToken(now/3600));
		rpn.addVariable("time_day",  new RpnSimpleToken(now/86400));
		
		struct tm date;
		OS::Time::Tstamp now_time = OS::Time::subTz(now);
		
		localtime_r(&now_time, &date);
		
		rpn.addVariable("time_SoM",    new RpnSimpleToken((long) date.tm_sec));
		rpn.addVariable("time_MoH",    new RpnSimpleToken((long) date.tm_min));
		rpn.addVariable("time_Hod",    new RpnSimpleToken((long) date.tm_hour));
		rpn.addVariable("time_dow",    new RpnSimpleToken((long) date.tm_wday));
		rpn.addVariable("time_dom",    new RpnSimpleToken((long) date.tm_mday));
		rpn.addVariable("time_moy",    new RpnSimpleToken((long) date.tm_mon));
		rpn.addVariable("time_doy",    new RpnSimpleToken((long) date.tm_yday));
		rpn.addVariable("time_wom",    new RpnSimpleToken((long) (date.tm_mday + 6 - (date.tm_wday + 6) % 7) / 7));
		rpn.addVariable("time_woy",    new RpnSimpleToken((long) (date.tm_yday + 7 - (date.tm_wday + 6) % 7) / 7));
		rpn.addVariable("time_year",   new RpnSimpleToken((long) date.tm_year + 1900));
		rpn.addVariable("time_is_dst", new RpnSimpleToken((long) date.tm_isdst));
	}
	
	void Run::prepareProcVars(Rpn<RpnSimpleToken> &rpn, int proc_count, long exec_count) {
		rpn.addVariable("proc_count",  new RpnSimpleToken((long) proc_count));
		rpn.addVariable("exec_count",  new RpnSimpleToken(exec_count));
	}
	
	void Run::prepareFunctions(Rpn<RpnSimpleToken> &rpn) {
		RpnFunctionsArithElem::load(rpn);
		RpnFunctionsComp::load(rpn);
		RpnFunctionsLogic::load(rpn);
		RpnFunctionsTypes::load(rpn);
		RpnFunctionsQueue::load(rpn);
		RpnFunctionsBranching::load(rpn);
		RpnFunctionsMisc::load(rpn);
		RpnFunctionsTime::load(rpn);
	}
}
