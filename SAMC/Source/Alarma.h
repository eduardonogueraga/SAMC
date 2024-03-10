/*
 * Alarma.h
 *
 *  Created on: 3 ago. 2021
 *      Author: isrev
 */

#ifndef SOURCE_ALARMA_H_
#define SOURCE_ALARMA_H_

#define TINY_GSM_MODEM_SIM7600 //Definimos el modem @PEND Poner el sim800l

#include "Arduino.h"
#include <Keypad.h>
#include <Preferences.h>
#include <HardwareSerial.h>
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>


#include "Autenticacion.h"
#include "ComandoSerie.h"
#include "Bocina.h"
#include "AlarmaUtil/Teclado.h"
#include "Env.h"
#include "AlarmaUtil/Macros.h"
#include "Mensajes.h"

//TEST WIFI
#ifdef WIFI_PUERTO_SERIE
AsyncWebServer serverDos(80);

void recvMsg(uint8_t *data, size_t len){
	String d = "";
	for(int i=0; i < len; i++){
		d += char(data[i]);
	}
	WebSerial.println(d);
}
#endif
//TEST WIFI

//VERSION (VE -> Version Estable VD -> Version Desarrollo)
const char* version[] = {"PIONEER VE01R0", "XX/0X/24"};


//VARIABLES GLOBALES
ConfigSystem configSystem;
EE_DatosSalto eeDatosSalto;

#ifdef ALARMA_EN_MODO_DEBUG
byte MODO_DEFAULT = 0;  //@develop
#else
byte MODO_DEFAULT = 1;
#endif

//INSTANCIAS

//UART
HardwareSerial UART_GSM(1);

// Crea un objeto TinyGsm para comunicarse con el módulo SIM800L
TinyGsm modem(UART_GSM);
TinyGsmClient client(modem); // @suppress("Abstract class cannot be instantiated")
//TinyGsmClientSecure client(modem);


//NVS
Preferences NVSMemory; //Memoria

//TYPE DEF
ProcesoCentral procesoCentral;
EstadosAlarma estadoAlarma;
LLAMADAS_GSM estadoLlamada;


//CLASES
Autenticacion auth;
ComandoSerie demonio;
Bocina bocina;
Mensajes mensaje(UART_GSM);

//TIEMPOS MARGEN
const unsigned long TIEMPO_OFF = 120000; // (*0.1666) -> 20000 (*0.1666) -> 20000 sensible
const unsigned long TIEMPO_ON = 600000; //(*0.01666) -> 10000 (*0.01) -> 6000 en auto activacion
const unsigned long TIEMPO_REACTIVACION = 240000; // (*0.1) ->  24000
const unsigned long TIEMPO_MODO_SENSIBLE = 3600000; // (*0.0166)  -> 60000*
const unsigned long TIEMPO_BOCINA = 600000; // (*0.0333) -> 20000* //300000(*0.0666) ->20000
const unsigned long TIEMPO_PRORROGA_GSM = 1200000; // (*0.05) -> 60000
const unsigned long TIEMPO_CARGA_GSM = 10000;

unsigned long tiempoMargen;

//TIEMPO SLEEPMODE
unsigned long tiempoRefrescoGSM;
//TIEMPO BOCINA
unsigned long tiempoBocina;


//FLAG PUERTA
byte flagPuertaAbierta = 0;
//FLAG ALERTA RESTAURADA
byte flagAlertaRestaurada = 0;

//FUNCIONES//
void leerEntradaTeclado(){
	key = keypad.getKey();
	auth.comprobarEntrada();
}

void setMargenTiempo(unsigned long &tiempoMargen, const unsigned long tiempo, float porcentaje = 1.0F){

	if(MODO_DEFAULT == 1){
		porcentaje = 1.0F;
	}

	tiempoMargen = millis() + (tiempo * porcentaje);
}

bool checkearMargenTiempo(unsigned long tiempoMargen){

	return millis() > tiempoMargen;
}

void desactivarAlarma(){
	if(auth.isPasswordCached()){
		if (key != NO_KEY){
			if(key == '#'){
				setEstadoReposo();
			}
		}
	}
}


void sonarBocina(){
	if(!checkearMargenTiempo(tiempoBocina)){
		bocina.sonarBocina();
	}else{
		bocina.stopBocina();
	}
}

void pararBocina()
{
	bocina.stopBocina();
	tiempoBocina = 0;
}



void watchDog(){
	digitalWrite(WATCHDOG, !digitalRead(WATCHDOG));
}

void resetear(){
	Serial.println(F("\nReseteando"));
	digitalWrite(GSM_PIN, LOW); //Reseteo el modulo GSM
	delay(200);
	digitalWrite(RESETEAR, HIGH);
}


void resetearAlarma(){
	if(auth.isPasswordCached()){
		if (key != NO_KEY){
			if(key == '0'){
				setEstadoReposo();
				resetear();
			}
		}
	}
}


void realizarLlamadas(){

#ifdef ALARMA_EN_MODO_DEBUG
	if(MODO_DEFAULT) //@develop !MODO_DEFAULT
		return;
#else
	if(!MODO_DEFAULT)
		return;
#endif

	static byte estadoAnterior;

	switch(estadoLlamada){

	case TLF_1:

		if(millis() > tiempoMargen - (TIEMPO_REACTIVACION*0.95) && millis() < tiempoMargen - (TIEMPO_REACTIVACION*0.90)){
			mensaje.llamarTlf((char*)telefonoLlamada_1);
			estadoLlamada = COLGAR;
			estadoAnterior = TLF_1;
		}

		break;

	case TLF_2:

		if(millis() > tiempoMargen - (TIEMPO_REACTIVACION*0.80)){
			mensaje.llamarTlf((char*)telefonoLlamada_2);
			estadoLlamada = COLGAR;
			estadoAnterior = TLF_2;

		}

		break;

	case COLGAR:

		if((millis() > tiempoMargen - (TIEMPO_REACTIVACION*0.85))){
			if(estadoAnterior == TLF_1){
				mensaje.colgarLlamada();
				estadoLlamada = TLF_2;
			}
		}

		if((millis() > tiempoMargen - (TIEMPO_REACTIVACION*0.70))){
			if(estadoAnterior == TLF_2){
				mensaje.colgarLlamada();
				estadoLlamada = TLF_1;
			}
		}
		break;
	}
}

void printSystemInfo(){

	Serial.print("INFORMACION DEL SISTEMA\n"); //@PEND AJUSTAR NUMERO DE LLAMADAS HECHAS
	Serial.print("\n");

	Serial.printf("FLAG GUARDIA = %d\n", leerFlagEEInt("ESTADO_GUARDIA"));
	Serial.printf("FLAG ALERTA = %d\n", leerFlagEEInt("ESTADO_ALERTA"));
	Serial.printf("NUM TOQUES BOCINA = %d\n", leerFlagEEInt("TICKET_BOCINA"));

}


#include "AlarmaUtil/Utiles.h"
#include "AlarmaUtil/Restauracion.h"
#include "AlarmaUtil/Checker.h"


#endif /* SOURCE_ALARMA_H_ */
