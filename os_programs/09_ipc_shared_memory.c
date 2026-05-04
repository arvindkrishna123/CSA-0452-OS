/*
 * Program 9: Inter-Process Communication (IPC) using Shared Memory
 * A parent writes a message to shared memory; the child reads it.
 *
 * System calls used:
 *   shmget()  - Create/access shared memory segment
 *   shmat()   - Attach shared memory to process address space
 *   shmdt()   - Detach shared memory
 *   shmctl()  - Control/remove shared memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY  0x1234       /* Unique key for shared memory */
#define SHM_SIZE 1024         /* Size of shared memory (bytes) */

int main() {
    int   shm_id;
    char *shm_ptr;
    pid_t pid;

    printf("=== IPC using Shared Memory ===\n\n");

    /* Step 1: Create shared memory segment */
    shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
    printf("[Parent] Shared memory created. ID = %d\n", shm_id);

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);

    } else if (pid == 0) {
        /* ===== CHILD PROCESS — Reader ===== */
        sleep(1);  /* Wait for parent to write */

        /* Attach shared memory */
        shm_ptr = (char *)shmat(shm_id, NULL, 0);
        if (shm_ptr == (char *)-1) {
            perror("shmat failed in child");
            exit(EXIT_FAILURE);
        }

        printf("[Child]  Attached to shared memory at %p\n", (void *)shm_ptr);
        printf("[Child]  Message read from shared memory: \"%s\"\n", shm_ptr);

        /* Detach */
        shmdt(shm_ptr);
        printf("[Child]  Detached from shared memory.\n");
        exit(EXIT_SUCCESS);

    } else {
        /* ===== PARENT PROCESS — Writer ===== */

        /* Attach shared memory */
        shm_ptr = (char *)shmat(shm_id, NULL, 0);
        if (shm_ptr == (char *)-1) {
            perror("shmat failed in parent");
            exit(EXIT_FAILURE);
        }

        printf("[Parent] Attached to shared memory at %p\n", (void *)shm_ptr);

        /* Write message */
        const char *message = "Hello from Parent! This is shared memory IPC.";
        strncpy(shm_ptr, message, SHM_SIZE - 1);
        shm_ptr[SHM_SIZE - 1] = '\0';
        printf("[Parent] Message written: \"%s\"\n", shm_ptr);

        wait(NULL);  /* Wait for child to finish reading */

        /* Detach and remove shared memory */
        shmdt(shm_ptr);
        shmctl(shm_id, IPC_RMID, NULL);
        printf("[Parent] Shared memory detached and removed.\n");
        printf("\n[Done] IPC via Shared Memory completed successfully.\n");
    }

    return 0;
}
