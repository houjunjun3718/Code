/*************************************************************************
	> File Name: UDP.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月18日 星期二 08时56分06秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include<string.h>
#include<arpa/inet.h>

#include"lise.h"
#include"UDP.h"


struct sockaddr_in cli;
socklen_t len;

extern Queue queue;

/*******************************************************
 *初始化ＵＤＰ通信
 *参数１：ｉｐ地址
 *参数２：端口号
 *返回值：成功返回套接字，失败返回－１
 * ****************************************************/
int initUDP(uint16_t hostshort)
{
    //创建socket
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);

    //创建网络通信
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(hostshort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定套接字
    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(0 > ret)
    {
        perror("UDP bind:");
        return -1;
    }
    bzero(&cli,sizeof(cli));
    len = sizeof(cli);
    return sockfd;
}


/*******************************************************
 *接收客户端发来的数据的函数
 *参数１：ＵＤＰ套接字
 *参数２：要读的数据
 *参数３：要读的字节数
 *返回值：实际读到的字节数
 * ****************************************************/
char * read_UDP(int UDP_fd)
{
    int i = 0;
//    char data[1024];
    static char buf[1024];
    memset(buf,0,sizeof(buf));
//    printf("1212121212\n");

    i = recvfrom(UDP_fd,&buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
//    printf("11111\n");
//    i = strlen(data);
    printf("i = %d\n",i);
 //   printf("data: %s\n",buf);
    return buf;
}

void *read_UDP_pthread(void *UDP_fd)
{
//    printf("......\n");
    int *sock_fd = (int *)UDP_fd;
    char *data;
    while(1)
    {
//        printf("---------\n");
        data = read_UDP(*sock_fd);
//        printf("21212121212\n");
//        printf("%s\n",data);
        
//        printf("读到的数据：%s\n",data);
//        printf("%s\n",data);
        InsertQueue(&queue,data);
//        printf("遍历数据\n");
//        TraverseQueue(&queue);
    }
}

/*******************************************************
 *关闭ＵＤＰ套接字
 *参数１：ＵＤＰ套接字
 *返回值：无返回值
 * ****************************************************/
void close_UDP(int sockfd)
{
    close(sockfd);
}


