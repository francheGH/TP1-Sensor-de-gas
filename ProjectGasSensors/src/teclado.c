#include "derivative.h"
#include "sapi.h"

const char Nulo = ' ';
const char teclado[4][4]={{"1", "2", "3", "A"}, 
	 					  {"4", "5", "6", "B"},
						  {"7", "8", "9", "C"}, 
						  {"*", "0", "#", "D"}};

unsigned char pkey;
char LastKey=Nulo;
char LastValidKey=Nulo;	
char KeyPad_Update(char *pkey);
void Init_KeyPad(void);
char Key_Pressed(void);


void init_KeyPad(){
	 /*PTBDD=0x0F; //Se declara una entrada y una salida
	 PTBPE=0xF0; // El pull up esta en 0 en las entradas*/
}

char Key_Pressed(){
	unsigned char aux;
	/*PTBD = 0xFE; // 0x(columnas)(filas)
	aux=~PTBD;*/
	if(aux & 0x10) return '1';
	if(aux & 0x20) return '2';
	if(aux & 0x40) return '3';
	if(aux & 0x80) return 'A';
	/*PTBD= 0xFD;
	aux=~PTBD;*/
	if(aux & 0x10) return '4';
	if(aux & 0x20) return '5';
	if(aux & 0x40) return '6';
	if(aux & 0x80) return 'B';
	/*PTBD=0xFB;
	aux=~PTBD;*/
	if(aux & 0x10) return '7';
	if(aux & 0x20) return '8';
	if(aux & 0x40) return '9';
	if(aux & 0x80) return 'C';
	/*PTBD=0xF7;
	aux=~PTBD;*/
	if(aux & 0x10) return '*';
	if(aux & 0x20) return '0';
	if(aux & 0x40) return '#';
	if(aux & 0x80) return 'D';
	return Nulo;	
}

char KeyPad_Update(char *pkey){
    char key = Key_Pressed();
	if(key == Nulo){
		LastKey=Nulo;
		LastValidKey=Nulo;
		return 0;
	}
    if (key == LastKey){
        if (key != LastValidKey){
            *pkey=key;
            LastValidKey=key;
            return 1;
        }
    }
    LastKey = key;
    return 0;   
}
