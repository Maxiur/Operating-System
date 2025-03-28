//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Autor: Mateusz Kamiński                      			Krakow 27.03.2025
// CWICZENIE 3C Wysylanie sygnalow do procesow i ich obsluga
//

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

int main(int argc, char *argv[]) {
    assert(argc == 5 && "Zła liczba argumentów! Proszę wywołać - ./program_c program_leader program_a numer_sygnału"); // Zabezpieczenie

    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("[PROGRAM C] >> (PID: %d | PPID: %d)\n", getpid(), getppid());

    char path_l[256];
    snprintf(path_l, sizeof(path_l), "./%s", argv[1]);

    pid_t pid = fork();

    switch(pid){
      case -1:
        perror("fork error");
        exit(1);
      case 0:
        // Proces potomny uruchamia inny program przy pomocy execlp
        printf("[PROGRAM C] Uruchamiam program %s (PID: %d | PPID: %d)\n", argv[1], getpid(), getppid());
        printf("-----------------------------------------------------------\n");
        execlp(path_l, path_l, argv[2], argv[3], argv[4], (char *)NULL);
        perror("execlp error");
        _exit(2);

      default:
        sleep(2);

        pid_t pgid=getpgid(pid);
        if(pgid == -1){
          perror("getpgid error");
          exit(3);
        }

        printf("[PROGRAM C] >> Wysyłam sygnał %d (%s) do grupy procesów PGID: %d\n ", atoi(argv[3]),strsignal(atoi(argv[3])), pgid);

        if(kill(-pgid, atoi(argv[3])) == -1){
          perror("kill error");
          exit(4);
        }

        int status;
        wait(&status);

        printf("[PROGRAM C] >> Program leader zakończył pracę\n");
        break;
    }

    printf("Proces macierzysty [PROGRAM C] (PID: %d) zakończył pracę.\n", getpid());

    return 0;
}
