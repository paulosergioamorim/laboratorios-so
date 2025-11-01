#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	pid_t pid = 0;

	if (argc < 2) 
		return 0;

	printf("%p\n", argv[0]);
	printf("%p\n", argv[1]);
	printf("%p\n", argv[2]);
	printf("%p\n", argv[3]);

	if (!(pid = fork())) {
		execvp(argv[1], argv + 1);
	}

	int status = 0;
	waitpid(pid, &status, 0);

	if (WIFEXITED(status)) 
		printf("[PAI] Processo filho terminado. Status code: %d.\n", WEXITSTATUS(status));

	return 0;
}
