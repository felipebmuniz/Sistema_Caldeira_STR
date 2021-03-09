#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions/tela/info_tela.h"
#include "functions/socket/socket.h"

int main(int argc, char *argv[])
{
    cria_socket(argv[1], atoi(argv[2]));

    pthread_t t0, t1, t2, t3, t4, t5, t6, t7;

    pthread_create(&t0, NULL, (void *)thread_le_teclado, NULL);
    pthread_create(&t1, NULL, (void *)thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *)thread_le_sensor, NULL);
    pthread_create(&t3, NULL, (void *)thread_alarme, NULL);
    pthread_create(&t4, NULL, (void *)controle_amostra_sensores, NULL);
    pthread_create(&t5, NULL, (void *)thread_grava_arquivo_sensores, NULL);
    pthread_create(&t6, NULL, (void *)controle_amostra_resposta, NULL);
    pthread_create(&t7, NULL, (void *)thread_grava_arquivo_resposta, NULL);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    pthread_join(t6, NULL);
    pthread_join(t7, NULL);
}