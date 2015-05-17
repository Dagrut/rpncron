NAME
====

rpncrontab - maintain rpncrontab files for individual users

SYNOPSIS
========

rpncrontab
  \[
    \[ **e** | **ed** | **edit** _edfile_ \] |
    \[ **r** | **rm** | **remove** _rmfile_ \] |
    \[ **p** | **print** | **show** _pfile_ \] |
    \[ **l** | **list** \] |
    \[ **help** | **-h** | **-?** | **-help** **--help** \]
  \]
  \[ **as** | **user** _username_ \]
  \[ **v** | **verb** | **verbosity** _verb_ \]

DESCRIPTION
===========

 **rpncrontab** is the program that you should use to manage user-specific rpncron files ran by **rpncron(8)**.

OPTIONS
=======

The options **edit**, **remove** **print**, **list** and **help** are mutually exclusive. For duplicate or exclusive options, only the last one will be taken 
into account. If no arguments are given, the help will be displayed.

> **e** **ed** **edit**
> > Edit the file _edfile_ in the rpncron user directory of _username_. If the file is inside a directory, it will be created. _edfile_ needs to be a valid 
> > file name.

> **r** **rm** **remove**
> > Remove the file _rmfile_ from the rpncron user directory of _username_. If an empty directory is left after his removal, this/these directory/ies will be
> > removed too.

> **p** **print** **show**
> > Display the file _pfile_ from the rpncron user directory of _username_ on stdout. If it does not exists, an error will be displayed.

> **l** **list**
> > List all files inside the rpncron user directory of _username_ on stdout.

> **help** **-h** **-?** **-help** **--help**
> > Show the program documentation.

> **as** **user**
> > Ask to handle files for _username_. If not specified, the current user name will be taken.

> **v** **verb** **verbosity**
> > Set the program verbosity to _verb_. The value may be anything from 0 (tight as a clam) to 3 (chatterbox).

NOTES
=====

The files managed by **rpncrontab** should be located in _/var/spool/rpncrontab/username`_ where username is the name given by the _user_ option. To edit 
files, the _EDITOR_ environment variable will be used to find yours.

FILES
=====

/var/spool/rpncrontab
> This directory should only contain other directories named as valid system users, themselves containing files that rpncron can read and execute.

EXAMPLES
========

Here are some examples that could be used :

	rpncrontab as foo edit some/task/file
	rpncrontab show some/task/file
	rpncrontab verb 3 remove some/task/file
	rpncrontab list user bar
	rpncrontab help

SEE ALSO
========

rpncron(8)
