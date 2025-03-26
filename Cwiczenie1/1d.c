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
    pid_t pid = fork();

    switch (pid) {
    case -1:
      perror("fork error");
      exit(1);

    case 0:
      // Proces potomny
      printf(
          "Proces potomny %d: UID: %d, GID: %d, PID: %d, PPID: %d, PGID: %d\n",
          getppid(), getuid(), getgid(), getpid(), getppid(), getpgid(0));
      sleep(i + 1);
    }
  }
  sleep(5);

  return 0;
}
