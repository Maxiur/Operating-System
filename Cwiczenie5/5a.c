//-------------------------------------------------------------
// Autor: Mateusz Kamiński                   Kraków, 05.04.2025
//
// CWICZENIE 5A - Problem Producenta – Konsumenta
// Wykorzystanie potoków nazwanych (FIFO) do komunikacji
// między procesami
//
// Wersja (a):
//  - Potok FIFO tworzony jest z poziomu programu przy użyciu mkfifo()
//  - Proces macierzysty uruchamia Producenta i Konsumenta (fork)
//  - Procesy komunikują się za pomocą utworzonego potoku
//  - Potok jest automatycznie usuwany przy zakończeniu programu
//    za pomocą funkcji zarejestrowanej przez atexit()
//
// Przykładowe uruchomienie:
//     ./producent_konsument <input.txt> <output.txt> <fifo_path>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

const char* fifo_path_global = NULL;

void error_message(const char* msg)
{
    perror(msg);
    exit(1);
}

void clean_fifo()
{
    if (unlink(fifo_path_global) == -1)
    {
        error_message("unlink fifo error");
    }
}

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nOtrzymano sygnał SIGINT. Kończę program i usuwam Potok FIFO.\n");
        clean_fifo();
        _exit(1); // Kończenie programu bez wywoływania atexit
    }
}


int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Za mało argumentów! Proszę wywołać: %s <output> <input> <fifo_path> \n", argv[0]);
        exit(1);
    }

    const char* fifo_path = argv[3];
    fifo_path_global = fifo_path;

    // Rejestracja sygnału SIGINT
    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        error_message("signal error");
    }

    int fifo = mkfifo(fifo_path, 0644);
    if (fifo == -1)
    {
        error_message("mkfifo error");
    }

    if (atexit(clean_fifo) != 0)
    {
        error_message("atexit error");
    }

    // Proces 1 - Producent
    switch (fork())
    {
    case -1:
        error_message("fork error");

    case 0:
        execlp("./read_fifo", "./read_fifo", argv[2], fifo_path, (char*)NULL);
        perror("execlp error");
        _exit(1);
    }

    // Proces 2 - Konsument
    switch (fork())
    {
    case -1:
        error_message("fork error");

    case 0:
        execlp("./write_fifo", "./write_fifo", argv[1], fifo_path, (char*)NULL);
        perror("execlp error");
        _exit(1);
    }

    // Jeśli porcja bitów, które pobiera potok, różni się między Producentem, a Konsumentem
    // to może się zdarzyć, że Producent zakończy się przed Konsumentem i przejdzie nam przez wait() i zakończy program
    // kiedy to Kosument nadal pobiera dane. // Można zrobić to po prostu dwoma wait(NULL).

    // Czeka na pierwsze dziecko oraz drugie dziecko
    for(int i = 0; i < 2; i++)
    {
        if (wait(NULL) == -1)
        {
            error_message("wait error");
        }
    }

    printf("\n\033[32m[Producent]\033[0m Proces Producenta zakończony.\n");
    printf("\033[32m[Konsument]\033[0m Proces Konsumenta zakończony.\n");
    printf("Procesy zakończone. Usuwam FIFO.\n");

    return 0;
}
