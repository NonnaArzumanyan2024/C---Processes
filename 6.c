/*
Implement a C program that creates shared memory for a string. The parent process prompts the user for a sentence and writes it to shared memory. The first child counts the number of words in the sentence and writes the count back to shared memory. The second child counts the number of characters (excluding spaces) and writes that back as well. The parent waits for both children and prints the results.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

// Function to count the number of words in a string
int count_words(const char* str) {
    int count = 0;
    // Loop through the string to find words
    while (*str) {
        // Skip leading spaces, tabs, and newlines
        while (*str && (*str == ' ' || *str == '\t' || *str == '\n')) {
            str++;  
        }
        // If a non-space character is found, it's the start of a new word
        if (*str) {
            count++;  
            // Move through the word
            while (*str && *str != ' ' && *str != '\t' && *str != '\n') {
                str++; 
            }
        }
    }
    return count;
}

// Custom function to check if a character is alphabetic
int my_isalpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// Function to count the number of alphabetic characters in a string
int count_characters(const char *str) {
    int count = 0;
    while (*str) {
        // Count only alphabetic characters
        if (my_isalpha(*str)) { 
            ++count;
        }
        ++str;
    }
    return count;
}

int main() {
    // Create shared memory using mmap
    void* shared_memory = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid1, pid2;

    // Parent prompts for and writes a sentence to shared memory
    printf("Enter a sentence: ");
    fgets((char*)shared_memory, SHM_SIZE, stdin);
    ((char*)shared_memory)[strcspn((char*)shared_memory, "\n")] = '\0'; // Remove newline character

    // Create the first child process to count words
    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // Child Process 1: Count the number of words in the sentence
        char* shmaddr = (char*)shared_memory;
        int word_count = count_words(shmaddr);
        sprintf(shmaddr + strlen(shmaddr), "\nWord Count: %d", word_count);
        exit(EXIT_SUCCESS);
    }

    // Wait for the first child to finish
    waitpid(pid1, NULL, 0);

    // Create the second child process to count characters
    pid2 = fork();

    if (pid2 < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // Child Process 2: Count the number of alphabetic characters in the sentence
        char *shmaddr = (char *)shared_memory;
        int char_count = count_characters(shmaddr);
        sprintf(shmaddr + strlen(shmaddr), "\nCharacter Count: %d", char_count);
        exit(EXIT_SUCCESS);
    }

    // Wait for the second child to finish
    waitpid(pid2, NULL, 0);

    // Parent process prints the results from shared memory
    printf("\nParent Process (PID: %d):\n%s\n", getpid(), (char *)shared_memory);

    // Clean up shared memory
    if (munmap(shared_memory, SHM_SIZE) == -1) {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

