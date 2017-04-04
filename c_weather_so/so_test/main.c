#include <stdio.h>
#include <unistd.h>
#include "libwb.h"


int main(int argc, char **argv)
{
	int status = 0;
	int option = 0;
	int WBVersion = 2;
	char *device = "/dev/i2c-1";

	int s = initWB(device);
	switch(s){
		case PERMISSIONS_ERROR: printf("Can't open '%s'. Check permissions or run me with sudo!\n",device);break;
		case IIC_ERROR: 			printf("Can't find '%s'. Does this computer have IIC?!\n",device); break;
		case SI1132_ERROR: 		printf("Could not init the SI1132!\n",device); break;
		case BME280_ERROR: 		printf("Could not init the BME280!\n",device); break;
	}

	if(s!=0) return s;

	float temperature, pressure, humidity, uv, visable, ir;

	getWBData(&temperature, &pressure, &humidity, &uv, &visable, &ir);

	printf("temperature,pressure,humidity,uv,visable,ir\n");
	printf("%f %f %f %f %f %f \n",temperature,pressure,humidity,uv,visable,ir);
	return 0;
}


