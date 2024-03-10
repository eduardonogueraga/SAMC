/*
 * ComandoSerie.h
 *
 *  Created on: 29 jul. 2021
 *      Author: isrev
 */

#ifndef SOURCE_COMANDOSERIE_H_
#define SOURCE_COMANDOSERIE_H_
#include "Arduino.h"
#include "AlarmaUtil/Macros.h"
#include <HardwareSerial.h>
#include "Env.h"


extern void setEstadoGuardia();
extern void setEstadoAlerta();
extern void setEstadoReposo();
extern void setEstadoEnvio();
extern void resetear();
extern void desactivarAlarma();

extern byte MODO_DEFAULT;
extern HardwareSerial UART_GSM;
extern void colgarLlamadaAPI();
extern void llamarTlfAPI(char* tlf);


class ComandoSerie {

private:
	String comandoRecibido;
	char data[100];
	void comprobarComando();
	boolean compararCadena(const char* data, const char* cadena);
	void nombreComando(const char* data);
	void esperarRespuestaUart2();
	void mostrarAyuda();

public:
	ComandoSerie();
	void demonioSerie();
};

#endif /* SOURCE_COMANDOSERIE_H_ */
