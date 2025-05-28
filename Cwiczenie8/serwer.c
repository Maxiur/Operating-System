/* ==============================================================================
 * Autor: Mateusz Kamiński
 * Data: 21-05-2025
 *
 * Serwer – odbiera od klientów żądania obliczeń arytmetycznych przez kolejki POSIX,
 * wykonuje działanie (z obsługą błędów: nieprawidłowy format, nieznany operator,
 * dzielenie przez zero) i odsyła wynik lub komunikat o błędzie.
 * ============================================================================ */

#include "./include/my_mqueue.h"
#include "./include/common.h"
#include "./include/CheckError.h"

mqd_t server_queue;

static volatile sig_atomic_t running = 1;

// Zwalnianie zasobów kolejki
void cleanup() {
    CheckError(my_mq_close(server_queue));
    CheckError(my_mq_unlink(SERVER_QUEUE_NAME));
}

// Handler SIGINT (CTRL + C)
void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\n[Serwer] Otrzymano SIGINT. Kończę program i usuwam zasoby.\n");
		running = 0;
		exit(0);
	}
}

int main() {
    // cleanup() & Handler
    atexit(cleanup);
    CheckError(signal(SIGINT, signal_handler) != SIG_ERR);

    // Atrybuty kolejki serwera
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_curmsgs = 0
    };

    // Tworzenie kolejki serwera
    CheckError(my_mq_open(&server_queue, SERVER_QUEUE_NAME, O_CREAT | O_RDONLY, 0666, &attr));

    // wypisz atrybuty
    struct mq_attr got;
    CheckError(my_mq_getattr(server_queue, &got));
    printf("[Serwer] Kolejka %s\n", SERVER_QUEUE_NAME);
    printf("[Serwer] Deskryptor: %d  maxmsg=%ld  msgsize=%ld\n",
           server_queue, got.mq_maxmsg, got.mq_msgsize);

    // Bufory
    char message[MAX_MSG_SIZE];
    char expr[MAX_EXPR_SIZE];
    char response[MAX_MSG_SIZE];
    char client_queue_name[MAX_CLIENT_NAME_LEN];

   // główna pętla
    while (running) {
        // odbiór
        memset(message, 0, sizeof message);
        CheckError(my_mq_receive(server_queue, message, sizeof message,NULL));

        // parsowanie PID i wyrażenia
        pid_t pid;
        int resp_len;
        if (sscanf(message, "%d %[^\n]", &pid, expr) != 2) {
            fprintf(stderr, "[Serwer] Błędny format wiadomości: %s\n", message);
            resp_len = snprintf(response, sizeof response, "ERROR: nieprawidłowy format");
        }
        else {
            printf("[Serwer] Odebrano od PID %d: %s\n", pid, expr);

            // parsowanie operandów i operatora
            double a, b, result;
            char op;
            int m = sscanf(expr, "%lf %c %lf", &a, &op, &b);
            if (m != 3) {
                m = sscanf(expr, "%lf%c%lf", &a, &op, &b);
            }

            if (m != 3) {
                resp_len = snprintf(response, sizeof response, "ERROR: nieprawidłowy format");
            }
            else if (op!='+' && op!='-' && op!='*' && op!='/') {
                resp_len = snprintf(response, sizeof response, "ERROR: nieznany operator");
            }
            else if (op=='/' && b==0) {
                resp_len = snprintf(response, sizeof response, "ERROR: dzielenie przez zero");
            }
            else {
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': result = a / b; break;
                }
                resp_len = snprintf(response, sizeof response, "%g", result);
            }
        }
        CheckError(resp_len > 0 && resp_len < (int)sizeof response);

        // przygotowanie nazwy kolejki klienta
        int name_len = snprintf(client_queue_name, sizeof client_queue_name, "/%d", pid);
        CheckError(name_len > 0 && name_len < (int)sizeof client_queue_name);

        // symulacja opóźnienia
        sleep(1);

        // otwarcie, wysyłka i zamknięcie
        mqd_t cq;

        CheckError(my_mq_open(&cq, client_queue_name, O_WRONLY, 0, NULL));
        CheckError(my_mq_send(cq, response, resp_len + 1, 0));
        CheckError(my_mq_close(cq));

        // logowanie
        printf("[Serwer] Wysłano \"%s\" do klienta %d\n", response, (int)pid);
    }

    return 0;
}
