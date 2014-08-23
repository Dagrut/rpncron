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

#ifndef _HPP_RPNCRON_ARGS_ARGS_DOC_RC_
#define _HPP_RPNCRON_ARGS_ARGS_DOC_RC_

#include "args_rc.hpp"

namespace RC {
	namespace ArgsDocRc {
		void showAllDocumentation(ArgsRc &current);
		
		#define CALLBACK(name) \
			void name ## Doc(ArgsRc &current);
		
		CALLBACK(onDaemon)
		CALLBACK(onRun)
		CALLBACK(onRunFile)
		CALLBACK(onRunDir)
		CALLBACK(onDebugFile)
		CALLBACK(onVerbose)
		CALLBACK(onQuiet)
		CALLBACK(onEdit)
		CALLBACK(onList)
		CALLBACK(onUser)
		CALLBACK(onRemove)
		CALLBACK(onHelp)
		
		#undef CALLBACK
	};
}

#endif
