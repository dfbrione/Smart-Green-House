#ifndef GREENHOUSE_H
#define GREENHOUSE_H

#include <stdio.h>

#include "Serial.h"
#include "Parallel_LCD_4b.h"
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "SoilSensor.h"

void init();
uint16_t scd40_check_for_error();

#endif /* GREENHOUSE_H */