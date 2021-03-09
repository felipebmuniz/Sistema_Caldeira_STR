// Monitor para o acesso aos sensore da caldeira

#include <math.h>
#include <pthread.h>
#include "monitor_sensor.h"

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t alarme = PTHREAD_MUTEX_INITIALIZER;
static double sensor_lido[5] = {0, 0, 0, 0, 0}; // sensores -> Ta, T, Ti, No, H;
static double limite_atual = HUGE_VAL;

// Chama pela thread do sensor e disponibiliza o valor dele lido
// Aplica valores na variavel compartilhada
void sensor_put(double lido, int i)
{
    pthread_mutex_lock(&exclusao_mutua);
    sensor_lido[i] = lido;
    if (sensor_lido[i] >= limite_atual)
        pthread_cond_signal(&alarme);
    pthread_mutex_unlock(&exclusao_mutua);
}

// Chama por qualquer thread que precise do valor do sensor
// Envia o valor da variavel compartilhada
double sensor_get(int i)
{
    double aux;
    pthread_mutex_lock(&exclusao_mutua);
    aux = sensor_lido[i];
    pthread_mutex_unlock(&exclusao_mutua);
    return aux;
}

// Funcao de thread que fica ploqueada até o valor de sensor chegue em seu limite
void sensor_alarme(double limite)
{
    pthread_mutex_lock(&exclusao_mutua);
    limite_atual = limite;
    while (sensor_lido[1] < limite_atual)
        pthread_cond_wait(&alarme, &exclusao_mutua);
    limite_atual = HUGE_VAL;
    pthread_mutex_unlock(&exclusao_mutua);
}
