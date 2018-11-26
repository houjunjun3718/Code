/*************************************************************************
	> File Name: Client_UDP.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月19日 星期三 11时36分24秒
 ************************************************************************/

#include<stdio.h>
#include<arpa/inet.h>
#include"Client_UDP.h"

struct sockaddr_in addr;
int initUDP(char *ip,uint16_t hostshort)
{
    int sock_fd;
    sock_fd = socket(AF_INET,SOCK_DGRAM,0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(hostshort);
    addr.sin_addr.s_addr = inet_addr(ip);
    return sock_fd;
}

int write_UDP(int UDP_fd,char *data,int datalen)
{
    int a;
    a = sendto(UDP_fd,data,datalen,0,(struct sockaddr *)&addr,sizeof(addr));
    printf("a = %d\n",a);
    return 0;
}


