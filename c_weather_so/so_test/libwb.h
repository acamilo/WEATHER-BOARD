#define OK						0
#define ARGUMENT_ERROR 		1
#define PERMISSIONS_ERROR	2
#define IIC_ERROR				3
#define SENSOR_ERROR			4
#define SI1132_ERROR			5
#define BME280_ERROR			6

extern int initWB(char * iic);

extern void getWBData(float *temperature, float *pressure, float *humidity, float *uv, float *visable, float *ir);
