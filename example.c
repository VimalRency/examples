
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define CHILD_COUNT 3
int main(void)
{
    pid_t child_pids[CHILD_COUNT];
    for (int i = 0; i < CHILD_COUNT; i++)
    {
        child_pids[i] = fork();
        if (child_pids[i] < 0)
        {
            perror("fork");
            return EXIT_FAILURE;
        }
        if (child_pids[i] == 0)
        {
            int exit_code = (i + 1) * 10;
            printf("Child %d: PID = %ld, exit code = %d\n",
                   i + 1,
                   (long)getpid(),
                   exit_code);
            _exit(exit_code);
        }
    }
    for (int i = 0; i < CHILD_COUNT; i++)
    {
        int status;
        pid_t terminated_pid =
            waitpid(child_pids[i],
                    &status,
                    0);
        if (terminated_pid == -1)
        {
            perror("waitpid");
            continue;
        }
        if (WIFEXITED(status))
        {
            printf("Parent: child PID %ld exited normally with status %d\n",
                   (long)terminated_pid,
                   WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Parent: child PID %ld terminated by signal %d\n",
                   (long)terminated_pid,
                   WTERMSIG(status));
        }
        else
        {
            printf("Parent: child PID %ld ended abnormally\n",
                   (long)terminated_pid);
        }
    }
    return EXIT_SUCCESS;
}
