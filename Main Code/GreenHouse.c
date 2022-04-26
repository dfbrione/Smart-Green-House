#include "Greenhouse.h"
#include "Parallel_LCD_4b.h"
#include "sensirion_i2c_hal.h"
#include <util/delay.h>
#include "i2c.h"

#ifndef NULL
#define NULL 0
#endif




void splash(){
  lcd_writecommand(1);
  lcd_moveto(0,0);
  lcd_stringout("Greenhouse");
  _delay_ms(1000);
}


int main(void) {

  lcd_init();
  i2c_init(BDIV);
  splash(); //Display splash screen on LCD for 1 second
  //lcd_writecommand(1); //Clear LCD
  

  
    unsigned char status;
    unsigned char abuf = {'0x01'};
    unsigned char rbuf [32];
    unsigned char wbuf [2];

    sensirion_i2c_hal_init();

    
    
    //status = i2c_io(0xD0, abuf, 1, NULL, 0, rbuf, 32);
    //status = i2c_io(0xC5, abuf, 1, NULL, 50, rbuf, 32);
    //status = i2c_io(0xC4, NULL, 0, wbuf, 2, NULL, 0);

  
/*
   A typical write with a 2-byte address is done with

    i2c_io(0xA0, abuf, 2, wbuf, 50, NULL, 0);

A typical read with a 1-byte address is done with

    i2c_io(0xD0, abuf, 1, NULL, 0, rbuf, 20);

*/


}
