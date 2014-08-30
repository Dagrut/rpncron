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

#include "conf_parser.hpp"
#include "conf_callbacks.hpp"

#include "../rpn/rpn.hpp"

#include "../os/file.hpp"

#include <sstream>
#include <fstream>

#include <cstdlib>

namespace RC {
	namespace Conf {
		Parser::Parser() {
			this->reset();
		}
		
		void parseConfBlockLine(ConfEntity &ci, std::string &buffer);
		void checkConfEntity(ConfEntity &ci);
		
		bool Parser::parseFile(const std::string &path, Confs &confs) {
			std::ifstream ifs;
			std::ostringstream oss;
			char buffer[1024];
			
			ifs.open(path.c_str());
			if(!ifs.is_open()) {
				return(false);
			}
			
			while(ifs.read(buffer, sizeof(buffer))) {
				oss.write(buffer, ifs.gcount());
			}
			
			if(!ifs.eof()) 
				return(false);
			
			if(ifs.gcount() > 0)
				oss.write(buffer, ifs.gcount());
			
			parseString(oss.str(), confs);
			
			return(true);
		}
		
		void Parser::parseString(const std::string &content, Confs &confs) {
			std::string buffer;
			std::istringstream iss(content);
			
			Parser parser;
			
			parser.output_confs = &confs;
			
			iss.width(16384);
			
			while(std::getline(iss, buffer, '\n')) {
				parser.current_line++;
				parser.line_cb(&parser, buffer);
			}
		}
		
		void Parser::reset() {
			this->line_cb = Callbacks::onParserStart;
			
			this->current_ci.conf.mode            = CONF_MODE_BOOL;
			this->current_ci.conf.shell.clear();
			this->current_ci.conf.shell.push_back(OS::Users::getCurrentUserShell());
			this->current_ci.conf.user            = OS::Users::getCurrentUserID();
			this->current_ci.conf.group           = OS::Users::getCurrentGroupID();
			this->current_ci.conf.max_proc        = RPNCRON_CONF_DEFAULT_MAX_PROC;
			this->current_ci.conf.cwd             = RPNCRON_CONF_DEFAULT_CWD_DIR;
			this->current_ci.conf.exec_mode       = CONF_EXEC_MODE_PIPE;
			this->current_ci.conf.code_err_action = CONF_ON_ERROR_LOG | CONF_ON_ERROR_MAIL;
			this->current_ci.conf.output_action   = CONF_ON_ERROR_LOG | CONF_ON_ERROR_MAIL;
			this->current_ci.conf.exec_err_action = CONF_ON_ERROR_LOG | CONF_ON_ERROR_MAIL;
			
			this->current_ci.expr.clear();
			this->current_ci.cmds_lines.clear();
			
			this->expr_buff.str("");
			
			this->have_conf = false;
			this->have_cmds = false;
			this->have_expr = false;
		}
	}
}
