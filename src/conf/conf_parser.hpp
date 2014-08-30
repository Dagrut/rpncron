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

#ifndef _HPP_RPNCRON_CONF_CONF_PARSER_
#define _HPP_RPNCRON_CONF_CONF_PARSER_

#include "conf.hpp"

#include <string>
#include <vector>
#include <map>

#include <sstream>

#define RPNCRON_CONF_PARSER_CONFIGURATION "@CFG "
#define RPNCRON_CONF_PARSER_CONFIGURATION_LEN (sizeof(RPNCRON_CONF_PARSER_CONFIGURATION) - 1)
#define RPNCRON_CONF_PARSER_FILE_HEADER_LEN (sizeof(RPNCRON_CONF_PARSER_FILE_HEADER) - 1)
#define RPNCRON_CONF_PARSER_EXPRESSION "@RPN "
#define RPNCRON_CONF_PARSER_EXPRESSION_LEN (sizeof(RPNCRON_CONF_PARSER_EXPRESSION) - 1)
#define RPNCRON_CONF_PARSER_COMMANDS "@CMD "
#define RPNCRON_CONF_PARSER_COMMANDS_LEN (sizeof(RPNCRON_CONF_PARSER_COMMANDS) - 1)

#define RPNCRON_CONF_DEFAULT_MAX_PROC 128
#define RPNCRON_CONF_DEFAULT_CWD_DIR  "/"

namespace RC {
	namespace Conf {
		class Parser {
			public:
				typedef std::vector<ConfEntity> Confs;
				
				static bool parseFile(const std::string &path, Confs &confs);
				static void parseString(const std::string &content, Confs &confs);
				
				void reset();
				
				LineCallback line_cb;
				int current_line;
				
				ConfEntity current_ci;
				
				std::string end_of_block;
				
				bool have_conf;
				bool have_expr;
				bool have_cmds;
				
				Confs *output_confs;
				std::ostringstream expr_buff;
				
			private:
				Parser();
		};
	}
}

#endif
