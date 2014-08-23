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

#include "file.hpp"

#include <unistd.h>
#include <cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace RC {
	OS::File::File() :
		have_infos(false)
	{}
	
	OS::File::File(const std::string &name) {
		this->have_infos = (stat(name.c_str(), &this->infos) != -1);
	}
	
	bool OS::File::open(const std::string &name) {
		this->have_infos = (stat(name.c_str(), &this->infos) != -1);
		return(this->have_infos);
	}
	
	bool OS::File::exists() {
		if(!this->have_infos)
			return(false);
		
		return(true);
	}
	
	bool OS::File::isFile() {
		if(!this->have_infos)
			return(false);
		
		return(this->infos.st_mode & S_IFREG);
	}
	
	bool OS::File::isDirectory() {
		if(!this->have_infos)
			return(false);
		
		return(this->infos.st_mode & S_IFDIR);
	}
	
	bool OS::File::isExecutable() {
		if(!this->have_infos)
			return(false);
		
		if(!(this->infos.st_mode & S_IFREG))
			return(false);
		
		return(
			(this->infos.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
			                    == (S_IXUSR | S_IXGRP | S_IXOTH)
		);
	}
	
	bool OS::File::remove(const std::string &path) {
		if(unlink(path.c_str()) == 0)
			return(true);
		return(false);
	}
	
	std::string OS::File::tmpFileName() {
		char tmp[L_tmpnam];
		
		if(tmpnam(tmp) == NULL) {
			throw SystemError("Could not find an unused temporary file name");
		}
		
		return(tmp);
	}
	
	void OS::File::dupToTmpFile(const std::string &src, std::string &dst) {
		char tmp[L_tmpnam];
		
		if(tmpnam(tmp) == NULL) {
			throw SystemError("Could not find an unused temporary file name");
		}
		
		dst = tmp;
		
		OS::File::copy(src, dst, true, true);
	}
	
	bool OS::File::mkdir(const std::string &path, bool recursive, bool ignore_last, int mode) {
		int ret;
		File f;
		
		if(recursive) {
			std::string tmp = path;
			std::size_t ptr = 0;
			
			if(path[0] != '/')
				return(false);
			
			ptr = path.find('/', 1);
			while(ptr != std::string::npos) {
				tmp = path.substr(0, ptr);
				
				f.open(tmp);
				if(!f.isDirectory()) {
					ret = ::mkdir(tmp.c_str(), mode);
					if(ret != 0)
						return(false);
				}
				
				ptr = path.find('/', ptr + 1);
			}
			
			if(ignore_last) {
				return(true);
			}
		}
		else if(ignore_last) {
			std::size_t ptr = path.rfind('/');
			
			if(ptr == std::string::npos || ptr == 0)
				return(false);
			
			std::string path2 = path.substr(0, ptr);
			
			f.open(path2);
			if(!f.isDirectory()) {
				ret = ::mkdir(path2.c_str(), mode);
				if(ret != 0)
					return(false);
			}
			
			return(true);
		}
		
		f.open(path);
		if(!f.isDirectory()) {
			ret = ::mkdir(path.c_str(), mode);
			if(ret != 0)
				return(false);
		}
		return(true);
	}
	
	void OS::File::copy(
		const std::string &src,
		const std::string &dst,
		bool force,
		bool no_dst
	) {
		int src_fd;
		int dst_fd;
		int buffer[1024];
		bool stop = false;
		int ret;
		int ret2;
		
		File f(dst);
		if(f.exists() && !force)
			return;
		
		src_fd = ::open(src.c_str(), O_RDONLY | O_NOATIME);
		dst_fd = ::open(dst.c_str(), O_WRONLY | O_CREAT | (no_dst ? O_EXCL : 0) | O_TRUNC, 0666);
		if(src_fd == -1) {
			if(dst_fd != -1)
				close(dst_fd);
			throw SystemError(std::string("Could not open file '") + src + "'");
		}
		
		if(dst_fd == -1) {
			close(src_fd);
			throw SystemError(std::string("Could not open file '") + dst + "'");
		}
		
		#define STOP() \
			close(src_fd); \
			close(dst_fd);
		
		while(!stop) {
			ret = read(src_fd, buffer, sizeof(buffer));
			if(ret >= 0) {
				if(ret == 0)
					stop = true;
				ret2 = write(dst_fd, buffer, ret);
				if(ret2 == -1) {
					STOP()
					throw SystemError(std::string("Could not write to file '") + dst + "' (1)");
				}
				if(ret2 < ret) {
					ret2 = write(dst_fd, buffer + ret2, ret - ret2) + ret2;
					if(ret2 < ret) {
						STOP()
						throw SystemError(std::string("Could not write to file '") + dst + "' (2)");
					}
				}
			}
			else {
				STOP()
				throw SystemError(std::string("Could not read file '") + src + "'");
			}
		}
		
		STOP()
		#undef STOP
	}
	
	void OS::File::move(const std::string &src, const std::string &dst) {
		if(rename(src.c_str(), dst.c_str()) == 0)
			return;
		
		if(OS::File::mkdir(dst, true, true, 0755) == false) {
			throw SystemError(
				std::string("Could not create directories for file '") +
				dst +
				"'"
			);
		}
		
		OS::File::copy(src, dst, true, false);
	}
}
