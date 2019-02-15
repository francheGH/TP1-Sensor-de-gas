#include "ControladorHora.h"
#include "ControladorGases.h"
#include "lcd.h"




typedef enum {INICIAL, CAMBIAR_HORA, CAMBIAR_MINUTO, CAMBIAR_SEGUNDO, ALARMA, MODO_PROGRAMADOR} enumEstados;
static enumEstados estado;
static enumEstados estadoAnt;
static char tecla; //valor que obtengo del teclado
unsigned char string[16]; //para imprimir en el LED

void init_MEF(){
	estado=INICIAL;
	estadoAnt=INICIAL;
	//inicio de variables de control
	initGases();
	imprimirHora();
}
void update_MEF(){
	switch (estado){
		case INICIAL:
			//muestra la hora y abajo hace un barrido de los gases y sus concentraciones cada 1 seg
			start();
			break;
		case CAMBIAR_HORA:
			fcambiar_H();
			break;
		case CAMBIAR_MINUTO:
			fcambiar_M();
			break;
		case CAMBIAR_SEGUNDO:
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
		cambiarEstado(estado);
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
	imprimirHora();//preguntar a elias por que de vuelta un imprimir hora
	ActualizarHora();
	//se opto por actualizar de a uno, porque shit happens broh.
	imprimirGas();
	actualizarConcentraciones();
}

boolean eventHappens(){
//Funcion que se encarga de chequear 2 eventos posibles
//1: Que se presione una tecla del KeyPad
//2: Que las concentraciones sobrepasen el limite y activen la alarma
//cualquiera de los 2 eventos anteriores dispararian un cambio de estado
boolean ok= FALSE;

	if(KeyPad_Update(&tecla){
		ok=true;
	}
	if (sobrepasar_limite(concentraciones)){
		ok=true;
	}
	if(ok){
		return 1;
	}else{
		return 0;
	}
}

void cambiarEstado(estado){
	if (tecla!=" "){
		switch (tecla){
		case 'A':
			estado=CAMBIAR_HORA;
			break;
		case 'B':
			estado=CAMBIAR_MINUTO;
			break;
		case 'C':
			estado=CAMBIAR_SEGUNDO;
			break;
		case 'D':
			estado=MODO_PROGRAMADOR;
			break;
		}
	}
}
