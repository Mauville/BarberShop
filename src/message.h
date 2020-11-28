#include <errno.h>
#include <fcntl.h> /* For O_* constants */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <unistd.h>
typedef struct
{
    int op;
    // 1: initChairs()
    // 2: putonstreet
    // 3: putonwaiting
    // 4: removestreet
    // 5: removewaiting
    // 6: putonbarber
    // 7: removefrombarber
    // Chairs
    int chairs;
} Message;

int own_id;
key_t own_key;

void acquireMessageQueue()
{
    // create message queue
    if (((own_key = ftok("src/Main.c", 0)) == -1))
    {
        perror("ftok error\n");
        exit(1);
    }
    if ((own_id = msgget(own_key, IPC_CREAT | S_IRWXU)) == -1)
    {
        perror("msgget\n");
        exit(1);
    }
    printf("Successful INIT\n");
}