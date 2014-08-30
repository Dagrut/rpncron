#ifndef _HPP_RPNCRON_CONF_CONF_
#define _HPP_RPNCRON_CONF_CONF_

#include "../utils/str_fmt.hpp"

#include "../os/users.hpp"

#include <string>
#include <vector>

#include <exception>

namespace RC {
	namespace Conf {
		class Parser;
		
		typedef void (*LineCallback)(
			Parser* parser,
			const std::string &line
		);
		
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
		
	}
}

#endif
