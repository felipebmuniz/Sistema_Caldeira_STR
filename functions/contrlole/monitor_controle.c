// Monitor para o acesso aos atuadores da caldeira

#include <math.h>
#include <pthread.h>
#include "monitor_controle.h"

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;

// static pthread_cond_t limite_T = PTHREAD_MUTEX_INITIALIZER;
// static pthread_cond_t limite_H = PTHREAD_MUTEX_INITIALIZER;

static double atuador_lido[4] = {0, 0, 0, 0}; // atuadores -> Q, Ni, Nf, Na;
// static double limite_atual_t = HUGE_VAL;
// static double limite_atual_h = HUGE_VAL;

// Chama pela thread do atuador e disponibiliza o valor dele lido
// Aplica valores na variavel compartilhada
void atuador_put(double lido, int i)
{
    pthread_mutex_lock(&exclusao_mutua);
    atuador_lido[i] = lido;
    // if (atuador_lido[i] >= limite_atual_t)
    //     pthread_cond_signal(&limite_T);
    // if (atuador_lido[i] >= limite_atual_h)
    //     pthread_cond_signal(&limite_H);
    pthread_mutex_unlock(&exclusao_mutua);
}

// Chama por qualquer thread que precise do valor do atuador
// Envia o valor da variavel compartilhada
double atuador_get(int i)
{
    double aux;
    pthread_mutex_lock(&exclusao_mutua);
    aux = atuador_lido[i];
    pthread_mutex_unlock(&exclusao_mutua);
    return aux;
}

// Funcao de thread que fica ploqueada até o valor de atuador chegue em seu limite
// void atuador_alarme_T(double limite)
// {
//     pthread_mutex_lock(&exclusao_mutua);
//     limite_atual_t = limite;
//     while (atuador_lido[1] < limite_atual_t)
//     {
//         pthread_cond_wait(&limite_T, &exclusao_mutua);
//     }
//     limite_atual_t = HUGE_VAL;
//     pthread_mutex_unlock(&exclusao_mutua);
// }

// void atuador_alarme_H(double limite)
// {
//     pthread_mutex_lock(&exclusao_mutua);
//     limite_atual_h = limite;
//     while (atuador_lido[1] < limite_atual_h)
//     {
//         pthread_cond_wait(&limite_H, &exclusao_mutua);
//     }
//     limite_atual = HUGE_VAL;
//     pthread_mutex_unlock(&exclusao_mutua);
// }
