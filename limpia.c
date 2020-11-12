#include <stdio.h>
#include <semaphore.h>

#define CLIENTE "Cliente"
#define SALA_ESPERA_MUTEX "Sala espera"

sem_t *clientes;
sem_t *salaEsperaMutex;

int main()
{
    // Semaforos
    clientes = sem_open(CLIENTE, O_CREAT, S_IRWXU, 0);
    salaEsperaMutex = sem_open(SALA_ESPERA_MUTEX, O_CREAT, S_IRWXU, 1);
    if (clientes == SEM_FAILED)
    {
        perror("problemas al crear el semáforo");
        return 1;
    }
    if (salaEsperaMutex == SEM_FAILED)
    {
        perror("problemas al crear el semáforo");
        return 1;
    }

    // Cerrar y desligar semáforos
    sem_close(clientes);
    sem_close(salaEsperaMutex);
    sem_unlink(CLIENTE);
    sem_unlink(SALA_ESPERA_MUTEX);
    return 0;
}