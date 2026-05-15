/*
 * Program 19: Process Synchronization using Mutex Locks
 *
 * Demonstrates the classic "race condition" problem and shows
 * how mutex locks fix it.
 *
 * Part A — WITHOUT mutex: multiple threads increment a shared counter
 *           unsafely → result is incorrect (race condition).
 * Part B — WITH    mutex: same operation protected by a mutex lock
 *           → result is always correct.
 *
 * Compile: gcc 19_mutex_synchronization.c -o 19_mutex_synchronization -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS     5
#define INCREMENTS      100000   /* Each thread increments this many times */

/* Shared counter (unprotected) */
long unsafe_counter = 0;

/* Shared counter (protected by mutex) */
long safe_counter = 0;
pthread_mutex_t counter_mutex;

/* ------ Thread function WITHOUT mutex ------ */
void *unsafe_increment(void *arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        /* Race condition: read-modify-write is NOT atomic */
        unsafe_counter++;
    }
    pthread_exit(NULL);
}

/* ------ Thread function WITH mutex ------ */
void *safe_increment(void *arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&counter_mutex);   /* LOCK   */
        safe_counter++;                        /* Critical Section */
        pthread_mutex_unlock(&counter_mutex); /* UNLOCK */
    }
    pthread_exit(NULL);
}

/* ------ Bank Account Simulation ------ */
typedef struct {
    double  balance;
    pthread_mutex_t lock;
    char    name[50];
} BankAccount;

BankAccount account;

typedef struct {
    int    thread_id;
    double amount;
    int    is_deposit; /* 1 = deposit, 0 = withdraw */
} Transaction;

void *bank_transaction(void *arg) {
    Transaction *t = (Transaction *)arg;

    pthread_mutex_lock(&account.lock);

    /* Critical Section */
    double old_balance = account.balance;
    sleep(0); /* Simulate a tiny delay */

    if (t->is_deposit) {
        account.balance += t->amount;
        printf("  [Thread %d] Deposited  Rs.%-8.2f | Balance: Rs.%.2f -> Rs.%.2f\n",
               t->thread_id, t->amount, old_balance, account.balance);
    } else {
        if (account.balance >= t->amount) {
            account.balance -= t->amount;
            printf("  [Thread %d] Withdrew   Rs.%-8.2f | Balance: Rs.%.2f -> Rs.%.2f\n",
                   t->thread_id, t->amount, old_balance, account.balance);
        } else {
            printf("  [Thread %d] Withdraw Rs.%-8.2f FAILED (Insufficient funds: Rs.%.2f)\n",
                   t->thread_id, t->amount, account.balance);
        }
    }

    pthread_mutex_unlock(&account.lock);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    long expected = (long)NUM_THREADS * INCREMENTS;

    printf("==============================================\n");
    printf("  Process Synchronization using Mutex Locks  \n");
    printf("==============================================\n\n");

    /* ---- PART A: Race Condition Demo ---- */
    printf("--- PART A: WITHOUT Mutex (Race Condition) ---\n");
    printf("Threads: %d  |  Increments each: %d  |  Expected: %ld\n",
           NUM_THREADS, INCREMENTS, expected);

    unsafe_counter = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, unsafe_increment, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    printf("Result (unsafe): %ld  <- MAY BE WRONG due to race condition!\n\n", unsafe_counter);

    /* ---- PART B: Mutex Protected ---- */
    printf("--- PART B: WITH Mutex (Synchronized) ---\n");
    printf("Threads: %d  |  Increments each: %d  |  Expected: %ld\n",
           NUM_THREADS, INCREMENTS, expected);

    safe_counter = 0;
    pthread_mutex_init(&counter_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, safe_increment, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    pthread_mutex_destroy(&counter_mutex);

    printf("Result (safe):   %ld  <- ALWAYS CORRECT with mutex!\n\n", safe_counter);
    printf("Difference: %ld\n\n", expected - safe_counter);

    /* ---- PART C: Bank Account Simulation ---- */
    printf("--- PART C: Bank Account Simulation (Mutex Protected) ---\n");
    account.balance = 1000.00;
    pthread_mutex_init(&account.lock, NULL);
    printf("Initial Balance: Rs.%.2f\n\n", account.balance);

    Transaction txns[6] = {
        {1, 500.00, 1},   /* deposit  */
        {2, 200.00, 0},   /* withdraw */
        {3, 1800.00, 0},  /* withdraw */
        {4, 300.00, 1},   /* deposit  */
        {5, 400.00, 0},   /* withdraw */
        {6, 100.00, 1}    /* deposit  */
    };

    pthread_t bank_threads[6];
    for (int i = 0; i < 6; i++)
        pthread_create(&bank_threads[i], NULL, bank_transaction, &txns[i]);
    for (int i = 0; i < 6; i++)
        pthread_join(bank_threads[i], NULL);

    pthread_mutex_destroy(&account.lock);
    printf("\nFinal Balance: Rs.%.2f\n", account.balance);

    return 0;
}
