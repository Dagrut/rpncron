cmake_minimum_required (VERSION 2.8)
project(rpncron)

set(CMAKE_CXX_FLAGS "-ggdb -Wall")

add_executable(rpncron
	src/rpncron.cpp
	src/rpn/rpn_simple_token.cpp
	src/rpn/functions/rpn_functions_arith_elem.cpp
	src/rpn/functions/rpn_functions_comp.cpp
	src/rpn/functions/rpn_functions_logic.cpp
	src/rpn/functions/rpn_functions_types.cpp
	src/rpn/functions/rpn_functions_queue.cpp
	src/rpn/functions/rpn_functions_branching.cpp
	src/rpn/functions/rpn_functions_misc.cpp
	src/rpn/functions/rpn_functions_time.cpp
	src/conf/conf_parser.cpp
	src/conf/conf_callbacks.cpp
	src/args/args_parser.cpp
	src/args/args_rc.cpp
	src/args/args_doc_rc.cpp
	src/os/users.cpp
	src/os/file.cpp
	src/os/directory.cpp
	src/os/processes.cpp
	src/os/environment.cpp
	src/os/logs.cpp
	src/os/mail.cpp
	src/os/dir_crawler.cpp
	src/os/time.cpp
	src/run/run_rpncron.cpp
	src/run/run.cpp
	src/run/run_debug.cpp
	src/utils/vixie_cron.cpp
)

target_link_libraries(rpncron m)

add_executable(rpncrontab
	src/rpncrontab.cpp
	src/conf/conf_parser.cpp
	src/conf/conf_callbacks.cpp
	src/args/args_parser.cpp
	src/args/args_rct.cpp
	src/args/args_doc_rct.cpp
	src/os/users.cpp
	src/os/file.cpp
	src/os/directory.cpp
	src/os/processes.cpp
	src/os/environment.cpp
	src/os/dir_crawler.cpp
	src/run/run_list.cpp
	src/run/run_remove.cpp
	src/run/run_edit.cpp
	src/run/run_print.cpp
)

target_link_libraries(rpncrontab m)

install(TARGETS
	rpncron
	rpncrontab
	RUNTIME DESTINATION bin
)

install(FILES
	README.md
	doc/rpncron.md
	doc/rpncrontab.md
	doc/rpncron-config.md
	
	DESTINATION share/doc/rpncron
)

install(FILES extra/init.d/rpncron DESTINATION /etc/init.d)

install(FILES extra/default/rpncron DESTINATION /etc/default)
