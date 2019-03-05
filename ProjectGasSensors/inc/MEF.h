#include <sapi.h>
typedef unsigned char uchar8;

typedef enum {
	INICIAL,
	CAMBIAR_HORA,
	CAMBIAR_SEGUNDOS,
	CAMBIAR_MINUTOS,
	ALARMA,
	MODO_PROGRAMADOR
} state;
void init_MEF();
void update_MEF();
void start();
bool_t eventHappens();
void cambiarEstado();
void fcambiar_H();
void fcambiar_M();
void fcambiar_S();
