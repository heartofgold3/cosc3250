/* COSC 3250 - Lab Practical 1
 * MacKenna Bochnak
 * 
 */


#include <stdlib.h>

typdef struct Node{
	int values[BLOCK_SIZE];
	int count;
	struct Node* next;
}Node;

void struct Stack{
	Node* head;
}Stack;

void init_stack(Stack*stack){
	stack->head = NULL;
}

void push(Stack* stack, int value){
	if(stack->head == NULL || stack->head->count == BLOCK_SIZE){
		Node* new_node = (Node*)malloc(sizeof(Node));
		if(new_node == NULL){
			exit(EXIT_FAILURE);
		}
		new_node->count =0;
		new_node->next = stack->head;
		stack->head = new_node;
	}
}

void print_stack (Stack *stack){
}

int main(){

	char buffer[10]; // Xharacter array
	int index = 0;

	printf("=== Binary Conversion ===");

	char c; // Variable to store input character temp
	while ((c = getchar()) != '\n' && index < 9){
		buffer[index++] = c;
	}

	buffer[index] = '\0'; // terminate string to ensure it is valid

	int amount; //variable to store value entered by user
	float binary_conversion; // variable to store the conversion

	//identify binary (base 2) value and convert to binary (base 10)
	if(buffer[0] == '1'){ // value 1
		printf("1");







}
