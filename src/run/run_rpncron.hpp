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

#ifndef _HPP_RPNCRON_RUN_RUN_RPNCRON_
#define _HPP_RPNCRON_RUN_RUN_RPNCRON_

#include "../rpn/rpn.hpp"
#include "../rpn/rpn_simple_token.hpp"
#include "../args/args_rc.hpp"
#include "../conf/conf_parser.hpp"
#include "../utils/ptr_comp.hpp"
#include "../os/processes.hpp"

#include <set>

#include <csignal>

namespace RC {
	class RunRpncron {
		public:
			static void run(ArgsRc &args);
			static void runFile(ArgsRc &args);
			static void runDir(ArgsRc &args);
			
		private:
			struct Task {
				bool is_user;
				int task_id;
				std::string file;
				long exec_time;
				Conf::ConfEntity ce;
				std::set<OS::Processes::ID> childs;
				long exec_count;
				
				bool operator<(const Task &that) const {
					return(this->exec_time < that.exec_time);
				}
			};
			
			typedef std::multiset<Task*, Utils::PtrComp<Task> > TaskSet;
			
			
			RunRpncron(ArgsRc &args);
			
			void loadDirectory(const std::string &path);
			void loadFile(const std::string &path);
			void loadUsers(const std::string &path);
			
			void insertConfs(
				Conf::Parser::Confs &list,
				bool is_user,
				const std::string &file
			);
			
			bool reloadTask(RunRpncron::Task *task, time_t now);
			
			void daemonize();
			void start();
			
			bool processConfModeBool(Task *t);
			bool processConfModeOffset(Task *t);
			void executeTask(Task *t);
			void updateTaskCounter(Task *t);
			void prepareTaskContext(Task *t);
			void executeTaskSystem(Task *t);
			void executeTaskPipe(Task *t);
			
			void initSignals();
			void clearSignals();
			
			void onExecError(Task *t, const char *err);
			void onExecEnd(RunRpncron::Task *t, const std::vector<char> &output, int return_code);
			
			static void processStopNicelyOnSignal(int sig, siginfo_t *info, void *unused);
			static void processChildTerminated(int sig, siginfo_t *info, void *unused);
			static time_t curMin(time_t t);
			static time_t nextMin(time_t t);
			
			time_t now;
			ArgsRc &args;
			
			TaskSet tasks_db;
			
			static bool keep_running;
	};
}

#endif
