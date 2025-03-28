//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Autor: Mateusz Kamiński                      Krakow 27.03.2025
// CWICZENIE 3A Wysylanie sygnalow do procesow i ich obsluga
//
// Program wysyla sygnal do samego siebie i ustawia jego obsluge
// na domyslna, ignorowanie lub przechwycenie. W przypadku przechwycenia
// sygnalu, program wyswietla jego numer i nazwe.
//
// Przykładowe uruchomienie: ./program_a 2 P

#define _XOPEN_SOURCE 700 // Definiuje standart POSIX, ktory wlacza getpgid()
#define _GNU_SOURCE // Dla strsignal()
#define _POSIX_C_SOURCE 200112L // Dla kill

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

// Funkcja obsługi sygnału
void staff(int sig) {
    printf("[OBSŁUGA] >> Przechwycono sygnał numer: %d (%s)\n", sig, strsignal(sig));
    sleep(1);
}

int main(int argc, char *argv[]){
    assert(argc == 3 && "Zła liczba argumentów! Proszę wywołać - ./program numer_sygnału opcja[D/I/P]"); // Zabezpieczenie

    // Argumenty wywołania
    int sig = atoi(argv[1]); // Konwersja napisu na liczbę
    if (sig == SIGKILL || sig == SIGSTOP) {
        fprintf(stderr, "Sygnał %d (%s) nie może być przechwycony, ignorowany ani obsłużony przez signal().\n", sig, strsignal(sig));
        exit(1);
    }

    char opcja = toupper(argv[2][0]); // argv[2][n] czyta n - ty znak z napisu

    // Konfiguracji reakcji na sygnał
    switch (opcja) {
        case 'D':
            if (signal(sig, SIG_DFL) == SIG_ERR) {
                perror("Funkcja signal ma problem z ustawieniem domyślnej obsługi sygnału");
                exit(1);
            }
        printf("[Tryb Domyślny] Wysyłam sygnał %d do procesu o PID: %d\n", sig, getpid());
        break;
        case 'I':
            if (signal(sig, SIG_IGN) == SIG_ERR) {
                perror("Funkcja signal ma problem z ignorowaniem sygnału");
                exit(1);
            }
        printf("[Tryb Ignorowania] Wysyłam sygnał %d do procesu o PID: %d\n", sig, getpid());
        break;
        case 'P':
            if (signal(sig, staff) == SIG_ERR) {
                perror("Funkcja signal ma problem przy ustawianiu własnej obsługi sygnału");
                exit(1);
            }
        printf("[Tryb Przechwycenia] Wysyłam sygnał %d do procesu o PID: %d\n", sig, getpid());
        break;
        default:
            printf("Niezdefiniowana opcja! Proszę podać D, I lub P\n");
        exit(1);
    }

    printf("Czekam na sygnał %d (%s)...\n", sig, strsignal(sig));

    while (1){
        pause();
    }

    return 0;
 }