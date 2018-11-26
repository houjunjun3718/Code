/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年11月15日 星期四 13时43分49秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

#include"Data_Delivery_Cloud.h"
#include"Reteiever_Processing.h"
#include"Retriever_collection.h"
#include"Watchdog.h"

#define IP "http://cloud.leadwin-agritech.com/LWI-getVersion.php"
#define DATA_IP "http://cloud.leadwin-agritech.com/LWI-pushData.php" 


int main()
{
    FILE *fd;
    while(init_Data_Delivery_Cloud(IP) < 0) //初始化数据上传
    {
        printf("init_Data_Delivery_Cloud:error");
        sleep(1);
    }
    
    int T = 60;//定时60分钟
    while(initWatchdog(T,SpectrumRetriever) < 0) //初始化看门狗
    {
        printf("initWatchdog:error\n");
    }
    fd = init_Retriever("/dev/ttyO1",9600,8,0,1);  //初始化串口
    if(fd ==NULL)
    {
        //串口打开失败
        printf("init_Data_Delivery_Cloud error\n");
        return -1;
    }
    char data[200];  //接收数据的数组空间
    printf("初始化成功\n");
    while(1)
    {
        if(0 < Read_Retriever(fd,data,200)){
            comply_Data_Delivery_Cloud(DATA_IP,data);
            printf("%s\n",data);
            loadWatchdog(SpectrumRetriever);
            printf("数据发送完成\n");
        }else{
            printf("Read_Retriever:error:\n");
        }
    }
    return 0;
}





