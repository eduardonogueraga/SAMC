/**
 * SISTEMA DE ALARMA DEL MOLINO CHICO
 * Version: VE01R0
 * Leyenda: @develop  @PEND (Pendiente de adaptar a ESP32 Wrober)
 *
 * POR HACER:
 *
 *  Llamadas unicamente sin SMS + control de llamadas
 *  Reparar restauraciones
 *  Implementar reseteo GSM
 *	Comuninacion con SAA
 *
 *  Sistema de toques bocina + tiempo de operancion bocina tras salto
 *
 * INCIDENCIAS
 *
 */


#include "Arduino.h"
#include "Alarma.h"

void EstadoInicio(){
	procesoCentral = ALARMA;
	estadoAlarma = ESTADO_REPOSO;
#ifdef ALARMA_EN_MODO_DEBUG
	printSystemInfo();
#endif
}

void setup()
{
	Serial.begin(115200);
	mensaje.inicioGSM();
	Serial.println(version[0]);

	//Restaurar configuracion almacenada
	configSystem = NVS_RestoreData<configuracion_sistema_t>("CONF_SYSTEM");

	//Inicio de perifericos
	iniciarTecladoI2C();

	//Declaracion de los puertos I/O //@PEND
	//MG
	pinMode(MG_SENSOR, INPUT);

	//MODULO GSM
	pinMode(GSM_PIN, OUTPUT);
	digitalWrite(GSM_PIN, HIGH);

	//PUERTOS INTERNOS
	pinMode(BOCINA_PIN, OUTPUT);
	pinMode(RELE_AUXILIAR, OUTPUT);


	//Configuracion de los puertos
	digitalWrite(LED_COCHERA, LOW);
	digitalWrite(RS_CTL,LOW);

	EstadoInicio();
	cargarEstadoPrevio();
	checkearAlertasDetenidas();

	//TEST WIFI
#ifdef WIFI_PUERTO_SERIE
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssidWifi, passwordWifi);
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.printf("WiFi Failed!\n");
		//return;
	}else {
		Serial.print("IP Address: ");
		Serial.print(WiFi.localIP());
		Serial.println("/webserial");
		// WebSerial is accessible at "<IP Address>/webserial" in browser
		WebSerial.begin(&serverDos);
		WebSerial.msgCallback(recvMsg);
		serverDos.begin();
	}
#endif
	//TEST WIFI
}

void loop()
{
	leerEntradaTeclado();
	demonio.demonioSerie();
	procesosSistema();
	procesoAlarma();

}

void procesosSistema(){
	watchDog();
	resetearAlarma();
}

void procesoAlarma(){

	switch(estadoAlarma){

	case ESTADO_REPOSO:

		//pantalla.lcdLoadView(&pantalla, &Pantalla::lcdReposo); @PEND Luz verde
		if(auth.isPasswordCached()){
			if (key != NO_KEY){
				if(key == '*'){
					setEstadoGuardia();
				}

				if(key == 'B'){ //Tecla menu
					procesoCentral = MENU;
				}
			}
		}

		break;

	case ESTADO_GUARDIA:

		//pantalla.lcdLoadView(&pantalla, &Pantalla::lcdGuardia); @PEND Luces RGB

		if(checkearMargenTiempo(tiempoMargen)){
			//Sensor MG
			//Comprobar sensor MG y Vibracion

			setEstadoAlerta();
		}

		sonarBocina();
		desactivarAlarma();

		break;

	case ESTADO_ALERTA:

		//pantalla.lcdLoadView(&pantalla, &Pantalla::lcdAlerta); @PEND Luces RGB

		if(checkearMargenTiempo(tiempoMargen)){

			setEstadoEnvio();
		}

		desactivarAlarma();

		break;

	case ESTADO_ENVIO:


		//pantalla.lcdLoadView(&pantalla, &Pantalla::lcdAvisoEnviado); @PEND Luces RGB

		//En este punto ya no queda mas nada que hacer
		//esperar la ayuda y sonar la bocina como el demonio

		realizarLlamadas();
		sonarBocina();
		desactivarAlarma();

		break;

	}
}


void setEstadoGuardia()
{
	Serial.println(F("\nAlarma Activada"));
	estadoAlarma = ESTADO_GUARDIA;
	guardarFlagEE("ESTADO_GUARDIA", 1);

	setMargenTiempo(tiempoMargen,TIEMPO_ON, TIEMPO_ON_TEST);
}


void setEstadoAlerta()
{
	Serial.print("\nIntrusismo detectado puerta abierta");
	estadoAlarma = ESTADO_ALERTA;

	guardarFlagEE("ESTADO_ALERTA", 1);
}

void setEstadoEnvio()
{
	Serial.println(F("\nTiempo acabado \nAVISO ENVIADO"));
	estadoAlarma = ESTADO_ENVIO;

	setMargenTiempo(tiempoBocina, TIEMPO_BOCINA, TIEMPO_BOCINA_TEST);
	setMargenTiempo(tiempoMargen,TIEMPO_REACTIVACION, TIEMPO_REACTIVACION_TEST);


	//Desde aqui se envian los correspondientes avisos
	//mensaje.mensajeAlerta(); //@PEND Cambiar por mensaje unico

	/*Llamada al SAA para ditribuir los avisos al servidor*/

	guardarFlagEE("ESTADO_ALERTA", 0);
}

void setEstadoReposo()
{
	Serial.println(F("\nAlarma Desactivada"));
	estadoAlarma = ESTADO_REPOSO;

	pararBocina();
	estadoLlamada = TLF_1;

	guardarFlagEE("ESTADO_GUARDIA", 0);
	guardarFlagEE("ESTADO_ALERTA", 0);
	guardarFlagEE("TICKET_BOCINA", 0);
}


