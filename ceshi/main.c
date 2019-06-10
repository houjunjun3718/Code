/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年05月23日 星期四 17时08分17秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/types.h>

#include<unistd.h>

#include"Shared.h"



#define TTY "/dev/ttyO3"

char read_data[20] = {0};   //接收数据
int fd = 0;  //串口文件描述符

//数据接收线程
void *read_pthread(void)
{
    int size = 0;
    int i = 0;
//    while(1)
//    {
        size = read(fd,read_data,1024);
        for(i = 0;i < size;i++)
        {
            printf("%x ",read_data[i]);
        }
        printf("\n");

//    }
}



int main(void)
{
//    char write_data[8] = {0x01,0x04,0x00,0x00,0x00,0x03,0xb0,0x0b}; //数据请求
    char write_data[8] = {0x01,0x03,0x00,0x12,0x00,0x03,0xa5,0xce};

//    int fd = 0;  //串口文件描述符
    
    fd = InitSerialPort(TTY,9600,8,0,1);
    if(fd <= 0)
    {
        printf("初始化失败");
        return-1;
    }

    //开启数据接收线程
//    pthread_t tid;   //接收线程
//    pthread_create(&tid,NULL,(void *)read_pthread,NULL);
//    printf("打开数据接收线程\n");
    int i = 0;
    int rc;
    struct timeval tv_timeout;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd,&set);
    tv_timeout.tv_sec = 1;
    tv_timeout.tv_usec = 0;
    int size = 0;
    int j = 0;

    //发送数据
    while(1)
    {
        
//        pthread_create(&tid,NULL,(void *)read_pthread,NULL);
//        printf("打开数据接收线程\n");
        write(fd,write_data,8);
        printf("发送的数据: ");
        
        for(i = 0;i < 8;i++)
        {
            printf("%x ",write_data[i]);
        }
        printf("\n");
        
        rc = select(fd + 1,&set,NULL,NULL,&tv_timeout);
        printf("RC = %d\n",rc);
        if(FD_ISSET(fd,&set))
        {
            
            size = read(fd,read_data,1024);
            for(i = 0;i < size;i++)
            {
                printf("%x ",read_data[i]);
            }
            printf("\n");
            size = 0;
        }else{
            printf("接收出现错误!!!\n");
            perror("错误原因:");
            return -1;
        }
        
        FD_ZERO(&set);
        FD_SET(fd,&set);
        tv_timeout.tv_sec = 1;
        tv_timeout.tv_usec = 0;
        sleep(5);
        printf("j = %d\n",j++);
    }
}
