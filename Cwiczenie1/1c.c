#define _XOPEN_SOURCE 700 // Definiuje standart POSIX, ktory wlacza getpgid()

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  for (int i = 0; i < 3; i++) {
    switch (fork()) {
    case -1:
      perror("fork error");
      exit(1);

    case 0:
      // Proces potomny
      //printf("Proces potomny: UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d\n",
      //       getuid(), getgid(), getpid(), getppid(), getpgid(0));

      sleep(1); // Usypiamy proces potomny, aby stal sie osierocony

      printf(
          "Proces osierocony: UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d\n",
          getuid(), getgid(), getpid(), getppid(), getpgid(0));
      break;
    default:
      // Proces macierzysty kontynuuje petle
	break;
    }
  }

  return 0;
}
