/*************************************************************************
	> File Name: WuHan.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月18日 星期二 08时42分04秒
    > 功能：实现数据的透传功能，从ＵＤＰ接收数据在通过ＴＣＰ发送到客户端
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>


#include"lise.h"
#include"TCP.h"
#include"UDP.h"

Queue queue;



int main(void)
{
//    Queue queue;
    int host_udp = 8000;
    int host_tcp = 8003;
    int UDP_fd;
    int TCP_fd;
    light_t = 0;
   
//    printf("???????\n");
    InitQueue(&queue);//创建队列
//    printf(".......\n");
    while((UDP_fd = initUDP((uint16_t)host_udp)) < 0) //初始化ＵＤＰ
    {
        sleep(1);
    }
//    printf("-------\n");
    while(initTCP(host_tcp) < 0)
    {
        sleep;
    }
    //运行ＵＤＰ接收数据的线程
    int *udp_fd = &UDP_fd;
    pthread_t pthread_UDP;
//    printf("11111\n");
    pthread_create(&pthread_UDP,NULL,(void *)read_UDP_pthread,(void *)udp_fd);
//    sleep(10);
//    printf("22\n");
    pthread_detach(pthread_UDP);
//    printf("33333\n");

//    sleep(10);
    //运行ＴＣＰ发送数据的线程
    pthread_t pthread_TCP;
//    printf("222222\n");
    pthread_create(&pthread_TCP,NULL,(void *)write_TCP,NULL);
    pthread_detach(pthread_TCP);

    char *buff = NULL;
    char jice[10] = {0};
    while(1)
    {
//        printf("light_t = %d\n",light_t);
        if((IsEmptyQueue(&queue) == 1) && (light_t == 0))
        {
            buff = DeleteQueue(&queue);
            memcpy(jice,buff,2);
            jice[2] = '\0';
            if(strcmp("RS",jice) != 0)
            {
                continue;
            }
            memset(data,0,1024);
            memcpy(data,buff,strlen(buff));
            light_t = 1;
            printf("light_t = %d,data = %s\n",light_t,data);
        } 
        sleep(1);
    }
    printf("失败\n");
    return 0;
}



