/*---------------------------------------------------------------------------*\
  Site-specific configuration file for run-as utility

  Input to `autoheader'

  This source code is released under a BSD-style license. See the LICENSE
  file for details.
\*---------------------------------------------------------------------------*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <auto_config.h>
#include <sys/types.h>

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

extern int x_getopt (int argc, char **argv, const char *opts);
extern int x_opterr;
extern int x_optind;
extern int x_optopt;
extern char *x_optarg;

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
