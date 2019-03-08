#include <sapi.h>

#ifndef CONTROLADORHORA_H_ 
#define CONTROLADORHORA_H_

void ActualizarHora(void);
void imprimirHora(void);
void cambiarHora(char decena, char unidad);
void cambiarMinuto(char decena, char unidad);
void cambiarSegundo(char decena, char unidad);
void imprimihoraP(int pos);
//void imprimirHoraParpadeando(state estadoact);
#endif 
