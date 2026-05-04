/*
 * Program 5: CPU Scheduling - Non-Preemptive Priority Scheduling
 * Selects the waiting process with the highest priority (lowest number = highest priority).
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 10

typedef struct {
    int pid;
    int burst_time;
    int arrival_time;
    int priority;       /* Lower number = Higher priority */
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_done;
} Process;

void priority_schedule(Process proc[], int n) {
    int completed = 0, current_time = 0, best_idx;
    int best_priority;

    while (completed < n) {
        best_priority = INT_MAX;
        best_idx      = -1;

        /* Find highest priority (lowest number) among arrived & not done */
        for (int i = 0; i < n; i++) {
            if (!proc[i].is_done &&
                proc[i].arrival_time <= current_time &&
                proc[i].priority < best_priority) {
                best_priority = proc[i].priority;
                best_idx = i;
            }
        }

        if (best_idx == -1) {
            current_time++; /* CPU idle */
            continue;
        }

        current_time += proc[best_idx].burst_time;
        proc[best_idx].completion_time  = current_time;
        proc[best_idx].turnaround_time  = current_time - proc[best_idx].arrival_time;
        proc[best_idx].waiting_time     = proc[best_idx].turnaround_time - proc[best_idx].burst_time;
        proc[best_idx].is_done          = 1;
        completed++;
    }
}

void display(Process proc[], int n) {
    double avg_tat = 0, avg_wt = 0;

    printf("\n%-6s %-12s %-12s %-10s %-16s %-16s %-12s\n",
           "PID", "Burst Time", "Arrival", "Priority", "Completion", "Turnaround", "Waiting");
    printf("%s\n", "-----------------------------------------------------------------------------");

    for (int i = 0; i < n; i++) {
        printf("%-6d %-12d %-12d %-10d %-16d %-16d %-12d\n",
               proc[i].pid,
               proc[i].burst_time,
               proc[i].arrival_time,
               proc[i].priority,
               proc[i].completion_time,
               proc[i].turnaround_time,
               proc[i].waiting_time);
        avg_tat += proc[i].turnaround_time;
        avg_wt  += proc[i].waiting_time;
    }

    printf("%s\n", "-----------------------------------------------------------------------------");
    printf("Average Turnaround Time : %.2f\n", avg_tat / n);
    printf("Average Waiting Time    : %.2f\n", avg_wt  / n);
}

int main() {
    Process proc[MAX_PROCESSES];
    int n;

    printf("=== Non-Preemptive Priority CPU Scheduling ===\n");
    printf("(Lower priority number = Higher priority)\n\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid     = i + 1;
        proc[i].is_done = 0;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
        printf("Enter priority for P%d (lower = higher priority): ", i + 1);
        scanf("%d", &proc[i].priority);
    }

    priority_schedule(proc, n);
    display(proc, n);

    return 0;
}
