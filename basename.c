/*---------------------------------------------------------------------------*\
  NAME
	basename.c - replacement basename(3) function

  DESCRIPTION

        This source file contains a version of the basename() function.

  LICENSE

	This source code is released under a BSD-style license. See the
        LICENSE file for details.

  Copyright (c) 2006-2009 Brian M. Clapper, bmc <at> clapper <dot> org
\*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*\
                                 Includes
\*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------------*\
                              Public Routines
\*---------------------------------------------------------------------------*/

char *basename (char *path)
{
    char *s = strrchr (path, '/');

    return (s == NULL) ? path : ++s;
}
