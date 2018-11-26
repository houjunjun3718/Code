/*************************************************************************
	> File Name: TCP.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月18日 星期二 08时55分07秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<signal.h>


#include"TCP.h"



//一直等待ｔｃｐ连接的线程
void *lesten_TCP(void)
{
    int i = 0;
//    socklen_t client_socketlen;
//    struct sockaddr_in client_socket;
//    sleep(1);
    while(1)
    {
//        printf("ddddddd\n");
        i = i%10;
        if(IP[i] != 0)
        {
            i++;
            continue;
        }
//        printf("aaaaaaa\n");
        if((IP[i]= accept(listenfd,NULL,NULL)) == -1)
        {
            perror("accept:");
            continue;
        }
//        printf("bbbbbbbb\n");
        printf("有ｔｃｐ连接  %d\n",i);
        i++;
    }
}



//初始化ＴＣＰ套接字
int initTCP(int hton)
{
//    int listenfd;   //监听套接字
    struct sockaddr_in sercaddr;
    socklen_t clilen;

    listenfd = socket(AF_INET,SOCK_STREAM,0);   //创建ＴＣＰ套接字
    bzero(&sercaddr,sizeof(sercaddr));  //初始化sercaddr
    sercaddr.sin_family = AF_INET;  //ＩＰＶ４地址族
    sercaddr.sin_addr.s_addr = htonl(INADDR_ANY);  //ip地址，所有ｉｐ
    sercaddr.sin_port = htons((uint16_t)hton); //端口号
    
    //允许重复使用本地地址和端口，用于实现同一端口的收发功能
    int on = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    //绑定端口
    if(bind(listenfd,(struct sockaddr *)&sercaddr,sizeof(sercaddr)) == -1)
    {
        perror("bind:");
        return -1;
    }

    //监听端口套接字
    if(listen(listenfd,20) == -1)
    {
        perror("listen:");
        return -1;
    }
    /*
    for(int i = 0;i < 10;i++)
    {
        IP[i].client_socketlen = sizeof(IP[i].client_socket);   
    }
    */
    pthread_t pthread_id;
    signal(SIGPIPE,SIG_IGN); //屏蔽信号
//    int *listen_fd = &listenfd;
//    printf("ccccccc\n");
    pthread_create(&pthread_id,NULL,(void *)lesten_TCP,(void *)&listenfd);//创建一个线程，一直监听端口的ＴＣＰ连接
    pthread_detach(pthread_id);
    return 1;
}




//向客户端发送传感器信息，一条数据发送完成后会将
void *write_TCP(void)
{
    int a = 0; //TCP发送数据的标志
    while(1)
    {
        if(1 == light_t)  //判断数据是否可读，０标示数据不可读，１标示数据可读
        {
            for(int j = 0;j < 10;j++)
            {
                if(IP[j] == 0) //判断IP[j]是不是套接字
                {
                    continue;
                }
                printf("-----------\n");
                if(send(IP[j],data,strlen(data),0) < 0) //判断IP[j]发送是否成功
                {
                    perror("send");
                    IP[j] = 0;
                    printf("IP[%d]\n",j);
                    continue;
                }
                a = 1;
            }
//            printf("1111111\n");
            if(a == 1)
            {
                a = 0;
                light_t = 0;  //标示数据发送完成
                
            }
//            light_t = 0;  //标示数据发送完成
        }        
    }
}


