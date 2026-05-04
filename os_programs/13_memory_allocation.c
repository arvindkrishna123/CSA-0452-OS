/*
 * Program 13: Memory Allocation Strategies
 * Simulates First Fit, Best Fit, and Worst Fit memory allocation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS    10
#define MAX_PROCESSES 10

void first_fit(int blocks[], int m, int processes[], int n) {
    int allocation[MAX_PROCESSES];
    memset(allocation, -1, sizeof(allocation));
    int block_used[MAX_BLOCKS] = {0};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!block_used[j] && blocks[j] >= processes[i]) {
                allocation[i] = j;
                block_used[j] = 1;
                break;
            }
        }
    }

    printf("\n--- First Fit ---\n");
    printf("%-12s %-15s %-12s\n", "Process", "Process Size", "Block No.");
    for (int i = 0; i < n; i++) {
        if (allocation[i] != -1)
            printf("%-12d %-15d %-12d\n", i+1, processes[i], allocation[i]+1);
        else
            printf("%-12d %-15d %-12s\n", i+1, processes[i], "Not Allocated");
    }
}

void best_fit(int blocks[], int m, int processes[], int n) {
    int allocation[MAX_PROCESSES];
    memset(allocation, -1, sizeof(allocation));

    for (int i = 0; i < n; i++) {
        int best_idx = -1;
        for (int j = 0; j < m; j++) {
            if (blocks[j] >= processes[i]) {
                if (best_idx == -1 || blocks[j] < blocks[best_idx])
                    best_idx = j;
            }
        }
        if (best_idx != -1) {
            allocation[i]       = best_idx;
            blocks[best_idx]   -= processes[i];
        }
    }

    printf("\n--- Best Fit ---\n");
    printf("%-12s %-15s %-12s\n", "Process", "Process Size", "Block No.");
    for (int i = 0; i < n; i++) {
        if (allocation[i] != -1)
            printf("%-12d %-15d %-12d\n", i+1, processes[i], allocation[i]+1);
        else
            printf("%-12d %-15d %-12s\n", i+1, processes[i], "Not Allocated");
    }
}

void worst_fit(int blocks[], int m, int processes[], int n) {
    int allocation[MAX_PROCESSES];
    memset(allocation, -1, sizeof(allocation));

    for (int i = 0; i < n; i++) {
        int worst_idx = -1;
        for (int j = 0; j < m; j++) {
            if (blocks[j] >= processes[i]) {
                if (worst_idx == -1 || blocks[j] > blocks[worst_idx])
                    worst_idx = j;
            }
        }
        if (worst_idx != -1) {
            allocation[i]        = worst_idx;
            blocks[worst_idx]   -= processes[i];
        }
    }

    printf("\n--- Worst Fit ---\n");
    printf("%-12s %-15s %-12s\n", "Process", "Process Size", "Block No.");
    for (int i = 0; i < n; i++) {
        if (allocation[i] != -1)
            printf("%-12d %-15d %-12d\n", i+1, processes[i], allocation[i]+1);
        else
            printf("%-12d %-15d %-12s\n", i+1, processes[i], "Not Allocated");
    }
}

int main() {
    int original_blocks[MAX_BLOCKS], blocks[MAX_BLOCKS];
    int processes[MAX_PROCESSES];
    int m, n;

    printf("=== Memory Allocation Strategies ===\n\n");

    printf("Enter number of memory blocks (max %d): ", MAX_BLOCKS);
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        printf("Enter size of block %d: ", i+1);
        scanf("%d", &original_blocks[i]);
    }

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Enter size of process %d: ", i+1);
        scanf("%d", &processes[i]);
    }

    printf("\nMemory Blocks: ");
    for (int i = 0; i < m; i++) printf("%d ", original_blocks[i]);
    printf("\nProcesses    : ");
    for (int i = 0; i < n; i++) printf("%d ", processes[i]);
    printf("\n");

    /* First Fit — uses block once per process */
    memcpy(blocks, original_blocks, sizeof(int) * m);
    first_fit(blocks, m, processes, n);

    /* Best Fit */
    memcpy(blocks, original_blocks, sizeof(int) * m);
    best_fit(blocks, m, processes, n);

    /* Worst Fit */
    memcpy(blocks, original_blocks, sizeof(int) * m);
    worst_fit(blocks, m, processes, n);

    return 0;
}
