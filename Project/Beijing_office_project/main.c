/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月19日 星期五 14时03分38秒
 ************************************************************************/

#include<stdio.h>


#include"Data_Delivery_Cloud.h"
#include"Retriever_collection.h"


#define IP "http://101.201.80.90/LWI-getVersion.php"
#define DATA_IP "http://101.201.80.90/LWI-pushData.php"

int main(void)
{
    int fd;
    init_Data_Delivery_Cloud(IP);
    fd = init_Retriever("/dev/ttyO1",9600,8,0,1);
    if(-1 == fd)
    {
        printf("USB error\n");
        return -1;
    }
    printf("USB OK\n");
    char data[1024];
    while(1)
    {
//        char data[1024];
        if(0 < Read_Retriever(fd,data,1024))
        {
            comply_Data_Delivery_Cloud(DATA_IP,data);
        }
    }

    exit_Data_Delivery_Cloud();
    Clock_Retriever(fd);
    return 0;
}



