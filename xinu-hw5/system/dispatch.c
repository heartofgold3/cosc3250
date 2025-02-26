/**
 * @file dispatch.c
 * @provides dispatch
 * COSC3250 Assignment 5                                                     * @editors [MacKenna Bochnak and Dayane Garcia-Avila]
 * Instructor [Dennis Brylow]
 * TA-BOT:MAILTO mackenna.bochnak@marquette.edu dayane.garcia-avila@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2008, 2023.  All rights reserved. */


#include <xinu.h>
#include <interrupt.h> //needed found in the 'include' section within the project, needed in order to pass specific arguments.

/**
 * @ingroup process
 * Dispatch the trap or exception handler, called via interrupt.S
 * @param cause  The value of the scause register 
 * @param stval  The value of the stval register  
 * @param frame  The stack pointer of the process that caused the interupt 
 * @param program_counter  The value of the sepc register 
 */

void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    ulong swi_opcode;
    
    if((long)cause > 0) {    //the if-statement is to check to see if 'cause' is a positive value.
        cause = cause << 1;  //left shift
        cause = cause >> 1;  //then a right shift, Clears the most signifficant bit, after the shifting, cause is not utilizing the exception code.

       /**
	* TODO:
	* Check to ensure the trap is an environment call from U-Mode
	* Find the system call number that's triggered
	* Pass the system call number and any arguments into syscall_dispatch.
	* Make sure to set the return value in the appropriate spot.
	* Update the program counter appropriately with set_sepc
	*
	* If the trap is not an environment call from U-Mode call xtrap
	*/
	//needed if statement, that will allow for
        if (cause == E_ENVCALL_FROM_UMODE) {  //checking to see if cause is equal to to the enviromental
                swi_opcode = *(frame + 7);

                *frame = syscall_dispatch(swi_opcode, frame); //getting back the system call number,

                set_sepc((ulong)program_counter + 4);
        }

        //else statement, if the trap is not a system
        else {
                xtrap(frame, cause, val, program_counter);
        }
    }
}

