#include"mp74hc595.h"

/*
&spi4{
	pinctrl-names = "default";
	pinctrl-0 = <&spi4_pins_a>;
	cs-gpios = <&gpioe 11 0>;
	status = "okay";

	m74hc595@0{
		compatible = "hqyj,myspi";
		reg = <0>;
		spi-max-frequency = <10000000>; //10Mhz
	};
};
*/
struct spi_device * gspi = NULL;
int	 mp74hc595_probe(struct spi_device *spi)
{
    gspi = spi;
    printk("%s---%s----%d\n",__FILE__,__func__,__LINE__);
    return 0;
}

int mp74hc595_remove(struct spi_device *spi)
{
    printk("%s---%s----%d\n", __FILE__, __func__, __LINE__);
    return 0;
}
const struct of_device_id oftable_spi[] = {
    {.compatible = "hqyj,myspi"},
    {},
};
struct spi_driver mp74hc595 = {
    .probe = mp74hc595_probe,
    .remove = mp74hc595_remove,
    .driver = {
        .name = "hi",
        .of_match_table = oftable_spi,
    },
};

int my_spi_write(unsigned int value, int which) 
{
    char buf[2] = "";

    buf[0] = which;
    buf[1] = code[value];

    spi_write(gspi, buf, 2);

    return 0;
}

void  my_spi_value(unsigned short value)
{
    int i = 0;
    char buf[4] = "";
    while (i < 500)   //怎么循环出来
        {
            buf[0] = 0x1;
            buf[1] = code[value / 1000];
            spi_write(gspi, buf, 2);
            mdelay(2);

            buf[0] = 0x2;
            buf[1] = code[value %1000/100];
            spi_write(gspi, buf, 2);
            mdelay(2);

            buf[0] = 0x4;
            buf[1] = code[value %1000 %100 /10];
            spi_write(gspi, buf, 2);
            mdelay(2);

            buf[0] = 0x8;
            buf[1] = code[value %1000 %100 %10];
            spi_write(gspi, buf, 2);
            mdelay(2);
            i++;
        }
}

void my_spi_init(void)
{
    spi_register_driver(&mp74hc595);
}

void my_spi_del(void)
{
    spi_unregister_driver(&mp74hc595);
}



