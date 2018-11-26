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
            ２．将数据在ｌｅｄ显示屏上显示出来
 ************************************************************************/

#include<stdio.h>
#include<error.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>


#include"Client_UDP.h"
#include"Retriever.h"
//#include"PLC_serial.h"
#include"LED.h"

//#define PLC "/dev/ttyO5"   //PLC的串口地址
#define RETRIEVER "/dev/ttyO1"   //Retriever的串口地址

#define IP "120.132.114.208" //服务器的ＩＰ和端口号
#define HOST 8000

#define LEDIP "192.168.0.236"
#define LEDHOST 5005

int main(void)
{
    int Retriever_fd;     //Retriever的串口文件描述符
    int PLC_fd;           //PLC的串口文件描述符
    Retriever_pup pup_data[16];//从ｒｅｔｒｉｅｖｅｒ中读到的数据
    int pup = 2;          //pup的个数
    char *buf[10];
    char buff_A_temp[10];
    char buff_B_temp[10];
    char buff_A_EC[10];
    char buff_B_EC[10];
    char buff_A_humidity[10];
    char buff_B_humidity[10];
    char buff_JS[10];
    double ren = 0;
    int sun = 0;

    /*打开串口*/
    Retriever_fd = initRetriever(RETRIEVER,9600,8,0,1);//打开Retriever串口
    while(Retriever_fd < 0)
    {
        perror("Retriever_fd open :");
    }
//    PLC_fd = initPLC_serial(PLC,9600,7,2,1);//打开PLC串口
    while(PLC_fd < 0)
    {
        perror("PLC_fd open :");
    }
    int UDP_fd = initUDP(IP,(uint16_t)HOST);  //初始化ＵＤＰ

    LED_fd = initLED(LEDIP,LEDHOST); //初始化ＬＥＤ的ｕｄｐ通信

    pthread_t pthread_LED;
    pthread_create(&pthread_LED,NULL,(void *)write_LED,NULL);
    pthread_detach(pthread_LED);
    
    while(1)
    {
    /*读取ｒｅｔｒｉｅｖｅ的数据*/
        if(0 > Retriever_data_processing_function(Retriever_fd,pup_data,pup,UDP_fd))
        {
            printf("数据出错\n");
            continue;
        }

        //加一个判断的标值，判断是否可以将传感器的数据写入ＬＥＤ的缓冲区
            if(strcmp(pup_data[0].pup_id,"20180063") == 0)
            {
                strcpy(led_data[0].id,pup_data[0].pup_id);
                printf("a:%s\n",led_data[0].id);
                strcpy(led_data[0].time,pup_data[0].time);

                ren = (atof(pup_data[0].pup_A_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[0].A_temp,"%2.1f",ren);
                
        
                ren = atof(pup_data[0].pup_A_humidity);
                sprintf(led_data[0].A_homidity,"%2.1f",ren);
            

                ren = atof(pup_data[0].pup_A_EC);
                sprintf(led_data[0].A_EC,"%1.2f",ren);

                ren = (atof(pup_data[0].pup_B_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[0].B_temp,"%2.1f",ren);

                ren = atof(pup_data[0].pup_B_humidity);
                sprintf(led_data[0].B_homidity,"%2.1f",ren);

                ren = atof(pup_data[0].pup_B_EC);
                sprintf(led_data[0].B_EC,"%1.2f",ren);
           
                strcpy(led_data[1].id,pup_data[1].pup_id);
                strcpy(led_data[1].time,pup_data[1].time);

                ren = (atof(pup_data[1].pup_A_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[1].A_temp,"%2.1f",ren);
            
                ren = atof(pup_data[1].pup_A_humidity);
                sprintf(led_data[1].A_homidity,"%2.1f",ren);

                ren = atof(pup_data[1].pup_A_EC);
                sprintf(led_data[1].A_EC,"%1.2f",ren);

                ren = (atof(pup_data[1].pup_B_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[1].B_temp,"%2.1f",ren);

                ren = atof(pup_data[1].pup_B_humidity);
                sprintf(led_data[1].B_homidity,"%2.1f",ren);
                
                ren = atof(pup_data[1].pup_B_EC);
                sprintf(led_data[1].B_EC,"%1.2f",ren);
                
                
                
            }else{
                strcpy(led_data[1].id,pup_data[0].pup_id);
                printf("b:%s\n",led_data[1].id);
                strcpy(led_data[1].time,pup_data[0].time);
                
                ren = (atof(pup_data[0].pup_A_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[1].A_temp,"%2.1f",ren);
                printf("A:%s\n",led_data[1].A_temp);
        
                ren = atof(pup_data[0].pup_A_humidity);
                sprintf(led_data[1].A_homidity,"%2.1f",ren);

                ren = (atof(pup_data[0].pup_A_EC));
                sprintf(led_data[1].A_EC,"%1.2f",ren);

                ren = (atof(pup_data[0].pup_B_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[1].B_temp,"%2.1f",ren);

                ren = atof(pup_data[0].pup_B_humidity);
                sprintf(led_data[1].B_homidity,"%2.1f",ren);

                ren = atof(pup_data[0].pup_B_EC);
                sprintf(led_data[1].B_EC,"%1.2f",ren);
            
                strcpy(led_data[0].id,pup_data[1].pup_id);
                
                strcpy(led_data[0].time,pup_data[1].time);
                ren = (atof(pup_data[1].pup_A_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[0].A_temp,"%2.1f",ren);
            
                ren = atof(pup_data[1].pup_A_humidity);
                sprintf(led_data[0].A_homidity,"%2.1f",ren);
                

                ren = atof(pup_data[1].pup_A_EC);
                sprintf(led_data[0].A_EC,"%1.2f",ren);

                ren = (atof(pup_data[1].pup_B_temp) - 32.0) * 5.0 / 9.0;
                sprintf(led_data[0].B_temp,"%2.1f",ren);

                ren = atof(pup_data[1].pup_B_humidity);
                sprintf(led_data[0].B_homidity,"%2.1f",ren);
                

                ren = atof(pup_data[1].pup_B_EC);
                sprintf(led_data[0].B_EC,"%1.2f",ren);
                
            }
        
        printf("1:%s\n",led_data[0].A_temp);
        printf("2:%s\n",led_data[0].A_homidity);
        printf("3:%s\n",led_data[0].A_EC);
        printf("4:%s\n",led_data[0].B_temp);
        printf("5:%s\n",led_data[0].B_homidity);
        printf("6:%s\n",led_data[0].B_EC);
        printf("7:%s\n",led_data[1].A_temp);
        printf("8:%s\n",led_data[1].A_homidity);
        printf("9:%s\n",led_data[1].A_EC);
        printf("A:%s\n",led_data[1].B_temp);
        printf("B:%s\n",led_data[1].B_homidity);
        printf("C:%s\n",led_data[1].B_EC);

    }
    closeRetriever(Retriever_fd);

    return 0;

    
}


