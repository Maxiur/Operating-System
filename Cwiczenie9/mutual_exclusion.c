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
#define COLOR_PRIVATE   "\033[1;32m"     // jasna zieleń - sekcje prywatne
#define COLOR_CRITICAL  "\033[1;31m"     // jasnoczerwony - sekcje krytyczne
#define CLEAR_SCREEN()  printf("\033[2J")
#define MOVE_CURSOR(r,c) printf("\033[%d;%dH", (r),(c))
#define CLEAR_LINE()    printf("\033[K") // usuwa zawartość bieżącej linii od kursora

//-------------------------------------------------------------
// Zmienne globalne
//-------------------------------------------------------------
pthread_mutex_t muteks;         // muteks POSIX do ochrony sekcji krytycznej
long global_counter = 0;        // wspólny licznik inkrementowany w sekcji krytycznej
int THREAD_COUNT, SECTION_COUNT;

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
        printf(COLOR_PRIVATE
               "[Wątek %d] Prywatna sekcja #%d, global = %ld"
               COLOR_RESET,
               id, i, global_counter);
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
        pthread_mutex_lock(&muteks);
        global_counter++;
        MOVE_CURSOR(id + 2, 40);
        CLEAR_LINE();
        printf(COLOR_CRITICAL
               "[Wątek %d] Krytyczna sekcja #%d, global = %ld"
               COLOR_RESET,
               id, i, global_counter);
        fflush(stdout);
        rsleep(&seed);
        pthread_mutex_unlock(&muteks);

        //---------------------------------------------------------
        // Przywrócenie koloru prywatnego po krytycznej sekcji
        //---------------------------------------------------------
        MOVE_CURSOR(id + 2, 1);
        CLEAR_LINE();
        printf(COLOR_PRIVATE
               "[Wątek %d] Po sekcji krytycznej #%d"
               COLOR_RESET,
               id, i);
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
        fprintf(stderr, "Użycie: %s <liczba_wątków> <liczba_sekcji>\n", argv[0]);
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

    // Inicjalizacja muteksu
    if (pthread_mutex_init(&muteks, NULL) != 0) {
        fprintf(stderr, "Błąd inicjalizacji muteksu: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Wypisanie informacji o muteksie i ID wątków
    MOVE_CURSOR(1, 1);
    printf("Adres muteksu: %p\n", (void*)&muteks);
    pthread_t *threads = malloc(sizeof(pthread_t) * THREAD_COUNT);
    int *args = malloc(sizeof(int) * THREAD_COUNT);
    for (int i = 0; i < THREAD_COUNT; ++i) {
        args[i] = i;
        printf("Wątek %d, pthread_t ID: %lu\n",
               i, (unsigned long)threads[i]);
    }

    // Uruchomienie wątków
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_create(&threads[i], NULL, thread_fn, &args[i]);
    }
    // Oczekiwanie na zakończenie wątków
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Niszczenie muteksu
    if (pthread_mutex_destroy(&muteks) != 0) {
        fprintf(stderr, "Błąd niszczenia muteksu: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Wypisanie wyniku końcowego
    MOVE_CURSOR(THREAD_COUNT + 4, 1);
    long expected = (long)THREAD_COUNT * SECTION_COUNT;
    printf("\nWszystkie wątki zakończone.\n");
    printf("Licznik globalny = %ld, oczekiwana wartość = %ld\n",
           global_counter, expected);

    return 0;
}