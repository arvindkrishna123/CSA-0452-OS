/*
 * Program 11: Multithreading using POSIX Threads (pthreads)
 * Demonstrates:
 *   - Creating multiple threads with pthread_create()
 *   - Passing arguments to threads
 *   - Joining threads with pthread_join()
 *   - Using a mutex for thread synchronization
 *
 * Compile: gcc 11_multithreading.c -o 11_multithreading -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS    5
#define ARRAY_SIZE     10

/* Global counter protected by a mutex */
int global_counter = 0;
pthread_mutex_t mutex;

/* Thread argument structure */
typedef struct {
    int thread_id;
    int start;
    int end;
    int numbers[ARRAY_SIZE];
    long sum;
} ThreadArg;

/* Thread function: computes sum of array portion */
void *compute_sum(void *arg) {
    ThreadArg *data = (ThreadArg *)arg;
    data->sum = 0;

    printf("[Thread %d] Starting. Computing sum from index %d to %d\n",
           data->thread_id, data->start, data->end - 1);

    for (int i = data->start; i < data->end; i++)
        data->sum += data->numbers[i];

    /* Safely increment global counter */
    pthread_mutex_lock(&mutex);
    global_counter++;
    printf("[Thread %d] Sum = %ld  | Threads completed so far: %d\n",
           data->thread_id, data->sum, global_counter);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

/* Demo thread: just prints a greeting */
void *greet_thread(void *arg) {
    int id = *((int *)arg);
    printf("[Greet Thread %d] Hello from thread! PID=%d\n", id, getpid());
    sleep(1);
    printf("[Greet Thread %d] Work done, exiting.\n", id);
    pthread_exit(NULL);
}

int main() {
    printf("=== Multithreading with POSIX Threads ===\n\n");

    /* --- Demo 1: Multiple greeting threads --- */
    printf("-- Demo 1: Creating %d greeting threads --\n", NUM_THREADS);
    pthread_t greet_tids[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        if (pthread_create(&greet_tids[i], NULL, greet_thread, &ids[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(greet_tids[i], NULL);

    printf("\nAll greeting threads completed.\n\n");

    /* --- Demo 2: Parallel sum using threads --- */
    printf("-- Demo 2: Parallel Array Sum using 2 Threads --\n");

    int numbers[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("Array: ");
    for (int i = 0; i < ARRAY_SIZE; i++) printf("%d ", numbers[i]);
    printf("\n");

    pthread_mutex_init(&mutex, NULL);

    pthread_t sum_tids[2];
    ThreadArg args[2];
    global_counter = 0;

    for (int i = 0; i < 2; i++) {
        args[i].thread_id = i + 1;
        args[i].start     = i * (ARRAY_SIZE / 2);
        args[i].end       = (i + 1) * (ARRAY_SIZE / 2);
        for (int j = 0; j < ARRAY_SIZE; j++) args[i].numbers[j] = numbers[j];

        pthread_create(&sum_tids[i], NULL, compute_sum, &args[i]);
    }

    long total = 0;
    for (int i = 0; i < 2; i++) {
        pthread_join(sum_tids[i], NULL);
        total += args[i].sum;
    }

    pthread_mutex_destroy(&mutex);

    printf("\nTotal sum (combined from both threads): %ld\n", total);
    printf("Expected sum (1+2+...+10)            : 55\n");

    return 0;
}
