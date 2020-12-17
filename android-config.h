#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <libgen.h>

#define FALSE false
#define TRUE true

extern int x_getopt (int argc, char **argv, const char *opts);
extern int x_opterr;
extern int x_optind;
extern int x_optopt;
extern char *x_optarg;