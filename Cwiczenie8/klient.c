#include "./include/my_mqueue.h"
#include "./include/common.h"
#include "./include/CheckError.h"

mqd_t client_queue;
mqd_t server_queue;
char client_queue_name[64];

void cleanup() {
    CheckError(my_mq_close(client_queue));
    CheckError(my_mq_unlink(client_queue_name));
}

void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\n[Klient] Otrzymano SIGINT. Kończę program i usuwam zasoby.\n");
        cleanup();
        _exit(1);
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
    CheckError(signal(SIGINT, signal_handler) != SIG_ERR);
    atexit(cleanup);  // na wszelki wypadek

    pid_t pid = getpid();
    snprintf(client_queue_name, sizeof(client_queue_name), "/%d", pid);

    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = MAX_MSG_SIZE,
        .mq_curmsgs = 0
    };

    // Tworzenie kolejki klienta
    CheckError(my_mq_open(&client_queue, client_queue_name, O_CREAT | O_RDONLY, 0666, &attr));

    // Otwieranie kolejki serwera
    CheckError(my_mq_open(&server_queue, SERVER_QUEUE_NAME, O_WRONLY, 0, NULL));

    char expr[MAX_EXPR_SIZE];
    char message[MAX_MSG_SIZE];
    char response[MAX_MSG_SIZE];

    printf("[Klient] Podaj działanie (np. 2 + 3) lub Ctrl+D, aby zakończyć:\n");

    while (fgets(expr, sizeof(expr), stdin)) {
        expr[strcspn(expr, "\n")] = '\0';  // usuń \n z końca

        snprintf(message, sizeof(message), "%d %s", pid, expr);

        CheckError(my_mq_send(server_queue, message, strlen(message) + 1, 0));

        memset(response, 0, sizeof(response));
        CheckError(my_mq_receive(client_queue, response, sizeof(response), NULL));
        response[MAX_MSG_SIZE - 1] = '\0';

        printf("[Klient] Wynik: %s\n", response);
        printf("[Klient] Podaj kolejne działanie lub Ctrl+D, aby zakończyć:\n");
    }

    return 0;
}
