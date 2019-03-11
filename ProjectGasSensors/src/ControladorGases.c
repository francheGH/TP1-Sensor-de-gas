#include "sapi.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "stdint.h"
#include "ControladorHora.h"
#include "mqX.h"
//#include "MEF.h"
#define CANT_GASES 3

//arreglo con las concentraciones maximas teoricas impuestas por los programadores.
static int concentracion_maxima[CANT_GASES] = { 400, 400, 1500 };
static int vectorAux[4] = { 0, 0, 0, 0 };
static int indiceVectorAux = 0;
char *gases[5] = { "CH4 :", "LPG :", "C0 :" };

//unsigned char gases[CANT_GASES] = { "CH4", "LPG C4H10 C3H8", "CO" }; //arreglo de nombres de los gases
uint8_t gas_actual = 0;
int concentracion_actual;
uint8_t i = 0;
static int flagTimer=0, cont=0;
keypad_t keypad;
bool_t valor;
uint16_t entrada[4], tecla1 = 0; //valor que obtengo del teclado
const static uint16_t teclado[16] = { 1, 2, 3, 0x0a, 4, 5, 6, 0x0b, 7, 8, 9,
		0x0c, 0x0e, 0, 0x0f, 0x0d };
//arreglo donde van a estar las concentraciones de los gases tomadas por los sensores
int concentration_mq[CANT_GASES];
char str[16];
static int timer = -3;
int flag = 0;
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

void imprimirGas() {

	int fseg;
	//supuestamente esto seria el recorrido ciclico del vector, verificar
	//concentracion_actual = (concentracion_actual + 1) % 3;

	limpiarFila(2);
	itoa(concentration_mq[gas_actual], str, 10);
	lcdGoToXY(1, 2);
	lcdSendStringRaw(gases[gas_actual]);
	lcdGoToXY(6, 2);
	lcdSendStringRaw(str);
	if (timer++ == 7) {
		fseg = 1;
		timer = -3;
	}

	//
	if (fseg == 1) {
		//limpiarFila(2);
		gas_actual = (gas_actual + 1) % 3;
		fseg = 0;
	}

}

void actualizarConcentraciones(int mq, keypad_t keypad2, int *fin) { //cambia los valores de total
	uint16_t teclaAux;
	uint16_t x;
	lcdClear();
	lcdGoToXY(1, 1);	//Posiciono donde voy a escribir
	keypad = keypad2;
	lcdSendStringRaw("Limite Sensor:");
	//delay(50);
	if  (keypadRead(&keypad, &tecla1)) {
		teclaAux = teclado[tecla1];
		if ((teclaAux >= 0) && (teclaAux <= 9)) {
			vectorAux[indiceVectorAux] = teclaAux;
			indiceVectorAux++;
		}

	}
	if (indiceVectorAux == 4) {

		if (mq == 4) {
			x = 0;
			concentracion_maxima[0] = (vectorAux[0] * 1000)
					+ (vectorAux[1] * 100) + (vectorAux[2] * 10) + vectorAux[3];
		} else if (mq == 6) {
			x = 1;
			concentracion_maxima[1] = vectorAux[0] * 1000 + vectorAux[1] * 100
					+ vectorAux[2] * 10 + vectorAux[3];
		} else if (mq == 7) {
			x = 2;
			concentracion_maxima[2] = vectorAux[0] * 1000 + vectorAux[1] * 100
					+ vectorAux[2] * 10 + vectorAux[3];
		}

		itoa(concentracion_maxima[x], str, 10);
		lcdGoToXY(1, 2);
		lcdSendStringRaw(str);

		if(cont++==5)
		{
			flagTimer=1;
			cont=0;
		}
		if(flagTimer){
			indiceVectorAux = 0;
			flagTimer=0;
			*fin = 1;
		}
	}

}

bool_t chequearGases() {
	char str[5];
	flag = 0;
	if (concentracion_maxima[0] < concentration_mq[0]) {
		lcdClear();	//Posiciono donde voy a escribir
		lcdSendStringRaw("Alerta mq4");
		flag = 1;
		lcdGoToXY(12, 1);
		itoa(concentration_mq[2], str, 10);
		lcdSendStringRaw(str);
	}

	if (concentracion_maxima[1] < concentration_mq[1]) {
		lcdClear();
		lcdSendStringRaw("Alerta mq6");
		flag = 1;
		lcdGoToXY(12, 1);
		itoa(concentration_mq[1], str, 10);
		lcdSendStringRaw(str);

	}
	if (concentracion_maxima[2] < concentration_mq[2]) {
		lcdClear();
		lcdSendStringRaw("Alerta mq7");
		lcdGoToXY(12, 1);
		itoa(concentration_mq[2], str, 10);
		flag = 1;
		lcdSendStringRaw(str);

	}

	return flag;
}

void limpiarFila(int row) {
	if (row == 1) {
		lcdGoToXY(1, 1);
		lcdSendStringRaw("                ");
	} else {
		lcdGoToXY(1, 2);
		lcdSendStringRaw("                ");
	}

}


//
//if ((indiceVectorAux >= 4) && (keypadRead(&keypad, &tecla1))) {
//			teclaAux = teclado[tecla1];
//			if ((teclaAux == 0x0d)) {
//				indiceVectorAux = 0;
//				*fin = 1;
//			}
//		}
