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

#include "run_debug.hpp"
#include "../conf/conf_parser.hpp"
#include "../rpn/rpn.hpp"
#include "../rpn/rpn_simple_token.hpp"
#include "run.hpp"

#include <cstdio>

namespace RC {
	namespace RunDebug {
		static void showStack(Rpn<RpnSimpleToken> &rpn);
		static void printEnvironment(Conf::ConfEntity &conf);
		static void printEventsFlags(const char *desc, int code);
	}
	
	void RunDebug::runDebug(ArgsRc &args) {
		std::string dbgFile = args.getDebugFile();
		time_t now = args.getDebugStart();
		
		Rpn<RpnSimpleToken> rpn;
		Conf::Parser::Confs confs;
		Conf::Parser::parseFile(dbgFile, confs);
		
		bool cont;
		
		for(int ci = 0, cl = confs.size() ; ci < cl ; ci++) {
			Conf::ConfEntity &conf = confs[ci];
			
			Run::prepare(rpn, now, 0, 0);
			
			printf("# Executing configuration %d\n", ci);
			printf("> Mode is          <%s>\n", conf.conf.mode == Conf::CONF_MODE_BOOL ? "bool" : "offset");
			printf("> Shell is        ");
			for(int i = 0, l = conf.conf.shell.size() ; i < l ; i++)
				printf(" <%s>", conf.conf.shell[i].c_str());
			printf("\n");
			printf("> User ID is       <%d>\n", (int) conf.conf.user);
			printf("> Group ID is      <%d>\n", (int) conf.conf.group);
			printf("> CWD is           <%s>\n", conf.conf.cwd.c_str());
			printf("> Max process is   <%d>\n", conf.conf.max_proc);
			printf("> Exec mode is     <%s>\n", conf.conf.exec_mode == Conf::CONF_EXEC_MODE_PIPE ? "pipe" : "system");
			printEventsFlags("On error        ", conf.conf.code_err_action);
			printEventsFlags("On output       ", conf.conf.output_action);
			printEventsFlags("On exec error   ", conf.conf.exec_err_action);
			printf("> Environement     <%lu>\n", conf.conf.env_updates.size());
			printEnvironment(conf);
			printf("\n");
			
			try {
				printf("# Parsing RPN expression...\n");
				for(int ei = 0, el = conf.expr.size() ; ei < el ; ei++) {
					printf("> \"%s\"\n", conf.expr[ei].c_str());
					rpn.parseToken(conf.expr[ei]);
					RunDebug::showStack(rpn);
				}
			}
			catch (std::exception &e) {
				printf("# Catched exception while processing expressions :\n# %s\n", e.what());
				continue;
			}
			
			cont = false;
			if(rpn.queue.size() == 0) {
				printf("# The queue is empty, the result is considered as FALSE.\n");
			}
			else {
				rpn.queue.front()->parse();
				if(rpn.queue.front()->getType() == RpnSimpleToken::TYPE_INT)
					cont = !!rpn.queue.front()->parseAsInt();
				else if(rpn.queue.front()->getType() == RpnSimpleToken::TYPE_FLOAT)
					cont = !!rpn.queue.front()->parseAsFloat();
			}
			
			if(conf.conf.mode == Conf::CONF_MODE_BOOL) {
				if(!cont)
					printf("# The final value evaluates to FALSE. The program will not be executed @%ld.\n", now);
				else
					printf("# The final value evaluates to TRUE. The program will be executed @%ld.\n", now);
			}
			else {
				printf("# The program will be executed in %ld seconds.\n", rpn.queue.front()->parseAsInt());
			}
			
			printf("\n");
			printf("# Commands execution debug :\n");
			for(int i = 0, l = conf.cmds_lines.size() ; i < l ; i++) {
				printf("> %s\n", conf.cmds_lines[i].c_str());
			}
			printf("\n\n");
		}
	}
	
	static void RunDebug::showStack(Rpn<RpnSimpleToken> &rpn) {
		Rpn<RpnSimpleToken>::QueueIterator it;
		Rpn<RpnSimpleToken>::QueueIterator end;
		printf("|");
		
		it = rpn.queue.begin();
		end = rpn.queue.end();
		for(; it != end ; it++) {
			switch((*it)->getType()) {
				case RpnSimpleToken::TYPE_UNPARSED:
					printf(" u<%s>", (*it)->getString().c_str());
					break;
				case RpnSimpleToken::TYPE_STRING:
					printf(" s<%s>", (*it)->parseAsString().c_str());
					break;
				case RpnSimpleToken::TYPE_INT:
					printf(" i<%ld>", (*it)->parseAsInt());
					break;
				case RpnSimpleToken::TYPE_FLOAT:
					printf(" f<%lf>", (*it)->parseAsFloat());
					break;
			}
		}
		
		printf("\n");
	}
	
	static void RunDebug::printEventsFlags(const char *desc, int code) {
		printf("> %s <", desc);
		if(code & Conf::CONF_ON_ERROR_IGNORE) {
			printf("ignore>\n");
			return;
		}
		
		if(code & Conf::CONF_ON_ERROR_LOG) {
			printf("log");
			if(code & Conf::CONF_ON_ERROR_MAIL)
				printf(" mail>\n");
		}
		else if(code & Conf::CONF_ON_ERROR_MAIL) {
			printf("mail>\n");
		}
	}
	
	static void RunDebug::printEnvironment(Conf::ConfEntity &conf) {
		for(int i = 0, l = conf.conf.env_updates.size() ; i < l ; i++) {
			if(conf.conf.env_updates[i].set) {
				printf(">> set             <%s> = <%s>%s\n",
					conf.conf.env_updates[i].key.c_str(),
					conf.conf.env_updates[i].val.c_str(),
					conf.conf.env_updates[i].override ? " override" : ""
				);
			}
			else {
				printf(">> unset           <%s>\n", conf.conf.env_updates[i].key.c_str());
			}
		}
	}
}
