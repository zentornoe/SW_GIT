#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "QueueADT.h"

#define QUEUE_MAX 50



int main(void) {

	QUEUE* queue;
	queue = createQueue();

	char form[100], buf[100];
	char *end = "end";
	const char* oper = "+-*/%";
	int* fin;
	int** dat;
	int index;
	int val1, val2;
	bool state = true;
	printf("Enter the calculation with 2 operands and 1 operator.");
	printf("\n(EX : 1*2)\n");
	printf("If you want to escape, enter the \"end\"\n\n\n");


	while (state==true) {
		printf("\nformula : ");
		scanf("%s", form);

		if (0==strcmp(form, end)) {
			//finish the while
	
			break;
		}

		memset(buf, 0, sizeof(buf));
		index = strcspn(form, oper);
		memcpy(buf, form, index);
		val1 = atoi(buf);
		val2 = atoi(&form[index+1]);

		switch (form[index]) {
		case '+': val1 += val2;	break;
		case '-': val1 -= val2; break;
		case '*': val1 *= val2; break;
		case '/': val1 /= val2; break;
		case '%': val1 %= val2; break;
		default: printf("Error. Invalid operator."); break;
		}
		if (queueCount(queue) >= QUEUE_MAX)
			printf("Full of Memory");
		fin = (int*)malloc(sizeof(int));
		*fin = val1;
		enqueue(queue, fin);

		printf("%s = %d, count : %d\n", form, *fin, queueCount(queue));
	}//while

	int sum = 0;
	int cnt = 0;
	
	while (!emptyQueue(queue)) {
		cnt++;
		(int*)dat = (int*)malloc(sizeof(int));
		*dat = (int*)queue->front->dataPtr;
		printf("%d : %d\n", cnt, **dat);
		sum += **dat;
		dequeue(queue, dat);
	}

	printf("\nThe sum is : ");
	printf("%d", sum);
	destroyQueue(queue);
	
	return 0;
}
