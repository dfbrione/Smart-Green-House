#include "Parallel_LCD_4b.h"
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include <stdio.h>
#include "Serial.h"
#include "SoilSensor.h"

int main(void) {

	//lcd_init();

    //uint16_t error;
    //char errorMessage[256];
    
	//sensirion_i2c_hal_init();
	//int count;
	//char buf[16];
	
	//serial_init(0x2F);

    // Clean up potential SCD40 states
    //scd4x_wake_up();
    //scd4x_stop_periodic_measurement();
    //scd4x_reinit();

    uint16_t serial_0;
    uint16_t serial_1;
    uint16_t serial_2;
    error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error) {
        count = snprintf(buf,16,"CO2ErrorGetSN:%i", error);
    } else {
        count = snprintf(buf,16,"SN: %04x%04x%04x ", serial_0, serial_1, serial_2);
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
    char buf[17];
    
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
	

    }

}
