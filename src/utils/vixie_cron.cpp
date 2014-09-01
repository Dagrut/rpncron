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

#include "vixie_cron.hpp"

#include "../rpn/rpn.hpp"

#include <cstdlib>

namespace RC {
	static void rangeSet(std::vector<bool> &range, bool val);
	
	Utils::VixieCron::Entry::Entry() : reboot(false) {
		this->minutes.resize(60, false);
		this->hours.resize(24, false);
		this->doms.resize(31, false);
		this->dows.resize(8, false);
		this->months.resize(12, false);
	}
	
	bool Utils::VixieCron::Entry::parseString(const std::string &str) {
		if(str.size() < 6)
			return(false);
		
		if(str[0] == '@') {
			return(this->parseKeyword(str.substr(1)));
		}
		else {
			return(this->parseFields(str));
		}
		
		return(true);
	}
	
	bool Utils::VixieCron::Entry::parseKeyword(const std::string &kw) {
		rangeSet(this->minutes, false);
		rangeSet(this->hours, false);
		rangeSet(this->doms, false);
		rangeSet(this->dows, false);
		rangeSet(this->months, false);
		this->minutes[0] = true;
		
		if(kw.compare(0, 6, "reboot") == 0) {
			this->reboot = true;
		}
		else if(kw.compare(0, 6, "yearly") == 0) {
			this->hours[0] = true;
			this->doms[0] = true;
			this->months[0] = true;
			rangeSet(this->dows, true);
		}
		else if(kw.compare(0, 7, "monthly") == 0) {
			this->hours[0] = true;
			this->doms[0] = true;
			this->months[0] = true;
			rangeSet(this->dows, true);
		}
		else if(kw.compare(0, 6, "weekly") == 0) {
			this->hours[0] = true;
			this->dows[0] = true;
			rangeSet(this->doms, true);
			rangeSet(this->months, true);
		}
		else if(kw.compare(0, 5, "daily") == 0) {
			this->hours[0] = true;
			rangeSet(this->doms, true);
			rangeSet(this->dows, true);
			rangeSet(this->months, true);
		}
		else if(kw.compare(0, 6, "hourly") == 0) {
			rangeSet(this->hours, true);
			rangeSet(this->doms, true);
			rangeSet(this->dows, true);
			rangeSet(this->months, true);
		}
		else {
			return(false);
		}
		
		return(true);
	}
	
	bool Utils::VixieCron::Entry::parseFields(const std::string &fields) {
		std::vector<std::string> tokens;
		bool ret = true;
		
		Rpn<void>::parse(fields, tokens, " \t", '\\', "", "");
		if(tokens.size() != 5)
			return(false);
		
		rangeSet(this->minutes, false);
		rangeSet(this->hours, false);
		rangeSet(this->doms, false);
		rangeSet(this->dows, false);
		rangeSet(this->months, false);
		
		ret = ret && this->parseField(tokens[0], this->minutes);
		
		ret = ret && this->parseField(tokens[1], this->hours);
		
		ret = ret && this->parseField(tokens[2], this->doms, 1);
		
		ret = ret && this->parseField(tokens[3], this->months, 1);
		
		ret = ret && this->parseField(tokens[4], this->dows);
		
		if(ret) {
			this->dows[0] = this->dows[7] = this->dows[0] || this->dows[7];
		}
		
		return(ret);
	}
	
	bool Utils::VixieCron::Entry::parseField(
		const std::string &field,
		std::vector<bool> &out,
		int min
	) {
		std::vector<std::string> comma_values;
		
		Rpn<void>::parse(field, comma_values, ",", '\\', "", "");
		
		for(int i = 0, l = comma_values.size() ; i < l ; i++) {
			if(this->parseSubfield(comma_values[i], out, min) == false)
				return(false);
		}
		
		return(true);
	}
	
	bool Utils::VixieCron::Entry::parseSubfield(
		const std::string &subfield,
		std::vector<bool> &out,
		int min
	) {
		std::string from;
		std::string to;
		std::string step;
		int from_i;
		int to_i;
		int step_i;
		
		int token = 0;
		
		if(subfield.size() == 0)
			return(false);
		
		if(subfield == "*") {
			rangeSet(out, true);
			return(true);
		}
		else if(subfield.size() > 2) {
			if(subfield.compare(0, 2, "*/") == 0) {
				from_i = 0;
				to_i = out.size() - 1;
				step_i = atoi(subfield.c_str() + 2);
				token = 2;
				goto parsed;
			}
		}
		
		for(int i = 0, l = subfield.size() ; i < l ; i++) {
			int c = subfield[i];
			
			if(token == 0) {
				if(c == '-')
					token = 1;
				else
					from += c;
			}
			else if(token == 1) {
				if(c == '/')
					token = 2;
				else
					to += c;
			}
			else if(token == 2) {
				step += c;
			}
		}
		
		from_i = atoi(from.c_str()) - min;
		to_i = atoi(to.c_str()) - min;
		step_i = atoi(step.c_str());
		
		parsed:
		
		if(from_i < 0 || (unsigned) from_i >= out.size())
			return(false);
		
		if(token >= 1) {
			if(to_i < 0 || (unsigned) to_i >= out.size())
				return(false);
		}
		else {
			to_i = from_i;
		}
		
		if(token == 2) {
			if(step_i <= 0)
				return(false);
		}
		else {
			step_i = 1;
		}
		
		for(int i = from_i ; i <= to_i ; i += step_i) {
			out[i] = true;
		}
		
		return(true);
	}
	
	static void rangeSet(std::vector<bool> &range, bool val) {
		for(int i = 0, l = range.size() ; i < l ; i++) {
			range[i] = val;
		}
	}
}
