#ifndef COMMON_H
#define COMMON_H

#define SERVER_QUEUE_NAME "/server_queue"

#define MAX_MSG_SIZE             256 // maksymalna długość wyrażenia
#define MAX_EXPR_SIZE            (MAX_MSG_SIZE + 32) // 32 znaki na PID, spację i null
#define MAX_CLIENT_NAME_LEN      32 // długość bufora

#endif //COMMON_H
