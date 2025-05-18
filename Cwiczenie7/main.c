/*
============================================================================
 Projekt:      Cwiczenie 7 – Producent-Konsument (pamięć dzielona + semafory)
 Autor:        Mateusz Kamiński
 Data:         14 maja 2025

 Opis:
 Program główny tworzy pamięć dzieloną i semafory, a następnie uruchamia
 dwa procesy: producenta i konsumenta. Komunikacja odbywa się poprzez
 bufor cykliczny w pamięci dzielonej synchronizowany semaforami POSIX.

 Kompilacja i uruchomienie:

 1. Wersja statyczna:
    $ make run_static

 2. Wersja dynamiczna (shared library):
    $ make run_shared

 ============================================================================
*/

#include "./include/CheckError.h"
#include "./include/my_semaphore.h"
#include "./include/shared_memory.h"
#include "./include/shared_buffer.h"

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

int main(int argc, char* argv[]) {
    atexit(cleanup);

	if (argc < 5) {
        fprintf(stderr, "Poprawne wywołanie: %s <producent> <konsument> <input> <output>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char* PRODUCENT_PATH = argv[1];
    const char* KONSUMENT_PATH = argv[2];
    const char* INPUT_FILE = argv[3];
    const char* OUTPUT_FILE = argv[4];

    // Zmienne początkowe
    void* addr = NULL;
    int shm_fd = -1;
    sem_t* sem_write = NULL;
    sem_t* sem_read = NULL;

    // Ustawienie obsługi sygnału SIGINT
    CheckError(signal(SIGINT, signal_handler) != SIG_ERR);

    // Utworzenie pamięci dzielonej
    CheckError((shm_fd = create_shared_memory(SHM_NAME, SHM_SIZE)));
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
            execlp(PRODUCENT_PATH, PRODUCENT_PATH, INPUT_FILE, (char*)NULL);
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
            execlp(KONSUMENT_PATH, KONSUMENT_PATH, OUTPUT_FILE, (char*)NULL);
            perror("execlp error");
            exit(EXIT_FAILURE);

        default:
            break;
    }

    // Oczekiwanie na zakończenie obu procesów
   	for(int i = 0; i < 2; i++) {
		if(wait(NULL) == -1) {
			perror("wait error");
			exit(EXIT_FAILURE);
		}
	}

    // atexit()

    return 0;
}