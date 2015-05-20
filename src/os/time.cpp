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

#include "time.hpp"

#include <ctime>

namespace RC {
	void OS::Time::init() {
		tzset();
	}
	
	OS::Time::Tstamp OS::Time::get(bool utc) {
		Tstamp now;
		
		time(&now);
		if(utc)
			now -= timezone; /* libc global >_<... set by tzset */
		
		return(now);
	}
	
	OS::Time::Tstamp OS::Time::addTz(OS::Time::Tstamp time) {
		return(time - timezone);
	}
	
	OS::Time::Tstamp OS::Time::subTz(OS::Time::Tstamp time) {
		return(time + timezone);
	}
}
