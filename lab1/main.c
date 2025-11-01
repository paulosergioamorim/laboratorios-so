#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return EXIT_FAILURE;
    }

    const int N = atoi(argv[1]);
    int id = 0;

    for (int i = 0; i < N; i++)
        if ((id = fork()) == 0)
            break;

    printf("Processo criado com sucesso\n");

    return 0;
}