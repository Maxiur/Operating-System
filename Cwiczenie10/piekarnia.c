//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 28.05.2025
//
// ĆWICZENIE 10 - Wzajemne wykluczanie dla wątków: Algorytm piekarni z kolorami
// Program demonstruje wzajemne wykluczanie wątków przy użyciu algorytmu piekarni.
// Dodano kolorowanie sekcji prywatnych (zielony), krytycznych (czerwony) oraz
// czyszczenie linii przed wypisaniem, by uniknąć nakładania tekstu.
// Program działa analogicznie wizualnie do Ćwiczenia 9 (muteksy).
//
// Parametry wywołania:
//   <liczba_wątków>    liczba tworzonych wątków w programie
//   <liczba_sekcji>    liczba sekcji prywatnych i krytycznych na wątek
//
// Przykładowe wywołanie:
//   make run THREADS=4 SECTIONS=5
//-------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define COLOR_RESET     "\033[0m"
#define COLOR_PRIVATE   "\033[1;32m"
#define COLOR_CRITICAL  "\033[1;31m"
#define COLOR_DONE      "\033[1;34m"
#define CLEAR_SCREEN()  printf("\033[2J")
#define MOVE_CURSOR(r,c) printf("\033[%d;%dH", (r),(c))
#define CLEAR_LINE()    printf("\033[K")

#define RSLEEP_MIN      200
#define RSLEEP_MAX      500

int THREAD_COUNT, SECTION_COUNT;
long global_counter = 0;
int *choosing;
int *number;

void rsleep(unsigned int *seed) {
    int ms = rand_r(seed) % (RSLEEP_MAX - RSLEEP_MIN + 1) + RSLEEP_MIN;
    usleep(ms * 1000);
}

void *thread_fn(void *arg) {
    int id = *(int *)arg;
    unsigned int seed = time(NULL) ^ id;
    long local_copy = 0;

    for (int i = 1; i <= SECTION_COUNT; ++i) {
        // Sekcja prywatna po lewej
        MOVE_CURSOR(id + 2, 1);
        CLEAR_LINE();
        printf(COLOR_PRIVATE "[Wątek %d] Prywatna sekcja #%d, global = %ld" COLOR_RESET, id, i, local_copy);
        fflush(stdout);
        rsleep(&seed);

        choosing[id] = 1;
        int max = 0;
        for (int j = 0; j < THREAD_COUNT; j++) {
            if (number[j] > max) max = number[j];
        }
        number[id] = max + 1;
        choosing[id] = 0;

        for (int j = 0; j < THREAD_COUNT; j++) {
            while (choosing[j]);
            while (number[j] != 0 && (number[j] < number[id] || (number[j] == number[id] && j < id)));
        }

        // Pusta linia z lewej
        MOVE_CURSOR(id + 2, 1);
        CLEAR_LINE();
        fflush(stdout);
        rsleep(&seed);

        // Sekcja krytyczna po prawej
        MOVE_CURSOR(id + 2, 48);
        CLEAR_LINE();
        global_counter++;
        local_copy = global_counter;
        printf(COLOR_CRITICAL "[Wątek %d] Krytyczna sekcja #%d, global = %ld" COLOR_RESET, id, i, global_counter);
        fflush(stdout);
        usleep(700 * 1000);  // dłużej zatrzymana sekcja krytyczna

        number[id] = 0;

        // Powrót do lewej
        MOVE_CURSOR(id + 2, 1);
        CLEAR_LINE();
        if (i == SECTION_COUNT)
            printf(COLOR_DONE "[Wątek %d] Po sekcji krytycznej #%d, global = %ld" COLOR_RESET, id, i, local_copy);
        else
            printf(COLOR_PRIVATE "[Wątek %d] Po sekcji krytycznej #%d, global = %ld" COLOR_RESET, id, i, local_copy);

        fflush(stdout);
        rsleep(&seed);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Użycie: %s <liczba_wątków> <liczba_sekcji>\n", argv[0]);
        return EXIT_FAILURE;
    }
    THREAD_COUNT = atoi(argv[1]);
    SECTION_COUNT = atoi(argv[2]);
    if (THREAD_COUNT <= 0 || SECTION_COUNT <= 0) {
        fprintf(stderr, "Argumenty muszą być dodatnie.\n");
        return EXIT_FAILURE;
    }

    CLEAR_SCREEN();
    fflush(stdout);

    choosing = calloc(THREAD_COUNT, sizeof(int));
    number = calloc(THREAD_COUNT, sizeof(int));
    if (!choosing || !number) {
        perror("Błąd alokacji pamięci");
        return EXIT_FAILURE;
    }

    pthread_t *threads = calloc(THREAD_COUNT, sizeof(pthread_t));
    int *args = calloc(THREAD_COUNT, sizeof(int));
    if (!threads || !args) {
        perror("Błąd alokacji pamięci dla wątków");
        return EXIT_FAILURE;
    }

    MOVE_CURSOR(1, 1);
    printf("Algorytm piekarni – start.\n");

    for (int i = 0; i < THREAD_COUNT; ++i) {
        args[i] = i;
        printf("Wątek %d, pthread_t ID: %lu\n", i, (unsigned long)threads[i]);
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        if (pthread_create(&threads[i], NULL, thread_fn, &args[i]) != 0) {
            perror("Błąd tworzenia wątku");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Błąd dołączania wątku");
        }
    }

    MOVE_CURSOR(THREAD_COUNT + 4, 1);
    long expected = THREAD_COUNT * SECTION_COUNT;
    printf("\nWszystkie wątki zakończone.\n");
    printf("Licznik globalny = %ld, oczekiwana wartość = %ld\n", global_counter, expected);

    return 0;
}