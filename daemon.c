/*---------------------------------------------------------------------------*\
  $Id$

  NAME
	daemon.c - replacement daemon(3) function

  DESCRIPTION

        This source file contains a version of BSD-style daemon(3)
        function, a function to "daemonize" the calling process.  This
        implementation is based both on the generic daemon logic defined in
        the Unix Programmer's FAQ and on the daemon_start() function in
        W. Richard Stevens' _Unix_Network_Programming_ book (Prentice-Hall,
        1990).  At the time of this writing, the Unix Programmer's FAQ is
        located at `http://www.whitefang.com/unix/faq_toc.html' (among
        other places).

  LICENSE

	This source code is released under the GNU Public License. See the
        COPYING file for details.

  Copyright (c) 2003 Brian M. Clapper, bmc <at> clapper <dot> org
\*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*\
                                 Includes
\*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include "config.h"

#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif /* HAVE_SYS_FILE_H */

/*---------------------------------------------------------------------------*\
                              Static Routines
\*---------------------------------------------------------------------------*/

/* redirect_fds(): redirect stdin, stdout, and stderr to /dev/NULL */

static void redirect_fds()
{
    (void) close (0);
    (void) close (1);
    (void) close (2);

    open ("/dev/null", O_RDWR);
    (void) dup (0);
    (void) dup (0);
}

static int do_fork (void)
{
    int status = 0;

    switch (fork())
    {
        case 0:
            /* This is the child that will become the daemon. */
            break;

        case -1:
            /* Fork failure. */
            status = -1;
            break;

        default:
            /* Parent: Exit. */
            _exit (0);
    }

    return status;
}

/*---------------------------------------------------------------------------*\
                              Public Routines
\*---------------------------------------------------------------------------*/

int daemon (int nochdir, int noclose)
{
    int status = 0;

    printf ("_SC_OPEN_MAX=%d", sysconf (_SC_OPEN_MAX));

    /* Fork once to go into the background. */
    if ( (status = do_fork()) < 0 )
        ;

    /* Create new session */
    else if (setsid() < 0)               /* shouldn't fail */
        status = -1;

    /* Fork again to ensure that daemon never reacquires a control terminal. */
    else if ( (status = do_fork()) < 0 )
        ;

    else
    {
        /* clear any inherited umask(2) value */

        umask (0);

        /* We're there. */

        if (! nochdir)
        {
            /* Go to a neutral corner. */
            chdir("/");
        }

        if (! noclose)
            redirect_fds();
    }

    return status;
}
