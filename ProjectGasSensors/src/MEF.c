#include "sapi.h"
#include "MEF.h"
#include "ControladorHora.h"
#include "ControladorGases.h"

volatile int total[3] = { 400, 400, 200 }; // pueden ir variando

//static char aux;
//static char valorAuxiliar;
const static uint16_t teclado[16] = { 1, 2, 3, 0x0a, 4, 5, 6, 0x0b, 7, 8, 9,
		0x0c, 0x0e, 0, 0x0f, 0x0d };
const static char tecladochar[16] = {

'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '#', '0', '*', 'D'

};
keypad_t keypad;
//extern bool_t valor;
uint16_t tecla = 0; //valor que obtengo del teclado
//esto era un char
static char teclaAux;
static state estado;
static state estadoAnt;
static uint16_t ocupado = 0;
static int flagCambio;
uint16_t contador;

static int Fread = 1;
int flagAlarma = 0;

void init_MEF(keypad_t keypad2) {
	estado = INICIAL;
	estadoAnt = INICIAL;
	keypad = keypad2;
	//initKeypad();
	//inicio de variables de control
//	imprimirHora();
}

void update_MEF() {

	switch (estado) {
	case INICIAL:
		//muestra la hora y abajo hace un barrido de los gases y sus concentraciones cada 1 seg
		start();
		break;
	case CAMBIAR_HORA:
		fcambiar_H();
		break;
	case CAMBIAR_MINUTOS:
		fcambiar_M();
		break;
	case CAMBIAR_SEGUNDOS:
		fcambiar_S();
		break;
	case ALARMA:
		gpioWrite(GPIO1, ON);
		readSensor();
		estado=INICIAL;
		break;
	case MODO_PROGRAMADOR:
		fmodo_P();
		break;
	}
	if (eventHappens()) {
		cambiarEstado();
	}
}

void start() {
	//problema: donde tengo que actualizar el arreglo de gases?
	//el arreglo de gases se debe actualizar continuamente
	//posible solucion: actualizarlo luego de ActualizarHora, pero se va a mostrar un gas actualizado cada
	//1 segundo cuando se actualizan los 3 (no tiene sentido actualizar los otros 2?)
	//la idea seria modelar de alguna manera que si se sobrepasa uno de los gases
	//que se pase al estado de alarmas
	//2da solucion: actualizar en el arreglo solo el gas que se va a mostrar
	//critica: si ocurre una situacion critica, el arreglo de gases y por lo tanto la alarma se demoriaria
	//3 segundos
	//ambas soluciones desde el punto de vista del tiempo pueden resultar nimiedades.
	//pero desde el punto de vista de presicion y efectividad presentan errores graves
	readSensor();

	imprimirGas();
	gpioWrite(GPIO1, OFF);
	//preguntar a elias por que de vuelta un imprimir hora
	//ActualizarHora();
	//se opto por actualizar de a uno, porque shit happens broh.

	//actualizarConcentraciones();
}

bool_t eventHappens() {
//Funcion que se encarga de chequear 2 eventos posibles
//1: Que se presione una tecla del KeyPad
//2: Que las concentraciones sobrepasen el limite y activen la alarma
//cualquiera de los 2 eventos anteriores dispararian un cambio de estado
	bool_t ok = FALSE;
	if (ocupado == 0) {
		if (keypadRead(&keypad, &tecla)) {
			teclaAux = tecladochar[tecla];
			ok = TRUE;
		}
	}
	if (chequearGases()) {
		ok = TRUE;
		flagAlarma = 1;
	}
	return ok;
}

void cambiarEstado() {
	if (teclaAux != ' ') {
		switch (teclaAux) {
		case 'A':
			estado = CAMBIAR_HORA;
			flagCambio = 0;
			break;
		case 'B':
			estado = CAMBIAR_MINUTOS;
			flagCambio = 0;
			break;
		case 'C':
			estado = CAMBIAR_SEGUNDOS;
			flagCambio = 0;
			break;
		case 'D':
			estado = MODO_PROGRAMADOR;
			break;
		default:
			estado=INICIAL;
			break;
		}
	}
	if (flagAlarma) {
		estado = ALARMA;
		flagAlarma=0;
	}
}
void fcambiar_H() {
	static char teclaAuxiliar;
	static char decena = ' ';
	static char unidad = ' ';
	static uint8_t cont = 0;
	if (cont++ == 2) {
		imprimihoraP(5);
		cont = 0;
	}
	ocupado = 1;
	if (keypadRead(&keypad, &tecla)) {
		teclaAuxiliar = teclado[tecla];
		if (flagCambio == 0) {
			decena = teclaAuxiliar;
			flagCambio = 1;
		} else {
			unidad = teclaAuxiliar;
			cambiarHora(decena, unidad);
			flagCambio = 0;
			ocupado = 0;
			estado = INICIAL;
		}
	}

}
void fcambiar_M() {
	uint16_t teclaAuxiliar;
	static char decena;
	static char unidad;
	ocupado = 1;
	static uint8_t cont = 0;
	if (cont++ == 2) {
		imprimihoraP(8);
		cont = 0;
	}
	if (keypadRead(&keypad, &tecla)) {
		teclaAuxiliar = teclado[tecla];
		if (flagCambio == 0) {
			decena = teclaAuxiliar;
			flagCambio = 1;
		} else {
			unidad = teclaAuxiliar;
			cambiarMinuto(decena, unidad);
			flagCambio = 0;
			ocupado = 0;
			estado = INICIAL;
		}
	}

}
void fcambiar_S() {
	uint16_t teclaAuxiliar;
	static char decena;
	static char unidad;
	ocupado = 1;
	static uint8_t cont = 0;
	if (cont++ == 2) {
		imprimihoraP(11);
		cont = 0;
	}
	//imprimirHoraParpadeando(3);
	if (keypadRead(&keypad, &tecla)) {
		teclaAuxiliar = teclado[tecla];
		if (flagCambio == 0) {
			decena = teclaAuxiliar;
			flagCambio = 1;
		} else {
			unidad = teclaAuxiliar;
			cambiarSegundo(decena, unidad);
			flagCambio = 0;
			ocupado = 0;
			estado = INICIAL;
		}
	}
}
void fmodo_P() {
	static uint16_t teclaAuxiliar;
	int mq = 0;
	int fin = 0; //flag de finalizacion

	lcdClear();
	lcdGoToXY(1, 1);	//Posiciono donde voy a escribir

	//	itoa(mq, str, 10);
	//

	if(Fread==1){
		ocupado=1;
		lcdSendStringRaw("MQ4 MQ6 MQ7");
		lcdGoToXY(1, 2);	//Posiciono donde voy a escribir
		lcdSendStringRaw("OPCIONES: A B C");
	}
	//delay(50);
	if (keypadRead(&keypad, &tecla) && Fread) {
		teclaAuxiliar = tecladochar[tecla];
		Fread = 0; //flag de lectura
	}
//	delay(50);
	if (Fread == 0) {
		switch (teclaAuxiliar) {
		case 'A':
			mq = 4;
			actualizarConcentraciones(mq, keypad, &fin);
			break;
		case 'B':
			mq = 6;
			actualizarConcentraciones(mq, keypad, &fin);
			break;
		case 'C':
			mq = 7;
			actualizarConcentraciones(mq, keypad, &fin);
			break;
		default:
			Fread = 1;
			ocupado=0;
			estado=INICIAL;
			break;
		}
		if (fin) {
			Fread = 1;
			ocupado=0;
			estado = INICIAL;
		}

	}
}

/*void Parpadeo(char decena, char unidad, char pos, char *cont_parpadeo){
 *cont_parpadeo= *cont_parpadeo + 1;
 if(*cont_parpadeo == 5){
 switch (pos){
 case 1:
 ImprimirReloj(NULO, Min, Seg);
 break;
 case 2:
 ImprimirReloj(Hora, NULO, Seg);
 break;
 case 3:
 ImprimirReloj(Hora, Min, NULO);
 break;
 }
 }
 if(*cont_parpadeo == 10){
 switch (pos){
 case 1:
 ImprimirReloj(A, Min, Seg);
 break;
 case 2:
 ImprimirReloj(Hora, A, Seg);
 break;
 case 3:
 ImprimirReloj(Hora, Min, A);
 break;
 }
 }
 }*/
