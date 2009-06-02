/*---------------------------------------------------------------------------*\
  NAME
        strerror - get string representation of errno value

  DESCRIPTION

        Replacement for standard C strerror() function, in case it's missing.
        Not thread-safe.

  LICENSE

	This source code is released under a BSD-style license. See the
        LICENSE file for details.

  Copyright (c) 2003-2009 Brian M. Clapper, bmc <at> clapper <dot> org
\*---------------------------------------------------------------------------*/

#define MAX_INT_DIGITS  (11)		/* max. digits in a LONG (w/ sign) */

char *strerror (const int errno)
{
   extern char *sys_errlist [];		/* list of error messages */
   extern int sys_nerr;			/* size of sys_errlist */

   static char buffer [MAX_INT_DIGITS + 1];

   char *result;			/* function result */


   if ( (errno >= 0) && (errno < sys_nerr) )
       result = sys_errlist[errno];
   else
   {
       /*
         To avoid possible buffer overflow, avoid using sprintf() with
         "%d". snprintf() isn't available everywhere, so we convert the
         error code to a string manually. This is ASCII/ISO-8859-1 specific.
       */
       int i = MAX_INT_DIGITS;
       int save_errno = errno;
       int neg = 0;
       int stop = 0;

       buffer[i] = '\0';

       if (save_errno < 0)
       {
           neg++;
           stop = 1;
           save_errno = -save_errno;
       }

       while ( (--i >= stop) && (save_errno != 0) )
       {
           int digit = save_errno % 10;
           save_errno /= 10;
           buffer[i] = (char) (digit + '0');
       }

       if (neg)
           buffer[i] = '-';

       result = &(buffer[i]);
   }

   return (result);
}
