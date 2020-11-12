#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define CLIENTE "Cliente"
#define SALA_ESPERA_MUTEX "Sala espera"

sem_t *clientes;
sem_t *salaEsperaMutex; // Protege variable salaEspera
int numClientes;
int salaEspera;
int salaEsperaMax;
int salaEsperaArr[] = {-1, -1, -1, -1, -1}; // Solo para pruebas
int tiempoCorteDePelo;

void imprimeSalaEspera()
{
    printf("[ ");
    for (int i = 0; i < salaEsperaMax; i++)
    {
        printf("%d ", salaEsperaArr[i]);
    }
    printf("]\n");
}

void *barbero_func()
{
    while (1)
    {
        printf("Barbero durmiendo...\n");
        // Espera a que un cliente lo despierte
        sem_wait(clientes);
        printf("Barbero es despertado por un cliente...\n");
        // Como el cliente se sentó con el barbero, hay otro lugar libre en la sala de espera
        sem_wait(salaEsperaMutex);
        salaEsperaArr[salaEspera] = -1;
        salaEspera++;
        sem_post(salaEsperaMutex);
        printf("Barbero cortando el cabello...\n");
        sleep(tiempoCorteDePelo);
        printf("Barbero terminó de cortar el cabello...\n");
        printf("Sala de espera: ");
        imprimeSalaEspera();
    }

    return NULL;
}

void *cliente_func(void *arg)
{
    int num_cliente = *((int *)arg);
    printf("Llega cliente %d...\n", num_cliente);
    // Protege variable salaEspera
    sem_wait(salaEsperaMutex);
    printf("Lugares libres en sala de espera: %d\n", salaEspera);
    if (salaEspera > 0)
    {
        // Hay asientos libres en la sala de espera
        salaEsperaArr[salaEspera - 1] = num_cliente;
        salaEspera--;
        printf("El cliente %d se encuentra en la sala de espera: ", num_cliente);
        imprimeSalaEspera();
        printf("Lugares libres en sala de espera: %d\n", salaEspera);
        // Se puede liberar variable salaEspera, ya fué modificada
        sem_post(salaEsperaMutex);
        // Avisa al barbero que hay cliente(s) esperando, pueden ser varios, pero el barbero
        // solo puede atender 1 a la vez
        sem_post(clientes);
        // Espera a que el barbero le corte el pelo
    }
    else
    {
        // No hay asientos libres en la sala de espera
        printf("Está lleno, un cliente %d se va...\n", num_cliente);
        sem_post(salaEsperaMutex);
    }

    return NULL;
}

int main()
{
    // Semaforos
    clientes = sem_open(CLIENTE, O_CREAT, S_IRWXU, 0); // cleintes esperando
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

    // hilos
    pthread_attr_t attr;
    /* inicializa los atributos para los threads */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    // Inputs del usuario
    printf("Numero de clientes: ");
    scanf("%d", &numClientes);
    printf("Numero de sillas en sala de espera: ");
    scanf("%d", &salaEsperaMax);
    salaEspera = salaEsperaMax;
    printf("Tiempo de corte de pelo(seg): ");
    scanf("%d", &tiempoCorteDePelo);

    // Hilo Barbero
    pthread_t barbero_thread;
    int tmp = pthread_create(&barbero_thread, &attr, barbero_func, NULL);
    if (tmp != 0)
    {
        printf("Problemas al crear el thread barbero\n");
        exit(EXIT_FAILURE);
    }

    // Hilos clientes
    pthread_t clientes_threads[numClientes];
    for (int i = 0; i < numClientes; i++)
    {
        sleep(2);
        int tmp = pthread_create(&clientes_threads[i], &attr, cliente_func, &i);
        if (tmp != 0)
        {
            printf("Problemas al crear el thread cliente %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Terminar hilo barbero
    tmp = pthread_join(barbero_thread, NULL);
    if (tmp != 0)
    {
        printf("Problemas al crear el thread barbero\n");
        exit(EXIT_FAILURE);
    }

    // Terminar hilos clientes
    for (int i = 0; i < numClientes; i++)
    {
        int tmp = pthread_join(clientes_threads[i], NULL);
        if (tmp != 0)
        {
            printf("Problemas al crear el thread cliente %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Cerrar y desligar semáforos
    sem_close(clientes);
    sem_close(salaEsperaMutex);
    sem_unlink(CLIENTE);
    sem_unlink(SALA_ESPERA_MUTEX);

    return 0;
}