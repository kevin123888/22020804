#ifndef __FAN_H__
#define __FAN_H__
#include<linux/gpio.h>
#include<linux/of_gpio.h>
#include<linux/of.h>


void fan_init(void);

void fan_del(void);

void fan_set(int status);

void bee_set(int status);

void motor_set(int status);

#endif