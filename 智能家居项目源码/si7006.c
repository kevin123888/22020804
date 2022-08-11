#include"si7006.h"

/*

    &i2c1{
    pinctrl-names = "default";
    pinctrl-0 = <&i2c1_pins_b>;
    i2c-scl-rising-time-ns = <100>;
    i2c-scl-falling-time-ns = <7>;
    status = "okay";
    /delete-property/dmas;      //删除属性
    /delete-property/dma-names;

    si7006@40{
        compatible = "hqyj,si7006";
        reg = <0x40>;
    };
};
*/
struct i2c_client *gclient = NULL;
//读取温湿度  
int i2c_read_hum_tmp(char reg)
{
    char buf[] = {reg};
    unsigned short data = 0;
    int ret;
    struct i2c_msg w_msg[2] = {
        [0] = {
            .addr = gclient->addr,
            .flags = 0,
            .len = 1,
            .buf = buf,
        },
        [1] = {
            .addr = gclient->addr,
            .flags = 1,
            .len = 2,
            .buf = (__u8 *)&data,
        }
    };

   ret =  i2c_transfer(gclient->adapter,w_msg,2);
   if(ret != 2)
   {
        printk("transfer failed\n");
        return ret;
   }

   data = data >> 8 | data << 8;
   return data;
}


int si7006_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    printk("%s---%s----%d\n",__FILE__,__func__,__LINE__);
    gclient = client;
    return 0;
}
int si7006_remove(struct i2c_client *client)
{
    printk("%s---%s----%d\n",__FILE__,__func__,__LINE__);
    return 0;
}
const struct of_device_id oftable[]={
    {.compatible = "hqyj,si7006"},
    {},
};
struct i2c_driver si7006 = {
    .probe = si7006_probe,
    .remove = si7006_remove,
    .driver = {
        .name = "hhhh",
        .of_match_table = oftable,
    },
};

void si7006_init(void)
{
    i2c_add_driver(&si7006);
}
void si7006_del(void)
{
    i2c_del_driver(&si7006);
}







