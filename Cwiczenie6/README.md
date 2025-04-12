# Autor: Mateusz Kamiński

Kraków, 12.04.2025

---

## Polecenie:

**ĆWICZENIE 6: Wzajemne wykluczanie dla procesów – Semafory POSIX**

Przy pomocy **semaforów nazwanych POSIX** zaimplementować wzajemne wykluczanie procesów zgodnie z pseudokodem z
podrozdziału 5.1.  
Program powinien demonstrować poprawność synchronizacji poprzez odpowiednie komunikaty oraz manipulację wspólnym
zasobem (`numer.txt`).

### Co ma robić:

- Procesy realizujące sekcje krytyczne uruchamiane są przez **program powielający** (`duplicator.c`) przy użyciu
  `fork()` i `execlp()`.
- Program `duplicator`:
    - Tworzy semafor nazwany z flagą `O_EXCL` i wartością początkową `1`,
    - Tworzy plik `numer.txt` z początkową wartością `0`,
    - Uruchamia podaną liczbę procesów `critical_worker`,
    - Po ich zakończeniu odczytuje końcową wartość i porównuje z oczekiwaną.
- Program `critical_worker`:
    - W każdej iteracji:
        - wypisuje PID i stan semafora **przed sekcją krytyczną** (sekcja prywatna),
        - przechodzi do sekcji krytycznej po `sem_wait`,
        - odczytuje i zwiększa wartość w `numer.txt`,
        - wypisuje wartość i PID w sekcji krytycznej,
        - zapisuje zaktualizowaną wartość i wychodzi z sekcji,
        - wypisuje stan po `sem_post`.

### Uwaga:

- W `duplicator` semafor powinien być **usuwany** po zakończeniu programu (zarejestrowany `atexit()`).
- Należy również dodać **obsługę sygnału SIGINT** (np. `Ctrl+C`) w celu poprawnego usuwania semafora.
- W programie `worker` należy używać **niskopoziomowych funkcji** (`read`, `write`, `open`, `close`).
- Synchronizacja z `sem_t` i wypisywanie `sem_getvalue()`.

---

## Zawartość katalogu:

W katalogu znajdują się:

- `duplicator.c` – program główny, uruchamia procesy i tworzy semafor oraz plik `numer.txt`,
- `critical_worker.c` – procesy wykonujące wzajemne wykluczanie i operacje na pliku,
- `source/my_semaphore.c` – biblioteka własnych funkcji operujących na semaforze POSIX z obsługą błędów,
- `include/my_semaphore.h` – nagłówek z funkcjami i makrem `CheckError`,
- `numer.txt` – plik zawierający aktualną wartość, modyfikowany tylko w sekcji krytycznej,
- `Makefile` – plik kompilacji i uruchomienia projektu.

---

## Kompilacja:

- `make` – kompiluje programy `duplicator` i `critical_worker` oraz bibliotekę semafora,
- `make run` – uruchamia `duplicator`, który uruchamia `n` workerów wykonujących `k` sekcji krytycznych  
  *(domyślnie: 5 i 10, ale można nadpisać np. `make run PROCESS=4 SEKCJE=3`)*,
- `make clean` – usuwa pliki `.o`, programy i `numer.txt`,
- `make tar` – pakuje cały katalog projektu do `nazwa_katalogu.tar.gz`,
- `make help` – wyświetla listę dostępnych komend.
