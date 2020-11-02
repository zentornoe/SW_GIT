#include <stdio.h>
#include "stacksADT.h"

int main(void) {
	unsigned int num;
	int* digit;
	STACK* stack;

	stack = createStack();

	printf("Enter an integer :	");
	scanf("%d", &num);		// input the number as decimal

	while (num > 0) {
		digit = (int*)malloc(sizeof(int));	// memory allocation - integer
		*digit = num % 2;					// decimal -> binary : output the remainder divided by 2 in reverse order.
		pushStack(stack, digit);			// push
		num = num / 2;						// for next remainder
	}

	printf("The binary number is :	");
	while (!emptyStack(stack)) {
		digit = (int*)popStack(stack);		// pop : reverse order of push
		printf("%1d", *digit);				// data of digit is 1-digit integer
	}
	printf("\n");

	destroyStack(stack);
	return 0;
}
