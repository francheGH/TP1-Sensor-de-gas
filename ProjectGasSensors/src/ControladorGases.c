#include "sapi.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "stdint.h"
#include "ControladorHora.h"
#include "mqX.h"
#include "MEF.h"
#define CANT_GASES 3

//arreglo con las concentraciones maximas teoricas impuestas por los programadores.
static int concentracion_maxima[CANT_GASES] = { 400, 400, 200 };
static int vectorAux[4] = { 0, 0, 0, 0 };
static int indiceVectorAux = 0;
char *gases[3] = {"CH4", "LPG", "C0"};


//unsigned char gases[CANT_GASES] = { "CH4", "LPG C4H10 C3H8", "CO" }; //arreglo de nombres de los gases
uint8_t gas_actual=0;
int concentracion_actual;
uint8_t i = 0;

keypad_t keypad;
bool_t valor;
uint16_t entrada[4], tecla; //valor que obtengo del teclado
const static uint16_t teclado[16] = { 1, 2, 3, 0x0a, 4, 5, 6, 0x0b, 7, 8, 9,
		0x0c, 0x0e, 0, 0x0f, 0x0d };
//arreglo donde van a estar las concentraciones de los gases tomadas por los sensores
int concentration_mq[CANT_GASES];
char str[16], str2[16];

void readSensor() {
	//lee los puertos y actualiza el arreglo de concentraciones

	concentration_mq[0] = sensorlecturamq(RL_MQ4, X0_MQ4, X1_MQ4, Y0_MQ4,
	Y1_MQ4, R0_MQ4, CH1); //MQ4
	concentration_mq[1] = sensorlecturamq(RL_MQ6, X0_MQ6, X1_MQ6, Y0_MQ6,
	Y1_MQ6, R0_MQ6, CH2);
	//MQ6
	concentration_mq[2] = sensorlecturamq(RL_MQ7, X0_MQ7, X1_MQ7, Y0_MQ7,
	Y1_MQ7, R0_MQ7, CH3);
	//MQ7

	//concentraciones[1]= adcRead ( CH2 ); //MQ6
	//concentraciones[2]= adcRead ( CH1 ); //MQ7

}
void limpiarFila(int row){
	if (row==1){
		lcdGoToXY(1,1);
		lcdSendStringRaw("                ");
	}else{
		lcdGoToXY(1,2);
		lcdSendStringRaw("                ");
	}

}
void imprimirGas() {

	 //supuestamente esto seria el recorrido ciclico del vector, verificar
	//concentracion_actual = (concentracion_actual + 1) % 3;
	limpiarFila(2);
	itoa(concentration_mq[gas_actual],str,10);
	lcdGoToXY(1,2);
	lcdSendStringRaw(gases[gas_actual]);
	lcdGoToXY(4,2);
	lcdSendStringRaw(str);

	gas_actual = (gas_actual + 1) % 3;
}


void actualizarConcentraciones(int mq, keypad_t keypad,int *fin) { //cambia los valores de total
	uint16_t teclaAux;
//	lcdClear();
//	lcdGoToXY(0, 1);	//Posiciono donde voy a escribir
//
//	itoa(mq, str, 10);
//
//	str2 = "Ingrese el limite del sensor ";
//	lcdSendStringRaw(str2);
	if (keypadRead(&keypad, &tecla)) {
		teclaAux = teclado[tecla];
		if ((teclaAux >= 0) && (teclaAux <= 9)) {
			vectorAux[indiceVectorAux] = teclaAux;
			indiceVectorAux++;
		}

	}
	if (indiceVectorAux == 4) {
		if (mq == 4) {

			concentracion_maxima[0] = (vectorAux[0] * 1000)
					+ (vectorAux[1] * 100) + (vectorAux[2] * 10) + vectorAux[3];
		} else if (mq == 6) {
			concentracion_maxima[1] = vectorAux[0] * 1000 + vectorAux[1] * 100
					+ vectorAux[2] * 10 + vectorAux[3];
		} else if (mq == 7) {

			concentracion_maxima[2] = vectorAux[0] * 1000 + vectorAux[1] * 100
					+ vectorAux[2] * 10 + vectorAux[3];
		}
		indiceVectorAux=0;
		*fin = 1;
	}

}

bool_t chequearGases() {
	int flag=0;
	if (concentracion_maxima[0] < concentration_mq[0] ) {
		//lcdClear();
		//lcdGoToXY(0, 1);	//Posiciono donde voy a escribir
		//lcdSendStringRaw("Alerta mq4");
		flag=1;

	}
	if (concentracion_maxima[1] < concentration_mq[1] ) {
		//lcdClear();
		//lcdSendStringRaw("Alerta mq6");
		flag=1;

	}
	if (concentracion_maxima[2] < concentration_mq[2]) {
		//lcdClear();
		//lcdSendStringRaw("Alerta mq7");
		flag=1;

	}

	return flag;
}


