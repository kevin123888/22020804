#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include <semaphore.h>




struct data
{
    char fst;
    char sec;
    char thrd;
    char foth;
}data;

struct msg
{
    unsigned short value;
    int which;
};

struct limit{
    int max_tmp;
    int min_tmp;
    int max_hum;
    int min_hum;
    int set;
};

#define LED_ON _IO('L',1)
#define LED_OFF _IO('L',0)

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

void ueser_ui(void);
void led_ctl(int fd);
void read_hum_tmp(int fd, double *rtmp, double *rhum);
void spi_handler(int fd);
void fan_ctl(int fd);
void bee_ctl(int fd);
void motor_ctl(int fd);
void hum_tmp_ctl(int fd,struct limit *p);

struct limit limit_value;
int flag = 0;
unsigned short num = 0;

void *info_handle(void *arg)
{
    int fd = *(int *)arg;
    double rtmp,rhum;
    while (1)
    {
        read_hum_tmp(fd,&rtmp,&rhum);

        if (limit_value.set == 1)
        {

            if (limit_value.max_tmp < rtmp || limit_value.max_hum < rhum)
            {
                //报警与蜂鸣器风扇、led灯工作---降温
               // ioctl(fd, BEE_ON);
                  ioctl(fd, FAN_ON);
                //闪烁led灯
                ioctl(fd,LED_TW_ON);
            }
            else if (limit_value.min_tmp > rtmp || limit_value.min_hum > rhum)
            {
                ioctl(fd, MORTOR_ON);
                ioctl(fd,LED_TW_ON);
            }
            else
            {
                ioctl(fd, BEE_OFF);
                ioctl(fd, FAN_OFF);
                ioctl(fd, MORTOR_OFF);
                ioctl(fd,LED_TW_OFF);
            }
        }
        else
        {
            ioctl(fd, BEE_OFF);
            ioctl(fd, FAN_OFF);
            ioctl(fd, MORTOR_OFF);
            ioctl(fd,LED_TW_OFF);
        }

        if (flag == 0) //显示温湿度的
        {
            data.fst = ((short)rtmp) / 10;
            data.sec = ((short)rtmp) % 10;
            data.thrd = ((short)rhum) / 10;
            data.foth = ((short)rhum) % 10;

        }
        else if (flag == 1) //只显示温度
        {
            data.fst = ((short)rtmp) / 10;
            data.sec = ((short)rtmp) % 10;
            data.thrd = (short)(rtmp*100 - (short)rtmp *100) / 10 ;
            data.foth = ((short)((rtmp - (short)rtmp)*100)) % 10;
            sleep(1);
        }
        else if (flag == 2) //只显示湿度
        {
            data.fst = ((short)rhum) / 10;
            data.sec = ((short)rhum) % 10;
            data.thrd = ((short)((rhum - (short)rhum) * 100)) / 10;
            data.foth = ((short)((rhum - (short)rhum) * 100)) % 10;
            sleep(1);
        }
        else if(flag ==3) //显示自定义数字
        {
            data.fst = num /1000;
            data.sec = num %1000 /100;
            data.thrd = num %1000 %100 /10;
            data.foth = num %1000 %100 %10;
        }
    }
}

void *callback(void * arg)
{
    pthread_detach(pthread_self());
    int fd = *(int *)arg;
    double rtmp,rhum;
    int i = 0;
    struct msg msg1;
    while (1)
    {
        if(flag == 0)                             //温湿度
        {
            msg1.value = data.fst;
            msg1.which = 0x1;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.sec;
            msg1.which = 0x2;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.thrd;
            msg1.which = 0x4;
            write(fd, &msg1, sizeof(msg1));
             usleep(5000);

            msg1.value = data.foth;
            msg1.which = 0x8;
            write(fd, &msg1, sizeof(msg1));
             usleep(5000);
        }
        else if(flag == 1)
        {
            msg1.value = data.fst;
            msg1.which = 0x1;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.sec + 16;         //16是后面的小数点
            msg1.which = 0x2;
            write(fd, &msg1, sizeof(msg1));
           usleep(5000);

            msg1.value = data.thrd;
            msg1.which = 0x4;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.foth;
            msg1.which = 0x8;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);
        }
        else if(flag == 2)
        {
            msg1.value = data.fst;
            msg1.which = 0x1;
            write(fd, &msg1, sizeof(msg1));
           usleep(5000);

            msg1.value = data.sec + 16;
            msg1.which = 0x2;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.thrd;
            msg1.which = 0x4;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.foth;
            msg1.which = 0x8;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);
        }
        else if(flag == 3)
        {
            msg1.value = data.fst;
            msg1.which = 0x1;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.sec;
            msg1.which = 0x2;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.thrd;
            msg1.which = 0x4;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);

            msg1.value = data.foth;
            msg1.which = 0x8;
            write(fd, &msg1, sizeof(msg1));
            usleep(5000);
        }

    }

    pthread_exit(NULL);
}

int main(int argc,const char * argv[])
{
    int choice;
    int fd = open("/dev/mycdev",O_RDWR);
    if(fd < 0)
    {
        perror("open");
        return -1;
    }

    double rtmp,rhum;
    read_hum_tmp(fd, &rtmp, &rhum);
    data.fst = ((short)rtmp) / 10;               //这个地方为啥用short
    data.sec = ((short)rtmp) % 10;
    data.thrd = ((short)rhum) / 10;
    data.foth = ((short)rhum) % 10;

    pthread_t tid1,tid2;

    pthread_create(&tid1,NULL,callback,&fd);
    pthread_create(&tid2,NULL,info_handle,&fd);
    
   struct msg msg_ret;
    

    while(1)
    {

        ueser_ui();
        printf("请输入你的选择>>");
        scanf("%d", &choice);
        while (getchar() != '\n')
            ;
        switch (choice)
        {
        case 1: // led开关
            led_ctl(fd);
            break;
        case 2: //读取温湿度
            read_hum_tmp(fd,&rtmp,&rhum);
            printf("tmp = %.2lf , hum = %.2lf\n", rtmp, rhum);
            break;
        case 3: //蜂鸣器开关
           bee_ctl(fd);
            break;
        case 4: //风扇开关
            fan_ctl(fd);
            break;
        case 5: //马达开关
             motor_ctl(fd);
            break;
        case 6: //数码管显示
            spi_handler(fd);
            break;
        case 7: //温度阈值设置
            hum_tmp_ctl(fd,&limit_value);
            break;
        case 8: //退出
            msg_ret.which =0x0;
            msg_ret.value = 0;
            for(int i = 0 ; i < 8 ; i ++)
            {
                write(fd,&msg_ret,sizeof(msg_ret));
            }
            close(fd);
            return 0;
        }

 
    }

    

    return 0;
}

void ueser_ui(void)
{
    printf("****************************************************\n");
    printf("******************智能家居系统**********************\n");
    printf("******************1. led开关************************\n");
    printf("******************2. 读取温湿度*********************\n");
    printf("******************3. 蜂鸣器开关*********************\n");
    printf("******************4. 风扇开关***********************\n");
    printf("******************5. 马达开关***********************\n");
    printf("******************6. 数码管显示*********************\n");
    printf("******************7. 温湿度阈值设置*****************\n");
    printf("******************8. 退出系统***********************\n");
    printf("****************************************************\n");
}
void led_ctl(int fd)
{
    int choice;
    while (1)
    {

        printf("****************************************************\n");
        printf("******************LED控制***************************\n");
        printf("******************1.开******************************\n");
        printf("******************2.关******************************\n");
        printf("******************3. 返回上一级*********************\n");
        printf("****************************************************\n");
        printf("请输入你的选择>>");
        scanf("%d", &choice);
        while (getchar() != 10)
            ;
        switch (choice)
        {
        case 1:
            ioctl(fd,LED_ON);
            break;
        case 2:
            ioctl(fd,LED_OFF);
            break;
        case 3:
            return;
        default:
            printf("输入有误，请重新输入\n");
            break;
        }
    }
}
//读取温湿度
void read_hum_tmp(int fd, double *rtmp, double *rhum)
{
    int tmp,hum;
    ioctl(fd, READ_TEMP, &tmp);
    ioctl(fd, READ_HUM, &hum);
    *rtmp = 175.72 * tmp / 65536 - 46.85;
    *rhum = 125 * hum / 65536 - 6;
  //  printf("读取到的值为:rtmp = %lf,rhum= %lf\n",*rtmp,*rhum);

}



void spi_handler(int fd)
{
    int choice;
    double rtmp, rhum;
    while (1)
    {

        printf("****************************************************\n");
        printf("******************数码管显示************************\n");
        printf("******************1. 显示温度***********************\n");
        printf("******************2. 显示湿度***********************\n");
        printf("******************3. 显示温湿度*********************\n");
        printf("******************4. 显示自定义数字*****************\n");
        printf("******************5. 返回上一级*********************\n");
        printf("****************************************************\n");
        printf("请输入你的选择>>");
        scanf("%d", &choice);
        while (getchar() != 10)
            ;
        switch (choice)
        {
        case 1:
            flag = 1;
            break;
        case 2:
            flag = 2;
            break;
        case 3:
            flag = 0;
            break;
        case 4:
            flag = 3;
            printf("请输入需要显示的数字(0~9999)>>");
            scanf("%d", &num);
            while (getchar() != 10)
                ;
            break;
        case 5:
            flag = 0;
          //  sem_post(&sem);
            return;
        default:
            printf("输入有误,请重新输入\n");
        }
    }
}

void fan_ctl(int fd)
{
    int choice;
    while (1)
    {

        printf("****************************************************\n");
        printf("******************风扇控制***************************\n");
        printf("******************1.开******************************\n");
        printf("******************2.关******************************\n");
        printf("******************3. 返回上一级*********************\n");
        printf("****************************************************\n");
        printf("请输入你的选择>>");
        scanf("%d", &choice);
        while (getchar() != 10)
            ;
        switch (choice)
        {
        case 1:
            ioctl(fd,FAN_ON);
            break;
        case 2:
            ioctl(fd,FAN_OFF);
            break;
        case 3:
            return;
        default:
            printf("输入有误，请重新输入\n");
            break;
        }
    }
}

void bee_ctl(int fd)
{
    int choice;
    while (1)
    {

        printf("****************************************************\n");
        printf("******************蜂鸣器控制***************************\n");
        printf("******************1.开******************************\n");
        printf("******************2.关******************************\n");
        printf("******************3. 返回上一级*********************\n");
        printf("****************************************************\n");
        printf("请输入你的选择>>");
        scanf("%d", &choice);
        while (getchar() != 10)
            ;
        switch (choice)
        {
        case 1:
            ioctl(fd,BEE_ON);
            break;
        case 2:
            ioctl(fd,BEE_OFF);
            break;
        case 3:
            return;
        default:
            printf("输入有误，请重新输入\n");
            break;
        }
    }
}

void motor_ctl(int fd)
{
    int choice;
    while (1)
    {

        printf("****************************************************\n");
        printf("******************马达控制***************************\n");
        printf("******************1.开******************************\n");
        printf("******************2.关******************************\n");
        printf("******************3. 返回上一级*********************\n");
        printf("****************************************************\n");
        printf("请输入你的选择>>");
        scanf("%d", &choice);
        while (getchar() != 10)
            ;
        switch (choice)
        {
        case 1:
            ioctl(fd,MORTOR_ON);
            break;
        case 2:
            ioctl(fd,MORTOR_OFF);
            break;
        case 3:
            return;
        default:
            printf("输入有误，请重新输入\n");
            break;
        }
    }
}

void hum_tmp_ctl(int fd,struct limit *p)
{
    limit_value.set = 0;
    printf("请输入温度的上限值>>");
    scanf("%d", &p->max_tmp);
    while (getchar() != 10)
        ;

    printf("请输入温度的下限值>>");
    scanf("%d", &p->min_tmp);
    while (getchar() != 10)
        ;
    printf("请输入湿度上限值>>");
    scanf("%d", &p->max_hum);
    while (getchar() != 10)
        ;

    printf("请输入湿度下限值>>");
    scanf("%d", &p->min_hum);
    while (getchar() != 10)
        ;
    limit_value.set = 1;
}
