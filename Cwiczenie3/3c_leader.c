//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Autor: Mateusz Kamiński                      			Krakow 27.03.2025
//
// Program pomocniczy (LEADER) do podpunktu C
// Tworzy grupę procesów i uruchamia 3 potomki programu z podpunktu A.
// Ignoruje sygnał, ustawia siebie jako lidera grupy i czeka na dzieci.

#define _XOPEN_SOURCE 700 // Definiuje standart POSIX, ktory wlacza getpgid()
#define _GNU_SOURCE // Dla strsignal()
#define _POSIX_C_SOURCE 200112L // Dla kill

#define CHILDREN 3 // Liczba procesów potomnych

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[]) {

      assert(argc == 4 && "Zła liczba argumentów! Proszę wywołać - ./program_leader program_a numer_sygnału"); // Zabezpieczenie

    // Jeśli argv[1] nie zawiera ścieżki (tzn. nie zaczyna się od './' to dodajemy ją)
    char path[256];
    snprintf(path, sizeof(path), "./%s", argv[1]);

      printf("[Lider] >> (PID: %d | PPID: %d)\n", getpid(), getppid());
      // Ignorowanie sygnału w procesie lidera
      if(signal(SIGINT, SIG_IGN) == SIG_ERR){
        perror("signal error");
        exit(1);
      }
      // Ustawienie jako lider grupy procesów
      if(setpgid(0, 0) == -1){
        perror("setpgid error");
        exit(2);
      }

        for(int i = 0; i < CHILDREN; i++) { //
            pid_t pid = fork();
            switch (pid) {
                case -1:
                    perror("fork error");
                    exit(3);

                case 0:
                    // Proces potomny uruchamia inny program przy pomocy execlp
                    printf(" >> [Dziecko %d] PID: %d PPID: %d\n", i, getpid(), getppid());
                    execlp(path, path, argv[2], argv[3], (char *)NULL);
                    perror("execlp error");
                    _exit(4);

                default:
                    sleep(0.1); // Czekamy chwilę przed stworzeniem kolejnego dziecka
                  break;
            }
        }

    printf("[Lider] >> PID: %d PPID: %d stworzył %d dzieci\n", getpid(), getppid(), CHILDREN);
    printf("---------------------------------------------------------------\n");

    for(int i = 0; i < CHILDREN; i++) {
        int status;
        pid_t ended = wait(&status);

        // Czekamy na zakończenie każdego dziecka
        if (ended == -1) {
            perror("wait error");
            continue;
        }

        printf("[Lider] Potomek %d PID: %d zakończył się\n", i, ended);

        if(WIFSIGNALED(status)) {
          int sig = WTERMSIG(status);
          printf("[Lider]    --> Zabił go sygnał %d (%s)\n", sig, strsignal(sig));
        }
        else if(WIFSIGNALED(status))
          printf("[Lider    --> Zakończony normalnie (kod: %d)\n", WEXITSTATUS(status));
    }

    //printf("[Lider] >> Wszystkie dzieci się zakończyły, kończę pracę\n");
    printf("Proces macierzysty [Lider] zakończył pracę\n");

    return 0;
}