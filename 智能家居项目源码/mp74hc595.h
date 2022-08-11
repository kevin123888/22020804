#ifndef __MP74HC595_H__
#define __MP74HC595_H__
#include<linux/spi/spi.h>
#include<linux/delay.h>

u8 code[] = {
	0x3f, // 0
	0x06, // 1
	0x5b, // 2
	0x4f, // 3
	0x66, // 4
	0x6d, // 5
	0x7d, // 6
	0x07, // 7
	0x7f, // 8
	0x6f, // 9
	0x77, // A
	0x7c, // b
	0x39, // c
	0x5e, // d
	0x79, // e
	0x71, // f
	0xbf, // 0.
	0x86, // 1.
	0xdb, // 2.
	0xcf, // 3.
	0xe6, // 4.
	0xed, // 5.
	0xfd, // 6.
	0x87, // 7.
	0xff, // 8.
	0xef, // 9.

};

int my_spi_write(unsigned int value,int which);

void my_spi_del(void);

void my_spi_init(void);

void  my_spi_value(unsigned short value);

#endif