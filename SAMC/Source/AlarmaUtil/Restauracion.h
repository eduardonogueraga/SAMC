/*
 * Restauracion.h
 *
 *  Created on: 2 sept. 2023
 *      Author: Eduardo
 *
 *      Funciones encargadas de restuarar el estado previo tras reinicio
 *      inesperado
 */

#ifndef SOURCE_ALARMAUTIL_RESTAURACION_H_
#define SOURCE_ALARMAUTIL_RESTAURACION_H_


void cargarEstadoPrevio(){
	flagPuertaAbierta = leerFlagEEInt("PUERTA_ABIERTA") == 1;


	if (leerFlagEEInt("ESTADO_GUARDIA") == 1) {
		Serial.println("Estado de guardia restaurado");
		estadoAlarma = ESTADO_GUARDIA;

		//Informar de que se ha restaurado una entrada
		guardarFlagEE("F_RESTAURADO", 1);
	}


}

void guardarEstadoInterrupcion(){
	guardarFlagEE("ERR_INTERRUPT", 1);
	guardarFlagEE("INTERUP_HIST", (leerFlagEEInt("INTERUP_HIST") + 1));
}

#endif /* SOURCE_ALARMAUTIL_RESTAURACION_H_ */
