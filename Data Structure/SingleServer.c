#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "QueueADT.h"	// Header of Queue

typedef struct {
	int custNum, arriveTime, startTime, svcTime;
}CustStatus;

typedef struct {
	int numCust, totSvcTime, totWaitTime,startTime, arvTime;
}SimStats;



/*determines if new Customer has arrived*/
void newCustomer(QUEUE* queue, int clock, int custNum) {
	srand(time(NULL));
	if (3 == (rand() % 4)) {	//new customer
		CustStatus* custStatus = { 0, };
		custStatus = (CustStatus*)malloc(sizeof(CustStatus));
		custNum++;
		custStatus->custNum = custNum;
		custStatus->arriveTime = clock;
		custStatus->startTime = 0;
		srand(time(NULL));
		custStatus->svcTime = rand() % 10 + 1;
		enqueue(queue, custStatus);
	}
}

/*the server is idle of not*/
void serverFree(QUEUE* queue, int clock, int svcTime, SimStats* simStats, bool moreCusts) {
	if (clock > simStats->startTime + svcTime - 1) {
		simStats->numCust++;
		//server is idle
		if (emptyQueue(queue) == false) {
			CustStatus* custData = { 0, };
			custData = (CustStatus*)malloc(sizeof(CustStatus));
			dequeue(queue, custData);
			custData->startTime = clock;
			simStats->arvTime = custData->arriveTime;
			svcTime = custData->svcTime;
			simStats->totSvcTime += custData->svcTime;
			simStats->totWaitTime += custData->startTime - custData->arriveTime;
			moreCusts = true;
		}
	}
}

void svcComplete(QUEUE* queue, int clock, SimStats* simStats, int svcTime, bool moreCusts) {
	if (clock == simStats->startTime+svcTime-1) {
		printf("Cust num : %d, arrive time : %d, start time : %d", simStats->numCust, simStats->arvTime);
		printf(", service time : %d", simStats->startTime);
		printf(", wait Time : %d, Queue count : %d", simStats->totWaitTime - simStats->startTime, queue->count);
		moreCusts = false;
	}
}



int main(void) {

	QUEUE* head;
	head = createQueue();

	

	printf("This is the simulation of Single Server\n");

	SimStats* simStats = { 0, };
	simStats = (SimStats*)malloc(sizeof(SimStats));
	
	int clock = 0;
	int custNum = 0, arriveTime = 0, startTime = 0, svcTime = 0;	//custStatus
	int numCust = 0, toSvcTime = 0, toWaitTime = 0, maxQueueSize = 0;	//simStats
	bool moreCusts = true;

	


	while (clock< 480) {	// 480 min = 60 min/hours * 8 hours
		newCustomer(head, clock, custNum);
		serverFree(head, clock, svcTime, simStats, moreCusts);
		svcComplete(head, clock, simStats, svcTime, moreCusts);

		if (true!=emptyQueue)
			moreCusts = true;

		clock++;
	}
	//print

}
