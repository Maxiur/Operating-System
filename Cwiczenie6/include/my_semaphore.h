#IFNDEF MY_SEPARATOR_H
#DEFINE MY_SEPARATOR_H

#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

// Funkcja tworząca semafor
sem_t* my_sem_init(const char* name, int init_value);

// Funkcja otwierająca semafor
sem_t* my_sem_open(const char* name);

// Funkcja zamykająca semafor
sem_t* my_sem_close(sem_t* sem);

// Funkcja usuwająca semafor
int my_sem_unlink(const char* name);

// Funkcja oczekująca na semafor
int my_sem_wait(sem_t* sem);

// Funkcja sygnalizująca zwolnienie zasobów semafora
int my_sem_post(sem_t* sem);

// Funkcja sprawdzająca wartość semafora
int my_sem_get_value(sem_t* sem, int *svalue);

#endif // MY_SEPARATOR_H

