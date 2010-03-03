*daemonize* Frequently Asked Questions
======================================

---

## Why can't I just use "&" from the shell?

Using "&" puts the program in the background, but it doesn't do all the other
things that make the program a true daemon. With some shell, a backgrounded
program will ignore `SIGHUP`; with others shell, it will not. Regardless, using
"&" does not:

* disaassociate the program from its process group or control terminal
* ignore *all* terminal I/O signals
* close all file descriptors
* change the process's working directory
* handle `SIGCLD` and related signals

These are all things a true daemon must do.

## Well, why can't I use *nohup*(1) then?

The *nohup* command has similar problems to "&". While it *does* guarantee
that the process will ignore `SIGHUP`, it doesn't do any of the other
things necessary to make the program a true daemon.

## Aren't there existing commands that do what *daemonize* does?

On some platforms, yes. [FreeBSD][freebsd] has a *daemon*(8) command that
essentially does what *daemonize* does, though it supports fewer options.
But not all flavors of Unix have such a command.

[freebsd]: http://www.freebsd.org/

## *daemonize* doesn't work well with log rotation.

When you specify output and error files (via `-o` and `-e`), *daemonize*
opens those output files and passes the open file descriptors to the daemon
program. Many log rotation programs (e.g., [*logrotate*][logrotate]) rotate
log files by compressing the existing file to a new file, unlinking the old
file. The problem is, the running daemon maintains a file pointer to the
old, unlinked inode; it doesn't pick up the newly rolled file, because it
still has the old (deleted) one open.

[logrotate]: http://linuxcommand.org/man_pages/logrotate8.html

Unfortunately, there's no easy way to have *daemonize* close the file
descriptor and reopen it, to prevent this problem. The main problem is that
*daemonize*, itself, is no longer running at this point. When *daemonize*
comes up, it opens the output files, then overlays itself (via *exec*(2))
with the program you really want to run. By the time something like
*logrotate* gets around to rolling the real program's log file, *daemonize*
is totally out of the picture.

People typically handle this kind of problem in a few different ways.

* The [Apache web server][apache], for instance, can be configured to pipe
  its log output into a program, and there are programs out there that'll
  read standard input, write it to a log, and handle log rotation.
  [Cronolog][cronolog] is one such program. I could certainly extend
  *daemonize* to permit this kind of thing, I suppose (e.g., allow `-o` to
  specify a pipe to a command). I can see the benefit, but adding it to
  *daemonize* smacks of creeping featurism. *daemonize* is small and
  simple; I'd hate to make it needlessly complicated.

[apache]: http://httpd.apache.org/
[cronolog]: http://cronolog.org/

* Another approach I've seen on Unix systems is run a script that kills the
  daemon, roll its log, restarts the server. This second option will often
  work fine; unless something goes wrong, the daemon won't be down more
  than a second or two.

* Yet another approach is to use [*syslogd*][syslogd]. If you cannot change
  the called program (i.e., the program being launched by *daemonize*) to
  log directly to the *syslog* API, you can always write a small wrapper
  program or script that pipes the output to the *logger*(1) program. The
  wrapper would do the equivalent of the following:

    `realdaemon | logger -p local7.info -t realdaemon`

[syslogd]: http://linuxcommand.org/man_pages/sysklogd8.html

## Will *daemonize* work on Windows?

In a word, "No."

The closest Windows analogy to a "daemon" is a [Windows service][winservice].
Extending *daemonize* to work on Windows simply isn't feasible or necessary:

* Windows services bear only a superficial resemblance to Unix daemons.
* There are already existing Windows service wrappers out there. Google for
  *windows service wrapper* for a representative sample.

[winservice]: http://en.wikipedia.org/wiki/Windows_service

---
