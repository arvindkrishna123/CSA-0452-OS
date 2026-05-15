/*
 * Program 17: Banker's Algorithm for Deadlock Avoidance
 *
 * The Banker's Algorithm checks whether granting a resource request
 * will leave the system in a SAFE STATE. A safe state is one where
 * there exists at least one safe sequence of process execution that
 * allows all processes to complete.
 *
 * Compile: gcc 17_bankers_algorithm.c -o 17_bankers_algorithm
 */

#include <stdio.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int n, m; /* n = number of processes, m = number of resource types */

int allocation[MAX_PROCESSES][MAX_RESOURCES]; /* Currently allocated */
int maximum[MAX_PROCESSES][MAX_RESOURCES];    /* Max demand          */
int need[MAX_PROCESSES][MAX_RESOURCES];       /* Remaining need      */
int available[MAX_RESOURCES];                 /* Available resources */

/* Calculate need matrix: Need = Maximum - Allocation */
void calculate_need() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
}

/* Safety Algorithm: returns 1 if safe, fills safe_seq[] */
int safety_algorithm(int safe_seq[]) {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES];
    memset(finish, 0, sizeof(finish));

    for (int j = 0; j < m; j++)
        work[j] = available[j];

    int count = 0;
    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                /* Check if Need[i] <= Work */
                int possible = 1;
                for (int j = 0; j < m; j++)
                    if (need[i][j] > work[j]) { possible = 0; break; }

                if (possible) {
                    /* Process i can finish; release its resources */
                    for (int j = 0; j < m; j++)
                        work[j] += allocation[i][j];
                    safe_seq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) return 0; /* No progress → unsafe */
    }
    return 1; /* All processes finished → safe */
}

/* Resource Request Algorithm */
void request_resources(int pid, int request[]) {
    printf("\n========== Resource Request ==========\n");
    printf("Process P%d requests: ", pid);
    for (int j = 0; j < m; j++) printf("%d ", request[j]);
    printf("\n");

    /* Step 1: Request <= Need? */
    for (int j = 0; j < m; j++) {
        if (request[j] > need[pid][j]) {
            printf("ERROR: Request exceeds maximum claim! Request denied.\n");
            return;
        }
    }

    /* Step 2: Request <= Available? */
    for (int j = 0; j < m; j++) {
        if (request[j] > available[j]) {
            printf("Resources not available right now. P%d must wait.\n", pid);
            return;
        }
    }

    /* Step 3: Pretend to allocate and check safety */
    for (int j = 0; j < m; j++) {
        available[j]         -= request[j];
        allocation[pid][j]   += request[j];
        need[pid][j]         -= request[j];
    }

    int safe_seq[MAX_PROCESSES];
    if (safety_algorithm(safe_seq)) {
        printf("Request GRANTED. System remains in SAFE STATE.\n");
        printf("New Safe Sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d%s", safe_seq[i], i < n-1 ? " -> " : "\n");
    } else {
        /* Rollback */
        printf("Request DENIED. Granting would lead to UNSAFE STATE. Rolling back.\n");
        for (int j = 0; j < m; j++) {
            available[j]         += request[j];
            allocation[pid][j]   -= request[j];
            need[pid][j]         += request[j];
        }
    }
}

void display_state() {
    printf("\n---------- Current System State ----------\n");
    printf("%-6s", "Proc");
    printf("  Allocation");
    for (int j = 1; j < m; j++) printf("  ");
    printf("  Maximum  ");
    for (int j = 1; j < m; j++) printf("  ");
    printf("  Need\n");

    for (int i = 0; i < n; i++) {
        printf("P%-5d", i);
        for (int j = 0; j < m; j++) printf("%-3d", allocation[i][j]);
        printf("     ");
        for (int j = 0; j < m; j++) printf("%-3d", maximum[i][j]);
        printf("     ");
        for (int j = 0; j < m; j++) printf("%-3d", need[i][j]);
        printf("\n");
    }

    printf("\nAvailable: ");
    for (int j = 0; j < m; j++) printf("%d ", available[j]);
    printf("\n");
    printf("------------------------------------------\n");
}

int main() {
    printf("========================================\n");
    printf("   Banker's Algorithm - Deadlock Avoidance\n");
    printf("========================================\n\n");

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("  P%d: ", i);
        for (int j = 0; j < m; j++)
            scanf("%d", &allocation[i][j]);
    }

    printf("\nEnter Maximum Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++) {
        printf("  P%d: ", i);
        for (int j = 0; j < m; j++)
            scanf("%d", &maximum[i][j]);
    }

    printf("\nEnter Available Resources (%d values): ", m);
    for (int j = 0; j < m; j++)
        scanf("%d", &available[j]);

    calculate_need();
    display_state();

    /* Check initial safety */
    int safe_seq[MAX_PROCESSES];
    printf("\n--- Safety Check ---\n");
    if (safety_algorithm(safe_seq)) {
        printf("System is in a SAFE STATE.\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d%s", safe_seq[i], i < n-1 ? " -> " : "\n");
    } else {
        printf("System is in an UNSAFE STATE (Deadlock possible).\n");
    }

    /* Resource request simulation */
    int choice;
    printf("\nDo you want to simulate a resource request? (1=Yes / 0=No): ");
    scanf("%d", &choice);

    while (choice) {
        int pid, request[MAX_RESOURCES];
        printf("Enter process number (0 to %d): ", n-1);
        scanf("%d", &pid);
        printf("Enter request vector (%d values): ", m);
        for (int j = 0; j < m; j++)
            scanf("%d", &request[j]);

        request_resources(pid, request);
        display_state();

        printf("\nAnother request? (1=Yes / 0=No): ");
        scanf("%d", &choice);
    }

    printf("\nProgram ended.\n");
    return 0;
}
