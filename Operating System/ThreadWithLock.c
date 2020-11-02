#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RUN 100000 // random : 100,000 times
#define MIN 0      // random min val
#define MAX 99     // random max val
#define NUM 100    // size of array
#define THR 10     // # of thread

// Mutex
pthread_mutex_t locker;

// Global Variables
int number[100]; //shared variable, counted random number

// Thread Function
void *t_function(void *data)
{
    int count = 0; // counting 100,000
    while (count < RUN)
    {
        srand(time(NULL));              //seed = time,
        int n = rand() % NUM;           // random number n : 0 ~ 99
        // *** Critical Section ***
        pthread_mutex_lock(&locker);    // Mutex lock
        number[n]++;                    // Counting total numbers
        pthread_mutex_unlock(&locker);  // Mutex unlock
        // ************************
        ((int *)data)[n]++;             // counting thread's numbers
        count++;
    }
}

int main()
{
    int i = 0, j = 0;               // Loop variables
    int num[THR][NUM] = { 0, };     // Number array of each Thread
    int thr_com[THR];               // Thread creation status

    pthread_t tid[THR];     // Threads

    pthread_mutex_init(&locker, NULL);  // Mutex initialization
    // Thread Creation
    for (i = 0; i < THR; i++)
    { // Detect the thread creation error
        if (pthread_create(&tid[i], NULL, t_function, (void *)num[i]) < 0)
        {
            // Error message
            perror("*** Thread create error!!! ***");
            exit(0);
        }
    }

    // Wait for threads to end
    for (i = 0; i < THR; i++)
    { // Return Threads
        pthread_join(tid[i], NULL);
    }

    // Total
    int total = 0;                  // Ideal : 1,000,000
    int th_total = 0;               // Ideal : 1,000,000
    int t_total[THR] = { 0, };      // Each thread - Ideal : 100,000
    int t_sum[NUM] = { 0, };        //  Sum of thread for each number
    for (i = 0; i < NUM; i++)
    {
        total += number[i];     // Sum of all Threads
        for (j = 0; j < THR; j++)
        {
            t_sum[i] += num[j][i];      // Sum of each number for all threads
            t_total[j] += num[j][i];    // Sum of each Threads
        }
        th_total += t_sum[i];
    }

    // File
    FILE *fp;                      // File pointer
    fp = fopen("result.dat", "w"); // Mode : writing(+create), File name : result.dat

    fprintf(fp, "Result\t   Total   \tThread Total\t");
    for (i = 0; i < THR; i++)
        fprintf(fp, "Thread%d\t", i); // Result header

    for (i = 0; i < NUM; i++)
    {                                       // Write each number data
        fprintf(fp, "\n%d\t", i);           // Number
        fprintf(fp, "%7d\t\t", number[i]);  // Shared variable in each number
        fprintf(fp, "%7d\t\t", t_sum[i]);   // Sum of all threads in each number
        for (j = 0; j < THR; j++)
        {
            fprintf(fp, "%7d\t", num[j][i]); // number of each thread
        }
    }

    fprintf(fp, "\nSum\t%9d\t", total);     // Sum of all threads
    fprintf(fp, "%9d\t", th_total);         // Sum of each thread - by number

    for (i = 0; i < THR; i++)
        fprintf(fp, "%7d\t", t_total[i]);   // Sum of each thread

    fclose(fp); // Close file
}
