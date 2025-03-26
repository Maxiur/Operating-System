//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//Autor: Mateusz Kamiński                      Krakow 25.03.2025
//CWICZENIE 2 URUCHAMIANIE PROGRAMOW: EXEC

#define _XOPEN_SOURCE 700 // Definiuje standart POSIX, ktory wlacza getpgid()

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[]) {
  	assert(argc > 1 && "Za malo argumentow, prosze podac przynajmniej jeden."); // Zabezpieczenie

  for (int i = 0; i < 3; i++) {
    switch (fork()) {
    case -1:
      perror("fork error");
      exit(1);

    case 0:
      // Proces potomny uruchamia inny program przy pomocy execlp
      //printf("Proces potomny (PID: %d PPID: %d) uruchamia %s\n", getpid(),getppid(), argv[1]);
      execlp(argv[1], argv[1], (char *)NULL);
      perror("execlp error");
      _exit(2);
    }

    // Proces macierzysty czeka na zakonczenie kazdego procesu potomnego przed
    // przejsciem do nastepnego
    if(wait(NULL)==-1) {
    perror("wait error");
    exit(1);
    }
    sleep(1);
  }

  // Proces macierzysty czeka na zakonczenie wszystkich potomkow
  printf("Proces macierzysty (PID: %d PPID: %d) zakonczyl prace.\n", getpid(),getppid());

  return 0;
}
