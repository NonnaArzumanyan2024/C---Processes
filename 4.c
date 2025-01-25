/*
Write a program that will allow the user to enter Linux commands until the user types the word exit. Implement a function that will take the entered command as an argument and create a child process, which will then display the result of the command in the terminal.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD_LENGTH 256  // Maximum length of a command

// Function to execute a Linux command in a child process
void execute_command(char* command) {
    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        perror("Fork failed"); // Handle fork error
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process: Execute the command using /bin/sh
        char* args[] = {"/bin/sh", "-c", command, NULL};
        if ((execv("/bin/sh", args)) < 0) {
            perror("execv failed"); 
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process: Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Command exited with status %d\n", WEXITSTATUS(status));
        }
    }
}

int main() {
    char command[MAX_CMD_LENGTH];  // Buffer for storing user input

    printf("Enter Linux commands (type 'exit' to quit):\n");

    while (1) {
        printf("> ");  // Display a prompt for better UX

        // Read user input
        if (fgets(command, MAX_CMD_LENGTH, stdin) == NULL) {
            perror("fgets failed"); // Handle input error
            break;
        }

        // Remove the newline character from the input
        command[strcspn(command, "\n")] = '\0';

        // Check if the user wants to exit
        if (strcmp(command, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Execute the entered command
        execute_command(command);
    }

    return 0;
}