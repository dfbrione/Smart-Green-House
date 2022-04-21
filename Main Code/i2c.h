#ifndef IC2_H
#define IC2_H

#include <stdint.h>

extern void i2c_init(uint8_t);
extern uint8_t i2c_io(uint8_t, uint8_t *, uint16_t,
		      uint8_t *, uint16_t, uint8_t *, uint16_t);


#endif /* IC2_H */