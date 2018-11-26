/*************************************************************************
	> File Name: Client_UDP.h
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月19日 星期三 11时36分24秒
 ************************************************************************/

#ifndef _CLIENT_UDP_H
#define _CLIENT_UDP_H
#include<arpa/inet.h>

/*
#define IP "120.132.114.208"
#define HOST "8000"
*/
/***************************************************
 *初始化ＵＤＰ
 *
 *
 * ************************************************/
int initUDP(char *ip,uint16_t hostshort);

/***************************************************
 *发送数据
 *
 *
 * ************************************************/
int write_UDP(int UDP_fd,char *data,int datalen);


#endif