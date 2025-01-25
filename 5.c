/*
Write a program that will create 2 processes, each of which will call different functions. One of the functions will print the elements of the principal diagonal of a dynamic two-dimensional array to the screen, the other function will calculate the factorial of a number recursively. Take into account that the Parent process must wait until the child processes have finished. Print the PIDs of each process to the screen.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to print the main diagonal of a dynamically allocated 2D array
void print_diagonal(int n) {
    int **matrix = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++) {
            matrix[i][j] = (i + 1) * (j + 1); // Example values
        }
    }

    printf("Diagonal elements: ");
    
    for (int i = 0; i < n; i++) {
        printf("%d ", matrix[i][i]);
    }
    printf("\n");

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to calculate factorial recursively
long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    pid_t pid1, pid2;

    // Create the first process
    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // Child Process 1: Print the diagonal of a 2D array
        printf("Child Process 1 (PID: %d):\n", getpid());
        print_diagonal(5);
        exit(EXIT_SUCCESS);
    }

    // Create the second process
    pid2 = fork();

    if (pid2 < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // Child Process 2: Calculate factorial
        int num = 5;
        printf("Child Process 2 (PID: %d):\n", getpid());
        printf("Factorial of %d is %ld\n", num, factorial(num));
        exit(EXIT_SUCCESS);
    }

    // Parent process waits for both children
    printf("Parent Process (PID: %d): Waiting for children...\n", getpid());
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Parent Process: All children have finished.\n");
    
    return 0;
}
