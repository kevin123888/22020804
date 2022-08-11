#ifndef __LED_H__
#define __LED_H__

#include<linux/of.h>
#include<linux/of_gpio.h>
#include<linux/timer.h>



void led_init(int *gpiono,char *propname);
void led_del(int gpiono);
void led_set_value(int gpiono,int value);
int led_get_value(int gpiono);

#endif