/*
 * Program 3: CPU Scheduling - First Come First Served (FCFS)
 * All processes arrive at time 0. No I/O wait.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10

typedef struct {
    int pid;
    int burst_time;
    int arrival_time;   /* Always 0 as per problem */
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void fcfs(Process proc[], int n) {
    int current_time = 0;

    /* Sort by arrival time (all are 0 here, so order is preserved) */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].arrival_time > proc[j+1].arrival_time) {
                Process tmp = proc[j];
                proc[j] = proc[j+1];
                proc[j+1] = tmp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (current_time < proc[i].arrival_time)
            current_time = proc[i].arrival_time;

        proc[i].completion_time  = current_time + proc[i].burst_time;
        proc[i].turnaround_time  = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time     = proc[i].turnaround_time - proc[i].burst_time;
        current_time             = proc[i].completion_time;
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

void draw_gantt(Process proc[], int n) {
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < n; i++)
        printf(" P%d |", proc[i].pid);
    printf("\n0");
    int t = 0;
    for (int i = 0; i < n; i++) {
        t += proc[i].burst_time;
        printf("    %d", t);
    }
    printf("\n");
}

int main() {
    Process proc[MAX_PROCESSES];
    int n;

    printf("=== First Come First Served (FCFS) CPU Scheduling ===\n");
    printf("(All processes arrive at time 0)\n\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        proc[i].pid          = i + 1;
        proc[i].arrival_time = 0;   /* All arrive at time 0 */
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
    }

    fcfs(proc, n);
    display(proc, n);
    draw_gantt(proc, n);

    return 0;
}
