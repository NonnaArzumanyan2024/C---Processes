/*
Using execv, write a program that prints to the screen the output of the ps -ef command located in the bin/ps folder.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char* path = "/bin/ps";
    char* args[] = {"ps", "-ef", NULL};

    if (execv(path, args) == -1) {
        perror("Error executing ps -ef using execv");        
        exit(EXIT_FAILURE);
    }

    return 0;
}
