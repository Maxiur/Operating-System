//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 25.05.2025
//
// ĆWICZENIE 9 - Wzajemne wykluczanie dla wątków: Muteksy z kolorami
// Program demonstruje wzajemne wykluczanie wątków przy użyciu muteksu POSIX.
// Dodano kolorowanie sekcji prywatnych (zielony), krytycznych (czerwony) oraz
// czyszczenie linii przed wypisaniem, by uniknąć nakładania tekstu.
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
#include <errno.h>
#include <string.h>
#include <time.h>

//-------------------------------------------------------------
// Definicje stałych i makr
//-------------------------------------------------------------
#define RSLEEP_MIN      700   // minimalny czas uspienia w ms
#define RSLEEP_MAX      1000  // maksymalny czas uspienia w ms

// ANSI kolory i formatowanie
#define COLOR_RESET     "\033[0m"         // reset koloru
#define COLOR_PRIVATE   "\033[1;32m"     // zielony - sekcje prywatne
#define COLOR_CRITICAL  "\033[1;31m"     // czerwony - sekcje krytyczne
#define COLOR_DONE      "\033[1;34m"     // niebieski - koniec działania wątku
#define CLEAR_SCREEN()  printf("\033[2J")
#define MOVE_CURSOR(r,c) printf("\033[%d;%dH", (r),(c))
#define CLEAR_LINE()    printf("\033[K") // usuwa zawartość bieżącej linii od kursora

//-------------------------------------------------------------
// Zmienne globalne
//-------------------------------------------------------------
pthread_mutex_t muteks;         // muteks POSIX do ochrony sekcji krytycznej
long global_counter = 0;        // wspólny licznik inkrementowany w sekcji krytycznej
int THREAD_COUNT, SECTION_COUNT;

// Obsługa błędów
int my_error = 0;

//-------------------------------------------------------------
// Funkcja losowego uspienia wątku
//-------------------------------------------------------------
void rsleep(unsigned int *seed) {
    int ms = rand_r(seed) % (RSLEEP_MAX - RSLEEP_MIN + 1) + RSLEEP_MIN;
    usleep(ms * 1000);
}

//-------------------------------------------------------------
// Funkcja wykonywana przez każdy wątek
//-------------------------------------------------------------
void *thread_fn(void *arg) {
    int id = *(int*)arg;
    long local_counter_copy = 0;
    unsigned int seed = time(NULL) ^ id;
    for (int i = 1; i <= SECTION_COUNT; ++i) {
        //---------------------------------------------------------
        // Sekcja prywatna
        //  - zielony kolor
        //  - wyczyszczenie linii, wypisanie numeru wątku i sekcji
        //  - losowe uspienie
        //---------------------------------------------------------
        MOVE_CURSOR(id + 2, 1);
        CLEAR_LINE();
        printf(COLOR_PRIVATE "[Wątek %d] Prywatna sekcja #%d, global = %ld " COLOR_RESET, id, i, local_counter_copy);

        fflush(stdout);
        rsleep(&seed);

        //---------------------------------------------------------
        // Sekcja krytyczna
        //  - czerwony kolor
        //  - czyszczenie linii, blokada muteksu
        //  - inkrementacja licznika, wypisanie komunikatu
        //  - losowe uspienie
        //  - odblokowanie muteksu
        //---------------------------------------------------------

        if ((my_error = pthread_mutex_lock(&muteks)) != 0) {
            fprintf(stderr, "Wątek %d: błąd pthread_mutex_lock: %s\n", id, strerror(my_error));
            pthread_exit(NULL);
        }

        global_counter++;
        local_counter_copy = global_counter;

        MOVE_CURSOR(id + 2, 48);
        CLEAR_LINE();
        printf(COLOR_CRITICAL "[Wątek %d] Krytyczna sekcja #%d, global = %ld" COLOR_RESET, id, i, global_counter);
        fflush(stdout);
        rsleep(&seed);

        if ((my_error = pthread_mutex_unlock(&muteks)) != 0) {
            fprintf(stderr, "Wątek %d: błąd pthread_mutex_unlock: %s\n", id, strerror(my_error));
            pthread_exit(NULL);
        }

        //---------------------------------------------------------
        // Przywrócenie koloru prywatnego po krytycznej sekcji
        //---------------------------------------------------------
        MOVE_CURSOR(id + 2, 1);
        CLEAR_LINE();
        if(i == SECTION_COUNT)
            printf(COLOR_DONE "[Wątek %d] Po sekcji krytycznej #%d, global = %ld" COLOR_RESET, id, i, local_counter_copy);
        else
            printf(COLOR_PRIVATE "[Wątek %d] Po sekcji krytycznej #%d, global = %ld" COLOR_RESET, id, i, local_counter_copy);

        fflush(stdout);
        rsleep(&seed);
    }
    return NULL;
}

//-------------------------------------------------------------
// Funkcja główna
//-------------------------------------------------------------
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Poprawne wywołanie: %s <liczba_wątków> <liczba_sekcji>\n", argv[0]);
        return EXIT_FAILURE;
    }
    THREAD_COUNT = atoi(argv[1]);
    SECTION_COUNT = atoi(argv[2]);
    if (THREAD_COUNT <= 0 || SECTION_COUNT <= 0) {
        fprintf(stderr, "Argumenty muszą być dodatnie.\n");
        return EXIT_FAILURE;
    }

    // Przygotowanie ekranu
    CLEAR_SCREEN();
    fflush(stdout);

    // Inicjalizacja muteksu
    if ((my_error = pthread_mutex_init(&muteks, NULL)) != 0) {
        fprintf(stderr, "Błąd inicjalizacji muteksu: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Wypisanie informacji o muteksie i ID wątków
    MOVE_CURSOR(1, 1);
    printf("Adres muteksu: %p\n", (void*)&muteks);

    pthread_t *threads;
    int *args;

    if ((threads = malloc(sizeof(pthread_t) * THREAD_COUNT)) == NULL) {
        perror("Błąd alokacji pamięci dla threads");
        return EXIT_FAILURE;
    }
    if ((args = malloc(sizeof(int) * THREAD_COUNT)) == NULL) {
        perror("Błąd alokacji pamięci dla args");
        free(threads);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        args[i] = i;
        printf("Wątek %d, pthread_t ID: %lu\n", i, (unsigned long)threads[i]);
    }

    // Uruchomienie wątków
    for (int i = 0; i < THREAD_COUNT; ++i) {
        if((my_error = pthread_create(&threads[i], NULL, thread_fn, &args[i])) != 0) {
            fprintf(stderr, "Błąd tworzenia wątku %d: %s\n", i, strerror(my_error));
            exit(EXIT_FAILURE);
        }
    }
    // Oczekiwanie na zakończenie wątków
    for (int i = 0; i < THREAD_COUNT; ++i) {
        if ((my_error = pthread_join(threads[i], NULL)) != 0) {
            fprintf(stderr, "Błąd dołączania wątku %d: %s\n", i, strerror(my_error));
            // Idziemy dalej pomimo tego
        }
    }

    // Niszczenie muteksu
    if (pthread_mutex_destroy(&muteks) != 0) {
        fprintf(stderr, "Błąd niszczenia muteksu: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Wypisanie wyniku końcowego
    MOVE_CURSOR(THREAD_COUNT + 4, 1);
    long expected = THREAD_COUNT * SECTION_COUNT;
    printf("\nWszystkie wątki zakończone.\n");
    printf("Licznik globalny = %ld, oczekiwana wartość = %ld\n", global_counter, expected);

    return 0;
}