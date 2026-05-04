/*
 * Program 8: CPU Scheduling - Round Robin (RR)
 * Each process gets a fixed time quantum. If a process doesn't finish
 * within its quantum, it's preempted and placed at the back of the ready queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_GANTT     500

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

int gantt_pid[MAX_GANTT];
int gantt_time[MAX_GANTT];
int gantt_count = 0;

/* Simple queue */
int queue[MAX_PROCESSES * 100];
int front = 0, rear = 0;

void enqueue(int pid) { queue[rear++] = pid; }
int  dequeue()        { return queue[front++]; }
int  is_empty()       { return front == rear; }

void round_robin(Process proc[], int n, int quantum) {
    int current_time = 0, completed = 0;
    int in_queue[MAX_PROCESSES];
    memset(in_queue, 0, sizeof(in_queue));

    /* Enqueue all processes that arrive at time 0 */
    for (int i = 0; i < n; i++) {
        if (proc[i].arrival_time == 0) {
            enqueue(i);
            in_queue[i] = 1;
        }
    }

    while (completed < n) {
        if (is_empty()) {
            /* CPU idle — advance to next arrival */
            int next = -1, min_arr = 1e9;
            for (int i = 0; i < n; i++)
                if (!proc[i].is_done && !in_queue[i] && proc[i].arrival_time < min_arr) {
                    min_arr = proc[i].arrival_time;
                    next    = i;
                }
            if (next == -1) break;
            current_time = min_arr;
            enqueue(next);
            in_queue[next] = 1;
        }

        int idx = dequeue();
        in_queue[idx] = 0;

        int exec_time = (proc[idx].remaining_time < quantum) ? proc[idx].remaining_time : quantum;

        gantt_pid[gantt_count]    = proc[idx].pid;
        gantt_time[gantt_count++] = current_time;

        /* Check for new arrivals during execution */
        for (int t = current_time + 1; t <= current_time + exec_time; t++) {
            for (int i = 0; i < n; i++) {
                if (!proc[i].is_done && !in_queue[i] && proc[i].arrival_time == t && i != idx) {
                    enqueue(i);
                    in_queue[i] = 1;
                }
            }
        }

        current_time            += exec_time;
        proc[idx].remaining_time -= exec_time;

        if (proc[idx].remaining_time == 0) {
            proc[idx].completion_time  = current_time;
            proc[idx].turnaround_time  = current_time - proc[idx].arrival_time;
            proc[idx].waiting_time     = proc[idx].turnaround_time - proc[idx].burst_time;
            proc[idx].is_done          = 1;
            completed++;
        } else {
            /* Re-add newly arrived processes first, then this process */
            enqueue(idx);
            in_queue[idx] = 1;
        }
    }

    gantt_time[gantt_count] = current_time; /* End time */
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

    /* Gantt Chart */
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < gantt_count; i++)
        printf(" P%d |", gantt_pid[i]);
    printf("\n");
    for (int i = 0; i <= gantt_count; i++)
        printf("%-4d ", gantt_time[i]);
    printf("\n");
}

int main() {
    Process proc[MAX_PROCESSES];
    int n, quantum;

    printf("=== Round Robin CPU Scheduling ===\n\n");

    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    for (int i = 0; i < n; i++) {
        proc[i].pid          = i + 1;
        proc[i].is_done      = 0;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &proc[i].arrival_time);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &proc[i].burst_time);
        proc[i].remaining_time = proc[i].burst_time;
    }

    round_robin(proc, n, quantum);
    display(proc, n);

    return 0;
}
