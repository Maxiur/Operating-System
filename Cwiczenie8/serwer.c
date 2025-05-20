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
	}
}

int calculator(const char *expr, double *result) {
    double a, b;
    char op;

    if (sscanf(expr, "%lf %c %lf", &a, &op, &b) != 3)
        return 0;

    switch (op) {
        case '+': *result = a + b; return 1;
        case '-': *result = a - b; return 1;
        case '*': *result = a * b; return 1;
        case '/': if (b == 0) return 0; *result = a / b; return 1;
        default: return 0;
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
    printf("[Serwer] Kolejka %s utworzona. Oczekiwanie na wiadomości...\n", SERVER_QUEUE_NAME);

    // Bufory
    char message[MAX_MSG_SIZE];
    char expr[MAX_EXPR_SIZE];
    char response[MAX_MSG_SIZE];
    char client_queue_name[32];

    while (running) {
        // Odbiór
        memset(message, 0, sizeof(message));
        CheckError(my_mq_receive(server_queue, message, sizeof(message), NULL));

        // Parsowanie
        pid_t client_pid;
        if (sscanf(message, "%d %[^\n]", &client_pid, expr) != 2) {
            fprintf(stderr, "[Serwer] Błędny format wiadomości: %s\n", message);
            continue;
        }

        printf("[Serwer] Odebrano od PID %d: %s\n", client_pid, expr);

        // Obliczenia
        double result;
        if (!calculator(expr, &result)) {
            fprintf(stderr, "[Serwer] Błąd obliczenia wyrażenia: %s\n", expr);
            continue;
        }

        // Formatowanie odpowiedzi
        int L;
		L = snprintf(response, sizeof response, "%.6lf", result);
		CheckError(L > 0 && L < (int)sizeof response);

        // Przygotowanie nazwy kolejki klienta
		L = snprintf(client_queue_name, sizeof client_queue_name, "/%d", client_pid);
		CheckError(L > 0 && L < (int)sizeof client_queue_name);

        // Wysyłka i zamknięcie
        mqd_t client_queue;
        CheckError(my_mq_open(&client_queue, client_queue_name, O_WRONLY, 0, NULL));
        CheckError(my_mq_send(client_queue, response, L + 1, 0));
        CheckError(my_mq_close(client_queue));

        printf("[Serwer] Wysłano wynik %.6lf do klienta %d\n", result, client_pid);
    }

    cleanup();
    return 0;
}
