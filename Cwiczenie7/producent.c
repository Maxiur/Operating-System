/*
============================================================================
 Plik:         producent.c
 Projekt:      Ćwiczenie 7 – Producent-Konsument (pamięć dzielona + semafory)
 Autor:        Mateusz Kamiński
 Data:         14 maja 2025

 Opis:
 Proces producenta odczytuje dane linia po linii z pliku tekstowego
 przekazanego jako argument, a następnie zapisuje je do bufora cyklicznego
 znajdującego się w pamięci dzielonej. Synchronizacja odbywa się z użyciem
 semaforów nazwanych POSIX. Na koniec wysyła komunikat "__END__"
 oznaczający koniec transmisji.
============================================================================
*/

#include "./include/CheckError.h"
#include "./include/my_semaphore.h"
#include "./include/shared_memory.h"
#include "./include/shared_buffer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Poprawne wywołanie: %s <plik_wejściowy>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Otwarcie pliku
    FILE* input = fopen(argv[1], "r");
    CheckError(input != NULL);

    // Pamięć dzielona
    int shm_fd;
    void* addr;
    SegmentPD* buffer;

    CheckError((shm_fd = open_shared_memory(SHM_NAME, SHM_SIZE)));
    CheckError((addr = map_shared_memory(shm_fd, SHM_SIZE)) != NULL);
    buffer = (SegmentPD*)addr;

    // Semafory
    sem_t* sem_write = my_sem_open(SEM_WRITE_TO_SHM);
    CheckError(sem_write != NULL);
    sem_t* sem_read = my_sem_open(SEM_READ_FROM_SHM);
    CheckError(sem_read != NULL);

    // Wysyłanie danych
    Towar towar;

    while (fgets(towar.element, NELE, input) != NULL) {

        // Czekanie na wolne miejsce w buforze
        CheckError(my_sem_wait(sem_write));

        // Wstawianie towaru do bufora
        buffer->bufor[buffer->wstaw] = towar;
        printf("\033[32m[PRODUCENT] Wstawiam:\033[0m %s (indeks %d)\n", towar.element, buffer->wstaw);

         // Przesunięcie wskaźnika wstawiania
        buffer->wstaw = (buffer->wstaw + 1) % NBUF;

        // Powiadomienie do Konsumenta
        CheckError(my_sem_post(sem_read));

        sleep(1); // symulacja pracy
    }

    // Wstawienie komunikatu końca transmisji
    strncpy(towar.element, "__END__", NELE);
    CheckError(my_sem_wait(sem_write));
    buffer->bufor[buffer->wstaw] = towar;
    printf("\033[32m[PRODUCENT] Wstawiam:\033[0m __END__ (indeks %d)\n", buffer->wstaw);
    buffer->wstaw = (buffer->wstaw + 1) % NBUF;
    CheckError(my_sem_post(sem_read));

    // Sprzątanie
    if (fclose(input) == EOF) {
        perror("fclose error");
        exit(EXIT_FAILURE);
    }
    CheckError(my_sem_close(sem_write));
    CheckError(my_sem_close(sem_read));
    CheckError(unmap_shared_memory(addr, SHM_SIZE));
    CheckError(close_shared_memory(shm_fd));

    return 0;
}
