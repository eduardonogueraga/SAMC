/*
 * Checker.h
 *
 *  Created on: 2 sept. 2023
 *      Author: isrev
 *
 * En este fichero se organizan los metodos encargados de checkear distintos eventos
 * en la alarma
 *
 */

#ifndef MUSIC_SOURCE_ALARMAUTIL_CHECKER_H_
#define MUSIC_SOURCE_ALARMAUTIL_CHECKER_H_


void checkearAlertasDetenidas(){ //@PEND
		if (leerFlagEEInt("ESTADO_ALERTA") == 1 && leerFlagEEInt("ERR_INTERRUPT") == 0) {

			flagAlertaRestaurada =1;
			eeDatosSalto = NVS_RestoreData<datos_saltos_t>("SALTO_DATA");

			Serial.print("\nIntrusismo restaurado");

			estadoAlarma = ESTADO_ALERTA;
		}
}


#endif /* MUSIC_SOURCE_ALARMAUTIL_CHECKER_H_ */
