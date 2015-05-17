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

#include "processes.hpp"

#include "../utils/minmax.hpp"
#include "../utils/str_fmt.hpp"

#include <fstream>

#include <cstdlib>
#include <cstring>

#include <sys/types.h>

#include <cctype>

#include <unistd.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/select.h>


namespace RC {
	int OS::Processes::system(const std::string &cmd) {
		return(::system(cmd.c_str()));
	}
	
	OS::Processes::ID OS::Processes::fork() {
		OS::Processes::ID id;
		
		id = ::fork();
		if(id == -1) {
			throw SystemError("Error when calling fork()!");
		}
		
		return(id);
	}
	
	void OS::Processes::daemonize(bool reset_pwd, bool close_io) {
		if(::daemon(reset_pwd ? 0 : 1, close_io ? 0 : 1) != 0) {
			throw SystemError("Could not daemonize the current process");
		}
	}
	
	void OS::Processes::exec(
		const std::string &cmd,
		const std::vector<std::string> &args,
		const std::vector<std::string> *env
	) {
		char **char_args;
		char **env_args;
		
		char_args = new char*[args.size() + 1];
		for(int i = 0, l = args.size() ; i < l ; i++) {
			char_args[i] = strdup(args[i].c_str());
		}
		char_args[args.size()] = NULL;
		
		if(env) {
			env_args = new char*[env->size()];
			for(int i = 0, l = env->size() ; i < l ; i++) {
				env_args[i] = strdup((*env)[i].c_str());
			}
		}
		else {
			env_args = environ;
		}
		
		::execve(cmd.c_str(), char_args, env_args);
		
		throw SystemError(std::string("Error while calling execve(") + cmd + ")!");
	}
	
	static void osProcessFdFlags(int fd, int flags_add, int flags_del) {
		int flags;
		flags = fcntl(fd, F_GETFL, 0);
		if(flags != -1)
			fcntl(fd, F_SETFL, (flags & ~flags_del) | flags_add);
	}
	
	int OS::Processes::popenPipe(
		const std::vector<char> &input,
		std::vector<char> &output,
		int max_output,
		const std::string &cmd,
		const std::vector<std::string> &args,
		const std::vector<std::string> *env
	) {
		unsigned int input_offset = 0;
		std::string error_str("");
		std::string warn_str("");
		int process_retval;
		
		OS::Processes::ID id;
		int w_io[2] = {-1, -1}; //stdin
		int r_io[2] = {-1, -1}; //stdout, stderr
		int e_io[2] = {-1, -1}; //used internally, for errors.
		int ret;
		
		ret = pipe(w_io);
		if(ret == -1) {
			error_str = "Error when calling pipe()";
			goto fn_end;
		}
		
		ret = pipe(r_io);
		if(ret == -1) {
			error_str = "Error when calling pipe()";
			goto fn_end;
		}
		
		ret = pipe(e_io);
		if(ret == -1) {
			error_str = "Error when calling pipe()";
			goto fn_end;
		}
		
		osProcessFdFlags(r_io[0], O_NONBLOCK | FD_CLOEXEC, 0);
		osProcessFdFlags(r_io[1], FD_CLOEXEC, 0);
		osProcessFdFlags(w_io[0], FD_CLOEXEC, 0);
		osProcessFdFlags(w_io[1], O_NONBLOCK | FD_CLOEXEC, 0);
		osProcessFdFlags(e_io[0], FD_CLOEXEC, 0);
		osProcessFdFlags(e_io[1], FD_CLOEXEC, 0);
		
		id = OS::Processes::fork();
		if(id == -1) {
			error_str = "Error when calling fork()";
			goto fn_end;
		}
		
		if(id == 0) {
			close(r_io[0]);
			close(w_io[1]);
			close(e_io[0]);
			r_io[0] = -1;
			w_io[1] = -1;
			e_io[0] = -1;
			
			if(
				dup2(r_io[1], 1) == -1 ||
				dup2(r_io[1], 2) == -1 ||
				dup2(w_io[0], 0) == -1
			) {
				error_str = "Error when calling dup2()";
				goto fn_end;
			}
			
			try {
				OS::Processes::exec(cmd, args);
			}
			catch(OS::SystemError &e) {
				write(e_io[1], e.what(), strlen(e.what()));
				exit(1);
			}
			//Will never reach here
		}
		else {
			int maxfd = RPNCRON_UTILS_MAX3(r_io[0], w_io[1], e_io[0]) + 1;
			struct timeval tv;
			fd_set rfds;
			fd_set wfds;
			int proc_status;
			OS::Processes::ID ret_pid;
			int length;
			int wpid_opts = WNOHANG;
			char read_buffer[1024];
			
			close(r_io[1]);
			close(w_io[0]);
			close(e_io[1]);
			r_io[1] = -1;
			w_io[0] = -1;
			e_io[1] = -1;
			
			if(input.size() == 0) {
				close(w_io[1]);
				w_io[1] = -1;
			}
			
			while(true) {
				tv.tv_sec = 1;
				tv.tv_usec = 0;
				
				FD_ZERO(&rfds);
				if(r_io[0] != -1) {
					FD_SET(r_io[0], &rfds);
				}
				FD_SET(e_io[0], &rfds);
				FD_ZERO(&wfds);
				if(w_io[1] != -1) {
					FD_SET(w_io[1], &wfds);
				}
				
				ret = select(maxfd, &rfds, &wfds, NULL, &tv);
				if (ret == -1) {
					if(warn_str.size() == 0)
						warn_str = "Error while calling select()";
				}
				else if(ret > 0) {
					if(FD_ISSET(w_io[1], &wfds)) {
						length = input.size() - input_offset;
						ret = write(w_io[1], input.data() + input_offset, length);
						if(ret == -1) {
							if(warn_str.size() == 0)
								warn_str = "Error while calling write()";
						}
						else if(ret >= 0) {
							input_offset += ret;
							if(input_offset == input.size()) {
								close(w_io[1]);
								w_io[1] = -1;
								maxfd = RPNCRON_UTILS_MAX3(r_io[0], w_io[1], e_io[0]) + 1;
							}
						}
					}
					
					if(FD_ISSET(r_io[0], &rfds)) {
						ret = read(r_io[0], read_buffer, sizeof(read_buffer));
						if(ret == -1) {
							if(warn_str.size() == 0)
								warn_str = "Error while calling read()";
						}
						else if(ret >= 0) {
							length = max_output - output.size();
							if(length > 0) {
								if(length > ret)
									length = ret;
								
								for(int i = 0 ; i < length ; i++)
									output.push_back(read_buffer[i]);
							}
						}
					}
					
					if(FD_ISSET(e_io[0], &rfds)) {
						ret = read(e_io[0], read_buffer, sizeof(read_buffer));
						if(ret == -1) {
							if(error_str.size() == 0)
								error_str = "Fatal error while calling read() (Error flow was not empty!)";
							
							process_retval = RPNCRON_OS_PROCESS_POPENPIPE_EXECUTION_ERROR;
							wpid_opts = 0;
						}
						else if(ret > 0) {
							error_str = std::string(read_buffer, ret);
							process_retval = RPNCRON_OS_PROCESS_POPENPIPE_EXECUTION_ERROR;
							wpid_opts = 0;
						}
					}
				}
				
				ret_pid = waitpid(id, &proc_status, wpid_opts);
				if(ret_pid == -1) {
					if(warn_str.size() == 0)
						warn_str = "Error while calling waitpid()";
				}
				else if(ret_pid == id) {
					process_retval = WEXITSTATUS(proc_status);
					break;
				}
			}
		}
		
		fn_end:
		
		if(r_io[0] != -1)
			close(r_io[0]);
		if(r_io[1] != -1)
			close(r_io[1]);
		if(w_io[0] != -1)
			close(w_io[0]);
		if(w_io[1] != -1)
			close(w_io[1]);
		if(e_io[0] != -1)
			close(e_io[0]);
		if(e_io[1] != -1)
			close(e_io[1]);
		
		if(error_str.size() > 0)
			throw SystemError(error_str);
		
		if(warn_str.size() > 0)
			throw SystemWarning(warn_str);
		
		return(process_retval);
	}
	
	bool OS::Processes::setuid(OS::Users::UserID id) {
		return(::setuid(id) == 0);
	}
	
	bool OS::Processes::setgid(OS::Users::GroupID id) {
		return(::setgid(id) == 0);
	}
	
	OS::Processes::ID OS::Processes::getPPidOfPid(OS::Processes::ID id) {
		std::string info_file = Utils::format("/proc/%d/status", id);
		std::ifstream ifs;
		std::string buffer;
		
		ifs.open(info_file.c_str());
		if(!ifs.is_open()) {
			return(-1);
		}
		
		while(std::getline(ifs, buffer, '\n')) {
			if(strncasecmp(buffer.c_str(), "ppid:", 5) == 0) {
				const char *ptr = buffer.c_str() + 5;
				
				while(ptr != '\0') {
					if(isdigit(*ptr))
						break;
					
					ptr++;
				}
				
				if(!isdigit(*ptr))
					throw SystemError(std::string("Bad format for Ppid line in ") + info_file);
				
				return(Utils::parse<long>("%ld", ptr));
			}
		}
		
		throw SystemError(std::string("Ppid line not found in ") + info_file);
	}
	
	OS::Processes::ID OS::Processes::getPid() {
		return(getpid());
	}
	
	OS::Processes::ID OS::Processes::getPPid() {
		return(getppid());
	}
	
	void OS::Processes::acceptChildDeath() {
		while(waitpid(-1, NULL, WNOHANG) > 0);
	}
}
