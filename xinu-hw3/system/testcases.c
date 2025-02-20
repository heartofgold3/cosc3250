/**
 * @file testcases.c
 * @provides testcases
 *
 * Modified by:
 * Dayane Garcia-Avila
 * and
 * MacKenna Bochnak
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>
#define UNGETMAX 10

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void) {
	
	int c;
	int result;
	char test_char = 'E';
	
	kprintf("=== TEST BEGIN ===\r\n");
	
	// Test 1 - Check if kputc() correctly outputs a character
	kprintf("Test 1: Testing kputc(): ");
	if (kputc(test_char) == test_char) {
		kprintf("PASSED\r\n");
	} else {
		kprintf("FAILED\r\n");
	}
	
	// Test 2 - Check kputc() with an invalid character
	kprintf("Test 2: Testing kputc() with an invalid character: ");
	if (kputc('\0') == SYSERR) {
		kprintf("PASSED\r\n");
	} else {
		kprintf("FAILED\r\n");
	}
	
	// Test 3 - Check if kgetc() correctly retrieves a character
    	kprintf("Test 3: Testing kgetc(): ");
    	kungetc(test_char); // Put back a character to test retrieval
    	c = kgetc();
    	if (c == test_char) {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED (Got character %c)\r\n", c);
    	}

    	// Test 4 - Check kgetc() when buffer is empty
    	kprintf("Test 4: Testing kgetc() when buffer is empty: ");
    	bufp = 0; // Ensure buffer is empty
    	if (kgetc() == SYSERR) {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED\r\n");
    	}	

    	// Test 5 - Verify kcheckc() detects available data
    	kprintf("Test 3: Testing kcheckc() when data is available: ");
    	kungetc(test_char); // Put back a character so it should be available
    	if (kcheckc() == TRUE) {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED\r\n");
    	}

    	// Test 4 - Verify kcheckc() detects no available data
    	kprintf("Test 4: Testing kcheckc() when no data is available: ");
    	bufp = 0; // Clear buffer to ensure no data
    	if (kcheckc() == FALSE) {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED\r\n");
    	}

    	// Test 5 - Check kungetc() when there is available space
    	kprintf("Test 5: Testing kungetc() with available space: ");
    	bufp = 0; // Reset buffer to empty
    	result = kungetc(test_char);
    	if (result == test_char && bufp == 1) {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED\r\n");
    	}

    	// Test 6 - Check kungetc() when the buffer is full
    	kprintf("Test 6: Testing kungetc() when buffer is full: ");
    	bufp = UNGETMAX; // Fill up buffer
    	if (kungetc(test_char) == SYSERR) {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED\r\n");
    	}

    	// Test 7 - Check multiple successive calls to kungetc()
   	kprintf("Test 7: Testing multiple successive kungetc() calls: ");
    	bufp = 0; // Reset buffer
    	kungetc('A');
    	kungetc('B');
    	if (kgetc() == 'B' && kgetc() == 'A') {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED\r\n");
    	}

    	// Test 8 - Check if buffer properly resets after multiple reads
    	kprintf("Test 8: Testing buffer reset after multiple reads: ");
    	bufp = 0;
    	kungetc('X');
    	kgetc(); // Read the character
    	if (bufp == 0) {
        	kprintf("PASSED\r\n");
    	} else {
        	kprintf("FAILED\r\n");
    	}

    kprintf("\r\n===TEST END===\r\n");
    return;
}

