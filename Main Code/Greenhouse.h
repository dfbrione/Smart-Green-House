#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <stdio.h>
#include "Serial.h"
#include <util/delay.h>
#include "Parallel_LCD_4b.h"
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "SoilSensor.h"

#define CO2_THRESHOLD					1000

enum states { //Enum to tie our states to constant numbers

	INITIAL_STATE = 0,
	DAY_OPEN = 1,
	DAY_CLOSED = 2,
	NIGHT_OPEN = 3,
	NIGHT_CLOSED = 4

};

void init();
//uint16_t scd40_check_for_error();

#endif /* GREENHOUSE_H */