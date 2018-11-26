/*************************************************************************
	> File Name: LED.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月22日 星期六 11时08分13秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<strings.h>
#include<arpa/inet.h>
#include<unistd.h>

#include"LED.h"


/*********************************************
 * 初始化ＬＥＤ显示屏
 * 参数１：ＬＥＤ显示屏的ｉｐ地址
 * 参数２：ＬＥＤ显示屏的端口
 * 返回值：ＬＥＤ显示屏的ＴＣＰ套接字
 * ******************************************/
int initLED(char *ip,int port)
{
    int sockfd; 
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    switch_1 = 1;
    return sockfd;
}


/*********************************************
 * 向ＬＥＤ显示屏发送数据的线程
 * 无参数
 * 无返回值
 * ******************************************/
void *write_LED(void)
{
    
    char data_A[1024] = {"!#001%1West  CEC 8.55mol/kgItem      Upper DeepTemp(C)   ----  ----VWC(%)    ----  ----EC(mS/cm) ----  ----    ----------------$$"};
    char data_B[1024] = {"!#001%1East  CEC 8.73mol/kgItem      Upper DeepTemp(C)   ----  ----VWC(%)    ----  ----EC(mS/cm) ----  ----    ----------------$$"};
    int ren = 0;
    int sun = 0;
//    double su = 32.7;
    printf("%s\n",data_A); 
    printf("%s\n",data_B);
    
    while(1)
    {  
            if(strcmp(led_data[0].id,"20180063") == 0)
            {
                memcpy(data_A+111,led_data[0].time,strlen(led_data[0].time)); 
                memcpy(data_A+57,led_data[0].A_temp,strlen(led_data[0].A_temp));
                if(strlen(led_data[0].A_temp) == 3)
                    data_A[60] = '0';      
                memcpy(data_A+63,led_data[0].B_temp,strlen(led_data[0].B_temp));
                if(strlen(led_data[0].B_temp) == 3)
                    data_A[66] = '0';
                memcpy(data_A+77,led_data[0].A_homidity,strlen(led_data[0].A_homidity));
                if(strlen(led_data[0].A_homidity) == 3)
                    data_A[80] = '0';
                memcpy(data_A+83,led_data[0].B_homidity,strlen(led_data[0].B_homidity));
                if(strlen(led_data[0].B_homidity) == 3)
                    data_A[86] = '0';
                memcpy(data_A+97,led_data[0].A_EC,strlen(led_data[0].A_EC));
                if(strlen(led_data[0].A_EC) == 3)
                    data_A[100] = '0';
                memcpy(data_A+103,led_data[0].B_EC,strlen(led_data[0].B_EC));
                if(strlen(led_data[0].B_EC) == 3)
                    data_A[106] = '0';
                memcpy(data_B+57,led_data[1].A_temp,strlen(led_data[1].A_temp));
                if(strlen(led_data[1].A_temp) == 3)
                    data_B[60] = '0';
                memcpy(data_B+63,led_data[1].B_temp,strlen(led_data[1].B_temp));
                if(strlen(led_data[1].B_temp) == 3)
                    data_B[66] = '0';
                memcpy(data_B+77,led_data[1].A_homidity,strlen(led_data[1].A_homidity));
                if(strlen(led_data[1].A_homidity) == 3)
                    data_B[80] = '0';
                memcpy(data_B+83,led_data[1].B_homidity,strlen(led_data[1].B_homidity));
                if(strlen(led_data[1].B_homidity) == 3)
                    data_B[86] = '0';
                memcpy(data_B+97,led_data[1].A_EC,strlen(led_data[1].A_EC));
                if(strlen(led_data[1].A_EC) == 3)
                    data_B[100] = '0';
                memcpy(data_B+103,led_data[1].B_EC,strlen(led_data[1].B_EC));
                if(strlen(led_data[1].B_EC) == 3)
                    data_B[106] = '0';
                memcpy(data_B+111,led_data[1].time,strlen(led_data[1].time)); 
                printf("Wast!!!!!!!!!!\n");
            }else 
            {
                memcpy(data_B+57,led_data[0].A_temp,strlen(led_data[0].A_temp));
                if(strlen(led_data[0].A_temp) == 3)
                    data_B[60] = '0';
                memcpy(data_B+63,led_data[0].B_temp,strlen(led_data[0].B_temp));
                if(strlen(led_data[0].B_temp) == 3)
                    data_B[66] = '0';
                memcpy(data_B+77,led_data[0].A_homidity,strlen(led_data[0].A_homidity));
                if(strlen(led_data[0].A_homidity) == 3)
                    data_B[80] = '0';
                memcpy(data_B+83,led_data[0].B_homidity,strlen(led_data[0].B_homidity));
                if(strlen(led_data[0].B_homidity) == 3)
                    data_B[86] = '0';
                memcpy(data_B+97,led_data[0].A_EC,strlen(led_data[0].A_EC));
                if(strlen(led_data[0].A_EC) == 3)
                    data_B[100] = '0';
                memcpy(data_B+103,led_data[0].B_EC,strlen(led_data[0].B_EC));
                if(strlen(led_data[0].B_EC) == 3)
                    data_B[106] = '0';
                memcpy(data_B+111,led_data[0].time,strlen(led_data[0].time)); 
                memcpy(data_A+57,led_data[1].A_temp,strlen(led_data[1].A_temp));
                if(strlen(led_data[1].A_temp) == 3)
                    data_A[60] = '0';
                memcpy(data_A+63,led_data[1].B_temp,strlen(led_data[1].B_temp));
                if(strlen(led_data[1].B_temp) == 3)
                    data_A[66] = '0';
                memcpy(data_A+77,led_data[1].A_homidity,strlen(led_data[1].A_homidity));
                if(strlen(led_data[1].A_homidity) == 3)
                    data_A[80] = '0';
                memcpy(data_A+83,led_data[1].B_homidity,strlen(led_data[1].B_homidity));
                if(strlen(led_data[1].B_homidity) == 3)
                    data_A[86] = '0';
                memcpy(data_A+97,led_data[1].A_EC,strlen(led_data[1].A_EC));
                if(strlen(led_data[1].A_EC) == 3)
                    data_A[100] = '0';
                memcpy(data_A+103,led_data[1].B_EC,strlen(led_data[1].B_EC));
                if(strlen(led_data[1].B_EC) == 3)
                    data_A[106] = '0';
                memcpy(data_A+111,led_data[1].time,strlen(led_data[1].time)); 
                
            }
//            printf("id2: %s\n",led_data[0].id);
        
        sendto(LED_fd,&data_A,strlen(data_A),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
            printf("%s\n",data_A);
            sleep(15);
            printf("%s\n",data_B);
        sendto(LED_fd,&data_B,strlen(data_A),0,(struct sockaddr*)&server_addr,sizeof(server_addr));
            sleep(15);
    }
}
