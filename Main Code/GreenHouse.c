#include "Parallel_LCD_4b.h"

int main(void) {

  lcd_init();
  //lcd_writecommand(1); //Clear LCD
  
  
 
  lcd_moveto(1, 3);
  lcd_writedata('c');

  while(1){


  }

}