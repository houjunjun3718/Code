#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define MAXLINE 1024
int post(char *ip,int port,char *page,char *msg){
    int sockfd,n;
    char recvline[MAXLINE];
    struct sockaddr_in servaddr;
    char content[4096];
    char content_page[50];
    sprintf(content_page,"POST /%s HTTP/1.1\r\n",page);
    char content_host[50];
    sprintf(content_host,"HOST: %s:%d\r\n",ip,port);
    char content_type[] = "Content-Type: application/x-www-form-urlencoded\r\n";
    char content_len[50];
    sprintf(content_len,"Content-Length: %d\r\n\r\n",strlen(msg));
    sprintf(content,"%s%s%s%s%s",content_page,content_host,content_type,content_len,msg);
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        printf("socket error\n");
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);
    if(inet_pton(AF_INET,ip,&servaddr.sin_addr) <= 0)
        printf("inet_pton error\n");
    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
        printf("connect error\n");
    write(sockfd,content,strlen(content));
    while((n = read(sockfd,recvline,MAXLINE)) > 0)
    {
        recvline[n] = 0;
        if(fputs(recvline,stdout) == EOF)
            printf("fputs error\n");
    }
    if(n < 0)
        printf("read error\n");
}

int main()
{
    char msg[] = "data=RS;2018-09-07 17:10;10180016;100;20180034;40;100;4;9;A;0;3;a;75.9;A;1;37;a;4.9;A;2;38;a;0.01;B;0;3;a;80.9;B;1;37;a;0.0;B;2;38;a;0.01;C;0;3;a;81.5;C;1;37;a;0.0;C;2;38;a;0.01;2633";
    char ip[] = "101.201.80.90";
    int port = 80;
    char page[] = "LWI-pushData.php";
    post(ip,80,page,msg);
    exit(0);
}
