#ifndef __SI7006_H__
#define __SI7006_H__
#include<linux/i2c.h>
#include<linux/device.h>

//读取温湿度  
int i2c_read_hum_tmp(char reg);

void si7006_init(void);

void si7006_del(void);

#endif