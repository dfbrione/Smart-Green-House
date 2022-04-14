#include "i2c.h"
#include <stdio.h>

#define ADDR 0x62
#define REINIT 0x3646
#define FACTORYRESET 0x3632
#define FORCEDRECAL 0x362F
#define SELFTEST 0x3639
#define DATAREADY 0xE4B8
#define STOPPERIODICMEASUREMENT 0x3F86
#define STARTPERIODICMEASUREMENT 0x21B1
#define STARTLOWPOWERPERIODICMEASUREMENT 0x21AC
#define READMEASUREMENT 0xEC05
#define SERIALNUMBER 0x3682
#define GETTEMPOFFSET 0x2318
#define SETTEMPOFFSET 0x241D
#define GETALTITUDE 0x2322
#define SETALTITUDE 0x2427
#define SETPRESSURE 0xE000
#define PERSISTSETTINGS 0x3615
#define GETASCE 0x2313
#define SETASCE 0x2416

int main(void){
	i2c_init();
	unsigned char status;
	unsigned char buffer[4] = {1, 2, 3, 4};
	//uint16_t offset = 0;
	unsigned char ad = 9;
	//uint16_t data = SERIALNUMBER;
	
	//buffer[offset++] = (uint8_t)((data & 0xFF00) >> 8);
    //buffer[offset++] = (uint8_t)((data & 0x00FF) >> 0);
	
	status = i2c_io(ADDR, &ad, 1, buffer, 4, NULL, 0);
}