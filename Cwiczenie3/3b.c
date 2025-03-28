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
    assert(argc == 4 && "Zła liczba argumentów! Proszę podać program do uruchomienia!\n"); // Zabezpieczenie

    // Jeśli argv[1] nie zawiera ścieżki (tzn. nie zaczyna się od './' to dodajemy ją
    char path[256];
    snprintf(path, sizeof(path), "./%s", argv[1]);

    pid_t pid = fork();
        switch (pid) {
            case -1:
                perror("fork error");
                exit(1);

            case 0:
                // Proces potomny uruchamia inny program przy pomocy execlp
                printf(" >> Stworzenie potomka PID: %d PPID: %d\n", getpid(), getppid());
                execlp(path, path, argv[2], argv[3], (char *)NULL);
                perror("execlp error");
                _exit(2);

            default:
              break;
        }
    // Proces macierzysty czeka chwilę
    sleep(2);

    // Sprawdzenie czy proces potomny zyje
    if(kill(pid,0) == 0){
        printf("[Macierzysty] Proces potomny (PID:%d) żyje, więc wysyłam sygnał\n", pid);
        sleep(2);
        kill(pid, atoi(argv[2])); // wysłanie SIGINT
        }
     else
        perror("[Macierzysty] Proces nie istnieje");

       int status;
       if (wait(&status) == -1) {
    		perror("wait error");
    		exit(1);
		}
       else {
    		printf("[Macierzysty] Proces potomny (PID:%d) zakonczyl zycie\n", pid);
    		if(WIFSIGNALED(status))
        	{
                  sleep(1);
                  printf("[Macierzysty] >> Zabił go sygnał %d (%s)\n", WTERMSIG(status), strsignal(WTERMSIG(status)));
            }
		}

    // Proces macierzysty czeka na zakonczenie wszystkich potomkow
    printf("Proces macierzysty (PID: %d PPID: %d) zakonczyl prace.\n", getpid(),getppid());

    return 0;
}
