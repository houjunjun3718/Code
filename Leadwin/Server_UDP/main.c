/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月20日 星期四 09时32分55秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<arpa/inet.h>


#include"UDP.h"

/*******************************
 *执行ＵＤＰ接收数据的线程
 *参数１：队列头
 * ****************************/
void *read_UDP_pthread(void *UDP_fd)
{
    int *sock_fd = (int *)UDP_fd;
    char *data;
    while(1)
    {
        data = read_UDP(*sock_fd);
        printf("读到的数据：%s\n",data);
    }
}

int main(void)
{
    pthread_t pthread_id;
    int host = 8000;
    int UDP_fd;
    UDP_fd = initUDP((uint16_t)host);
    int *udp_fd = &UDP_fd;
    pthread_create(&pthread_id,NULL,(void *)read_UDP_pthread,(void *)udp_fd);
    pthread_detach(pthread_id);
    while(1);
    
}

