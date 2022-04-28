#include "Greenhouse.h"

//Check header file for initializations and declarations
//Skeleton code for state machine has been added. Button debouncing code still needs to be added.

int main (void) {

	int state = INITIAL_STATE;
	char buf[17]; //buffer to write to LCD
	uint16_t error;
	

	init(); //Inialize everything

	while(1) {  //Continually run the state machine. Please fill out state transitions and functionality

    //Following lines are responsible for the SCD40 Sensor

    // Read Measurement
    sensirion_i2c_hal_sleep_usec(100000);
	bool data_ready_flag = false;
	error = scd4x_get_data_ready_flag(&data_ready_flag);
	if (error) {
		//count = snprintf(buf,255,"Error executing scd4x_get_data_ready_flag(): %i\n", error);
		//serial_write(buf,count);
		snprintf(buf,17,"CO2ErrDataFlag%i",error);
		lcd_writecommand(1); // Clear LCD
		lcd_moveto(0,0);
		lcd_stringout(buf);
		continue;
	}
	if (!data_ready_flag) {
		continue;
	}

	uint16_t co2;
	int32_t temperature;
	int32_t mf;
	int32_t humidity;
	error = scd4x_read_measurement(&co2, &temperature, &humidity);
	if (error) {
		//count = snprintf(buf,255,"Error executing scd4x_read_measurement(): %i\n", error);
		//serial_write(buf,count);
		snprintf(buf,17,"CO2ErrRead:%04i   ",error);
		lcd_writecommand(1);
		lcd_moveto(0,0);
		lcd_stringout(buf);
	} else if (co2 == 0) {
		//count = snprintf(buf,255,"Invalid sample detected, skipping.\n");
		//serial_write(buf,count);
		lcd_writecommand(1);
		lcd_moveto(0,0);
		lcd_stringout("CO2 Read Invalid");
		lcd_moveto(1,0);
		lcd_stringout("Skipping...");
	} else {
		//count = snprintf(buf,255,"CO2: %u\n", co2);
		//serial_write(buf,count);
		//count = snprintf(buf,255,"Temperature: %d m°C\n", temperature);
		//serial_write(buf,count);
		//count = snprintf(buf,255,"Humidity: %d mRH\n", humidity);
		//serial_write(buf,count);
		mf = temperature *9/5+32000;
		snprintf(buf,17,"CO2: %04u %02ld.%01ld C",co2,temperature/1000,(temperature%1000)/100);
		lcd_writecommand(1);
		lcd_moveto(0,0);
		lcd_stringout(buf);
		snprintf(buf,17,"Temp: %02ld.%01ld F",mf/1000, (mf%1000)/100);
		lcd_moveto(1,0);
		lcd_stringout(buf);
		lcd_moveto(1,10);
		lcd_writedata((char) 223); // Insert degree symbol
		lcd_moveto(1,16);
	}

		sensirion_i2c_hal_sleep_usec(5000);

		//on channel  0x36 -> Bit Shift -> 0x49
		double tempC = getTemp();
		uint16_t capread = touchRead(0);
		char serial_buffer[255];
		int count;
    
		count = snprintf(serial_buffer,255,"Temperature: %f*C\n",tempC);
		serial_write(serial_buffer,count);
		count = snprintf(serial_buffer,255,"Capcaitive: %i\n",capread);   
		serial_write(serial_buffer,count);


		//Following lines are for state transition and output logic. Please fill in accordingly

		if (state == INITIAL_STATE) { //Code for initial state



		}

		else if (state == DAY_OPEN) { //Code for the DAY_OPEN state


		}


		else if (state == DAY_CLOSED) { //Code for the DAY_CLOSED state


		}


		else if (state == NIGHT_OPEN) { //Code for the NIGHT_OPEN state


		}

		else if (state == NIGHT_CLOSED) { //Code for the NIGHT_CLOSED state



		} 




	}


	return 0;
} 

void init () { //INITIALIZE EVERYTHING
	// init lcd
	lcd_init();
	lcd_writecommand(1); //Clear LCD
	lcd_moveto(0,0);
	lcd_stringout("Smart Greenhouse");
	_delay_ms(500); //Delay for half a second so user can see startup screen

	// // init serial for testing (Show up on monitor)
	 serial_init(0x2F);
	// init SCD40 CO2 Temp Humidity Sensor
	sensirion_i2c_hal_init();

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();

	char buf[16];
	int count;
	uint16_t serial_0;
    uint16_t serial_1;
    uint16_t serial_2;
    uint16_t error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error) {
        count = snprintf(buf,16,"CO2ErrorGetSN:%i", error);
    } else {
        count = snprintf(buf,16,"SN: %04x%04x%04x ", serial_0, serial_1, serial_2);
}

	lcd_writecommand(1); //Clear LCD
	lcd_moveto(0,0);
	lcd_stringout("C02 Sensor initialized");
	_delay_ms(500);

}
