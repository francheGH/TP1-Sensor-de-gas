#include "sapi.h"
#include "MEF.h"
#include "ControladorHora.h"
#include "ControladorGases.h"

static state estadonuevo;

#define HORA 1
#define MINUTO 2
#define SEGUNDO 3
#define DECENA 1
#define UNIDAD 2

volatile int total[3] = { 400, 400, 200 }; // pueden ir variando
int concentracion_maxima[3] = { 400, 400, 200 }; //fijar por defecto
uint8 aux;
uint8 valorAuxiliar;

keypad_t keypad;
bool_t valor;
uint16_t tecla = 0; //valor que obtengo del teclado

static state estado;
static state estadoAnt;
unsigned char string[16]; //para imprimir en el LED

typedef enum {
	DEC, UNI
} pos;

pos posicion;
volatile uint8 posc;

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

	lcdClear();
	imprimirHora();	//preguntar a elias por que de vuelta un imprimir hora
	ActualizarHora();
	//se opto por actualizar de a uno, porque shit happens broh.
	chequearGases(total[3], concentracion_maxima[3]);//actualiza los gases y chequea si no se pasaron de rango
	imprimirGas();

	//actualizarConcentraciones();
}

void init_MEF() {
	estado = INICIAL;
	estadoAnt = INICIAL;
	//inicio de variables de control
	initGases();
	imprimirHora();
}

state inicializarEstado(void) {
	return INICIAL;
}

state update_MEF(state estadoactual, uint8 tecla) {

	estadonuevo = INICIAL;

	switch (estado) {
	case INICIAL:
		//muestra la hora y abajo hace un barrido de los gases
		//y sus concentraciones cada 1 seg
		start();
		break;

		switch (tecla) {
		case 'A':
			estadonuevo = CAMBIAR_HORA;
			posicion = DEC;
			guardarTiempo();
			break;
		case 'B':
			estadonuevo = CAMBIAR_MINUTOS;
			posicion = DEC;
			guardarTiempo();
			break;
		case 'C':
			estadonuevo = CAMBIAR_SEGUNDOS;
			posicion = DEC;
			guardarTiempo();
			break;
		case "D":
			estadonuevo = MODO_PROGRAMADOR;
			break;
		default:
			estadonuevo = estadoactual;
			break;
		}
		break;
	case MODO_PROGRAMADOR:
		switch (tecla) {
		case 'A':
			fmodo_P(4, *total[3]);//que sensor es y el vector para programar su maximo
			break;
		case 'B':
			fmodo_P(6, *total[3]);
			break;
		case 'C':
			fmodo_P(7, *total[3]);
			break;
			estadonuevo = MODO_PROGRAMADOR;
			break;
		default:
			estadonuevo = estadoactual;
			break;
		}
		break;
	case ALARMA:
		fmodo_A();
		break;
	case CAMBIAR_HORA:
		switch (tecla) {
		case 'D': {
			estadonuevo = INICIAL;
		}
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (posicion == DEC) {
				if (tecla == '0' || tecla == '1'
						|| (tecla == '2' && unidadHora())) {
					valorAuxiliar = tecla - '0';

					aux = valorAuxiliar;

					posicion = UNI;
				}
			} else {

				if (aux == 2) {
					if (tecla == '0' || tecla == '1' || tecla == '2'
							|| tecla == '3') {
						valorAuxiliar = tecla - '0';
						cambiarValoresReloj(valorAuxiliar, HORA, UNIDAD);
					}
					posicion = DEC;

				} else {
					valorAuxiliar = tecla - '0';
					cambiarValoresReloj(valorAuxiliar, HORA, UNIDAD);
				}
				posicion = DEC;
			}
		}
		estadonuevo = estadoactual;
		break;
	default:
		estadonuevo = estadoactual;
		break;
	}
	break;
	case CAMBIAR_MINUTOS:
	switch (tecla) {
	case 'D':
		estadonuevo = INICIAL;
		recuperarTiempo();
		break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		if (posicion == DEC) {
			if (tecla == '6' || tecla == '7' || tecla == '8' || tecla == '9') {
			} else {
				valorAuxiliar = tecla - '0';
				cambiarValoresReloj(valorAuxiliar, MINUTO, DECENA);

				posicion = UNI;
			}
		} else {
			valorAuxiliar = tecla - '0';
			cambiarValoresReloj(valorAuxiliar, MINUTO, UNIDAD);
		}
		posicion = DEC;
	}
	estadonuevo = estadoactual;
	break;

	default:
	estadonuevo = estadoactual;
	break;

	case CAMBIAR_SEGUNDOS:
	switch (tecla) {
	case 'D':
		estadonuevo = INICIAL;
		recuperarTiempo();
		break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		if (posicion == DEC) {
			if (tecla == '6' || tecla == '7' || tecla == '8' || tecla == '9') {
			} else {
				valorAuxiliar = tecla - '0';
				cambiarValoresReloj(valorAuxiliar, SEGUNDO, DECENA);
				posicion = UNI;
			}
		} else {
			valorAuxiliar = tecla - '0';
			cambiarValoresReloj(valorAuxiliar, SEGUNDO, UNIDAD);
			posicion = DEC;
		}
		estadonuevo = estadoactual;
		break;

	default:
		estadonuevo = estadoactual;
		break;

		break;
	}
	return estadonuevo;

}
