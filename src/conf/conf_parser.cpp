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
#include "../rpn/rpn.hpp"

#include "../os/file.hpp"

#include <sstream>
#include <fstream>

#include <cstdlib>

namespace RC {
	namespace ConfParser {
		void parseConfBlockLine(ConfEntity &ci, std::string &buffer);
		void checkConfEntity(ConfEntity &ci);
		void confReset(ConfEntity &ci);
		
		bool parseFile(const std::string &path, Confs &confs) {
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
		
		void parseString(const std::string &content, Confs &confs) {
			std::string buffer;
			std::istringstream iss(content);
			std::string end_of_block;
			
			int state = 0;
			std::ostringstream expr_buff;
			ConfEntity ci;
			bool have_conf = false;
			bool have_expr = false;
			bool have_cmds = false;
			int line = 0;
			
			confReset(ci);
			
			iss.width(16384);
			
			while(std::getline(iss, buffer, '\n')) {
				line++;
				if(state == 0) { // Waiting a block
					if(buffer.compare(0, RPNCRON_CONF_PARSER_CONFIGURATION_LEN, RPNCRON_CONF_PARSER_CONFIGURATION) == 0) {
						if(have_conf) {
							throw ParseError(
								"Found two " RPNCRON_CONF_PARSER_CONFIGURATION
								" blocks side by side ",
								line
							);
						}
						
						end_of_block = buffer.substr(RPNCRON_CONF_PARSER_CONFIGURATION_LEN);
						have_conf = true;
						state = 1;
					}
					else if(buffer.compare(0, RPNCRON_CONF_PARSER_EXPRESSION_LEN, RPNCRON_CONF_PARSER_EXPRESSION) == 0) {
						if(have_expr) {
							throw ParseError(
								"Found two " RPNCRON_CONF_PARSER_EXPRESSION
								" blocks side by side ",
								line
							);
						}
						
						end_of_block = buffer.substr(RPNCRON_CONF_PARSER_EXPRESSION_LEN);
						have_expr = true;
						state = 2;
					}
					else if (buffer.compare(0, RPNCRON_CONF_PARSER_COMMANDS_LEN, RPNCRON_CONF_PARSER_COMMANDS) == 0) {
						if(have_cmds) {
							throw ParseError(
								"Found two " RPNCRON_CONF_PARSER_COMMANDS
								" blocks side by side ",
								line
							);
						}
						
						end_of_block = buffer.substr(RPNCRON_CONF_PARSER_COMMANDS_LEN);
						have_cmds = true;
						state = 3;
					}
				}
				else if(state == 1) {
					if(buffer.size() == 0)
						continue;
					
					if(buffer == end_of_block) {
						state = 0;
					}
					else {
						parseConfBlockLine(ci, buffer);
					}
				}
				else if(state == 2) {
					if(buffer.size() == 0)
						continue;
					
					if(buffer == end_of_block) {
						Rpn<void>::parse(expr_buff.str(), ci.expr);
						if(have_cmds) {
							checkConfEntity(ci);
							confs.push_back(ci);
							confReset(ci);
							ci.expr.clear();
							ci.cmds_lines.clear();
							expr_buff.str("");
							have_conf = false;
							have_cmds = false;
							have_expr = false;
						}
						state = 0;
					}
					else {
						expr_buff.write(buffer.c_str(), buffer.size());
						expr_buff.put('\n');
					}
				}
				else if(state == 3) {
					if(buffer == end_of_block) {
						if(have_expr) {
							checkConfEntity(ci);
							confs.push_back(ci);
							confReset(ci);
							ci.expr.clear();
							ci.cmds_lines.clear();
							expr_buff.str("");
							have_conf = false;
							have_cmds = false;
							have_expr = false;
						}
						state = 0;
					}
					else {
						ci.cmds_lines.push_back(buffer);
					}
				}
			}
		}
		
		void parseConfBlockLine(ConfEntity &ci, std::string &buffer) {
			std::vector<std::string> tokens;
			Rpn<void>::parse(buffer, tokens);
			
			if(tokens.size() < 2)
				return;
			
			if(tokens[0] == "mode") {
				if(tokens[1] == "bool")
					ci.conf.mode = CONF_MODE_BOOL;
				else if(tokens[1] == "offset")
					ci.conf.mode = CONF_MODE_OFFSET;
			}
			else if(tokens[0] == "shell") {
				ci.conf.shell.clear();
				ci.conf.shell.insert(
					ci.conf.shell.end(),
					(++tokens.begin()),
					tokens.end()
				);
			}
			else if(tokens[0] == "user") {
				try {
					ci.conf.user = OS::Users::userNameToUID(tokens[1]);
				}
				catch(std::exception &e) {
					ci.conf.user = OS::Users::getCurrentUserID();
				}
			}
			else if(tokens[0] == "group") {
				try {
					ci.conf.group = OS::Users::groupNameToGID(tokens[1]);
				}
				catch(std::exception &e) {
					ci.conf.group = OS::Users::getCurrentGroupID();
				}
			}
			else if(tokens[0] == "max-proc") {
				ci.conf.max_proc = atoi(tokens[1].c_str());
			}
			else if(tokens[0] == "cwd") {
				ci.conf.cwd = tokens[1];
			}
			else if(tokens[0] == "exec") {
				if(tokens[1] == "system")
					ci.conf.exec_mode = ConfParser::CONF_EXEC_MODE_SYSTEM;
				else if(tokens[1] == "pipe")
					ci.conf.exec_mode = ConfParser::CONF_EXEC_MODE_PIPE;
			}
			else if(tokens[0] == "env-set") {
				if(tokens.size() < 3)
					return;
				
				EnvUpdate eu(tokens[1], tokens[2]);
				
				if(tokens.size() >= 4) {
					if(tokens[3] == "override")
						eu.override = true;
				}
				ci.conf.env_updates.push_back(eu);
			}
			else if(tokens[0] == "env-unset") {
				EnvUpdate eu(tokens[1]);
				ci.conf.env_updates.push_back(eu);
			}
			else if(tokens[0] == "on-output") {
				ci.conf.output_action = 0;
				for(int i = 1, l = tokens.size() ; i < l ; i++) {
					if(tokens[i] == "log")
						ci.conf.output_action |= ConfParser::CONF_ON_ERROR_LOG;
					else if(tokens[i] == "mail")
						ci.conf.output_action |= ConfParser::CONF_ON_ERROR_MAIL;
					else if(tokens[i] == "ignore")
						ci.conf.output_action |= ConfParser::CONF_ON_ERROR_IGNORE;
				}
			}
			else if(tokens[0] == "on-error") {
				ci.conf.code_err_action = 0;
				for(int i = 1, l = tokens.size() ; i < l ; i++) {
					if(tokens[i] == "log")
						ci.conf.code_err_action |= ConfParser::CONF_ON_ERROR_LOG;
					else if(tokens[i] == "mail")
						ci.conf.code_err_action |= ConfParser::CONF_ON_ERROR_MAIL;
					else if(tokens[i] == "ignore")
						ci.conf.code_err_action |= ConfParser::CONF_ON_ERROR_IGNORE;
				}
			}
			else if(tokens[0] == "on-exec-error") {
				ci.conf.exec_err_action = 0;
				for(int i = 1, l = tokens.size() ; i < l ; i++) {
					if(tokens[i] == "log")
						ci.conf.exec_err_action |= ConfParser::CONF_ON_ERROR_LOG;
					else if(tokens[i] == "mail")
						ci.conf.exec_err_action |= ConfParser::CONF_ON_ERROR_MAIL;
					else if(tokens[i] == "ignore")
						ci.conf.exec_err_action |= ConfParser::CONF_ON_ERROR_IGNORE;
				}
			}
		}
		
		void confReset(ConfEntity &ci) {
			ci.conf.mode            = CONF_MODE_BOOL;
			ci.conf.shell.clear();
			ci.conf.shell.push_back(OS::Users::getCurrentUserShell());
			ci.conf.user            = OS::Users::getCurrentUserID();
			ci.conf.group           = OS::Users::getCurrentGroupID();
			ci.conf.max_proc        = RPNCRON_CONF_DEFAULT_MAX_PROC;
			ci.conf.cwd             = RPNCRON_CONF_DEFAULT_CWD_DIR;
			ci.conf.exec_mode       = ConfParser::CONF_EXEC_MODE_PIPE;
			ci.conf.code_err_action = ConfParser::CONF_ON_ERROR_LOG | ConfParser::CONF_ON_ERROR_MAIL;
			ci.conf.output_action   = ConfParser::CONF_ON_ERROR_LOG | ConfParser::CONF_ON_ERROR_MAIL;
			ci.conf.exec_err_action = ConfParser::CONF_ON_ERROR_LOG | ConfParser::CONF_ON_ERROR_MAIL;
		}
		
		void checkConfEntity(ConfEntity &ci) {
			if(ci.conf.max_proc <= 0)
				ci.conf.max_proc = RPNCRON_CONF_DEFAULT_MAX_PROC;
			
			OS::File tdinfos(ci.conf.cwd);
			if(tdinfos.exists()) {
				if(!tdinfos.isDirectory())
					ci.conf.cwd = RPNCRON_CONF_DEFAULT_CWD_DIR;
			}
			
			if(ci.conf.exec_mode == ConfParser::CONF_EXEC_MODE_PIPE) {
				for(int i = 0, l = ci.cmds_lines.size() ; i < l ; i++) {
					ci.cmds_block.insert(
						ci.cmds_block.end(),
						ci.cmds_lines[i].begin(),
						ci.cmds_lines[i].end()
					);
					ci.cmds_block.push_back('\n');
				}
			}
		}
	}
}
