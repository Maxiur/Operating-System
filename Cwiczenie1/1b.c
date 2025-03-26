#define _XOPEN_SOURCE 700 // Definiuje standart POSIX, ktory wlacza getpgid()

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Proces macierzysty: UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d\n",
         getuid(), getgid(), getpid(), getppid(), getpgid(0));

  for (int i = 0; i < 3; i++) {
    switch (fork()) {
    case -1:
      perror("fork error");
      exit(1);

    case 0:
      // Proces potomny
      printf("Proces potomny: UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d\n",
             getuid(), getgid(), getpid(), getppid(), getpgid(0));

    default:
      // Proces macierzysty kontynuuje petle
      break;
    }
  }

  // Proces macierzysty czeka na zakonczenie wszystkich potomkow
  while (wait(NULL) > 0)
    ; // Nie interesuje nas status zakonczenia, tylko kiedy procesy potomne sie
      // zakoncza

  return 0;
}
