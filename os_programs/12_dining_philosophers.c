/*
 * Program 12: Dining Philosophers Problem
 * 5 philosophers sit at a table. Each needs 2 forks (left & right) to eat.
 * Uses mutexes to simulate forks and avoid deadlock using asymmetric solution.
 *
 * Compile: gcc 12_dining_philosophers.c -o 12_dining_philosophers -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5  /* Number of philosophers */

pthread_mutex_t forks[N];

typedef struct {
    int id;
} Philosopher;

void think(int id) {
    printf("Philosopher %d is THINKING.\n", id);
    sleep(rand() % 2 + 1);
}

void eat(int id) {
    printf("Philosopher %d is EATING.\n", id);
    sleep(rand() % 2 + 1);
}

void *philosopher(void *arg) {
    Philosopher *p = (Philosopher *)arg;
    int id    = p->id;
    int left  = id;
    int right = (id + 1) % N;

    for (int i = 0; i < 3; i++) {
        think(id);

        /* Asymmetric solution: even picks left then right; odd picks right then left */
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[left]);
            printf("Philosopher %d picked up LEFT fork %d.\n", id, left);
            pthread_mutex_lock(&forks[right]);
            printf("Philosopher %d picked up RIGHT fork %d.\n", id, right);
        } else {
            pthread_mutex_lock(&forks[right]);
            printf("Philosopher %d picked up RIGHT fork %d.\n", id, right);
            pthread_mutex_lock(&forks[left]);
            printf("Philosopher %d picked up LEFT fork %d.\n", id, left);
        }

        eat(id);

        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
        printf("Philosopher %d put down forks %d and %d.\n", id, left, right);
    }

    printf("Philosopher %d is DONE.\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[N];
    Philosopher philosophers[N];

    printf("=== Dining Philosophers Problem ===\n\n");

    for (int i = 0; i < N; i++)
        pthread_mutex_init(&forks[i], NULL);

    for (int i = 0; i < N; i++) {
        philosophers[i].id = i;
        pthread_create(&threads[i], NULL, philosopher, &philosophers[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    for (int i = 0; i < N; i++)
        pthread_mutex_destroy(&forks[i]);

    printf("\nAll philosophers have finished dining.\n");
    return 0;
}
