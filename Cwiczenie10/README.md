# Ćwiczenie 10 – Algorytm Piekarni z kolorami

**Autor:** Mateusz Kamiński  
**Data:** 28.05.2025

## Opis

Program demonstruje **wzajemne wykluczanie wątków** z wykorzystaniem klasycznego **algorytmu piekarni** (Bakery
Algorithm). Dodatkowo program oferuje **wizualną reprezentację** działania:

- **Zielony tekst**: sekcja prywatna wątku
- **Czerwony tekst**: sekcja krytyczna wątku
- **Niebieski tekst**: zakończenie działania wątku
- **Puste linie po lewej**: oznaczają, że dany wątek przebywa w sekcji krytycznej

## Parametry uruchomienia

Program przyjmuje dwa argumenty:

```
./piekarnia <liczba_wątków> <liczba_sekcji>
```

Przykład:

```
./piekarnia 4 3
```

lub za pomocą Makefile:

```
make run THREADS=4 SECTIONS=3
```

## Kompilacja

Do kompilacji używany jest `Makefile` (plik `piekarnia.c` jako źródło):

```bash
make
```

Uruchomienie:

```bash
make run THREADS=4 SECTIONS=3
```

Czyszczenie plików po kompilacji:

```bash
make clean
```

## Zasada działania

Każdy wątek:

1. Wchodzi do sekcji prywatnej (zielony komunikat po lewej),
2. Wybiera numer biletu (część algorytmu piekarni),
3. Czeka na swoją kolej (kolejność wg numerów i ID wątku),
4. Czyści lewą linię (symuluje „przejście w bok”),
5. Wchodzi do sekcji krytycznej (czerwony komunikat po prawej),
6. Zwiększa wspólny licznik `global_counter`,
7. Po zakończeniu wraca do lewej strony i wypisuje komunikat (zielony lub niebieski).

