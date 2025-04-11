#ifndef MY_SEMAPHORE_H
#define MY_SEMAPHORE_H

/*
    =======================================================
    Makro CheckError: wskazuje lokalizację błędu, wypisuje nazwę
    przekazywanego argumentu (#Arg) i kończy program.
    =======================================================
*/
#define CheckError(Arg)                                                              \
    if (!(Arg)) {                                                                     \
        fprintf(stderr, "[ERROR] %s:%d (%s) -> CheckError argument: %s\n",             \
        __FILE__, __LINE__, __func__, #Arg);                                            \
        exit(EXIT_FAILURE);                                                              \
}                                                                                         \

#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>

// Funkcja tworząca semafor
sem_t* my_sem_init(const char* name, int init_value);

// Funkcja otwierająca semafor
// (Już utworzonego)
sem_t* my_sem_open(const char* name);

// Funkcja zamykająca semafor
int my_sem_close(sem_t* sem);

// Funkcja usuwająca semafor
int my_sem_unlink(const char* name);

// Funkcja oczekująca na semafor
int my_sem_wait(sem_t* sem);

// Funkcja sygnalizująca zwolnienie zasobów semafora
int my_sem_post(sem_t* sem);

// Funkcja sprawdzająca wartość semafora
int my_sem_get_value(sem_t* sem, int *svalue);

#endif // MY_SEMAPHORE

