/*
 * Mensajes.h
 *
 *  Created on: 23 jun. 2020
 *      Author: isrev
 *
 */

#ifndef PROYECTO_MENSAJES_H_
#define PROYECTO_MENSAJES_H_

#include "Arduino.h"
#include <HardwareSerial.h>

#include "AlarmaUtil/Macros.h"
#include "Env.h"

extern byte MODO_DEFAULT;
extern void colgarLlamadaAPI();
extern void llamarTlfAPI(char* tlf);


class Mensajes {

private:

	HardwareSerial& UART_GSM;
	const byte LIMITE_MAXIMO_SMS = 15;

public:
	Mensajes(HardwareSerial& serialInstance);
	void inicioGSM();
	void llamarTlf(char* tlf);
	void colgarLlamada();
};

#endif /* PROYECTO_MENSAJES_H_ */
