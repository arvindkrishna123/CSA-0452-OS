/*
 * Program 20: Reader-Writer Problem using Semaphores
 *
 * Rules:
 *   - Multiple readers can read simultaneously (no mutual exclusion among readers).
 *   - A writer needs exclusive access — no reader or writer can access at the same time.
 *   - Uses FIRST READERS-WRITERS solution (readers have priority).
 *
 * Semaphores used:
 *   mutex    : protects the reader_count variable
 *   rw_mutex : grants exclusive access to writers (and first/last reader)
 *
 * Compile: gcc 20_reader_writer.c -o 20_reader_writer -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS  4
#define NUM_WRITERS  2
#define ITERATIONS   3

/* Shared data */
int shared_data = 0;

/* Synchronization variables */
sem_t  mutex;        /* Protects reader_count                */
sem_t  rw_mutex;     /* Exclusive access for writers         */
int    reader_count = 0; /* Number of active readers         */

/* ---- Reader Thread ---- */
void *reader(void *arg) {
    int id = *((int *)arg);

    for (int iter = 0; iter < ITERATIONS; iter++) {
        /* Entry Section */
        sem_wait(&mutex);
        reader_count++;
        if (reader_count == 1)
            sem_wait(&rw_mutex);   /* First reader blocks writers */
        sem_post(&mutex);

        /* ---- Reading (Critical Section) ---- */
        printf("  [Reader %d] Reading shared_data = %d  (active readers: %d)\n",
               id, shared_data, reader_count);
        sleep(1); /* Simulate read time */
        /* ---- End Read ---- */

        /* Exit Section */
        sem_wait(&mutex);
        reader_count--;
        if (reader_count == 0)
            sem_post(&rw_mutex);   /* Last reader unblocks writers */
        sem_post(&mutex);

        sleep(1); /* Think time between reads */
    }

    printf("  [Reader %d] Done.\n", id);
    pthread_exit(NULL);
}

/* ---- Writer Thread ---- */
void *writer(void *arg) {
    int id = *((int *)arg);

    for (int iter = 0; iter < ITERATIONS; iter++) {
        /* Entry Section */
        sem_wait(&rw_mutex);   /* Exclusive access */

        /* ---- Writing (Critical Section) ---- */
        shared_data += (id * 10);
        printf("  [Writer %d] Wrote shared_data = %d\n", id, shared_data);
        sleep(1); /* Simulate write time */
        /* ---- End Write ---- */

        /* Exit Section */
        sem_post(&rw_mutex);

        sleep(2); /* Think time between writes */
    }

    printf("  [Writer %d] Done.\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t reader_threads[NUM_READERS];
    pthread_t writer_threads[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];

    printf("==============================================\n");
    printf("  Reader-Writer Problem using Semaphores     \n");
    printf("  Readers: %d  |  Writers: %d  |  Iterations: %d\n",
           NUM_READERS, NUM_WRITERS, ITERATIONS);
    printf("  Initial shared_data = %d\n", shared_data);
    printf("==============================================\n\n");

    /* Initialize semaphores */
    sem_init(&mutex,    0, 1);  /* Binary semaphore for reader_count */
    sem_init(&rw_mutex, 0, 1);  /* Binary semaphore for write exclusion */

    /* Create writer threads first (they wait while readers hold rw_mutex) */
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writer_threads[i], NULL, writer, &writer_ids[i]);
    }

    /* Create reader threads */
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]);
    }

    /* Join all threads */
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writer_threads[i], NULL);
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(reader_threads[i], NULL);

    /* Cleanup */
    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);

    printf("\nFinal shared_data = %d\n", shared_data);
    printf("Reader-Writer simulation complete.\n");
    return 0;
}
