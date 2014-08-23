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

#include "directory.hpp"

#include <unistd.h>

namespace RC {
	OS::Directory::Directory() :
		dir(NULL), cursor(NULL), end(false)
	{}
	
	OS::Directory::~Directory() {
		if(this->dir)
			closedir(this->dir);
	}
	
	bool OS::Directory::open(const std::string &path) {
		if(this->dir)
			this->close();
		
		this->dir = opendir(path.c_str());
		if(this->dir == NULL) {
			return(false);
		}
		
		return(true);
	}
	
	void OS::Directory::close() {
		if(this->dir) {
			closedir(this->dir);
			this->dir = NULL;
		}
	}
	
	bool OS::Directory::isOpen() {
		return(this->dir != NULL);
	}
	
	bool OS::Directory::isEnd() const {
		return(this->end);
	}
	
	bool OS::Directory::next(std::string &name) {
		if(this->end)
			return(false);
		
		this->cursor = readdir(this->dir);
		if(this->cursor == NULL) {
			this->end = true;
			return(false);
		}
		
		name = this->cursor->d_name;
		return(true);
	}
	
	bool OS::Directory::current(std::string &name) const {
		if(this->end)
			return(false);
		
		if(this->cursor == NULL)
			return(false);
		
		name = this->cursor->d_name;
		return(true);
	}
	
	void OS::Directory::rewind() {
		rewinddir(this->dir);
		this->end = false;
		this->cursor = NULL;
	}
}
