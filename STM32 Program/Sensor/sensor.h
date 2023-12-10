#include <stm32f30x.h>

#define Sensor_xNumberAxie      0
#define Sensor_yNumberAxie      1
#define Sensor_zNumberAxie      2


volatile uint8_t Sensor_counter;

volatile uint8_t Sensor_numberSensor;

volatile uint16_t Sensor_result;

volatile int Sensor_intResult[3];

volatile uint8_t Sensor_numberAxie;
//Sensor_numberAxie = 0 - x
//Sensor_numberAxie = 1 - y
//Sensor_numberAxie = 2 - z

void Sensor_initParameters();

void Sensor_choiseSensor();

void Sensor_changeNumberAxie();
