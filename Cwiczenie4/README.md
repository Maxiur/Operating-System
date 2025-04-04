Autor: Mateusz Kamiński                            Kraków, 04.04.2025

## Polecenie:

ĆWICZENIE 4: Producent–Konsument: Potoki Nienazwane
Przy pomocy potoków nienazwanych systemu UNIX zaimplementować problem „Producenta i Konsumenta”.
Dla zademonstrowania, że nie doszło do utraty ani zwielokrotnienia towaru, niech Producent pobiera „surowiec” (np. porcje bajtów) z pliku tekstowego i wstawia go jako towar do potoku, a Konsument niech umieszcza pobrany z potoku towar w innym pliku tekstowym (porcje danych Producenta i Konsumenta nie muszą być równe).
Po zakończeniu działania programów (wyczerpaniu zasobów „surowca”) oba pliki tekstowe powinny być identyczne (można to sprawdzić poleceniem diff -s, które najlepiej umieścić w pliku Makefile po poleceniu uruchomienia programu).
Oba procesy niech drukują odpowiednie komunikaty na ekranie, w tym towar, który przesyłają.
Do zasymulowania różnych szybkości działania programów użyć funkcji sleep, np. z losowym czasem usypiania.
Do czytania/pisania danych z/do pliku tekstowego, jak również wypisywania ich na ekran użyć funkcji read i write.

## Zawartość katalogu:

W katalogu znajdują się:
- 4.c - program realizujący problem Producenta - Konsumenta,
- Makefile - plik do kompilacji,
- input.txt - plik z danymi wejściowymi,

## Kompilacja:

 Aby skompilować główny program:
---> make

Dostępne cele pomocnicze:
---> make run            – uruchomienie programu z domyślnymi argumentami
---> make clean          – usunięcie plików tymczasowych
---> make tar            – spakowanie całego katalogu do .tar.gz
---> make help           – lista dostępnych komend

## Schemat działania programu:

                 +------------------+
                 |    input.txt     |
                 +--------+---------+
                          |
                          v
                 +--------+---------+
                 |     Producent     |
                 |   (proces główny) |
                 +--------+---------+
                          |
                       [ pipe ]
                          |
                          v
                 +--------+---------+
                 |     Konsument     |
                 |  (proces potomny) |
                 +------------------+
                          |
                          v
               +------------------------+
               |  output.txt / terminal |
               +------------------------+
