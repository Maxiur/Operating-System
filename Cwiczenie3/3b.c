//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Autor: Mateusz Kamiński                      			Krakow 27.03.2025
// CWICZENIE 3B Wysylanie sygnalow do procesow i ich obsluga
//
// W procesie potomnym uruchamiany jest program z podpunktu (a)
// za pomocą execlp() i przekazywane są do niego argumenty:
// numer sygnału i tryb obsługi (domyślny, ignorowanie, własna obsługa).
//
// Przykład uruchomienia:
// 		./program_b program_a 2 D
// gdzie:
//		program_a - nazwa programu z (a)
//      2 - numer sygnału (np. SIGINT)
//      D - tryb obsługi: D - domyślny, I - ignorowanie, P - własna obsługa
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


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

    // Ignorowanie sygnału, który sami wyślemy, by nie zabił procesu macierzystego
    signal(atoi(argv[2]), SIG_IGN);
    // Jeśli argv[1] nie zawiera ścieżki (tzn. nie zaczyna się od './' to dodajemy ją)
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
        printf("[Lider] Proces potomny (PID:%d) istnieje (może być zombie), więc wysyłam sygnał\n", pid);
        sleep(2);
        kill(pid, atoi(argv[2])); // wysłanie sygnału podanego przez użytkownika
        }
     else
        perror("[Lider] Proces nie istnieje");

       int status;
       if (wait(&status) == -1) {
    		perror("wait error");
    		exit(1);
		}
       else {
    		printf("[Lider] Proces potomny (PID:%d) zakonczyl zycie\n", pid);
    		if(WIFSIGNALED(status))
        	{
                  sleep(1);
                  printf("[Lider] >> Zabił go sygnał %d (%s) (kod zakonczenia: %d)\n", WTERMSIG(status), strsignal(WTERMSIG(status)), status);
            }
		}

    // Proces macierzysty czeka na zakonczenie wszystkich potomkow
    printf("Proces macierzysty [Lider] (PID: %d PPID: %d) zakończyl prace.\n", getpid(),getppid());

    return 0;
}
