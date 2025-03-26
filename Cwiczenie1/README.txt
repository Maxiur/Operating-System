>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Autor: Mateusz Kamiński                            Krakow, 21.03.2025

				#Zawartosc:
Katalog Temat2a zawiera programy o nazwach 1?.c, poprzedzone numerem podpunktu
Aby zkompilowac program, nalezy zmienic w Makefile zmienna MAIN_NAME
Plik wynikowy ukrywa sie pod nazwa "program"

Aby uruchomic program nalezy uzyc:
---> make run

Aby wyczyscic zbedne pliki:
---> make clean

W celu archiwizacji:
---> make tar
plik z rozszerzeniem .tar.gz zostanie umieszczony w katalogu poprzedzajacym

Wiecej informacji pod:
--->make help

				#Opis podpunktów

(a) Wypisanie identyfikatorów procesu

Program wypisuje podstawowe identyfikatory procesu, takie jak UID, GID, PID, PPID oraz PGID.

(b) Tworzenie procesów potomnych

Program wywołuje fork() trzy razy w pętli i wypisuje identyfikatory dla procesu macierzystego oraz wszystkich procesów potomnych. Proces macierzysty czeka na zakończenie potomków przy użyciu wait().

(c) Osierocenie procesów

Proces macierzysty kończy się natychmiast po stworzeniu potomków, pozostawiając ich osieroconych. Potomkowie śpią przez kilka sekund, zanim zakończą swoje działanie, aby możliwe było zobaczenie ich adopcji przez init/systemd.

(d) Grupowanie procesów pokoleniami

Procesy potomne są tworzone w sposób kontrolowany, aby były grupowane pokoleniami. sleep() jest używane do kontrolowania kolejności ich pojawiania się na ekranie.

(e) Każdy proces jako lider własnej grupy

Procesy potomne stają się liderami swoich własnych grup procesów poprzez użycie setpgid(), co pozwala na ich izolację w osobnych grupach.

(f) Uruchamianie programów w procesach potomnych

Procesy potomne używają execlp(), aby zastąpić swój kod innym programem, podanym jako argument do programu macierzystego. Proces macierzysty czeka na zakończenie każdego potomka.
