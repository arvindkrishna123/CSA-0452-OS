/*
 * Program 7: CPU Scheduling - Non-Preemptive Shortest Job First (SJF)
 * Once a process starts, it runs to completion (no preemption).
 * Among all ready processes, the one with the smallest burst time is chosen.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 10

typedef struct {
    int pid;
    int burst_time;
    int arrival_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int is_done;
} Process;

void sjf_nonpreemptive(Process proc[], int n) {
    int completed = 0, current_time = 0;

    while (completed < n) {
        int min_burst = INT_MAX, selected = -1;

        /* Select process with minimum burst among arrived & not done */
        for (int i = 0; i < n; i++) {
            if (!proc[i].is_done &&
                proc[i].arrival_time <= current_time) {
                if (proc[i].burst_time < min_burst ||
                   (proc[i].burst_time == min_burst && proc[i].arrival_time < proc[selected].arrival_time)) {
                    min_burst = proc[i].burst_time;
                    selected  = i;
                }
            }
        }

        if (selected == -1) {
            /* No process ready — advance time to next arrival */
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++)
                if (!proc[i].is_done && proc[i].arrival_time < next_arrival)
                    next_arrival = proc[i].arrival_time;
            current_time = next_arrival;
            continue;
        }

        /* Execute chosen process fully */
        current_time += proc[selected].burst_time;
        proc[selected].completion_time  = current_time;
        proc[selected].turnaround_time  = current_time - proc[selected].arrival_time;
        proc[selected].waiting_time     = proc[selected].turnaround_time - proc[selected].burst_time;
        proc[selected].is_done          = 1;
        completed++;
    }
}

void display(Process proc[], int n) {
    double avg_tat = 0, avg_wt = 0;

    /* Sort by completion order for display */
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

    printf("=== Non-Preemptive Shortest Job First (SJF) Scheduling ===\n\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid     = i + 1;
        proc[i].is_done = 0;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
    }

    sjf_nonpreemptive(proc, n);
    display(proc, n);

    return 0;
}
