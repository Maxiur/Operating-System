#include <stdio.h>
#include <unistd.h>

int main() {
	printf(">>> Komunikat z programu uruchomionego przez execlp!\n");
	printf(">>> PID: %d | PPID: %d\n", getpid(), getppid());

	return 0;
}
