/*************************************************************************
	> File Name: MEC10_Sensor.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年05月22日 星期三 11时46分32秒
 ************************************************************************/

#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

#include"MEC10_Sensor.h"
/*添加CRC沉余校验函数的头文件*/
#include"Shared.h"

/*
char MEC10_A[10] = {0x01,0x03,0x00,0x12,0x00,0x03,0xa5,0xce};  //20CM的传感器
char MEC10_B[10] = {0x02,0x03,0x00,0x12,0x00,0x03,0xa5,0xfd};  //40cm的传感器
char MEC10_C[10] = {0x03,0x03,0x00,0x12,0x00,0x03,0xa4,0x2c};  //60CM的传感器
*/

/*发送数据采集命令,一次采集地址
 * 参数1:传感器的地址
 * 参数2:回填的数据
 * 参数3:串口文件描述符
 * 返回值:0,成功
 *        -1,失败*/
int MEC10_Collection(int Address,MEC10 *Sensor,int fd)
{
    char MEC10_A[10] = {0x01,0x03,0x00,0x12,0x00,0x03,0xa5,0xce};  //20CM的传感器
    char MEC10_B[10] = {0x02,0x03,0x00,0x12,0x00,0x03,0xa5,0xfd};  //40cm的传感器
    char MEC10_C[10] = {0x03,0x03,0x00,0x12,0x00,0x03,0xa4,0x2c};  //60CM的传感器
    /*初始化串口*/
    int rc;  //串口监控
    struct timeval tv_timeout;  //时间结构体
    fd_set set;  //监控列表
    FD_ZERO(&set);  //清空监控列表
    FD_SET(fd,&set);  //将串口文件描述符添加到监控列表
    tv_timeout.tv_sec = 1;  //设置等待时间,1秒
    tv_timeout.tv_usec = 0; //设置等待时间,0微妙
    int size = 0;   //读写串口发送和接收的字节数
    char read_data[20] = {0};  //从串口读到的原始数据
    unsigned int crc16 = 0;  //用于计算crc16校验
    int i;
    //判断Address的值,
    switch(Address)
    {
        case 1:
            printf("接收到的数据: ");
            for(i = 0;i < 8;i++)
            {
                printf("%x ",MEC10_A[i]);
            }
            printf("\n");
            size = write(fd,MEC10_A,8);
            break;
        case 2:
            printf("接收到的数据: ");
            for(i = 0;i < 8;i++)
            {
                printf("%x ",MEC10_B[i]);
            }
            printf("\n");
            size = write(fd,MEC10_B,8);
            break;
        case 3:
            printf("接收到的数据: ");
            for(i = 0;i < 8;i++)
            {
                printf("%x ",MEC10_C[i]);
            }
            printf("\n");
            size = write(fd,MEC10_C,8);
            break;
        default:
            printf("输入的地址错误!!!\n");
            return -1;
    }
    if(size != 8)
    {
        printf("发送数据采集命令错误,%d\n",Address);
        return -1;
    }

    size = 0;
//    int i;
    rc = select(fd + 1,&set,NULL,NULL,&tv_timeout);   //监控串口
    if(FD_ISSET(fd,&set))
    {
        /*数据读取成功*/
        size = read(fd,read_data,20);
        /*进行校验,判断数据是否正确*/
        crc16 = calc_crc16(read_data,size);
        if(crc16 != 0)
        {
            printf("校验失败!!!\n");
            return -1;
        }
        printf("接收到的数据: ");
        for(i = 0;i < size;i++)
        {
            printf("%x ",read_data[i]);
        }
        printf("\n");

        /*读取温度数据,判断温度是大于零度还是小于零度,将读到的温度写入结构体的温度区域*/
        if(read_data[5] >= 0x80)
        {
            /*当前温度是负数*/
            Sensor->temp = ((read_data[5] * 256 + read_data[6]) - 0xFFFF - 0x01) / 10;
            Sensor->temp = 9 * Sensor->temp / 5 + 32;   //将摄氏度转成华氏度
        }else
        {
            Sensor->temp = (read_data[5] *256 + read_data[6]) / 10.0;
            Sensor->temp = 9 * Sensor->temp / 5 + 32;   //将摄氏度转成华氏度
        }
        printf("温度A: %lf\n",Sensor->temp);
        /*读取湿度数据,*/
        Sensor->vwc = (read_data[3] * 256 +read_data[4]) / 10.0;
        printf("湿度A: %lf\n",Sensor->vwc);

        /*读取EC数据*/
        Sensor->ec = (read_data[7] *256 + read_data[8]) / 1000.0;
        printf("EC A: %lf\n",Sensor->ec);

        return 0;   //成功读取到数据,正常返回

    }else
    {
        /*数据读取失败*/
        printf("接收数据出现了错误!\n");
        perror("错误原因:");
        printf("fd = %d\n",fd);
        printf("rc = %d\n",rc);
        return -1;
    }
}

