#include <stdarg.h>
//#include "error_functions.h"
#include "tlpi_hdr.h"
#include "ename.c.inc"    /* Defines ename and MAX_ENAME */

#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#endif

static void
terminate(Boolean useExit3)
{
    char *s;

    /* Dump core if EF_DUMPCORE environment variable is defined and
     * is a nonempty string; otherwise call exit(3) or _exit(2),
     * depending on the value of "useExit3". */

    s = getenv("EF_DUMPCORE");
    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

/* TODO: complete last functions */
