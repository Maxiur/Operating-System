Autor: Mateusz Kamiński  
Kraków, 05.04.2025

## Polecenie:

ĆWICZENIE 5a: Producent–Konsument: Potoki Nazwane  
Przy pomocy **potoków nazwanych** systemu UNIX zaimplementować problem „Producenta i Konsumenta” analogicznie jak w
ćwiczeniu 4.

Wersja (a):

- Potok FIFO należy utworzyć **z poziomu programu** (funkcja `mkfifo()`), a następnie uruchomić Producenta i Konsumenta
  w tym samym programie (np. w procesie macierzystym i potomnym).
- Potok powinien być **usuwany automatycznie** w funkcji zarejestrowanej za pomocą `atexit()`.

Dla zademonstrowania, że nie doszło do utraty ani zwielokrotnienia towaru, Producent pobiera dane z pliku wejściowego i
przesyła je przez FIFO, natomiast Konsument odbiera dane i zapisuje do pliku wyjściowego.  
Po zakończeniu działania programów oba pliki powinny być **identyczne** (można to sprawdzić za pomocą `diff -s`).  
W celu zasymulowania różnych prędkości działania programów, należy użyć funkcji `sleep()` z losowym czasem.  
Do operacji wejścia/wyjścia używamy funkcji `read()` i `write()`.

## Zawartość katalogu:

W katalogu znajdują się:

- `5a.c` – program główny: tworzy FIFO, uruchamia producenta i konsumenta,
- `write_fifo.c` – producent: odczytuje dane z pliku i przesyła przez FIFO,
- `read_fifo.c` – konsument: odbiera dane z FIFO i zapisuje do pliku,
- `Makefile` – plik do kompilacji i uruchomienia,
- `input.txt` – plik z danymi wejściowymi.

## Kompilacja:

Aby skompilować wszystkie programy:
Dostępne cele pomocnicze:

- `make run` – uruchomienie programu z domyślnymi argumentami (`input.txt output.txt`)
  sprawdzenie zgodności plików wejściowego i wyjściowego (`diff -s`)
- `make clean` – usunięcie plików tymczasowych
- `make tar` – spakowanie katalogu do `cwiczenie5.tar.gz`
- `make help` – wyświetlenie dostępnych komend

## Schemat działania programu:

                 +------------------+
                 |    input.txt     |
                 +--------+---------+
                          |
                          v
                 +--------+---------+
                 |     Producent     |
                 |   (write_fifo.c)  |
                 +--------+---------+
                          |
                       [ FIFO ]
                          |
                          v
                 +--------+---------+
                 |     Konsument     |
                 |   (read_fifo.c)   |
                 +--------+---------+
                          |
                          v
                 +------------------+
                 |   output.txt     |
                 +------------------+

