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

#include "mail.hpp"

#include "processes.hpp"

#include <cstring>

namespace RC {
	namespace OS {
		void Mail::appendToBody(const std::string &str) {
			this->body << str;
		}
		
		void Mail::addHeader(const std::string &key, const std::string &val) {
			this->headers.insert(std::make_pair(
				key,
				val
			));
		}
		
		void Mail::delHeader(const std::string &key, const std::string &val) {
			std::multimap<std::string, std::string>::iterator it;
			std::multimap<std::string, std::string>::iterator it2;
			
			it = this->headers.find(key);
			while(it != this->headers.end()) {
				if(it->first != key)
					break;
				if(it->second == val) {
					it2 = it;
					it2++;
					this->headers.erase(it);
					it = it2;
				}
				else {
					it++;
				}
			}
		}
		
		void Mail::delHeaders(const std::string &key) {
			this->headers.erase(key);
		}
		
		std::string Mail::build() {
			std::ostringstream result;
			std::multimap<std::string, std::string>::iterator it;
			std::multimap<std::string, std::string>::iterator end;
			
			it = this->headers.begin();
			end = this->headers.end();
			for(; it != end ; it++) {
				result << it->first << ": " << it->second << "\r\n";
			}
			
			result << "\r\n";
			
			result << this->body.str();
			
			return(result.str());
		}
		
		void Mail::send(const std::string &to) {
			std::string mail = this->body.str();
			std::vector<char> mail_data(mail.begin(), mail.end());
			std::vector<char> unused;
			std::vector<std::string> args;
			std::multimap<std::string, std::string>::iterator it;
			std::multimap<std::string, std::string>::iterator end;
			
			args.push_back("/usr/bin/mail");
			
			it = this->headers.begin();
			end = this->headers.end();
			for(; it != end ; it++) {
				if(strcasecmp(it->first.c_str(), "subject") == 0) {
					args.push_back("-s");
					args.push_back(it->second);
				}
				else {
					args.push_back("-a");
					args.push_back(it->first + ": " + it->second);
				}
			}
			
			args.push_back(to);
			
			Processes::popenPipe(
				mail_data,
				unused,
				0,
				args[0],
				args,
				NULL
			);
		}
	}
}
