#include"led.h"

/*

myleds{
		compatible = "hqyj,leds";
		core_leds{
			led1 = <&gpioz 5 0>; 
			led2 = <&gpioz 6 0>;  
			led3 = <&gpioz 7 0>;   
		};
		extend_leds{
			led1 = <&gpioe 10 0>;  //10:gpioe10   0默认状态
			led2 = <&gpiof 10 0>;  //10:gpiof10   0默认状态
			led3 = <&gpioe 8 0>;   //8:gpioe8   0默认状态
		};

*/


void led_init(int *gpiono,char *propname)
{
	struct device_node * led_node;
	int ret;
	// 通过设备树获取节点信息
    led_node = of_find_node_by_path("/myleds/extend_leds");
    if (led_node == NULL)
    {
        printk("led_node find failed\n");
        return;
    }

	//获取gpio 号
	*gpiono = of_get_named_gpio(led_node,propname,0);
	if(*gpiono < 0)
	{
		printk("of get named gpio error\n");
		return ;
	}

	printk("gpiono=%d\n",*gpiono);
	//注册gpio号
	ret = gpio_request(*gpiono,NULL);
	if(ret)
	{
		printk("request gipio error\n");
		return ;
	}

	//设置gpio引脚为输出模式
	ret = gpio_direction_output(*gpiono, 0);
	if (ret)
	{
		printk(" gipio direction error\n");
		return ;
	}

}

void led_del(int gpiono)
{
	//释放gpio编号
	gpio_free(gpiono);
}

void led_set_value(int gpiono,int value)
{
	//设置gpio输出value 
	gpio_set_value(gpiono,value);
}

int led_get_value(int gpiono)
{
	return gpio_get_value(gpiono);
}










