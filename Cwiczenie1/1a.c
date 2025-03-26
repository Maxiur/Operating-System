#define _XOPEN_SOURCE 700 // Definiuje standart POSIX, ktory wlacza getpgid()

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  printf("UID: %d\n", getuid());    // Identyfikator uzytkownika
  printf("GID: %d\n", getgid());    // Identyfikator grupy uzytkownika
  printf("PID: %d\n", getpid());    // Identyfikator procesu
  printf("PPID: %d\n", getppid());  // Identyfikator procesu macierzystego
  printf("PGID: %d\n", getpgid(0)); // Identyfikator grupy procesow

  return 0;
}
