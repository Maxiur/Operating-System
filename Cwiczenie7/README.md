# Ćwiczenie 7 – Producent-Konsument z użyciem pamięci dzielonej i semaforów POSIX

**Autor:** Mateusz Kamiński
**Data:** 14 maja 2025

---

## Opis

Program realizuje klasyczny problem **producenta i konsumenta** z wykorzystaniem:

- pamięci dzielonej POSIX (`shm_open`, `mmap`)
- semaforów nazwanych POSIX (`sem_open`, `sem_wait`, `sem_post`)
- bufora cyklicznego do komunikacji między procesami

Wersje programu dostępne są z użyciem **bibliotek statycznych** oraz **dynamicznych** w dwóch wariantach: `rpath` oraz
`LD_LIBRARY_PATH`.

---

## Struktura projektu

```
Cwiczenie7/
├── include/           # Pliki nagłówkowe
│   ├── my_semaphore.h
│   ├── shared_buffer.h
│   ├── shared_memory.h
│   └── CheckError.h
├── source/            # Źródła do bibliotek
│   ├── semaphore.c
│   └── shared_memory.c
├── lib/               # Wygenerowane biblioteki statyczne i dynamiczne
├── main.c             # Program główny (tworzy segmenty i uruchamia producenta i konsumenta)
├── producent.c        # Producent – zapisuje dane do pamięci dzielonej
├── konsument.c        # Konsument – odbiera dane i zapisuje do pliku
├── input.txt          # Przykładowe dane wejściowe
├── output.txt         # Plik wyjściowy tworzony przez konsumenta
├── Makefile           # Kompilacja i uruchamianie
```

---

## Kompilacja

```bash
make            # kompiluje wersje statyczną i dynamiczną
make static     # tylko wersja statyczna
make shared     # tylko wersja dynamiczna (shared1 i shared2)
```

---

## Uruchamianie

```bash
make run_static      # uruchomienie programu z biblioteką statyczną
make run_shared1     # uruchomienie z biblioteką dynamiczną (z rpath)
make run_shared2     # uruchomienie z biblioteką dynamiczną (LD_LIBRARY_PATH)
```

Każde uruchomienie porównuje dane z `input.txt` i `output.txt`.

---

## Czyszczenie

```bash
make clean
```

Usuwa pliki `.o`, `.a`, `.so`, pliki binarne i `output.txt`.

---

## Archiwizacja

```bash
make tar
```

Tworzy archiwum `Cwiczenie7.tar.gz` katalogu projektu.

---

## Pomoc

```bash
make help
```

Wyświetla listę dostępnych komend `make`.

---

## Uwagi

- Program wymaga systemu z obsługą POSIX (`Linux`, `macOS`).
- Plik `input.txt` może być podmieniany – ścieżki można zmieniać przez zmienne `make`, np.:
  ```bash
  make run_static INPUT_FILE=dane.txt OUTPUT_FILE=wynik.txt
  ```
