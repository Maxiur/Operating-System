# Wzajemne Wykluczanie dla Wątków: Muteksy z Kolorami

Projekt demonstruje wzajemne wykluczanie wątkach przy użyciu muteksu POSIX. Każdy wątek wykonuje cykl sekcji prywatnych
i krytycznych, z wyróżnieniem kolorami i czyszczeniem linii, aby uniknąć nakładania komunikatów.

## Pliki w projekcie

* **mutual\_exclusion\_colored.c** – kod źródłowy programu.
* **Makefile** – skrypt kompilacji i uruchamiania.

## Kompilacja

```bash
make
```

Tworzy plik wykonywalny `mutex_threads_example`.

## Uruchomienie

```bash
make run THREADS=<liczba_wątków> SECTIONS=<liczba_sekcji>
```

Przykład:

```bash
make run THREADS=4 SECTIONS=5
```

Uruchamia program z 4 wątkami i 5 sekcjami każdego wątku.

## Makefile

* `all` – kompiluje program.
* `run` – uruchamia program z parametrami `THREADS` i `SECTIONS`.
* `clean` – usuwa pliki obiektów i binarium.
* `tar` – tworzy archiwum projektu.
* `help` – wyświetla dostępne cele Makefile.

## Opis działania programu

1. **Inicjalizacja**

    * Czyści ekran.
    * Inicjalizuje muteks.
    * Wyświetla adres muteksu i listę identyfikatorów wątków.
2. **Praca wątków**

    * Każdy wątek w pętli:

        * **Sekcja prywatna** (zielony): czyszczenie linii, wypisanie numeru sekcji i wartości globalnego licznika,
          losowe uspienie.
        * **Sekcja krytyczna** (czerwony): blokada muteksu, inkrementacja licznika, wypisanie komunikatu, losowe
          uspienie, odblokowanie muteksu.
        * **Przywrócenie prywatnego** (zielony): informacja po zakończeniu sekcji krytycznej, losowe uspienie.
3. **Zakończenie**

    * Dołączenie wątków.
    * Niszczenie muteksu.
    * Wyświetlenie finalnej wartości `global_counter` i porównanie z wartością oczekiwaną.

---

Mateusz Kamiński, Kraków, 25 maja 2025
