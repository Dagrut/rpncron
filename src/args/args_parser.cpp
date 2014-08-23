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

#include "args_parser.hpp"

namespace RC {
	bool ArgsParser::parse(
		int argc,
		char **argv,
		std::vector<int> &unknown_args
	) {
		Args::iterator it;
		int remaining_args = 0;
		std::vector<std::string> current_args;
		
		this->must_stop = false;
		
		for(int i = 1 ; i < argc && !this->must_stop ; i++) {
			if(remaining_args > 1) {
				current_args.push_back(std::string(argv[i]));
				remaining_args--;
			}
			else if(remaining_args == 1) {
				current_args.push_back(std::string(argv[i]));
				it->second.event(this, it->first, current_args);
				current_args.clear();
				remaining_args = 0;
			}
			else {
				it = this->args.find(argv[i]);
				if(it != this->args.end()) {
					if(it->second.args_count == 0) {
						it->second.event(this, it->first, current_args);
					}
					else {
						remaining_args = it->second.args_count;
					}
				}
				else {
					unknown_args.push_back(i);
				}
			}
		}
		
		return(!this->must_stop);
	}
	
	void ArgsParser::stopParser() {
		this->must_stop = true;
	}
	
	void ArgsParser::addOption(
		const std::string &name,
		OptionEvent event,
		int args_count
	) {
		ArgOpts opts;
		
		opts.event = event;
		opts.args_count = args_count;
		
		this->args.insert(std::make_pair(name, opts));
	}
	
	void ArgsParser::delOption(const std::string &name) {
		this->args.erase(name);
	}
}
