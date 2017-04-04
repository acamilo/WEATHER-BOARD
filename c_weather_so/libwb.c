#include <stdio.h>
#include "bme280-i2c.h"
#include "si1132.h"
#include "si702x.h"
#include "bmp180.h"
#include "time.h"
#include <unistd.h>
#include <getopt.h>
#include "libwb.h"



float SEALEVELPRESSURE_HPA = 1024.25;

int initWB(char * iic){
	// Does iic exist?
	if(access(iic,F_OK)==-1) return IIC_ERROR;
		
		
	// Could we write to iic with our current permissions?  
   if(access(iic,W_OK)==-1) return PERMISSIONS_ERROR;
   
	// init the sensors     
	if (si1132_begin(iic)==-1) return SI1132_ERROR;
	if (bme280_begin(iic)==-1) return BME280_ERROR;

	return OK;
}

void getWBData(float *temperature, float *pressure, float *humidity, float *uv, float *visable, float *ir){

	bme280_read_pressure_temperature_humidity(pressure,temperature,humidity);

	*uv       = Si1132_readUV();
	*visable  = Si1132_readVisible();
	*ir       = Si1132_readIR();


}
