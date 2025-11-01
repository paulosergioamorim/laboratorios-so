#include <stdio.h>
#include <sys/wait.h> // For wait and related macros.
#include <unistd.h>   // For the syscall functions.

int main()
{
    pid_t pid = fork(); // Fork a child process.
    if (pid < 0)
    { // Error occurred.
        fprintf(stderr, "Fork failed!\n");
        return 1;
    }
    else if (pid == 0)
    { // Child process.
        printf("[CHILD]: I'm finished.\n");
		pause();
       return 42;
    }
    else
    { // Parent process.
        printf("[PARENT]: Waiting on child.\n");
        int wstatus;
        wait(&wstatus);
        if (WIFSIGNALED(wstatus))
        {
            printf("[PARENT]: Child returned with code %d.\n", WTERMSIG(wstatus));
        }
    }
    return 0;
}
