/*
 * ComandoSerie.cpp
 *
 *  Created on: 29 jul. 2021
 *      Author: isrev
 */

#include "ComandoSerie.h"


ComandoSerie::ComandoSerie() {
	// TODO Auto-generated constructor stub
}

void ComandoSerie::demonioSerie(){

	if (Serial.available() > 0 ){

		if(Serial.available() > 0)
		comandoRecibido = Serial.readStringUntil('\n');
		comandoRecibido.trim();
		comandoRecibido.toCharArray(data, sizeof(data));

		comprobarComando();
	}

}

boolean ComandoSerie::compararCadena(const char* data, const char* cadena) {
  return strcmp(data, cadena) == 0;
}

void ComandoSerie::nombreComando(const char* data){
	Serial.print("demonio@saa:~$	");
	Serial.println(data);
}


void ComandoSerie::esperarRespuestaUart2(){
	for (int i = 0; i < 10; i++)
	{
		// Verificamos si hay datos disponibles para leer

		char tramaRecibida[200] = "";
		size_t byteCount = UART_GSM.readBytesUntil('\n', tramaRecibida, sizeof(tramaRecibida) - 1); //read in data to buffer
		tramaRecibida[byteCount] = NULL;	//put an end character on the data


		Serial.print("UART@GSM-> ");
		Serial.println(tramaRecibida);
		UART_GSM.flush();
		i++;
	}
}

void ComandoSerie::comprobarComando() {

	if (compararCadena(data, "help")) {
		nombreComando(data);
		mostrarAyuda();
	}

	if (compararCadena(data, "set on")) {
		nombreComando(data);
		setEstadoGuardia();
	}

	if (compararCadena(data, "set off")) {
		nombreComando(data);
		setEstadoReposo();
		//desactivarAlarma();
	}

	if (compararCadena(data, "set mode")) {
		nombreComando(data);

		if(MODO_DEFAULT){
			Serial.println("Alarma en modo de pruebas");
			MODO_DEFAULT = 0;
		}else {
			Serial.println("Alarma en modo default");
			MODO_DEFAULT = 1;
		}
	}

	if (compararCadena(data, "mg")) {
		nombreComando(data);
		//sensorCore.sensorMG = 1; @PEND
		Serial.println("Comando pendiente de adpatacion");
	}

	if (compararCadena(data, "alert")) {
		nombreComando(data);
		setEstadoAlerta();
	}

	if (compararCadena(data, "mail")) {
		nombreComando(data);
		setEstadoEnvio();
	}


	if(compararCadena(data, "llamar")){
		nombreComando(data);
		llamarTlfAPI((char*)telefonoLlamada_1);
	}

	if(compararCadena(data, "colgar")){
		nombreComando(data);
		colgarLlamadaAPI();
	}


	if(compararCadena(data, "at")){
		nombreComando(data);
		UART_GSM.println("AT");
		UART_GSM.println("AT+COPS?");

		esperarRespuestaUart2();
	}

	if(compararCadena(data, "echo ?")){
		nombreComando(data);
		UART_GSM.println("ATE?");

		esperarRespuestaUart2();
	}


	if(compararCadena(data, "speed ?")){
		nombreComando(data);
		UART_GSM.println("AT+IPR?");

		esperarRespuestaUart2();

	}

	if(compararCadena(data, "speed -c")){
		nombreComando(data);
		UART_GSM.println("AT+IPR=115200");
		UART_GSM.println("AT&W");
		UART_GSM.println("AT+CFUN=1,1");
	}

	if(compararCadena(data, "info")){
		nombreComando(data);
		printSystemInfo();
	}

	if(compararCadena(data, "bye")){
		nombreComando(data);
		resetear();
	}

}

void ComandoSerie::mostrarAyuda() {
  Serial.println("Comandos disponibles:");
  Serial.println("set on - Encender alarma");
  Serial.println("set off - Apagar alarma");
  Serial.println("set mode - Cambiar modo de alarma");
  Serial.println("mg - Simular Puerta");
  Serial.println("mail - Test para Estado Envio");
  Serial.println("at - Enviar comando AT a GSM");
  Serial.println("echo ? - Consultar configuracion de eco GSM");
  Serial.println("speed ? - Consultar velocidad de GSM");
  Serial.println("speed -c - Cambiar velocidad de GSM");
  Serial.println("pila -at - Probar timeout en pila de tareas para el primer elemento");
  Serial.println("info - Mostrar informacion del sistema");
  Serial.println("bye - Reiniciar el sistema");

}










