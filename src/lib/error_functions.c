#include <stdarg.h>
#include "error_functions.h"
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

static void
outputError(Boolean useErr, int err, Boolean flushStdout,
        const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");
    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);
    if (flushStdout)
        fflush(stdout);      /* Flush any pending stdout */
    fputs(buf, stderr);
    fflush(stderr);          /* In case stderr is not line-buffered */
}

/* errMsg:  prints a message on standard error */
void
errMsg(const char *format, ...)
{
    va_list argList;
    int savedErrno;

    savedErrno = errno;  /* In case we change it here */

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);
    errno = savedErrno;
}

/* errExit:  prints a message on standard error and teminates the 
 * program */
void
errExit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/* err_exit:  similar to "errExit", except that:
 * 1 It doesn't flush standard output before printing the error message
 * 2 It terminates the process by calling _exit() instead of exit() */
void
err_exit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, FALSE, format, argList);
    va_end(argList);

    terminate(FALSE);
}

/* errExitEN:  prints a message based on the argument errnum and 
 * terminates the program */
void
errExitEN(int errnum, const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errnum, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/* fatal:  diagnose general errors, including errors from library
 * functions that don't set errno */
void fatal(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(FALSE, 0, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/* usageErr:  diagnose errors in command-line argument usage */
void
usageErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);    /* Flush any pending stdout */
    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);    /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}

/* cmdLineErr:  similar to usageErr(), but is intended for diagnosing
 * errors in the command-line arguments specified to a program */
void
cmdLineErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);    /* Flush any pending stdout */

    fprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);    /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}
