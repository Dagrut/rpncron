#ifndef _HPP_RPNCRON_CONF_CONF_CALLBACKS_
#define _HPP_RPNCRON_CONF_CONF_CALLBACKS_

#include "conf.hpp"

namespace RC {
	namespace Conf {
		namespace Callbacks {
			typedef void (*LineConfBlockCallback)(
				ConfEntity *ci,
				const std::string &line
			);
			
			void onParserStart(Parser* parser, const std::string &line);
			void onParserConf(Parser* parser, const std::string &line);
			void onParserRpn(Parser* parser, const std::string &line);
			void onParserCmds(Parser* parser, const std::string &line);
		}
	}
}

#endif
