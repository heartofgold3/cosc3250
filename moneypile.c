/**
 * COSC 3250 - Project 2
 * C program that reads in an arbitrarily long sequence of positive integer currency values and outputs each category of currency in reverse order.
 * MacKenna Bochnak
 * Dennis Brylow
 * TA-BOT:MAILTO mackenna.bochnak@marquette.edu
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLOCK_SIZE 1000  // Define the maximum number of values a block (node) can hold


// Node structure to hold a block of integers
typedef struct Node {
	int values[BLOCK_SIZE];  // Array to store integer values, with a maximum size of BLOCK_SIZE
	int count;                // Count of values currently in this no
	struct Node* next;       // Pointer to the next node in the stack
} Node;


// Structure to manage a stack of currency values
typedef struct Stack {
	Node* head;  // Pointer to the top node of the stack (the most recently added node)
} Stack;


// Initialize a stack by setting the head pointer to NULL (empty stack)
void init_stack(Stack* stack) {
	stack->head = NULL;  // Initially, the stack is empty, so head is NULL
}


// Push a value onto the stack
void push(Stack* stack, int value) {
	// If the stack is empty or the current top node is full, create a new node
	if (stack->head == NULL || stack->head->count == BLOCK_SIZE) {
		Node* new_node = (Node*)malloc(sizeof(Node));  // Allocate memory for a new node
		
		if (new_node == NULL) {  // Check for memory allocation failure
			fprintf(stderr, "Memory allocation failed.\n");
			exit(EXIT_FAILURE);  // Exit if memory allocation fails
		}

		new_node->count = 0;  // Initialize the count to 0, as there are no values in the new node
		new_node->next = stack->head;  // Link the new node to the current top of the stack
		stack->head = new_node;  // Make the new node the top of the stack
	}
	
	// Add the value to the current top node
	stack->head->values[stack->head->count++] = value;  // Store value and increment count
}


// Print and free the stack
void print_stack(Stack* stack, const char* currency_symbol) {
	printf("Virtual %s pile:\n", currency_symbol);  // Print the currency pile heading
	
	// Traverse the stack, printing the values in each node (from top to bottom)
	while (stack->head != NULL) {
		// Print the values in the current node in reverse order (from top to bottom)
		for (int i = stack->head->count - 1; i >= 0; i--) {
			printf("%s%d\n", currency_symbol, stack->head->values[i]);  // Print the currency value with the symbol
		}
		
		// Free the current node and move to the next node in the stack
		Node* temp = stack->head;
		stack->head = stack->head->next;
		free(temp);  // Free the memory of the node
	}
}


// Function to parse the input and determine currency type
void process_input(Stack* dollar_stack, Stack* euro_stack, Stack* pound_stack, Stack* yen_stack, Stack* rupee_stack) {
	char buffer[100];  // Buffer to hold the input string (e.g., "$100", "€200", etc.)
	int amount;        // Variable to store the parsed currency amount
	char c;            // Character to read input
	
	// Read input until EOF
	while ((c = getchar()) != EOF) {
		if (isspace(c)) {  // Ignore whitespace characters (spaces, newlines, etc.)
			continue;
		}
		
		// Start reading a currency value
		int index = 0;
		buffer[index++] = c;  // Store the first character of the value (currency symbol)
		while ((c = getchar()) != EOF && !isspace(c)) {  // Continue reading until whitespace or EOF
			buffer[index++] = c;  // Store the characters of the currency value
		}

		buffer[index] = '\0';  // Null-terminate the string
		
		// Process the currency value based on its prefix (currency symbol)
		if (buffer[0] == '$') {  // US Dollar
			sscanf(buffer + 1, "%d", &amount);  // Parse the amount after the "$"
			printf("Putting %s in $ pile.\n", buffer);
			push(dollar_stack, amount);  // Push the amount to the dollar stack
		}
		
		else if (buffer[0] == '\xC2' && buffer[1] == '\xA3') {  // British Pound (Unicode: £)
			sscanf(buffer + 2, "%d", &amount);
			printf("Putting %s in \xC2\xA3 pile.\n", buffer);
			push(pound_stack, amount);  // Push the amount to the pound stack
		}
		
		else if (buffer[0] == '\xC2' && buffer[1] == '\xA5') {  // Yen (Unicode: ¥)
			sscanf(buffer + 2, "%d", &amount);
			printf("Putting %s in \xC2\xA5 pile.\n", buffer);
			push(yen_stack, amount);  // Push the amount to the yen stack
		}
		
		else if (buffer[0] == '\xE2' && buffer[1] == '\x82' && buffer[2] == '\xAC') {  // Euro (Unicode: €)
			sscanf(buffer + 3, "%d", &amount);
			printf("Putting %s in \xE2\x82\xAC pile.\n", buffer);
			push(euro_stack, amount);  // Push the amount to the euro stack
		}
		
		else if (buffer[0] == '\xE2' && buffer[1] == '\x82' && buffer[2] == '\xB9') {  // Rupee (Unicode: ₹)
			sscanf(buffer + 3, "%d", &amount);
			printf("Putting %s in \xE2\x82\xB9 pile.\n", buffer);
			push(rupee_stack, amount);  // Push the amount to the rupee stack
		}
		
		else {  // If the currency symbol is unrecognized, print an error
			fprintf(stderr, "Unrecognized currency: %s\n", buffer);
		}
	}
}

int main() {
	// Declare stacks for each currency
	Stack dollar_stack, euro_stack, pound_stack, yen_stack, rupee_stack;
	
	// Initialize each stack
	init_stack(&dollar_stack);
	init_stack(&euro_stack);
	init_stack(&pound_stack);
	init_stack(&yen_stack);
	init_stack(&rupee_stack);
	
	printf("=== Virtual Money Pile ===\n");
	
	// Process the input and sort the currency values into the appropriate stacks
	process_input(&dollar_stack, &euro_stack, &pound_stack, &yen_stack, &rupee_stack);
	
	// Print the stacks for each currency type
	print_stack(&dollar_stack, "$");
	print_stack(&pound_stack, "\xC2\xA3");
	print_stack(&yen_stack, "\xC2\xA5");
	print_stack(&euro_stack, "\xE2\x82\xAC");
	print_stack(&rupee_stack, "\xE2\x82\xB9");
	
	return 0;  // Exit the program successfully
}

