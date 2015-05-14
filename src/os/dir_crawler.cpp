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

#include "dir_crawler.hpp"
#include "file.hpp"

#include <unistd.h>

namespace RC {
	OS::DirCrawler::~DirCrawler() {
		for(unsigned int i = 0 ; i < this->dirs.size() ; i++) {
			if(this->dirs[i].dir)
				closedir(this->dirs[i].dir);
		}
		this->dirs.clear();
	}
	
	bool OS::DirCrawler::open(const std::string &path) {
		if(this->dirs.size() > 0)
			this->close();
		
		this->end = false;
		
		this->dirs.push_back(Stack());
		this->dirs[0].dir = opendir(path.c_str());
		if(this->dirs[0].dir == NULL) {
			this->dirs.clear();
			return(false);
		}
		this->dirs[0].path = path;
		
		return(true);
	}
	
	void OS::DirCrawler::close() {
		for(unsigned int i = 0 ; i < this->dirs.size() ; i++) {
			if(this->dirs[i].dir)
				closedir(this->dirs[i].dir);
		}
		this->dirs.clear();
	}
	
	bool OS::DirCrawler::isOpen() {
		return(this->dirs.size() > 0);
	}
	
	bool OS::DirCrawler::isEnd() const {
		return(this->end);
	}
	
	bool OS::DirCrawler::next(FileInfo &finfo) {
		if(this->end)
			return(false);
		
		struct dirent *cursor;
		int cur;
		
		finfo.error = false;
		
		cur = this->dirs.size() - 1;
		cursor = readdir(this->dirs[cur].dir);
		
		while(true) {
			if(cursor != NULL) {
				if(std::string(".") != cursor->d_name && std::string("..") != cursor->d_name) {
					break;
				}
			}
			else if(cur == 0) {
				this->end = true;
				return(false);
			}
			else {
				closedir(this->dirs[cur].dir);
				this->dirs.pop_back();
				cur--;
			}
			
			cursor = readdir(this->dirs[cur].dir);
		}
		
		finfo.path = this->dirs[cur].path + '/' + cursor->d_name;
		finfo.file.open(finfo.path);
		
		if(finfo.file.isDirectory()) {
			this->dirs.push_back(Stack());
			this->dirs[cur+1].path = finfo.path;
			this->dirs[cur+1].dir = opendir(this->dirs[cur+1].path.c_str());
			if(this->dirs[cur+1].dir == NULL) {
				finfo.error = true;
				this->dirs.pop_back();
			}
		}
		
		return(true);
	}
}
