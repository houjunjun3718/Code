/*************************************************************************
	> File Name: post.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月16日 星期二 11时34分03秒
 ************************************************************************/

#include<stdio.h>


#define MAXLINE 1024
int post(char *ip,int port,char *page,char *msg)
{
    int sockfd,n;
    char recvline[MAXLINE];
    struct sockaddr_in servaddr;
    char content[4096];
    char content_page[50];
    char content_host[50];
    char content_type[] = "Content-Type: application/x-www-form-urlencoded\r\n";
    char content_len[50];

    sprintf(content_page,"POST /%s HTTP/1.1\r\n",page);
    
    sprintf(content_host,"HOST %s:%d\r\n",ip,port);
    
    sprintf(content_len,"%s%s%s%s%s",content_page,content_host,content_type,content_len,msg);


    
}
