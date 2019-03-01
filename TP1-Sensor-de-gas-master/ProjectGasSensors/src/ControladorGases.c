/*#include "sapi.h"

#define CANT_GASES 3
//arreglo donde van a estar las concentraciones de los gases tomadas por los sensores
uint16_t concentraciones[CANT_GASES];
//arreglo con las concentraciones maximas teoricas impuestas por los programadores.
uint16_t concentracion_maxima[CANT_GASES]={200,200,100};
unsigned char gases[CANT_GASES]={"Metano", "Propano", "Monoxido"}; //arreglo de nombres de los gases
uint8_t gas_actual;
uint16_t concentracion_actual;
uint8_t i=0;


void actualizarConcentraciones(){
	//lee los puertos y actualiza el arreglo de concentraciones
	concentraciones[0]= adcRead ( CH3 ); //MQ4
	concentraciones[1]= adcRead ( CH2 ); //MQ6
	concentraciones[2]= adcRead ( CH1 ); //MQ7
}
void initGases(){
	//inicializa los indices de los arreglos, configura el adc y actualiza el arreglo de concentraciones.
	concentracion_actual=0;
	gas_actual=0;
	adcConfig( ADC_ENABLE );
	actualizarConcentraciones();
}
void imprimirGas(){

	gas_actual=(gas_actual+1)%3; //supuestamente esto seria el recorrido ciclico del vector, verificar
	concentracion_actual=(concentracion_actual+1)%3;
}*/
