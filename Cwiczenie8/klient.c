/* ==============================================================================
* Autor: Mateusz Kamiński
 * Data: 21-05-2025
 *
 * Klient – tworzy kolejkę odpowiedzi (/PID), wysyła przez kolejkę serwera żądanie
 * obliczenia wyrażenia arytmetycznego (np. “5+5”), odbiera wynik lub komunikat błędu
 * i wyświetla go użytkownikowi. Zamyka kolejki po zakończeniu i obsługuje SIGINT.
 * ============================================================================ */

#include "./include/my_mqueue.h"
#include "./include/common.h"
#include "./include/CheckError.h"

static volatile sig_atomic_t running = 1;
mqd_t client_queue;
mqd_t server_queue;
char client_queue_name[MAX_CLIENT_NAME_LEN];

// Sprzątanie zasobów
void cleanup(void) {
    if (client_queue >= 0) {
        my_mq_close(client_queue);
        my_mq_unlink(client_queue_name);
    }
}

// Handler SIGINT – wyjście z atexit(cleanup)
void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\n[Klient] Otrzymano SIGINT. Kończę i usuwam zasoby.\n");
        exit(0);
    }
}

int main(void) {
    // Handler SIGINT + atexit
    CheckError(signal(SIGINT, signal_handler) != SIG_ERR);
    atexit(cleanup);

    // Tworzenie własnej kolejki /PID
    pid_t pid = getpid();
    int n = snprintf(client_queue_name, sizeof client_queue_name, "/%d", pid);
    CheckError(n > 0 && n < (int)sizeof client_queue_name);

    struct mq_attr attr = {
        .mq_flags   = 0,
        .mq_maxmsg  = 10,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_curmsgs = 0
    };
    CheckError(my_mq_open(&client_queue,client_queue_name, O_CREAT | O_RDONLY, 0666, &attr));
    printf("[Klient] Kolejka %s utworzona.\n", client_queue_name);

    // Otwarcie kolejki serwera
    CheckError(my_mq_open(&server_queue, SERVER_QUEUE_NAME, O_WRONLY, 0, NULL));

    char expr[MAX_EXPR_SIZE + 1];
    char message[MAX_MSG_SIZE];
    char response[MAX_MSG_SIZE];

    printf("[Klient] Podaj działanie (np. 2 + 3) lub Ctrl+D, aby zakończyć:\n");

    while (1) {
        if (!fgets(expr, sizeof expr, stdin)) {
            if (feof(stdin)) { printf("\n[Klient] EOF (Ctrl+D). Kończę.\n"); break; }
            perror("[Klient] fgets"); continue;
        }

        expr[strcspn(expr, "\n")] = '\0';

        n = snprintf(message, sizeof message, "%d %s", (int)pid, expr);
        CheckError(n > 0 && n < (int)sizeof message);
        // Wysyłanie wiadomości do serwera
        CheckError(my_mq_send(server_queue, message, n + 1, 0));

        sleep(1);

        memset(response, 0, sizeof response);
        // Odebranie wiadomosci z kolejki
        CheckError(my_mq_receive(client_queue, response, sizeof response, NULL));
        response[sizeof response - 1] = '\0';

        if (strncmp(response, "ERROR:", 6) == 0) {
            char *reason = response + 7;  // skip "ERROR: "
            printf("[Klient] Błąd serwera: %s\n", reason);
        } else {
            printf("[Klient] Wynik: %s\n", response);
        }

        printf("[Klient] Podaj kolejne działanie lub Ctrl+D, aby zakończyć:\n");
    }
    return 0;
}
