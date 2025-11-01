#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    for (int i = 0; i < 3; i++)
    {
        if (!fork())
        {
            sleep(i * 3 + 5);
            return 0;
        }
    }

    while (1)
    {
        sleep(2);
        pid_t pid = waitpid(-1, NULL, WNOHANG);

        // -1 se waitpid() falhar. 0 se não ocorrer mudança devido especificarmos WNOHANG
        if (pid != -1 && pid != 0)
        {
            printf("Meu filho com PID=%d terminou\n", pid);
            continue;
        }

        if (pid == -1 && errno == ECHILD)
            break;

        printf("Nenhum filho terminou\n");
    }

    return 0;
}
