/*
 * Copyright (c) 1989 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#include <popper.h>
RCSID("$Id$");

/* 
 *  log:    Make a log entry
 */

static char msgbuf[MAXLINELEN];

int
pop_log(POP *p, int stat, char *format, ...)
{
    va_list     ap;

    va_start(ap, format);
#ifdef HAVE_VSPRINTF
        vsprintf(msgbuf,format,ap);
#else
        {
	    int a0 = va_arg(ap, int);
	    int a1 = va_arg(ap, int);
	    int a2 = va_arg(ap, int);
	    int a3 = va_arg(ap, int);
	    int a4 = va_arg(ap, int);
	    int a5 = va_arg(ap, int);
	    sprintf(msgbuf, format, a0, a1, a2, a3, a4, a5, 0, 4711);
	}
#endif /* HAVE_VSPRINTF */

    if (p->debug && p->trace) {
        fprintf(p->trace,"%s\n",msgbuf);
        fflush(p->trace);
    }
    else {
        syslog (stat,"%s",msgbuf);
    }
    va_end(ap);

    return(stat);
}
