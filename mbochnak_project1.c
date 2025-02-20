/**
 * COSC 3250 - project 1
 * International Currency Converter
 * MacKenna Bochnak
 * Dennis Brylow
 * TA-BOT:MAILTO mackenna.bochnak@marquette.edu
*/

#include <stdio.h>
#include <stdlib.h>

// Function to convert amount from one currency to dollars
int convert_currency(int entered_amount, float conversion_rate) {
	return (int)(entered_amount / conversion_rate); // Truncate to integer
}

int main() {
	// Define conversion rates (to dollars)
	float dollar_dollar = 1.0;
	float dollar_euro = 0.897;
	float dollar_bps = 0.766;
	float dollar_yen = 109.94;
	float dollar_rupee = 70.845;
	
	char buffer[10]; // Buffer to hold the currency symbol and amount
	int index = 0;   // Current index in the buffer
	
	printf("=== Currency Converter ===\n");
	
	// Read input character by character
	char c;
	while ((c = getchar()) != '\n' && index < 9) {
		buffer[index++] = c;
	}
	buffer[index] = '\0'; // Null-terminate the string
	
	// Identify the currency symbol (first few characters)
	int amount;
	float conversion_rate;
	if (buffer[0] == '\xE2' && buffer[1] == '\x82' && buffer[2] == '\xAC') { // Euro (\u20AC)
		conversion_rate = dollar_euro;
		sscanf(buffer + 3, "%d", &amount);
	}
	else if (buffer[0] == '\xC2' && buffer[1] == '\xA3') { // British Pound (\u00A3)
		conversion_rate = dollar_bps;
		sscanf(buffer + 2, "%d", &amount);
	}
	else if (buffer[0] == '\xC2' && buffer[1] == '\xA5') { // Yen (\u00A5)
		conversion_rate = dollar_yen;
		sscanf(buffer + 2, "%d", &amount);
	}
	else if (buffer[0] == '\xE2' && buffer[1] == '\x82' && buffer[2] == '\xB9') { // Rupee (\u20B9)
		conversion_rate = dollar_rupee;
		sscanf(buffer + 3, "%d", &amount);
	}
	else if (buffer[0] == '$') { // Dollar ($)
		conversion_rate = dollar_dollar;
		sscanf(buffer + 1, "%d", &amount);
	}
	else {
		printf("Invalid input or unsupported currency symbol.\n");
		return 1;
	}
	
	// Convert the amount to dollars
	int converted_amount = convert_currency(amount, conversion_rate);
	
	// Print the result
	printf("%s = $%d\n", buffer, converted_amount);
	
	return 0;
}
