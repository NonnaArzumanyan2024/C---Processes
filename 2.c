/*
Write a program that will create 3 child processes and wait until any of the 3 completes its work,
the child process that completes its work first will print the pid of that process to the screen. 
When you find that the process has completed, kill the other 2 processes as well.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define NUM_CHILDREN 3

int main() {
    pid_t pids[NUM_CHILDREN];
    int i;

    srand(time(NULL));

    for (i = 0; i < NUM_CHILDREN; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) {
            printf("Child process %d started with PID %d\n", i + 1, getpid());
            sleep((rand() % 5) + 1);

            printf("Child process with PID %d finished\n", getpid());
            exit(EXIT_SUCCESS);
        }
    }

    printf("Waiting for the first child process to complete...\n");

    int status;
    pid_t finished_pid = wait(&status);

    if (finished_pid > 0) {
        printf("Child process with PID %d finished first.\n", finished_pid);

        for (i = 0; i < NUM_CHILDREN; i++) {
            if (pids[i] != finished_pid) {
                printf("Killing child process with PID %d\n", pids[i]);
                if (kill(pids[i], SIGKILL) == -1) {
                    perror("Error killing process");
                }
            }
        }
    } else {
        perror("Error waiting for child process");
        exit(EXIT_FAILURE);
    }

    while (wait(NULL) > 0);

    printf("All child processes have been handled.\n");

    return 0;
}
