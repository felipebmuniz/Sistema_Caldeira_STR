#ifndef MONITOR_SENSOR_H
#define MONITOR_SENSOR_H

void sensor_put(double lido, int i);

double sensor_get(int i);

void sensor_alarme(double limite);

#endif