#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
 
int main(void)
{
    pid_t pid = fork();
 
    if (pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
 
    if (pid > 0) {
        printf("Parent: PID = %ld, child PID = %ld\n",
               (long)getpid(), (long)pid);
        printf("Parent is terminating now.\n");
 
        return EXIT_SUCCESS;
    }
 
    printf("Child before parent termination: PID = %ld, PPID = %ld\n",
           (long)getpid(), (long)getppid());
 
    sleep(3);
 
    printf("Child after parent termination: PID = %ld, PPID = %ld\n",
           (long)getpid(), (long)getppid());
 
    return EXIT_SUCCESS;
}
 
