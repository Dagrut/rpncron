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

#ifndef _HPP_RPNCRON_RPNCRON_
#define _HPP_RPNCRON_RPNCRON_

#define RPNCRON_PROGRAM_VERSION_0 0
#define RPNCRON_PROGRAM_VERSION_1 0
#define RPNCRON_PROGRAM_VERSION_2 1

#define RPNCRON_PROGRAM_STRVAL(x) #x
#define RPNCRON_PROGRAM_MKVERSION(a, b, c) \
	RPNCRON_PROGRAM_STRVAL(a) "." \
	RPNCRON_PROGRAM_STRVAL(b) "." \
	RPNCRON_PROGRAM_STRVAL(c)

#define RPNCRON_PROGRAM_VERSION \
RPNCRON_PROGRAM_MKVERSION( \
	RPNCRON_PROGRAM_VERSION_0, \
	RPNCRON_PROGRAM_VERSION_1, \
	RPNCRON_PROGRAM_VERSION_2 \
)

#define RPNCRON_PROGRAM_LOG_FILE          "/var/log/rpncron.log"
#define RPNCRON_PROGRAM_PID_FILE          "/var/run/rpncron.pid"
#define RPNCRON_PROGRAM_SYSTEM_PATH       "/etc/rpncron.d"
#define RPNCRON_PROGRAM_USERS_PATH        "/var/spool/rpncron"
#define RPNCRON_PROGRAM_EDITOR            "/usr/bin/vi"
#define RPNCRON_PROGRAM_FATAL_MSG \
	printf("ERROR: THAT SHOULD NEVER HAPPEN! (%s:%d)\n", __FILE__, __LINE__); \
	exit(1);

#define DEBUG(...) \
	fprintf(stderr, __VA_ARGS__)

#define FATAL(...) \
	fprintf(stderr, __VA_ARGS__); \
	exit(1)

#endif
