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

#ifndef _HPP_RPNCRON_RPN_RPN_
#define _HPP_RPNCRON_RPN_RPN_

#include <deque>
#include <vector>
#include <map>
#include <string>

#include <cstring>

namespace RC {
	template <typename Token>
	class Rpn {
		public:
			typedef void (*RpnFunction)(Rpn *, const std::string &key);
			typedef std::map<std::string, Token*> Variables;
			typedef std::map<std::string, RpnFunction> Functions;
			typedef std::deque<Token*> Queue;
			typedef typename Variables::iterator VariablesIterator;
			typedef typename Functions::iterator FunctionsIterator;
			typedef typename Queue::iterator QueueIterator;
			
			Variables variables;
			Functions functions;
			Queue queue;
			
			Rpn() {}
			
			~Rpn() {
				typename Queue::iterator it;
				typename Queue::iterator end;
				
				it = this->queue.begin();
				end = this->queue.end();
				for(; it != end ; it++) {
					delete *it;
				}
			}
			
			bool addFunction(const std::string &key, RpnFunction func) {
				std::pair<Rpn::FunctionsIterator,bool> a;
				a = this->functions.insert(std::pair<std::string, RpnFunction>(
					key,
					func
				));
				return(a.second);
			}
			
			bool addVariable(const std::string &key, Token *token) {
				std::pair<Rpn::VariablesIterator,bool> a;
				a = this->variables.insert(std::pair<std::string, Token*>(
					key,
					token
				));
				return(a.second);
			}
			
			void clearVariables() {
				VariablesIterator it;
				VariablesIterator end;
				
				it = this->variables.begin();
				end = this->variables.end();
				for(; it != end ; it++) {
					delete it->second;
				}
				this->variables.clear();
			}
			
			void clearFunctions() {
				this->functions.clear();
			}
			
			void clearQueue() {
				QueueIterator it;
				QueueIterator end;
				
				it = this->queue.begin();
				end = this->queue.end();
				for(; it != end ; it++) {
					delete *it;
				}
				this->queue.clear();
			}
			
			static int parse(
				const std::string &query,
				std::vector<std::string> &parsed,
				const std::string &seps = " \t\v\n\r",
				const char escape = '\\',
				const std::string &lquotes = "\"'",
				const std::string &rquotes = "\"'"
			) {
				std::string buffer;
				bool is_quoted;
				bool is_esc;
				const char *ptr;
				char expected_rquote;
				
				buffer.reserve(256);
				
				if(lquotes.size() != rquotes.size())
					return(-1);
				
				is_quoted = false;
				is_esc = false;
				ptr = query.c_str();
				for(int i = 0, l = query.size() ; i < l && ptr[i] ; i++) {
					if(is_esc) {
						buffer += ptr[i];
						is_esc = false;
					}
					else if(strchr(seps.c_str(), ptr[i]) && !is_quoted) {
						if(buffer.size() > 0) {
							parsed.push_back(buffer);
							buffer.clear();
						}
					}
					else if(strchr(lquotes.c_str(), ptr[i]) && !is_quoted) {
						expected_rquote = *(rquotes.c_str() + (strchr(lquotes.c_str(), ptr[i]) - lquotes.c_str()));
						is_quoted = true;
					}
					else if(expected_rquote == ptr[i] && is_quoted) {
						is_quoted = false;
					}
					else if(ptr[i] == escape && is_quoted) {
						is_esc = true;
					}
					else {
						buffer += ptr[i];
					}
				}
				
				if(buffer.size() > 0) {
					parsed.push_back(buffer);
					buffer.clear();
				}
				
				return(parsed.size());
			}
			
			void process(
				const std::string &query,
				const std::string &seps = " \t\v\n\r",
				const char escape = '\\',
				const std::string &lquotes = "\"'",
				const std::string &rquotes = "\"'"
			) {
				std::vector<std::string> query_vec;
				
				if(Rpn::parse(query, query_vec, seps, escape, lquotes, rquotes) > 0) {
					this->process(query_vec.begin(), query_vec.end());
				}
			}
			
			template <typename I>
			void process(
				I begin,
				I end
			) {
				for(; begin != end ; begin++) {
					this->parseToken(*begin);
				}
			}
			
			void parseToken(const std::string &token) {
				VariablesIterator v_it;
				FunctionsIterator f_it;
				
				f_it = this->functions.find(token);
				
				if(f_it != this->functions.end()) {
					f_it->second(this, token);
					
					return;
				}
				
				v_it = this->variables.find(token);
				if(v_it != this->variables.end()) {
					this->queue.push_front(v_it->second->clone());
					
					return;
				}
				
				this->queue.push_front(new Token(token));
			}
			
			int result_size() const {
				return(this->queue.size());
			}
			
			Token* result() {
				if(this->queue.size() > 0)
					return(this->queue.front());
				return(NULL);
			}
			
			const Queue& result_queue() const {
				return(this->queue);
			}
	};
}

#endif
