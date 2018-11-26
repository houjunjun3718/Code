/*************************************************************************
	> File Name: WuHan_main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月31日 星期五 08时55分26秒
    > 武汉的一期项目的主函数，
    > 功能：
            １．使用一个ｐｕｐ和一个ｒｅｔｒｉｅｖｅｒ，ｐｕｐ接２个ＭＳＥＣ３００传感器
                施肥机与工控板通过４８５串口相连，ｒｅｔｒｉｅｖｅｒ与工控板通过２３２串
                口相连，工控板将接收到的一个ｐｕｐ的数据处理处理出来将其以温度Ａ，湿度Ａ
                ，ＥＣＡ，温度Ｂ，湿度Ｂ，ＥＣＢ的顺序写入ＰＬＣ，ＰＬＣ的地址从Ｄ４００
                开始，Ｄ４００为计数，Ｄ４０１开始是数据的地址．
            ２．
 ************************************************************************/

#include<stdio.h>
#include<error.h>
#include<stdlib.h>
#include<arpa/inet.h>


#include"Client_UDP.h"
#include"Retriever.h"
#include"PLC_serial.h"

#define PLC "/dev/ttyO5"   //PLC的串口地址
#define RETRIEVER "/dev/ttyO1"   //Retriever的串口地址

#define IP "120.132.114.208"
#define HOST 8000

int main(void)
{
    int Retriever_fd;     //Retriever的串口文件描述符
    int PLC_fd;           //PLC的串口文件描述符
    Retriever_pup pup_data[16];//从ｒｅｔｒｉｅｖｅｒ中读到的数据
    int pup = 1;          //pup的个数
    char *buf[10];
    char buff_A_temp[10];
    char buff_B_temp[10];
    char buff_A_EC[10];
    char buff_B_EC[10];
    char buff_A_humidity[10];
    char buff_B_humidity[10];
    char buff_JS[10];
    int ren = 0;
    int sun = 0;
    /*打开串口*/
    Retriever_fd = initRetriever(RETRIEVER,9600,8,0,1);//打开Retriever串口
    while(Retriever_fd < 0)
    {
        perror("Retriever_fd open :");
    }
    PLC_fd = initPLC_serial(PLC,9600,7,2,1);//打开PLC串口
    while(PLC_fd < 0)
    {
        perror("PLC_fd open :");
    }
    int UDP_fd = initUDP(IP,(uint16_t)HOST);  //初始化ＵＤＰ


    
    while(1)
    {
    /*读取ｒｅｔｒｉｅｖｅ的数据*/
        if(0 > Retriever_data_processing_function(Retriever_fd,pup_data,pup,UDP_fd))
        {
            printf("数据出错\n");
            continue;
        }

    /*将数据整理成给ＰＬＣ传输的格式*/
        sun = (sun+1)%100;
//        printf("--------56--------\n");
        sprintf(buff_JS,"%d",sun);
        buf[0] = buff_JS;
//        printf("计数:%s\n",buff_JS);
//        printf("计数:%s\n",buf[0]);
//        printf("--------58--------\n");
        ren = ((atof(pup_data[0].pup_A_temp) - 32.0) * 5.0 / 9.0) * 10;
        sprintf(buff_A_temp,"%d",ren);
        buf[1] = buff_A_temp;
//       printf("温度A:%s\n",buff_A_temp);
//        printf("温度A:%s\n",buf[1]);

        ren = (atof(pup_data[0].pup_A_humidity))*10;
        sprintf(buff_A_humidity,"%d",ren);
        buf[2] = buff_A_humidity;
//        printf("湿度A:%s\n",buff_A_humidity);
//        printf("湿度A:%s\n",buf[2]);

        ren = (atof(pup_data[0].pup_A_EC))*100;
        sprintf(buff_A_EC,"%d",ren);
        buf[3] = buff_A_EC;
//        printf("ＥＣA:%s\n",buff_A_EC);
//        printf("ＥＣA:%s\n",buf[3]);


        ren = ((atof(pup_data[0].pup_B_temp) - 32.0) * 5.0 / 9.0) * 10;
        sprintf(buff_B_temp,"%d",ren);
        buf[4] = buff_B_temp;
//        printf("温度B:%s\n",buff_B_temp);
//        printf("温度B:%s\n",buf[4]);

        ren = (atof(pup_data[0].pup_B_humidity))*10;
        sprintf(buff_B_humidity,"%d",ren);
        buf[5] = buff_B_humidity;
//        printf("湿度B:%s\n",buff_B_humidity);
//        printf("湿度B:%s\n",buf[5]);

        ren = (atof(pup_data[0].pup_B_EC))*100;
        sprintf(buff_B_EC,"%d",ren);
        buf[6] = buff_B_EC;
//        printf("ＥＣB:%s\n",buff_B_EC);
//        printf("ＥＣB:%s\n",buf[6]);


//        printf("--------------------\n");


    /*给ＰＬＣ发送数据*/
        if(0 > write_PLC_serial(PLC_fd,buf,7,400,60))
        {
            printf("数据出错\n");
            continue;
        }

    }
    /*退除处理函数，关闭打开的串口*/

    closeRetriever(Retriever_fd);
    closeRetriever(PLC_fd);

    return 0;
}



