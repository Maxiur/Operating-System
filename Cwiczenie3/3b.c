//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Autor: Mateusz Kamiński                      Krakow 27.03.2025
// CWICZENIE 3B Wysylanie sygnalow do procesow i ich obsluga
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
    assert(argc == 2 && "Zła liczba argumentów! Proszę podać program do uruchomienia!\n"); // Zabezpieczenie

    pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("fork error");
                exit(1);

            case 0:
                // Proces potomny uruchamia inny program przy pomocy execlp
                printf(" >> Stworzenie potommka PID: %d PPID: %d\n", getpid(), getppid());
                execlp(argv[1], argv[1], "2" , "P", (char *)NULL);
                perror("execlp error");
                _exit(2);

            default:
              break;
        }
    // Proces macierzysty czeka chwilę
    sleep(2);

    // Sprawdzenie czy proces potomny zyje
    if(kill(pid,0) == 0){
        printf("Proces potomny (PID: %d) zyje\n", pid);
        kill(pid, 2); // wysłanie SIGINT
        }
     else
        perror("Proces nie istnieje");

     if(wait(NULL) == -1) {
        perror("wait error");
        exit(1);
        }

    // Proces macierzysty czeka na zakonczenie wszystkich potomkow
    printf("Proces macierzysty (PID: %d PPID: %d) zakonczyl prace.\n", getpid(),getppid());

    return 0;
}
