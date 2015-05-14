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

#include "conf_callbacks.hpp"
#include "conf_parser.hpp"

#include "../rpn/rpn.hpp"

#include "../os/file.hpp"

#include <cstdlib>

#define RC_CONF_CB_CFG			"@CFG "
#define RC_CONF_CB_RPN			"@RPN "
#define RC_CONF_CB_CMD			"@CMD "
#define RC_CONF_CB_DFT			"@DFT "

#define RC_CONF_CB_CFG_L		(sizeof(RC_CONF_CB_CFG) - 1)
#define RC_CONF_CB_RPN_L		(sizeof(RC_CONF_CB_RPN) - 1)
#define RC_CONF_CB_CMD_L		(sizeof(RC_CONF_CB_CMD) - 1)
#define RC_CONF_CB_DFT_L		(sizeof(RC_CONF_CB_DFT) - 1)

namespace RC {
	namespace Conf {
		namespace Callbacks {
			static void onParserConf(Parser* parser, const std::string &line);
			static void onParserRpn(Parser* parser, const std::string &line);
			static void onParserCmds(Parser* parser, const std::string &line);
			static void onParserDefault(Parser* parser, const std::string &line);
			
			static void parseConfBlockLine(ConfEntity &ci, const std::string &line);
			static void checkConfEntity(ConfEntity &ci);
			
			typedef void (*ConfBlockCmdCallback)(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdMode(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdShell(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdUser(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdGroup(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdMaxProc(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdCwd(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdExec(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdEnvSet(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdEnvUnset(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdOnOutput(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdOnError(ConfEntity &ci, std::vector<std::string> &tokens);
			static void parseConfBlockCmdOnExecError(ConfEntity &ci, std::vector<std::string> &tokens);
			
			static void parseConfBlockCmdOnEvent(int &output, std::vector<std::string> &tokens);
		}
		
		void Callbacks::onParserStart(Parser* parser, const std::string &line) {
			if(line.size() == 0)
				return;
			
			if(line.compare(0, RC_CONF_CB_CFG_L, RC_CONF_CB_CFG) == 0) {
				if(parser->have_conf) {
					throw ParseError(
						"Found two " RC_CONF_CB_CFG
						" blocks side by side ",
						parser->current_line
					);
				}
				
				parser->end_of_block = line.substr(RC_CONF_CB_CFG_L);
				parser->have_conf = true;
				parser->line_cb = onParserConf;
			}
			else if(line.compare(0, RC_CONF_CB_RPN_L, RC_CONF_CB_RPN) == 0) {
				if(parser->have_expr) {
					throw ParseError(
						"Found two " RC_CONF_CB_RPN
						" blocks side by side ",
						parser->current_line
					);
				}
				
				parser->end_of_block = line.substr(RC_CONF_CB_RPN_L);
				parser->have_expr = true;
				parser->line_cb = onParserRpn;
			}
			else if(line.compare(0, RC_CONF_CB_CMD_L, RC_CONF_CB_CMD) == 0) {
				if(parser->have_cmds) {
					throw ParseError(
						"Found two " RC_CONF_CB_CMD
						" blocks side by side ",
						parser->current_line
					);
				}
				
				parser->end_of_block = line.substr(RC_CONF_CB_CMD_L);
				parser->have_cmds = true;
				parser->line_cb = onParserCmds;
			}
			else if(line.compare(0, RC_CONF_CB_DFT_L, RC_CONF_CB_DFT) == 0) {
				parser->end_of_block = line.substr(RC_CONF_CB_DFT_L);
				parser->line_cb = onParserDefault;
			}
		}
		
		static void Callbacks::onParserConf(Parser* parser, const std::string &line) {
			if(line.size() == 0)
				return;
			
			if(line == parser->end_of_block) {
				parser->line_cb = onParserStart;
			}
			else {
				parseConfBlockLine(parser->current_ci, line);
			}
		}
		
		static void Callbacks::onParserRpn(Parser* parser, const std::string &line) {
			if(line.size() == 0)
				return;
			if(line[0] == '#')
				return;
			
			if(line == parser->end_of_block) {
				Rpn<void>::parse(parser->expr_buff.str(), parser->current_ci.expr);
				if(parser->have_cmds) {
					checkConfEntity(parser->current_ci);
					parser->output_confs->push_back(parser->current_ci);
					parser->reset();
				}
				else {
					parser->line_cb = onParserStart;
				}
			}
			else {
				parser->expr_buff.write(line.c_str(), line.size());
				parser->expr_buff.put('\n');
			}
		}
		
		static void Callbacks::onParserCmds(Parser* parser, const std::string &line) {
			if(line == parser->end_of_block) {
				if(parser->have_expr) {
					checkConfEntity(parser->current_ci);
					parser->output_confs->push_back(parser->current_ci);
					parser->reset();
				}
				else {
					parser->line_cb = onParserStart;
				}
			}
			else {
				parser->current_ci.cmds_lines.push_back(line);
			}
		}
		
		static void Callbacks::onParserDefault(Parser* parser, const std::string &line) {
			if(line == parser->end_of_block) {
				parser->line_cb = onParserStart;
				parser->reset();
			}
			else {
				parseConfBlockLine(parser->default_ci, line);
			}
		}
		
		static void Callbacks::parseConfBlockLine(ConfEntity &ci, const std::string &line) {
			std::vector<std::string> tokens;
			
			Rpn<void>::parse(line, tokens);
			if(tokens.size() < 1)
				return;
			
			static std::map<std::string, ConfBlockCmdCallback> kvMap;
			std::map<std::string, ConfBlockCmdCallback>::iterator it;
			
			if(kvMap.size() == 0) {
				kvMap.insert(std::make_pair("mode",          parseConfBlockCmdMode));
				kvMap.insert(std::make_pair("shell",         parseConfBlockCmdShell));
				kvMap.insert(std::make_pair("user",          parseConfBlockCmdUser));
				kvMap.insert(std::make_pair("group",         parseConfBlockCmdGroup));
				kvMap.insert(std::make_pair("max-proc",      parseConfBlockCmdMaxProc));
				kvMap.insert(std::make_pair("cwd",           parseConfBlockCmdCwd));
				kvMap.insert(std::make_pair("exec",          parseConfBlockCmdExec));
				kvMap.insert(std::make_pair("env-set",       parseConfBlockCmdEnvSet));
				kvMap.insert(std::make_pair("env-unset",     parseConfBlockCmdEnvUnset));
				kvMap.insert(std::make_pair("on-output",     parseConfBlockCmdOnOutput));
				kvMap.insert(std::make_pair("on-error",      parseConfBlockCmdOnError));
				kvMap.insert(std::make_pair("on-exec-error", parseConfBlockCmdOnExecError));
			}
			
			it = kvMap.find(tokens[0]);
			if(it == kvMap.end())
				return;
			
			it->second(ci, tokens);
		}
		
		static void Callbacks::parseConfBlockCmdMode(ConfEntity &ci, std::vector<std::string> &tokens) {
			if(tokens[1] == "bool")
				ci.conf.mode = CONF_MODE_BOOL;
			else if(tokens[1] == "offset")
				ci.conf.mode = CONF_MODE_OFFSET;
		}
		
		static void Callbacks::parseConfBlockCmdShell(ConfEntity &ci, std::vector<std::string> &tokens) {
			ci.conf.shell.clear();
			ci.conf.shell.insert(
				ci.conf.shell.end(),
				(++tokens.begin()),
				tokens.end()
			);
		}
		
		static void Callbacks::parseConfBlockCmdUser(ConfEntity &ci, std::vector<std::string> &tokens) {
			try {
				ci.conf.user = OS::Users::userNameToUID(tokens[1]);
			}
			catch(std::exception &e) {
				ci.conf.user = OS::Users::getCurrentUserID();
			}
		}
		
		static void Callbacks::parseConfBlockCmdGroup(ConfEntity &ci, std::vector<std::string> &tokens) {
			try {
				ci.conf.group = OS::Users::groupNameToGID(tokens[1]);
			}
			catch(std::exception &e) {
				ci.conf.group = OS::Users::getCurrentGroupID();
			}
		}
		
		static void Callbacks::parseConfBlockCmdMaxProc(ConfEntity &ci, std::vector<std::string> &tokens) {
			ci.conf.max_proc = atoi(tokens[1].c_str());
		}
		
		static void Callbacks::parseConfBlockCmdCwd(ConfEntity &ci, std::vector<std::string> &tokens) {
			ci.conf.cwd = tokens[1];
		}
		
		static void Callbacks::parseConfBlockCmdExec(ConfEntity &ci, std::vector<std::string> &tokens) {
			if(tokens[1] == "system")
				ci.conf.exec_mode = CONF_EXEC_MODE_SYSTEM;
			else if(tokens[1] == "pipe")
				ci.conf.exec_mode = CONF_EXEC_MODE_PIPE;
		}
		
		static void Callbacks::parseConfBlockCmdEnvSet(ConfEntity &ci, std::vector<std::string> &tokens) {
			if(tokens.size() < 3)
				return;
			
			EnvUpdate eu(tokens[1], tokens[2]);
			
			if(tokens.size() >= 4) {
				if(tokens[3] == "override")
					eu.override = true;
			}
			ci.conf.env_updates.push_back(eu);
		}
		
		static void Callbacks::parseConfBlockCmdEnvUnset(ConfEntity &ci, std::vector<std::string> &tokens) {
			EnvUpdate eu(tokens[1]);
			ci.conf.env_updates.push_back(eu);
		}
		
		static void Callbacks::parseConfBlockCmdOnOutput(ConfEntity &ci, std::vector<std::string> &tokens) {
			parseConfBlockCmdOnEvent(ci.conf.output_action, tokens);
		}
		
		static void Callbacks::parseConfBlockCmdOnError(ConfEntity &ci, std::vector<std::string> &tokens) {
			parseConfBlockCmdOnEvent(ci.conf.code_err_action, tokens);
		}
		
		static void Callbacks::parseConfBlockCmdOnExecError(ConfEntity &ci, std::vector<std::string> &tokens) {
			parseConfBlockCmdOnEvent(ci.conf.exec_err_action, tokens);
		}
		
		static void Callbacks::parseConfBlockCmdOnEvent(int &output, std::vector<std::string> &tokens) {
			output = 0;
			for(int i = 1, l = tokens.size() ; i < l ; i++) {
				if(tokens[i] == "log")
					output |= CONF_ON_ERROR_LOG;
				else if(tokens[i] == "mail")
					output |= CONF_ON_ERROR_MAIL;
				else if(tokens[i] == "ignore")
					output |= CONF_ON_ERROR_IGNORE;
			}
		}
		
		static void Callbacks::checkConfEntity(ConfEntity &ci) {
			if(ci.conf.max_proc <= 0)
				ci.conf.max_proc = RPNCRON_CONF_DEFAULT_MAX_PROC;
			
			OS::File tdinfos(ci.conf.cwd);
			if(tdinfos.exists()) {
				if(!tdinfos.isDirectory())
					ci.conf.cwd = RPNCRON_CONF_DEFAULT_CWD_DIR;
			}
			
			if(ci.conf.exec_mode == CONF_EXEC_MODE_PIPE) {
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
