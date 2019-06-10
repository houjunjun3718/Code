/*************************************************************************
	> File Name: ceshi.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年05月26日 星期日 08时23分49秒
    > 
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

#include"Shared.h"
#include"MEC10_Sensor.h"

#define TTY "/dev/ttyO3"

int main(void)
{
    /*初始化串口*/
    int fd = 0;  //串口文件描述符
    fd = InitSerialPort(TTY,9600,8,0,1);
    if(fd <= 0)
    {
        printf("串口打开失败!!!\n");
        return -1;
    }
    MEC10 A;
    A.temp = 0.0;
    A.vwc = 0.0;
    A.ec = 0.0;
    MEC10 B;
    B.temp = 0.0;
    B.vwc = 0.0;
    B.ec = 0.0;
    MEC10 C;
    C.temp = 0.0;
    C.vwc = 0.0;
    C.ec = 0.0;
    int rc;
    while(1)
    {
#if 1
    /*采集数据*/
        printf("fd = %d\n",fd);
        rc = MEC10_Collection(1,&A,fd);
        if(rc < 0)
        {
            printf("数据解析错误!");
            return -1;
        }
    /*显示数据*/
        printf("温度_1: %2f\n",A.temp);
        printf("湿度_1: %2f\n",A.vwc);
        printf("EC_1: %2f\n",A.ec);

        printf("fd = %d\n",fd);
        sleep(1);
    /*采集数据*/
#endif        
        rc = MEC10_Collection(2,&B,fd);
        if(rc < 0)
        {
            printf("数据解析错误!");
            return -1;
        }
        
    /*显示数据*/
        printf("温度_2: %2f\n",B.temp);
        printf("湿度_2: %2f\n",B.vwc);
        printf("EC_2: %2f\n",B.ec);

        printf("fd = %d\n",fd);
        sleep(1);

    /*采集数据*/
        
        rc = MEC10_Collection(3,&C,fd);
        if(rc < 0)
        {
            printf("数据解析错误!");
            return -1;
        }
        
    /*显示数据*/
        
        printf("温度_3: %2f\n",C.temp);
        printf("湿度_3: %2f\n",C.vwc);
        printf("EC_3: %2f\n",C.ec);

        sleep(30);
    }


    return 0;
}



