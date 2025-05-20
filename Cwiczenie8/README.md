# Kolejki Komunikatów POSIX – Klient–Serwer

Projekt w języku C realizujący komunikację międzyprocesową typu **klient–serwer** z wykorzystaniem **kolejek komunikatów
POSIX** (`mqueue.h`).

## Struktura projektu

```
.
├── klient.c                # Program klienta
├── serwer.c                # Program serwera
├── source/
│   └── my_mqueue.c         # Funkcje pomocnicze do obsługi mqueue z CheckError
├── include/
│   ├── CheckError.h        # Makro CheckError
│   ├── common.h            # Wspólne definicje (np. rozmiary, nazwy kolejek)
│   └── my_mqueue.h         # Interfejs do my_mqueue.c
├── Makefile
└── README.md
```

## Kompilacja

```bash
make
```

Zbuduje dwa pliki wykonywalne: `klient` oraz `serwer`.

## Uruchomienie

```bash
make run
```

Otworzy dwa terminale `xterm`:

- serwer (w trybie nasłuchiwania)
- klient (zachęta do wpisania wyrażenia)

**Wymagane:** `xterm` zainstalowany w systemie (np. `sudo apt install xterm`)

## Działanie programu

- Serwer tworzy kolejkę `/server_queue` i oczekuje na wiadomości od klientów.
- Klient tworzy unikalną kolejkę `/PID` (gdzie `PID` to identyfikator procesu).
- Klient wysyła do serwera wiadomość w formacie:

  ```
  <PID> <wyrażenie>
  ```

  np.

  ```
  12345 3.5 * 2
  ```

- Serwer odbiera wiadomość, przetwarza wyrażenie (wspierane operatory: `+ - * /`), oblicza wynik i odsyła go do kolejki
  `/PID`.

- Klient odbiera odpowiedź i wypisuje wynik.

## Obsługa błędów

Wszystkie operacje systemowe są opakowane funkcjami z pliku `my_mqueue.c`, które zwracają `0` lub `1`. W kodzie
wywoływane są przez makro `CheckError(...)`, które:

- wypisuje miejsce błędu (plik, linia, funkcja, argument)
- kończy program z `exit(EXIT_FAILURE)`

## Czyszczenie

```bash
make clean
```

Usuwa pliki wykonywalne i tymczasowe (`.o`, `.a`, `.so`, `core`, itd.)

## Archiwizacja

```bash
make tar
```

Pakuje cały katalog projektu do archiwum `KolejkiKomunikatow.tar.gz`.

## Pomoc

```bash
make help
```

Wyświetla listę dostępnych celów Makefile.
