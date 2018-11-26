/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月17日 星期一 16时39分17秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>

#include"UDP.h"

#define IP "120.132.114.208"
#define HOST 8000

int main(void)
{
    int socket_fd;
    socket_fd = initUDP(IP,(uint16_t)HOST);
    
    int i = 0;
    char data_1[1024] = {"RS;2018-09-07 17:15;10180016;100;20180034;40;100;4;9;A;0;3;a;75.9;A;1;37;a;4.9;A;2;38;a;0.01;B;0;3;a;80.7;B;1;37;a;0.0;B;2;38;a;0.01;C;0;3;a;81.3;C;1;37;a;0.0;C;2;38;a;0.01;2634"};
    char data_2[1024] = {"RS;2018-09-16 10:20;10180025;100;20180035;40;100;4;9;A;0;3;a;80.9;A;1;37;a;5.2;A;2;38;a;0.01;B;0;3;a;83.6;B;1;37;a;6.9;B;2;38;a;0.03;2633"};
    char data_3[1024] = {"RS;2018-09-16 10:20;10180012;100;20180051;40;100;4;9;A;0;3;a;80.9;A;1;37;a;5.2;A;2;38;a;0.01;B;0;3;a;83.6;B;1;37;a;6.9;B;2;38;a;0.03;2633"};
    char data_4[1024] = {"RS;2018-09-16 10:20;10180012;100;20180063;40;100;4;9;A;0;3;a;80.9;A;1;37;a;5.2;A;2;38;a;0.01;B;0;3;a;83.6;B;1;37;a;6.9;B;2;38;a;0.03;2633"};
    while(1)
    {
//        i++;
//        printf("i = %d\n",i);
//        sprintf(data,"%d",i);
        write_UDP(socket_fd,&data_1,strlen(data_1));
        sleep(1);
        write_UDP(socket_fd,&data_2,strlen(data_2));
        sleep(1);
        write_UDP(socket_fd,&data_3,strlen(data_3));
        sleep(1);
        write_UDP(socket_fd,&data_4,strlen(data_4));
        sleep(1);
        
//        sendto(socket_fd,&data,sizeof(char)*10,0,(struct sockaddr *)&addr,sizeof(addr));
//        printf("%s\n",data);
        printf("--------------\n");
//        if(i >= 9)
//        {
//            i = 0;
//        }
        sleep(60);
    }

    return 0;
}


