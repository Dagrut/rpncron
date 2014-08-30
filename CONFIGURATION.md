General Informations
====================

The configuration files can currently be used using one single syntax. This 
syntax uses blocks to define a configuration, an execution time, and the 
commands to be executed (respectively `@CFG`, `@RPN` and `@CMD`). You can 
also define a default configuration with `@DFT` so that each next 
configurations will use the values set in this block. They can of course be 
overridden. Its syntax is exactly the same as the `@CFG` part.

Each block start with its keyword at the beginning of a line, followed by 
one space and an arbitrary end delimiter. This end delimiter is used to know 
when the block stops, and have to be written on a line alone to do so. It 
can contain any character except the end of line (`\n`) character. An 
example block could be :

	@CFG <THIS IS |THE end}{"
	Some configuration here...
	<THIS IS |THE end}{"

The `@CFG` part is read line by line, independantly. The `@RPN` part is read 
as a whole text block, which can be splitted into several lines. The `@CMD` 
part may be read as a set of lines or as a block, depending on the 
configuration.

Parser details
--------------

The parser used to split words in the `@RPN` part and in each line of the 
`@CFG` part is the same : The words are separated by blank characters 
(Space, Vertical and Horizontal tabulations, Line breaks and Carriage 
return, or in the C way : ` \t\v\n\r`). These space characters can be 
escaped using the single and double quotes (`"` and `'`). Inside quoted 
text, any character can be escaped by a backslash (`\`) to insert it. 
Escaping an unknown character (like the letter 'a') results in the given 
letter. Also note that an ending quote followed by a text or an other quote 
will not split the text, it will work exactly as in bash and sh.

Here are some examples of how a text will be interpreted :

	Some text                         | Some                  | text
	Some Sp3c1@l \C#h&a{r[s           | Some                  | Sp3c1@l     | \C#h&a{r[s
	A 'quoted text'                   | A                     | quoted text
	"n o "'s p a c e'" h e r e"       | n o s p a c e h e r e
	"Es\c\a\\ping\" \"ex@m\p\>le"     | Esca\ping" "ex@mp>le
	same" big"' word'                 | same big word

Configuration Keywords
======================

@CFG part
---------

The `@CFG` and `@DFT` keywords are :

- **mode** The execution mode of the `@RPN` part. It can be 'offset' or 
  'bool' (default).
  
  Offset tell us that the `@RPN` expression will return an offset in 
  seconds, during which we should wait before executing the commands. It 
  implies that no extra computation will be done before this time for this 
  configuration item, and also that the execution time can be accurate to 
  within a second. If the value is lower or equal to 1, it will be set to 1.
  
  Bool tell us that the `@RPN` expression will return a boolean value, where 
  true means execute it now, and false means don't execute it. The 
  expression will be run each minute, to check if the expression is true or not.
- **shell** The shell program to execute the commands. It can be any complete
  command execution, including arguments. For example
  `shell /bin/bash --posix --verbose` is valid. For more informations about
  how this line is parsed, see the 'Parser details' part.
- **user** The user to use to run the command. It defaults to the user running
  the rpncron command.
- **group** The group to use to run the command. It defaults to the group running
  the rpncron command.
- **cwd** The current working directory to use to run the command. It defaults 
  to `/`.
- **max-proc** The maximum running process count. It it reaches this values, 
  the commands will not be launched anymore. When set to 0, no more limits 
  are used. It defaults to 128.
- **exec** The way the commands will be executed. It can be `system` 
  or `pipe` (default).
  
  System calls the C system() function for each `@CMD` line. Beware that 
  according to its man page, using system() after changing the program user 
  and group can cause somr problems. This option may be removed or rewritten 
  later, to stop using system().
  
  Pipe execute the shell given by the `shell` option, and send it the 
  contents of the `@CMD` part. You can use anything here, that supports 
  script piping (Tested only with bash).
- **env-set** Set the given environment variable before starting the process. 
  The first value is the variable name. The second is the variable value. 
  The third is a flag, which requires to override the value if it exists.
- **env-unset** Unset the given environment variable before starting the 
  process. You should note that the order of set and unset are kept. It may 
  not be useful, so this feature may be removed later.
- **on-error** Tells rpncron what actions it should take when the command 
  returns a value which is not zero. Possible values are `log`, `mail` and 
  `ignore`, to log informations, send a mail, or do nothing. Values are 
  separated with spaces. The default is `log mail`.
- **on-output** Tells rpncron what actions it should take when the command 
  prints something on stdout. Possible values are `log`, `mail` and 
  `ignore`, to log informations, send a mail, or do nothing. Values are 
  separated with spaces. The default is `log mail`.
- **on-exec-error** Tells rpncron what actions it should take when a command 
  execution fails because of a system error (no FD available, read/write 
  error, fork error, execve error, ...). Possible values are `log`, `mail` 
  and `ignore`, to log informations, send a mail, or do nothing. Values are 
  separated with spaces. The default is `log mail`.

For the last three options, two other values are planed : `stop` and `remove`
, to stop executing lines (with the `system` execution mode), or to remove 
the task from the memory, so that it will not be processed anymore upon file 
update or program reload/restart.

@RPN part
---------

This part contains several keywords. Some of them are variables, and some 
others are functions. Note that the variable values are resolved when they 
are inserted in the queue, and not when they are read by a function. It 
means that they can be considered as constants rather than variables.

Here is a list of standard variables :

- **time_sec** The number of seconds since epoch (01/01/1970 00:00:00) as
  returned by time().
- **time_min** The number of minutes since epoch (01/01/1970 00:00:00) as
  returned by time() / 60.
- **time_hour** The number of hours since epoch (01/01/1970 00:00:00) as
  returned by time() / 3600.
- **time_day** The number of days since epoch (01/01/1970 00:00:00) as
  returned by time() / 86400.
- **time_SoM** (Seconds of Minutes) The value of seconds in the current minute.
- **time_MoH** (Minutes of Hour) The value of minutes in the current hour.
- **time_Hod** (Hour of day) The value of hours in the current day.
- **time_dom** (day of month) The current day of the month.
- **time_dow** (day of week) The current day of the week.
- **time_doy** (day of year) The current day of the year.
- **time_moy** (month of year) The current month of the year.
- **time_year** The current year.
- **time\_is\_dst** It equals 1 if we are in Daylight Saving Time (=> DST).
- **proc_count** The number of processes that are still running for this 
  configuration. This values is always zero when starting the program.
- **exec_count** The number of times the commands of this configuration have 
  been executed. This values is always zero when starting the program.

Note that the ranges of each variable is the same as the one given by 
localtime(). Here are the ranges that should be used : 0 to 59 for seconds 
and minutes (Seconds may go to 60 for a leap second), 0 to 23 for the hours, 
1 to 31 for the day of month, 0 to 6 for the week (0 is sunday), 0 to 11 for 
the month, 0 to 365 for the day of year. The only modified value is the 
year, which is the correct year value, and not the years since 1900.

This part also contains functions. Here is a list of available functions :

- **+ - * / %** Generic operations, including modulo. They require two
  arguments.
- **== === != < > <= >=** Generic comparison functions. They require two
  arguments. Note that === also checks if both variables have the same type 
  (With ==, 3 is equal to 3.0).
- **|| && or() and() not() !** Generic logic operations. They require two
  arguments. `not()` and `!` are the same.
- **||(*) &&(*) or(*) and(*)** Generic logic operations applied to the whole
  queue. For example `1 2 3 ||(*)` is a short way for `1 2 3 || ||`.
- **int() float() string() bool() !! void()** Generic cast operations. They
  require one arguments. `bool()` and `!!` are the same. `void()` does simply
  nothing and does not read/write anything from/to the queue.
- **swap() pop(head) pop(tail) rotate(push) rotate(pop) reverse() clear()**
  Queue operations :
  - **swap()** Swaps the two first items of the queue.
  - **pop(head)** Remove the first element of the queue.
  - **pop(tail)** Remove the last element of the queue.
  - **rotate(push)** Push the last element of the queue to the front.
  - **rotate(pop)** Pop the first queue element of the queue and push it to
    the tail.
  - **reverse()** Reverse the entire queue.
  - **clear()** Empty the queue.
- **if() ifelse() switch()** Conditions :
  - **if()** If the first element of the queue is false, the second element is
    removed. It requires two elements.
  - **ifelse()** If the first element of the queue is true, the third element
    is removed. Otherwise, the second element is removed. It requires three
    elements.
  - **switch()** Use the first queue value as an integer, which will be used to
    know which item in the queue will be taken (the other items will be 
    removed). So 0 means the second item, 1 the third, etc. If the value is 
    greater or lower than the number of remaining elements, the value modulo 
    the number of remaining elements is taken.
- **eval()** Evaluates the given RPN expression. Beware that it will use the
  same queue, so operations like `clear()` will clear the current queue.
- **= set()** Variables operations. They both set a variable with the name 
  taken from the first queue element and the value from the second queue 
  element. Note that any value can be used as a variable name, so that writing
  '5 3 =' will turn each '3' single item into '5'.
  
  And because variables cannot be unset nor overriden, this expression :
  'a b = c b =' will do 'b=a', and then 'a=c' and not 'b=c' as you would 
  expect, since the second 'b' will be translated to 'a' first. Read the 
  'Parser details' part for more informations.
- **min2sec() hour2sec() day2sec() week2sec() year2sec()**
  **sec2min() sec2hour() sec2day() sec2week() sec2year()** Time unit 
  conversion. These are short ways for the product/division with the 
  appropriate time amount in a readable way. For example, min2sec makes a 
  product by 60 with the first value of the queue. The values are converted 
  to integers before computation.
