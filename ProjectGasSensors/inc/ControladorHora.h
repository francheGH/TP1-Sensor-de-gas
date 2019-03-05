#ifndef CONTROLADORHORA_H_ 
#define CONTROLADORHORA_H_
typedef unsigned char uchar8;
void ActualizarHora(void);
void imprimirHora(void);
void cambiarHora(uchar8 decena, uchar8 unidad);
void cambiarMinuto(uchar8 decena, uchar8 unidad);
void cambiarSegundo(uchar8 decena, uchar8 unidad);
//void imprimirHoraParpadeando(state estadoact);
#endif 
