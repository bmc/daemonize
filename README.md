daemonize: A tool to run a command as a daemon
==============================================

## Introduction

*daemonize* is a command-line utility that runs a command as a Unix daemon.
See the accompanying man page for full details.

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

[daemonize-home]: http://software.clapper.org/daemonize
[github-repo]: http://github.com/bmc/daemonize

## LICENSE

With the exception of the "install-sh" script and the "getopt.c" source,
this software is released under the BSD-style license. See the LICENSE
file for details.

## COPYRIGHT

With the exception of the "install-sh" script and the "getopt.c" source,
this software is copyright 2003-2010, Brian M. Clapper

---------------------------------------------------------------------------

