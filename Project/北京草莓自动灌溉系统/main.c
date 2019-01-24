/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年01月19日 星期六 13时45分13秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


#include"Reteiever_Processing.h"
#include"Data_Delivery_Cloud.h"
#include"Watchdog.h"
#include"Retriever_collection.h"
#include"ReuseFunction.h"

//数据的极限值
#define TEMP_UP 40
#define TEMP_LO 5

#define HUMI_UP 40
#define HUMI_LO 10

#define EC_UP 1.5
#define EC_LO 0.1

#define PUP_SN "20180060"
#define RETRIEVER "/dev/ttyUSB0"    //Retriever 的通讯串口

#define IP "http://39.105.67.7/LWI-getVersion.php"
#define DATA_IP "http://39.105.67.7/LWI-pushData.php"

int main(void)
{
    FILE *retriever_fd; //retriever的串口
    int T = 60;
    
    while(init_Data_Delivery_Cloud(IP) < 0)  //初始化数据上传
    {
        printf("当前文件名 = %s,当前函数 = %s,当前行号 =%d:init_Data_Delivery_Cloud:error!!!\n",__FILE__,__FUNCTION__,__LINE__);
        sleep(1);
    }
    while(initWatchdog(T,SpectrumRetriever) < 0) //初始化看门狗
    {
        printf("当前文件名 = %s,当前函数 = %s,当前行号 =%d:initWatchdog:error!!!\n",__FILE__,__FUNCTION__,__LINE__);
        sleep(1);
    }
    retriever_fd = init_Retriever(RETRIEVER,9600,8,0,1);  //初始化retriever的串口
    if(retriever_fd == NULL)
    {
        printf("当前文件名 = %s,当前函数 = %s,当前行号 =%d:init_Retriever:error!!!\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    
    printf("初始化完成!!!\n");


    char data[200] = {0};   //串口接收到的数据
    PUP pup_data;   //解析出来的数据
    double A0 = 0.0;    //A口的温度
    double A1 = 0.0;    //A口的湿度
    double A2 = 0.0;    //A口的EC
    double B0 = 0.0;    //B口的温度
    double B1 = 0.0;    //B口的湿度
    double B2 = 0.0;    //B口的EC
    double C0 = 0.0;    //C口的温度
    double C1 = 0.0;    //C口的湿度
    double C2 = 0.0;    //C口的EC
    double A9 = 0.0;

    char ren[20];    //用于转换数据

    while(1)
    {
        //读取数据
        if(0 > Read_Retriever(retriever_fd,data,200));
        {
            printf("当前文件名 = %s,当前函数 = %s,当前行号 =%d:Read_Retriever:error!!!\n",__FILE__,__FUNCTION__,__LINE__);
            continue;
        }
        //发送数据到云平台
        comply_Data_Delivery_Cloud(DATA_IP,data);
        printf("接收到的数据:%s\n",data);
        //喂狗
        loadWatchdog(SpectrumRetriever);
        //提取数据
        printf("开始拆分数据!!!\n");
        if(0 > Reteiever_Processing(data,&pup_data))
        {
            printf("当前文件名 = %s,当前函数 = %s,当前行号 =%d:Reteiever_Processing:error!!!\n",__FILE__,__FUNCTION__,__LINE__);
            continue;
        }
        printf("数据处理完成!!!\n");
        //处理数据,判断Pup是不是草莓中插的Pup
        if(strcmp(pup_data.pup_id,PUP_SN) == 0)
        {
            //A口的温度
            memset(ren,0,20);
            TemperatureConversionFunction(pup_data.pup_A0,ren);      
            A0 = atof(ren);
            //B口的温度
            memset(ren,0,20);
            TemperatureConversionFunction(pup_data.pup_B0,ren);
            B0 = atof(ren);
            //A口的湿度
            A1 = atof(pup_data.pup_A1);
            //B口的湿度
            B1 = atof(pup_data.pup_B1);
            //A口的EC
            A2 = atof(pup_data.pup_A2);
            //B口的EC
            B2 = atof(pup_data.pup_B2);
        }
    }
    return 0;
}


