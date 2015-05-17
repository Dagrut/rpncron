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

#ifndef _HPP_RPNCRON_UTILS_PTRCOMP_
#define _HPP_RPNCRON_UTILS_PTRCOMP_

#include <functional>

namespace RC {
	namespace Utils {
		template <typename T>
		class PtrComp : public std::less<T*> {
			public:
				bool operator()(const T* p1, const T* p2) const {
					return(*p1 < *p2);
				}
		};
	}
}

#endif
