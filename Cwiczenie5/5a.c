//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 04.04.2025
//
// CWICZENIE 4 - Problem Producenta - Konsumenta
// Wykorzystanie potoków do komunikacji między procesami
//
// Przykładowe uruchomienie: ./producent_konsument input.txt output.txt

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 64 // rozmiar bufora (liczba bajtów na raz)

void error_message(const char *msg) {
    perror(msg);
    exit(1);
}
