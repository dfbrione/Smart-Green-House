/**
 *****************************************************************************************
 
 
 
 *  @brief      Read the temperature of the seesaw board in degrees Celsius.
 *NOTE: not all seesaw firmwares have the temperature sensor enabled.
 *  @return     Temperature in degrees Celsius as a floating point value.
 ****************************************************************************************/
float Adafruit_seesaw::getTemp() {
  uint8_t buf[4];
  this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP, buf, 4, 1000);
  int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
  return (1.0 / (1UL << 16)) * ret;
}



/*!
 ******************************************************************************
 *  @brief      read the analog value on an capacitive touch-enabled pin.
 *
 *  @param      pin the number of the pin to read.
 *
 *  @return     the analog value. This is an integer between 0 and 1023
 ****************************************************************************/
uint16_t Adafruit_seesaw::touchRead(uint8_t pin) {
  uint8_t buf[2];
  uint8_t p = pin;
  uint16_t ret = 65535;

  for (uint8_t retry = 0; retry < 5; retry++) {
    if (this->read(SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET + p, buf, 2,
                   3000 + retry * 1000)) {
      ret = ((uint16_t)buf[0] << 8) | buf[1];
      break;
    }
  }
  return ret;
}
