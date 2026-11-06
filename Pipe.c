#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
 
#define BUFFER_SIZE 256
 
int main(void)
{
    int parent_to_child[2];
    int child_to_parent[2];
 
    if (pipe(parent_to_child) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }
 
    if (pipe(child_to_parent) == -1) {
        perror("pipe");
        close(parent_to_child[0]);
        close(parent_to_child[1]);
        return EXIT_FAILURE;
    }
 
    pid_t pid = fork();
 
    if (pid < 0) {
        perror("fork");
 
        close(parent_to_child[0]);
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        close(child_to_parent[1]);
 
        return EXIT_FAILURE;
    }
 
    if (pid == 0) {
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
 
        close(parent_to_child[1]);
        close(child_to_parent[0]);
 
        bytes_read = read(parent_to_child[0],
                          buffer,
                          sizeof(buffer) - 1);
 
        if (bytes_read < 0) {
            perror("child read");
            _exit(EXIT_FAILURE);
        }
 
        buffer[bytes_read] = '\0';
 
        for (ssize_t i = 0; i < bytes_read; i++) {
            buffer[i] = (char)toupper((unsigned char)buffer[i]);
        }
 
        size_t length = strlen(buffer);
 
        if (write(child_to_parent[1], buffer, length + 1) == -1) {
            perror("child write");
            _exit(EXIT_FAILURE);
        }
 
        close(parent_to_child[0]);
        close(child_to_parent[1]);
 
        _exit(EXIT_SUCCESS);
    }
 
    char message[BUFFER_SIZE];
    char result[BUFFER_SIZE];
 
    close(parent_to_child[0]);
    close(child_to_parent[1]);
 
    printf("Enter a message: ");
 
    if (fgets(message, sizeof(message), stdin) == NULL) {
        fprintf(stderr, "Failed to read input.\n");
 
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        waitpid(pid, NULL, 0);
 
        return EXIT_FAILURE;
    }
 
    size_t message_length = strlen(message);
 
    if (write(parent_to_child[1],
              message,
              message_length + 1) == -1) {
        perror("parent write");
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        waitpid(pid, NULL, 0);
        return EXIT_FAILURE;
    }
 
    close(parent_to_child[1]);
 
    ssize_t bytes_read = read(child_to_parent[0],
                              result,
                              sizeof(result) - 1);
 
    if (bytes_read < 0) {
        perror("parent read");
        close(child_to_parent[0]);
        waitpid(pid, NULL, 0);
        return EXIT_FAILURE;
    }
 
    result[bytes_read] = '\0';
 
    printf("Uppercase result: %s", result);
 
    close(child_to_parent[0]);
 
    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
 
    return EXIT_SUCCESS;
}
