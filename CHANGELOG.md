daemonize Change Log
====================

Changes in version 1.7.3:

- Merged patch from Ewan Mellor (ewan.mellor _at_ citrix.com) that adds an
  appropriate definition for `HAVE_INITGROUPS` to `config.h.in`.
-----------------------------------------------------------------------------
Changes in version 1.7.2:

- Fixed bad reference to `LOG_ERROR` in `daemon.c`, causing compilation 
  problems (Should have been `LOG_ERR`.)
-----------------------------------------------------------------------------
Changes in version 1.7.1:

- Added emulator for *flock*(2) file locking call, emulated in terms of
  *lockf*(3), for systems (like Solaris) that don't have *flock*. Addresses
  [Issue #3][]. Tested on Solaris 11 (x86).
  
[Issue #3]: https://github.com/bmc/daemonize/issues/3

-----------------------------------------------------------------------------
Changes in version 1.7.0:

- Now supports a `-E` parameter, which allows setting one or more environment
  variables, to be passed to the daemon. Based on a suggestion from
  Michel Gouget *michel /at/ gouget.org*.

-----------------------------------------------------------------------------
Changes in version 1.6.2:

- Local copy of daemon(3) now errors out (with a message to syslog(3)) if
  it cannot open /dev/null. Thanks to Folkert van Heusden
  *folkert.mobiel /at/ gmail.com* for noticing the problem.

-----------------------------------------------------------------------------
Changes in version 1.6.1:

- Makefile now generates daemonize.html, an HTML-ized version of the man page.
- Fixed usage message, which incorrectly said "stderr", instead of "stdout"
  in the description of the "-o" option. Thanks to [Sam Pierson][] for the
  patch.

[Sam Pierson]: http://sampierson.com/blog

-----------------------------------------------------------------------------
Changes in version 1.6:

Rolled in changes submitted by Ken D. Farnen, (kenf /at/ 14Cubed.COM):

- On slackware, all of the rc.???? scripts run directly from init, and init
  doesn't set up much in the way of an environment, whereas login stuffs
  loads of stuff into the environment. Daemonize now ensures that a minimum
  environment is passed to the exec-ed daemon (IFS and PATH).

- The -u flag now puts USER, HOME and LOGNAME into the environment, for the
  benefit of programs that rely on them. The lack of these environment
  variables seems to have deleterious effects on MythTV, for example.

- On Slackware Linux, access to video and audio device entries is
  controlled by secondary group membership. (e.g., A user who needs access
  to a dvb device needs to be in group "video", and a user who needs access
  to dsp devices needs to be in group "audio".), This is typically done by
  the login code. Daemonize now initializes those group memberships, as
  well, if the initgroups() function is present.

-----------------------------------------------------------------------------
Changes in version 1.5.6:

- Accepted and applied a patch from Gary T. Giesen (giesen /at/ snickers.org)
  that allows the daemonize Makefile to play well with Linux RPMs.

-----------------------------------------------------------------------------
Changes in version 1.5.5:

- Changes in 1.5.4 made it impossible to create PID files in directories
  not owned by the user specified with -u. Prior to 1.5.4, doing so was
  possible. It is now possible again, and the functionality has been
  corrected slightly to ensure that the proper PID is written and that the
  PID file has mode 0644.

-----------------------------------------------------------------------------
Changes in version 1.5.4:

- If a user name is specified via the -u option, the output files specified
  by the -o and -e options are now opened *after* daemonize switches users,
  not before (as happened in previous versions). This change ensures that
  the output files end up being owned by that user, not by root. This
  change has no effect if -u is not used.

- Rearranged and simplified some of the code in daemonize.c

- Added some documentation to the code.

-----------------------------------------------------------------------------
Changes in version 1.5.3:

- Minor licensing change. (configure.in still emitted a message indicated
  that daemonize is GPL-licensed. It's BSD-licensed.)
- Updated copyright information.

-----------------------------------------------------------------------------
Changes in version 1.5.2:

Two changes suggested and supplied by Yakov Lerner <iler.ml <at> gmail.com>:

- Added a "-l lockfile" option, permitting daemonize to use a lockfile to
  ensure that only one instance of the daemon runs at a time.

- Now makes additional checks to ensure that the program specified to
  daemonize is a regular file, not a directory.

-----------------------------------------------------------------------------
Changes in version 1.5.1:

- Now released under BSD-style license, not GNU Public License.

-----------------------------------------------------------------------------
Changes in version 1.5:

- daemonize can now optionally redirect the daemon's standard output and
  standard error to files

- configure now supports a local basename() emulator.

-----------------------------------------------------------------------------
Changes in version 1.4:

- Version of daemon(3) shipped with daemonize attempted to redirect
  stdin, stdout and stderr to /dev/null, by closing all file descriptors
  (using sysconf(_SC_OPEN_MAX) to determine the maximum file descriptor).
  This method wasn't working, and apparently the forked daemon process
  was reacquiring (or still had) valid stdin, stdout and stderr file
  descriptors. I made some changes to daemon.c to be more consistent with
  how the native FreeBSD and Linux versions behave.

  This bug may have contributed to a problem on AIX. According to
  Ron Avriel <ron /at/ cti2 /dot/ com>, when he telnets to an AIX 5.2
  system and uses daemonize create a daemon, he can't log out of the telnet
  session (i.e., the logout operation hangs) until he kills the daemon.
  That observation is consistent with the theory that the daemon process is
  still holding open the controlling terminal.

- daemonize now installs with 0755 permissions, instead of 0700.

-----------------------------------------------------------------------------
Changes in version 1.3:

- The "-c" option now actually works. Duh.

-----------------------------------------------------------------------------
Changes in version 1.2:

- The external names in getopt.c are now prefixed with "x_" (e.g.,
  x_getopt(), x_opterr, etc.). The function's name has been changed to
  avoid type conflicts with the native getopt() on the host operating
  system. (This is known to happen on Solaris 8, for instance.) Thanks
  to Michael Fuchs <mf /at/ mfox /dot/ de> for pointing out the problem
  on Solaris.

-----------------------------------------------------------------------------
Changes in version 1.1:

- Now uses local getopt.c unconditionally, instead of defaulting to
  whatever's on the system. This gets around a problem when installing on
  systems with GNU getopt (e.g., Linux). Unless POSIXLY_CORRECT is set in
  the environment, GNU getopt scans the entire command line, which means
  it'll complain about "unknown options" that are specified in the command
  being passed to daemonize. One solution is to have daemonize set
  POSIXLY_CORRECT right before calling getopt() (and then restore it before
  running the program-to-daemonize, in case it might affect that program's
  behavior). However, it's just simpler to use the old, public domain
  getopt routine.

- daemonize now verifies that the program actually exists before
  daemonizing. (It didn't check before.)
