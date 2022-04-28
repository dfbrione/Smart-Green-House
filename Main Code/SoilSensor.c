#include "SoilSensor.h"
//#include <Arduino.h>
#include <stdio.h>
#include <util/delay.h>
#include "i2c.h"

#define true 1
#define false 0
#define maxBufferSize 1024
#define SEESAW_I2C_DEBUG 1


int min(int a, int b)
{
	if (a <= b)
		return a;
	return b;
}

void yield()
{
	// Do nothing
}

/*!
 *    @brief  Read from I2C into a buffer from the I2C device.
 *    Cannot be more than maxBufferSize bytes.
 *    @param  buffer Pointer to buffer of data to read into
 *    @param  len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal on read
 *    @return True if read was successful, otherwise false.
 */
int read1(uint8_t *buffer, size_t len, int stop) {
  size_t pos = 0;
  while (pos < len) {
    size_t read_len = ((len - pos) > maxBufferSize) ? maxBufferSize : (len - pos);
    int read_stop = (pos < (len - read_len)) ? false : stop;
    if (!read1(buffer + pos, read_len, read_stop))
      return false;
    pos += read_len;
  }
  return true;
}

int read(uint8_t regHigh, uint8_t regLow, uint8_t *buf,
                           uint8_t num, uint16_t delay) {
  uint8_t pos = 0;
  uint8_t prefix[2];
  prefix[0] = (uint8_t)regHigh;
  prefix[1] = (uint8_t)regLow;

  // on arduino we need to read in 32 byte chunks
  while (pos < num) {
    uint8_t read_now = min(32, num - pos);

    if (!i2c_io(SEESAW_ADDRESS,NULL,0,prefix, 2,NULL,0)) {
      return false;
    }

    // TODO: tune this
    // _delay_us(delay);
    _delay_us(200);
    char serial_buffer[255];
    int count;

#ifdef SEESAW_I2C_DEBUG
    count = snprintf(serial_buffer, 255, "Reading %u Bytes\n", read_now);
    serial_write(serial_buffer, count);
#endif

    if (!i2c_io(SEESAW_ADDRESS,NULL,0,NULL,0,buf + pos, read_now)) {
      return false;
    }
    pos += read_now;
#ifdef SEESAW_I2C_DEBUG
    count = snprintf(serial_buffer, 255, "Pos: %u  Num: %u\n", pos, num);
    serial_write(serial_buffer, count);
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

  uint8_t retry;
  for (retry = 0; retry < 5; retry++) {
    if (read(SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET + p, buf, 2,
                   3000 + retry * 1000)) {
      ret = ((uint16_t)buf[0] << 8) | buf[1];
      break;
    }
  }
  return ret;
}
