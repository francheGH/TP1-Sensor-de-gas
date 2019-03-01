#ifndef KEYPAD_H_
#define KEYPAD_H_
typedef unsigned char uint8;
void KEYPAD_init(void);
uint8 KEYPAD_Charat(uint8 key);
uint8 KEYPAD_Scan(uint8 *key); // si fijaque tecla presiona y si se presiono una tecla
uint8 KEYPAD_Pulse(uint8 *key); // se fija si presiono una tecla
#endif

void init_KeyPad();
char Key_Pressed();
char KeyPad_Update(char *pkey);
