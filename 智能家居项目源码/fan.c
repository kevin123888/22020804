#include"fan.h"


/*
	myplatform{
		compatible = "hqyj,motor,bee,fan";
		fan = <&gpioe 9 0>;
		motor = <&gpiof 6 0>;
		bee = <&gpiob 6 0>;   
	};
*/


struct device_node *p_node = NULL;
int gpiono_fan;
int gpiono_bee;
int gpiono_motor;
void fan_init(void)
{
    int ret;
    p_node = of_find_node_by_path("/myplatform");
    if(p_node == NULL)
    {
        printk("%s:%s:get node by path failed\n",__FILE__,__func__);
        return;
    }

   gpiono_fan = of_get_named_gpio(p_node,"fan",0);
   if(gpiono_fan < 0)
   {
        printk("get gpio fan error\n");
        return ;
   }

   gpiono_bee = of_get_named_gpio(p_node, "bee", 0);
   if (gpiono_bee < 0)
   {
       printk("get gpio bee error\n");
       return;
   }

   gpiono_motor = of_get_named_gpio(p_node, "motor", 0);
   if (gpiono_motor < 0)
   {
       printk("get gpio motor error\n");
       return;
   }

   ret = gpio_request(gpiono_fan,NULL);
   if(ret)
   {
        printk("requet gpio fan error\n");
        return;
   }

   ret = gpio_request(gpiono_bee, NULL);
   if (ret)
   {
       printk("requet gpio bee error\n");
       return;
   }

   ret = gpio_request(gpiono_motor, NULL);
   if (ret)
   {
       printk("requet gpio motor error\n");
       return;
   }

   gpio_direction_output(gpiono_fan,0);
   gpio_direction_output(gpiono_bee,0);
   gpio_direction_output(gpiono_motor,0);

}

void fan_del(void)
{
    gpio_set_value(gpiono_fan,0);
    gpio_free(gpiono_fan);

    gpio_set_value(gpiono_bee, 0);
    gpio_free(gpiono_bee);

    gpio_set_value(gpiono_motor, 0);
    gpio_free(gpiono_motor);

}

void fan_set(int status)
{
    gpio_set_value(gpiono_fan,status);
}

void bee_set(int status)
{
    gpio_set_value(gpiono_bee,status);
}

void motor_set(int status)
{
    gpio_set_value(gpiono_motor,status);
}