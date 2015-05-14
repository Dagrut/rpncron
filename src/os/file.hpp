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

#ifndef _HPP_RPNCRON_OS_FILE_
#define _HPP_RPNCRON_OS_FILE_

#include "exceptions.hpp"

#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace RC {
	namespace OS {
		class File {
			public:
				File(const std::string &fname);
				File();
				
				bool open(const std::string &fname);
				bool exists();
				bool isFile();
				bool isDirectory();
				bool isExecutable();
				
				static bool remove(const std::string &path);
				static std::string tmpFileName();
				static FILE* tmpFile(
					std::string &name,
					const std::string &tpl = "/tmp/rpncron-tmpfile"
				);
				static void dupToTmpFile(const std::string &src, std::string &dst);
				static bool mkdir(
					const std::string &path,
					bool recursive = false,
					bool ignore_last = false,
					int mode = 0755);
				static void copy(
					const std::string &src,
					const std::string &dst,
					bool force = false,
					bool no_dst = false
				);
				static void putContent(
					const std::string &fname,
					const std::string &data
				);
				static void move(const std::string &src, const std::string &dst);
				
			private:
				struct stat infos;
				bool have_infos;
		};
	}
}

#endif
