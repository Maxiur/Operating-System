>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Autor: Mateusz Kamiński                            Kraków, 25.03.2025

                                #Zawartość:
Katalog Temat2b zawiera program 2a.c realizujący tworzenie procesów potomnych,
które uruchamiają zewnętrzny program (komunikat) przy pomocy funkcji execlp.

Dodatkowo w katalogu znajdują się:
- Makefile – umożliwia kompilację, uruchomienie i archiwizację projektu
- komunikat.c – program uruchamiany przez procesy potomne (za pomocą execlp)
- odpowiedz2a.txt – zawiera odpowiedź dotyczącą liczby procesów

Aby skompilować program, należy ustawić w Makefile zmienną MAIN_NAME.
Plik wynikowy tworzony jest pod nazwą "program".

Aby uruchomić program należy użyć:
---> make run

lub z podaniem dowolnego programu do uruchomienia:
---> make run PROGRAM=./nazwa_programu

Aby wyczyścić zbędne pliki:
---> make clean

W celu archiwizacji:
---> make tar
plik z rozszerzeniem .tar.gz zostanie umieszczony w katalogu poprzedzającym

Więcej informacji pod:
---> make help

                                #Opis programu
			   Uruchamianie Procesow: EXEC

Program trzykrotnie wywołuje fork() w pętli. Każdy proces potomny nie wypisuje
żadnych komunikatów samodzielnie — zamiast tego uruchamia nowy program przy
pomocy execlp(), przekazując nazwę tego programu jako argument. Wypisywanie
komunikatów odbywa się wewnątrz tych uruchomionych programów. Proces
macierzysty czeka na zakończenie każdego z potomków przy użyciu wait().
