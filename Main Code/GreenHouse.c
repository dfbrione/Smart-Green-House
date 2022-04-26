#include "Greenhouse.h"

int main (void) {
	return 0;
}

void init () {
	// init lcd
	lcd_init();

	// // init serial
	// serial_init(0x2F);

	// init SCD40 CO2 Temp Humidity Sensor
	uint16_t error;

	int count;
	char buf[16];
	
	sensirion_i2c_hal_init();

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();
}