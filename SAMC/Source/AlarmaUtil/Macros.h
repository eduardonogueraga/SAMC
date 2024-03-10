/*
 * Macros.h
 *
 *  Created on: 18 mar. 2023
 *      Author: isrev
 */

#ifndef SOURCE_MACROS_H_
#define SOURCE_MACROS_H_


//MACROS
//#define ALARMA_EN_MODO_DEBUG //Nunca definir en produccion
#define SENSOR_MG_CONECTADO //Definir cuando la alarma este conectada al sensor real
//#define WIFI_PUERTO_SERIE //Definir solo para debug


#define TIEMPO_OFF_TEST 0.1666
#define TIEMPO_ON_TEST 0.01
#define TIEMPO_REACTIVACION_TEST 0.1
#define TIEMPO_MODO_SENSIBLE_TEST 0.0166
#define TIEMPO_OFF_MODO_SENSIBLE 0.1666
#define TIEMPO_PRORROGA_GSM_TEST 0.05
#define TIEMPO_BOCINA_TEST 0.0333
#define TIEMPO_BOCINA_REACTIVACION_TEST 0.0666

//TECLADO
#define ROW_1 36
#define ROW_2 34
#define ROW_3 32
#define ROW_4 30
#define COL_1 38
#define COL_2 40
#define COL_3 42

//PANTALLA
#define CODEC_LCD 0x25 //  0x3F 0x27

//SENSORES
#define PIR_SENSOR_1 5
#define PIR_SENSOR_2 7
#define PIR_SENSOR_3 6
#define MG_SENSOR 	 4

//BOCINA
#define BOCINA_PIN 0

//WATCHDOG
#define WATCHDOG 3

//RS485
#define RS_CTL 13

//CONTROL RELES 12V
#define RELE_AUXILIAR 10

//SLEEPMODE
#define GSM_PIN 2


#define GSM_TX 18
#define GSM_RX 5



#define TEST_PIN_RS 13

//RESET
#define RESETEAR 9

//LED PUERTA
#define LED_COCHERA 8

//SISTEMA
#define TIEMPO_ALERT_LCD 6000
#define	NUMERO_ALERTAS 4
#define TOTAL_SENSORES 4 //EL numero de sensores activos
#define MAX_SIZE_JSON 2100 //Bytes maximos del modelo JSON
#define MAX_TOQUES_BOCINA_RESTANTES 15 //Intentos en modo inquieto
#define MAX_INTENTOS_REACTIVACION 3


//DEFINICIONES DE FUNCIONES
void printSystemInfo();

void guardarFlagEE(const char* key, int value);
void guardarFlagEE(const char* key, uint8_t value);
//uint8_t leerFlagEE(const char* key); //Deprecated
int leerFlagEEInt(const char* key);
String leerCadenaEE(const char* key);
void guardarCadenaEE(const char* key, String* value);
void guardarEstadoInterrupcion();

template <typename T> void NVS_SaveData(const char* key, T value);
template <typename T> T NVS_RestoreData(const char* key);

String fixedLengthString(String& original, size_t fixedLength);


//STRUCTS
struct listas_terminales_json{
	 char terminalCoreJson[2048];
};

typedef struct listas_terminales_json TerminalListJson;

struct datos_saltos_t{
	byte ID_SENSOR;
	byte ID_TERMINAL;
	byte INTENTOS_REACTIVACION = 0;
	TerminalListJson LISTADOS_TERMINALES;
};

typedef struct datos_saltos_t EE_DatosSalto;

struct configuracion_sistema_t {

	byte MODO_SENSIBLE = 1;
	byte MODULO_SD = 1;
	byte MODULO_RTC = 1;
	byte ENVIO_SAAS = 1;
	byte ENVIO_SAAS_NOTIFICACION = 1;
	byte ENVIO_FTP = 1;
	byte ESPERA_SAAS_MULTIPLICADOR = 0;
	byte ESCUCHAR_LINEA = 1;
	byte SENSORES_HABLITADOS[4] = {1,1,1,1};
	byte SMS_HISTORICO;
	char FECHA_SMS_HITORICO[30];

};

 typedef struct configuracion_sistema_t ConfigSystem;

 typedef struct  {
     int intensidadSignal = 0;
     String proveedor = "";
 } ProveedorEstado;


//ENUM Y TYPEDEFS
enum aliasConfigSistema{MD_SENS, MDL_SD, MDL_RTC};
enum ZonasSensor{MG,PIR_1,PIR_2,PIR_3};

typedef enum {
	ALARMA,
	MENU,
	ERROR
} ProcesoCentral;

typedef enum {
	ESTADO_REPOSO,
	ESTADO_GUARDIA,
	ESTADO_ALERTA,
	ESTADO_ENVIO,
	ESTADO_INQUIETO
} EstadosAlarma;


typedef enum {
	TLF_1,
	TLF_2,
	COLGAR
} LLAMADAS_GSM;




//TEMPLATES GLOBALES
template <class T> uint8_t arrSum(byte arr[], byte tam){
	byte r = 0;

	for (int i = 0; i <tam; i++) {
		r += arr[i];
	}
	return r;
}


template <class T> void arrCopy(byte origen[], byte destino[], byte tam) {
	byte *p = origen;
	byte *q = destino;
	while (p != origen + tam)
		*q++ = *p++;
}

template <class T> void arrCopy(int origen[], int destino[], byte tam) {
	int *p = origen;
	int *q = destino;
	while (p != origen + tam)
		*q++ = *p++;
}

#endif /* SOURCE_MACROS_H_ */
