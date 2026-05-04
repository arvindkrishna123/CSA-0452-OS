/*
 * Program 4: CPU Scheduling - Shortest Job First (SJF) - Non-Preemptive
 * Selects the waiting process with the smallest execution time to execute next.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 10

typedef struct {
    int pid;
    int burst_time;
    int remaining_time;
    int arrival_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_done;
} Process;

void sjf(Process proc[], int n) {
    int completed = 0, current_time = 0, min_idx;
    int min_burst;

    while (completed < n) {
        min_burst = INT_MAX;
        min_idx = -1;

        /* Find the process with shortest burst time among arrived & not done */
        for (int i = 0; i < n; i++) {
            if (!proc[i].is_done &&
                proc[i].arrival_time <= current_time &&
                proc[i].burst_time < min_burst) {
                min_burst = proc[i].burst_time;
                min_idx = i;
            }
        }

        if (min_idx == -1) {
            current_time++; /* CPU idle */
            continue;
        }

        /* Execute the chosen process to completion */
        current_time += proc[min_idx].burst_time;
        proc[min_idx].completion_time  = current_time;
        proc[min_idx].turnaround_time  = current_time - proc[min_idx].arrival_time;
        proc[min_idx].waiting_time     = proc[min_idx].turnaround_time - proc[min_idx].burst_time;
        proc[min_idx].is_done          = 1;
        completed++;
    }
}

void display(Process proc[], int n) {
    double avg_tat = 0, avg_wt = 0;

    printf("\n%-6s %-12s %-12s %-16s %-16s %-12s\n",
           "PID", "Burst Time", "Arrival", "Completion", "Turnaround", "Waiting");
    printf("%s\n", "----------------------------------------------------------------------");

    for (int i = 0; i < n; i++) {
        printf("%-6d %-12d %-12d %-16d %-16d %-12d\n",
               proc[i].pid,
               proc[i].burst_time,
               proc[i].arrival_time,
               proc[i].completion_time,
               proc[i].turnaround_time,
               proc[i].waiting_time);
        avg_tat += proc[i].turnaround_time;
        avg_wt  += proc[i].waiting_time;
    }

    printf("%s\n", "----------------------------------------------------------------------");
    printf("Average Turnaround Time : %.2f\n", avg_tat / n);
    printf("Average Waiting Time    : %.2f\n", avg_wt  / n);
}

int main() {
    Process proc[MAX_PROCESSES];
    int n;

    printf("=== Shortest Job First (SJF) - Non-Preemptive CPU Scheduling ===\n\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid      = i + 1;
        proc[i].is_done  = 0;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
    }

    sjf(proc, n);
    display(proc, n);

    return 0;
}
