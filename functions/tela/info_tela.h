#ifndef INFO_TELA_H
#define INFO_TELA_H

void thread_le_teclado(void);

void thread_mostra_status(void);

void thread_le_sensor(void);

void thread_alarme(void);

void controle_amostra_sensores(void);

void thread_grava_arquivo_sensores(void);

void controle_amostra_resposta(void);

void thread_grava_arquivo_resposta(void);

#endif