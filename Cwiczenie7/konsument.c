/*
============================================================================
 Plik:         konsument.c
 Projekt:      Ćwiczenie 7 – Producent-Konsument (pamięć dzielona + semafory)
 Autor:        Mateusz Kamiński
 Data:         14 maja 2025

 Opis:
 Proces konsumenta odbiera dane z bufora cyklicznego w pamięci dzielonej
 i zapisuje je do pliku wyjściowego przekazanego jako argument. Dodatkowo
 wypisuje odbierane dane na ekran. Synchronizacja odbywa się z użyciem
 semaforów nazwanych POSIX. Transmisja kończy się po odebraniu komunikatu "__END__".

 ============================================================================
*/

#include "./include/CheckError.h"
#include "./include/my_semaphore.h"
#include "./include/shared_memory.h"
#include "./include/shared_buffer.h"
#include <string.h>

int main(int argc, char* argv[]) {

    // Sprawdzenie argumentów
    if (argc < 2) {
        fprintf(stderr, "Poprawne wywołanie: %s <plik_wyjściowy>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Otwarcie pliku do zapisu
    FILE* output = fopen(argv[1], "w");
    CheckError(output != NULL);

    // Otwarcie pamięci dzielonej
    int shm_fd;
    void *addr;
    SegmentPD *buffer;

    CheckError((shm_fd = open_shared_memory(SHM_NAME, SHM_SIZE)));
    CheckError((addr = map_shared_memory(shm_fd, SHM_SIZE)) != NULL);
    buffer = (SegmentPD *)addr;

    // Otwarcie semaforów
    sem_t* sem_write = my_sem_open(SEM_WRITE_TO_SHM);
    CheckError(sem_write != NULL);
    sem_t* sem_read = my_sem_open(SEM_READ_FROM_SHM);
    CheckError(sem_read != NULL);

    // Dane do wyjmowania
    Towar towar;

    while (1) {
        // Czekamy na dostępne dane
        CheckError(my_sem_wait(sem_read));

        // Pobieramy dane z bufora
        towar = buffer->bufor[buffer->wyjmij];

        // Sprawdzenie końca transmisji
        if (strncmp(towar.element, "__END__", NELE) == 0) {
            printf("\033[34m[KONSUMENT] Otrzymano sygnał zakończenia.\033[0m\n");
            CheckError(my_sem_post(sem_write));
            break;
        }

        printf("\033[34m[KONSUMENT] Odbieram:\033[0m %s (indeks %d)\n", towar.element, buffer->wyjmij);

        // Zapisujemy dane do pliku
        fprintf(output, "%s", towar.element);

        // Przesuwamy indeks
        buffer->wyjmij = (buffer->wyjmij + 1) % NBUF;

        // Powiadamiamy producenta o wolnym miejscu
        CheckError(my_sem_post(sem_write));

        sleep(1); // symulacja konsumpcji
    }

    // Sprzątanie
    if (fclose(output) == EOF) {
        perror("fclose error");
        exit(EXIT_FAILURE);
    }
    CheckError(my_sem_close(sem_write));
    CheckError(my_sem_close(sem_read));
    CheckError(unmap_shared_memory(buffer, SHM_SIZE));
    CheckError(close_shared_memory(shm_fd));

    return 0;
}
