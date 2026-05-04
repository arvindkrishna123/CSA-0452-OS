/*
 * Program 1: Create a new process using fork() system call.
 * Display the PID of the current process and its parent using
 * getpid() and getppid() system calls.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("=== Process Creation using fork() ===\n\n");

    /* Display parent process info before fork */
    printf("Before fork:\n");
    printf("  Current Process PID  : %d\n", getpid());
    printf("  Parent Process PID   : %d\n\n", getppid());

    /* Create a child process */
    pid = fork();

    if (pid < 0) {
        /* Fork failed */
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        printf("--- Inside CHILD Process ---\n");
        printf("  Child PID            : %d\n", getpid());
        printf("  Child's Parent PID   : %d\n", getppid());
        printf("  Child process exiting...\n\n");
        exit(EXIT_SUCCESS);
    } else {
        /* Parent process */
        wait(NULL); /* Wait for child to finish */
        printf("--- Inside PARENT Process ---\n");
        printf("  Parent PID           : %d\n", getpid());
        printf("  Parent's Parent PID  : %d\n", getppid());
        printf("  Child that was created had PID: %d\n", pid);
        printf("  Parent process exiting...\n");
    }

    return 0;
}
