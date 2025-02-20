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


//Needed libraries to ensure everything works correctly.
#include <xinu.h>
#include <proc.h>

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
    ppcb->state = PRREADY;   //Proccess is ready to be exectuted
    ppcb->stkbase = (void *)((ulong)saddr + ssize);   // Stack base
    ppcb->stklen = ssize;    // Stack size
    strncpy(ppcb->name, name, PNMLEN);	//needs the name of the proccess in order to be able put it in the PCB struct

    // Initialize stack with accounting block
    *saddr = STACKMAGIC;   	//STACKMAGIC is essentially the information of that specific 'coloumn'/space
    *--saddr = pid;   		//Being able to push the pid to the stack
    *--saddr = ppcb->stklen;  	//Being able to know the stack size reference (aka stklen) and pushes size onto the stack 
    *--saddr = (ulong)ppcb->stkbase; //needed for base address to know where something is located in the stack

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / ARG_REG_MAX) * ARG_REG_MAX;
    }
    /* If more than 8 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;  //decreases the pointer, then derferences
    }        	       //making sure it sets that memory loc to zero. 

    // TODO: Initialize process context.
    // Need to have these initalized, because these are important for the stack to know where we are within our list/coloumn
    //
    ppcb -> ctx[CTX_PC] = (ulong)funcaddr; // Setting up the PC = pointer counter to the function's address
    ppcb -> ctx[CTX_RA] = (ulong)userret; //Setting up the RA = return address 
    ppcb -> ctx[CTX_SP] = (ulong)saddr; // Setting up the SP = stack pointer  to stack adress

    
    // TODO:  Place arguments into context and/or activation record.
    //        See K&R 7.3 for example using va_start, va_arg and
    //        va_end macros for variable argument functions.
    //

    va_start(ap, nargs);  //initalizing, making sure we can intake arguments 
    // Store first 8 args in a0-a7 regs
    for(i=0; i < nargs && i < 8; i++){
            ppcb -> ctx[CTX_A0 + i] = va_arg(ap, ulong);   //the A0 is the beginning of where the arguments will go into
    }
    // Store rest in stack, the other arguments will be going to the s's registers. 
    for(; i < nargs; i++){
            *--saddr = va_arg(ap, ulong);
    }
    va_end(ap);   //ensuring that the arguments come to an end


    return pid;  //ensuring to return back to the proccess id
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 * Ensuring that new PIDs can be allocated, even with multiple proccess
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return*/
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots*/
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
