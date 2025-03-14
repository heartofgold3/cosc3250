/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 Assignment 4
 *
 * Embedded XINU, Copyright (C) 2008.  All rights reserved.
 * @editor MacKenna Bochnak Dayane Garcia-Avila
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO mackenna.bochnak@marquette.edu dayane.garcia-avila@marquette.edu
 */

#include <xinu.h>
#include <stdarg.h>
#include <string.h>

#define ARG_REG_MAX 8

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */

    if (ssize < MINSTK)
        ssize = MINSTK;

    ssize = (ulong)((((ulong)(ssize + 3)) >> 2) << 2);
    /* round up to even boundary    */
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];
	
    // TODO: Setup PCB entry for new process.
    ppcb->state = PRREADY;   
    ppcb->stkbase = (ulong*)saddr;     // Stack base
    ppcb->stklen = ssize;              // Stack size
    //strncpy(ppcb->name, name, PNMLEN); // Assign name
    ppcb -> name[PNMLEN - 1] = '\0';

    // Initialize stack with accounting block
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs > ARG_REG_MAX)
    {
        pads = ((nargs - ARG_REG_MAX - 1) / ARG_REG_MAX) * ARG_REG_MAX;
    }
    /* If more than 8 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

    // TODO: Initialize process context.
    for (i = 0; i < CONTEXT; i++){
	    ppcb -> ctx[i] = 0;
    }
    
    ppcb -> ctx[CTX_RA] = (ulong)userret;  // Sets RA to userret
    ppcb -> ctx[CTX_PC] = (ulong)funcaddr; // Sets PC to the function's address
    
    // TODO:  Place arguments into context and/or activation record.
    //        See K&R 7.3 for example using va_start, va_arg and
    //        va_end macros for variable argument functions.
    va_start(ap, nargs);
    // Store first 8 args in a0-a7 regs
    for(i=0; i < nargs && i < ARG_REG_MAX; i++){
	    ppcb -> ctx[CTX_A0 + i] = va_arg(ap, ulong);
    }

    for(i = nargs; i > ARG_REG_MAX; i--){
	    *++saddr = va_arg(ap, ulong);
    }
    va_end(ap);

    ppcb -> ctx[CTX_SP] = (ulong)saddr; // Sets SP to stack address
    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    // ASSIGNMENT 5 TODO: Replace the call to kill(); with user_kill();
    // when you believe your trap handler is working in Assignment 5
    // user_kill();
    kill(currpid); 
}
