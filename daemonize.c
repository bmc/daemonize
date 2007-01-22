/*---------------------------------------------------------------------------*\
  $Id$

  NAME
	daemonize - run a command as a Unix daemon

  DESCRIPTION

	See accompanying man page for full details

  LICENSE

	This source code is released under a BSD-style license. See the
        LICENSE file for details.

  Copyright (c) 2003-2007 Brian M. Clapper, bmc <at> clapper <dot> org
\*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdarg.h>
#include <fcntl.h>
#include "config.h"

/*---------------------------------------------------------------------------*\
                                 Constants
\*---------------------------------------------------------------------------*/

#define VERSION "1.5.1"

/*---------------------------------------------------------------------------*\
                                  Globals
\*---------------------------------------------------------------------------*/

static const char  *pidFile   = NULL;
static const char  *outFile   = NULL;
static const char  *errFile   = NULL;
static bool         beVerbose = FALSE;
static const char  *user      = NULL;
static char       **cmd       = NULL;
static const char  *cwd       = "/";
static int          nullFd    = -1;
static int          outFd     = -1;
static int          errFd     = -1;
static int          append    = 0;

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

static void usage (char *prog)
{
    static const char *USAGE[] = 
    {
"Usage: %s [OPTIONS] path [arg] ...",
"",
"OPTIONS",
"",
"-a           Append to, instead of overwriting, output files. Ignored ",
"             unless -e and/or -o are specified.",
"-c <dir>     Set daemon's working directory to <dir>.",
"-e <stderr>  Send daemon's stderr to file <stderr>, instead of /dev/null.",
"-o <stdout>  Send daemon's stderr to file <stdout>, instead of /dev/null.",
"-p <pidfile> Save PID to <pidfile>.",
"-u <user>    Run daemon as user <user>. Requires invocation as root.",
"-v           Issue verbose messages to stdout while daemonizing"
    };
    int i;

    prog = basename (prog);
    fprintf (stderr, "%s, version %s\n", prog, VERSION);
    for (i = 0; i < sizeof (USAGE) / sizeof (const char *); i++)
    {
        fprintf (stderr, USAGE[i], prog);
        fputc ('\n', stderr);
    }

    exit (1);
}

static void parseParams (int argc, char **argv)
{
    int  opt;
    int  argsLeft;

    opterr = 0;

    /*
      NOTE: x_getopt() is the old public domain getopt(). The source lives
      in "getopt.c". The function's name has been changed to avoid
      conflicts with the native getopt() on the host operating system. So
      far, I've seen two kinds of conflicts:

      1. GNU getopt() (e.g., on Linux systems) behaves differently from the
         old getopt(), unless POSIXLY_CORRECT is defined in the
         environment. Specifically, it insists on processing options even
         after the first non-option argument has been seen on the command
         line. Initially, I got around this problem by forcing the use of
         the included public domain getopt() function.

      2. The types used in the included public domain getopt() conflict with
         the types of the native getopt() on some operating systems (e.g.,
         Solaris 8).

      Using x_getopt() ensures that daemonize uses its own version, which
      always behaves consistently.
    */
    while ( (opt = x_getopt (argc, argv, "ac:u:p:vo:e:")) != -1)
    {
        switch (opt)
        {
            case 'a':
                append = 1;
                break;

            case 'c':
                cwd = x_optarg;
                break;

            case 'p':
                pidFile = x_optarg;
                break;

            case 'v':
                beVerbose = TRUE;
                break;

            case 'u':
                user = x_optarg;
                break;

            case 'o':
                outFile = x_optarg;
                break;

            case 'e':
                errFile = x_optarg;
                break;
	

            default:
                fprintf (stderr, "Bad option: -%c\n", x_optopt);
                usage (argv[0]);
        }
    }

    argsLeft = argc - x_optind;
    if (argsLeft < 1)
        usage (argv[0]);

    cmd  = &argv[x_optind];
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

static int open_output_file (const char *path)
{
    int flags = O_CREAT | O_WRONLY;

    if (append)
    {
        verbose ("Appending to %s\n", path);
        flags |= O_APPEND;
    }

    else
    {
        verbose ("Overwriting %s\n", path);
        flags |= O_TRUNC;
    }

    return open (path, flags, 0666);
}

static void open_output_files()
{
    /* open files for stdout/stderr */

    if ((outFile != NULL) || (errFile != NULL))
    {
        if ((nullFd = open ("/dev/null", O_WRONLY)) == -1)
            die ("Can't open /dev/null: %s\n", strerror (errno));

        close (STDIN_FILENO);
        dup2 (nullFd, STDIN_FILENO);

        if (outFile != NULL)
        {
            if ((outFd = open_output_file (outFile)) == -1)
            {
                die ("Can't open \"%s\" for stdout: %s\n", 
                     outFile, strerror (errno));
            }
        }

        else 
        {
            outFd = nullFd;
        }

        if (errFile != NULL)
        {
            if ((outFile != NULL) && (strcmp (errFile, outFile) == 0))
                errFd = outFd;

            else if ((errFd = open_output_file (errFile)) == -1)
            {
                die ("Can't open \"%s\" for stderr: %s\n", 
                     errFile, strerror (errno));
            }
        }

        else
        {
            errFd = nullFd;
        }
    }
}

static int redirect_stdout_stderr()
{
    int rc = 0;

    /* Redirect stderr/stdout */

    if ((outFile != NULL) || (errFile != NULL))
    {
        close (STDIN_FILENO);
        close (STDOUT_FILENO);
        close (STDERR_FILENO);
        dup2 (nullFd, STDIN_FILENO);
        dup2 (outFd, STDOUT_FILENO);
        dup2 (errFd, STDERR_FILENO);
        rc = 1;
    }

    return rc;
}

/*---------------------------------------------------------------------------*\
                               Main Program
\*---------------------------------------------------------------------------*/

int main (int argc, char **argv)
{
    uid_t  uid = getuid();
    FILE  *fPid = NULL;
    int    noclose = 0;

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

    open_output_files();

    if (user != NULL)
        switchUser (user, pidFile, uid);

    if (chdir (cwd) != 0)
    {
	die ("Can't change working directory to \"%s\": %s\n",
             cwd, strerror (errno));
    }

    verbose ("Daemonizing...");

    if (redirect_stdout_stderr())
        noclose = 1;

    if (daemon (1, noclose) != 0)
        die ("Can't daemonize: %s\n", strerror (errno));

    if (fPid != NULL)
    {
        fprintf (fPid, "%d\n", getpid());
        fclose (fPid);
    }

    if (chdir (cwd) != 0)
    {
	die ("Can't change working directory to \"%s\": %s\n",
             cwd, strerror (errno));
    }

    execvp (cmd[0], cmd);

    die ("Can't exec \"%s\": %s\n", cmd[0], strerror (errno));
}

/*  vim: set et sw=4 sts=4 : */

