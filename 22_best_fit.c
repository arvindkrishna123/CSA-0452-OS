/*
 * Program 18: Producer-Consumer Problem using Semaphores
 *
 * A producer thread generates items and places them in a bounded buffer.
 * A consumer thread removes items from the buffer.
 * Three semaphores coordinate access:
 *   - mutex   : mutual exclusion for buffer access (binary semaphore)
 *   - empty   : counts empty slots (initialized to BUFFER_SIZE)
 *   - full    : counts filled slots (initialized to 0)
 *
 * Compile: gcc 18_producer_consumer.c -o 18_producer_consumer -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE   5
#define PRODUCE_COUNT 10   /* Total items to produce */

int buffer[BUFFER_SIZE];   /* Shared bounded buffer */
int in  = 0;               /* Producer insert position */
int out = 0;               /* Consumer remove position */
int item_count = 0;        /* Items currently in buffer */

sem_t mutex;   /* Binary semaphore for mutual exclusion */
sem_t empty;   /* Counts empty slots                    */
sem_t full;    /* Counts filled slots                   */

/* Utility: print buffer state */
void print_buffer() {
    printf("  Buffer [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (/* slot occupied? we track via item_count indirectly */
            /* always show all slots, mark filled ones */
            1) {
            /* Print index with value or '-' */
            int occupied = 0;
            int pos = out;
            for (int k = 0; k < item_count; k++) {
                if (pos % BUFFER_SIZE == i) { occupied = 1; break; }
                pos++;
            }
            if (occupied)
                printf("%3d", buffer[i]);
            else
                printf("  _");
        }
    }
    printf(" ]  (items=%d)\n", item_count);
}

/* Producer thread */
void *producer(void *arg) {
    for (int i = 1; i <= PRODUCE_COUNT; i++) {
        int item = i * 10;   /* Produce item */

        sem_wait(&empty);    /* Wait for an empty slot    */
        sem_wait(&mutex);    /* Enter critical section    */

        /* --- Critical Section --- */
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        item_count++;
        printf("[Producer] Produced item: %3d  | ", item);
        print_buffer();
        /* --- End Critical Section --- */

        sem_post(&mutex);    /* Leave critical section    */
        sem_post(&full);     /* Signal a filled slot      */

        sleep(1);            /* Simulate production time  */
    }
    printf("[Producer] Done producing.\n");
    pthread_exit(NULL);
}

/* Consumer thread */
void *consumer(void *arg) {
    for (int i = 1; i <= PRODUCE_COUNT; i++) {
        sem_wait(&full);     /* Wait for a filled slot    */
        sem_wait(&mutex);    /* Enter critical section    */

        /* --- Critical Section --- */
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        item_count--;
        printf("[Consumer] Consumed item: %3d  | ", item);
        print_buffer();
        /* --- End Critical Section --- */

        sem_post(&mutex);    /* Leave critical section    */
        sem_post(&empty);    /* Signal an empty slot      */

        sleep(2);            /* Simulate consumption time */
    }
    printf("[Consumer] Done consuming.\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_tid, cons_tid;

    printf("============================================\n");
    printf("  Producer-Consumer Problem using Semaphores\n");
    printf("  Buffer Size: %d  |  Items to produce: %d\n", BUFFER_SIZE, PRODUCE_COUNT);
    printf("============================================\n\n");

    /* Initialize semaphores */
    sem_init(&mutex, 0, 1);           /* Binary semaphore       */
    sem_init(&empty, 0, BUFFER_SIZE); /* All slots empty at start */
    sem_init(&full,  0, 0);           /* No items at start      */

    /* Create producer and consumer threads */
    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid, NULL, consumer, NULL);

    /* Wait for both to finish */
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    /* Cleanup */
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("\nAll items produced and consumed successfully.\n");
    return 0;
}
