#include <avr/io.h>

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