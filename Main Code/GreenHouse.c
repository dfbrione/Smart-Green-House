#include "Greenhouse.h"

int main (void) {
	return 0;
}

void init () {
	// init lcd
	lcd_init();

	// // init serial for testing (Show up on monitor)
	 serial_init(0x2F);

	// init SCD40 CO2 Temp Humidity Sensor
	

	int count;
	char buf[16];
	
	sensirion_i2c_hal_init();

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();
}

uint16_t scd40_check_for_error() { //Please check return value of this function. INCOMPLETE
	uint16_t error;
	error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error) {
        count = snprintf(buf,16,"CO2ErrorGetSN:%i", error);
    } else {
        count = snprintf(buf,16,"SN: %04x%04x%04x ", serial_0, serial_1, serial_2);
    }
	
}