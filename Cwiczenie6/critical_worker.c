//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 12.04.2025
//
// CWICZENIE 6 - Wzajemne wykluczanie dla procesów
// Program: critical_worker.c
//
// Proces realizujący zadane liczby sekcji krytycznych.
// W sekcji krytycznej odczytuje, zwiększa i zapisuje
// wartość w pliku numer.txt z użyciem semafora POSIX.
// Wypisuje PID, wartość semafora i aktualny numer.

#include "../include/my_semaphore.h"

int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        fprintf(stderr, "Za mało argumentów! Proszę wywołać: %s <nazwa semafora> <liczba sekcji krytycznych> \n", argv[0]);
        exit(1);
    }

    const char* sem_name = argv[1];
    int critical_sections = atoi(argv[2]);

    srand(time(NULL) + getpid());

    sem_t* my_sem = my_sem_open(sem_name);
    CheckError(my_sem != NULL);

    int value;
    for(int i = 0; i < critical_sections; i++) {

         // Zmienne pomocnicze do wypisania komunikatu w całości
         // snprintf zwraca liczbę zapisanych znaków w buforze
         char message[1024];
         int offset = 0;
         // --------------------------------------Sekcja Prywatna-------------------------------------------------
         // ------------------------------------------------------------------------------------------------------
         CheckError(my_sem_get_value(my_sem, &value));
         offset += snprintf(message + offset, sizeof(message) - offset,
         "PID: %d, Sekcja Prywatna, przed sekcją krytyczną (i = %d), semafor = %d\n",
         getpid(), i, value);
         // Symulacja pracy wewnątrz sekcji prywatnej
         sleep(rand() % 2 + 1);

         // --------------------------------------Sekcja Krytyczna------------------------------------------------
         // ------------------------------------------------------------------------------------------------------
         CheckError(my_sem_wait(my_sem));
         CheckError(my_sem_get_value(my_sem, &value));
         offset += snprintf(message + offset, sizeof(message) - offset,
         "\tPID: %d, Sekcja Krytyczna (i = %d), semafor = %d\n",
         getpid(), i, value);

         // Odczyt numer.txt
         int number = open("numer.txt", O_RDWR, 0644);
         CheckError(number != -1);
         char read_buf[32];
         ssize_t read_bytes = read(number, read_buf, sizeof(read_buf) - 1);
         CheckError(read_bytes != -1);
         read_buf[read_bytes] = '\0';
         CheckError(close(number) != -1);
         int current_value = atoi(read_buf);

         offset += snprintf(message + offset, sizeof(message) - offset,
         "\tPID: %d, Odczytano z numer.txt: %d\n", getpid(), current_value);
         // Losowy czas wewnątrz sekcji krytycznej
         sleep(rand() % 2 + 1);

         // Zapis do numer.txt
         current_value++;
         number = open("numer.txt", O_WRONLY | O_TRUNC, 0644);
         CheckError(number != -1);
         // Zmiana inta na char[]
         snprintf(read_buf, sizeof(read_buf), "%d\n", current_value);
         CheckError(write(number, read_buf, strlen(read_buf)) != -1);
         CheckError(close(number) != -1);

         //-------------------------------Wyjście z sekcji krytycznej---------------------------------------------
         // ------------------------------------------------------------------------------------------------------
         CheckError(my_sem_post(my_sem));
         CheckError(my_sem_get_value(my_sem, &value));
         offset += snprintf(message + offset, sizeof(message) - offset,
         "PID: %d, Po wyjściu z sekcji krytycznej (i = %d), semafor = %d\n\n",
         getpid(), i, value);

         // Wypisanie wiadomości
         fputs(message, stdout);
         fflush(stdout);
    }

         return 0;
}

