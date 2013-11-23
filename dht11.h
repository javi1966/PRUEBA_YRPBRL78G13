#ifndef __DHT11_H
#define __DHT11_H

#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"

//datos para reloj 32Mhz,ciclo instruccion 32M/4
#define MIN_PULSE_START 1200
#define MAX_PULSE_START 1400
#define MIN_PULSE_CERO  560
#define MAX_PULSE_CERO  720
#define MIN_PULSE_UNO   880
#define MAX_PULSE_UNO   1040

#define START 0
#define DATO  1
#define FIN   2


//prototipos

unsigned char readTemperatura(void);
unsigned char readHumedad(void);
unsigned char getCaptura();
unsigned char setCaptura(unsigned char );

#endif