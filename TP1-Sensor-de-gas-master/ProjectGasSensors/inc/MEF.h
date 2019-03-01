

#ifndef MEF_H_
#define MEF_H_

typedef unsigned char uint8;
typedef enum { INICIAL, CAMBIAR_HORA, CAMBIAR_SEGUNDOS, CAMBIAR_MINUTOS,ALARMA,MODO_PROGRAMADOR} state;
void init_MEF()

state update_MEF(state estadoact, uint8 tecla);
typedef enum {
	INICIAL,
	CAMBIAR_HORA,
	CAMBIAR_MINUTO,
	CAMBIAR_SEGUNDO,
	ALARMA,
	MODO_PROGRAMADOR
} enumEstados;
