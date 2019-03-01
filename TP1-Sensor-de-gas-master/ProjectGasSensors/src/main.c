#include "sapi.h"
#include "stdio.h"
#include "math.h"
#include "stdint.h"
#include "ControladorHora.h"
#include "mqX.h"
#include "MEF.h"
volatile char fseg;
/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook(void *ptr) {
	static uint16_t count;
	count++;
	if(count==20)
		fseg=1;

}

int main(void) {

	/* Variable de Retardo no bloqueante */
	delay_t delay;

	/* Inicializar la placa */
	boardConfig();

	/* Inicializar el conteo de Ticks con resolucion de 50ms (se ejecuta
	 periodicamente una interrupcion cada 50ms que incrementa un contador de
	 Ticks obteniendose una base de tiempos). */
	tickConfig(50);

	/* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
	 simplemente una funcion que se ejecutara peri�odicamente con cada
	 interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
	 a una interrupcion.
	 El segundo parametro es el parametro que recibe la funcion myTickHook
	 al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
	 */
	tickCallbackSet(myTickHook, (void*) LEDR);
	//delay(1000);

	keypad_t keypad;
	bool_t valor;
	// Filas --> Salidas
	uint8_t keypadRowPins1[4] = { GPIO3, 	// Row 0
			GPIO5,	// Row 1
			GPIO7,  // Row 2
			GPIO8   // Row 3
			};

	// Columnas --> Entradas con pull-up (MODO = GPIO_INPUT_PULLUP)
	uint8_t keypadColPins1[4] = { GPIO0,   // Column 0
			GPIO2,   // Column 1
			GPIO4,   // Column 2
			GPIO6    // Column 3
			};
	uint16_t tecla = 0;

	delayConfig(&delay, 1000);

	keypadConfig(&keypad, keypadRowPins1, 4, keypadColPins1, 4);
	// Inicializar LCD de 16x2 (caracteres x lineas) con cada caracter de 5x2 pixeles
	lcdInit(16, 2, 5, 8);

	lcdClear(); // Borrar la pantalla
	gpioConfig(GPIO1, GPIO_OUTPUT);
	lcdGoToXY(1, 1); // Poner cursor en 1, 1
	//concentraciones[1]= adcRead ( CH2 ); //MQ6
	//concentraciones[2]= adcRead ( CH1 ); //MQ7

	int concentration_mq4;
	char str[16];
	adcConfig(ADC_ENABLE);


	state estado;
	ActualizarHora();
	imprimirHora();
	init_MEF();
	for (;;) {

		if(fseg=1)
		{
			fseg=0;
		if (keypadRead(&keypad, &tecla)) {

		// De acuerdo al caracter y el estado actual obtengo el nuevo estado
		                    	estado = update_MEF(estado,valor,tecla);
		              	}
		}










		/*prueba teclado*/

		if (keypadRead(&keypad, &tecla)) {
			if (tecla == 1) {
				lcdSendStringRaw("2");

			}
		}
		valor = !gpioRead(TEC4);
		gpioWrite(GPIO1, valor);

		if (delayRead(&delay)) {/*pruebalcd*/
			lcdClear();
			lcdGoToXY(1, 1);
			ActualizarHora();
			imprimirHora();
			lcdGoToXY(6, 2);

			concentration_mq4 = sensorlecturamq(RL_MQ4, X0_MQ4, X1_MQ4, Y0_MQ4,Y1_MQ4, R0_MQ4, CH1);
			itoa(concentration_mq4, str, 10);
			lcdSendStringRaw(str);/*prueba sensor*/

			tickCallbackSet(myTickHook, (void*) LEDG);
			//delay(1000);

			// printf("Hocmd");
		}

	}
}
//Fin de la configuracion del keypad
