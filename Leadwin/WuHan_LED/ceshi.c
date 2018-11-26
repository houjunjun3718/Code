/*************************************************************************
	> File Name: ceshi.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月23日 星期日 16时50分22秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<strings.h>

int main(void)
{
    int socketfd = socket(AF_INET,SOCK_DGRAM,0);  
    int port = 5005;
    char *ip = "192.168.0.236";

    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    printf("1111111\n");
    int ren;
    char buff_1[1024] = {"!#001%1欢迎光临$$"};
//    char buff_2[1024];
//    memcpy(buff_2,"!#001%1欢迎光临$$",strlen("!#001%1欢迎光临$$"));
//    printf("12121212\n");
    ren = sendto(socketfd,buff_1,strlen(buff_1),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
    printf("ren = %d\n",ren);
//    printf("21212121212\n");
    return 1;
}



