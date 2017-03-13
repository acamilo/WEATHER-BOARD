#include <stdio.h>
#include "bme280-i2c.h"
#include "si1132.h"
#include "si702x.h"
#include "bmp180.h"
#include "time.h"

const char version[] = "v1.5";

static int pressure;
static int temperature;
static int humidity;
static float altitude;

float SEALEVELPRESSURE_HPA = 1024.25;

int main(int argc, char **argv)
{
	int status = 0;
	int WBVersion = 2;
	char *device = "/dev/i2c-1";

	if (argc == 2) {
		device = argv[1];
	} else if (argc > 2) {
		printf("Usage :\n");
		printf("sudo ./weather_board [i2c node](default \"/dev/i2c-1\")\n");
		return -1;
	}

	if (si1132_begin(device)==-1) return -1;
	if (bme280_begin(device) < 0) {
		if(si702x_begin(device)==-1) return -1;
		if(bmp180_begin(device)==-1) return -1;
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
