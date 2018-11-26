/*************************************************************************
	> File Name: UDP.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月17日 星期一 09时44分16秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

#include"UDP.h"

/*************************************************************************
 * 初始化ＵＤＰ的通信协议
 * 参数１：ｉｐｖ４的地址
 * 参数２：端口号
 * 返回值：成功返回套接字，失败返回－１
 * **********************************************************************/
int initUDP(char *ip,uint16_t hostshort)
{
    //创建ｓｏｃｋｅｔ
    int sock_fd;   //ｕｄｐ的套接字
    sock_fd = socket(AF_INET,SOCK_DGRAM,0);
    //创建通信对象
    addr.sin_family = AF_INET;
    addr.sin_port = htons(hostshort);
    addr.sin_addr.s_addr = inet_addr(ip);
    return sock_fd;
}


/*************************************************************************
 * 通过ＵＤＰ发送数据
 * 参数１：ＵＤＰ的套接字
 * 参数２：要发送的数据
 * 参数３：数据的字节数
 * 返回值：成功返回0，失败返回－１
 * **********************************************************************/
int write_UDP(int UDP_fd,char *data,int datalen)
{
    sendto(UDP_fd,data,datalen,0,(struct sockaddr *)&addr,sizeof(addr));
    return 0;
}


