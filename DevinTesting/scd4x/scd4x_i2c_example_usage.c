/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>  // printf

#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"

#include <avr/io.h>

/**
 * TO USE CONSOLE OUTPUT (PRINTF) IF NOT PRESENT ON YOUR PLATFORM
 */
//#define printf(...)

void serial_init ( unsigned short ubrr ) {
	UBRR0 = ubrr ; // Set baud rate
	UCSR0B |= (1 << TXEN0 ); // Turn on transmitter
	UCSR0B |= (1 << RXEN0 ); // Turn on receiver
	UCSR0C = (3 << UCSZ00 ); // Set for async . operation , no parity ,
	// one stop bit , 8 data bits
}
void serial_out ( char ch )
{
	while (( UCSR0A & (1 << UDRE0 )) == 0);
		UDR0 = ch ;
}

void serial_write(char* str, int count){
	int i;
	for (i = 0; i < count; i++){
		serial_out(str[i]);
	}
}

int main(void) {
    int16_t error = 0;
	
	serial_init(0x2F);
	serial_out('!');
    sensirion_i2c_hal_init();
	int count;
	char buf[255];

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();

    uint16_t serial_0;
    uint16_t serial_1;
    uint16_t serial_2;
    error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error) {
        count = snprintf(buf,255,"Error executing scd4x_get_serial_number(): %i\n", error);
    } else {
        count = snprintf(buf,255,"serial: 0x%04x%04x%04x\n", serial_0, serial_1, serial_2);
    }
	serial_write(buf,count);
    // Start Measurement

    error = scd4x_start_periodic_measurement();
    if (error) {
        count = snprintf(buf,255,"Error executing scd4x_start_periodic_measurement(): %i\n",
               error);
    }

    count = snprintf(buf,255,"Waiting for first measurement... (5 sec)\n");
	serial_write(buf,count);

    for (;;) {
        // Read Measurement
        sensirion_i2c_hal_sleep_usec(100000);
        bool data_ready_flag = false;
        error = scd4x_get_data_ready_flag(&data_ready_flag);
        if (error) {
            count = snprintf(buf,255,"Error executing scd4x_get_data_ready_flag(): %i\n", error);
			serial_write(buf,count);
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
            count = snprintf(buf,255,"Error executing scd4x_read_measurement(): %i\n", error);
			serial_write(buf,count);
        } else if (co2 == 0) {
            count = snprintf(buf,255,"Invalid sample detected, skipping.\n");
			serial_write(buf,count);
        } else {
            count = snprintf(buf,255,"CO2: %u\n", co2);
			serial_write(buf,count);
            count = snprintf(buf,255,"Temperature: %d m°C\n", temperature);
			serial_write(buf,count);
            count = snprintf(buf,255,"Humidity: %d mRH\n", humidity);
			serial_write(buf,count);
        }
    }

    return 0;
}
