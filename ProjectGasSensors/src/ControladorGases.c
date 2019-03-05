
#include "sapi.h"

#include "stdio.h"
#include "math.h"
#include "stdint.h"
#include "ControladorHora.h"
#include "mqX.h"
#include "MEF.h"
#define CANT_GASES 3

//arreglo con las concentraciones maximas teoricas impuestas por los programadores.
int concentracion_maxima[CANT_GASES] = { 400, 400, 200 };
unsigned char gases[CANT_GASES] = { "CH4", "LPG C4H10 C3H8", "CO" }; //arreglo de nombres de los gases
uint8_t gas_actual;
int concentracion_actual;
uint8_t i = 0;

keypad_t keypad;
bool_t valor;
uint16_t entrada[4], tecla; //valor que obtengo del teclado

//arreglo donde van a estar las concentraciones de los gases tomadas por los sensores
int concentration_mq[CANT_GASES];
char str[16], str2[16];

void actualizarConcentraciones() {
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
void initGases() {
//inicializa los indices de los arreglos, configura el adc y actualiza el arreglo de concentraciones.
	concentracion_actual = 0;
	gas_actual = 0;

	adcConfig(ADC_ENABLE);
	actualizarConcentraciones();
	 imprimirGas();
}
void imprimirGas() {
	lcdGoToXY(0, 1);	//Posiciono donde voy a escribir
	itoa(concentration_mq[0], str, 10);
	lcdSendStringRaw(str);
	lcdSendStringRaw(gases[0]);

	lcdGoToXY(5, 1);	//Posiciono donde voy a escribir
	itoa(concentration_mq[1], str, 10);
	lcdSendStringRaw(str);
	lcdSendStringRaw(gases[1]);

	lcdGoToXY(10, 1);	//Posiciono donde voy a escribir
	itoa(concentration_mq[2], str, 10);
	lcdSendStringRaw(str);
	lcdSendStringRaw(gases[2]);

	gas_actual = (gas_actual + 1) % 3; //supuestamente esto seria el recorrido ciclico del vector, verificar
	 concentracion_actual = (concentracion_actual + 1) % 3;
}

void fmodo_A();

void fmodo_P(int mq, int *total[3]) {//cambia los valores de total

	lcdClear();
	lcdGoToXY(0, 1);	//Posiciono donde voy a escribir

	itoa(mq, str, 10);

	str2 = "Ingrese el limite del sensor ";
	lcdSendStringRaw(str2);
	if (mq == 4) {
		if (keypadRead(&keypad, &tecla)) {
			entrada[0] = tecla;
			entrada[1] = entrada[0];
			entrada[2] = entrada[1];
			entrada[3] = entrada[2];
			*total[0] = (entrada[0] * 1000 )+ (entrada[1] * 100 )+ (entrada[2] * 10) + entrada[3];
		} else if (mq == 6) {
			if (keypadRead(&keypad, &tecla)) {
				entrada[0] = tecla;
				entrada[1] = entrada[0];
				entrada[2] = entrada[1];
				entrada[3] = entrada[2];
				*total[1] = entrada[0] * 1000 + entrada[1] * 100+ entrada[2] * 10 + entrada[3];
			} else if (mq == 7) {


				if (keypadRead(&keypad, &tecla)) {
					entrada[0] = tecla;
					entrada[1] = entrada[0];
					entrada[2] = entrada[1];
					entrada[3] = entrada[2];
					*total[2] = entrada[0] * 1000 + entrada[1] * 100
							+ entrada[2] * 10 + entrada[3];
				}
			}

		}
	}
}

	void chequearGases(int total[3], int concentracion_maxima[CANT_GASES]) {
		int flag4, flag6, flag7;
		actualizarConcentraciones();
		if (total[0] < concentration_mq[0]
				|| total[0] > concentracion_maxima[0]) {
			lcdClear();
			//lcdGoToXY(0, 1);	//Posiciono donde voy a escribir
			lcdSendStringRaw("Alerta mq4");
			flag4 = 1;

		}
		if (total[1] < concentration_mq[1]
				|| total[1] > concentracion_maxima[1]) {
			lcdClear();
			lcdSendStringRaw("Alerta mq6");
			flag6 = 1;

		}
		if (total[2] < concentration_mq[2]
				|| total[2] > concentracion_maxima[2]) {
			lcdClear();
			lcdSendStringRaw("Alerta mq7");
			flag7 = 1;

		}

		if (flag4 || flag6 || flag7) {
			gpioWrite(GPIO1, ON);
		}
}

