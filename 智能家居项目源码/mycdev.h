#ifndef __MYCDEV_H__
#define __MYCDEV_H__

extern void led_init(int *gpiono,char *propname);
extern void led_del(int gpiono);
extern void led_set_value(int gpiono,int value);
extern int led_get_value(int gpiono);


extern int i2c_read_hum_tmp(char reg);
extern void si7006_init(void);
extern void si7006_del(void);


extern int my_spi_write(unsigned int value,int which);
extern void my_spi_del(void);
extern void my_spi_init(void);



extern void fan_init(void);
extern void fan_del(void);
extern void fan_set(int status);
extern void bee_set(int status);

extern void motor_set(int status);

#define LED_ON _IO('L',1)
#define LED_OFF _IO('L',0)
#define TMP_ADDR 0xE3
#define HUM_ADDR 0xE5
#define READ_TEMP _IOR('S',1,int)
#define READ_HUM  _IOR('S',0,int)



#define FAN_ON  _IO('F',1)
#define FAN_OFF _IO('F',0)

#define BEE_ON  _IO('B',1)
#define BEE_OFF _IO('B',0)

#define MORTOR_ON  _IO('D',1)
#define MORTOR_OFF _IO('D',0)

#define LED_TW_ON _IO('T',1)
#define LED_TW_OFF _IO('T',0)

struct info
{
    unsigned short hum;
    unsigned short tmp;
};

struct msg
{
    unsigned short value;
    int which;
};




#endif 


