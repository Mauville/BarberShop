#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


#define CLIENTE "Cliente"
#define SALA_ESPERA_MUTEX "Sala espera"

sem_t *clientes;
sem_t *salaEsperaMutex;

int main()
{
    // Semaforos
    clientes = sem_open(CLIENTE, IPC_CREAT, S_IRWXU, 0);
    salaEsperaMutex = sem_open(SALA_ESPERA_MUTEX, IPC_CREAT, S_IRWXU, 1);
    if (clientes == SEM_FAILED)
    {
        perror("Clean semaphore!");
    }
    if (salaEsperaMutex == SEM_FAILED)
    {
        perror("Clean semaphore!");
    }

    // Cerrar y desligar semáforos
    sem_close(clientes);
    sem_close(salaEsperaMutex);
    sem_unlink(CLIENTE);
    sem_unlink(SALA_ESPERA_MUTEX);
    return 0;
}
