#include "../include/my_semaphore.h"
#include "../include/shared_memory.h"
#include "../include/shared_buffer.h"

int main() {
    // Otwarcie pamięci dzielonej
    int shm_fd;
    void *addr;
    SegmentPD *buffer;

    CheckError((shm_fd = open_shared_memory(SHM_NAME, SHM_SIZE)) != -1);
    CheckError((addr = map_shared_memory(shm_fd, SHM_SIZE)) != NULL);
    buffer = (SegmentPD *)addr;

    // Otwarcie semaforów
    sem_t* sem_write = my_sem_open(SEM_WRITE_TO_SHM);
    CheckError(sem_write != NULL);
    sem_t* sem_read = my_sem_open(SEM_READ_FROM_SHM);
    CheckError(sem_read != NULL);

    // Dane do wyjmowania
    Towar towar;
    int counter = 1;

    while (counter <= ILE_TOWAROW) {
        // Czekamy na dostępne dane
        CheckError(my_sem_wait(sem_read));

        // Pobieramy dane z bufora
        towar = buffer->bufor[buffer->wyjmij];
        printf("\033[34m[KONSUMENT] Odbieram:\033[0m %s (indeks %d)\n", towar.element, buffer->wyjmij);

        // Przesuwamy indeks
        buffer->wyjmij = (buffer->wyjmij + 1) % NBUF;

        // Powiadamiamy producenta o wolnym miejscu
        CheckError(my_sem_post(sem_write));

        counter++;
        sleep(1); // symulacja konsumpcji
    }

    // Sprzątanie
    CheckError(my_sem_close(sem_write));
    CheckError(my_sem_close(sem_read));
    unmap_shared_memory(buffer, SHM_SIZE);
    close_shared_memory(shm_fd);

    return 0;
}