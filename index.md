---
title: daemonize — A tool to run a command as a daemon
layout: withTOC
---

## Introduction

*daemonize* is a command-line utility that runs a command as a Unix daemon.

*daemonize* runs a command as a Unix daemon. As defined in W. Richard
Stevens’ 1990 book, [*UNIX Network Programming*][] (Addison-Wesley, 1990),
a daemon is “a process that executes ‘in the background’ (i.e., without an
associated terminal or login shell) either waiting for some event to occur,
or waiting to perform some specified task on a periodic basis.” Upon
startup, a typical daemon program will:

* Close all open file descriptors (especially standard input, standard
  output and standard error)
* Change its working directory to the root filesystem, to ensure that it
  doesn’t tie up another filesystem and prevent it from being unmounted
* Reset its *umask* value
* Run in the background (i.e., fork)
* Disassociate from its process group (usually a shell), to insulate itself
  from signals (such as HUP) sent to the process group
* Ignore all terminal I/O signals
* Disassociate from the control terminal (and take steps not to reacquire one)
* Handle any `SIGCLD` signals

Most programs that are designed to be run as daemons do that work for
themselves. However, you’ll occasionally run across one that does not. When
you must run a daemon program that does not properly make itself into a
true Unix daemon, you can use *daemonize* to force it to run as a true
daemon.

See the [*man* page][] for full details.

[*man* page]: daemonize.html
[*UNIX Network Programming*]: http://www.kohala.com/start/unp.html

## Notes

* If the host operating system provides the *daemon*(3) library routine,
  *daemonize* will use it. Otherwise, *daemonize* uses its own version of
  *daemon*(3). This choice is made at compile time. (BSD 4.4-derived
  operating systems tend to provide their own *daemon*(3) routine.)

* [FreeBSD][] 5.0 introduced a *daemon*(1) command that is similar to, but
  less functional, than *daemonize*.

[FreeBSD]: http://www.freebsd.org/

## Installation

Normal installation:

    $ sh configure
    $ make
    $ su 
    # make install

For a detailed report of the available `configure` options:

    $ sh configure --help

## Notes

I have personally compiled and tested daemonize on the following platforms:

- FreeBSD 4.x and 8.0-RELEASE
- Mandrake Linux 8.1
- Red Hat Linux 6.2
- Red Hat Linux 8.0
- Red Hat Enterprise Linux 4 / CentOS 4
- Solaris (SunOS 5.8)
- Fedora Core 5
- Ubuntu 8 and 9
- Mac OS X 10.4 (Tiger)

The accompanying "configure" script was generated with GNU autoconf
version 2.1.3. It should work, as is, for most Unix systems.

## Author

Brian Clapper, *bmc@clapper.org*

## Web Page

* [Home Page][daemonize-home]
* [GitHub repo][github-repo]

[daemonize-home]: http://www.clapper.org/software/daemonize
[github-repo]: http://github.com/bmc/daemonize

## LICENSE

With the exception of the `install-sh` script and the `getopt.c` source,
this software is released under BSD license. See the [license][] for details.

[license]: license.html

## COPYRIGHT

With the exception of the "install-sh" script and the "getopt.c" source,
this software is copyright 2003-2010, Brian M. Clapper

---------------------------------------------------------------------------


