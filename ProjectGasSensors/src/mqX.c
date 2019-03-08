#include "math.h"
#include "sapi.h"
#include "stdio.h"
#include  "stdlib.h"
#include "string.h"

//const uint16_t R0 = 4.4;          // Resistencia R0 del sensor en Kilo ohms
// Datos para lectura multiple
const uint16_t READ_SAMPLE_INTERVAL = 100;    // Tiempo entre muestras
const uint16_t READ_SAMPLE_TIMES = 5;       // Numero muestras

// Ajustar estos valores para vuestro sensor según el Datasheet
// (opcionalmente, según la calibración que hayáis realizado)

float readMQ (uint16_t RL_VALUE, int CH);
float getMQResistance (uint16_t raw_adc, uint16_t RL_VALUE);
float getConcentrationmq (float rs_ro_ratio,float coord, float scope);
int sensorlecturamq (uint16_t RL, float X0, float X1, float Y0,float Y1,float R0, int CH);


int sensorlecturamq (uint16_t RL, float X0, float X1, float Y0,float Y1,float R0, int CH) {

	// Puntos de la curva de concentración {X, Y}
	const float punto0[2] = { log10(X0), log10(Y0) };
	const float punto1[2] = { log10(X1), log10(Y1) };
	// Calcular pendiente y coorde abscisas
	float scope = (punto1[1] - punto0[1]) / (punto1[0] - punto0[0]);
	float coord = punto0[1] - punto0[0] * scope;
	float rs_med = readMQ(RL,CH);      // Obtener la Rs promedio
	float concentration;
	concentration = getConcentrationmq(rs_med / R0, coord, scope); // Obtener la concentración

	return (int) concentration;

}
// Mostrar el valor de la ctncentración por serial


// Obtener la resistencia promedio en N muestras
float readMQ(uint16_t RL_VALUE, int CH)
{  uint16_t mq;

   float rs = 0;
   for (int i = 0;i<READ_SAMPLE_TIMES;i++) {
	  mq=adcRead ( CH );
      rs = rs+ getMQResistance(mq,RL_VALUE);
      //delay(100);
   }
   return rs / 100;
}

// Obtener resistencia a partir de la lectura analogica
float getMQResistance(uint16_t raw_adc, uint16_t RL_VALUE )
{
   return (((float)RL_VALUE / 1000.0*(1023 - raw_adc) / raw_adc));
}

// Obtener concentracion 10^(coord + scope * log (rs/r0)
float getConcentrationmq(float rs_ro_ratio,float coord, float scope)
{
   return (float) pow(10, coord + scope * log10(rs_ro_ratio));
}


// convert float to string one decimal digit at a time
// assumes float is < 65536 and ARRAYSIZE is big enough
// problem: it truncates numbers at size without rounding
// str is a char array to hold the result, float is the number to convert
// size is the number of decimal digits you want


