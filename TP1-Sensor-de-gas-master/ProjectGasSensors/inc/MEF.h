#ifndef MEF_H_
#define MEF_H_

typedef unsigned char uint8;
state inicializarEstado(void);
typedef enum {
	INICIAL,
	CAMBIAR_HORA,
	CAMBIAR_SEGUNDOS,
	CAMBIAR_MINUTOS,
	ALARMA,
	MODO_PROGRAMADOR
} state;
void init_MEF();
state update_MEF(state estadoact, uint8 tecla);
