#include "Parallel_LCD_4b.h"

int main(void) {

  lcd_init();
  //lcd_writecommand(1); //Clear LCD
  
  
 
  lcd_moveto(1, 3);
  lcd_writedata('c');

  
    unsigned char status ;
    unsigned char abuf = {0 x01 , 0 x20 };
    unsigned char rbuf [32];
    
    status = i2c_io (0 x62 , abuf , 2 , NULL , 0 , rbuf , 32);
  
  while(1){

  }

}
