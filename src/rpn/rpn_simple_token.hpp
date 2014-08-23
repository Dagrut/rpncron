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

#ifndef _HPP_RPNCRON_RPN_RPN_SIMPLE_TOKEN_
#define _HPP_RPNCRON_RPN_RPN_SIMPLE_TOKEN_

#include <string>

namespace RC {
	class RpnSimpleToken {
		public:
			enum TokenType {
				TYPE_UNPARSED,
				TYPE_STRING,
				TYPE_FLOAT,
				TYPE_INT,
			};
			
			RpnSimpleToken(const std::string &val, bool parsed = false);
			RpnSimpleToken(double val);
			RpnSimpleToken(long val);
			
			void reset(const std::string &val, bool parsed = false);
			void reset(double val);
			void reset(long val);
			
			void parse();
			long parseAsInt();
			double parseAsFloat();
			std::string parseAsString();
			std::string getString() const;
			
			RpnSimpleToken* clone() const;
			TokenType getType() const;
			
			static bool getBoolValue(RpnSimpleToken *token);
			
		private:
			TokenType type;
			std::string string_val;
			double float_val;
			long int_val;
	};
};

#endif
