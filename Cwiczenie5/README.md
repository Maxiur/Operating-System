Autor: Mateusz Kamiński  
Kraków, 05.04.2025

## Polecenie:

ĆWICZENIE 5: Producent–Konsument: Potoki Nazwane  
Przy pomocy **potoków nazwanych** systemu UNIX zaimplementować problem „Producenta i Konsumenta” analogicznie jak w
ćwiczeniu 4.

### Wersja (a):

- Potok FIFO należy utworzyć **z poziomu programu** (funkcja `mkfifo()`), a następnie uruchomić Producenta i Konsumenta
  w tym samym programie (np. w procesie macierzystym i potomnym).
- Potok powinien być **usuwany automatycznie** w funkcji zarejestrowanej za pomocą `atexit()`.

Dla zademonstrowania, że nie doszło do utraty ani zwielokrotnienia towaru, Producent pobiera dane z pliku wejściowego i
przesyła je przez FIFO, natomiast Konsument odbiera dane i zapisuje do pliku wyjściowego.  
Po zakończeniu działania programów oba pliki powinny być **identyczne** (można to sprawdzić za pomocą `diff -s`).  
W celu zasymulowania różnych prędkości działania programów, należy użyć funkcji `sleep()` z losowym czasem.  
Do operacji wejścia/wyjścia używamy funkcji `read()` i `write()`.

### Wersja (b):

- Potok FIFO należy utworzyć **z poziomu powłoki** (`mkfifo`) lub w regule `Makefile`.
- Programy `producent` (`write_fifo`) i `konsument` (`read_fifo`) należy uruchomić jako **niezależne procesy**, np. w
  osobnych terminalach z użyciem `xterm`.
- Producent i konsument powinny komunikować się przez wcześniej utworzony FIFO, a na koniec należy **sprawdzić zgodność
  ** danych za pomocą `diff -s`.
- Po zakończeniu działania FIFO powinien zostać usunięty (np. przez `make clean`).

## Zawartość katalogu:

W katalogu znajdują się:

- `5a.c` – **program główny**: tworzy FIFO, uruchamia producenta i konsumenta,
  `write_fifo.c` – **Producent**: odczytuje dane z pliku i przesyła przez FIFO,
- `read_fifo.c` – **Konsument**: odbiera dane z FIFO i zapisuje do pliku,
- `Makefile` – plik do kompilacji, uruchamiania i czyszczenia projektu,
- `input.txt` – przykładowy plik wejściowy,
- `output.txt` – generowany plik wyjściowy przez konsumenta (do porównania).

## Kompilacja:

Aby skompilować wszystkie programy:
Dostępne cele pomocnicze:

- `make` - Kompiluje wszystkie programy (5a, write_fifo, read_fifo)
- `make run` - Uruchamia wersję (a): 5a tworzy FIFO, uruchamia producenta i konsumenta, a na końcu porównuje pliki
- `make run_b` - Uruchamia wersję (b): tworzy FIFO, uruchamia osobno write_fifo i read_fifo w xterm, a następnie
  wykonuje diff -s
- `make diff` - Porównuje pliki input.txt i output.txt przy pomocy diff -s
- `make mkfifo` - Tworzy potok nazwany mojfifo w katalogu projektu
- `make clean` - Usuwa pliki wynikowe oraz katalog temp/
- `make tar` - Pakuje cały katalog projektu do nazwa_katalogu.tar.gz
- `make help` - Wyświetla listę dostępnych komend

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

