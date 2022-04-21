#include "Parallel_LCD_4b.h"
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include <avr/io.h>
#include <stdio.h>  // printf

int main(void) {

	lcd_init();
    //lcd_writecommand(1); //Clear LCD
  
  
 
	lcd_moveto(1, 3);
    lcd_writedata('c');

    uint16_t error;
    char errorMessage[256];
    
	sensirion_i2c_hal_init();
	int count;
	char buf[16];

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();

    uint16_t serial_0;
    uint16_t serial_1;
    uint16_t serial_2;
    error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error) {
        count = snprintf(buf,16,"CO2ErrorGetSN:%i", error);
    } else {
        count = snprintf(buf,16,"ser:%04x%04x%04x", serial_0, serial_1, serial_2);
    }
	
	lcd_writecommand(1); //Clear LCD
	lcd_moveto(0,0);
	lcd_stringout("CO2 Initializing");
	lcd_moveto(1,0);
	lcd_stringout(buf);
	
	/*
    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        //Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        lcd_moveto(0,0);
		lcd_stringout("CO2Err:StopMeas ");
		errorToString(error, errorMessage, 256);
        //Serial.println(errorMessage);
		lcd_moveto(1,0);
		lcd_stringout(errorMessage);
    }
    */
    error = scd4x_start_periodic_measurement();
    if (error) {
        //Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        lcd_moveto(0,0);
		lcd_stringout("CO2Err:StartMeas");
		//errorToString(error, errorMessage, 256);
        //Serial.println(errorMessage);
		//lcd_moveto(1,0);
		//lcd_stringout(errorMessage);
    }

  
  
  while(1) {
    char buf[16];
    
    // Read Measurement
    sensirion_i2c_hal_sleep_usec(100000);
	bool data_ready_flag = false;
	error = scd4x_get_data_ready_flag(&data_ready_flag);
	if (error) {
		//count = snprintf(buf,255,"Error executing scd4x_get_data_ready_flag(): %i\n", error);
		//serial_write(buf,count);
		snprintf(buf,16,"CO2ErrDataFlag%i",error);
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
	int32_t humidity;
	error = scd4x_read_measurement(&co2, &temperature, &humidity);
	if (error) {
		//count = snprintf(buf,255,"Error executing scd4x_read_measurement(): %i\n", error);
		//serial_write(buf,count);
		snprintf(buf,16,"CO2ErrRead:%i   ",error);
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
		snprintf(buf,16,"CO2: %u",co2);
		lcd_writecommand(1);
		lcd_moveto(0,0);
		lcd_stringout(buf);
		snprintf(buf,16,"Temp: %ld.%li C",temperature/1000, temperature%1000);
		lcd_moveto(1,0);
		lcd_stringout(buf);
	}
/*
	sensirion_i2c_hal_sleep_usec(10000);

    //on channel  0x36)
    float tempC = getTemp();
    uint16_t capread = touchRead(0);

    Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
    Serial.print("Capacitive: "); Serial.println(capread);
    */
    }

}
