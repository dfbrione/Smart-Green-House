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
  
  
  Wire.begin();

    uint16_t error;
    char errorMessage[256];

    scd4x.begin(Wire);

    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
  
  error = scd4x.startPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

  
  
  while(1) {
    
    
     // Read Measurement
    uint16_t co2;
    float temperature;
    float humidity;
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping.");
    } else {
        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print("\t");
        Serial.print("Temperature:");
        Serial.print(temperature);
        Serial.print("\t");
        Serial.print("Humidity:");
        Serial.println(humidity);
    }
    
    
    
    
    
        //on channel  0x36)
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);
  delay(100);
  }

}
