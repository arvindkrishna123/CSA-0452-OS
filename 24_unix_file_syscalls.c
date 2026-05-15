/*
 * Program 23: First Fit Algorithm - Memory Management
 *
 * FIRST FIT: Scan memory blocks from the beginning and allocate the process
 * to the FIRST block that is large enough. Fast and simple.
 *
 * Compile: gcc 23_first_fit.c -o 23_first_fit
 */

#include <stdio.h>
#include <string.h>

#define MAX_BLOCKS    10
#define MAX_PROCESSES 10

/* Returns total internal fragmentation */
int first_fit(int block_size[], int m, int proc_size[], int n,
              int allocation[], int remaining[]) {

    for (int i = 0; i < n; i++) allocation[i] = -1;
    for (int j = 0; j < m; j++) remaining[j]  = block_size[j];

    int total_frag = 0;

    for (int i = 0; i < n; i++) {
        /* Scan blocks from beginning — take the FIRST that fits */
        for (int j = 0; j < m; j++) {
            if (remaining[j] >= proc_size[i]) {
                allocation[i]  = j;
                total_frag    += (remaining[j] - proc_size[i]);
                remaining[j]  -= proc_size[i];
                break;
            }
        }
    }
    return total_frag;
}

void display_results(int block_size[], int m, int proc_size[], int n,
                     int allocation[], int remaining[]) {
    printf("\n============================================\n");
    printf("         FIRST FIT - Allocation Result     \n");
    printf("============================================\n");
    printf("%-12s %-16s %-12s %-15s %-12s\n",
           "Process", "Proc Size(KB)", "Block No.",
           "Block Size(KB)", "Fragment(KB)");
    printf("----------------------------------------------------\n");

    int allocated_count = 0;
    for (int i = 0; i < n; i++) {
        if (allocation[i] != -1) {
            int frag = block_size[allocation[i]] - proc_size[i];
            printf("P%-11d %-16d %-12d %-15d %-12d\n",
                   i + 1, proc_size[i],
                   allocation[i] + 1,
                   block_size[allocation[i]],
                   frag);
            allocated_count++;
        } else {
            printf("P%-11d %-16d %-12s %-15s %-12s\n",
                   i + 1, proc_size[i],
                   "Not Alloc.", "---", "---");
        }
    }

    printf("----------------------------------------------------\n");
    printf("Processes allocated     : %d / %d\n", allocated_count, n);
    printf("Processes not allocated : %d / %d\n", n - allocated_count, n);

    /* Calculate total fragmentation */
    int total_used = 0, total_frag = 0, total_avail = 0;
    for (int j = 0; j < m; j++) {
        total_used  += (block_size[j] - remaining[j]);
        total_frag  += remaining[j];
        total_avail += block_size[j];
    }

    printf("\n--- Memory Summary ---\n");
    printf("Total memory           : %d KB\n", total_avail);
    printf("Memory used            : %d KB\n", total_used);
    printf("External fragmentation : %d KB (unused portions of blocks)\n", total_frag);

    printf("\n--- Block Status After Allocation ---\n");
    printf("%-10s %-15s %-12s %-15s %-10s\n",
           "Block", "Original(KB)", "Used(KB)", "Remaining(KB)", "Status");
    printf("----------------------------------------------------\n");
    for (int j = 0; j < m; j++) {
        int used = block_size[j] - remaining[j];
        printf("%-10d %-15d %-12d %-15d %-10s\n",
               j + 1, block_size[j], used, remaining[j],
               used == 0 ? "Free" : (remaining[j] == 0 ? "Full" : "Partial"));
    }
}

/* Visualize memory layout */
void visualize_memory(int block_size[], int m, int proc_size[], int n,
                      int allocation[]) {
    printf("\n--- Memory Block Visualization ---\n");
    for (int j = 0; j < m; j++) {
        printf("Block %d [%3d KB]: |", j + 1, block_size[j]);
        int printed = 0;
        for (int i = 0; i < n; i++) {
            if (allocation[i] == j) {
                printf(" P%d(%dKB) |", i + 1, proc_size[i]);
                printed += proc_size[i];
            }
        }
        int leftover = block_size[j] - printed;
        if (leftover > 0)
            printf(" Free(%dKB) |", leftover);
        printf("\n");
    }
}

int main() {
    int block_size[MAX_BLOCKS], proc_size[MAX_PROCESSES];
    int allocation[MAX_PROCESSES], remaining[MAX_BLOCKS];
    int m, n;

    printf("============================================\n");
    printf("    First Fit Memory Allocation Algorithm   \n");
    printf("============================================\n\n");

    printf("Enter number of memory blocks (max %d): ", MAX_BLOCKS);
    scanf("%d", &m);
    printf("Enter size of each block (in KB):\n");
    for (int i = 0; i < m; i++) {
        printf("  Block %d: ", i + 1);
        scanf("%d", &block_size[i]);
    }

    printf("\nEnter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);
    printf("Enter memory required by each process (in KB):\n");
    for (int i = 0; i < n; i++) {
        printf("  Process %d: ", i + 1);
        scanf("%d", &proc_size[i]);
    }

    printf("\nMemory Blocks : ");
    for (int i = 0; i < m; i++) printf("%d ", block_size[i]);
    printf("(KB)");
    printf("\nProcess Sizes : ");
    for (int i = 0; i < n; i++) printf("%d ", proc_size[i]);
    printf("(KB)\n");

    first_fit(block_size, m, proc_size, n, allocation, remaining);
    display_results(block_size, m, proc_size, n, allocation, remaining);
    visualize_memory(block_size, m, proc_size, n, allocation);

    return 0;
}
