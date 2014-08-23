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

#include <string>
#include <vector>

#include <exception>

#include "../os/users.hpp"
#include "../utils/str_fmt.hpp"

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
	namespace ConfParser {
		class ParseError : public std::exception {
			public:
				ParseError(const std::string &e, int line) {
					this->e = "At line ";
					this->e += Utils::format("%d", line);
					this->e += " : ";
					this->e += e;
				}
				virtual ~ParseError() throw() {}
				
				const char* what() const throw() {
					return(this->e.c_str());
				}
				
			private:
				std::string e;
		};
		
		struct EnvUpdate {
			EnvUpdate(const std::string &key) :
				set(false), key(key)
			{}
			EnvUpdate(const std::string &key, const std::string &val) :
				set(true), override(false), key(key), val(val)
			{}
			bool set;
			bool override;
			std::string key;
			std::string val;
		};
		
		enum ConfEntityMode {
			CONF_MODE_BOOL,
			CONF_MODE_OFFSET,
		};
		
		enum ConfEntityExecMode {
			CONF_EXEC_MODE_SYSTEM,
			CONF_EXEC_MODE_PIPE,
		};
		
		enum ConfEntityErrorAction {
			CONF_ON_ERROR_LOG    = 1 << 0,
			CONF_ON_ERROR_MAIL   = 1 << 1,
			CONF_ON_ERROR_IGNORE = 1 << 2,
		};
		
		struct ConfEntity {
			struct Conf {
				ConfEntityMode mode;
				std::vector<std::string> shell;
				OS::Users::UserID  user;
				OS::Users::GroupID group;
				unsigned int max_proc;
				ConfEntityExecMode exec_mode;
				int code_err_action; /** < ConfEntityErrorAction */
				int output_action;   /** < ConfEntityErrorAction */
				int exec_err_action; /** < ConfEntityErrorAction */
				std::string cwd;
				std::vector<EnvUpdate> env_updates;
			} conf;
			std::vector<std::string> expr;
			std::vector<std::string> cmds_lines;
			std::vector<char> cmds_block;
			//std::vector<std::string> cmds;
		};
		
		typedef std::vector<ConfEntity> Confs;
		
		bool parseFile(const std::string &path, Confs &confs);
		void parseString(const std::string &content, Confs &confs);
	}
}

#endif
