#ifndef MONITOR_CONTROLE_H
#define MONITOR_CONTROLE_H

void atuador_put(double lido, int i);

double atuador_get(int i);

void atuador_alarme(double limite);

#endif