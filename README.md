Rpncron
=======

Hey, what is Rpncron?
---------------------

Rpncron is a background process that runs particular programs at particular
times (This sentence was taken from the cron debian package description :D ).

So it's like cron?
------------------

It is meant to be used like cron (Vixie's cron), yes, but unlike cron :

* Rpncron uses Reverse Polnish Notation (RPN) expressions to know when
  programs should be executed.
* Rpncron allows more fine-tuning when running programs (Like having a maximum
  running program count, the ability to change the process group and shell, 
  the ability to run program at specified seconds, etc.)
* Rpncron allows you to have a really accurate fine-tuning of when the
  programs should run. For example, because each month don't have an even 
  number of days, you cannot run a program exactly each two days with cron.
* The source code is first written to be as readable as possible, and THEN 
  to be fast, so that updates and improvements can be added easily. It was 
  also made in the aim that any code should be easily reusable, and not only 
  in the project itself.

What are the goals of this project?
-----------------------------------

Let's make an other list! Here are the main goals :

* Being able to run a task at a regular inverval, no matter what the 
  interval value is (Like each 37 seconds, or each 19 days).
* Have an up-to-date, easily readable and reusable source code.
* Being able to set several parameters before running the program.
* Being able to run multiple commands when the timer is reached.
* Avoid taking CPU when it's not needed (Like the way it's done in mcron).
* Have (almost) no dependancies.

Here are the other goals, that I will reach if I can and if it's not too 
boring and if I don't change my mind meanwhile :

* Being extensible (For example, by loading shared libraries to extend the RPN 
  functionnalities.
* Being as fast as possible, without messing up with item 2.

What are the pros and cons of rpncron and its alternatives?
-----------------------------------------------------------

An other list! Here are they, to my mind, for the mere mortal every day user :
* *cron (Vixie cron)* :
  
  Pros: Really simple syntax, easy to understand and to run.
  
  Cons: No active development, no out of the ordinary feature, may contain 
  bugs (according to a forum).
  
* *rpncron* :
  
  Pros: Flexible for the time computation part and the program execution 
  part, configurable, and under development.
  
  Cons: RPN syntax may be a bit scary for new-comers, and the configuration 
  files are quite verbose (for the moment).
  
* *GNU cron (mcron)* :
  
  Pros: It seems to be the most powerful alternative to cron for the time 
  computation part. Small and easily reviewable source code.
  
  Cons: Requires the user to know scheme and to install a scheme interpreter.
  
* *dillon's cron (dcron)* :
  
  Pros: Complete rework of cron that adds some nice features.
  
  Cons: Not well known, and it seems to have the same limitations as cron 
  for the time computation part. Also, no active development.
  
* *cronie* :
  
  Pros: Complete rework of cron that adds some features and also improves 
  the security a lot.
  
  Cons: Not in the debian repositories (`:-°`), and same thing for the time 
  computation part.
  
* *gcron* :
  
  Pros: ?
  
  Cons: Program not finished.

There is also *Launchd*, which, according to wikipedia, can replace cron. 
However, this program do much more than the others above, and may not work 
everywhere.

How do I install it?
--------------------

No more list here, sorry! `:-D`. To install rpncron, just run these commands :

	$ cmake
	$ make
	# make install

Note that the make install step won't work now, because it is not written at 
the moment.

Can you show me some configuration samples? Is there a documentation for it?
----------------------------------------------------------------------------

Sure, sir! See the `examples` directory. You can also try to run files in 
this directory. The only thing they will do is creating/updating files in 
`/tmp/` and printing things on stdout/stderr. Also, if anything is printed 
or if the return code is not OK, they may be written into your syslog log 
file and/or sent by mail to yourself.

You can read details about the configuration files syntax in the 
[CONFIGURATION.md](./CONFIGURATION.md) file.

How is the development going?
-----------------------------

Here is a list (YEAH!) of what is missing and what is done :

- [X] Run the programs when needed.
- [X] Add the option to run the program as a whole script instead of a set of 
      commands.
- [X] Change the setuid/setgid/cwd of the ran programs if needed.
- [ ] Write the `make install` code.
- [ ] Search for security issues.
- [ ] Prepare some unit tests.
- [ ] Test extensively the program.
- [ ] Check for portability issues (mainly in src/os).
- [X] Change the spawned process environment.
- [ ] Handle errors in an other way than printf().
- [X] Log informations.
- [X] Send mails on error.
- [ ] Use the given verbosity level.
- [ ] Give a way to write the PID somewhere.
- [X] Simultaneous processes limit.
- [X] Write configuration files documentation.
- [ ] Check for file updates in an other thread and combine the process of 
      waiting for updates and sleep() in the main run loop, using select().
- [ ] And many more features, that I will pick from other projects... `;-)`

A last word for the ASCII camera?
---------------------------------

Yes. When I talk about portability, I don't include windows.

That's all, folks!
