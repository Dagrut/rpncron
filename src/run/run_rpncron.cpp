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

#include "run_rpncron.hpp"

#include "../rpn/functions/rpn_functions_arith_elem.hpp"
#include "../rpn/functions/rpn_functions_comp.hpp"
#include "../rpn/functions/rpn_functions_logic.hpp"
#include "../rpn/functions/rpn_functions_types.hpp"
#include "../rpn/functions/rpn_functions_queue.hpp"
#include "../rpn/functions/rpn_functions_branching.hpp"
#include "../rpn/functions/rpn_functions_misc.hpp"
#include "../rpn/functions/rpn_functions_time.hpp"

#include "../os/processes.hpp"
#include "../os/file.hpp"
#include "../os/directory.hpp"
#include "../os/environment.hpp"

#include "../os/logs.hpp"
#include "../os/mail.hpp"

#include "../rpncron.hpp"

#include <cstdio>
#include <cstdlib>


bool RC::RunRpncron::keep_running;

namespace RC {
	void RunRpncron::prepare(Rpn<RpnSimpleToken> &rpn, long now, int proc_count, long exec_count) {
		rpn.clearVariables();
		rpn.clearFunctions();
		rpn.clearQueue();
		
		RunRpncron::prepareProcVars(rpn, proc_count, exec_count);
		RunRpncron::prepareTimeVars(rpn, now);
		RunRpncron::prepareFunctions(rpn);
	}
	
	void RunRpncron::prepareTimeVars(Rpn<RpnSimpleToken> &rpn, long now) {
		rpn.addVariable("time_sec",  new RpnSimpleToken(now));
		rpn.addVariable("time_min",  new RpnSimpleToken(now/60));
		rpn.addVariable("time_hour", new RpnSimpleToken(now/3600));
		rpn.addVariable("time_day",  new RpnSimpleToken(now/86400));
		
		struct tm date;
		time_t now_time = now;
		
		localtime_r(&now_time, &date);
		
		rpn.addVariable("time_SoM",    new RpnSimpleToken((long) date.tm_sec));
		rpn.addVariable("time_MoH",    new RpnSimpleToken((long) date.tm_min));
		rpn.addVariable("time_Hod",    new RpnSimpleToken((long) date.tm_hour));
		rpn.addVariable("time_dow",    new RpnSimpleToken((long) date.tm_wday));
		rpn.addVariable("time_dom",    new RpnSimpleToken((long) date.tm_mday));
		rpn.addVariable("time_moy",    new RpnSimpleToken((long) date.tm_mon));
		rpn.addVariable("time_doy",    new RpnSimpleToken((long) date.tm_yday));
		rpn.addVariable("time_year",   new RpnSimpleToken((long) date.tm_year + 1900));
		rpn.addVariable("time_is_dst", new RpnSimpleToken((long) date.tm_isdst));
	}
	
	void RunRpncron::prepareProcVars(Rpn<RpnSimpleToken> &rpn, int proc_count, long exec_count) {
		rpn.addVariable("proc_count",  new RpnSimpleToken((long) proc_count));
		rpn.addVariable("exec_count",  new RpnSimpleToken(exec_count));
	}
	
	void RunRpncron::prepareFunctions(Rpn<RpnSimpleToken> &rpn) {
		RpnFunctionsArithElem::load(rpn);
		RpnFunctionsComp::load(rpn);
		RpnFunctionsLogic::load(rpn);
		RpnFunctionsTypes::load(rpn);
		RpnFunctionsQueue::load(rpn);
		RpnFunctionsBranching::load(rpn);
		RpnFunctionsMisc::load(rpn);
		RpnFunctionsTime::load(rpn);
	}
	
	void RunRpncron::run(ArgsRc &args) {
		RunRpncron me(args);
		me.daemonize();
		me.start();
	}
	
	void RunRpncron::runFile(ArgsRc &args) {
		RunRpncron me(args);
		me.daemonize();
		me.start();
	}
	
	void RunRpncron::runDir(ArgsRc &args) {
		RunRpncron me(args);
		me.daemonize();
		me.start();
	}
	
	RunRpncron::RunRpncron(ArgsRc &args) : args(args) {
		ArgsRc::ConfAction action;
		time(&this->now);
		
		action = args.getAction();
		if(action == ArgsRc::ACTION_RUN) {
			this->loadDirectory(RPNCRON_PROGRAM_SYSTEM_PATH);
			this->loadUsers(RPNCRON_PROGRAM_USERS_PATH);
		}
		else if(action == ArgsRc::ACTION_RUN_FILE) {
			this->loadFile(args.getRunPath());
		}
		else if(action == ArgsRc::ACTION_RUN_DIR) {
			this->loadDirectory(args.getRunPath());
		}
		else {
			RPNCRON_PROGRAM_FATAL_MSG
		}
	}
	
	void RunRpncron::loadDirectory(const std::string &path) {
		OS::Directory dir;
		OS::File fstats;
		std::string dir_item;
		std::string dir_item_path;
		Conf::Parser::Confs tmp_conf_list;
		
		if(dir.open(path) == true) {
			while(dir.next(dir_item) != false) {
				if(dir_item == "." || dir_item == "..")
					continue;
				
				dir_item_path = path + "/" + dir_item;
				fstats.open(dir_item_path);
				
				if(!fstats.isFile())
					continue;
				
				tmp_conf_list.clear();
				try {
					Conf::Parser::parseFile(dir_item_path, tmp_conf_list);
				}
				catch(Conf::ParseError &e) {
					DEBUG("Parse error on file %s : %s\n", dir_item_path.c_str(), e.what());
					continue;
				}
				
				this->insertConfs(tmp_conf_list, false, dir_item_path);
			}
			
			dir.close();
		}
	}
	
	void RunRpncron::loadFile(const std::string &path) {
		OS::File fstats;
		Conf::Parser::Confs tmp_conf_list;
		
		fstats.open(path);
		
		if(!fstats.isFile()) {
			//printf("DEBUG: The given element %s is not a file!\n", path.c_str());
			return;
		}
		
		tmp_conf_list.clear();
		try {
			Conf::Parser::parseFile(path, tmp_conf_list);
		}
		catch(Conf::ParseError &e) {
			DEBUG("Parse error on file %s : %s\n", path.c_str(), e.what());
			return;
		}
		
		this->insertConfs(tmp_conf_list, false, path);
	}
	
	void RunRpncron::loadUsers(const std::string &path) {
		OS::Directory dir;
		OS::File fstats;
		std::string dir_item;
		std::string dir_item_path;
		Conf::Parser::Confs tmp_conf_list;
		
		if(dir.open(path) == true) {
			while(dir.next(dir_item) != false) {
				if(dir_item == "." || dir_item == "..")
					continue;
				
				try {
					OS::Users::userNameToUID(dir_item);
				}
				catch(OS::SystemError &e) {
					DEBUG(
						"Could not load rpncrontab of user %s : "
						"User does not exist!\n",
						dir_item.c_str()
					);
					continue;
				}
				
				dir_item_path = path + "/" + dir_item;
				fstats.open(dir_item_path);
				
				if(!fstats.isFile())
					continue;
				
				tmp_conf_list.clear();
				try {
					Conf::Parser::parseFile(dir_item_path, tmp_conf_list);
				}
				catch(Conf::ParseError &e) {
					DEBUG("Parse error on file %s : %s\n", dir_item_path.c_str(), e.what());
					continue;
				}
				
				this->insertConfs(tmp_conf_list, true, dir_item);
			}
			
			dir.close();
		}
	}
	
	void RunRpncron::insertConfs(
		Conf::Parser::Confs &list,
		bool is_user,
		const std::string &identifier
	) {
		Task *task = NULL;
		
		for(int i = 0, l = list.size() ; i < l ; i++) {
			if(task == NULL)
				task = new Task();
			task->is_user = is_user;
			task->identifier = identifier;
			task->ce = list[i];
			task->task_id = i;
			task->exec_count = 0;
			if(this->reloadTask(task, this->now)) {
				this->tasks_db.insert(task);
				task = NULL;
			}
		}
		
		if(task != NULL)
			delete task;
	}
	
	bool RunRpncron::reloadTask(RunRpncron::Task *task, time_t now) {
		Rpn<RpnSimpleToken> rpn;
		
		RunRpncron::prepare(rpn, now, task->childs.size(), task->exec_count);
		
		try {
			for(int i = 0, l = task->ce.expr.size() ; i < l ; i++) {
				rpn.parseToken(task->ce.expr[i]);
			}
		}
		catch(std::exception &e) {
			DEBUG("Error: could not parse configuration %d of %s %s : %s\n",
				task->task_id + 1,
				task->is_user ? "user" : "file",
				task->identifier.c_str(),
				e.what()
			);
			return(false);
		}
		
		if(rpn.result_size() == 0) {
			if(task->ce.conf.mode == Conf::CONF_MODE_OFFSET) {
				DEBUG("Task %d of %s %s have an empty ouput. Ignoring task!\n",
					task->task_id + 1,
					task->is_user ? "user" : "file",
					task->identifier.c_str()
				);
				return(false);
			}
			else {
				task->exec_time = RunRpncron::nextMin(now);
				return(true);
			}
		}
		
		if(task->ce.conf.mode == Conf::CONF_MODE_OFFSET)
			task->exec_time = now + rpn.result()->parseAsInt();
		else
			task->exec_time = RpnSimpleToken::getBoolValue(rpn.result()) ? RunRpncron::curMin(now) : RunRpncron::nextMin(now);
		
		return(true);
	}
	
	void RunRpncron::daemonize() {
		if(this->args.getDaemonize()) {
			try {
				OS::Processes::daemonize(true, true);
			}
			catch(OS::SystemError &e) {
				FATAL("Could not daemonize : %s\n", e.what());
			}
		}
	}
	
	void RunRpncron::start() {
		TaskSet::iterator it;
		TaskSet::iterator it2;
		bool ret;
		int sleep_sec;
		RunRpncron::Task *t;
		
		if(this->tasks_db.size() == 0) {
			DEBUG("No task found, leaving.\n");
			return;
		}
		
		RunRpncron::keep_running = true;
		this->initSignals();
		
		while(RunRpncron::keep_running) {
			time(&this->now);
			
			it = this->tasks_db.begin();
			while(it != this->tasks_db.end()) {
				if((*it)->exec_time > this->now)
					break;
				
				t = *it;
				it2 = it;
				it2++;
				
				if((*it)->ce.conf.mode == Conf::CONF_MODE_BOOL) {
					this->tasks_db.erase(it);
					ret = this->processConfModeBool(t);
					if(ret)
						this->tasks_db.insert(it2, t);
					else
						delete t;
				}
				else if((*it)->ce.conf.mode == Conf::CONF_MODE_OFFSET) {
					this->tasks_db.erase(it);
					ret = this->processConfModeOffset(t);
					if(ret)
						this->tasks_db.insert(it2, t);
					else
						delete t;
				}
				else {
					RPNCRON_PROGRAM_FATAL_MSG
				}
				
				it = it2;
			}
			
			it = this->tasks_db.begin();
			t = *it;
			
			time(&this->now);
			
			if(t->exec_time <= this->now)
				sleep_sec = 1;
			else
				sleep_sec = t->exec_time - this->now;
			
			DEBUG("Sleeping for %d sec\n\n", sleep_sec);
			sleep(sleep_sec);
		}
	}
	
	void RunRpncron::initSignals() {
		struct sigaction action;
		action.sa_handler = NULL;
		action.sa_sigaction = RunRpncron::processStopNicelyOnSignal;
		action.sa_restorer = NULL;
		sigemptyset(&action.sa_mask);
		sigaddset(&action.sa_mask, SIGINT);
		action.sa_flags = SA_SIGINFO;
		
		sigaction(SIGINT, &action, NULL);
		
		action.sa_sigaction = RunRpncron::processChildTerminated;
		sigemptyset(&action.sa_mask);
		sigaddset(&action.sa_mask, SIGCHLD);
		
		sigaction(SIGCHLD, &action, NULL);
	}
	
	void RunRpncron::clearSignals() {
		struct sigaction action;
		action.sa_handler = SIG_DFL;
		action.sa_sigaction = NULL;
		action.sa_restorer = NULL;
		sigemptyset(&action.sa_mask);
		sigaddset(&action.sa_mask, SIGINT);
		action.sa_flags = 0;
		
		sigaction(SIGINT, &action, NULL);
		
		action.sa_handler = SIG_DFL;
		sigemptyset(&action.sa_mask);
		sigaddset(&action.sa_mask, SIGCHLD);
		
		sigaction(SIGCHLD, &action, NULL);
	}
	
	bool RunRpncron::processConfModeBool(RunRpncron::Task *t) {
		bool ret;
		
		this->updateTaskCounter(t);
		
		ret = this->reloadTask(t, this->now);
		if(ret) {
			if(t->exec_time <= this->now) {
				if(t->childs.size() < t->ce.conf.max_proc || t->ce.conf.max_proc == 0) {
					this->executeTask(t);
				}
				t->exec_time += 60;
			}
			return(true);
		}
		
		return(false);
	}
	
	bool RunRpncron::processConfModeOffset(RunRpncron::Task *t) {
		this->updateTaskCounter(t);
		if(t->childs.size() < t->ce.conf.max_proc || t->ce.conf.max_proc == 0) {
			this->executeTask(t);
		}
		return(this->reloadTask(t, this->now));
	}
	
	void RunRpncron::executeTask(RunRpncron::Task *t) {
		OS::Processes::ID id;
		
		t->exec_count++;
		
		DEBUG(
			"Executing task %d of %s %s (CNT=%ld) @",
			t->task_id + 1,
			t->is_user ? "user" : "file",
			t->identifier.c_str(),
			t->childs.size()
		);
		fflush(stdout);
		system("date");
		
		id = OS::Processes::fork();
		if(id == -1) {
			DEBUG("Error while calling fork()!\n");
			return;
		}
		
		if(id != 0) {
			t->childs.insert(id);
			return;
		}
		
		this->clearSignals();
		
		this->prepareTaskContext(t);
		
		if(t->ce.conf.exec_mode == Conf::CONF_EXEC_MODE_SYSTEM) {
			this->executeTaskSystem(t);
		}
		else if(t->ce.conf.exec_mode == Conf::CONF_EXEC_MODE_PIPE) {
			this->executeTaskPipe(t);
		}
		else {
			RPNCRON_PROGRAM_FATAL_MSG
		}
		
		exit(0);
	}
	
	void RunRpncron::updateTaskCounter(RunRpncron::Task *t) {
		std::set<OS::Processes::ID>::iterator it;
		std::set<OS::Processes::ID>::iterator it2;
		std::set<OS::Processes::ID>::iterator end;
		OS::Processes::ID pid;
		const OS::Processes::ID self_pid = OS::Processes::getPid();
		
		it = t->childs.begin();
		end = t->childs.end();
		while(it != end) {
			try {
				pid = OS::Processes::getPPidOfPid(*it);
			}
			catch(OS::SystemError &e) {
				pid = -1;
			}
			
			if(pid != self_pid) {
				it2 = it++;
				t->childs.erase(it2);
			}
			else {
				it++;
			}
		}
	}
	
	void RunRpncron::prepareTaskContext(RunRpncron::Task *t) {
		for(int i = 0, l = t->ce.conf.env_updates.size() ; i < l ; i++) {
			if(t->ce.conf.env_updates[i].set) {
				OS::Environment::set(
					t->ce.conf.env_updates[i].key, 
					t->ce.conf.env_updates[i].val,
					t->ce.conf.env_updates[i].override
				);
			}
			else {
				OS::Environment::unset(t->ce.conf.env_updates[i].key);
			}
		}
		
		OS::Environment::setCwd(t->ce.conf.cwd);
		try {
			if(OS::Users::getCurrentUserID() != 0) {
				OS::Processes::setgid(t->ce.conf.group);
				OS::Processes::setuid(t->ce.conf.user);
			} 
		}
		catch(std::exception &e) {}
	}
	
	void RunRpncron::executeTaskSystem(RunRpncron::Task *t) {
		for(int i = 0, l = t->ce.cmds_lines.size() ; i < l ; i++) {
			system(t->ce.cmds_lines[i].c_str());
		}
	}
	
	void RunRpncron::executeTaskPipe(RunRpncron::Task *t) {
		std::vector<char> output;
		int process_retval;
		
		try {
			process_retval = OS::Processes::popenPipe(
				t->ce.cmds_block,
				output,
				262144,
				t->ce.conf.shell[0],
				t->ce.conf.shell,
				NULL
			);
		}
		catch(OS::SystemError &error) {
			this->onExecError(t, error.what());
			DEBUG("Error when executing task %d of %s : %s\n",
				t->task_id,
				t->identifier.c_str(),
				error.what()
			);
			return;
		}
		catch(OS::SystemWarning &warning) {
			this->onExecError(t, warning.what());
			DEBUG("Received warning when executing task %d of %s : %s\n",
				t->task_id,
				t->identifier.c_str(),
				warning.what()
			);
			return;
		}
		
		this->onExecEnd(t, output, process_retval);
		output.push_back('\0');
		DEBUG("popenPipe returned %d output=(%lu bytes) %s\n", process_retval, output.size() - 1, output.data());
	}
	
	void RunRpncron::onExecError(RunRpncron::Task *t, const char *err) {
		if(t->ce.conf.exec_err_action & Conf::CONF_ON_ERROR_IGNORE)
			return;
		
		if(t->ce.conf.exec_err_action & Conf::CONF_ON_ERROR_LOG) {
			Logs::log(Logs::LVL_WARN, "Error when executing task %d of %s : %s",
				t->task_id,
				t->identifier.c_str(),
				err
			);
		}
		
		if(t->ce.conf.exec_err_action & Conf::CONF_ON_ERROR_MAIL) {
			OS::Mail toSend;
			std::string body;
			
			toSend.addHeader(
				"Subject",
				std::string("[RPNCRON] Command execution error of task ") +
					Utils::format("%d", t->task_id) + " / " + t->identifier
			);
			
			body = "An error was encountered when executing task ";
			body += Utils::format("%d", t->task_id);
			body += " of ";
			body += t->identifier;
			body += " : ";
			body += err;
			
			toSend.appendToBody(body);
			
			toSend.send("root <root@localhost>");
		}
	}
	
	void RunRpncron::onExecEnd(RunRpncron::Task *t, const std::vector<char> &output, int return_code) {
		if(output.size() == 0 && return_code == 0)
			return;
		
		if(
			t->ce.conf.code_err_action & Conf::CONF_ON_ERROR_IGNORE &&
			t->ce.conf.output_action & Conf::CONF_ON_ERROR_IGNORE
		)
			return;
		
		bool is_output;
		bool is_retcode;
		is_output =
			output.size() != 0 &&
			t->ce.conf.output_action & Conf::CONF_ON_ERROR_LOG &&
			!(t->ce.conf.output_action & Conf::CONF_ON_ERROR_IGNORE)
			;
		is_retcode =
			return_code != 0 &&
			t->ce.conf.code_err_action & Conf::CONF_ON_ERROR_LOG &&
			!(t->ce.conf.code_err_action & Conf::CONF_ON_ERROR_IGNORE);
		
		if(is_retcode) {
			Logs::log(Logs::LVL_WARN, "Task %d of %s failed with returned code %d",
				t->task_id,
				t->identifier.c_str(),
				return_code
			);	
		}
		else if(is_output) {
			Logs::log(Logs::LVL_WARN, "Task %d of %s printed an output of %d bytes",
				t->task_id,
				t->identifier.c_str(),
				output.size()
			);
		}
		
		is_output =
			output.size() != 0 &&
			t->ce.conf.output_action & Conf::CONF_ON_ERROR_MAIL &&
			!(t->ce.conf.output_action & Conf::CONF_ON_ERROR_IGNORE);
		is_retcode =
			return_code != 0 &&
			t->ce.conf.code_err_action & Conf::CONF_ON_ERROR_MAIL &&
			!(t->ce.conf.code_err_action & Conf::CONF_ON_ERROR_IGNORE);
		
		if(is_output || is_retcode) {
			OS::Mail toSend;
			
			toSend.addHeader(
				"Subject",
				std::string("[RPNCRON] Execution error of task ") +
					Utils::format("%d", t->task_id) + " / " + t->identifier
			);
			
			toSend.appendToBody("The task ");
			toSend.appendToBody(Utils::format("%d", t->task_id));
			toSend.appendToBody(" of ");
			toSend.appendToBody(t->identifier);
			toSend.appendToBody(" exited with return code ");
			toSend.appendToBody(Utils::format("%d", return_code));
			toSend.appendToBody(" and an output of ");
			toSend.appendToBody(Utils::format("%lu", (long)output.size()));
			toSend.appendToBody(" bytes :\n");
			toSend.appendToBody(std::string(output.data(), output.size()));
			
			toSend.send("root <root@localhost>");
		}
	}
	
	void RunRpncron::processStopNicelyOnSignal(int sig, siginfo_t *info, void *unused) {
		RunRpncron::keep_running = false;
	}
	
	void RunRpncron::processChildTerminated(int sig, siginfo_t *info, void *unused) {
		OS::Processes::acceptChildDeath();
	}
	
	time_t RunRpncron::curMin(time_t t) {
		t /= 60;
		t *= 60;
		return(t);
	}
	
	time_t RunRpncron::nextMin(time_t t) {
		return(RunRpncron::curMin(t) + 60);
	}
}
