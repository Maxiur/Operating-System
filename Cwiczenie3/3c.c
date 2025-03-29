//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Autor: Mateusz Kamiński                      			      Krakow 27.03.2025
// CWICZENIE 3C Wysylanie sygnalow do procesow i ich obsluga
//
// Program uruchamia program_leader w osobnym procesie,
// następnie pobiera identyfikator grupy procesów i wysyła do niej sygnał,
// którego numer oraz tryb obsługi przekazywane są jako argumenty.
// Po wysłaniu sygnału czeka na zakończenie programu_leader.
//
// Przykładowe uruchomienie: ./program_c program_leader program_a 2 D

#define _XOPEN_SOURCE 700 // Definiuje standart POSIX, ktory wlacza getpgid()
#define _GNU_SOURCE // Dla strsignal()
#define _POSIX_C_SOURCE 200112L // Dla kill

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int main(int argc, char *argv[]) {
    assert(argc == 5 && "Zła liczba argumentów! Proszę wywołać - ./program_c program_leader program_a numer_sygnału"); // Zabezpieczenie

    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("[PROGRAM C] >> (PID: %d | PPID: %d | PGID: %d)\n", getpid(), getppid(), getpgid(0));

    char path_l[256];
    snprintf(path_l, sizeof(path_l), "./%s", argv[1]);

    pid_t pid = fork();

    switch(pid){
      case -1:
        perror("fork error");
        exit(1);
      case 0:
        // Proces potomny uruchamia inny program przy pomocy execlp
        printf("[PROGRAM C] Uruchamiam %s\n", argv[1]);
        printf("-----------------------------------------------------------\n");
        execlp(path_l, path_l, argv[2], argv[3], argv[4], (char *)NULL);
        perror("execlp error");
        _exit(2);

      default:
        sleep(5);

        pid_t pgid=getpgid(pid);
        if(pgid == -1){
          perror("getpgid error");
          exit(3);
        }

        printf("[PROGRAM C] >> Wysyłam sygnał %d (%s) do grupy procesów PGID: %d\n ", atoi(argv[3]),strsignal(atoi(argv[3])), pgid);
        sleep(2);

        if(kill(-pgid, atoi(argv[3])) == -1){
          perror("kill error");
          exit(4);
        }

        int status;
        wait(&status);

        printf("[PROGRAM C] >> Program_leader zakończył pracę.\n");
        break;
    }

    printf("Proces macierzysty [PROGRAM C] (PID: %d) zakończył pracę.\n", getpid());

    return 0;
}
