#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include "stacksADT.h"

int main(void) {
	bool done = false;
	int* dataPtr;
	STACK* stack;

	stack = createStack();

	while (!done) {
		dataPtr = (int*)malloc(sizeof(int));		// local variable - dataPtr : memory allocation
		printf("Enter a number : <EOF> to stop :");
		if ((scanf("%d", dataPtr)) == EOF || fullStack(stack))	// if dataPtr is End of File
			done = true;										// done is true -> escape the while
		else
			pushStack(stack, dataPtr);							// else - number -> stack : push
	}

	printf("\n\nThe list of numbers reversed:\n");
	while (!emptyStack(stack)) {
		dataPtr = (int*)popStack(stack);						// dataPtr = int*
		printf("%3d\n", *dataPtr);								// print the data of dataPtr(pointer of integer)
		free(dataPtr);											// free the memory							
	}

	destroyStack(stack);
	return 0;
}