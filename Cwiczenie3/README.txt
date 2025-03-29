>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Autor: Mateusz Kamiński                            Kraków, 27.03.2025

                               #Zawartość:
Katalog Cwiczenie3 zawiera zestaw programów realizujących obsługę i wysyłanie
sygnałów w systemie UNIX z wykorzystaniem funkcji: signal(), kill(), fork(), wait(),
execlp() oraz pause().

W katalogu znajdują się:
- program_a.c – program obsługujący sygnał w trzech trybach: domyślnym, ignorowania i własnej obsługi
- program_b.c – program, który uruchamia program_a jako proces potomny i wysyła do niego sygnał
- program_leader.c – program tworzący grupę procesów, ignorujący sygnał i uruchamiający wiele kopii program_a
- program_c.c – program nadrzędny uruchamiający program_leader i wysyłający sygnał do całej grupy procesów
- Makefile – umożliwia kompilację, uruchomienie i archiwizację projektu

                               #Kompilacja:

Aby skompilować główny program:
---> make

Dostępne cele pomocnicze:
---> make run            – uruchomienie programu z domyślnymi argumentami
---> make run_b          – uruchomienie program_b z programem_a
---> make run_leader     – uruchomienie program_leader z programem_a
---> make run_c          – uruchomienie program_c
---> make clean          – usunięcie plików tymczasowych
---> make tar            – spakowanie całego katalogu do .tar.gz
---> make help           – lista dostępnych komend

                            #Parametry uruchamiania

Domyślnie programy uruchamiane są z sygnałem SIGINT (numer 2) i trybem "D" (domyślna obsługa).
Istnieje możliwość podania własnych wartości przy wywołaniu `make`, np.:

    make run_a SIG=3 TRYB=I

co jest równoważne wywołaniu:

    ./program_a 3 I


                               #Opis działania:

>> program_a – Ustawianie i obsługa sygnału

Program przyjmuje dwa argumenty: numer sygnału oraz tryb:
  - "D" – domyślna obsługa
  - "I" – ignorowanie
  - "P" – własna obsługa (przechwytuje sygnał i wypisuje numer oraz nazwę)

Po ustawieniu odpowiedniego trybu program wypisuje PID i przechodzi w stan oczekiwania
na sygnał za pomocą funkcji pause().

>> program_b – Testowanie pojedynczego potomka

Tworzy proces potomny i uruchamia w nim program_a z wybranym sygnałem i trybem.
Proces macierzysty po krótkim sleep() wysyła do niego sygnał, a następnie wypisuje
informację o zakończeniu oraz sygnale.

>> program_leader – Lider grupy procesów

Ustawia ignorowanie sygnału i staje się liderem nowej grupy procesów. Następnie tworzy
kilka procesów potomnych (np. 3), które uruchamiają program_a. Lider czeka na zakończenie
wszystkich swoich dzieci i wypisuje sposób ich zakończenia.

>> program_c – Nadrzędny kontroler

Tworzy jeden proces potomny, w którym uruchamia program_leader.
Po krótkim czasie wysyła sygnał do całej grupy procesów potomnych
(za pomocą funkcji kill() z ujemnym PGID), co pozwala zbadać ich reakcje
na wspólny sygnał.

                               #Przykład uruchomienia:

./program_c ./program_leader ./program_a 2 d
⇒ uruchomi leadera, który odpali 3 programy_a z sygnałem SIGINT i trybem "d"

⇒ po 2 sekundach program_c wyśle sygnał SIGINT do całej grupy procesów potomnych
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
