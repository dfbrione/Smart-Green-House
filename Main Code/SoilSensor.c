#include "SoilSensor.h"
//#include <Arduino.h>
#include <stdio.h>


/*!
 *    @brief  Read from I2C into a buffer from the I2C device.
 *    Cannot be more than maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to read into
 *    @param  len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal on read
 *    @return True if read was successful, otherwise false.
 */
bool read(uint8_t *buffer, size_t len, bool stop) {
  size_t pos = 0;
  while (pos < len) {
    size_t read_len =
        ((len - pos) > maxBufferSize()) ? maxBufferSize() : (len - pos);
    bool read_stop = (pos < (len - read_len)) ? false : stop;
    if (!_read(buffer + pos, read_len, read_stop))
      return false;
    pos += read_len;
  }
  return true;
}

bool read(uint8_t regHigh, uint8_t regLow, uint8_t *buf,
                           uint8_t num, uint16_t delay) {
  uint8_t pos = 0;
  uint8_t prefix[2];
  prefix[0] = (uint8_t)regHigh;
  prefix[1] = (uint8_t)regLow;

  // on arduino we need to read in 32 byte chunks
  while (pos < num) {
    uint8_t read_now = min(32, num - pos);

    if (_flow != -1) {
      while (digitalRead(_flow))
        yield();
    }

    if (!_i2c_dev->write(prefix, 2)) {
      return false;
    }

    // TODO: tune this
    delayMicroseconds(delay);

    if (_flow != -1) {
      while (digitalRead(_flow))
        yield();
    }

#ifdef SEESAW_I2C_DEBUG
    Serial.print("Reading ");
    Serial.print(read_now);
    Serial.println(" bytes");
#endif

    if (!_i2c_dev->read(buf + pos, read_now)) {
      return false;
    }
    pos += read_now;
#ifdef SEESAW_I2C_DEBUG
    Serial.print("pos: ");
    Serial.print(pos);
    Serial.print(" num:");
    Serial.println(num);
#endif
  }
  return true;
}




/**
 *****************************************************************************************
 *  @brief      Read the temperature of the seesaw board in degrees Celsius.
 *NOTE: not all seesaw firmwares have the temperature sensor enabled.
 *  @return     Temperature in degrees Celsius as a floating point value.
 ****************************************************************************************/
double getTemp() {
  uint8_t buf[4];
  read(SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP, buf, 4, 1000);
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
uint16_t touchRead(uint8_t pin) {
  uint8_t buf[2];
  uint8_t p = pin;
  uint16_t ret = 65535;

  for (uint8_t retry = 0; retry < 5; retry++) {
    if (read(SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET + p, buf, 2,
                   3000 + retry * 1000)) {
      ret = ((uint16_t)buf[0] << 8) | buf[1];
      break;
    }
  }
  return ret;
}
