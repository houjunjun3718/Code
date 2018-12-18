/*************************************************************************
	> File Name: LEDDisplayNetworkPortModule.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年12月03日 星期一 10时20分41秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#include"LEDDisplayNetworkPortModule.h"

 
static char data_A[300];    //资源圃东区
static char data_B[300];    //资源圃西区

int sockfd;
struct sockaddr_in server_addr;

static pthread_mutex_t mutex;    //互斥锁

/*************************************************************************
 *LED屏的数据刷新线程
 *功能:刷新LED屏上的数据,从全局数组data_A和data_B中读取数据
 *参数:无
 *返回值:无
 * **********************************************************************/
void *RefreshLED(void)   
{
    while(1)
    {
        pthread_mutex_lock(&mutex);    //加锁
        sendto(sockfd,&data_A,strlen(data_A),0,(struct sockaddr*)&server_addr,sizeof(server_addr));  //发送东区的数据
        pthread_mutex_unlock(&mutex);  //解锁
        printf("%s\n",data_A);
        sleep(15);

        pthread_mutex_lock(&mutex);    //加锁
        sendto(sockfd,&data_B,strlen(data_B),0,(struct sockaddr*)&server_addr,sizeof(server_addr));  //发送西区的数据
        pthread_mutex_unlock(&mutex);  //解锁
        printf("%s\n",data_B);
        sleep(15);
    }
}


/*************************************************************************
 *初始化LED屏的通信
 *功能:创建一个UDP的套接字,创建一个线程向LED屏发送数据
 *参数0:LED屏的IP地址
 *参数1:LED屏的端口
 *返回值:成功返回0
 *       失败返回-1
 * **********************************************************************/
int InitLEDModule(char *ip,int port)
{
    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    
    /**********东区*************/
    memcpy(data_A+0,"!#001%1",7);
    /**********第一行***********/
    data_A[7] = 0xD7;
    data_A[8] = 0xCA;
    data_A[9] = 0xD4;
    data_A[10] = 0xB4;
    data_A[11] = 0xC6;
    data_A[12] = 0xD4;
    data_A[13] = 0xB6;
    data_A[14] = 0xAB;
    data_A[15] = ' ';
    data_A[16] = ' ';
    data_A[17] = 0xC7;
    data_A[18] = 0xB3;
    data_A[19] = 0xCE;
    data_A[20] = 0xBB;
    data_A[21] = ' ';
    data_A[22] = ' ';
    data_A[23] = 0xC9;
    data_A[24] = 0xEE;
    data_A[25] = 0xCE;
    data_A[26] = 0xBB;
    /**********第二行***********/
    memcpy(data_A+27,"EC(mS/cm) ----  ----",20);
    /**********第三行***********/
    data_A[47] = 0xCE;
    data_A[48] = 0xC2;
    data_A[49] = 0xB6;
    data_A[50] = 0xC8;
    data_A[51] = '(';
    data_A[52] = 0xA1;
    data_A[53] = 0xE6;
    data_A[54] = ')';
    data_A[55] = ' ';
    data_A[56] = ' ';
    memcpy(data_A+57,"----  ----",10);
    /**********第四行***********/
    data_A[67] = 0xCA;
    data_A[68] = 0xAA;
    data_A[69] = 0xB6;
    data_A[70] = 0xC8;
    memcpy(data_A+71,"(%)   ----  ----",16);
    /**********第五行***********/
    memcpy(data_A+87,"CEC(mol/kg)     8.73",20);
    /**********第六行***********/
    data_A[107] = 0xB2;
    data_A[108] = 0xC9;
    data_A[109] = 0xBC;
    data_A[110] = 0xAF;
    data_A[111] = 0xCA;
    data_A[112] = 0xB1;
    data_A[113] = 0xBC;
    data_A[114] = 0xE4;
    memcpy(data_A+115," -----------$$",14);  //时间不显示年,只显示月,日,时间


    /**********西区*************/
    memcpy(data_B+0,"!#001%1",7);
    /**********第一行***********/
    data_B[7] = 0xD7;
    data_B[8] = 0xCA;
    data_B[9] = 0xD4;
    data_B[10] = 0xB4;
    data_B[11] = 0xC6;
    data_B[12] = 0xD4;
    data_B[13] = 0xCE;
    data_B[14] = 0xF7;
    data_B[15] = ' ';
    data_B[16] = ' ';
    data_B[17] = 0xC7;
    data_B[18] = 0xB3;
    data_B[19] = 0xCE;
    data_B[20] = 0xBB;
    data_B[21] = ' ';
    data_B[22] = ' ';
    data_B[23] = 0xC9;
    data_B[24] = 0xEE;
    data_B[25] = 0xCE;
    data_B[26] = 0xBB;
    /**********第二行***********/
    memcpy(data_B+27,"EC(mS/cm) ----  ----",20);
    /**********第三行***********/
    data_B[47] = 0xCE;
    data_B[48] = 0xC2;
    data_B[49] = 0xB6;
    data_B[50] = 0xC8;
    data_B[51] = '(';
    data_B[52] = 0xA1;
    data_B[53] = 0xE6;
    data_B[54] = ')';
    data_B[55] = ' ';
    data_B[56] = ' ';
    memcpy(data_B+57,"----  ----",10);
    /**********第四行***********/
    data_B[67] = 0xCA;
    data_B[68] = 0xAA;
    data_B[69] = 0xB6;
    data_B[70] = 0xC8;
    memcpy(data_B+71,"(%)   ----  ----",16);
    /**********第五行***********/
    memcpy(data_B+87,"CEC(mol/kg)     8.55",20);
    /**********第六行***********/
    data_B[107] = 0xB2;
    data_B[108] = 0xC9;
    data_B[109] = 0xBC;
    data_B[110] = 0xAF;
    data_B[111] = 0xCA;
    data_B[112] = 0xB1;
    data_B[113] = 0xBC;
    data_B[114] = 0xE4;
    memcpy(data_B+115," -----------$$",14);  //时间不显示年,只显示月,日,时间
    

    /**********创建自旋锁***********/
    if(pthread_mutex_init(&mutex,NULL) < 0) //初始化互斥锁
    {
        printf("互斥锁创建失败\n");
        return -1;
    }
    printf("初始化锁成功\n");

    /**********创建数据发送线程*****/
    pthread_t LEDPthread_fd;
    pthread_create(&LEDPthread_fd,NULL,(void *)RefreshLED,NULL);
    pthread_detach(LEDPthread_fd);
    printf("LED屏线程创建成功\n");
    
    return 0;
}

/*************************************************************************
 *向LED发送数据
 *功能:向LED显示屏发送数据
 *参数0:待发送的数据(用一个二维数组)
 *参数1:数据的个数(温度,湿度,EC,时间,pup编号)
 *返回值:成功返回0
 *       失败返回-1
 * **********************************************************************/
int SendLEDData(char **data,int datalen)
{
    if(strcmp(data[0],"20180063") == 0)
    {
        pthread_mutex_lock(&mutex);   //加锁
        //西区
        memcpy(data_B+116,data[1],strlen(data[1]));  //拷贝时间

        memcpy(data_B+57,data[2],4);   //拷贝浅位温度
        if(strlen(data[2]) == 3)
            data_B[60] = '0';
        memcpy(data_B+63,data[5],4);   //拷贝深位温度
        if(strlen(data[5]) == 3)
            data_B[66] = '0';
        memcpy(data_B+77,data[3],4);   //拷贝浅位湿度
        if(strlen(data[3]) == 3)
            data_B[80] = '0';
        memcpy(data_B+83,data[6],4);   //拷贝深位湿度
        if(strlen(data[6]) == 3)
            data_B[86] = '0';
        memcpy(data_B+37,data[4],4);   //拷贝浅位EC
        if(strlen(data[4]) == 3)
            data_B[40] = '0';
        memcpy(data_B+43,data[7],4);   //拷贝深位EC
        if(strlen(data[7]) == 3)
            data_B[46] = '0';

        pthread_mutex_unlock(&mutex);   //解锁
    }else
    {
        pthread_mutex_lock(&mutex);   //加锁
        //东区
        memcpy(data_A+116,data[1],strlen(data[1]));  //拷贝时间
        memcpy(data_A+57,data[2],4);   //拷贝浅位温度 
        if(strlen(data[2]) == 3)
            data_A[60] = '0';
        memcpy(data_A+63,data[5],4);   //拷贝深位温度
        if(strlen(data[5]) == 3)
            data_A[66] = '0';
        memcpy(data_A+77,data[3],4);   //拷贝浅位湿度
        if(strlen(data[3]) == 3)
            data_A[80] = '0';
        memcpy(data_A+83,data[6],4);   //拷贝深位湿度
        if(strlen(data[6]) == 3)
            data_A[86] = '0';
        memcpy(data_A+37,data[4],4);   //拷贝浅位EC
        if(strlen(data[4]) == 3)
            data_A[40] = '0';
        memcpy(data_A+43,data[7],4);   //拷贝深位EC
        if(strlen(data[7]) == 3)
            data_A[46] = '0';
        pthread_mutex_unlock(&mutex);   //解锁
    }
    return 0;
}

