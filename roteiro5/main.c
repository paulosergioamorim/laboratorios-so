#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void handlerA(int signal);

void handlerB(int signal);

int main()
{
    int pidA = 0;
    int pidB = 0;
    sigset_t mask;
    sigset_t prevMask;

    pidA = fork();

    if (pidA)
        pidB = fork();

    if (!pidA || !pidB)
    {
        struct sigaction action;
        struct sigaction prevAction;
        action.sa_handler = !pidA ? handlerA : handlerB;

        if (sigemptyset(&action.sa_mask) == -1 || sigaddset(&action.sa_mask, SIGINT) == -1 ||
            sigaction(SIGUSR1, &action, &prevAction) == -1)
        {
            printf("Falha ao carregar handler para o filho.\n");
            return 1;
        }

        if (sigfillset(&mask) == -1 || sigdelset(&mask, SIGUSR1) == -1 ||
            sigprocmask(SIG_BLOCK, &mask, &prevMask) == -1)
        {
            printf("Falha ao setar mascara de sinais para o filho.\n");
            return 1;
        }

        action.sa_handler = SIG_IGN;

        while (1)
        {
            pause();
            sleep(1);
            sigaction(SIGUSR1, &action, &prevAction);
            kill(0, SIGUSR1);
            sigaction(SIGUSR1, &prevAction, &action);
        }

        return 1;
    }

    if (sigemptyset(&mask) == -1 || sigaddset(&mask, SIGINT) == -1 || sigaddset(&mask, SIGUSR1) == -1 ||
        sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
    {
        printf("Falha ao setar mascara de sinais para o pai.\n");
        return 1;
    }

    sleep(1);
    kill(pidA, SIGUSR1);
    pid_t deadPid = waitpid(-1, NULL, 0);
    printf("Um dos meus filhos morreu. Vou matar meu outro filho se estiver vivo e encerrar o programa.\n");
    kill(deadPid == pidA ? pidB : pidA, SIGKILL);

    return 0;
}

void handlerA(int signal)
{
    char msg[100] = "";
    int len = snprintf(msg, 100, "[%d]-PING\n", getpid());
    write(STDOUT_FILENO, msg, len);
}

void handlerB(int signal)
{
    char msg[100] = "";
    int len = snprintf(msg, 100, "[%d]-PONG\n", getpid());
    write(STDOUT_FILENO, msg, len);
}
