#include "sapi.h"
#include "stdio.h"
#include "math.h"
#include "stdint.h"
//#include "MEF.h"
#include "ControladorHora.h"
#include "ControladorGases.h"
//#include "MEF.h"

//bool_t valor;
volatile uint16_t fseg, f200;
static int count = 0;

int main(void) {

	boardConfig();/* Inicializar la placa */

	/* Variable de Retardo no bloqueante */
	delay_t delay;
	delayConfig(&delay, 200);

	/* Inicializar el conteo de Ticks con resolucion de 50ms (se ejecuta
	 periodicamente una interrupcion cada 50ms que incrementa un contador de
	 Ticks obteniendose una base de tiempos). */
	//tickConfig(50);
// Inicializar LCD de 16x2 (caracteres x lineas) con cada caracter de 5x2 pixeles
	lcdInit(16, 2, 5, 8);
	lcdClear(); // Borrar la pantalla
	lcdGoToXY(1, 1); // Poner cursor en 1, 1
	adcConfig(ADC_ENABLE);
	gpioConfig(GPIO1, GPIO_OUTPUT);

	keypad_t keypad2;
// Filas --> Salidas
	uint8_t keypadRowPins2[4] = { GPIO3, 	// Row 0
			GPIO5,	// Row 1
			GPIO7,  // Row 2
			GPIO8   // Row 3
			};

// Columnas --> Entradas con pull-up (MODO = GPIO_INPUT_PULLUP)
	uint8_t keypadColPins2[4] = { GPIO0,   // Column 0
			GPIO2,   // Column 1
			GPIO4,   // Column 2
			GPIO6    // Column 3
			};

	keypadConfig(&keypad2, keypadRowPins2, 4, keypadColPins2, 4);

	/* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
	 simplemente una funcion que se ejecutara peri�odicamente con cada
	 interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
	 a una interrupcion.
	 El segundo parametro es el parametro que recibe la funcion myTickHook
	 al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
	 */
	//lcdSendStringRaw("hola");
	init_MEF(keypad2);
	//tickCallbackSet(myTickHook, (void*)fseg);
	//delay(1000);
	for (;;) {
		//uint16_t tecla;
		if (delayRead(&delay)) {
			count++;
			f200 = 1;

		}
		if (count == 5) {
			fseg = 1;
			count = 0;
		}

//
		if (fseg == 1) {
			fseg = 0;
			lcdClear();
			imprimirHora();
			ActualizarHora();
		}
		if (f200 == 1) {
		    update_MEF();
			f200 = 0;
		}
		/*if (delayRead(&delay)) {
		 contador++;
		 if (contador == 10) {
		 lcdClear();
		 imprimirHora();
		 ActualizarHora();
		 contador = 0;

		 }
		 update_MEF();

		 }*/

	}

}

//Fin de la configuracion del keypad

/*prueba sensor
 *
 * //			 int concentration_mq7;
 //			 char str[16];
 //			 adcConfig( ADC_ENABLE);
 //			 concentration_mq7 = sensorlecturamq(20, X0_MQ7, X1_MQ7, Y0_MQ7,
 //			 Y1_MQ7, R0_MQ7, CH3);
 //			 itoa(concentration_mq7, str, 10);
 //			 lcdSendStringRaw( str);
 *
 * */

/*prueba teclado
 if (keypadRead(&keypad, &tecla)) {
 if (tecla == 1) {
 lcdSendStringRaw("2");
 }
 }*/

/*prueba buzzer
 valor = !gpioRead(TEC4);
 gpioWrite(GPIO1, valor);*/

/*pruebalcd
 if (delayRead(&delay)) {
 lcdClear();
 lcdGoToXY(1, 1);
 ActualizarHora();
 imprimirHora();
 lcdGoToXY(6, 2);
 // printf("Hocmd");
 }*/
