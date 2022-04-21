#include <avr/io.h>

void serial_init ( unsigned short ubrr );
void serial_out ( char ch );
void serial_write(char* str, int count);