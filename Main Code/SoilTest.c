#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <string.h>

#include "SoilTest.h"

void init_i2c(uint8_t bdiv)
{
    TWSR = 0;                           // Set prescalar for 1
    TWBR = bdiv;                        // Set bit rate register
}

/*void i2c_meaningful_status(uint8_t status) {
	switch (status) {
		case 0x08: // START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("START\n"));
			break;
		case 0x10: // repeated START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("RESTART\n"));
			break;
		case 0x38: // NAK or DATA ARBITRATION LOST
			printf_P(PSTR("NOARB/NAK\n"));
			break;
		// MASTER TRANSMIT
		case 0x18: // SLA+W transmitted, ACK received
			printf_P(PSTR("MT SLA+W, ACK\n"));
			break;
		case 0x20: // SLA+W transmitted, NAK received
			printf_P(PSTR("MT SLA+W, NAK\n"));
				break;
		case 0x28: // DATA transmitted, ACK received
			printf_P(PSTR("MT DATA+W, ACK\n"));
			break;
		case 0x30: // DATA transmitted, NAK received
			printf_P(PSTR("MT DATA+W, NAK\n"));
			break;
		// MASTER RECEIVE
		case 0x40: // SLA+R transmitted, ACK received
			printf_P(PSTR("MR SLA+R, ACK\n"));
			break;
		case 0x48: // SLA+R transmitted, NAK received
			printf_P(PSTR("MR SLA+R, NAK\n"));
			break;
		case 0x50: // DATA received, ACK sent
			printf_P(PSTR("MR DATA+R, ACK\n"));
			break;
		case 0x58: // DATA received, NAK sent
			printf_P(PSTR("MR DATA+R, NAK\n"));
			break;
		default:
			printf_P(PSTR("N/A %02X\n"), status);
			break;
	}
}*/

inline void i2c_wait_on_TWINT(void) {
	while (!(TWCR & (1 << TWINT)))
		;
}

inline void i2c_start() {
	TWCR = (1 << TWSTA | 1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();
}

inline void i2c_stop() {
	TWCR = (1 << TWSTO | 1 << TWEN | 1 << TWINT);
	_delay_us(1000); // Recommended delay from adafruit
}

inline uint8_t i2c_get_status(void) {
	return TWSR & 0xF8;
}

inline void i2c_emit_addr(uint8_t address, uint8_t rw) {
	TWDR = address | rw;
	TWCR = (1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();
}

inline void i2c_emit_byte(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();
}

inline uint8_t i2c_read_ACK() {
	TWCR = (1 << TWEN | 1 << TWEA | 1 << TWINT);
	i2c_wait_on_TWINT();

	return TWDR;
}

inline uint8_t i2c_read_NAK() {
	TWCR = (1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();

	return TWDR;
}

void soilsensor_read_moisture(uint8_t *buf, uint8_t regHigh, uint8_t regLow, uint8_t len) {
	uint8_t ix = 0;
	uint8_t *ptr = buf;

	i2c_start();
	i2c_emit_addr((SOILSENSOR_ADDR << 1), I2C_W);

	i2c_emit_byte(regHigh); // Module base address
	i2c_emit_byte(regLow); // Function address

	i2c_start();
	i2c_emit_addr((SOILSENSOR_ADDR << 1), I2C_R);

	do {
		*(ptr++) = i2c_read_ACK();
	} while (ix++ < 1);

	i2c_stop();
}