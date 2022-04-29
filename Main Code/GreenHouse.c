#include "Greenhouse.h"

//Check header file for initializations and declarations
//Skeleton code for state machine has been added. Button debouncing code still needs to be added.

int main (void) {

	int state = INITIAL_STATE;
	bool flag_waterLevelLow = false;
	bool flag_soilMoistureLow = false;

	uint32_t counter_debounce = 0;

	char buf[17]; //buffer to write to LCD
	uint16_t error;

	init(); //Inialize everything
	bool data_ready_flag = false;
	bool button = false;
	bool prev_button = false;
	uint16_t co2;
	int32_t temperature;
	int32_t mf;
	int32_t humidity;
	//double tempC = getTemp();
	//uint16_t capread = touchRead(0);
	uint8_t capBuf[2];
	uint16_t capread;
	soilsensor_read_moisture(capBuf, SOILSENSOR_TOUCH_BASE, SOILSENSOR_TOUCH_CHANNEL_OFFSET, 2);
	capread = ((uint16_t)capBuf[0] << 8) | capBuf[1];
	
	char serial_buffer[255];
	int count;
	

	while(1) {  //Continually run the state machine. Please fill out state transitions and functionality

		// SCD40 Sensor Measurement BEGIN
		sensirion_i2c_hal_sleep_usec(5000);
		data_ready_flag = false;
		error = scd4x_get_data_ready_flag(&data_ready_flag);
		if (error) {
			//count = snprintf(buf,255,"Error executing scd4x_get_data_ready_flag(): %i\n", error);
			//serial_write(buf,count);
			snprintf(buf,17,"CO2ErrDataFlag%i",error);
			lcd_writecommand(1); // Clear LCD
			lcd_moveto(0,0);
			lcd_stringout(buf);
			continue;
		} else if (data_ready_flag) {
			error = scd4x_read_measurement(&co2, &temperature, &humidity);

			if (error) {
				//count = snprintf(buf,255,"Error executing scd4x_read_measurement(): %i\n", error);
				//serial_write(buf,count);
				//snprintf(buf,17,"CO2ErrRead:%04i   ",error);
				//lcd_writecommand(1);
				//lcd_moveto(0,0);
				//lcd_stringout(buf);
			} 
			else if (co2 == 0) {
				//count = snprintf(buf,255,"Invalid sample detected, skipping.\n");
				//serial_write(buf,count);
				//lcd_writecommand(1);
				//lcd_moveto(0,0);
				//lcd_stringout("CO2 Read Invalid");
				//lcd_moveto(1,0);
				//lcd_stringout("Skipping...");
			} 
			else {
				//count = snprintf(buf,255,"CO2: %u\n", co2);
				//serial_write(buf,count);
				//count = snprintf(buf,255,"Temperature: %d m°C\n", temperature);
				//serial_write(buf,count);
				//count = snprintf(buf,255,"Humidity: %d mRH\n", humidity);
				//serial_write(buf,count);
				mf = temperature *9/5+32000;
				//snprintf(buf,17,"CO2: %04u %02ld.%01ld C",co2,temperature/1000,(temperature%1000)/100);
				snprintf(buf,17,"CO2: %04u",co2);
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
		}

		//on channel  0x36 -> Bit Shift -> 0x49
	    //tempC = getTemp();
	    //uint16_t capread = touchRead(0);
		soilsensor_read_moisture(capBuf, SOILSENSOR_TOUCH_BASE, SOILSENSOR_TOUCH_CHANNEL_OFFSET, 2);
		capread = ((uint16_t)capBuf[0] << 8) | capBuf[1];
		
		// count = snprintf(serial_buffer,255,"Temperature: %f*C\n",tempC);
		// serial_write(serial_buffer,count);
		count = snprintf(serial_buffer,255,"Cap: %i\n",capread);   
		serial_write(serial_buffer,count);
		// SCD40 Sensor Measurement END

		// Water Level Sensor Measurement START
		flag_waterLevelLow = read_waterLevelLow(flag_waterLevelLow);
		// Water Level Sensor Measurement END

		// Soil Moisture Sensor Measurement START
		// TODO: Moisture Sensor Read Logic
		// Soil Moisture Sensor Measurement END

		// Button Read START
		button = readButton(counter_debounce);
		// Button Read END

		// Following lines are for state transition and output logic. Please fill in accordingly
		if (state == INITIAL_STATE) { //Code for initial state
			state = NIGHT_CLOSED;
		}
		else if (state == DAY_OPEN) { //Code for the DAY_OPEN state
			if (button && !prev_button){
				state = NIGHT_OPEN;
			}
			// TODO: open air duct
			growLightLED_ON();
		}
		else if (state == DAY_CLOSED) { //Code for the DAY_CLOSED state
			if (button && !prev_button){
				state = NIGHT_CLOSED;
			}
			// TODO: close air duct
			growLightLED_ON();
		}
		else if (state == NIGHT_OPEN) { //Code for the NIGHT_OPEN state
			if (button && !prev_button){
				state = DAY_OPEN;
			}
			// TODO: open air duct
			growLightLED_OFF();
		}
		else if (state == NIGHT_CLOSED) { //Code for the NIGHT_CLOSED state
			if (button && !prev_button){
				state = DAY_CLOSED;
			}
			// TODO: close air duct
			growLightLED_OFF();
		}

		if (flag_waterLevelLow){
			lowWaterLevelLED_ON();
		}
		else {
			lowWaterLevelLED_OFF();
		}

		if (flag_soilMoistureLow){
			// TODO: Open the solenoid Valve
			// needs logic design for debouncing and controling the water flow
		}

		prev_button = button;
	}
	return 0;
} 

void init () { //INITIALIZE EVERYTHING
	// init lcd START
	lcd_init();
	lcd_writecommand(1); //Clear LCD
	lcd_moveto(0,0);
	lcd_stringout("Smart Greenhouse");
	_delay_ms(500); //Delay for half a second so user can see startup screen
	// init lcd END

	// init serial for testing (Show up on monitor) START
	serial_init(0x2F);
	// init serial for testing END

	// init SCD40 CO2 Temp Humidity Sensor START
	//sensirion_i2c_hal_init();
    // Clean up potential SCD40 states
   // scd4x_wake_up();
   // scd4x_stop_periodic_measurement();
   // scd4x_reinit();

	int count;
	char buf[16];

	
	uint16_t serial_0;
    uint16_t serial_1;
   	uint16_t serial_2;
   	uint16_t error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
   	if (error) {
    	   count = snprintf(buf,16,"CO2ErrorGetSN:%i", error);
   	} 
	else {
        count = snprintf(buf,16,"SN: %04x%04x%04x ", serial_0, serial_1, serial_2);
	}

	

	lcd_writecommand(1); //Clear LCD
	lcd_moveto(0,0);
	lcd_stringout("CO2 Sensor Init");
	lcd_moveto(1,0);
	lcd_stringout(buf);

	// Delay for half a second so user can see init screen
	_delay_ms(500);

	error = scd4x_start_periodic_measurement();
	if (error) {
		lcd_moveto(0,0);
		lcd_stringout("CO2Err:StartMeas");
	}
	// init SCD40 CO2 Temp Humidity Sensor END

	// init Water Level Sensor START
	DDRC &= ~(1 << MIN_WATER_LEVEL_SENSOR_PINOUT | 1 << MAX_WATER_LEVEL_SENSOR_PINOUT);
	PORTC &= ~(1 << MIN_WATER_LEVEL_SENSOR_PINOUT | 1 << MAX_WATER_LEVEL_SENSOR_PINOUT);
	// init Water Level Sensor END

	// init Low Water Level LED START
	DDRB |= (1 << LOW_WATER_LEVEL_LED_PINOUT);
	// init Low Water Level LED END

	// init Grow Light LED START
	DDRB |= (1 << GROW_LIGHT_LED_PINOUT);
	// init Grow Light LED END

	// init Button START
	DDRB &= ~(1 << BUTTON_PINOUT);
	PORTB |= (1 << BUTTON_PINOUT);
	// init Button END
}

bool read_waterLevelLow(bool previousWaterLevelLow) {
	if (((PINC & (1 << MIN_WATER_LEVEL_SENSOR_PINOUT)) != 0) && !previousWaterLevelLow) { // READ 1
		// PC2 MIN WATER LEVEL NOT CONTACT
		return true;
	}
	else if ( ((PINC & (1 << MAX_WATER_LEVEL_SENSOR_PINOUT)) == 0 ) && previousWaterLevelLow) { // READ 0
		// PC3 MAX WATER LEVEL IN CONTACT
		return false;
	} else {
		return previousWaterLevelLow;
	}
}

void lowWaterLevelLED_ON() {
	PORTB |= (1 << LOW_WATER_LEVEL_LED_PINOUT);
}

void lowWaterLevelLED_OFF() {
	PORTB &= ~(1 << LOW_WATER_LEVEL_LED_PINOUT);
}

void growLightLED_ON() {
	PORTB |= (1 << GROW_LIGHT_LED_PINOUT);
}

void growLightLED_OFF() {
	PORTB &= ~(1 << GROW_LIGHT_LED_PINOUT);
}

bool readButton(uint32_t counter&) {
	if (!(PINB & (1 << BUTTON_PINOUT))) {
		counter++;
	} else {
		counter = 0;
	}
	if (counter >= DEBOUNCE_INTERVAL) {
		counter -= DEBOUNCE_INTERVAL;
		return true;
	} else {
		return false;
	}
}