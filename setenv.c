/*---------------------------------------------------------------------------*\
  Version of the POSIX setenv(3) function, implemented in terms of the older
  putenv(3) function, for systems that don't have setenv(3).

  LICENSE

	This source code is released under a BSD-style. See the LICENSE
        file for details.
\*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*\
                                 Includes
\*---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include "config.h"

/*---------------------------------------------------------------------------*\
                              Public Routines
\*---------------------------------------------------------------------------*/

int setenv(const char *name, const char *value, int overwrite)
{
    int res = 0;

    if ((name == NULL) || (strlen(name) == 0) || (strchr(name, '=') != NULL))
    {
        res = -1;
        errno = EINVAL;
    }

    else
    {
        char *buf = (char *) malloc(strlen(name) + strlen(value) + 2);
        strncat(buf, name, strlen(name));
        strncat(buf, "=", 1);
        strncat(buf, value, strlen(value));
        res = putenv(buf);
    }

    return res;
}
