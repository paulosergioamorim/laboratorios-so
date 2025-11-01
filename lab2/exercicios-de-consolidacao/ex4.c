#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BENCHMARK(block)                                                                                               \
    clock_t c1, c2;                                                                                                    \
    float tmp;                                                                                                         \
    c1 = clock();                                                                                                      \
    block;                                                                                                             \
    c2 = clock();                                                                                                      \
    tmp = (float)(c2 - c1) * 1000 / CLOCKS_PER_SEC;

void bubbleSort(int n, int *vec);

void quickSort(int *vec, int left, int right);

int main(int argc, char **argv)
{
    if (argc < 2)
        return 0;

    const int N = atoi(argv[1]);

    int *vec = malloc(N * sizeof(int));
    int ppid = getpid(); // to child only
    int pid = fork();

    if (!pid)
    {
        BENCHMARK(quickSort(vec, 0, N);)
        kill(ppid, SIGKILL);
        printf("[CHILD] Time with quick sort: %.2f\n", tmp);
        printf("Sou o filho. Matei meu pai\n");
        free(vec);

        return 0;
    }

    BENCHMARK(bubbleSort(N, vec);)
    kill(pid, SIGKILL);
    printf("[PARENT] Time with bubble sort: %.2f\n", tmp);
    printf("Sou o pai. Matei meu filho\n");
    free(vec);

    return 0;
}

void bubbleSort(int n, int *vec)
{
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
        {
            if (vec[i] <= vec[j])
                continue;

            int temp = vec[i];
            vec[i] = vec[j];
            vec[j] = temp;
        }
}

void quickSort(int *vec, int left, int right)
{
    int i, j, x, y;
    i = left;
    j = right;
    x = vec[(left + right) / 2];

    while (i <= j)
    {
        while (vec[i] < x && i < right)
            i++;

        while (vec[j] > x && j > left)
            j--;

        if (i <= j)
        {
            y = vec[i];
            vec[i] = vec[j];
            vec[j] = y;
            i++;
            j--;
        }
    }

    if (j > left)

        quickSort(vec, left, j);

    if (i < right)

        quickSort(vec, i, right);
}
