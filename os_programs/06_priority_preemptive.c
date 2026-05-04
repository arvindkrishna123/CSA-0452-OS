/*
 * Program 6: CPU Scheduling - Preemptive Priority Scheduling
 * The running process can be preempted if a new higher-priority process arrives.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_TIME      200

typedef struct {
    int pid;
    int burst_time;
    int remaining_time;
    int arrival_time;
    int priority;          /* Lower number = Higher priority */
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_done;
    int start_time;
    int started;
} Process;

int gantt[MAX_TIME];

void preemptive_priority(Process proc[], int n) {
    int current_time = 0, completed = 0;
    int total_time = 0;

    for (int i = 0; i < n; i++) total_time += proc[i].burst_time;

    for (int i = 0; i < n; i++) proc[i].started = 0;

    while (completed < n) {
        int best_idx   = -1;
        int best_prio  = INT_MAX;

        /* Find highest priority available process */
        for (int i = 0; i < n; i++) {
            if (!proc[i].is_done &&
                proc[i].arrival_time <= current_time &&
                proc[i].priority < best_prio) {
                best_prio = proc[i].priority;
                best_idx  = i;
            }
        }

        if (best_idx == -1) {
            gantt[current_time] = -1; /* Idle */
            current_time++;
            continue;
        }

        if (!proc[best_idx].started) {
            proc[best_idx].start_time = current_time;
            proc[best_idx].started    = 1;
        }

        gantt[current_time] = proc[best_idx].pid;
        proc[best_idx].remaining_time--;
        current_time++;

        if (proc[best_idx].remaining_time == 0) {
            proc[best_idx].completion_time  = current_time;
            proc[best_idx].turnaround_time  = current_time - proc[best_idx].arrival_time;
            proc[best_idx].waiting_time     = proc[best_idx].turnaround_time - proc[best_idx].burst_time;
            proc[best_idx].is_done          = 1;
            completed++;
        }
    }
}

void display(Process proc[], int n, int total_time) {
    double avg_tat = 0, avg_wt = 0;

    printf("\n%-6s %-10s %-10s %-10s %-12s %-12s %-10s\n",
           "PID", "Burst", "Arrival", "Priority", "Completion", "Turnaround", "Waiting");
    printf("%s\n", "----------------------------------------------------------------------");

    for (int i = 0; i < n; i++) {
        printf("%-6d %-10d %-10d %-10d %-12d %-12d %-10d\n",
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

    printf("%s\n", "----------------------------------------------------------------------");
    printf("Average Turnaround Time : %.2f\n", avg_tat / n);
    printf("Average Waiting Time    : %.2f\n", avg_wt  / n);

    /* Print Gantt Chart (condensed) */
    printf("\nGantt Chart (condensed):\n|");
    int t = 0;
    while (t < total_time) {
        int pid = gantt[t];
        int start = t;
        while (t < total_time && gantt[t] == pid) t++;
        if (pid == -1)
            printf(" IDLE |");
        else
            printf("  P%d  |", pid);
    }
    printf("\n0");
    t = 0;
    while (t < total_time) {
        int pid = gantt[t];
        int start = t;
        while (t < total_time && gantt[t] == pid) t++;
        printf("      %d", t);
    }
    printf("\n");
}

int main() {
    Process proc[MAX_PROCESSES];
    int n, total_time = 0;

    printf("=== Preemptive Priority CPU Scheduling ===\n");
    printf("(Lower priority number = Higher priority)\n\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid         = i + 1;
        proc[i].is_done     = 0;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
        proc[i].remaining_time = proc[i].burst_time;
        printf("Enter priority for P%d (lower = higher priority): ", i + 1);
        scanf("%d", &proc[i].priority);
        total_time += proc[i].burst_time;
    }

    preemptive_priority(proc, n);
    display(proc, n, total_time);

    return 0;
}
