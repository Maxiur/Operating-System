#include "../include/my_semaphore.h"
#include "../include/shared_memory.h"
#include "../include/shared_buffer.h"

#define PRODUCENT_NAME "./producent"
#define KONSUMENT_NAME "./konsument"

void cleanup() {
    CheckError(my_sem_unlink(SEM_WRITE_TO_SHM));
    CheckError(my_sem_unlink(SEM_READ_FROM_SHM));
    CheckError(unlink_shared_memory(SHM_NAME));
}

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nOtrzymano sygnał SIGINT. Kończę program i usuwam zasoby.\n");
        exit(0); // cleanup()
    }
}

int main() {
    atexit(cleanup);

    // Zmienne początkowe
    void* addr = NULL;
    int shm_fd = -1;
    sem_t* sem_write = NULL;
    sem_t* sem_read = NULL;

    // Ustawienie obsługi sygnału SIGINT
    CheckError(signal(SIGINT, signal_handler) != SIG_ERR);

    // Utworzenie pamięci dzielonej
    CheckError((shm_fd = create_shared_memory(SHM_NAME, SHM_SIZE)) != -1);
    CheckError((addr = map_shared_memory(shm_fd, SHM_SIZE)) != NULL);

    // Inicjalizacja wspólnego bufora
    SegmentPD* buffer = (SegmentPD*)addr;
    buffer->wstaw = 0;
    buffer->wyjmij = 0;

    // Tworzenie semaforów
    CheckError((sem_write = my_sem_init(SEM_WRITE_TO_SHM, NBUF)) != NULL);
    CheckError((sem_read = my_sem_init(SEM_READ_FROM_SHM, 0)) != NULL);

    // Tworzenie procesów producenta i konsumenta
    // --> Producent
    switch(fork()) {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);

        case 0:
            execlp(PRODUCENT_NAME, PRODUCENT_NAME, (char*)NULL);
            perror("execlp error");
            exit(EXIT_FAILURE);

        default:
            break;
    }
    // --> Konsument
    switch(fork()) {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);

        case 0:
            execlp(KONSUMENT_NAME, KONSUMENT_NAME, (char*)NULL);
            perror("execlp error");
            exit(EXIT_FAILURE);

        default:
            break;
    }

    // Oczekiwanie na zakończenie obu procesów
    wait(NULL);
    wait(NULL);

    // atexit()

    return 0;
}