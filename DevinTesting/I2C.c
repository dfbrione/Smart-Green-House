/*************************************************************
*       I2C.c - Demonstrate I2C functions of ATmega328
*************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "scd4x/scd4x_i2c.h"

#define FOSC 7372800
#define BDIV (FOSC / 10000 - 16) / 2 + 1


/*
serial_init - Initialize the USART port
*/

void serial_init ( unsigned short ubrr ) {
UBRR0 = ubrr ; // Set baud rate
UCSR0B |= (1 << TXEN0 ); // Turn on transmitter
UCSR0B |= (1 << RXEN0 ); // Turn on receiver
UCSR0C = (3 << UCSZ00 ); // Set for async . operation , no parity ,
// one stop bit , 8 data bits
}
/*
serial_out - Output a byte to the USART0 port
*/
void serial_out ( char ch )
{
while (( UCSR0A & (1 << UDRE0 )) == 0);
UDR0 = ch ;
}
/*
serial_in - Read a byte from the USART0 and return it
*/
char serial_in ()
{
while ( !( UCSR0A & (1 << RXC0 )) );
return UDR0 ;
}

void i2c_init()
{	
	TWSR = 0; 		// Set prescalar for 1
	TWBR = BDIV;	// Set bit rate register
	i2c_init(BDIV);
	
	/*unsigned char status;
	unsigned char addr = 9;
	unsigned char buf[4] = {1, 2, 3, 4};
	
	status = i2c_io(0x62, &addr, 1, buf, 4, NULL, 0);
	*/
	
	uint16_t error;
    char errorMessage[256];
	
	
}

int main (void){	
	serial_init(0x2F);
	char inp;
	while(1){
		inp = serial_in();
		serial_out(inp);
	}
}
