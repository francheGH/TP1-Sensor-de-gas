#include "ControladorHora.h"
#include "lcd.h"
#include "adc.h"

#define CANT_GASES 3

typedef enum {INICIAL, CAMBIAR_HORA, CAMBIAR_MINUTO, CAMBIAR_SEGUNDO, ALARMA, MODO_PROGRAMADOR} enumEstados;
static enumEstados estado;
static enumEstados estadoAnt;
static char tecla; //valor que obtengo del teclado
//arreglo donde van a estar las concentraciones de los gases tomadas por los sensores
unsigned int concentraciones[CANT_GASES]={1,2,3};
//arreglo con las concentraciones maximas teoricas impuestas por los programadores.
unsigned int concentracion_maxima[CANT_GASES]={4,5,70};
unsigned char gases[CANT_GASES]={"Metano", "Propano", "Monoxido"}; //arreglo de nombres de los gases
static int concentracion_actual;
static int gas_actual;
unsigned char string[16]; //para imprimir en el LED

void init_MEF(){
	estado=INICIAL;
	estadoAnt=INICIAL;
	//inicio de variables de control
	concentracion_actual=0;
	gas_actual=0;
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
	ActualizarConcentraciones(concentracion_actual);
	imprimirGas(gas_actual, concentracion_actual);
	gas_actual=(gas_actual+1)%3; //supuestamente esto seria el recorrido ciclico del vector, verificar
	concentracion_actual=(concentracion_actual+1)%3;
}

boolean eventHappens(){
//Funcion que se encarga de chequear 2 eventos posibles
//1: Que se presione una tecla del KeyPad
//2: Que las concentraciones sobrepasen el limite y activen la alarma
//cualquiera de los 2 eventos anteriores dispararian un cambio de estado
boolean ok=false;

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
			break:
		}
	}
}
void imprimirGas(int gas_actual, int concentracion_actual){
	//imprime en el LCD

}

void ActualizarConcentraciones(int concentracion_actual){
int concentracion_leida;
	switch (concentracion_actual){
		case 0:
			leerMQ4(concentracion_leida); //retorna un valor en ppm de acuerdo a la letura del sensor
			concentraciones[0]=concentracion_leida;
			break;
		case 1:
			leerMQ6(concentracion_leida); //retorna un valor en ppm de acuerdo a la letura del sensor
			concentraciones[1]=concentracion_leida;
			break;
		case 2:
			leerMQ7(concentracion_leida); //retorna un valor en ppm de acuerdo a la letura del sensor
			concentraciones[2]=concentracion_leida;
			break;
	}
}
