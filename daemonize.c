/*---------------------------------------------------------------------------*\
  $Id$

  NAME
	daemonize - run a command as a Unix daemon

  DESCRIPTION

	See accompanying man page for full details

  LICENSE

	This source code is released under the GNU Public License. See the
        COPYING file for details.

  Copyright (c) 2003 Brian M. Clapper, bmc <at> clapper <dot> org
\*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdarg.h>
#include "config.h"

/*---------------------------------------------------------------------------*\
                                  Globals
\*---------------------------------------------------------------------------*/

static const char  *pidFile   = NULL;
static bool         beVerbose = FALSE;
static const char  *user      = NULL;
static char       **cmd       = NULL;
static const char  *cwd       = "/";

/*---------------------------------------------------------------------------*\
                             Private Functions
\*---------------------------------------------------------------------------*/

static void die (const char *format, ...)
{
    va_list ap;

    va_start (ap, format);
    vfprintf (stderr, format, ap);
    va_end (ap);

    exit (1);
}

static void verbose (const char *format, ...)
{
    va_list ap;

    if (beVerbose)
    {
        va_start (ap, format);
        vfprintf (stdout, format, ap);
        va_end (ap);
    }
}

static void usage (const char *prog)
{
    die ("Usage: %s [-c dir] [-p pidfile] [-u user] [-v] path [arg] ...\n",
         prog);
}

static void parseParams (int argc, char **argv)
{
    int  opt;
    int  argsLeft;

    opterr = 0;
    while ( (opt = getopt (argc, argv, "u:p:v")) != -1)
    {
        switch (opt)
        {
            case 'c':
                cwd = optarg;
                break;

            case 'p':
                pidFile = optarg;
                break;

            case 'v':
                beVerbose = TRUE;
                break;

            case 'u':
                user = optarg;
                break;

            default:
                fprintf (stderr, "Bad option: -%c\n", optopt);
                usage (argv[0]);
        }
    }

    argsLeft = argc - optind;
    if (argsLeft < 1)
        usage (argv[0]);

    cmd  = &argv[optind];
    return;
}

static void switchUser (const char *userName,
                        const char *pidFile,
                        uid_t       uid)
{
    struct  passwd *pw;

    if (uid != 0)
        die ("Must be root to specify a different user.\n");

    if ( (pw = getpwnam (userName)) == NULL )
	die ("Can't find user \"%s\" in password file.\n", userName);

    if (pidFile != NULL)
    {
        verbose ("Changing ownership of PID file to \"%s\" (%d)\n",
                 user, pw->pw_uid);
        if (chown (pidFile, pw->pw_uid, pw->pw_gid) == -1)
        {
            die ("Can't change ownership of PID file to \"%s\" (%d): %s\n",
                 user, pw->pw_uid, strerror (errno));
        }
    }

    if (setgid (pw->pw_gid) != 0)
	die ("Can't set gid to %d: %s\n", pw->pw_gid, strerror (errno));

    if (setegid (pw->pw_gid) != 0)
	die ("Can't set egid to %d: %s\n", pw->pw_gid, strerror (errno));

    if (setuid (pw->pw_uid) != 0)
	die ("Can't set uid to %d: %s\n", pw->pw_uid, strerror (errno));

    if (seteuid (pw->pw_uid) != 0)
	die ("Can't set euid to %d: %s\n", pw->pw_uid, strerror (errno));
}

/*---------------------------------------------------------------------------*\
                               Main Program
\*---------------------------------------------------------------------------*/

int main (int argc, char **argv)
{
    uid_t   uid = getuid();
    FILE   *fPid = NULL;

    if (geteuid() != uid)
        die ("This executable is too dangerous to be setuid.\n");

    parseParams (argc, argv);

    if (cmd[0][0] != '/')
        die ("The 'path' parameter must be an absolute path name.");

    /* Verify that the path to the command points to an existing file. */

    if (access (cmd[0], F_OK) == -1)
        die ("File \"%s\" does not exist.\n", cmd[0]);

    if (pidFile != NULL)
    {
        verbose ("Opening PID file \"%s\".\n", pidFile);
        if ( (fPid = fopen (pidFile, "w")) == NULL )
        {
            die ("Can't open PID file \"%s\": %s\n",
                 pidFile, strerror (errno));
        }
    }

    if (user != NULL)
        switchUser (user, pidFile, uid);

    if (chdir (cwd) != 0)
    {
	die ("Can't change working directory to \"%s\": %s\n",
             cwd, strerror (errno));
    }

    verbose ("Daemonizing...");
    if (daemon (0, 0) != 0)
        die ("Can't daemonize: %s\n", strerror (errno));

    if (fPid != NULL)
    {
        fprintf (fPid, "%d\n", getpid());
        fclose (fPid);
    }

    execvp (cmd[0], cmd);

    die ("Can't exec \"%s\": %s\n", cmd[0], strerror (errno));
}
