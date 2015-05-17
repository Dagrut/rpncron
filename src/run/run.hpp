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

#ifndef _HPP_RPNCRON_RUN_RUN_
#define _HPP_RPNCRON_RUN_RUN_

#include "../rpn/rpn.hpp"
#include "../rpn/rpn_simple_token.hpp"
#include "../args/args_rc.hpp"
#include "../conf/conf_parser.hpp"
#include "../utils/ptr_comp.hpp"
#include "../os/processes.hpp"

#include <set>

#include <ctime>
#include <csignal>

namespace RC {
	namespace Run {
		void prepare(Rpn<RpnSimpleToken> &rpn, long now, int proc_count, long exec_count);
		void prepareTimeVars(Rpn<RpnSimpleToken> &rpn, long now);
		void prepareProcVars(Rpn<RpnSimpleToken> &rpn, int proc_count, long exec_count);
		void prepareFunctions(Rpn<RpnSimpleToken> &rpn);
	};
}

#endif
