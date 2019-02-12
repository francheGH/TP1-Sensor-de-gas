#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif
#include "board.h";

#define _ADC_CHANNEL ADC_CH1
#define _LPC_ADC_ID LPC_ADC0

static ADC_Clock_Setup_T ADCSetup;

void init_ADC(){
	Chip_ADC_Init(_LPC_ADC_ID, &ADCSetup);
	Chip_ADC_Channel_Enable_CMD(LPC_ADC0, ADC_CH1, ENABLE);

}
void leerDato(uint16_t* data){
	Chip_ADC_Read_Value (LPC_ADC_T* pADC, ADC_CH1, data);
}
