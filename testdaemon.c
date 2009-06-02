/*---------------------------------------------------------------------------*\
  NAME
	testdaemon - simple test daemon

  DESCRIPTION

	Simple test daemon. Loops forever and writes to stdout and stderr.

  LICENSE

	This source code is released under a BSD-style license. See the
        LICENSE file for details.

  Copyright (c) 2003-2009 Brian M. Clapper, bmc <at> clapper <dot> org
\*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

/*---------------------------------------------------------------------------*\
                             Private Functions
\*---------------------------------------------------------------------------*/

static char buf[64];
static const char *current_time()
{
    time_t now;

    now = time ((time_t *) 0);
    strftime (buf, sizeof (buf), "%Y/%m/%d %H:%M:%S", localtime (&now));
    return buf;
}

static void msg (FILE *fp, const char *format, ...)
{
    va_list ap;

    fputc ('[', fp);
    fputs (current_time(), fp);
    fputs ("] ", fp);

    va_start (ap, format);
    vfprintf (fp, format, ap);
    va_end (ap);

    fputc ('\n', fp);
    fflush (fp);
}


/*---------------------------------------------------------------------------*\
                               Main Program
\*---------------------------------------------------------------------------*/

int main (int argc, char **argv)
{
    char cwdbuf[256];

    msg (stdout, "----------- NEW RUN");
    msg (stderr, "----------- NEW RUN");
    msg (stdout, "Working directory=%s", getcwd (cwdbuf, sizeof (cwdbuf)));

    for (;;)
    {
        sleep (2);
        msg (stdout, "Message to standard output.");
        msg (stderr, "Message to standard error.");
    }

    return 0;
}

/*  vim: set et sw=4 sts=4 : */

