#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void)
{
pid_t pid = fork();
if (pid < 0) {
perror("fork");
return EXIT_FAILURE;
}
if (pid == 0) {
printf("Child: PID = %ld, terminating now.
", (long)getpid());
_exit(EXIT_SUCCESS);
}
printf("Parent: PID = %ld, child PID = %ld
",
(long)getpid(), (long)pid);
printf("For the next 10 seconds, inspect the child using:
");
printf("ps -o pid,ppid,state,cmd -p %ld
", (long)pid);
/*
The child has exited, but the parent has not yet collected
its status. During this period, the child is a zombie.
/
sleep(10);
if (waitpid(pid, NULL, 0) == -1) {
perror("waitpid");
return EXIT_FAILURE;
}
printf("Parent collected the child. The zombie is removed.
");
return EXIT_SUCCESS;
}
