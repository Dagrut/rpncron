NAME
====

rpncron - daemon to execute scheduled commands

SYNOPSIS
========

rpncron
  \[
    \[ **r** | **run** \] |
    \[ **rf** | **runfile** | **run-file** _rffile_ \] |
    \[ **rd** | **rundir** | **run-dir** _rddir_ \] |
    \[ **df** | **df2** | **debugfile** | **debug-file** _dffile_ _timestamp_ \] |
    \[ **help** | **-h** | **-?** | **-help** **--help** \]
  \]
  \[ **d** | **daemon** | **daemonize** \]
  \[ **pf** | **pid** | **pidfile** | **pid-file** _pidfile_ \]
  \[ **as** | **user** _username_ \]
  \[ **v** | **verb** | **verbosity** _verb_ \]

DESCRIPTION
===========

The **rpncron** command is mainly aimed to run tasks at some specific moments. It can be started automatically when the system starts, or at will and by 
anyone, to run tasks in other directories than the default ones.

OPTIONS
=======

The options **run**, **runfile** **rundir**, **debugfile** and **help** are mutually exclusive. For duplicate or exclusive options, only the last one will be 
taken into account. If no arguments are given, the help will be displayed.

> **r** **run**
> > Run the rpncron process, using the default system directory and the user directories.

> **rf** **runfile** **run-file**
> > Run the file _rffile_

> **rd** **rundir** **run-dir**
> > Run the files inside the directory _rddir_

> **df** **df2** **debugfile** **debug-file**
> > Simulates the run of the file _dffile_ at the time given by _timestamp_. The timestamp is the number of seconds elapsed since epoch (01/01/1970 at 
> > 00:00:00).

> **help** **-h** **-?** **-help** **--help**
> > Show the program documentation.

> **d** **daemon** **daemonize**
> > For the **run**, **runfile** and **rundir** options only, ask to daemonize.

> **pf** **pid** **pidfile** **pid-file**
> > Stores the process current PID inside _pidfile_

> **as** **user**
> > Ask to run the files as user _username_ if the user is not specified in the runned files.

> **v** **verb** **verbosity**
> > Set the program verbosity to _verb_. The value may be anything from 0 (tight as a clam) to 3 (chatterbox).

NOTES
=====

When running **rpncron**, it will search any files inside _/etc/rpncron.d_ or its sub-directories. It will also look for files in directories 
_/var/spool/rpncron/_**username** where username is the name of an existing user in this system. If the user is not found, nothing will be executed. For each 
user, the commands will be executed as them. For the first directory, programs are started as root, unless **rpncron** is not started as root itself, ot the 
configuration asks to change the user.

SEE ALSO
========

rpncrontab(8)
