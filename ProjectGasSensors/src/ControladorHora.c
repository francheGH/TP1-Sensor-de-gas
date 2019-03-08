#include "ControladorHora.h"
#include "MEF.h"
#include "sapi.h"
#define DECENA_HORA 4
#define UNIDAD_HORA 5
#define DECENA_MINUTO 7
#define UNIDAD_MINUTO 8
#define DECENA_SEGUNDO 10
#define UNIDAD_SEGUNDO 11

#define HORA 1
#define MINUTO 2
#define SEGUNDO 3
#define DECENA 1
#define UNIDAD 2

static char reloj[16] = {32, 32, 32, 32, 1, 2, ':', 0, 0, ':', 0, 0, 32, 32, 32, 32}; // Arreglo de codigos ascii que corresponden la hora (para imprimir se le debe sumar el caracter '0' a cada posicion)

// Variables que corresponden a las decenas y unidades de las horas, minutos y segundos

char dhora = 1;
char uhora = 2;

char dmin = 0;
char umin = 0;

char dseg = 0;
char useg = 0;

char dhoraAnterior;
char uhoraAnterior;

char dminAnterior;
char uminAnterior;

char dsegAnterior;
char usegAnterior;

volatile int i; // Indice para for

void cambiarTodosValoresReloj(void) {
	reloj[DECENA_HORA] = dhora;
	reloj[UNIDAD_HORA] = uhora;
	reloj[DECENA_MINUTO] = dmin;
	reloj[UNIDAD_MINUTO] = umin;
	reloj[DECENA_SEGUNDO] = dseg;
	reloj[UNIDAD_SEGUNDO] = useg;
}

void cambiarValoresReloj(char aux, char HS_MIN_SEG, char posicion) {
	switch (HS_MIN_SEG) {
	case HORA:
		switch (posicion) {
		case DECENA:
			dhora = aux;
			reloj[DECENA_HORA] = dhora;
			break;
		case UNIDAD:
			uhora = aux;
			reloj[UNIDAD_HORA] = uhora;
			break;
		}
		break;
	case MINUTO:
		switch (posicion) {
		case DECENA:
			dmin = aux;
			reloj[DECENA_MINUTO] = dmin;
			break;
		case UNIDAD:
			umin = aux;
			reloj[UNIDAD_MINUTO] = umin;
			break;
		}
		break;
	case SEGUNDO:
		switch (posicion) {
		case DECENA:
			dseg = aux;
			reloj[DECENA_SEGUNDO] = dseg;
			break;
		case UNIDAD:
			useg = aux;
			reloj[UNIDAD_SEGUNDO] = useg;
			break;
		}
		break;
	}
}

void ActualizarHora() { // Se encarga de manejar los valores de la hora 
	useg++; // Como se ejecutara cada 1 segundo, aumenta la unidad de los segundos
	if (useg == 10) { // Si la unidad llega a 10, se aumentan las decenas y se reinician las unidades
		dseg++;
		useg = 0;
	}
	if (dseg == 6) { // Si la decena de los segundos llega a 6, se aumenta la unidad de los minutos y se reinician los segundos
		dseg = 0;
		umin++;
	}
	if (umin == 10) { // Si la unidad llega a 10, se aumentan las decenas y se reinician las unidades
		dmin++;
		umin = 0;
	}
	if (dmin == 6) { // Si la decena de los minutos llega a 6, se aumenta la unidad de las horas y se reinician los minutos
		dmin = 0;
		uhora++;
	}
	if (uhora == 10) { // Si la unidad llega a 10, se aumentan las decenas y se reinician las unidades
		dhora++;
		uhora = 0;
	}
	if ((dhora == 2) && (uhora == 4)) { // Si la hora es 24, se reinician las horas (previo se han reiniciado los minutos y los segundos)
		uhora = 0;
		dhora = 0;
	}
	cambiarTodosValoresReloj();
	// Se asignan los nuevos valores al arreglo correspondiente a la hora

}

void imprimirHora() { // Escribe la hora

	for (i = 4; i < 12; ++i) {
		if (i != 6 && i != 9)
			reloj[i] = reloj[i] + '0'; //time[]=valor ascii + '0' valor ascii del 	0=48 ejemplo= -16 +48= 32 significado del espacio
	}
	lcdSendStringRaw(reloj); //cambie recien "*"
}
void guardarTiempo(void) {
	dhoraAnterior = dhora;
	uhoraAnterior = uhora;
	dminAnterior = dmin;
	uminAnterior = umin;
	dsegAnterior = dseg;
	usegAnterior = useg;
}

void recuperarTiempo(void) {
	dhora = dhoraAnterior;
	uhora = uhoraAnterior;
	dmin = dminAnterior;
	umin = uminAnterior;
	dseg = dsegAnterior;
	useg = usegAnterior;
}
void imprimihoraP(int pos)
{
	lcdGoToXY(pos,1);
	lcdSendStringRaw("  ");
}
/*void imprimirHoraParpadeando(int pos) {
	// Parpadea el parametro de hora correspondiente si se encuentra en algun
	//modo de 	modificació n
// Imprime blancos en la posicion de los digitos correspondientes de acuerdo al	estado
	switch (pos) {

	case 1:
		for (i = 0; i < 16; ++i) {

			lcdGoToXY(i, 1);
			switch (i) {
			case 4:
				reloj[i] = '-16';
				//lcdSendStringRaw(reloj);
				break;
			case 5:
				reloj[i] = '-16';
				//lcdSendStringRaw(reloj);
				break;
			//default:
				//lcdGoToXY(1, 1);
				//lcdSendStringRaw(reloj);
				//	break;//	LCD_write_char(reloj[i] + '0');
			}								//Corregir no imprime por posición.

		}
		break;
	case 2:
		for (i = 0; i < 16; ++i) {
			lcdGoToXY(i, 1);	//	LCD_pos_xy(i, 0);
			switch (i) {
			case 7:

				reloj[i] = '-16';
				//lcdSendStringRaw(reloj);
				break;
			case 8:

				reloj[i] = '-16';
				//lcdSendStringRaw(reloj);
				break;
		//	default:
				//lcdSendStringRaw(reloj);	//LCD_write_char(reloj[i] + '0');

				//	break;
			}
		}
		break;
	case 3:
		for (i = 0; i < 16; ++i) {
			lcdGoToXY(i, 1);	//	LCD_pos_xy(i, 0);

			switch (i) {
			case 10:

				reloj[i] = '-16';
				//	lcdSendStringRaw(reloj);

				break;
			case 11:

				reloj[i] = '-16';
				//	lcdSendStringRaw(reloj);

				break;
		//	default:

				//lcdSendStringRaw(reloj);	//LCD_write_char(reloj[i] + '0');

				//break;
			}
		}
		break;
	}
}*/

void cambiarHora(char decena, char unidad) {
	if (((decena == 2) && (unidad < 4)) || ((decena < 2) && (unidad < 10))) {
		dhora = decena;
		uhora = unidad;
		reloj[DECENA_HORA] = dhora;
		reloj[UNIDAD_HORA] = uhora;
	}
}
void cambiarMinuto(char decena, char unidad) {
	if ((decena < 6) && (unidad < 10)) {
		dmin = decena;
		umin = unidad;
		reloj[DECENA_MINUTO] = dmin;
		reloj[UNIDAD_MINUTO] = umin;
	}
}
void cambiarSegundo(char decena, char unidad) {
	if ((decena < 6) && (unidad < 10)) {
		dseg = decena;
		useg = unidad;
		reloj[DECENA_SEGUNDO] = dseg;
		reloj[UNIDAD_SEGUNDO] = useg;
	}
}

