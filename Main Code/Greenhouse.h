#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <stdio.h>
#include "Serial.h"
#include <util/delay.h>
#include "Parallel_LCD_4b.h"
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "SoilTest.h"

#define MIN_WATER_LEVEL_SENSOR_PINOUT	PC2
#define MAX_WATER_LEVEL_SENSOR_PINOUT	PC3
#define LOW_WATER_LEVEL_LED_PINOUT		PB0
#define GROW_LIGHT_LED_PINOUT			PB1

#define CO2_THRESHOLD					1000
#define SLEEP_INTERVAL_SCD40			1000000 / (2 * 7372800) + 1

enum states { //Enum to tie our states to constant numbers

	INITIAL_STATE = 0,
	DAY_OPEN = 1,
	DAY_CLOSED = 2,
	NIGHT_OPEN = 3,
	NIGHT_CLOSED = 4

};

void init();
bool read_waterLevelLow(bool previousWaterLevelLow);
void lowWaterLevelLED_ON();
void lowWaterLevelLED_OFF();
void growLightLED_ON();
void growLightLED_OFF();
//uint16_t scd40_check_for_error();

#endif /* GREENHOUSE_H */