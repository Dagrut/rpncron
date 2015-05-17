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

#include "rpn_simple_token.hpp"

#include <cstring>
#include <cstdio>

#define INTEGER_START_CHARS "+-0123456789"
#define NUMBER_START_CHARS INTEGER_START_CHARS "."
namespace RC {
	
	RpnSimpleToken::RpnSimpleToken(const std::string &val, bool parsed) :
		type(parsed ? TYPE_STRING : TYPE_UNPARSED),
		string_val(val)
	{}
	
	RpnSimpleToken::RpnSimpleToken(double val) :
		type(TYPE_FLOAT),
		float_val(val)
	{}
	
	RpnSimpleToken::RpnSimpleToken(long val) :
		type(TYPE_INT),
		int_val(val)
	{}
	
	void RpnSimpleToken::reset(const std::string &val, bool parsed) {
		this->type = (parsed ? TYPE_STRING : TYPE_UNPARSED);
		this->string_val = (val);
	}
	
	void RpnSimpleToken::reset(double val) {
		this->type = TYPE_FLOAT;
		this->float_val = val;
	}
	
	void RpnSimpleToken::reset(long val) {
		this->type = TYPE_INT;
		this->int_val = val;
	}
	
	RpnSimpleToken* RpnSimpleToken::clone() const {
		switch(this->type) {
			case TYPE_STRING:
				return(new RpnSimpleToken(this->string_val, true));
				break;
			case TYPE_FLOAT:
				return(new RpnSimpleToken(this->float_val));
				break;
			case TYPE_INT:
				return(new RpnSimpleToken(this->int_val));
				break;
			case TYPE_UNPARSED:
				return(new RpnSimpleToken(this->string_val, false));
				break;
		}
		
		return(new RpnSimpleToken(""));
	}
	
	void RpnSimpleToken::parse() {
		if(this->type != TYPE_UNPARSED)
			return;
		
		if(this->string_val.size() == 0) {
			this->type = TYPE_STRING;
			return;
		}
		
		const char *ptr = this->string_val.c_str();
		
		if(strchr(NUMBER_START_CHARS, ptr[0]) == NULL) {
			this->type = TYPE_STRING;
			return;
		}
		
		if(strchr(ptr, '.')) {
			if(sscanf(ptr, "%lf", &this->float_val) == 1) {
				this->type = TYPE_FLOAT;
				return;
			}
			else {
				this->type = TYPE_STRING;
				return;
			}
		}
		else {
			if(sscanf(ptr, "%ld", &this->int_val) == 1) {
				this->type = TYPE_INT;
				return;
			}
			else {
				this->type = TYPE_STRING;
				return;
			}
		}
	}
	
	long RpnSimpleToken::parseAsInt() {
		if(this->type == TYPE_INT)
			return(this->int_val);
		else if(this->type == TYPE_FLOAT) {
			this->type = TYPE_INT;
			this->int_val = this->float_val;
			return(this->int_val);
		}
		
		this->int_val = 0;
		
		if(this->string_val.size() == 0) {
			this->type = TYPE_STRING;
			return(this->int_val);
		}
		
		const char *ptr = this->string_val.c_str();
		
		if(strchr(INTEGER_START_CHARS, ptr[0]) == NULL) {
			this->type = TYPE_STRING;
			return(this->int_val);
		}
		
		if(sscanf(ptr, "%ld", &this->int_val) == 1) {
			this->type = TYPE_INT;
			return(this->int_val);
		}
		else {
			this->type = TYPE_STRING;
			return(this->int_val);
		}
		
		return(this->int_val);
	}
	
	double RpnSimpleToken::parseAsFloat() {
		if(this->type == TYPE_FLOAT)
			return(this->float_val);
		else if(this->type == TYPE_INT) {
			this->type = TYPE_FLOAT;
			this->float_val = this->int_val;
			return(this->float_val);
		}
		
		this->float_val = 0.0;
		
		if(this->string_val.size() == 0) {
			this->type = TYPE_STRING;
			return(this->float_val);
		}
		
		const char *ptr = this->string_val.c_str();
		
		if(strchr(NUMBER_START_CHARS, ptr[0]) == NULL) {
			this->type = TYPE_STRING;
			return(this->float_val);
		}
		
		if(sscanf(ptr, "%lf", &this->float_val) == 1) {
			this->type = TYPE_FLOAT;
			return(this->float_val);
		}
		else {
			this->type = TYPE_STRING;
			return(this->float_val);
		}
		
		return(this->float_val);
	}
	
	std::string RpnSimpleToken::parseAsString() {
		if(this->type == TYPE_STRING) {
			return(this->string_val);
		}
		else if(this->type == TYPE_UNPARSED) {
			this->type = TYPE_STRING;
			return(this->string_val);
		}
		else if(this->type == TYPE_INT) {
			char buffer[1024];
			snprintf(buffer, sizeof(buffer), "%ld", this->int_val);
			this->string_val = buffer;
			this->type = TYPE_STRING;
			return(this->string_val);
		}
		else if(this->type == TYPE_FLOAT) {
			char buffer[1024];
			snprintf(buffer, sizeof(buffer), "%lf", this->float_val);
			this->string_val = buffer;
			this->type = TYPE_STRING;
			return(this->string_val);
		}
		
		return("");
	}
	
	std::string RpnSimpleToken::getString() const {
		if(this->type == TYPE_STRING || this->type == TYPE_UNPARSED)
			return(this->string_val);
		return("");
	}
	
	RpnSimpleToken::TokenType RpnSimpleToken::getType() const {
		return(this->type);
	}
	
	bool RpnSimpleToken::getBoolValue(RpnSimpleToken *token) {
		if(token->type == TYPE_UNPARSED) {
			token->parse();
		}
		
		if(token->type == TYPE_STRING) {
			return(token->string_val.size() > 0);
		}
		else if(token->type == TYPE_INT) {
			return(token->int_val);
		}
		else if(token->type == TYPE_FLOAT) {
			return(token->float_val);
		}
		
		return(false);
	}
}
