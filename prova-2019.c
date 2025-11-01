#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void S(int x);

int main()
{
    S(1);
    int pid0 = fork();
    if (!pid0)
    {
        S(2);
        int pid1 = fork();
        if (!pid1)
        {
            S(3);
            S(6);
            exit(0);
        }
        S(4);
        waitpid(pid1, NULL, 0);
        S(7);
        exit(0);
    }
    S(5);
    waitpid(pid0, NULL, 0);
    S(8);
    exit(0);
}

void S(int x)
{
    char str[4];
    int len = snprintf(str, 4, "S%d\n", x);
    write(STDOUT_FILENO, str, len);
}
