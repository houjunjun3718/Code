/*************************************************************************
	> File Name: LEDDisplayNetworkPortModule.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年12月03日 星期一 10时20分41秒
 ************************************************************************/

#include<stdio.h>


#include"LEDDisplayNetworkPortModule.h"

 
char data_A[300];
char data_B[300];

int sockfd;
struct sockaddr_in server_addr;

/*************************************************************************
 *LED屏的数据刷新线程
 *功能:刷新LED屏上的数据,从全局数组data_A和data_B中读取数据
 *参数:无
 *返回值:无
 * **********************************************************************/
void *RefreshLED(void)   
{
    
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
    memcpy(data_A+115," -----------$$",14);
    
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

}

