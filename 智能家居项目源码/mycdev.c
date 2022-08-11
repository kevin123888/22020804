#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/timer.h>
#include"mycdev.h"


struct cdev *cdev = NULL; //字符设备结构体对象
int major = 0;            //主设备号
int minor = 0;            //次设备号
int count = 1;            
struct class * cls = NULL; //目录结构体对象
struct device * dev = NULL; //设备结构体对象
int led1 = 0;
struct timer_list tm_list;   //定时器结构体
int led2;
int flag = 0;

int mycdev_open (struct inode *inode, struct file *filp)
{
  printk("%s---%s----%d\n",__FILE__,__func__,__LINE__);
  return 0;
}
ssize_t mycdev_read (struct file *filp, char __user *ubuf, size_t size, loff_t *offset)
{
  return size;
}
ssize_t mycdev_write (struct file *filp, const char __user * ubuf, size_t size, loff_t * offset)
{
 // struct info tmp_hum;
  struct msg msg1;
  int ret;
  if(size > sizeof(struct msg))
  {
    size = sizeof(struct msg);
  }
  ret = copy_from_user(&msg1, ubuf, size);
  if (ret)
  {
    printk("copy to user error\n");
    return -EIO;
  }

  my_spi_write(msg1.value,msg1.which);
  return 0;
}
int  mycdev_close (struct inode * inode, struct file *filp)
{
  printk("%s---%s----%d\n",__FILE__,__func__,__LINE__);
  return 0;
}
long mycdev_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
  int data ;
  int ret;
  switch (cmd)
  {
  case LED_ON:
    led_set_value(led1, 1);
    break;
  case LED_OFF:
    led_set_value(led1, 0);
    break;
  case READ_HUM:
    data = i2c_read_hum_tmp(HUM_ADDR);
    ret = copy_to_user((void *)arg, &data, sizeof(data));
    if (ret)
    {
      printk("copy to user error\n");
      return -EIO;
    }
    break;
  case READ_TEMP:
    data = i2c_read_hum_tmp(TMP_ADDR);
    ret = copy_to_user((void *)arg, &data, sizeof(data));
    if (ret)
    {
      printk("copy to user error\n");
      return -EIO;
    }
    break;
    case FAN_ON:
        fan_set(1);
      break;
    case FAN_OFF:
        fan_set(0);
      break;
    case MORTOR_ON:
        motor_set(1);
        break;
    case MORTOR_OFF:
        motor_set(0);
        break;
    case BEE_ON:
        bee_set(1);
        break;
    case BEE_OFF:
        bee_set(0);
        break;
    case LED_TW_ON:
        if(flag == 1) break;
        flag = 1;
        mod_timer(&tm_list, jiffies + HZ);
        break;
    case LED_TW_OFF:
        flag = 0;
        led_set_value(led2,0);
        break;
  }
    return 0;
}


struct file_operations fops = {
  .open = mycdev_open,
  .release = mycdev_close,
  .read = mycdev_read,
  .write = mycdev_write,
  .unlocked_ioctl = mycdev_ioctl,
};

void timer_handle(struct timer_list *t)
{
  int value;
  if (flag == 0)
  {
    return;
  }
  value = led_get_value(led2);
  led_set_value(led2, !value);
  mod_timer(&tm_list, jiffies + HZ);
}


static int __init mycdev_init(void)           //驱动程序的入口   加载函数就执行
{
  int ret;                        //作为判断函数的返回值使用
  dev_t devno;                   //设备号

  devno = MKDEV(major,minor);      //设备号

  led_init(&led1,"led1");    //优化加个返回值判断
  led_init(&led2,"led2");

  fan_init();   


  cdev = cdev_alloc();            //针对的是整个设备驱动文件
  if (cdev == NULL)
  {
    printk("cdev alloc error\n");
  }

  cdev_init(cdev, &fops);

  if (major > 0)
  {
    ret = register_chrdev_region(devno, count, "mycdev");
    if (ret)
    {
      printk("regiset chrdev region failed\n");
      return ret;
    }
  }
  else
  {
    ret = alloc_chrdev_region(&devno, minor, count, "mycdev");
    if (ret)
    {
      printk("alloc chrdev region failed\n");
      return ret;
    }
    major = MAJOR(devno);
    minor = MINOR(devno);
  }

  ret = cdev_add(cdev,MKDEV(major,minor),count);
  if(ret)
  {
    printk("cdev add failed\n");
    return ret;
  }

  cls = class_create(THIS_MODULE,"Intelligence");
  if(IS_ERR(cls))
  {
    printk("class create error\n");
    return PTR_ERR(cls);
  }

  dev = device_create(cls,NULL,MKDEV(major,minor),NULL,"mycdev");
  if(IS_ERR(dev))
  {
    printk("device create error\n");
    return PTR_ERR(dev);
  }

  //i2c注册

  si7006_init();

  //spi注册
  my_spi_init();



	tm_list.expires = jiffies + HZ;
	timer_setup(&tm_list,&timer_handle,0);

	add_timer(&tm_list);

	




  return 0;
}

static void __exit mycdev_exit(void)           //驱动程序的出口   卸载执行
{
  del_timer(&tm_list);
  my_spi_del();
  si7006_del();
  
  device_destroy(cls,MKDEV(major,minor));
  class_destroy(cls);

  cdev_del(cdev);
  unregister_chrdev_region(MKDEV(major,minor),count);
  kfree(cdev);

  led_del(led1);
  led_del(led2);
  fan_del();
}




module_init(mycdev_init);           //入口
module_exit(mycdev_exit);           //出口
MODULE_LICENSE("GPL");              //许可证