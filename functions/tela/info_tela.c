#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../monitores/monitor_tela.h"
#include "../monitores/monitor_sensor.h"
#include "../buffers/buffer_duplo_sensores.h"
#include "../buffers/buffer_duplo_resposta.h"
#include "../socket/socket.h"

#define NSEC_PER_SEC (1000000000) // Numero de nanosegundos por segundos
static double limite_T = 30;
static double T_ref = 0;
static double H_ref = 0;

void thread_le_teclado(void) 
{
	aloca_tela();
    printf("#-*********************************************-#\n");
	printf("Digite um valor de referencia de Temperatura:  \n");
    scanf("%lf", &T_ref);
    printf(" Digite um valor de referencia de nivel da agua:  \n");
    scanf("%lf", &H_ref);
	libera_tela();
}

void thread_mostra_status(void)
{
    while (1)
    {
        sleep(1);
        aloca_tela();
        printf("\33[H\33[2J");
        printf("#-**********************************-#\n");
        printf("#\tSensor Ta => %lf \n", sensor_get(0));
        printf("#\tSensor T  => %lf \n", sensor_get(1));
        printf("#\tSensor Ti => %lf \n", sensor_get(2));
        printf("#\tSensor No => %lf \n", sensor_get(3));
        printf("#\tSensor H  => %lf \n", sensor_get(4));
        printf("#-**********************************-#\n");
        libera_tela();
    }
}

void thread_le_sensor(void)
{
    char msg_enviada[1000];
    while (1)
    {
        strcpy(msg_enviada, "sta0");            // Sensor Ta
        sensor_put(msg_socket(msg_enviada), 0); // Aloca o valor do sensor Ta
        strcpy(msg_enviada, "st-0");            // Sensor Ta
        sensor_put(msg_socket(msg_enviada), 1); // Aloca o valor do sensor Ta
        strcpy(msg_enviada, "sti0");            // Sensor Ta
        sensor_put(msg_socket(msg_enviada), 2); // Aloca o valor do sensor Ta
        strcpy(msg_enviada, "sno0");            // Sensor Ta
        sensor_put(msg_socket(msg_enviada), 3); // Aloca o valor do sensor Ta
        strcpy(msg_enviada, "sh-0");            // Sensor Ta
        sensor_put(msg_socket(msg_enviada), 4); // Aloca o valor do sensor Ta
    }
}

void thread_alarme(void)
{
    while (1)
    {
        sleep(1);
        sensor_alarme(limite_T); // Alarme para trinta graus em sensor T
        aloca_tela();
        printf("!!!Alarme!!!\n");
        libera_tela();
    }
}

void controle_temperatura(void)
{
    struct timespec t, t_fim;
    long periodo = 50000000; // 50ms

    // Le hora atual e coloca em t
    clock_gettime(CLOCK_MONOTONIC, &t);

    // Tarefa periodica comecara em 1 segundo
    t.tv_sec++;

    while (1)
    {
        // Espera ate o inicio do proximo periodo
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

        // algo para fazer
        if (sensor_get(1) < T_ref)
        {
        }

        // Le a hora atual e coloca em t_fim
        clock_gettime(CLOCK_MONOTONIC, &t_fim);

        // Calcula o tempo de resposta
        double temp_resp = (double)1000000 * (t_fim.tv_sec - t.tv_sec) + (t_fim.tv_nsec - t.tv_nsec) / 1000;

        bufduplo_insereLeitura_resposta(temp_resp);

        //Calcula o inicio do proximo periodo
        t.tv_nsec += periodo;
        while (t.tv_nsec >= NSEC_PER_SEC)
        {
            t.tv_nsec -= NSEC_PER_SEC;
            t.tv_sec++;
        }
    }
}

void controle_agua(void)
{
    struct timespec t, t_fim;
    long periodo = 70000000; // 70ms

    // Le hora atual e coloca em t
    clock_gettime(CLOCK_MONOTONIC, &t);

    // Tarefa periodica comecara em 1 segundo
    t.tv_sec++;

    while (1)
    {
        // Espera ate o inicio do proximo periodo
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

        // algo para fazer
        if (sensor_get(4) < H_ref)
        {
        }

        // Le a hora atual e coloca em t_fim
        clock_gettime(CLOCK_MONOTONIC, &t_fim);

        // Calcula o tempo de resposta
        double temp_resp = (double)1000000 * (t_fim.tv_sec - t.tv_sec) + (t_fim.tv_nsec - t.tv_nsec) / 1000;

        bufduplo_insereLeitura_resposta(temp_resp);

        //Calcula o inicio do proximo periodo
        t.tv_nsec += periodo;
        while (t.tv_nsec >= NSEC_PER_SEC)
        {
            t.tv_nsec -= NSEC_PER_SEC;
            t.tv_sec++;
        }
    }
}

void controle_amostra_sensores(void)
{
    int N_AMOSTRAS = 1;

    int i[3] = {1, 3, 4}; // Posicoes de T, No, H
    int aux = 0;

    struct timespec t, t_fim;
    long periodo = 200000000; // 200ms

    // Le hora atual e coloca em t
    clock_gettime(CLOCK_MONOTONIC, &t);

    // Tarefa periodica comecara em 1 segundo
    t.tv_sec++;

    while (N_AMOSTRAS <= 150)
    {
        // Espera ate o inicio do proximo periodo
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

        if (aux < 3)
        {
            bufduplo_insereLeitura_sensores(sensor_get(i[aux]));
            N_AMOSTRAS++;
            aux++;
            if (aux == 3)
                aux = 0;
        }
        // Le a hora atual e coloca em t_fim
        clock_gettime(CLOCK_MONOTONIC, &t_fim);

        //Calcula o inicio do proximo periodo
        t.tv_nsec += periodo;
        while (t.tv_nsec >= NSEC_PER_SEC)
        {
            t.tv_nsec -= NSEC_PER_SEC;
            t.tv_sec++;
        }
    }
}

void thread_grava_arquivo_sensores(void)
{
    FILE *dados_sensores;
    dados_sensores = fopen("data/dados_sensores.txt", "w");
    if (dados_sensores == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    while (1)
    {
        double *buffer = bufduplo_esperaBufferCheio_sensores();
        aloca_tela();
        printf("Gravando no arquivo...\n");
        while (*buffer != '\0')
        {
            fprintf(dados_sensores, "%lf ", *buffer++);
            fprintf(dados_sensores, "%lf ", *buffer++);
            fprintf(dados_sensores, "%lf \n", *buffer++);
            fflush(dados_sensores);
        }

        libera_tela();
    }
}

void controle_amostra_resposta(void)
{
    int N_AMOSTRAS = 1;
    struct timespec t, t_fim;
    long periodo = 200000000; // 200ms

    // Le hora atual e coloca em t
    clock_gettime(CLOCK_MONOTONIC, &t);

    // Tarefa periodica comecara em 1 segundo
    t.tv_sec++;

    while (N_AMOSTRAS <= 50)
    {
        // Espera ate o inicio do proximo periodo
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

        // Le a hora atual e coloca em t_fim
        clock_gettime(CLOCK_MONOTONIC, &t_fim);

        // Calcula o tempo de resposta
        double temp_resp = (double)1000000 * (t_fim.tv_sec - t.tv_sec) + (t_fim.tv_nsec - t.tv_nsec) / 1000;

        bufduplo_insereLeitura_resposta(temp_resp);
        N_AMOSTRAS++;

        //Calcula o inicio do proximo periodo
        t.tv_nsec += periodo;
        while (t.tv_nsec >= NSEC_PER_SEC)
        {
            t.tv_nsec -= NSEC_PER_SEC;
            t.tv_sec++;
        }
    }
}

void thread_grava_arquivo_resposta(void)
{
    FILE *dados_resposta;
    dados_resposta = fopen("data/dados_resposta.txt", "w");
    if (dados_resposta == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    while (1)
    {
        double *buffer_r = bufduplo_esperaBufferCheio_resposta();
        aloca_tela();
        printf("Gravando no arquivo...\n");
        while (*buffer_r != '\0')
        {
            fprintf(dados_resposta, "%lf \n", *buffer_r++);
            fflush(dados_resposta);
        }

        libera_tela();
    }
}
