float readMQ(uint16_t RL_VALUE, int CH);
float getMQResistance(uint16_t raw_adc, uint16_t RL_VALUE);
float getConcentration(float rs_ro_ratio,float coord, float scope);
int sensorlecturamq(uint16_t RL, float X0, float X1, float Y0,float Y1,float R0, int CH);
#define RL_MQ4 20
#define RL_MQ6 20
#define RL_MQ7 10
//#define R0_MQ4 4.4
//#define R0_MQ6 10
//#define R0_MQ7 28 //viejos

#define R0_MQ4 4.144  //con cuentas
#define R0_MQ6 2.101
#define R0_MQ7 0.010

#define X0_MQ4 200
#define X1_MQ4 10000
#define X0_MQ6 200
#define X1_MQ6 10000
#define X0_MQ7 50
#define X1_MQ7 4000
#define Y0_MQ4 1.86
#define Y1_MQ4 0.44
#define Y0_MQ6 2.1
#define Y1_MQ6 0.4
#define Y0_MQ7 1.75
#define Y1_MQ7 0.1
