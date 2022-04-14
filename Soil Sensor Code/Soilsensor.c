#include "Adafruit_seesaw.h"
#include <Arduino.h>

//#define SEESAW_I2C_DEBUG

/*!
 *****************************************************************************************
 *  @brief      Create a seesaw object on a given I2C bus
 *
 *  @param      i2c_bus the I2C bus connected to the seesaw, defaults to "Wire"
 ****************************************************************************************/
Adafruit_seesaw::Adafruit_seesaw(TwoWire *i2c_bus) {
  if (i2c_bus == NULL) {
    _i2cbus = &Wire;
  } else {
    _i2cbus = i2c_bus;
  }
}


/*!
 *****************************************************************************************
 *  @brief      Start the seesaw
 *
 *				This should be called when your sketch is
 *connecting to the seesaw
 *
 *  @param      addr the I2C address of the seesaw
 *  @param      flow the flow control pin to use
 *  @param		reset pass true to reset the seesaw on startup. Defaults
 *to true.
 *
 *  @return     true if we could connect to the seesaw, false otherwise
 ****************************************************************************************/
bool Adafruit_seesaw::begin(uint8_t addr, int8_t flow, bool reset) {
  _flow = flow;

  if (_flow != -1)
    ::pinMode(_flow, INPUT);

  if (_i2c_dev) {
    delete _i2c_dev;
  }

  _i2c_dev = new Adafruit_I2CDevice(addr, _i2cbus);

  bool found = false;
  for (int retries = 0; retries < 10; retries++) {
    if (_i2c_dev->begin()) {
      found = true;
      break;
    }
    delay(10);
  }

  if (!found) {
    return false;
  }

#ifdef SEESAW_I2C_DEBUG
  Serial.println("Begun");
#endif

  if (reset) {
    found = false;
    SWReset();
    for (int retries = 0; retries < 10; retries++) {
      if (_i2c_dev->detected()) {
        found = true;
        break;
      }
      delay(10);
    }
  }

  if (!found) {
    return false;
  }

#ifdef SEESAW_I2C_DEBUG
  Serial.println("Reset");
#endif

  found = false;
  for (int retries = 0; !found && retries < 10; retries++) {
    uint8_t c = 0;

    this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_HW_ID, &c, 1);
    if ((c == SEESAW_HW_ID_CODE_SAMD09) || (c == SEESAW_HW_ID_CODE_TINY8X7)) {
      found = true;
      _hardwaretype = c;
    }

    delay(10);
  }

#ifdef SEESAW_I2C_DEBUG
  Serial.println("Done!");
#endif

  return found;
}



/*!
 *******************************************************************
 *  @brief      perform a software reset. This resets all seesaw registers to
 *their default values.
 *  			This is called automatically from
 *Adafruit_seesaw.begin()
 * @returns  True on I2C write success, false otherwise
 ********************************************************************/
bool Adafruit_seesaw::SWReset() {
  return this->write8(SEESAW_STATUS_BASE, SEESAW_STATUS_SWRST, 0xFF);
}

/*!
 *********************************************************************
 *  @brief      Returns the version of the seesaw
 *  @return     The version code. Bits [31:16] will be a date code, [15:0] will
 *be the product id.
 ********************************************************************/
uint32_t Adafruit_seesaw::getVersion() {
  uint8_t buf[4];
  this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_VERSION, buf, 4);
  uint32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                 ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
  return ret;
}






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
