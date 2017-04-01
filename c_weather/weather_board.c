#include <stdio.h>
#include "bme280-i2c.h"
#include "si1132.h"
#include "si702x.h"
#include "bmp180.h"
#include "time.h"
#include <unistd.h>
#include <getopt.h>

const char version[] = "v1.5";
#define ARGUMENT_ERROR 		-1
#define PERMISSIONS_ERROR	-2
#define IIC_ERROR				-3
#define SENSOR_ERROR			-4

static int pressure;
static int temperature;
static int humidity;
static float altitude;
int verbose=0;
int csv=0;
int single=0;

float SEALEVELPRESSURE_HPA = 1024.25;

void print_usage() {
    printf("\nweather_board: [-vchs] [-d DEVICE]\n\t-h\tusage\n\t-s\tPrint values and exit. Don't keep printing.\n\t-c\tCSV Output\n\t-v\tVerbose Output\n\t-d\tiic bus to use (/dev/i2c-1) default \n\n");
}


int main(int argc, char **argv)
{
	int status = 0;
	int option = 0;
	int WBVersion = 2;
	char *device = "/dev/i2c-1";


    while ((option = getopt(argc, argv,"hvcd:")) != -1) {
        switch (option) {
             case 'v' : verbose = 1;
                 break;
             case 's' : single = 1;
                 break;
				 case 'c' : csv = 1;
                 break;
             case 'd' : device = optarg; 
                 break;
				 case 'h':
             default: print_usage(); 
                 exit(ARGUMENT_ERROR);
        }
    }
		if(access(device,F_OK)==-1){
			printf("Can't find '%s'. Does this computer have IIC?!\n",device);
			return IIC_ERROR;
		}
		  
      if(access(device,W_OK)){
			printf("Can't open '%s'. Check permissions or run me with sudo!\n",device);
			return PERMISSIONS_ERROR;
        }


	if (si1132_begin(device)==-1) return SENSOR_ERROR;
	if (bme280_begin(device) < 0) {
		if(si702x_begin(device)==-1) return SENSOR_ERROR;
		if(bmp180_begin(device)==-1) return SENSOR_ERROR;
		WBVersion = 1;
	}

	printf("datetime,UV Index,Visable Lux,IR Lux,Temp C,Humidity %%,Pressure hPa,Altitude m\n");
	while (1) {
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		printf("%d-%d-%d %d:%d:%d,\t", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		//printf("\e[H======== si1132 ========\n");
		printf("%.2f,\t", Si1132_readUV()/100.0);
		printf("%.0f,\t", Si1132_readVisible());
		printf("%.0f,\t", Si1132_readIR());
			bme280_read_pressure_temperature_humidity(
						&pressure, &temperature, &humidity);
			//printf("======== bme280 ========\n");
			printf("%.2lf,\t", (double)temperature/100.0);
			printf("%.2lf,\t", (double)humidity/1024.0);
			printf("%.2lf,\t", (double)pressure/100.0);
			printf("%f\n", bme280_readAltitude(pressure,
								SEALEVELPRESSURE_HPA));
		fflush(stdout);
		usleep(1000000);
	}
	return 0;
}
