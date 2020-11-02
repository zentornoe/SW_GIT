#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RUN 100000 // random : 100,000
#define MIN 0      // random min val
#define MAX 99     // random max val
#define NUM 100    // size of array
#define THR 10     // # of thread

// Thread input data structure
typedef struct
{
    int number[100]; // Shared variable
    int num[100];    // Thread variable
    int count;       // For Count 100,000 times
} th_data;

// Thread function
void *t_function(void *data)
{
    th_data *t_data = data;
    while (t_data->count < RUN)
    {
        srand(time(NULL));
        int n = rand() % NUM;  // seed = time, random number n : 0 ~ 99
        (t_data->number[n])++; // counting shared numbers
        (t_data->num[n])++;    // counting thread's numbers
    }
}

int main()
{
    int i = 0, j = 0; // Loop val
    int number[NUM] = {
        0,
    }; // Shared val
    int num[THR][NUM] = {
        0,
    }; // Val of each thread
    int count[THR] = {
        0,
    };                  // counting 100,000
    int thr_com[THR];   // Thread creation status
    th_data *data[THR]; // Input data

    pthread_t tid[THR]; // Threads

    // Thread data structure
    for (i = 0; i < THR; i++)
    {
        data[i] = (th_data *)malloc(sizeof(th_data));
        (*data[i])->number = number;
        (*data[i])->num = num[i];
        data[i]->count = count[i];
    }

    // Thread creation
    for (i = 0; i < THR; i++)
    { // Create Threads
        thr_com[i] = pthread_create(&tid[i], NULL, t_function, (void *)data[i]);

        if (thr_com[i] < 0)
        { // Thread creation error
            perror("*** Thread create error!!! ***");
            printf("\nThread%d\n\n", i);
            exit(0);
        }
    }

    // Wait for threads to end
    for (i = 0; i < THR; i++)
    { // Return Threads
        pthread_join(tid[i], NULL);
    }

    // Total
    int total = 0; // Ideal : 1,000,000
    int t_total[THR] = {
        0,
    }; // Each thread - Ideal : 100,000
    for (i = 0; i < NUM; i++)
    {
        total += number[i];
        for (j = 0; j < THR; j++)
        {
            t_total[j] += num[j][i];
        }
    }

    // File
    FILE *fp;                      // File pointer
    fp = fopen("result.dat", "w"); // Mode : writing(+create), File name : result.dat

    fprintf(fp, "Result\tTotal\t\t");
    for (i = 0; i < THR; i++)
        fprintf(fp, "Thread%d\t", i); // Result header

    for (i = 0; i < NUM; i++)
    {                                      // Write each number data
        fprintf(fp, "\n%d\t", i);          // Number
        fprintf(fp, "%7d\t\t", number[i]); // Sum of all threads in each number

        for (j = 0; j < THR; j++)
        {
            fprintf(fp, "%7d\t", num[j][i]); // number of each thread
        }
    }

    fprintf(fp, "\nSum\t%9d\t", total); // Sum of all threads
    for (i = 0; i < THR; i++)
        fprintf(fp, "%7d\t", t_total[i]); // Sum of each thread

    fclose(fp); // Close file

    for (i = 0; i < THR; i++)
    {
        free(data[i]);
    }
}
