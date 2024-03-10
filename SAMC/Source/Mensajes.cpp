/*
 * Mensajes.cpp
 *
 *  Created on: 06 ago. 2020
 *      Author: isrev
 *
 */

#include "Mensajes.h"


Mensajes::Mensajes(HardwareSerial& serialInstance) : UART_GSM(serialInstance){}


void Mensajes::inicioGSM(){
	UART_GSM.begin(115200, SERIAL_8N1, GSM_RX, GSM_TX); 	//RX TX  (H2 = RX23 TX19)
}


void Mensajes::llamarTlf(char* tlf){

#ifdef ALARMA_EN_MODO_DEBUG
	if(MODO_DEFAULT) //@develop !MODO_DEFAULT
		return;
#else
	if(!MODO_DEFAULT)
		return;
#endif

	if(leerFlagEEInt("N_SMS_ENVIADOS") >= (LIMITE_MAXIMO_SMS-7)){ //@PEND AJUSTAR CONTROL DE LLAMADAS
		Serial.println(F("Intentos diarios acabados")); //No se haran mas llamadas
		return;
	}

	Serial.println("Llamando "+(String)tlf);
	llamarTlfAPI(tlf);

}

void Mensajes::colgarLlamada(){
	Serial.println("Llamada finalizada");
	colgarLlamadaAPI();
}


