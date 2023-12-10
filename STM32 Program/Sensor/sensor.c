#include "Sensor/sensor.h"

void Sensor_initParameters()
{
	Sensor_counter = 0;
	Sensor_numberSensor = 0;
	Sensor_result = 0;
	Sensor_intResult[0] = 0;
	Sensor_intResult[1] = 0;
	Sensor_intResult[2] = 0;
	Sensor_numberAxie = 0;
}

void Sensor_choiseSensor()
{
	Sensor_counter++;
	if(Sensor_counter > 5)
	{
		Sensor_counter = 0;
		Sensor_numberSensor = 0;
	}

	if(Sensor_counter > 2)
	{
		Sensor_numberSensor = 1;
	}

	if(Sensor_counter > 5)
	{
		Sensor_numberSensor = 2;
	}
}

void Sensor_changeNumberAxie()
{
	Sensor_numberAxie++;
	if(Sensor_numberAxie > 2)
	{
		Sensor_numberAxie = 0;
	}
}
