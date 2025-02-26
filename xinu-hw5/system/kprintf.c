/**
 * COSC 3250 - Project 3
 * a simple synchronous serial driver for the embedded operating system
 * @authors MacKenna Bochnak and Dayane Gracia-Avila
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO mackenna.bochnak@marquette.edu dayane.garcia-avila@marquette.edu
 */

#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */
static unsigned char ungetArray[UNGETMAX];
unsigned int  bufp;

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: First, check the unget buffer for a characregptr->thr = c; // send the characterter.
    if (bufp > 0) {
	    return ungetArray[--bufp];  // Retrieve the last ungotten character
    }
    
    //       Otherwise, check UART line status register for available data
    while (!(regptr->lsr & UART_LSR_DR));  // Wait until data is available (LSR bit 0)
    
    //       once there is data ready, get character c.
    return regptr->rbr;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: Check the unget buffer and the UART for characters.
    if (bufp > 0) {
	    return TRUE;  // There is a character available in the buffer
    }

    if (regptr->lsr & UART_LSR_DR) {  // LSR bit 0 set means data is ready
        return TRUE;
    }

	return FALSE;  // No character available
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    // TODO: Check for room in unget buffer, put the character in or discard.
    if (bufp >= UNGETMAX) {
        return SYSERR;  // Buffer is full, cannot unget more characters
    }
    ungetArray[bufp++] = c;
    
    return c; //return the character
}

syscall kputc(uchar c)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: Check UART line status register.
    while(!(regptr->lsr & UART_LSR_THRE)); // wait for transmitter holding register to be empty

    //       Once the Transmitter FIFO is empty, send character c.
    regptr->thr = c; // send the character

    return c;
}

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(long, long))kputc, 0);
    va_end(ap);
    return retval;
}
