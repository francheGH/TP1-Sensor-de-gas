#include "sapi.h"
#include "MEF.h"
#include "ControladorHora.h"
#include "ContralodorGases.h"

static state estadonuevo;



volatile int total[3] = { 400, 400, 200 }; // pueden ir variando
static int concentracion_maxima[3] = { 400, 400, 200 }; //fijar por defecto
static uchar8 aux;
static uchar8 valorAuxiliar;
const static uint16_t teclado[16]={
        1,    2,    3, 0x0a,
        4,    5,    6, 0x0b,
        7,    8,    9, 0x0c,
     0x0e,    0, 0x0f, 0x0d
   };
static keypad_t keypad;
//extern bool_t valor;
static uint16_t tecla = 0; //valor que obtengo del teclado
static char teclaAux;
static state estado;
static state estadoAnt;
static unsigned char string[16]; //para imprimir en el LED
static int flagCambio;
uint16_t contador;
static char cont_parpadeo;

void init_MEF(keypad_t keypad2) {
	estado = INICIAL;
	estadoAnt = INICIAL;
	keypad=keypad2;
	//initKeypad();
	//inicio de variables de control
	imprimirHora();
	initGases();
}


void update_MEF() {

	switch (estado){
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
				falarma();
				break;
			case MODO_PROGRAMADOR:
				fmodo_P();
				break;
		}
		if (eventHappens()){
			cambiarEstado();
	}
}

void start(){
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
	actualizarConcentraciones();

	//imprimirHora();
	//ActualizarHora();
	//preguntar a elias por que de vuelta un imprimir hora
	//ActualizarHora();
	//se opto por actualizar de a uno, porque shit happens broh.
	//imprimirGas();
	//actualizarConcentraciones();
}

bool_t eventHappens(){
//Funcion que se encarga de chequear 2 eventos posibles
//1: Que se presione una tecla del KeyPad
//2: Que las concentraciones sobrepasen el limite y activen la alarma
//cualquiera de los 2 eventos anteriores dispararian un cambio de estado
bool_t ok= FALSE;

	if(keypadRead(&keypad, &tecla)){
		teclaAux=teclado[tecla];
		ok=TRUE;
	}
	if (chequear_gases()){
		ok=TRUE;
	}
	return ok;
}

void cambiarEstado(){
	if (teclaAux!=' '){
		switch (teclaAux){
		case 0x0a:
			estado=CAMBIAR_HORA;flagCambio=0;
			break;
		case 0x0b:
			estado=CAMBIAR_MINUTOS;flagCambio=0;
			break;
		case 0x0c:
			estado=CAMBIAR_SEGUNDOS;flagCambio=0;
			break;
		case 0x0d:
			estado=MODO_PROGRAMADOR;
			break;
		}
	}
}
void fcambiar_H(){
	char teclaAuxiliar;
	static uchar8 decena=' ';
	static uchar8 unidad=' ';
	//imprimirHoraParpadeando(1);
	if (keypadRead(&keypad, &tecla)){
				teclaAuxiliar=teclado[tecla];
				if (flagCambio==0){
							decena=teclaAuxiliar;
							flagCambio=1;
					}else{
						unidad=teclaAuxiliar;
						cambiarHora(decena, unidad);
						flagCambio=0;
						estado=INICIAL;
					}
	}

}
void fcambiar_M(){
	char teclaAuxiliar;
	static uchar8 decena;
	static uchar8 unidad;
	//Parpadeo (decena,unidad,2,&cont_parpadeo);
	//imprimirHoraParpadeando(2);
	if (keypadRead(&keypad, &tecla)){
				teclaAuxiliar=teclado[tecla];
				if (flagCambio==0){
							decena=teclaAuxiliar;
							flagCambio=1;
					}else{
						unidad=teclaAuxiliar;
						cambiarMinuto(decena, unidad);
						flagCambio=0;
						estado=INICIAL;
					}
	}

}
void fcambiar_S(){
	char teclaAuxiliar;
	static uchar8 decena;
	static uchar8 unidad;
	//imprimirHoraParpadeando(3);
	if (keypadRead(&keypad, &tecla)){
		teclaAuxiliar=teclado[tecla];
						if (flagCambio==0){
									decena=teclaAuxiliar;
									flagCambio=1;
							}else{
								unidad=teclaAuxiliar;
								cambiarSegundo(decena, unidad);
								flagCambio=0;
								estado=INICIAL;
							}
	}
}
void fmodo_P(){
	char teclaAuxiliar;
	int mq=0;
	if (keypadRead(&keypad, &tecla)&&Fread){
		teclaAuxiliar=teclado[tecla];
		Fread=0;
	}
	if ( Fread==0){
		switch (teclaAuxiliar){
			case 1:
			mq=4; actualizarConcentraciones (keypad, concentracion_maxima[0], fin);
			break;
			case 2:
			mq=6; ingresarValores(keypad, concentracion_maxima[1],fin);
			break;
			case 3:
			mq=7;ingresarValores(keypad, concentracion_maxima[2],fin);
			break;
		}
		if (fin)
			Fread=1;s
			estado=INICIAL;
	}
}


/*void Parpadeo(uchar8 decena, uchar8 unidad, char pos, char *cont_parpadeo){
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
