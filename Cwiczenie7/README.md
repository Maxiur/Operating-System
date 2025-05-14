# Cwiczenie 7 – Producent-Konsument z użyciem pamięci dzielonej i semaforów POSIX

## Opis

Program realizuje klasyczny problem **producenta i konsumenta** przy użyciu:

- pamięci dzielonej POSIX (`shm_open`, `mmap`)
- semaforów nazwanych POSIX (`sem_open`, `sem_wait`, `sem_post`)
- bufora cyklicznego

Wersje programu dostępne są zarówno z **biblioteką statyczną**, jak i **dynamiczną**.

## Struktura projektu

```
Cwiczenie7/
├── include/           # Pliki nagłówkowe
│   ├── my_semaphore.h
│   ├── shared_buffer.h
│   └── shared_memory.h
├── source/            # Pliki źródłowe do bibliotek
│   ├── semaphore.c
│   └── shared_memory.c
├── lib/               # Wygenerowane biblioteki .a i .so
├── main.c             # Program główny uruchamiający producenta i konsumenta
├── producent.c        # Producent
├── konsument.c        # Konsument
├── Makefile           # Plik make do kompilacji
├── input.txt          # Dane wejściowe (opcjonalne)
```

## Kompilacja

```bash
make            # kompiluje wersje statyczną i dynamiczną
make static     # tylko wersja statyczna
make shared     # tylko wersja dynamiczna
```

## Uruchomienie

```bash
make run_static     # uruchomienie programu z biblioteką statyczną
make run_shared     # uruchomienie programu z biblioteką dynamiczną
```

## Czyszczenie

```bash
make clean
```

## Archiwizacja

```bash
make tar     # tworzy archiwum Cwiczenie7.tar.gz
```

## Pomoc

```bash
make help
```

---

Projekt zakłada utworzenie bibliotek statycznych i dynamicznych w katalogu `./lib`. Pliki są automatycznie tworzone
podczas budowania projektu.
