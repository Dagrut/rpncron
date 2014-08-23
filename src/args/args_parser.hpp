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

#ifndef _HPP_RPNCRON_ARGS_ARGS_PARSER_
#define _HPP_RPNCRON_ARGS_ARGS_PARSER_

#include <string>
#include <vector>
#include <map>

namespace RC {
	class ArgsParser {
		public:
			typedef void (*OptionEvent)(
				ArgsParser *parser,
				const std::string &key,
				const std::vector<std::string> &values
			);
			bool parse(
				int argc,
				char **argv,
				std::vector<int> &unknown_args
			);
			void stopParser();
			
			void addOption(
				const std::string &name,
				OptionEvent event,
				int args_count = 0
			);
			void delOption(const std::string &name);
			
			template <typename T>
			void setData(T x) {
				this->data = reinterpret_cast<void*>(x);
			}
			
			template <typename T>
			T* getData() {
				return(reinterpret_cast<T*>(this->data));
			}
			
		private:
			void *data;
			bool must_stop;
			
			struct ArgOpts {
				int args_count;
				OptionEvent event;
			};
			typedef std::map<std::string, ArgOpts> Args;
			Args args;
	};
}

#endif
