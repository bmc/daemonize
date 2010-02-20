/*---------------------------------------------------------------------------*\
  Site-specific configuration file for run-as utility

  Input to `autoheader'

  This source code is released under a BSD-style license. See the LICENSE
  file for details.
\*---------------------------------------------------------------------------*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <sys/types.h>

@@TOP@@

/* define if your C compiler lacks a built-in `bool' type */
#undef bool

/* define to `int' if your system lacks `ssize_t' */
#undef ssize_t

/* define to `unsigned short' if your system lacks `ushort' */
#undef ushort

/*
   Define if you have the `sig_t' type in <signal.h> or <sys/signal.h>
   (e.g., FreeBSD)
*/
#undef HAVE_SIG_T

/*
  Define if your compiler supports a native "byte" type that holds at least
  eight bits.
*/
#undef HAVE_BYTE_TYPE

/* Define if you have the getpgrp() function. */
#undef HAVE_GETPGRP

/* Define if you have the setpgrp() function. */
#undef HAVE_SETPGRP

/* Define if you have the initgroups() function. */
#undef HAVE_INITGROUPS

/* Define if you have the setsid() function. Almost everyone does. */
#undef HAVE_SETSID

/* Define if you have the sysconf() function. */
#undef HAVE_SYSCONF

/* Define if you have the daemon() function. */
#undef HAVE_DAEMON

/* Define if you have the getopt() function. */
#undef HAVE_GETOPT

/* Define if you have the strerror() function. */
#undef HAVE_STRERROR

/* Define if you have vfork() */
#undef HAVE_VFORK

/* Define if you have vprintf() */
#undef HAVE_VPRINTF

/* Define if you have the `pw_comment' field in `struct passwd' (pwd.h) */
#undef HAVE_PW_COMMENT

/* Define if you have the `pw_gecos' field in `struct passwd' (pwd.h) */
#undef HAVE_PW_GECOS

/* Define to `long' if you don't have `off_t' */
#undef off_t

/* Define to `int' if you don't have `mode_t' */
#undef mode_t

/* Define to `int' if you don't have `pid_t' */
#undef pid_t

/* Define to `int' if you don't have `gid_t' and `uid_t' */
#undef uid_t
#undef gid_t

/* Define to `unsigned' if you don't have `size_t' */
#undef size_t

/* Define if you have <errno.h> */
#undef HAVE_ERRNO_H

@@BOTTOM@@

/*****************************************************************************\
                     DON'T TOUCH ANYTHING BELOW HERE!
\*****************************************************************************/

#ifndef HAVE_BYTE_TYPE
typedef unsigned char	byte;		     /* 8 bit unsigned critter */
#endif /* HAVE_BYTE_TYPE */

#ifndef HAVE_SIG_T
typedef void __sighandler_t (int);
typedef __sighandler_t  *sig_t;
#endif /* HAVE_SIG_T */

#ifndef HAVE_FALSE
#define FALSE (0)
#endif /* FALSE */

#ifndef HAVE_TRUE
#define TRUE (1)
#endif /* TRUE */

#ifndef HAVE_SETSID
#define setsid() ((pid_t) -1)
#endif /* HAVE_SETSID */

#ifndef HAVE_SYSCONF
#define sysconf(name) ((long) -1)
#endif /* HAVE_SYSCONF */

#ifndef HAVE_STRERROR
extern char *strerror (int errnum);
#endif /* HAVE_STRERROR */

#ifndef HAVE_GETOPT
extern int getopt (int argc, char **argv, const char *opts);
#endif /* HAVE_GETOPT */

#ifndef HAVE_DAEMON
extern int daemon (int nochdir, int noclose);
#endif

#ifdef HAVE_VFORK
#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif
#else /* HAVE_VFORK */
#undef vfork
#define vfork fork
#endif /* HAVE_VFORK */

#ifdef HAVE_ERRNO_H
#include <errno.h>
#else
extern int errno;
#endif

#ifndef STDC_HEADERS
#error "Must have standard C headers. Sorry."
#endif

/*
  Password field: If only one of `pw_comment' and `pw_gecos' is present,
  define the missing one in terms of the existing one. If neither is
  present, define both in terms of `pw_name'.
*/
#if !defined(HAVE_PW_COMMENT) && !defined(HAVE_PW_GECOS)
#define pw_comment pw_name
#define pw_gecos   pw_name
#else
#if defined(HAVE_PW_COMMENT) && !defined(HAVE_PW_GECOS)
#define pw_gecos pw_comment
#else
#if defined(HAVE_PW_GECOS) && !defined(HAVE_PW_COMMENT)
#define pw_comment pw_gecos
#endif
#endif
#endif

#include <sys/types.h>
#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#endif /* _CONFIG_H_ */
