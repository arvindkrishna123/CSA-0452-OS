/*
 * Program 10: Inter-Process Communication (IPC) using Message Queue
 * Parent sends messages; child receives them.
 *
 * System calls used:
 *   msgget()  - Create/access a message queue
 *   msgsnd()  - Send a message to the queue
 *   msgrcv()  - Receive a message from the queue
 *   msgctl()  - Control/remove the message queue
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_KEY  0x5678
#define MSG_SIZE 256

/* Message structure — first field must be long mtype */
typedef struct {
    long mtype;          /* Message type (must be > 0) */
    char mtext[MSG_SIZE]; /* Message content */
} Message;

int main() {
    int     msg_id;
    Message msg;
    pid_t   pid;

    printf("=== IPC using Message Queue ===\n\n");

    /* Create message queue */
    msg_id = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msg_id < 0) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
    printf("[Main]   Message queue created. ID = %d\n", msg_id);

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);

    } else if (pid == 0) {
        /* ===== CHILD — Receiver ===== */
        printf("[Child]  Waiting to receive messages...\n");

        /* Receive messages until type 0 sentinel */
        while (1) {
            if (msgrcv(msg_id, &msg, MSG_SIZE, 0, 0) < 0) {
                perror("msgrcv failed");
                exit(EXIT_FAILURE);
            }
            printf("[Child]  Received (type=%ld): \"%s\"\n", msg.mtype, msg.mtext);
            if (msg.mtype == 99) break; /* Sentinel to stop */
        }

        printf("[Child]  Done receiving. Exiting.\n");
        exit(EXIT_SUCCESS);

    } else {
        /* ===== PARENT — Sender ===== */
        sleep(1); /* Give child time to set up */

        /* Send multiple messages */
        const char *messages[] = {
            "Hello from Parent — Message 1",
            "OS Lab: Message Queue IPC Demo",
            "This is Message 3",
            "Final message — Goodbye!"
        };

        for (int i = 0; i < 4; i++) {
            msg.mtype = i + 1;
            strncpy(msg.mtext, messages[i], MSG_SIZE - 1);
            if (msgsnd(msg_id, &msg, strlen(msg.mtext) + 1, 0) < 0) {
                perror("msgsnd failed");
                exit(EXIT_FAILURE);
            }
            printf("[Parent] Sent (type=%ld): \"%s\"\n", msg.mtype, msg.mtext);
            sleep(1);
        }

        /* Send sentinel */
        msg.mtype = 99;
        strcpy(msg.mtext, "STOP");
        msgsnd(msg_id, &msg, strlen(msg.mtext) + 1, 0);
        printf("[Parent] Sent sentinel (type=99). Waiting for child...\n");

        wait(NULL);

        /* Remove message queue */
        msgctl(msg_id, IPC_RMID, NULL);
        printf("[Parent] Message queue removed.\n");
        printf("\n[Done] IPC via Message Queue completed successfully.\n");
    }

    return 0;
}
