/*
Design a program in C that implements inter-process communication between a parent and a child process 
using shared memory. The goal is to enable the child process to collect user input 
(a list of integers) and store it in shared memory. 
The parent process will then retrieve this data, find the maximum integer value, and display it. 
The program should include proper synchronization to ensure the parent waits for the child to complete its task before accessing the shared memory. Finally, the program should clean up the shared memory segment after both processes have finished.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main() {
    void* shared_memory = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        int n;
        printf("Child Process: Enter the number of integers: ");
        if (scanf("%d", &n) != 1 || n <= 0) {
            printf("Invalid input! Please enter a positive integer.\n");
            exit(EXIT_FAILURE);
        }

        if (n * sizeof(int) + sizeof(int) > SHM_SIZE) {
            printf("Error: Too many integers. Exceeds shared memory size.\n");
            exit(EXIT_FAILURE);
        }

        int* arr = (int*)shared_memory;
        arr[0] = n;

        printf("Child Process: Enter %d integers: ", n);
        for (int i = 1; i <= n; i++) {
            if (scanf("%d", &arr[i]) != 1) {
                printf("Invalid input! Please enter integers only.\n");
                exit(EXIT_FAILURE);
            }
        }

        printf("Child Process: Data written to shared memory.\n");

        if (munmap(shared_memory, SHM_SIZE) == -1) {
            perror("Child: munmap failed");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);

        int* arr = (int*)shared_memory;
        int n = arr[0];
        int max = arr[1];

        for (int i = 2; i <= n; i++) {
            if (arr[i] > max) {
                max = arr[i];
            }
        }

        printf("Parent Process: Maximum integer is %d\n", max);

        if (munmap(shared_memory, SHM_SIZE) == -1) {
            perror("Parent: munmap failed");
            exit(EXIT_FAILURE);
        }

        printf("Parent Process: Shared memory cleaned up.\n");
    }

    return 0;
}
